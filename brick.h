#ifndef BRICK_H
#define BRICK_H

#include "tools.h"


// Classe de base abstraite pour toutes les briques
class Brick {
public:
    Brick(Square s, int type) : bounds_(s), type_(type) {}

    //virtual ~Brick() {} //Pas sur que ce soit nécaissaire RENDU 1 

    const Square& get_bounds() const { return bounds_; }
    int get_type() const { return type_; }
private:
    Square bounds_; // Utilisation de struct Square de tools [cite: 382]
    int type_;      // 0: Rainbow, 1: Ball, 2: Split [cite: 251]
};

// --- Sous-classes spécifiques ---

class RainbowBrick : public Brick{
public:
    RainbowBrick(Square s, int hp)
        : Brick(s, 0), hp_(hp){}
    
    int get_hp() const { return hp_; }
private:
    int hp_;
};

class BallBrick : public Brick{
public:
    BallBrick(Square s) : Brick(s, 1) {}
};

class SplitBrick: public Brick{
public: 
    SplitBrick(Square s) : Brick(s, 2) {}
};


#endif