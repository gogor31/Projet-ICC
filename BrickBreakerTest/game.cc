// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : game.cc
// Description : Implémentation de la boucle de rafraîchissement physique, 
//               des routines de lecture syntaxique et de la détection de chocs.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "game.h"
#include "message.h"
#include "constants.h"

// ==========================================
// UTILS ET NAVIGATION FICHIER
// ==========================================

namespace {
    // Filtre et extrait la prochaine ligne utile en ignorant les blancs et les lignes de commentaire (#)
    bool get_valid_line(std::ifstream& file, std::string& line) {
        while (std::getline(file, line)) {
            size_t first_char = line.find_first_not_of(" \t\r\n");
            if (first_char != std::string::npos && line[first_char] != '#') {
                return true; 
            }
        }
        return false;
    }
    constexpr int NB_SUBSTEPS = 4;
}

// ==========================================
// CONSTRUCTEUR ET DESTRUCTEUR
// ==========================================

Game::Game() {}

Game::~Game() { 
    clear(); 
}

// ==========================================
// INITIALISATION ET ENTRÉES/SORTIES
// ==========================================

bool Game::load_file(const std::string& filename) {
    clear();
    std::ifstream file(filename);
    if (!file) return false;
    
    // Exécution stricte du chargement guidé par la syntaxe du fichier
    bool success = read_header(file) && read_paddle(file) && 
                   read_bricks(file) && read_balls(file) &&
                   validate_initial_state();

    if (!success) {
        clear(); // Annulation et isolation de l'état en cas d'erreur
        return false;
    }

    paddle_.set_active(true);
    std::cout << message::success() << std::endl;
    return true;
}

void Game::clear() { 
    bricks_.clear(); 
    balls_.clear();
    score_ = 0;
    lives_ = 0;
    status_ = ONGOING;
    paddle_.set_active(false);
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

// ==========================================
// BOUCLE DE SIMULATION ET STATUT
// ==========================================

void Game::update() {
    if (status_ != ONGOING) return;

    std::vector<std::unique_ptr<Brick>> bricks_to_add; 
    paddle_.move(target_paddle_x_, bricks_);

    const double inv_substeps = 1.0 / NB_SUBSTEPS;
    
    for (int step = 0; step < NB_SUBSTEPS; ++step) {

        // Déplacement partiel et vérification pour chaque balle
        for (auto& ball : balls_) {
            if (ball.is_dead()) continue;

            ball.backup_position(); // Sauvegarde de la micro-position de départ
            
            // On applique seulement 1/NB_SUBSTEPS du déplacement total
            ball.set_center({ 
                ball.get_circle().center.x + (ball.get_delta().x * inv_substeps),
                ball.get_circle().center.y + (ball.get_delta().y * inv_substeps) 
            });

            // Sortie immédiate par le bas
            if (ball.get_circle().center.y < 0) {
                ball.mark_as_dead();
                continue;
            }

            // Gestion des rebonds sur cette micro-position
            unsigned nb_rebonds = 0;
            while (nb_rebonds < nb_bounce_max) {
                if (handle_arena_collision(ball) || 
                    handle_bricks_collision(ball, bricks_to_add) || 
                    handle_paddle_collision(ball)) {
                    nb_rebonds++;
                } else {
                    break;
                }
            }
        }

        // Traitement des collisions entre balles à cette micro-étape
        handle_ball_ball_collisions();
    }

    // Les ajouts d'objets et nettoyages se font une seule fois à la fin du tick complet
    if (!balls_to_add_.empty()) {
        for (auto& new_ball : balls_to_add_) {
        balls_.push_back(std::move(new_ball));
        }
    balls_to_add_.clear();
    }   
    
    for (auto& new_brick : bricks_to_add) {
        bricks_.push_back(std::move(new_brick));
    }

    cleanup_dead_objects();
    check_game_status();
}

void Game::check_game_status() {
    if (bricks_.empty() && status_ == ONGOING) {
        score_ += (lives_ * score_per_life);
        status_ = WON;
        std::cout << message::won();
    } else if (balls_.empty() && lives_ == 0 && status_ == ONGOING) {
        status_ = LOST;
        std::cout << message::lost();
    }
}

// ==========================================
// ACTIONS ET INTERACTIONS SOURIS
// ==========================================

void Game::update_paddle_pos(double target_x) {
    if (status_ != ONGOING) return;
    target_paddle_x_ = target_x;
}

void Game::spawn_ball() {
    if (lives_ > 0 && balls_.empty()) {
        --lives_; 
        
        // Calcul du décalage pour positionner la balle de manière tangente au sommet du paddle
        double spawn_y = paddle_.get_circle().center.y 
                         + paddle_.get_circle().radius 
                         + new_ball_radius 
                         + tools::epsil_zero * 2;

        tools::Point pos = { paddle_.get_circle().center.x, spawn_y };
        tools::Point initial_delta = { 0.0, new_ball_delta_norm };
        
        add_ball_to_simulation(pos, initial_delta);
    }
}

// ==========================================
// ACCESSEURS NUMÉRIQUES ET CONSTANTS
// ==========================================

bool Game::is_over() const {
    return (status_ != ONGOING);
}

// ==========================================
// RENDU GRAPHIQUE
// ==========================================

void Game::draw() const {
    graphic::draw_arena();

    for (const auto& b : bricks_) {
        b->draw(); 
    }

    for (const auto& ball : balls_) {
        ball.draw();
    }

    if (paddle_.is_active()) {
        paddle_.draw(); 
    }
}

// ==========================================
// LECTURE ET PARSING
// ==========================================

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
    
    target_paddle_x_ = x; 
    return paddle_.check(); 
}

