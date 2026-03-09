#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

using namespace std;

class Paddle{
public:
    Paddle(Circle c);

    const Circle& get_circle() const { return circle_; } //renvoie de référence

private : 
    Circle circle_;
};

#endif

