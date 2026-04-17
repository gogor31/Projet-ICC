#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <memory>
#include "ball.h"
#include "brick.h"
#include "paddle.h"


class Game {
public:
    Game();
    ~Game(); // Libère la mémoire via clear()

    bool load_file(const std::string& filename);
    void clear();
    
    //je met ça ici temporairement pour pouvoir compiler
    void save(const std::string& filename) const; //prototype, implémentation incomplète
    bool is_over() const; //prototype, implémentation incomplète
    void draw() const; //prototype, implémentation incomplète
    void update_paddle_pos(double target_x); //prototype, implémentation incomplète
    void spawn_ball();
    int get_score() const { return score_; }
    int get_lives() const { return lives_; }
    int get_nb_bricks() const { return bricks_.size(); }
    int get_nb_balls() const { return balls_.size(); }
    Paddle get_paddle() const { return paddle_; }
    


private:
    int score_ = 0; 
    int lives_ = 0;
    
    Paddle paddle_;
    std::vector<Ball> balls_;
    std::vector<std::unique_ptr<Brick>> bricks_; 

    // Méthodes de lecture internes (O14)
    bool read_header(std::ifstream& file);
    bool read_paddle(std::ifstream& file);
    bool read_bricks(std::ifstream& file);
    bool read_balls(std::ifstream& file);
    
    bool create_brick(int type, double x, double y, double s, 
                        std::stringstream& ss);

    // Validations géométriques initiales (P2)
    bool validate_initial_state();
    bool check_bricks_intersections();
    bool check_paddle_brick_intersections();
    bool check_balls_intersections();
    bool check_ball_brick_intersections();
    bool check_ball_paddle_intersections();
};


#endif