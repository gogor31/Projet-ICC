#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <fstream> 
#include <sstream>  
#include "ball.h"
#include "brick.h"
#include "paddle.h"


class Game {
public:
    // Constructeur par défaut initialisant le score et les vies à 0
    Game();

    // Destructeur pour libérer la mémoire
    ~Game();

    // Lecture du fichier et initialisation
    bool load_file(const std::string& filename);
    
    // Nettoie les entités (important pour la gestion de mémoire des briques)
    void clear();


    
private:
    int score_ = 0;
    int lives_ = 0;
    
    Paddle paddle_;
    std::vector<Ball> balls_;
    std::vector<Brick*> bricks_; //? Utilisation de pointeurs pour le polymorphisme des briques
    
    // Déclarations OBLIGATOIRES pour que le .cc compile
    bool read_header(std::ifstream& file);
    bool read_paddle(std::ifstream& file);
    bool read_bricks(std::ifstream& file);
    bool read_balls(std::ifstream& file);
    
    // Pour la création des briques
    bool create_brick(int type, double x, double y, double s, std::stringstream& ss);

    bool validate_initial_state();
    bool check_bricks_intersections();
    bool check_paddle_brick_intersections();
    bool check_balls_intersections();
    bool check_ball_brick_intersections();
    bool check_ball_paddle_intersections();
};

#endif 