#include <iostream>
#include <cmath>
#include "paddle.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "graphic.h"

using namespace std;

Paddle::Paddle(tools::Circle c) : circle_(c) {} 

// Vérifie si la raquette est à l'intérieur de l'arène et touche l'axe X
bool Paddle::check() const {
    // Validation de la position verticale (doit intercepter y=0)
    if (circle_.center.y > 0.0 || (circle_.center.y + circle_.radius) <= 0.0) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    // 3. Vérification des limites de l'arène [0, arena_size]
    //double delta_x = calculer_delta_x(circle_.radius, circle_.center.x);
    
    double x1 = circle_.center.x - delta_x;
    double x2 = circle_.center.x + delta_x;

    // On décompose la condition complexe en booléens explicites (P61)
    bool x1_hors_limites = (x1 < 0.0 || x1 > arena_size);
    bool x2_hors_limites = (x2 < 0.0 || x2 > arena_size);

    if (x1_hors_limites || x2_hors_limites) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    return true;
}

void Paddle::draw() const {
    // On dessine l'arc supérieur de la raquette en rouge
    // Note: draw_arc dans ton module graphic s'occupe de la couleur
    graphic::draw_arc(circle_.center.x, circle_.center.y, circle_.radius, graphic::RED);
}
