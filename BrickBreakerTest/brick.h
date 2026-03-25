/*
 * Fichier: brick.h
 * Auteur:  Ilhan Salvatore Legio
 */

#ifndef BRICK_H
#define BRICK_H

#include "tools.h"

namespace brickbreaker { // Namespace nommé pour le projet (O31.2)

    // Classe de base abstraite pour toutes les briques (P2)
    class Brick {
    public:
        virtual ~Brick() = default; 
        virtual bool check() const; 

        const Square& get_bounds() const { return bounds_; }
        int get_type() const { return type_; }

    protected:  
        Brick(Square s, int type) : bounds_(s), type_(type) {}
        
        Square bounds_; 
        int type_;
    };

    class RainbowBrick : public Brick {
    public:
        RainbowBrick(Square s, int hp) : Brick(s, 0), hits_points_(hp) {}
        
        bool check() const override;

    private:
        int hits_points_;
    };

    class BallBrick : public Brick {
    public:
        BallBrick(Square s) : Brick(s, 1) {}
    };

    class SplitBrick : public Brick {
    public: 
        SplitBrick(Square s) : Brick(s, 2) {}
    };

} // namespace brickbreaker

#endif