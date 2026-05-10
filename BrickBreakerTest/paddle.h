#ifndef PADDLE_H
#define PADDLE_H

#include <vector>
#include <memory>
#include "tools.h"
#include "graphic.h"

class Brick; // declaration anticipée pour éviter les dépendances circulaires

class Paddle {
public:
    Paddle() : active(false) {}

    Paddle(tools::Circle c);

    bool check() const;
    void draw() const;
    
    bool is_active() const { return active; }
    void set_active(bool state) { active = state; }

    const tools::Circle& get_circle() const { return circle_; }
    void set_center(double x) { 
        circle_.center.x = x; 
    }
    
    tools::Point get_delta() const { return delta_; }
    
    void move(double target_x, const std::vector<std::unique_ptr<Brick>>& bricks);
private:
    tools::Circle circle_ = {{0.0, 0.0}, 0.0};
    tools::Point delta_ = {0.0, 0.0};
    bool active = false;
};


#endif

