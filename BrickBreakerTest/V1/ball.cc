#include <iostream>
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "ball.h"

//constructeur, initie avec une position default et un index propre a l'instance
Ball::Ball() : pos_x(arena_size/2), pos_y(arena_size/4) {++ball_index;}

Circle Ball::get_circle_next(){//cercle de la balle a la prochaine (potentielle position)
    return Circle((pos_x + delta_x,pos_y + delta_y), getRadius());
}

/*
Ball::~Ball() {destroy();}

void Ball::destroy(){delete static_cast<Ball*>;}
*/


/*
bool Ball::collisionBall(Ball B){
    //les cercles des 2 balles a leurs prochaines (potentielles) positions
    Circle cA((pos_x + delta_x,pos_y + delta_y), getRadius());
    Circle cB((B.getPos_x() + B.getDelta_x(),B.getPos_y() + B.getDelta_y()), B.getRadius());

    if (intersects_Circle_Circle(cA,cB)){
        std::cout << message::collision_balls(ball_index,B.getIndex()) << std::endl;
        return true;
    } else {return false;}
}
*/

/*
bool Ball::collsionBrick(Brick B){
    //le cercle de la balle a s prochaine (potentielle) position
    Circle cA((pos_x + delta_x,pos_y + delta_y), getRadius());
    if (intersects_Circle_Square(cA,B.get_bounds())){
         std::cout << message::collision_ball_brick(ball_index,/*brick_index) << std::endl; 
         return true;
    } else {return false;}
}
*/
/*
bool Ball::collsionPaddle(Paddle P){
    Circle cA((pos_x + delta_x,pos_y + delta_y), getRadius());
    Circle cP(P.get_circle());
    if (intersects_Circle_Circle(cA,cP)){
        std::cout << message::collision_balls(ball_index,0) << std::endl;//le paddle a pas d'index dcp je met 0
        return true;
    } else {return false;}
}
*/

bool Ball::out_of_bounds(){
    bool trop_haut(pos_y - C.getRadius() > arena_size);
    bool trop_bas(pos_y < 0);
    bool trop_gauche(pos_x + C.getRadius() < 0);
    bool trop_droite(pos_x - C.getRadius() < arena_size);
    if (trop_haut or trop_bas or trop_gauche or trop_droite){
        std::cout << message::ball_outside(pos_x,pos_y) << std::endl;
        return true;
        } else {return false;}
}
