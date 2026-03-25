#include <iostream>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour la conversion de chaînes en nombres
#include "game.h"
#include "message.h"
#include "constants.h" 


// --- Constructeur & Destructeur ---

Game::Game() : score_(0), lives_(0) {}

Game::~Game() { clear(); }

void Game::clear() { 
    // Si bricks_[i] est un RainbowBrick*, le destructeur de RainbowBrick sera appelé.
    for (Brick* b : bricks_) { 
        delete b; 
    }
    bricks_.clear(); 
    
    balls_.clear();
    score_ = 0;
    lives_ = 0;
}


bool get_valid_line(std::ifstream& file, std::string& line) {
    while (std::getline(file, line)) {
        // Cherche le premier caractère qui n'est PAS un espace, une tabulation ou un retour à la ligne
        size_t first_char = line.find_first_not_of(" \t\r\n");
        
        // Si la ligne est vide d'entités utiles ou commence par '#'
        if (first_char == std::string::npos || line[first_char] == '#') {
            continue; // On passe à la ligne suivante
        }
        
        // Si on arrive ici, la ligne contient de vraies données
        return true; 
    }
    return false; // Fin du fichier
}

// --- Méthodes de lecture (Parsing) ---

bool Game::load_file(const std::string& filename) { // Lecture du fichier et initialisation
    clear(); // Nettoie les entités avant de charger un nouveau fichier
    std::ifstream file(filename);
    if (!file) return false ;
    
    if (!read_header(file)) return false; 
    if (!read_paddle(file)) return false; 
    if (!read_bricks(file)) return false; 
    if (!read_balls(file))  return false;

    return validate_initial_state();
}

bool Game::read_header(std::ifstream& file) {
    std::string line;
    
    // 1. Lecture du score sur sa propre ligne
    if (!get_valid_line(file, line)) return false;
    std::stringstream ss_score(line);
    if (!(ss_score >> score_)) return false;

    // 2. Lecture du nombre de vies sur la ligne suivante
    if (!get_valid_line(file, line)) return false;
    std::stringstream ss_lives(line);
    if (!(ss_lives >> lives_)) return false;

    // Validation : Score et vies positifs ou nuls [cite: 272]
    if (score_ < 0) {
        // Remplace par le nom exact de ta fonction message
        std::cout << message::invalid_score(score_); 
        return false;
    }
    if (lives_ < 0) {
        // Remplace par le nom exact de ta fonction message
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

    // Création homogène avec la structure Circle
    paddle_ = Paddle({{x, y}, r});
    
    // On peut déjà vérifier si le paddle est valide individuellement
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
        if (!(ss >> type >> x >> y >> s)) return false; //vérifier l'ordre dans les fichiers d'entrée

        // Validation du type avant création
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

        // La balle se valide elle-même (taille, vitesse maximale, inclusion)
        if (!new_ball.check()) return false;

        balls_.push_back(new_ball);
    }
    return true;
}

bool Game::create_brick(int type, double x, double y, double s, std::stringstream& ss) {
    Brick* new_ptr = nullptr;

    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        new_ptr = new RainbowBrick({{x, y}, s}, hp);
    } 
    else if (type == 1) {
        new_ptr = new BallBrick({{x, y}, s});
    } 
    else if (type == 2) {
        new_ptr = new SplitBrick({{x, y}, s});
    }

    if (new_ptr != nullptr) {
        // Grâce au virtual, ceci appelle RainbowBrick::check() si c'est un Rainbow
        if (!new_ptr->check()) {
            delete new_ptr;
            return false;
        }
        bricks_.push_back(new_ptr);
        return true;
    }
    return false;
}


bool Game::check_bricks_intersections() {
    // On utilise une double boucle pour comparer chaque paire de briques unique 
    for (size_t i = 0; i < bricks_.size(); ++i) {
        for (size_t j = i + 1; j < bricks_.size(); ++j) {
            
            // On récupère les carrés (Square) de chaque brique via une référence constante
            // pour éviter toute copie inutile.
            const Square& s1 = bricks_[i]->get_bounds();
            const Square& s2 = bricks_[j]->get_bounds();

            // La donnée stipule : Intersection si distance < epsil_zero 
            if (intersects_Square_Square(s1, s2, 0.0)) {
                // En cas d'erreur, on affiche le message du module fourni 
                // L'ordre des arguments doit être rigoureux : (x1, y1, x2, y2)
                std::cout << message::collision_bricks(i, j);
                
                // Au Rendu 1, toute erreur de lecture entraîne l'arrêt immédiat 
                return false; 
            }
        }
    }
    return true; // Aucune collision détectée entre les briques
}

bool Game::check_paddle_brick_intersections() {
    // On récupère la forme géométrique de la raquette (un Circle de tools)
    // Note: Seul le cercle complet est utilisé pour les collisions 
    Circle paddle_circle = paddle_.get_circle(); 

    for (size_t i = 0; i < bricks_.size(); ++i) {
        const Square& brick_shape = bricks_[i]->get_bounds();
        // On délègue le test d'intersection au module tools
        if (intersects_Circle_Square(paddle_circle, brick_shape, 0.0)) {
            std::cout << message::collision_paddle_brick(i);
            return false; // Collision détectée
        }
    }
    return true;
}

bool Game::check_balls_intersections() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = i + 1; j < balls_.size(); ++j) {

            const Circle& c1 = balls_[i].get_circle();
            const Circle& c2 = balls_[j].get_circle();

            if (intersects_Circle_Circle(c1, c2, 0.0)) {
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
            const Circle& ball_shape = balls_[i].get_circle();
            const Square& brick_shape = bricks_[j]->get_bounds();

            // Utilisation de la fonction tools pour le test géométrique
            if (intersects_Circle_Square(ball_shape, brick_shape, 0.0)) {
                // On passe l'index de la balle (i) et l'index de la brique (j)
                std::cout << message::collision_ball_brick(i, j);
                return false;
            }
        }
    }
    return true; // Aucune collision initiale détectée
}

bool Game::check_ball_paddle_intersections() {
for (size_t i = 0; i < balls_.size(); ++i) {
        if (intersects_Circle_Circle(balls_[i].get_circle(), paddle_.get_circle(), 0.0)) {
            std::cout << message::collision_paddle_ball(i);
            return false;
        }
    }
    return true;
}


bool Game::validate_initial_state() {

    if (!check_bricks_intersections())       return false; 
    if (!check_paddle_brick_intersections()) return false; 
    if (!check_balls_intersections())        return false; 
    if (!check_ball_brick_intersections())   return false; 
    if (!check_ball_paddle_intersections())  return false; 

    return true;

}