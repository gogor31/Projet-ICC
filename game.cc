#include <iostream>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour la conversion de chaînes en nombres
#include "game.h"
#include "message.h"
#include "constants.h"


// TODO: verifier les messages d'erreur et les contraintes dans validate_constraints(). S'assurer du read_Balls 


// --- Constructeur & Destructeur ---

Game::Game() : score_(0), lives_(0) {}

Game::~Game() { clear(); }

void Game::clear() {
    for (Brick* b : bricks_) { 
        // On inspecte le type manuellement pour faire le bon delete
        if (b->get_type() == 0) {
            delete static_cast<RainbowBrick*>(b);
        } 
        else if (b->get_type() == 1) {
            delete static_cast<BallBrick*>(b);
        } 
        else if (b->get_type() == 2) {
            delete static_cast<SplitBrick*>(b);
        } else {
            // Sécurité de base au cas où
            delete b; 
        }
    }
    bricks_.clear(); 
    
    balls_.clear();
    score_ = 0;
    lives_ = 0;
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

// --- Méthodes de lecture (Parsing) ---

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
    paddle_ = Paddle({{x, y}, r});
    
    // On peut déjà vérifier si le paddle est valide individuellement
    return paddle_.check(); 
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
    if (type == 0) {
        int hp;
        if (!(ss >> hp)) return false;
        
        // 1. On instancie le pointeur avec le type EXACT
        RainbowBrick* rb = new RainbowBrick({{x, y}, s}, hp);
        
        // 2. Le compilateur sait que c'est un RainbowBrick, 
        // il appelle donc RainbowBrick::check() !
        if (!rb->check()) { 
            delete rb;
            return false;
        }
        // 3. Si c'est valide, on l'ajoute au vecteur générique (Upcasting implicite)
        bricks_.push_back(rb); 
    } 
    else if (type == 1) {
        BallBrick* bb = new BallBrick({{x, y}, s});
        if (!bb->check()) { // Appelle Brick::check()
            delete bb;
            return false;
        }
        bricks_.push_back(bb);
    } 
    else if (type == 2) {
        SplitBrick* sb = new SplitBrick({{x, y}, s});
        if (!sb->check()) { // Appelle Brick::check()
            delete sb;
            return false;
        }
        bricks_.push_back(sb);
    } else {
        return false; // Type inconnu
    }
    
    return true; 
}

/*
TODO: - Vérifier la cohérence globale du jeu (Score, vies, etc.)
TODO: - Vérifier les collisions entre les entités (paddle, balles, briques)
TODO: - Vérifier les contraintes d'énoncé (ex: pas de balles qui commencent en dehors de l'arène, etc.)
TODO: - Validation de la dynamique
*/


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
                std::cout << message::brick_collision(s1.center.x, s1.center.y,
                                                     s2.center.x, s2.center.y);
                
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

    for (const auto& brick_ptr : bricks_) {
        // On récupère la forme géométrique de la brique (un Square de tools) 
        const Square& brick_shape = brick_ptr->get_bounds(); // On suppose que get_shape() retourne un Square pour les briques
        // On délègue le test d'intersection au module tools
        if (intersects_Circle_Square(paddle_circle, brick_shape, 0.0)) {
            std::cout << message::paddle_brick_collision(
                paddle_circle.center.x, paddle_circle.center.y,
                brick_shape.center.x, brick_shape.center.y);
            return false; // Collision détectée
        }
    }
    return true;
}

bool Game::check_balls_intersections() {
    for (size_t i = 0; i < balls_.size(); ++i) {
        for (size_t j = i + 1; j < balls_.size(); ++j) {

            const Circle& c1 = balls_[i]->get_circle();
            const Circle& c2 = balls_[j]->get_circle();

            if (intersects_Circle_Circle(c1, c2, 0.0)) {
                std::cout << message::collision_balls(c1.center.x, c1.center.y,
                                                        c2.center.x, c2.center.y);

                return false;
            }
        }
    }
    return true;
}

bool Game::check_ball_brick_intersections() {
    // On parcourt chaque balle du vecteur de pointeurs
    for (const auto& ball : balls_) {
        const Circle& ball_shape = ball.get_circle();

        // On compare avec chaque brique du vecteur de pointeurs
        for (const auto& brick_ptr : bricks_) {
            const Square& brick_shape = brick_ptr->get_bounds();

            // Utilisation de la fonction tools pour le test géométrique
            if (intersects_Circle_Square(ball_shape, brick_shape, 0.0)) {
                
                // Affichage du message d'erreur obligatoire [cite: 366]
                // On utilise les centres respectifs pour le message
                std::cout << message::collision_ball_brick(
                    ball_shape.center.x,  ball_shape.center.y,
                    brick_shape.center.x, brick_shape.center.y);

                return false;
            }
        }
    }
    return true; // Aucune collision initiale détectée
}

bool Game::check_ball_paddle_intersections() {
    for (const auto& ball : balls_) {
        // paddle_.get_bounds() doit retourner le Circle de la raquette
        if (intersects_Circle_Circle(ball.get_circle(), paddle_.get_circle(), 0.0)) {
            std::cout << message::collision_paddle_ball(
                ball.get_circle().center.x, ball.get_circle().center.y,
                paddle_.get_circle().center.x, paddle_.get_circle().center.y);
            return false;
        }
    }
    return true;
}


bool Game::validate_initial_state()() {

    if (!check_bricks_intersections())       return false; 
    if (!check_paddle_brick_intersections()) return false; 
    if (!check_balls_intersections())        return false; 
    if (!check_ball_brick_intersections())   return false; 
    if (!check_ball_paddle_intersections())  return false; 

    std::cout << message::success(); 
    return true;

}

bool Game::collisionBallBall(Ball A, Ball B){
    if (intersects_Circle_Circle(A.get_circle_next(),B.get_circle_next())){
        std::cout << message::collision_balls(A.getIndex(),B.getIndex()) << std::endl;
        return true;
    } else {return false;}
}

bool Game::collsionBallBrick(Ball A,Brick B){
    if (intersects_Circle_Square(A.get_circle_next(),B.get_bounds())){
         std::cout << message::collision_ball_brick(A.getIndex(),B.getIndex()) << std::endl; //j'ai pas encore les indexes de briques
         return true;
    } else {return false;}
}

bool Game::collsionBallPaddle(Ball A, Paddle P){
    if (intersects_Circle_Circle(A.get_circle_next(),P.get_circle())){
        std::cout << message::collision_paddle_ball(A.getIndex()) << std::endl;
        return true;
    } else {return false;}
}

bool Game::Ball_out_of_bounds(Ball B){
    //pour eviter de repeter des fonctions get + lisibilte
    double pos_x = B.getPos_x();
    double pos_y = B.getPos_y();
    double r = B.getRadius();
    
    bool trop_haut(pos_y - r > arena_size);
    bool trop_bas(pos_y < 0);
    bool trop_gauche(pos_x + r < 0);
    bool trop_droite(pos_x - r < arena_size);

    if (trop_haut or trop_bas or trop_droite or trop_gauche){
        std::cout << message::ball_outside(pos_x, pos_y) << std::endl;
        return true;
    } else {return false;}
}