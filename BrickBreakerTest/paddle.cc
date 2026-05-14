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
    
    if (!tools::is_paddle_in_arena(circle_, arena_size)) {
        std::cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }
    return true;
}

bool Paddle::check_position(double next_x) const {
    tools::Circle future_circle = circle_;
    future_circle.center.x = next_x;
    return tools::is_paddle_in_arena(future_circle, arena_size);
}

tools::Circle Paddle::get_future_circle(double x) const {
    tools::Circle future = circle_;
    future.center.x = x;
    return future;
}

void Paddle::draw() const {
    if (!active) return;

    graphic::draw_arc(circle_.center.x, circle_.center.y, circle_.radius, graphic::BLACK);
}

void Paddle::move(double target_x, const std::vector<std::unique_ptr<Brick>>& bricks) {
    if (!active) return;

    const double current_x = circle_.center.x;
    double move_x = target_x - current_x;
    
    move_x = std::clamp(move_x, -delta_norm_max, delta_norm_max);

    tools::Circle next_circle = circle_;
    next_circle.center.x += move_x;

    bool collision = false;

    if (!tools::is_paddle_in_arena(next_circle, arena_size)) {
        collision = true;
    } else {
        for (const auto& b : bricks) {
            if (tools::intersects_Circle_Square(next_circle, b->get_bounds())) {
                collision = true;
                break;
            }
        }
    }

    if (!collision) {
        circle_.center.x = next_circle.center.x;
        delta_ = { move_x, 0.0 }; 
    } else {
        delta_ = { 0.0, 0.0 };
    }
}

void Paddle::set_x(double x) {
    circle_.center.x = x;
}

void Paddle::set_delta(tools::Point d) {
    delta_ = d;
}