#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

class Paddle {
public:
    Paddle() = default;
    Paddle(tools::Circle c);

    // Vérifie la validité des paramètres de la raquette
    bool check() const;

    const tools::Circle& get_circle() const { return circle_; }

    //double calculer_delta_x(circle_.radius, circle_.center.x);

private:
    tools::Circle circle_ = {{0.0, 0.0}, 0.0}; // Position et rayon de la raquette
    double delta_x;
    double delta_y;
};


#endif

