#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "game.h"
#include "message.h"
#include "constants.h"


// Lit la prochaine ligne qui n'est ni vide ni un commentaire (D3, P4)
bool get_valid_line(std::ifstream& file, std::string& line) {
    while (std::getline(file, line)) {
        // Suppression des espaces blancs initiaux pour le test (L21)
        size_t first_char = line.find_first_not_of(" \t\r\n");
        
        // On ignore les lignes vides ou commençant par '#' (O14)
        if (first_char != std::string::npos && line[first_char] != '#') {
            return true; 
        }
    }
    return false;
}



// Constructeur : initialisation explicite des membres (L25)
Game::Game() {}

// Destructeur : assure la libération de la mémoire (R1)
Game::~Game() { 
    clear(); 
}

void Game::clear() { 
    bricks_.clear(); 
    balls_.clear();
    score_ = 0;
    lives_ = 0;
    paddle_.set_active(false); // Désactive la raquette par défaut
}

// --- LECTURE DU FICHIER (Parsing) ---

bool Game::load_file(const std::string& filename) {
    clear();
    std::ifstream file(filename);
    if (!file) return false;
    
    bool success = read_header(file) && read_paddle(file) && 
                   read_bricks(file) && read_balls(file) &&
                   validate_initial_state();

    if (!success) {
        clear(); 
        return false;
    }

    paddle_.set_active(true); // Active la raquette après validation réussie
    std::cout << message::success() << std::endl;
    return true; // Affiche le message de succès et retourne true
}

bool Game::read_header(std::ifstream& file) {
    std::string line;
    
    if (!get_valid_line(file, line)) return false;
    std::stringstream ss_score(line);
    if (!(ss_score >> score_)) return false;

    if (!get_valid_line(file, line)) return false;
    std::stringstream ss_lives(line);
    if (!(ss_lives >> lives_)) return false;

    if (score_ < 0) {
        std::cout << message::invalid_score(score_); 
        return false;
    }
    if (lives_ < 0) {
        std::cout << message::invalid_lives(lives_); 
        return false;
    }
    return true;
}

bool Game::read_paddle(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    double x, y, r;
    std::stringstream ss(line);
    if (!(ss >> x >> y >> r)) return false;

    paddle_ = Paddle({{x, y}, r});
    return paddle_.check(); 
}

bool Game::read_bricks(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_bricks;
    if (!(std::stringstream(line) >> nb_bricks)) return false;
    if (nb_bricks < 0) return false; 

    for (int i = 0; i < nb_bricks; ++i) {
        if (!get_valid_line(file, line)) return false;
        
        std::stringstream ss(line);
        int type; double x, y, s; 
        if (!(ss >> type >> x >> y >> s)) return false;

        if (type < 0 || type > 2) {
            std::cout << message::invalid_brick_type(type);
            return false;
        }

        if (!create_brick(type, x, y, s, ss)) return false;
    }
    return true;
}

bool Game::read_balls(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_balls;
    if (!(std::stringstream(line) >> nb_balls)) return false;

    for (int i = 0; i < nb_balls; ++i) {
        if (!get_valid_line(file, line)) return false;
        
        std::stringstream ss(line);
        double x, y, r, dx, dy;
        if (!(ss >> x >> y >> r >> dx >> dy)) return false;

        Ball new_ball({{x, y}, r}, {dx, dy});
        if (!new_ball.check()) return false;

        balls_.push_back(new_ball);
    }
    return true;
}

bool Game::create_brick(int type, double x, double y, double s, 
                        std::stringstream& ss) {
    Brick* new_ptr = nullptr;

    // Factory simple pour le polymorphisme (P2)
    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        new_ptr = new RainbowBrick({{x, y}, s}, hp);
    } else if (type == 1) {
        new_ptr = new BallBrick({{x, y}, s});
    } else if (type == 2) {
        new_ptr = new SplitBrick({{x, y}, s});
    }

    if (new_ptr) {
        // Appel polymorphique de check() (D5)
        if (!new_ptr->check()) {
            delete new_ptr;
            return false;
        }
        bricks_.push_back(std::unique_ptr<Brick>(new_ptr));
        return true;
    }
    return false;
}

