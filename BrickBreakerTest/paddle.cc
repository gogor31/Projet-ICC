#include <iostream>
#include <cmath>
#include "paddle.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "graphic.h"

using namespace std;

Paddle::Paddle(tools::Circle c) : circle_(c), active(true) {} 

// Vérifie si la raquette est à l'intérieur de l'arène et touche l'axe X
bool Paddle::check() const {
    if(!active) return true;
    
    // Validation de la position verticale (doit intercepter y=0)
    if (circle_.center.y > tools::epsil_zero || (circle_.center.y + circle_.radius) <= tools::epsil_zero) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    // 3. Vérification des limites de l'arène [0, arena_size]
    //double delta_x = calculer_delta_x(circle_.radius, circle_.center.x);
    double dy = std::abs(circle_.center.y);
    double dx = std::sqrt(circle_.radius * circle_.radius - dy * dy);

    double x1 = circle_.center.x - dx;
    double x2 = circle_.center.x + dx;

    // On décompose la condition complexe en booléens explicites (P61)
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
