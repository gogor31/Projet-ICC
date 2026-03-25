/*
 * Fichier: paddle.cc
 * Auteur:  Ilhan Salvatore Legio
 */

#include <cmath>
#include <iostream>
#include "paddle.h"
#include "constants.h"
#include "message.h"

namespace { // Unnamed namespace (O12-O15)

    // Calcule l'écart horizontal entre le centre et l'intersection avec l'axe X
    double calculer_delta_x(double radius, double y_center) {
        double r_sq = radius * radius;
        double y_sq = y_center * y_center;
        double diff = r_sq - y_sq;
        
        double delta_x = 0.0;
        
        if (diff > 0.0) {
            delta_x = std::sqrt(diff);
        } else {
            // Cas où le cercle est tangent ou ne touche pas l'axe
            delta_x = 0.0;
        }
        
        return delta_x;
    }

} // namespace

namespace brickbreaker {
    Paddle::Paddle(Circle c) : circle_(c) {} 

    // Vérifie si la raquette est à l'intérieur de l'arène et touche l'axe X
    bool Paddle::check() const {
        // Validation de la position verticale (doit intercepter y=0)
        if (circle_.center.y > 0.0 || (circle_.center.y + circle_.radius) <= 0) {
            std::cout << message::paddle_outside(circle_.center.x, circle_.center.y);
            return false;
        }

        // 3. Vérification des limites de l'arène [0, arena_size]
        double delta_x = calculer_delta_x(circle_.radius, circle_.center.y);
        
        double x1 = circle_.center.x - delta_x;
        double x2 = circle_.center.x + delta_x;

        // On décompose la condition complexe en booléens explicites (P61)
        bool x1_hors_limites = (x1 < 0.0 || x1 > arena_size);
        bool x2_hors_limites = (x2 < 0.0 || x2 > arena_size);

        if (x1_hors_limites || x2_hors_limites) {
            std::cout << message::paddle_outside(circle_.center.x, circle_.center.y);
            return false;
        }

        return true;
    }
} // namespace brickbreaker