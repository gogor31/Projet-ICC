/*
 * Fichier: main.cc
 * Auteur:  Ilhan Salvatore Legio
 */

#include <iostream>
#include "game.h"
#include "gui.h"


/* [V1]
int main(int argc, char* argv[]) {
    // Vérification de la présence du fichier de configuration en argument
    if (argc != 2) {
        return 1;
    }

    // Utilisation du namespace pour instancier le moteur de jeu (O31.2)
    Game game;

    // Tentative de chargement et de validation du fichier d'entrée
    if (!(game.load_file(argv[1]))) {
    return 1;}

    return 0;
}
*/

int main(int argc, char* argv[]) {
    Game game;

    //lancement du jeu sans fichier test, ->[?] avec fichier init_state ou une instance game init_state
    if (argc == 1) {
        return 0;
    }

    //lancement du jeu avec fichier test, comme rendu1
    if  (argc == 2){
        // Tentative de chargement et de validation du fichier d'entrée
        if (!game.load_file(argv[1])) {return 1;}
        return 0;
    }
    //les autres valeurs possibles pour argc sont invalides
    return 1;

}