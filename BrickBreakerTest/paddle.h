#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "graphic.h"

class Paddle {
public:
    Paddle() : active(false) {}

    Paddle(tools::Circle c) : circle_(c), active(true) {}

    bool check() const;
    void draw() const;
    
    bool is_active() const { return active; }
    void set_active(bool state) { active = state; }

    const tools::Circle& get_circle() const { return circle_; }
    void set_center_x(double x) { circle_.center.x = x; }

private:
    tools::Circle circle_ = {{0.0, 0.0}, 0.0};
    double delta_x;
    double delta_y;
    bool active;
};


#endif

