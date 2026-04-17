#include <iostream>
#include "game.h"
#include "gui.h"


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.brickbreaker");
    
    app->set_flags(Gio::Application::Flags::HANDLES_OPEN);

    Game game;
    std::string filename = (argc == 2) ? argv[1] : "";

    //lancement du jeu sans fichier test
    if (argc == 1) {
        return app->make_window_and_run<My_window>(argc, argv, "no_file");
    }

    //lancement du jeu avec fichier test, comme rendu1
    if (argc == 2) { 
        if (!game.load_file(argv[1])) {
            return 1;  // Echec de chargement du fichier
        } else {
            return app->make_window_and_run<My_window>(argc, argv, filename);
        }
    }
}