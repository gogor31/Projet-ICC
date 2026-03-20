#include <iostream>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour la conversion de chaînes en nombres
#include "game.h"
#include "message.h"
#include "constants.h" 

using namespace std;

Game::Game() : score(0), lives(0) {}

Game::~Game() { clear(); }

void Game::clear() {
    for (size_t i = 0; i < bricks.size(); ++i) {// On supprime l'objet pointé à l'indice i 
    delete bricks[i];} //obliger d'utiliser delete pour les briques car ce sont des pointeurs
    bricks.clear(); 
    balls.clear();
    score = 0;
    lives = 0;
}


bool read_next_valid_line(ifstream& file, string& line) {
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Saute la ligne si vide ou commentaire
        }
        return true; // Ligne valide trouvée
    }
    return false; // Fin du fichier sans rien trouver
}

bool Game::load_file(const string& filename) { // Lecture du fichier et initialisation
    clear(); // Nettoie les entités avant de charger un nouveau fichier
    ifstream file(filename);
    if (!file) return false ;

    if (!read_header(file)) return false; // Score et vies
    if (!read_paddle(file)) return false; // Raquette
    if (!read_bricks(file)) return false; // Briques
    if (!read_balls(file))  return false; // Balles

    return validate_constraints();
}
bool Game::read_header(ifstream& file) {
    string line;
    if (!get_valid_line(file, line)) return false;
    
    stringstream ss(line);
    if (!(ss >> score >> lives)) return false;

    if (score < 0) {
        cout << message::invalid_score(score);
        return false;
    }
    if (lives < 0) {
        cout << message::invalid_lives(lives);
        return false;
    }
    return true;
}

bool Game::read_paddle(ifstream& file) {
    string line;
    if (!get_valid_line(file, line)) return false;
    
    double x, y, r;
    stringstream ss(line);
    if (!(ss >> x >> y >> r)) return false;

    // Création homogène avec la structure Circle
    paddle = Paddle({{x, y}, r});
    
    // On peut déjà vérifier si le paddle est valide individuellement
    return paddle.check(); 
}

bool Game::read_bricks(ifstream& file) {
    string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_bricks;
    if (!(stringstream(line) >> nb_bricks)) return false;

    for (int i = 0; i < nb_bricks; ++i) {
        if (!get_valid_line(file, line)) return false;
        
        stringstream ss(line);
        int type; double x, y, s; 
        if (!(ss >> type >> x >> y >> s)) return false; //vérifier l'ordre dans les fichiers d'entrée

        // Validation du type avant création
        if (type < 0 || type > 2) {
            cout << message::invalid_brick_type(type);
            return false;
        }

        if (!create_brick(type, x, y, s, ss)) return false;
    }
    return true;
}

bool Game::read_balls(ifstream& file) {
    /*
    string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_balls;
    if (!(stringstream(line) >> nb_balls)) return false;

    for (int i = 0; i < nb_balls; ++i) {
        if (!get_valid_line(file, line)) return false;
        
        stringstream ss(line);
        double x, y, r, dx, dy;
        if (!(ss >> x >> y >> r >> dx >> dy)) return false;

        // Création homogène : Circle pour la forme, Point pour la vitesse
        Ball new_ball({{x, y}, r}, {dx, dy});

        // Validation immédiate
        if (!new_ball.check()) return false;

        balls.push_back(new_ball);
    }
    return true;
    */
}

bool Game::create_brick(int type, double x, double y, double s, stringstream& ss) {
    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        if (hp < 1 || hp > 7) {
            cout << message::invalid_hit_points(hp);
            return false;
        }
        bricks.push_back(new RainbowBrick({{x, y}, s}, hp));
    } 
    else if (type == 1) {
        bricks.push_back(new BallBrick({{x, y}, s}));
    } 
    else {
        bricks.push_back(new SplitBrick({{x, y}, s}));
    }
    
    // On vérifie la brique qu'on vient d'ajouter (taille, arène)
    return bricks.back()->check();
}

bool Game::validate_constraints() {
    // 1. Score et vies (déjà fait, mais attention au type)
    if (score < 0 || lives < 0) {
        // Appeler message d'erreur approprié
    
        return false;

    }

    // 2. Validation de la raquette (Paddle)
    // Elle doit être comprise dans [0, arena_size][cite: 132, 273].
    if (!paddle.is_valid()) {
        std::cout << message::paddle_out_of_arena(); // Exemple de message [cite: 374]
        return false;
    }

    // 3. Validation des briques
    for (auto b : bricks) {
        // Inclusion stricte dans l'arène [cite: 98, 273]
        if (!b->is_inside_arena()) {
            std::cout << message::brick_out_of_arena(); 
            return false;
        }
        // Taille minimale [cite: 97, 274, 433]
        if (b->get_size() < brick_size_min) {
            std::cout << message::brick_too_small();
            return false;
        }
    }

    // 4. Validation des balles
    for (const auto& ball : balls) {
        // Inclusion (sauf bord inférieur) [cite: 125, 273]
        if (!ball.is_inside_arena()) {
            std::cout << message::ball_out_of_arena();
            return false;
        }
        // Vitesse maximale [cite: 124, 276, 431]
        if (ball.get_delta_norm() > delta_norm_max) {
            std::cout << message::ball_speed_too_high();
            return false;
        }
    }

    // 5. Absence de collisions initiales 
    if (check_initial_collisions()) {
        // Si check_initial_collisions renvoie true, c'est qu'il y a une erreur
        return false; 
    }

    return true;
}

