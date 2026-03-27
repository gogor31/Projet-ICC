/*
 * Fichier: ball.h
 * Auteur:  Ilhan Salvatore Legio
 */

#ifndef BALL_H
#define BALL_H

#include "tools.h"

namespace brickbreaker { // Namespace nommé (O31.2)

    class Ball {
    public:
        Ball(Circle c, Point d);

        
        // Vérifie la validité de la balle (vitesse et position)
        bool check() const;

        //fonctions get
        double getPos_x(){return pos_x;}
        double getPos_y(){return pos_y;}
        double getRadius(){return circle_.radius;}

        Circle get_circle_next();
        const Circle& get_circle() const { return circle_; }
        const Point& get_delta() const { return delta_; }


        bool out_of_bounds();

        //gestion de rebonds simples
        void change_dir(double coord);

    private:
        Circle circle_; 
        Point  delta_;


        double pos_x = circle_.center.x;
        double pos_y = circle_.center.y;

        //std::array <double, 2> delta_norm_max;
        int nb_bounce_max = 10;
    };


} // namespace brickbreaker



#endif