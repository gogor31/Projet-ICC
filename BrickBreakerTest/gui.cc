// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : gui.cc
// Description : Implémentation des mécanismes d'interaction, mapping des 
//               coordonnées pixel/modèle et boucle événementielle synchrone.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#include <filesystem>
#include <iostream>
#include "constants.h"
#include "graphic_gui.h"
#include "gui.h"
#include "game.h"

using namespace std;

// ==========================================
// CONSTANTES ET ENUMS LOCALES
// ==========================================

enum Response
{
    CANCEL,
    OPEN_FILE,
    SAVE_FILE
};

enum Buttons
{
    EXIT,
    OPEN,
    SAVE,
    RESTART,
    START,
    STEP
};

constexpr unsigned drawing_size(500); // Dimension carrée par défaut de la DrawingArea

// ==========================================
// CONSTRUCTEUR ET CONFIGURATION GLOBALE
// ==========================================

My_window::My_window(string file_name)
    : current_file(""), main_box(Gtk::Orientation::HORIZONTAL),
      panel_box(Gtk::Orientation::VERTICAL), command_box(Gtk::Orientation::VERTICAL), 
      info_grid(), info_frame("Infos :"), drawing(),
      buttons({Gtk::Button("exit"), Gtk::Button("open"), Gtk::Button("save"),
               Gtk::Button("restart"), Gtk::Button("start"), Gtk::Button("step")}),
      info_text({Gtk::Label("score:"), Gtk::Label("lives:"),
                 Gtk::Label("bricks:"), Gtk::Label("balls:")}),
      info_value(),
      loop_activated(false)
{
    set_title("Brick Breaker");
    set_child(main_box);
    main_box.append(panel_box);
    main_box.append(drawing);
    panel_box.append(command_box);
    panel_box.append(info_frame);

    // Initialisation séquentielle de tous les sous-systèmes UI
    set_commands();
    set_key_controller();
    set_mouse_controller();
    set_infos();
    set_drawing();

    if (!file_name.empty()) {
        if(game.load_file(file_name)) {
            current_file = file_name;
        }
    }
    update_infos();
}

void My_window::set_commands()
{
    for (auto &button : buttons)
    {
        command_box.append(button);
        command_box.set_size_request(120, -1);
        button.set_margin(1);
    }

    // Association de chaque signal de clic vers sa méthode déléguée
    buttons[EXIT].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::exit_clicked));
    buttons[OPEN].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::open_clicked));
    buttons[SAVE].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::save_clicked));
    buttons[RESTART].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::restart_clicked));
    buttons[START].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::start_clicked));
    buttons[STEP].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::step_clicked));
}

// ==========================================
// GESTION DES CLICKS SUR BOUTONS
// ==========================================

void My_window::exit_clicked()
{
    hide(); // Provoque la fermeture de la fenêtre et l'arrêt propre de l'application
}

void My_window::open_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::OPEN);
    set_dialog(dialog);
}

void My_window::save_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::SAVE);
    set_dialog(dialog);
}

void My_window::restart_clicked()
{
    if (!current_file.empty()) {
        game.load_file(current_file);
        drawing.queue_draw(); // Force l'invalidation graphique pour rafraîchir l'écran
        update_infos();
    }
}

void My_window::start_clicked()
{
    if (loop_activated)
    {
        // Passage en mode PAUSE : Déconnexion sûre du rafraîchissement périodique
        loop_conn.disconnect();
        loop_activated = false;
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[STEP].set_sensitive(true);
    }
    else 
    {
        // Passage en mode JEU ACTIF : Armement du timer
        loop_conn =
            Glib::signal_timeout().connect(sigc::mem_fun(*this, &My_window::loop), dt);
        loop_activated = true;
        buttons[EXIT].set_sensitive(false);
        buttons[OPEN].set_sensitive(false);
        buttons[SAVE].set_sensitive(false);
        buttons[RESTART].set_sensitive(false);
        buttons[START].set_label("stop");
        buttons[STEP].set_sensitive(false);
    }
}

void My_window::step_clicked()
{
    game.update(); 
    drawing.queue_draw();
    update_infos();
}

// ==========================================
// GESTION DES ENTRÉES CLAVIER ET SOURIS
// ==========================================

void My_window::set_key_controller()
{
    auto contr = Gtk::EventControllerKey::create();
    contr->signal_key_pressed().connect(sigc::mem_fun(*this, &My_window::key_pressed),
                                        false);
    add_controller(contr);
}

bool My_window::key_pressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
    (void)keycode, (void)state;

    // Raccourcis clavier calqués sur le comportement des boutons
    switch (keyval)
    {
    case '1':
        step_clicked();
        return true;
    case 's':
        start_clicked(); 
        return true;
    case 'r':
        restart_clicked(); 
        return true;
    default:
        break;
    }
    return false;
}

void My_window::set_mouse_controller()
{
    auto left_click = Gtk::GestureClick::create();
    auto move = Gtk::EventControllerMotion::create();

    left_click->set_button(GDK_BUTTON_PRIMARY); // Capture du clic gauche

    left_click->signal_pressed().connect(
        sigc::mem_fun(*this, &My_window::on_drawing_left_click));
    move->signal_motion().connect(sigc::mem_fun(*this, &My_window::on_drawing_move));

    drawing.add_controller(left_click);
    drawing.add_controller(move);
}

void My_window::on_drawing_left_click(int n_press, double x, double y)
{
    (void)n_press; (void)x; (void)y;
    
    // Autorise le relancement d'une balle uniquement si aucune n'est en cours
    if (game.get_nb_balls() == 0 && game.get_lives() > 0) {
        game.spawn_ball(); 
        
        drawing.queue_draw();
        update_infos();
    }
}

