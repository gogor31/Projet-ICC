// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : main.cc
// Description : Point d'entrée principal de l'application Brick Breaker.
//               Initialise le contexte Gtk::Application et lance la GUI.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526)
// Date : Mai 2026
// ============================================================================

#include <iostream>
#include "game.h"
#include "gui.h"

int main(int argc, char* argv[]) {
    
    setenv("GSK_RENDERER", "cairo", 1);
    // Création de l'application GTKMM avec un identifiant unique
    auto app = Gtk::Application::create("org.gtkmm.brickbreaker");
    
    // Configure l'application pour lui permettre de traiter des arguments de fichiers en ligne de commande
    app->set_flags(Gio::Application::Flags::HANDLES_OPEN);

    // Récupération optionnelle du nom de fichier d'arène passé en paramètre
    std::string filename;
    if (argc == 2) {
        filename = argv[1]; 
    } else {
        filename = "";  
    }

    // Instanciation de la fenêtre principale (My_window) et lancement de la boucle événementielle GTK
    return app->make_window_and_run<My_window>(1, argv, filename);
}