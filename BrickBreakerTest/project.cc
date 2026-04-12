/*
 * Fichier: main.cc
 * Auteur:  Ilhan Salvatore Legio
 */

#include <iostream>
#include "game.h"
#include "gui.h"


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create();

    Game game;
    
    //lancement du jeu sans fichier test
    if (argc == 1) {
        return app->make_window_and_run<My_window>(argc, argv, "temp");
    }

    //lancement du jeu avec fichier test, comme rendu1
    if (argc == 2) { 
        if (!game.load_file(argv[1])) {
            return app->make_window_and_run<My_window>(argc, argv, "temp");
        }
    }
    return 1;
}