void My_window::on_drawing_move(double x, double y)
{
    (void)y;
    
    if (!game.get_paddle().is_active()) {
        return; 
    }

    int width = drawing.get_width();
    int height = drawing.get_height();
    double side = min(width, height);
    
    // Calcul de l'offset pour conserver le ratio carré au centre du widget étendu
    double x_offset = (width - side) * 0.5;
    // Mapping direct pixel écran -> coordonnée normalisée de l'arène logique
    double model_x = (x - x_offset) * (arena_size / side);

    game.update_paddle_pos(model_x);
}

// ==========================================
// BOUCLE DE RAFRAÎCHISSEMENT AUTOMATIQUE
// ==========================================

bool My_window::loop()
{
    if (loop_activated)
    {
        // Arrêt automatique si la partie se termine
        if (game.is_over() && game.get_lives() <= 0) {
            start_clicked();
            return false; // Stoppe définitivement les itérations de signal_timeout
        }

        game.update();
        drawing.queue_draw();
        update_infos(); 
        return true; // Demande à signal_timeout de reprogrammer le prochain tick
    }
    return false;
}

// ==========================================
// COMPTEURS ET MISES À JOUR NUMÉRIQUES
// ==========================================

void My_window::set_infos()
{
    info_frame.set_child(info_grid);
    info_grid.set_column_homogeneous(true);
    for (size_t i(0); i < info_text.size(); ++i)
    {
        info_grid.attach(info_text[i], 0, i, 1, 1);
        info_grid.attach(info_value[i], 1, i, 1, 1);
        info_text[i].set_halign(Gtk::Align::START);
        info_value[i].set_halign(Gtk::Align::END);
        info_text[i].set_margin(3);
        info_value[i].set_margin(3);
    }
}

void My_window::update_infos()
{
    // Mise à jour du Score (Index 0)
    int current_score = game.get_score();
    if (current_score != last_displayed_score) {
        last_displayed_score = current_score;
        info_value[0].set_text(std::to_string(current_score));
    }

    // Mise à jour des Vies (Index 1)
    int current_lives = game.get_lives();
    if (current_lives != last_displayed_lives) {
        last_displayed_lives = current_lives;
        info_value[1].set_text(std::to_string(current_lives));
    }

    // Mise à jour des Briques (Index 2)
    int current_bricks = game.get_nb_bricks(); 
    if (current_bricks != last_displayed_bricks) {
        last_displayed_bricks = current_bricks;
        info_value[2].set_text(std::to_string(current_bricks));
    }

    // Mise à jour des Balles (Index 3)
    int current_balls = game.get_nb_balls(); 
    if (current_balls != last_displayed_balls) {
        last_displayed_balls = current_balls;
        info_value[3].set_text(std::to_string(current_balls));
    }
}

// ==========================================
// BOÎTES DE DIALOGUE
// ==========================================

void My_window::set_dialog(Gtk::FileChooserDialog *dialog)
{
    dialog->set_modal(true);
    dialog->set_transient_for(*this);
    dialog->set_select_multiple(false);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &My_window::dialog_response), dialog));

    dialog->add_button("_Cancel", CANCEL);

    switch (dialog->get_action())
    {
    case Gtk::FileChooserDialog::Action::OPEN:
        dialog->add_button("_Open", OPEN_FILE);
        dialog->set_default_response(OPEN_FILE);
        break;
    case Gtk::FileChooserDialog::Action::SAVE:
        dialog->add_button("_Save", SAVE_FILE);
        dialog->set_default_response(SAVE_FILE);
        break;
    default:
        break;
    }

    // Configuration des filtres d'extension de fichier autorisés (.txt)
    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_pattern("*.txt");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}

void My_window::dialog_response(int response, Gtk::FileChooserDialog *dialog)
{
    filesystem::path file_name = "";
    if (dialog->get_file())
    {
        file_name = dialog->get_file()->get_path();
    }

    if (file_name.empty() && response != CANCEL) {
        dialog->hide();
        delete dialog;
        return;
    }

    switch (response)
    {
    case CANCEL:
        dialog->hide();
        break;

    case OPEN_FILE:
        if (file_name.extension() == ".txt"){
            cout << "open file: [" << file_name.filename().string() << "]" << endl;
            current_file = file_name.string();
            if (!game.load_file(current_file)) {
                current_file = ""; // Invalidation si le contenu logique viole les règles
            }
            drawing.queue_draw();    
            update_infos();          
            dialog->hide();
            delete dialog;
        }
        break;

    case SAVE_FILE:
        if (file_name.extension() != ".txt") {
            file_name += ".txt"; // Forçage du suffixe .txt si omis par l'étudiant/utilisateur
        }

        cout << "save file: [" << file_name.filename().string() << "]" << endl;
        game.save(file_name.string());
        dialog->hide();
        delete dialog;
        break;
    default:
        dialog->hide();
        delete dialog;
        break;
    }
}

// ==========================================
// RENDU GRAPHIQUE (CAIRO)
// ==========================================

void My_window::set_drawing()
{
    drawing.set_content_width(drawing_size);
    drawing.set_content_height(drawing_size);
    drawing.set_expand();
    drawing.set_draw_func(sigc::mem_fun(*this, &My_window::on_draw));
}

void My_window::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    graphic_set_context(cr); // Injection du contexte courant dans le module graphique global
    double side(min(width, height));
    
    // Application de la matrice de transformation géométrique
    cr->translate((width - side) * 0.5, (height + side) * 0.5);
    cr->scale(side / (arena_size), -side / (arena_size)); // Y inversé
    
    game.draw();
}