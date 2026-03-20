#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

using namespace std; //a supprimer pour ne pas se melanger

class Paddle{
public:
    Paddle(Circle c = {{0, 0}, 0});
    bool check() const;

    const Circle& get_circle() const { return circle_; }
    
private : 
    Circle circle_;
};

#endif

