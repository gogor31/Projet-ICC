#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "graphic.h"

// ==========================================
// CLASSE DE BASE ABSTRAITE
// ==========================================

class Brick {
public:
    virtual ~Brick() = default; 
    virtual bool check() const; 
    virtual void draw() const = 0;
    virtual void write(std::ostream& out) const = 0;
    virtual int get_hit_points() const = 0;
    virtual bool hit() = 0;

    const tools::Square& get_bounds() const { return bounds_; }
    int get_type() const { return type_; } 
    bool is_dead() const { return dead_; }
    void mark_as_dead() { dead_ = true; }

protected:  
    Brick(tools::Square s, int type) : bounds_(s), type_(type) {}
    
    tools::Square bounds_; 
    int type_;
    bool dead_ = false;
};

// ==========================================
// CLASSES DÉRIVÉES
// ==========================================

class RainbowBrick : public Brick {
public:
    RainbowBrick(tools::Square s, int hp) : Brick(s, 0), hit_points_(hp) {}
    
    bool check() const override;
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return hit_points_; }
    bool hit() override {
        --hit_points_;
        return (hit_points_ <= 0);
    }

private:
    int hit_points_;
};

class BallBrick : public Brick {
public:
    BallBrick(tools::Square s) : Brick(s, 1) {}
    
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return 1; }
    bool hit() override { return true; }
};

class SplitBrick : public Brick {
public: 
    SplitBrick(tools::Square s, int hp = -1);
    
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return hit_points_; }
    bool hit() override { return true; }

private:
    void draw_recursive(tools::Square s, int hp) const;
    int hit_points_;
};

#endif