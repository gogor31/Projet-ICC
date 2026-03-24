#ifndef BALL_H
#define BALL_H

//jsp encore si tout les includes sont necessaires
#include <vector>
#include <string>
#include "tools.h"

class Ball{
    private:
        //Constructeur par défaut
        Ball();

        //Destructeur pour libérer la mémoire
        ~Ball();

        //efface la balle
        void destroy();

/*

        //collision Balle-Balle
        bool collisionBall(Ball B);

        //collision Balle-Brique
        bool collsionBrick(Brick B);

        //collision Balle-Paddle
        bool collsionPaddle(Paddle P);

        bool out_of_bounds();

*/
        /*
        void update_pos();
        void bounce();
        */

        //attributs
        Circle C;

        static int ball_index;

        double pos_x = C.getCenter().getX();
        double pos_y = C.getCenter().getY();

        double delta_x;
        double delta_y;
        
        std::array <double, 2> delta_norm_max;
        int nb_bounce_max = 10;
    public:
        //fonctions get
        double getPos_x(){return C.getCenter().getX();}
        double getPos_y(){return C.getCenter().getY();}
        double getRadius(){return C.getRadius();}
        int getIndex(){return ball_index;}

        double getDelta_x(){return delta_x;}
        double getDelta_y(){return delta_y;}

        Circle get_circle_next();

        bool out_of_bounds();

        //gestion de rebonds simples
        void change_dir(double coord);
};

int Ball::ball_index = 0;

#endif