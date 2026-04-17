#ifndef GUI_H
#define GUI_H

#include <array>
#include <gtkmm.h>
#include <string>
#include <memory> //
#include "game.h"


class My_window : public Gtk::Window
{
public:
    My_window(std::string file_name);

    virtual ~My_window() = default; //

private:
    
    Game game;
    std::string current_file;

    //interface
    Gtk::Box main_box, panel_box, command_box;
    Gtk::Grid info_grid;
    Gtk::Frame info_frame;
    Gtk::DrawingArea drawing;

    // Boutons et labels
    std::array<Gtk::Button, 6> buttons;
    std::array<Gtk::Label, 4> info_text, info_value;

    //boucle de rafraichissement
    bool loop_activated;
    sigc::connection loop_conn;
    bool loop();

    //controllers
    void set_commands();
    void set_infos();
    void set_key_controller();
    void set_drawing();
    void set_mouse_controller();

    //boutons
    void exit_clicked();
    void open_clicked();
    void save_clicked();
    void restart_clicked();
    void start_clicked();
    void step_clicked();

    //Clavier et souris
    bool key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_drawing_left_click(int n_press, double x, double y);
    void on_drawing_move(double x, double y);

    //dessin 
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

    //Dialogs
    void set_dialog(Gtk::FileChooserDialog *dialog);
    void dialog_response(int response, Gtk::FileChooserDialog *dialog);

    //MàJ
    void update_infos();

};

#endif