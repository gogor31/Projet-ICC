#include <iostream>
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "ball.h"

Ball::Ball() : pos_x(arena_size/2), pos_y(arena_size/4){}

/*
Ball::~Ball() {destroy();}

void Ball::destroy(){delete static_cast<Ball*>;}
*/

bool Ball::collisionBall(Ball B){
    //les cercles des 2 balles a leurs prochaines (potentielles) positions
    Circle cA((pos_x + delta_x,pos_y + delta_y), getRadius());
    Circle cB((B.getPos_x() + B.getDelta_x(),B.getPos_y() + B.getDelta_y()), B.getRadius());

    if (intersects_Circle_Circle(cA,cB)){
        //std::cout << message::collision_balls(cA,cB) //euh att
        return true;
    } else {return false;}

}
