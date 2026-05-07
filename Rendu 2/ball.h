#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "graphic.h"


class Ball {
public:
    Ball(tools::Circle c, tools::Point d);

    // Vérifie la validité de la balle (vitesse et position)
    bool check() const;

    void draw() const;

    // Méthodes getters
    double getPos_x(){return circle_.center.x;}
    double getPos_y(){return circle_.center.y;}
    double getRadius(){return circle_.radius;}
    const tools::Circle& get_circle() const { return circle_; }
    const tools::Point& get_delta() const { return delta_; }
    tools::Circle get_circle_next() const;

private:
    tools::Circle circle_; 
    tools::Point  delta_;
};


#endif