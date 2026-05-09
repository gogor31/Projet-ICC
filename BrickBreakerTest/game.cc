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
    paddle_.move(target_x, bricks_);
}

void Game::add_ball_to_simulation(tools::Point pos, tools::Point velocity) {
    tools::Circle ball_circle = {pos, new_ball_radius};
    balls_.push_back(Ball(ball_circle, velocity));
}

void Game::spawn_ball() {
    if (lives_ > 0 && balls_.empty()) {
        --lives_; 
    
        tools::Point pos = {paddle_.get_circle().center.x, 
                            paddle_.get_circle().center.y + 
                            paddle_.get_circle().radius + tools::epsil_zero};


        tools::Point initial_delta = {0.0, new_ball_delta_norm};
        
        add_ball_to_simulation(pos, initial_delta);
    }
}

void Game::spawn_new_ball(tools::Point pos) {
    tools::Point vel = {0.0, new_ball_delta_norm};
    add_ball_to_simulation(pos, vel);
}

void Game::handle_ball_ball_collisions() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = i + 1; j < balls_.size(); ++j) {
            if (tools::intersects_Circle_Circle(balls_[i].get_circle(), 
                                               balls_[j].get_circle())) {
                
                tools::Point v1 = balls_[i].get_delta();
                tools::Point v2 = balls_[j].get_delta();
                

                tools::Point new_v1 = tools::compute_impulse(v1, balls_[i].getRadius(), 
                                       balls_[i].get_circle().center,
                                       v2, balls_[j].getRadius(), 
                                       balls_[j].get_circle().center);
                                       
                tools::Point new_v2 = tools::compute_impulse(v2, balls_[j].getRadius(), 
                                       balls_[j].get_circle().center,
                                       v1, balls_[i].getRadius(), 
                                       balls_[i].get_circle().center);


                balls_[i].set_delta(new_v1);
                balls_[j].set_delta(new_v2);
                
                // Note: On pourrait ajouter un ball.restore_position() si elles s'interpénètrent
            }
        }
    }
}

void Game::handle_brick_destruction_effects(const Brick& b, 
                                            std::vector<std::unique_ptr<Brick>>& new_bricks) {
    switch (b.get_type()) {
        case 1: 
            spawn_new_ball(b.get_bounds().center);
            break;

        case 2: 
            double s = b.get_bounds().side;
            double small_s = (s - split_brick_gap) / 2.0;

            if (small_s >= brick_size_min) {
                double offset = (small_s + split_brick_gap) / 2.0;
                double cx = b.get_bounds().center.x;
                double cy = b.get_bounds().center.y;

    
                new_bricks.push_back(std::make_unique<RainbowBrick>(tools::Square{{cx-offset, cy-offset}, small_s}, 1));
                new_bricks.push_back(std::make_unique<RainbowBrick>(tools::Square{{cx+offset, cy-offset}, small_s}, 1));
                new_bricks.push_back(std::make_unique<RainbowBrick>(tools::Square{{cx-offset, cy+offset}, small_s}, 1));
                new_bricks.push_back(std::make_unique<RainbowBrick>(tools::Square{{cx+offset, cy+offset}, small_s}, 1));
            }
            break;

        default:
            break;
    }
}

void Game::handle_bricks_collision(Ball& ball) {
    for (auto& brick : bricks_) {
        if (!brick->is_dead() && tools::intersects_Circle_Square(ball.get_circle(), brick->get_bounds())) {
            
            ball.restore_position();
            tools::Point n = tools::compute_nominal_direction(ball.get_circle(), brick->get_bounds());
            ball.set_delta(tools::reflect(ball.get_delta(), n));
            
            score_ += score_per_hit;
            
            if (brick->hit()) {
                brick->mark_as_dead();
                
                std::vector<std::unique_ptr<Brick>> to_add;
                handle_brick_destruction_effects(*brick, to_add); 
                
                for (auto& b : to_add) {
                    bricks_.push_back(std::move(b));
                }
            }
            break;
        }
    }
    


}


void Game::handle_paddle_collision(Ball& ball) {
    if (tools::intersects_Circle_Circle(ball.get_circle(), paddle_.get_circle())) {
        ball.restore_position();
        
        tools::Point n = { ball.get_circle().center.x - paddle_.get_circle().center.x,
                           ball.get_circle().center.y - paddle_.get_circle().center.y };
        
        ball.set_delta(tools::reflect(ball.get_delta(), n));
    }
}

bool Game::handle_arena_collision(Ball& ball) { 
    tools::Circle ball_circ = ball.get_circle();
    

    if (ball_circ.center.x - ball_circ.radius <= tools::epsil_zero || 
        ball_circ.center.x + ball_circ.radius >= arena_size - tools::epsil_zero) {
        ball.reverse_dx();
        return true; 
    }

    if (ball_circ.center.y + ball_circ.radius >= arena_size - tools::epsil_zero) {
        ball.reverse_dy();
        return true;
    }

    return false;
}


void Game::cleanup_dead_objects() {
    
    auto it_b = bricks_.begin();
    while (it_b != bricks_.end()) {
        if ((*it_b)->is_dead()) {
            it_b = bricks_.erase(it_b);
        } else {
            ++it_b;
        }
    }

    // 2. Suppression des balles sorties par le bas (y < 0)
    auto it_ball = balls_.begin();
    while (it_ball != balls_.end()) {
        if (it_ball->get_circle().center.y + it_ball->getRadius() < 0) {
            it_ball = balls_.erase(it_ball);
        } else {
            ++it_ball;
        }
    }
}



void Game::update() {
    for (auto& ball : balls_) {
        ball.backup_position();
        ball.move();
        
        handle_arena_collision(ball);
        handle_paddle_collision(ball);
        handle_bricks_collision(ball);
    }

    handle_ball_ball_collisions();

    cleanup_dead_objects();
}

