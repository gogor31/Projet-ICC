// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : gui.h
// Description : Définition de la classe My_window gérant l'interface graphique 
//               Gtkmm (widgets, agencement, événements clavier/souris/timers).
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#ifndef GUI_H
#define GUI_H

#include <array>
#include <gtkmm.h>
#include <string>
#include <memory> 
#include "game.h"

class My_window : public Gtk::Window
{
public:
    // ==========================================
    // CONSTRUCTEUR ET DESTRUCTEUR
    // ==========================================

    // Initialise la fenêtre principale et tente de charger le fichier passé en paramètre
    My_window(std::string file_name);

    virtual ~My_window() = default; 

private:
    // ==========================================
    // STRUCTURES DE CONTRÔLE INTERNE
    // ==========================================
    
    Game game;               
    std::string current_file;  

    // Conteneurs d'agencement structurels
    Gtk::Box main_box, panel_box, command_box;
    Gtk::Grid info_grid;
    Gtk::Frame info_frame;
    Gtk::DrawingArea drawing;   // Zone dédiée au dessin vectoriel Cairo

    // Widgets interactifs et textuels
    std::array<Gtk::Button, 6> buttons;
    std::array<Gtk::Label, 4> info_text, info_value;

    // Gestion du rafraîchissement périodique
    bool loop_activated;
    sigc::connection loop_conn;
    bool loop();

    // ==========================================
    // CONFIGURATION DES CONTÔLEURS
    // ==========================================

    void set_commands();
    void set_infos();
    void set_key_controller();
    void set_drawing();
    void set_mouse_controller();

    // ==========================================
    // GESTION DES CLICKS SUR BOUTONS
    // ==========================================

    void exit_clicked();
    void open_clicked();
    void save_clicked();
    void restart_clicked();
    void start_clicked();
    void step_clicked();

    // ==========================================
    // GESTION DES ENTRÉES CLAVIER ET SOURIS
    // ==========================================

    bool key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_drawing_left_click(int n_press, double x, double y);
    void on_drawing_move(double x, double y);

    // ==========================================
    // COMPTEURS ET MISES À JOUR NUMÉRIQUES
    // ==========================================

    // Lit l'état de la simulation pour actualiser les labels textuels (Scores, Vies, etc.)
    void update_infos();
    
    int last_displayed_score = -1; // Initialisé à -1 pour forcer le premier affichage
    int last_displayed_lives = -1;
    int last_displayed_bricks = -1;
    int last_displayed_balls = -1;

    // ==========================================
    // BOÎTES DE DIALOGUE
    // ==========================================

    void set_dialog(Gtk::FileChooserDialog *dialog);
    void dialog_response(int response, Gtk::FileChooserDialog *dialog);

    // ==========================================
    // RENDU GRAPHIQUE (CAIRO)
    // ==========================================

    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
};

#endif