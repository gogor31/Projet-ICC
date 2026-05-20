#ifndef PADDLE_H
#define PADDLE_H

#include <vector>
#include <memory>
#include <algorithm>
#include "tools.h"
#include "graphic.h"

class Brick; 

class Paddle {
    
public:
    // ==========================================
    // CONSTRUCTEURS
    // ==========================================

    Paddle() : active(false) {}

    Paddle(tools::Circle c);

    // ==========================================
    // ACCESSEURS ET MUTATEURS
    // ==========================================

    bool is_active() const { return active; }

    void set_active(bool state) { active = state; }

    void set_x(double x);

    void set_delta(tools::Point d);

    const tools::Circle& get_circle() const { return circle_; }

    tools::Point get_delta() const { return delta_; }

    tools::Circle get_future_circle(double x) const;

    // ==========================================
    // VÉRIFICATIONS ET LIMITES
    // ==========================================

    bool check() const;

    bool check_position(double next_x) const;

    // ==========================================
    // DYNAMIQUE ET SIMULATION
    // ==========================================
    
    void move(double target_x, const std::vector<std::unique_ptr<Brick>>& bricks);
    
    // ==========================================
    // AFFICHAGE
    // ==========================================

    void draw() const;

private:
    tools::Circle circle_ = {{0.0, 0.0}, 0.0};
    tools::Point delta_ = {0.0, 0.0};
    bool active = false;
};

#endif