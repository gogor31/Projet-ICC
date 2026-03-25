#include "ball.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include <iostream>

Ball::Ball(Circle c, Point d) : circle_(c), delta_(d) {}

bool Ball::check() const {
    // 1. Vérification vitesse (optimisée sans sqrt/pow)
    double v2 = delta_.x * delta_.x + delta_.y * delta_.y;
    if (v2 > (delta_norm_max * delta_norm_max)) {
        std::cout << message::invalid_delta(delta_.x, delta_.y);
        return false;
    }

    // 2. Vérification arène en utilisant l'outil
    if (!is_circle_in_square(circle_, arena_size)) {
        std::cout << message::ball_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    return true;
}

