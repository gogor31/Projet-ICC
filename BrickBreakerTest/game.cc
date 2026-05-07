#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "game.h"
#include "message.h"
#include "constants.h"


// Lit la prochaine ligne qui n'est ni vide ni un commentaire 
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



Game::Game() {}


Game::~Game() { 
    clear(); 
}

void Game::clear() { 
    bricks_.clear(); 
    balls_.clear();
    score_ = 0;
    lives_ = 0;
    paddle_.set_active(false);
}

// --- LECTURE DU FICHIER ---

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

    // Factory simple pour le polymorphisme
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
        // Appel polymorphique de check()
        if (!new_ptr->check()) {
            delete new_ptr;
            return false;
        }
        bricks_.push_back(std::unique_ptr<Brick>(new_ptr));
        return true;
    }
    return false;
}

// --- VALIDATIONS GÉOMÉTRIQUES ---

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
    return (check_bricks_intersections() &&
            check_paddle_brick_intersections() &&
            check_balls_intersections() &&
            check_ball_brick_intersections() &&
            check_ball_paddle_intersections());
}



void Game::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) return;

    file << "# score" << std::endl << score_ << std::endl << std::endl;
    file << "# lives" << std::endl << lives_ << std::endl << std::endl;

    if (paddle_.is_active()) {
        file << "# paddle" << std::endl;
        file << paddle_.get_circle().center.x << " " 
             << paddle_.get_circle().center.y << " " 
             << paddle_.get_circle().radius << std::endl << std::endl;
    }

    file << "# bricks" << std::endl << bricks_.size() << std::endl;
    for (const auto& b : bricks_) {

        b->write(file); 
    }
    file << std::endl;

    file << "# balls" << std::endl << balls_.size() << std::endl;
    for (const auto& ball : balls_) {
        file << ball.get_circle().center.x << " " 
             << ball.get_circle().center.y << " " 
             << ball.get_circle().radius << " " 
             << ball.get_delta().x << " " 
             << ball.get_delta().y << std::endl;
    }
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



bool Game::ball_arena_collisions(Ball& ball) { 
    tools::Circle ball_circ = ball.get_circle();
    
    // Mur Gauche ou Droit
    if (ball_circ.center.x - ball_circ.radius <= tools::epsil_zero || 
        ball_circ.center.x + ball_circ.radius >= arena_size - tools::epsil_zero) {
        ball.reverse_dx();
        return true; 
    }
    // Plafond
    if (ball_circ.center.y + ball_circ.radius >= arena_size - tools::epsil_zero) {
        ball.reverse_dy();
        return true;
    }

    return false;
}

bool Game::ball_ball_collisions(Ball& b1, Ball& b2) {
    if (!(tools::intersects_Circle_Circle(b1.get_circle(), b2.get_circle(), tools::epsil_zero))) {
        return false;
    }
    
    tools::Point pos1 = b1.get_circle().center;
    tools::Point pos2 = b2.get_circle().center;

    double dist = tools::distance(pos1, pos2);
    if (dist < tools::epsil_zero) {
        return false; 
    }

    tools::Point n = {(pos2.x - pos1.x) / dist, (pos2.y - pos1.y) / dist};

    double vn1 = b1.get_delta().x * n.x + b1.get_delta().y * n.y;
    double vn2 = b2.get_delta().x * n.x + b2.get_delta().y * n.y;  
    
    double m1 = b1.get_circle().radius * b1.get_circle().radius;
    double m2 = b2.get_circle().radius * b2.get_circle().radius;
    
    double common_factor = (2.0 * (-vn1 + vn2)) / (m1 + m2);
    
    tools::Point impulse1 = { (common_factor * m2) * n.x, (common_factor * m2) * n.y };
    tools::Point impulse2 = { (-common_factor * m1) * n.x, (-common_factor * m1) * n.y };

    tools::Point new_d1 = { b1.get_delta().x + impulse1.x, b1.get_delta().y + impulse1.y };
    tools::Point new_d2 = { b2.get_delta().x + impulse2.x, b2.get_delta().y + impulse2.y };

    b1.set_delta(new_d1);
    b2.set_delta(new_d2);

    return true;
}

void Game::calc_ball_on_brick(Ball& ball, const tools::Square& brick_sq) {
    double diff_x = ball.getPos_x() - brick_sq.center.x;
    double diff_y = ball.getPos_y() - brick_sq.center.y;
    
    double half_side = brick_sq.side / 2.0; // fonction intersect ? 
    double bounded_x = std::max(-half_side, std::min(diff_x, half_side));
    double bounded_y = std::max(-half_side, std::min(diff_y, half_side));

    tools::Point v_n = {diff_x - bounded_x, diff_y - bounded_y};

    double norm = std::sqrt(v_n.x * v_n.x + v_n.y * v_n.y);
    if (norm < tools::epsil_zero) {
        v_n.x /= norm;
        v_n.y /= norm;
    }

    tools::Point v = ball.get_delta(); 
    double dot_product = v.x * v_n.x + v.y * v_n.y; 
    
    tools::Point new_delta = {
        v.x - 2 * dot_product * v_n.x,
        v.y - 2 * dot_product * v_n.y 
    };
    
    ball.set_delta(new_delta);
}

bool Game::handle_ball_brick_collisions(Ball& ball) {
    for (auto it = bricks_.begin(); it != bricks_.end(); ) { 
        if (tools::intersects_Circle_Square(b.get_circle(), (*it)->get_bounds(), tools::epsil_zero)) {
            
            calc_ball_on_brick(b, (*it)->get_bounds());
            
            score_ += score_per_hit; 


            int type = (*it)->get_type(); 
            tools::Square b_bounds = (*it)->get_bounds(); 
            
            if ((*it)->hit()) { 

                if (type == 1) spawn_ball_from_brick(b_bounds, b.get_delta()); // fonction à coder 
                if (type == 2) split_brick_logic(b_bounds); // fonction à coder
                
                it = bricks_.erase(it); 
            } else {
                ++it;
            }
            return true;
        } else {
            ++it;
        }
    }
    return false;
}


void Game::ball_paddle_collisions(Ball& ball) {

}

void Game::update() {//bouton step pour le rendu 3
    //déplace toutes les balles
    for (auto& ball : balls_) {
        ball.move();
    }
}

//TODO: collision balle balle(OK), balle arene(OK), balle brique(PAS FINI), balle raquette(A FAIRE) 