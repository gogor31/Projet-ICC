#ifndef BRICK_H
#define BRICK_H

#include <vector>
#include "tools.h"
#include "graphic.h"

class Brick {
public:
    virtual ~Brick() = default; 
    virtual bool check() const; 
    virtual void draw() const = 0;

    const Square& get_bounds() const { return bounds_; }
    int get_type() const { return type_; } //!utilité du type ? 

protected:  
    Brick(Square s, int type) : bounds_(s), type_(type) {}
    
    Square bounds_; 
    int type_;
};

class RainbowBrick : public Brick {
public:
    RainbowBrick(Square s, int hp) : Brick(s, 0), hits_points_(hp) {}
    
    bool check() const override;
    void draw() const override;

private:
    int hits_points_;
};

class BallBrick : public Brick {
public:
    BallBrick(Square s) : Brick(s, 1) {}
    void draw() const override;
};

class SplitBrick : public Brick {
public: 
    SplitBrick(Square s) : Brick(s, 2) {}
    void draw() const override;
};


#endif