#include <iostream>
#include "ball.h"
#include "tools.h"
#include "constants.h"
#include "message.h"

using namespace std;

Ball::Ball(tools::Circle c, tools::Point d) : circle_(c), delta_(d) {}

bool Ball::check() const { // Ajout de la conditon de position y<0
    double v_norm = tools::norm(delta_);
    if (v_norm > delta_norm_max + tools::epsil_zero) {
        std::cout << message::invalid_delta(delta_.x, delta_.y);
        return false;
    }

    double r = circle_.radius;
    double x = circle_.center.x;
    double y = circle_.center.y;

    bool out = (x - r < 0) || (x + r > arena_size) || 
                  (y + r > arena_size) || (y < 0);

    if (out) {
        std::cout << message::ball_outside(x, y);
        return false;
    }

    return true;
}

void Ball::draw() const {
    circle_.draw(graphic::BLACK, true);
}

tools::Circle Ball::get_circle_next() const { //cercle de la balle a la prochaine (potentielle) position
    return tools::Circle{circle_.center + delta_, circle_.radius};
}

void Ball::set_delta(tools::Point new_delta) {
    double v2 = new_delta.x * new_delta.x + new_delta.y * new_delta.y;
    double max_v2 = delta_norm_max * delta_norm_max;

    if (v2 > max_v2) {
        double factor = delta_norm_max / sqrt(v2);
        delta_.x = new_delta.x * factor; 
        delta_.y = new_delta.y * factor; 
    } else {
        delta_ = new_delta;
    }
}

void Ball::backup_position() {
    old_center_ = circle_.center;
}

void Ball::move() {
    old_center_ = circle_.center;
    circle_.center.x += delta_.x;
    circle_.center.y += delta_.y;
}

void Ball::restore_position() {
    circle_.center = old_center_;
}


