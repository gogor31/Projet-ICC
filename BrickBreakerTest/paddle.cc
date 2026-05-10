#include <iostream>
#include <cmath>
#include "paddle.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "graphic.h"
#include "brick.h"

using namespace std;

Paddle::Paddle(tools::Circle c) : circle_(c), active(true) {} 

bool Paddle::check() const {
    if(!active) return true;
    

    if (circle_.center.y > tools::epsil_zero || (circle_.center.y + circle_.radius) <= tools::epsil_zero) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    double dy = std::abs(circle_.center.y);
    double dx = std::sqrt(circle_.radius * circle_.radius - dy * dy);

    double x1 = circle_.center.x - dx;
    double x2 = circle_.center.x + dx;

    bool x1_hors_limites = (x1 < -tools::epsil_zero || x1 > arena_size + tools::epsil_zero);
    bool x2_hors_limites = (x2 < tools::epsil_zero || x2 > arena_size + tools::epsil_zero);

    if (x1_hors_limites || x2_hors_limites) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    return true;
}

void Paddle::draw() const {
    if (!active) return;

    graphic::draw_arc(circle_.center.x, circle_.center.y, circle_.radius, graphic::BLACK);
}

void Paddle::move(double target_x, const std::vector<std::unique_ptr<Brick>>& bricks) {
    if (!active) return;

    double current_x = circle_.center.x;
    double diff_x = target_x - current_x;
    
    double move_x = (std::abs(diff_x) > delta_norm_max) ? 
                    (diff_x > 0 ? delta_norm_max : -delta_norm_max) : diff_x;

    tools::Circle next_circle = circle_;
    next_circle.center.x += move_x;

    bool collision = false;
    if (tools::is_paddle_in_arena(next_circle, arena_size)) {
        for (const auto& b : bricks) {
            if (tools::intersects_Circle_Square(next_circle, b->get_bounds())) {
                collision = true;
                break;
            }
        }
    } else {
        collision = true;
    }

    if (!collision) {
            circle_.center.x = next_circle.center.x;
            delta_ = { circle_.center.x - current_x, 0.0 };
    } else {
            delta_ = {0.0, 0.0};
    }
}