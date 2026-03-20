#include <iostream>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour la conversion de chaînes en nombres
#include "game.h"
#include "message.h"
#include "constants.h" 

/*
!ENELVER NAMESPACE STD 
TODO: verifier les messages d'erreur et les contraintes dans validate_constraints(). S'assurer du read_Balls 
*/


Game::Game() : score(0), lives(0) {}

Game::~Game() { clear(); }

void Game::clear() {
    for (auto b : bricks) { 
    delete b;} //obliger d'utiliser delete pour les briques car ce sont des pointeurs
    bricks.clear(); 
    
    for(auto b : balls) {
    delete b;}
    balls.clear();

    score = 0;
    lives = 0;
}


bool get_valid_line(std::ifstream& file, std::string& line) {
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Saute la ligne si vide ou commentaire
        }
        return true; // Ligne valide trouvée
    }
    return false; // Fin du fichier sans rien trouver
}

bool Game::load_file(const std::string& filename) { // Lecture du fichier et initialisation
    clear(); // Nettoie les entités avant de charger un nouveau fichier
    std::ifstream file(filename);
    if (!file) return false ;

    if (!read_header(file)) return false; // Score et vies
    if (!read_paddle(file)) return false; // Raquette
    if (!read_bricks(file)) return false; // Briques
    if (!read_balls(file))  return false; // Balles

    return validate_constraints();
}

bool Game::read_header(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    std::stringstream ss(line);
    if (!(ss >> score >> lives)) return false;

    if (score < 0) {
        std::cout << message::invalid_score(score);
        return false;
    }
    if (lives < 0) {
        std::cout << message::invalid_lives(lives);
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
    paddle = Paddle({{x, y}, r});
    
    // On peut déjà vérifier si le paddle est valide individuellement
    return paddle.check(); 
}

bool Game::read_bricks(std::ifstream& file) {
    std::string line;
    if (!get_valid_line(file, line)) return false;
    
    int nb_bricks;
    if (!(std::stringstream(line) >> nb_bricks)) return false;

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

bool Game::create_brick(int type, double x, double y, double s, std::stringstream& ss) {
    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        if (hp < 1 || hp > 7) {
            std::cout << message::invalid_hit_points(hp);
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
    return bricks.back()->check(); //! Attention compréhension 
}

/*
TODO: - Vérifier la cohérence globale du jeu (Score, vies, etc.)
TODO: - Vérifier les collisions entre les entités (paddle, balles, briques)
TODO: - Vérifier les contraintes d'énoncé (ex: pas de balles qui commencent en dehors de l'arène, etc.)
TODO: - Validation de la dynamique
*/



bool Game::intersectsBB(){
// Vérification des collisions entre briques
    for (size_t i = 0; i < bricks.size(); ++i) {
        for (size_t j = i + 1; j < bricks.size(); ++j) {
            // On récupère les structures Square (bounds_) de chaque brique
            if (intersectsSS(bricks[i]->get_bounds(), bricks[j]->get_bounds())) {
                // Utilisation du message d'erreur du module message
                // Attention : l'ordre des briques dans le message peut être important
                std::cout << message::brick_collision(bricks[i]->get_bounds().center.x, 
                                                      bricks[i]->get_bounds().center.y,
                                                      bricks[j]->get_bounds().center.x, 
                                                      bricks[j]->get_bounds().center.y);
                return false; // Interruption immédiate pour le Rendu 1
            }
        }
    }
    return true;
}
//! Vérification des types
bool Game::CollisionBP(){
// Vérification des collisions entre le paddle et les briques
    for (auto b : bricks) {
        if (intersectsCS(paddle.get_bounds(), b->get_bounds())) {
            std::cout << message::paddle_brick_collision(paddle.get_bounds().center.x, 
                                                        paddle.get_bounds().center.y,
                                                        b->get_bounds().center.x, 
                                                        b->get_bounds().center.y);
            return false; // Interruption immédiate pour le Rendu 1
        }
    }
    return true;
}

//! Vérification des types
bool Game::CollisionBBall(){
// Vérification des collisions entre les balles et les briques
    for (auto ball : balls) {
        for (auto b : bricks) {
            if (intersectsCS(ball->get_bounds(), b->get_bounds())) {
                std::cout << message::ball_brick_collision(ball->get_bounds().center.x, 
                                                        ball->get_bounds().center.y,
                                                        b->get_bounds().center.x, 
                                                        b->get_bounds().center.y);
                return false; // Interruption immédiate pour le Rendu 1
            }
        }
    }
    return true;
}

//! Vérification des types
bool Game::CollisionBPaddle(){
// Vérification des collisions entre les balles et le paddle
    for (auto ball : balls) {
        if (intersectsCC(ball->get_bounds(), paddle.get_bounds())) {
            std::cout << message::ball_paddle_collision(ball->get_bounds().center.x, 
                                                        ball->get_bounds().center.y,
                                                        paddle.get_bounds().center.x, 
                                                        paddle.get_bounds().center.y);
            return false; // Interruption immédiate pour le Rendu 1
        }
    }
    return true;
}

//TODO: messages d'erreur
bool Game::validate_constraints() {
    //1. Vérifier le score et les vies sont cohérents avec les briques et les balles
    if(score < 0 || lives < 0) {
        std::cout << message::invalid_score_or_lives(score, lives);//!: Pas le bon message 
        return false;
    }

    //2. Vérifier que les bricks sont valides (taille, position, etc.)
    for (auto b : bricks) {
        if (!b->check()) {
            std::cout << message::invalid_brick_position(b->get_position());
            return false;
        }
    }
    
    // Vérification des collisions entre briques
    if (!intersectsBB()) return false;

    // Vérification des collisions entre le paddle et les briques
    if (!CollisionBP()) return false;

    // Vérification des collisions entre les balles et les briques
    if (!CollisionBBall()) return false;

    // Vérification des collisions entre les balles et le paddle
    if (!CollisionBPaddle()) return false;

    return true; // Toutes les contraintes sont respectées
}