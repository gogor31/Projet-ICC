#include <iostream>
#include <cmath>
#include "paddle.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "graphic.h"

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

    graphic::draw_arc(circle_.center.x, circle_.center.y, circle_.radius, graphic::RED);
}
