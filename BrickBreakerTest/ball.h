/*
 * Fichier: ball.h
 * Auteur:  Ilhan Salvatore Legio
 */

#ifndef BALL_H
#define BALL_H

#include "tools.h"

namespace brickbreaker { // Namespace nommé (O31.2)

    class Ball {
    public:
        Ball(Circle c, Point d);

        const Circle& get_circle() const { return circle_; }
        const Point& get_delta() const { return delta_; }

        // Vérifie la validité de la balle (vitesse et position)
        bool check() const;

    private:
        Circle circle_; 
        Point  delta_;  
    };

} // namespace brickbreaker

#endif