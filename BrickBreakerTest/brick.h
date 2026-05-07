#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "graphic.h"

class Brick {
public:
    virtual ~Brick() = default; 
    virtual bool check() const; 
    virtual void draw() const = 0;
    virtual void write(std::ostream& out) const = 0;

    const tools::Square& get_bounds() const { return bounds_; }
    int get_type() const { return type_; } 
    
    virtual void hit() = 0;

protected:  
    Brick(tools::Square s, int type) : bounds_(s), type_(type) {}
    
    tools::Square bounds_; 
    int type_;
};

class RainbowBrick : public Brick {
public:
    RainbowBrick(tools::Square s, int hp) : Brick(s, 0), hit_points_(hp) {}
    
    bool check() const override;
    void draw() const override;
    void write(std::ostream& out) const override;

    bool hit() override {
        return (--hit_points_ <= 0);
    }

private:
    int hit_points_;
};

class BallBrick : public Brick {
public:
    BallBrick(tools::Square s) : Brick(s, 1) {}
    void draw() const override;
    void write(std::ostream& out) const override;

    bool hit() override {
        return true; 
    }
};

class SplitBrick : public Brick {
public: 
    SplitBrick(tools::Square s) : Brick(s, 2) {}
    void draw() const override;
    void write(std::ostream& out) const override;

    bool hit() override {
        return true; 
    }
};


#endif