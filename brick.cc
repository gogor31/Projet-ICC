#include "brick.h"
#include "constants.h"
#include "message.h"
#include <iostream>

using namespace std;

// Validation de base pour TOUTES les briques
bool Brick::check() const {
    // 1. Taille minimale 
    if (bounds_.side < brick_size_min) {
        cout << message::invalid_brick_size(); //attention message
        return false;
    }

    // 2. Doit être intégralement dans l'arène 
    // Un carré est dans l'arène si : centre - côté/2 >= 0 ET centre + côté/2 <= arena_size
    double half = bounds_.side / 2.0;
    if ((bounds_.center.x - half) < 0.0 || (bounds_.center.x + half) > arena_size ||
        (bounds_.center.y - half) < 0.0 || (bounds_.center.y + half) > arena_size) {
        cout << message::brick_out_of_arena();
        return false;
    }
    return true;
} 


// --- Classe RainbowBrick ---
bool RainbowBrick::check() const {
    if (!Brick::check()){
        return false;
    } 
    
    if (hits_points_ < 1 || hits_points_ > 7) {
        return false;
    }
    return true;
}

//Ballbrick et Splitbrick n'ont pas de contraintes supplémentaires, donc pas besoin de redéfinir check() pour eux. Ils sont déjà définis dans le .h