bool Game::read_bricks(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_bricks;
    if (!(std::stringstream(line) >> nb_bricks) || nb_bricks < 0) return false;

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
    std::unique_ptr<Brick> new_ptr = nullptr;

    // Résolution du type lors de l'instanciation
    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        new_ptr = std::make_unique<RainbowBrick>(tools::Square{{x, y}, s}, hp);
    } else if (type == 1) {
        new_ptr = std::make_unique<BallBrick>(tools::Square{{x, y}, s});
    } else if (type == 2) {
        new_ptr = std::make_unique<SplitBrick>(tools::Square{{x, y}, s}); 
    }

    if (new_ptr && new_ptr->check()) {
        bricks_.push_back(std::move(new_ptr)); // Transfert de propriété unique dans le vecteur de base
        return true;
    }
    return false;
}

// ==========================================
// VALIDATIONS INITIALES
// ==========================================

bool Game::validate_initial_state() const {
    return (check_bricks_intersections() &&
            check_paddle_brick_intersections() &&
            check_balls_intersections() &&
            check_ball_brick_intersections() &&
            check_ball_paddle_intersections());
}

bool Game::check_bricks_intersections() const {
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

bool Game::check_paddle_brick_intersections() const {
    for (size_t i = 0; i < bricks_.size(); ++i) {
        if (intersects_Circle_Square(paddle_.get_circle(), 
                                     bricks_[i]->get_bounds(), 0.0)) {
            std::cout << message::collision_paddle_brick(i);
            return false;
        }
    }
    return true;
}

bool Game::check_balls_intersections() const {
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

bool Game::check_ball_brick_intersections() const {
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

bool Game::check_ball_paddle_intersections() const {
    for (size_t i = 0; i < balls_.size(); ++i) {
        if (intersects_Circle_Circle(balls_[i].get_circle(), 
                                     paddle_.get_circle(), 0.0)) {
            std::cout << message::collision_paddle_ball(i);
            return false;
        }
    }
    return true;
}

// ==========================================
// PHYSIQUE ET COLLISIONS
// ==========================================

void Game::handle_ball_ball_collisions() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = i + 1; j < balls_.size(); ++j) {
            if (tools::intersects_Circle_Circle(balls_[i].get_circle(), 
                                                balls_[j].get_circle())) {
                
                tools::Point p1 = balls_[i].get_circle().center;
                tools::Point p2 = balls_[j].get_circle().center;

                // Correction géométrique de la pénétration mutuelle
                tools::resolve_overlap(p1, balls_[i].get_radius(), 
                                       p2, balls_[j].get_radius());

                balls_[i].set_center(p1);
                balls_[j].set_center(p2);
                
                // Calcul et application de l'impulsion physique sur la vitesse
                tools::Point v1 = balls_[i].get_delta();
                tools::Point v2 = balls_[j].get_delta();

                tools::Point new_v1 = tools::compute_impulse(v1, balls_[i].get_radius(), p1, 
                                                            v2, balls_[j].get_radius(), p2);
                                               
                tools::Point new_v2 = tools::compute_impulse(v2, balls_[j].get_radius(), p2,
                                                            v1, balls_[i].get_radius(), p1);
                balls_[i].set_delta(new_v1);
                balls_[j].set_delta(new_v2);
            }
        }
    }
}

bool Game::handle_paddle_collision(Ball& ball) {
    if (tools::intersects_Circle_Circle(ball.get_circle(), 
                                        paddle_.get_circle(), 
                                        tools::epsil_zero)) 
    {
        ball.restore_position(); // Retour à la position sûre pré-cinématique

        tools::Point ball_position = ball.get_circle().center;
        double ball_radius = ball.get_radius();
        tools::Point paddle_position = paddle_.get_circle().center;
        double paddle_radius = paddle_.get_circle().radius;

        // Écarte physiquement la balle de la raquette pour éviter les cas de blocage ou de rebonds multiples indésirables
        tools::resolve_overlap(ball_position, ball_radius, 
                               paddle_position, paddle_radius);
        ball.set_center(ball_position);

        // Réflexion dynamique basée sur la vitesse propre du paddle
        tools::Point new_v = tools::compute_impulse_paddle(ball.get_delta(), ball_position,
                                                           paddle_.get_delta(), paddle_.get_circle().center);
        
        if (new_v.y < 0) { 
            new_v.y = std::abs(ball.get_delta().y); // Assure une expulsion vers le haut 
        }

        ball.set_delta(new_v);
        ball.move();
        return true;
    }
    return false;
}

