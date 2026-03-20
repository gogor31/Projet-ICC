#include "paddle.h"
#include <cmath>
#include <iostream>
#include "constants.h"
#include "message.h"

using namespace std;

Paddle::Paddle(Circle c) : circle_(c) {} 

bool Paddle::check() const {
    // 1. Vérification de la position y
    if (circle_.center.y > 0.0 || (circle_.center.y + circle_.radius) <= 0) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }

    // 2. Calcul des intersections avec l'axe X (y=0)
    // Formule : x = x_centre ± sqrt(R^2 - y_centre^2) à partir de la formule d'un cercle 
    double r_sq = pow(circle_.radius, 2);
    double y_sq = pow(circle_.center.y, 2);
    double diff = r_sq - y_sq;
    double delta_x = 0.0;
    if (diff > 0.0) {
        delta_x = sqrt(diff);
    } else {
        // Si diff <= 0, soit le cercle est tangent (diff=0), 
        // soit il y a une erreur de précision. On garde delta_x = 0.
        delta_x = 0.0;
    }
    
    double x1 = circle_.center.x - delta_x;
    double x2 = circle_.center.x + delta_x;


    // 3. Vérification des limites de l'arène [0, arena_size]
    if (x1 < 0.0 || x1 > arena_size || x2 < 0.0 || x2 > arena_size ) {
        cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }
    return true;
}