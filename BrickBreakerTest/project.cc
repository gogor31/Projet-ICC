#include <iostream>
#include "game.h"
#include "message.h" 

int main(int argc, char* argv[]) {
    // Vérification des arguments en ligne de commande 
    // argc doit valoir 2 (le nom de l'exécutable + le nom du fichier texte)
    if (argc != 2) {
        return 1;
    }

    // Instanciation du sous-système Modèle [cite: 359]
    Game game;

    // Lancement de la lecture du fichier
    // Ta méthode load_file se charge d'afficher les erreurs spécifiques si elle échoue
    if (game.load_file(argv[1])) {
        // Le programme s'arrête après la lecture s'il n'y a aucune erreur, 
        // en affichant le message de succès 
        std::cout << message::success();
    } else {
        // Si load_file retourne false, une erreur a été détectée.
        // Le programme s'arrête immédiatement en retournant un code d'erreur 
        return 1;
    }

    return 0; // Fin normale du programme
}