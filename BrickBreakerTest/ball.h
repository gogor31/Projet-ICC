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

    void set_delta(tools::Point new_delta);
    void move();
    void reverse_dx() {delta_.x = -delta_.x;};
    void reverse_dy() {delta_.y = -delta_.y;};
    
    void backup_position();
    void restore_position();

private:
    tools::Circle circle_; 
    tools::Point  delta_;
    tools::Point old_center_;
};


#endif