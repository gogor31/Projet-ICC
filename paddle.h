#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

class Paddle{
public:
    Paddle() = default;
    Paddle(Circle c);

    bool check() const;
    const Circle& get_circle() const { return circle_; }
    
private : 
    Circle circle_ = {{0.0, 0.0}, 0.0};
};

#endif

