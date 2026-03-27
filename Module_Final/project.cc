/*
 * Fichier: main.cc
 * Auteur:  Ilhan Salvatore Legio
 */

#include <iostream>
#include "game.h"

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