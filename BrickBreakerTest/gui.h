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

    My_window(std::string file_name);

    virtual ~My_window() = default; 

private:
    // ==========================================
    // STRUCTURES DE CONTRÔLE INTERNE
    // ==========================================
    
    Game game;
    std::string current_file;

    // Interface widgets
    Gtk::Box main_box, panel_box, command_box;
    Gtk::Grid info_grid;
    Gtk::Frame info_frame;
    Gtk::DrawingArea drawing;

    // Boutons et labels
    std::array<Gtk::Button, 6> buttons;
    std::array<Gtk::Label, 4> info_text, info_value;

    // Boucle de rafraîchissement
    bool loop_activated;
    sigc::connection loop_conn;
    bool loop();

    // ==========================================
    // CONFIGURATION DES CONTÔLEURS (INIT)
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

    void update_infos();

    // ==========================================
    // BOÎTES DE DIALOGUE (FILE CHOOSER)
    // ==========================================

    void set_dialog(Gtk::FileChooserDialog *dialog);
    void dialog_response(int response, Gtk::FileChooserDialog *dialog);

    // ==========================================
    // RENDU GRAPHIQUE (CAIRO)
    // ==========================================

    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
};

#endif