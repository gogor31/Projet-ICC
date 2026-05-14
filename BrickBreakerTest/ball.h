#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "graphic.h"


class Ball {
public:
    Ball(tools::Circle c, tools::Point d);

    bool check() const;

    void draw() const;

    double getPos_x(){return circle_.center.x;}
    double getPos_y(){return circle_.center.y;}
    double getRadius(){return circle_.radius;}
    const tools::Circle& get_circle() const { return circle_; }
    const tools::Point& get_delta() const { return delta_; }
    tools::Circle get_circle_next() const;

    void set_delta(tools::Point new_delta);
    void move();
    void reverse_dx();
    void reverse_dy();
    
    void backup_position();
    void restore_position();

    void set_center(tools::Point p);
    void mark_as_dead();
    bool is_dead() const;

private:
    tools::Circle circle_; 
    tools::Point  delta_;
    tools::Point old_center_;
    bool dead_ = false;
};


#endif