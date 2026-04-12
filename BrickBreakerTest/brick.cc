#include <iostream>
#include "brick.h"
#include "constants.h"
#include "tools.h"
#include "message.h"

// Vérifie si un carré dépasse les limites de l'arène (P61)
bool is_square_in_arena(double x, double y, double side) {
    double half = side / 2.0;
    bool hors_gauche = (x - half) < 0.0;
    bool hors_droite = (x + half) > arena_size;
    bool hors_haut   = (y - half) < 0.0;
    bool hors_bas    = (y + half) > arena_size;
    
    return (hors_gauche || hors_droite || hors_haut || hors_bas);
}



bool Brick::check() const {
    // Validation de la taille minimale (P5)
    if (bounds_.side < brick_size_min) {
        std::cout << message::invalid_brick_size(bounds_.side);
        return false;
    }

    // Validation de la position dans l'arène (L21)
    if (is_square_in_arena(bounds_.center.x, bounds_.center.y, bounds_.side)) {
        std::cout << message::brick_outside(bounds_.center.x, bounds_.center.y);
        return false;
    }
    
    return true;
}

bool RainbowBrick::check() const {
    // Vérification de la base en premier
    if (!Brick::check()) {
        return false;
    } 
    
    // Validation des points de vie spécifiques (E17, P5)
    if (hits_points_ < 1 || hits_points_ > 7) {
        std::cout << message::invalid_hit_points(hits_points_);
        return false;
    }
    return true;
}
