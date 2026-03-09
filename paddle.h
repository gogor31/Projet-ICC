#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

using namespace std;

class Paddle{
public:
    Paddle(Circle c, int r) : circle_(c), radius_(r) {}

    Circle get_circle_() const { return circle_; }
    int get_radius_() const { return radius_; }

private : 
    Circle circle_;
    int radius_;
};

#endif

