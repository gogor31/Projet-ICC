/*
 * Fichier: game.cc
 * Auteur:  Ilhan Salvatore Legio
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "game.h"
#include "message.h"
#include "constants.h"
#include "ball.h"

// --- UNNAMED NAMESPACE (O11.2, O15) ---
// Contient les fonctions utilitaires utilisées uniquement dans ce fichier.
namespace { 

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

} // fin du unnamed namespace

// --- NAMESPACE BRICKBREAKER (O16.2.0) ---
namespace brickbreaker {

    // Constructeur : initialisation explicite des membres (L25)
    Game::Game() {}

    // Destructeur : assure la libération de la mémoire (R1)
    Game::~Game() { 
        clear(); 
    }

    void Game::clear() { 
        // Libération de la mémoire allouée dynamiquement pour les briques (R2)
        for (Brick* b : bricks_) { 
            delete b; 
        }
        bricks_.clear(); 
        balls_.clear();
        score_ = 0;
        lives_ = 0;
    }

    // --- LECTURE DU FICHIER (Parsing) ---

    bool Game::load_file(const std::string& filename) {
        clear();
        std::ifstream file(filename);
        if (!file) return false;
        
        // Enchaînement des étapes de lecture avec sortie immédiate en cas d'erreur
        if (!read_header(file) || !read_paddle(file) || 
            !read_bricks(file) || !read_balls(file)) {
            return false;
        }

        return validate_initial_state();
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
            bricks_.push_back(new_ptr);
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
                if (intersects_Circle_Circle(balls_[i].get_circle(),balls_[j].get_circle())){
                    std::cout << message::collision_balls(i,j) << std::endl; 
                    return false;} 
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

} // namespace brickbreaker