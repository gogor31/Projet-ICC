#include "ball.h"
#include <cmath>
#include <iostream>
#include "constants.h"
#include "message.h"

Ball::Ball(Circle c, Point d) : circle_(c), delta_(d) {}

bool Ball::check() const {
    // 1. Vérification de la vitesse (norme du delta)
    double dx = std::pow(delta_.x, 2);
    double dy = std::pow(delta_.y, 2);
    double speed = std::sqrt(dx + dy);
    
    if (speed > delta_norm_max) {
        std::cout << message::invalid_delta(delta_.x, delta_.y); 
        return false;
    }

    // 2. Vérification de l'inclusion dans l'arène
    // Axe X 
    if (circle_.center.x < circle_.radius || 
        circle_.center.x > arena_size - circle_.radius ) {
        
        std::cout << message::ball_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    // Axe Y : Bord supérieur
    if (circle_.center.y > arena_size - circle_.radius || circle_.center.y < 0.0) {
        std::cout << message::ball_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    return true;
}

bool Ball::collides_with(const Ball& other) const {
    double dx = std::pow(circle_.center.x - other.circle_.center.x, 2);
    double dy = std::pow(circle_.center.y - other.circle_.center.y, 2);
    double distance = std::sqrt(dx + dy);

    return (distance < (circle_.radius + other.circle_.radius));
}
