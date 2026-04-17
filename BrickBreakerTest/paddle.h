#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "graphic.h"

class Paddle {
public:
    Paddle() = default;
    Paddle(tools::Circle c);

    bool check() const;
    void draw() const;
    
    const tools::Circle& get_circle() const { return circle_; }
    void set_center_x(double x) { circle_.center.x = x; }

private:
    tools::Circle circle_ = {{0.0, 0.0}, 0.0};
    double delta_x;
    double delta_y;
};


#endif

