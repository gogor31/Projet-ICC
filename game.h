#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "ball.h"
#include "brick.h"
#include "paddle.h"


using namespace std; //a supprimer pour ne pas se melanger 

class Game {
public:
    // Constructeur par défaut initialisant le score et les vies à 0
    Game();

    // Destructeur pour libérer la mémoire
    ~Game();

    // Lecture du fichier et initialisation
    bool load_file(const string& filename);
    
    // Nettoie les entités (important pour la gestion de mémoire des briques)
    void clear();


    
private:
    int score_ = 0;
    int lives_ = 0;
    
    Paddle paddle_;
    vector<Ball> balls_;
    vector<Brick*> bricks_; // Utilisation de pointeurs pour le polymorphisme des briques
    
    // Méthode interne pour valider les contraintes de l'énoncé (collisions, etc.)
    bool validate_constraints();
};

#endif 