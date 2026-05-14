#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "ball.h"
#include "brick.h"
#include "paddle.h"

// États possibles de la partie 
enum Status { ONGOING, WON, LOST };

class Game {
public:
    Game();
    ~Game();

    // --- PHASE 1 : INITIALISATION ET ENTRÉES/SORTIES ---
    // Initialise l'état du jeu à partir d'un fichier texte 
    bool load_file(const std::string& filename);
    
    // Réinitialise toutes les structures de données
    void clear();
    
    // Sauvegarde la configuration actuelle dans un fichier
    void save(const std::string& filename) const;

    // --- PHASE 2 : BOUCLE DE SIMULATION  ---
    // Orchestre le déplacement et les collisions
    void update();
    
    // Vérifie et met à jour le statut de la partie
    void check_game_status();

    // --- PHASE 3 : ACTIONS ET INTERACTIONS ---
    // Met à jour la position cible de la raquette via la souris 
    void update_paddle_pos(double target_x); 
    
    // Génère une nouvelle balle si lives > 0 
    void spawn_ball();
    
    // Dessine l'ensemble des entités dans le canevas
    void draw() const; 

    // --- PHASE 4 : ACCESSEURS ---
    bool is_over() const;
    int get_score() const { return score_; }
    int get_lives() const { return lives_; }
    int get_nb_bricks() const { return bricks_.size(); }
    int get_nb_balls() const { return balls_.size(); }
    Paddle get_paddle() const { return paddle_; }

private:
    // Données membres
    int score_ = 0; 
    int lives_ = 0;
    Paddle paddle_;
    std::vector<Ball> balls_;
    std::vector<std::unique_ptr<Brick>> bricks_; 
    Status status_ = ONGOING;

    // --- MÉTHODES PRIVÉES : LECTURE ET PARSING ---
    bool read_header(std::ifstream& file);
    bool read_paddle(std::ifstream& file);
    bool read_bricks(std::ifstream& file);
    bool read_balls(std::ifstream& file);

    // Factory pour instancier le bon type de brique
    bool create_brick(int type, double x, double y, double s, std::stringstream& ss);

    // --- MÉTHODES PRIVÉES : VALIDATIONS ---
    // Vérifie l'absence de collisions initiales
    bool validate_initial_state() const;
    bool check_bricks_intersections() const;
    bool check_paddle_brick_intersections() const;
    bool check_balls_intersections() const;
    bool check_ball_brick_intersections() const;
    bool check_ball_paddle_intersections() const;

    // --- MÉTHODES PRIVÉES : PHYSIQUE ET COLLISIONS ---
    // Gestion des rebonds et impulsions 
    void handle_ball_ball_collisions();
    void handle_paddle_collision(Ball& ball);
    bool handle_arena_collision(Ball& ball);
    bool handle_bricks_collision(Ball& ball, std::vector<std::unique_ptr<Brick>>& to_add);
    
    // Gère les bonus/malus lors de la casse d'une brique
    void handle_brick_destruction_effects(const Brick& b, 
                                          std::vector<std::unique_ptr<Brick>>& new_bricks);
    
    // Supprime les entités marquées comme "dead"
    void cleanup_dead_objects();
    
    // Utilitaires de création d'entités en cours de jeu
    void add_ball_to_simulation(tools::Point pos, tools::Point velocity);
    void spawn_new_ball(tools::Point pos);
};

#endif