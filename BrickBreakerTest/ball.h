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

    //fonctions get
    double getPos_x(){return circle_.center.x;}
    double getPos_y(){return circle_.center.y;}
    double getRadius(){return circle_.radius;}

    tools::Circle get_circle_next() const;
    const tools::Circle& get_circle() const { return circle_; }
    const tools::Point& get_delta() const { return delta_; }


    bool out_of_bounds();

    //gestion de rebonds simples
    void change_dir(double coord);

private:
    tools::Circle circle_; 
    tools::Point  delta_;

};


#endif