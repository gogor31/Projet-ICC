/*
 * Fichier: paddle.h
 * Auteur:  Ilhan Salvatore Legio
 */

#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

class Paddle {
public:
    Paddle() = default;
    Paddle(Circle c);

    // Vérifie la validité des paramètres de la raquette
    bool check() const;

    const Circle& get_circle() const { return circle_; }

private:
    Circle circle_ = {{0.0, 0.0}, 0.0}; // Position et rayon de la raquette
};


#endif

