#include <iostream>
#include "game.h"
#include "gui.h"


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.brickbreaker");
    
    app->set_flags(Gio::Application::Flags::HANDLES_OPEN);
    std::string filename = (argc == 2) ? argv[1] : "";

    return app->make_window_and_run<My_window>(1, argv, filename);
}