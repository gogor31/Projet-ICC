#include <iostream>
#include <cmath>
#include "ball.h"
#include "tools.h"
#include "constants.h"
#include "message.h"

using namespace std;

// ==========================================
// CONSTRUCTEURS
// ==========================================

Ball::Ball(tools::Circle c, tools::Point d) : circle_(c), delta_(d) {}

// ==========================================
// ACCESSEURS ET MUTATEURS
// ==========================================

tools::Circle Ball::get_circle_next() const {
    return tools::Circle{circle_.center + delta_, circle_.radius};
}

void Ball::set_delta(tools::Point new_delta) {
    tools::clamp_vector(new_delta, delta_norm_max);
    delta_ = new_delta;
}

void Ball::set_center(tools::Point p) {
    circle_.center = p;
}

void Ball::mark_as_dead() {
    dead_ = true;
}

bool Ball::is_dead() const {
    return dead_;
}

// ==========================================
// VÉRIFICATIONS ET LIMITES
// ==========================================

bool Ball::check() const {
    const double v_norm = tools::norm(delta_);

    if (v_norm > delta_norm_max) {
        std::cout << message::invalid_delta(delta_.x, delta_.y);
        return false;
    }

    const double r = circle_.radius;
    const double x = circle_.center.x;
    const double y = circle_.center.y;

    bool out = (x - r < -tools::epsil_zero) || 
               (x + r > arena_size + tools::epsil_zero) || 
               (y + r > arena_size + tools::epsil_zero) || 
               (y < -tools::epsil_zero);

    if (out) {
        std::cout << message::ball_outside(x, y);
        return false;
    }
    return true;
}

// ==========================================
// CINÉMATIQUE ET SIMULATION
// ==========================================

void Ball::move() {
    circle_.center.x += delta_.x;
    circle_.center.y += delta_.y;
}

void Ball::reverse_dx() {
    delta_.x *= -1;
}

void Ball::reverse_dy() {
    delta_.y *= -1;
}

void Ball::backup_position() {
    old_center_ = circle_.center;
}

void Ball::restore_position() {
    circle_.center = old_center_;
}

// ==========================================
// AFFICHAGE
// ==========================================

void Ball::draw() const {
    graphic::draw_circle(circle_.center.x, circle_.center.y, circle_.radius, graphic::BLACK, true);
}