bool Game::handle_arena_collision(Ball& ball) { 
    const tools::Circle& ball_circ = ball.get_circle();
    const tools::Point& pos = ball_circ.center;
    const double r = ball_circ.radius;
    
    const bool hit_top = (pos.y + r >= arena_size - tools::epsil_zero);
    const bool hit_left = (pos.x - r <= tools::epsil_zero);
    const bool hit_right = (pos.x + r >= arena_size - tools::epsil_zero);

    if (hit_top || hit_left || hit_right) {
        ball.restore_position(); 

        if (hit_top) {
            ball.reverse_dy();
        }
        if (hit_left || hit_right) {
            ball.reverse_dx();
        }

        ball.move(); 
        return true;
    }
    return false;
}

bool Game::handle_bricks_collision(Ball& ball, std::vector<std::unique_ptr<Brick>>& to_add) {
    for (const auto& brick : bricks_) {
    
        if (brick->is_dead()) continue;

        if (tools::intersects_Circle_Square(ball.get_circle(), 
                                            brick->get_bounds(), 
                                            tools::epsil_zero)) {
            ball.restore_position();

            const tools::Point n = tools::compute_nominal_direction(ball.get_circle(), 
                                                                   brick->get_bounds()); 
            ball.set_delta(tools::reflect(ball.get_delta(), n));
            
            score_ += score_per_hit;

            // Déclenchement de la destruction si la brique épuise ses HP
            if (brick->hit()) {
                brick->mark_as_dead();
                handle_brick_destruction_effects(*brick, to_add, ball.get_delta());
            }
            ball.move();
            return true; // Une balle ne traite qu'une collision de brique par itération
        }
    }
    return false;
}

void Game::handle_brick_destruction_effects(const Brick& b, 
                                            std::vector<std::unique_ptr<Brick>>& new_bricks, 
                                            tools::Point ball_delta) {
    switch (b.get_type()) {
        case 1: // BallBrick : Génération instantanée d'une nouvelle balle au centre de l'impact
            spawn_new_ball(b.get_bounds().center, ball_delta);
            break;
        case 2: // SplitBrick : Division géométrique en 4 sous-briques de niveau inférieur
        { 
            double s = b.get_bounds().side;
            double small_s = (s - split_brick_gap) * 0.5;

            if (small_s >= brick_size_min) {
                double offset = (small_s + split_brick_gap) * 0.5;
                double cx = b.get_bounds().center.x;
                double cy = b.get_bounds().center.y;
                
                int next_hp = b.get_hit_points() - 1;
                if (next_hp >= 1) {
                    new_bricks.push_back(std::make_unique<SplitBrick>(
                        tools::Square{{cx-offset, cy-offset}, small_s}, next_hp));
                    new_bricks.push_back(std::make_unique<SplitBrick>(
                        tools::Square{{cx+offset, cy-offset}, small_s}, next_hp));
                    new_bricks.push_back(std::make_unique<SplitBrick>(
                        tools::Square{{cx-offset, cy+offset}, small_s}, next_hp));
                    new_bricks.push_back(std::make_unique<SplitBrick>(
                        tools::Square{{cx+offset, cy+offset}, small_s}, next_hp));
                }
            }
            break;
        }
    }
}

void Game::cleanup_dead_objects() {
    // Nettoyage
    auto it_b = bricks_.begin();
    while (it_b != bricks_.end()) {
        if ((*it_b)->is_dead()) {
            it_b = bricks_.erase(it_b); // Libération automatique de la mémoire par le unique_ptr
        } else {
            ++it_b;
        }
    }

    auto it_ball = balls_.begin();
    while (it_ball != balls_.end()) {
        if (it_ball->is_dead() || it_ball->get_circle().center.y < 0) {
            it_ball = balls_.erase(it_ball);
        } else {
            ++it_ball;
        }
    }
}

// ==========================================
// UTILITAIRES DE CRÉATION DE BALLES
// ==========================================

void Game::add_ball_to_simulation(tools::Point pos, tools::Point velocity) {
    tools::Circle ball_circle = {pos, new_ball_radius};
    balls_to_add_.push_back(Ball(ball_circle, velocity));
}

void Game::spawn_new_ball(tools::Point pos, tools::Point delta) {
    tools::Point spawn_delta = {-delta.x, -delta.y};
    add_ball_to_simulation(pos, spawn_delta);
}