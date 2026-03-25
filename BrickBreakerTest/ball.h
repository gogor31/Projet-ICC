#ifndef BALL_H
#define BALL_H

#include "tools.h"

class Ball {
public:
    // Constructeur : initialise la balle avec ses coordonnées, son rayon et son delta
    Ball(Circle c, Point d);

    // Retourne le cercle pour les calculs géométriques externes
    const Circle& get_circle() const { return circle_; }
    const Point& get_delta() const { return delta_; }

    // Vérifie si la balle est valide (vitesse et inclusion)
    bool check() const;

    // Vérifie la collision avec une autre balle
    bool collides_with(const Ball& other) const;

private:
    Circle circle_; 
    Point  delta_;  
};

#endif