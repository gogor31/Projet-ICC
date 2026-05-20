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

// ==========================================
// ÉNUMÉRATIONS ET ÉTATS DU JEU
// ==========================================

enum Status { ONGOING, WON, LOST };

class Game {
public:
    // ==========================================
    // CONSTRUCTEUR ET DESTRUCTEUR
    // ==========================================

    Game();
    ~Game();

    // ==========================================
    // INITIALISATION ET ENTRÉES/SORTIES
    // ==========================================

    bool load_file(const std::string& filename);
    
    void clear();
    
    void save(const std::string& filename) const;

    // ==========================================
    // BOUCLE DE SIMULATION ET STATUT
    // ==========================================

    void update();
    
    void check_game_status();

    // ==========================================
    // ACTIONS ET INTERACTIONS SOURIS
    // ==========================================

    void update_paddle_pos(double target_x); 
    
    void spawn_ball();

    // ==========================================
    // ACCESSEURS NUMÉRIQUES ET CONSTANTS
    // ==========================================

    bool is_over() const;
    int get_score() const { return score_; }
    int get_lives() const { return lives_; }
    int get_nb_bricks() const { return bricks_.size(); }
    int get_nb_balls() const { return balls_.size(); }
    Paddle get_paddle() const { return paddle_; }

    // ==========================================
    // RENDU GRAPHIQUE
    // ==========================================

    void draw() const; 

private:
    // ==========================================
    // ATTRIBUTS ET DONNÉES MEMBRES
    // ==========================================

    int score_ = 0; 
    int lives_ = 0;
    Paddle paddle_;
    std::vector<Ball> balls_;
    std::vector<std::unique_ptr<Brick>> bricks_; 
    Status status_ = ONGOING;
    double target_paddle_x_ = 50.0;
    std::vector<Ball> balls_to_add_;

    // ==========================================
    // LECTURE ET PARSING
    // ==========================================

    bool read_header(std::ifstream& file);
    bool read_paddle(std::ifstream& file);
    bool read_bricks(std::ifstream& file);
    bool read_balls(std::ifstream& file);
    bool create_brick(int type, double x, double y, double s, std::stringstream& ss);

    // ==========================================
    // VALIDATIONS INITIALES
    // ==========================================

    bool validate_initial_state() const;
    bool check_bricks_intersections() const;
    bool check_paddle_brick_intersections() const;
    bool check_balls_intersections() const;
    bool check_ball_brick_intersections() const;
    bool check_ball_paddle_intersections() const;

    // ==========================================
    // PHYSIQUE ET COLLISIONS
    // ==========================================

    void handle_ball_ball_collisions();
    bool handle_paddle_collision(Ball& ball);
    bool handle_arena_collision(Ball& ball);
    bool handle_bricks_collision(Ball& ball, std::vector<std::unique_ptr<Brick>>& to_add);
    void handle_brick_destruction_effects(const Brick& b, std::vector<std::unique_ptr<Brick>>& new_bricks);
    void cleanup_dead_objects();
    
    // ==========================================
    // UTILITAIRES DE CRÉATION DE BALLES
    // ==========================================

    void add_ball_to_simulation(tools::Point pos, tools::Point velocity);
    void spawn_new_ball(tools::Point pos);
};

#endif