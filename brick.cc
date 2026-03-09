#include "brick.h"
#include "constants.h"
#include "message.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Brick::Brick(Square s, int type) : bounds_(s), type_(type) {
    double half = s.side / 2.0;
    
    if (s.side <= brick_size_min) {
        cout << message::invalid_brick_size(s.side) << endl;
        exit(EXIT_FAILURE);
    }

        // 2. Test d'inclusion dans l'arène (Section 3.1)
    if ((s.center.x - half < 0) || (s.center.x + half > arena_size) ||
        (s.center.y - half < 0) || (s.center.y + half > arena_size)) {
        cout << message::brick_outside(s.center.x, s.center.y) << endl;
        exit(EXIT_FAILURE);
    }
} 


// --- Classe RainbowBrick ---
RainbowBrick::RainbowBrick(Square s, int hp) : Brick(s, 0), hp_(hp){
    if (hp < 1 || hp > 7) {
        cout << message::invalid_hit_points(hp) << endl;
        exit(EXIT_FAILURE);
    }
}
// --- Classe BallBrick ---
BallBrick::BallBrick(Square s) : Brick(s, 1){
}  

// --- Classe SplitBrick ---
SplitBrick::SplitBrick(Square s) : Brick(s, 2){
} 