// --- VALIDATIONS GÉOMÉTRIQUES (L21, P61) ---

bool Game::check_bricks_intersections() {
    for (size_t i = 0; i < bricks_.size(); ++i) {
        for (size_t j = i + 1; j < bricks_.size(); ++j) {
            if (intersects_Square_Square(bricks_[i]->get_bounds(), 
                                            bricks_[j]->get_bounds(), 0.0)) {
                std::cout << message::collision_bricks(i, j);
                return false; 
            }
        }
    }
    return true;
}

bool Game::check_paddle_brick_intersections() {
    for (size_t i = 0; i < bricks_.size(); ++i) {
        if (intersects_Circle_Square(paddle_.get_circle(), 
                                        bricks_[i]->get_bounds(), 0.0)) {
            std::cout << message::collision_paddle_brick(i);
            return false;
        }
    }
    return true;
}

bool Game::check_balls_intersections() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = i + 1; j < balls_.size(); ++j) {
            if (intersects_Circle_Circle(balls_[i].get_circle(), 
                                            balls_[j].get_circle(), 0.0)) {
                std::cout << message::collision_balls(i, j);
                return false;
            }
        }
    }
    return true;
}

bool Game::check_ball_brick_intersections() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = 0; j < bricks_.size(); ++j) {
            if (intersects_Circle_Square(balls_[i].get_circle(), 
                                            bricks_[j]->get_bounds(), 0.0)) {
                std::cout << message::collision_ball_brick(i, j);
                return false;
            }
        }
    }
    return true;
}

bool Game::check_ball_paddle_intersections() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        if (intersects_Circle_Circle(balls_[i].get_circle(), 
                                        paddle_.get_circle(), 0.0)) {
            std::cout << message::collision_paddle_ball(i);
            return false;
        }
    }
    return true;
}

bool Game::validate_initial_state() {
    // Regroupement des tests logiques (P2)
    return (check_bricks_intersections() &&
            check_paddle_brick_intersections() &&
            check_balls_intersections() &&
            check_ball_brick_intersections() &&
            check_ball_paddle_intersections());
}


//implémantations incomplètes, temporaire

void Game::save(const std::string& filename) const {
    
}

bool Game::is_over() const {
    return (balls_.empty() || bricks_.empty() || lives_ < 0);
}

void Game::draw() const {
    graphic::draw_arena();

    // 2. Dessiner les briques (appel polymorphique)
    for (const auto& b : bricks_) {
        b->draw();
    }

    // 3. Dessiner les balles
    for (const auto& ball : balls_) {
        ball.draw();
    }

    if (paddle_.is_active()) {
        paddle_.draw(); 
    }
}

void Game::update_paddle_pos(double target_x) {
    double current_x = paddle_.get_circle().center.x;
    double diff_x = target_x - current_x;

    
    double move_x = 0.0;
    if (std::abs(diff_x) > delta_norm_max) {

        move_x = (diff_x > 0 ? delta_norm_max : -delta_norm_max);
    } else {
        
        move_x = diff_x; 
    }

    
    tools::Circle next_circle = paddle_.get_circle();
    next_circle.center.x += move_x; 

    if (tools::is_paddle_in_arena(next_circle, arena_size)) { 
        bool collision = false;
        for (const auto& b : bricks_) {
            if (tools::intersects_Circle_Square(next_circle, b->get_bounds(), tools::epsil_zero)) {
                collision = true;
                break;
            }
        }
        

        if (!collision) {
            paddle_.set_center(next_circle.center.x); 
        }
    }
}

void Game::spawn_ball() {
    if (lives_ > 0 && balls_.empty()) {
        --lives_; // On consomme une vie 
    
        tools::Point pos = {paddle_.get_circle().center.x, 
                     paddle_.get_circle().center.y + paddle_.get_circle().radius + tools::epsil_zero};

        tools::Circle ball_circle = {pos, new_ball_radius};

        tools::Point initial_delta = {0.0, new_ball_delta_norm};

        balls_.push_back(Ball(ball_circle, initial_delta));
    }
}
