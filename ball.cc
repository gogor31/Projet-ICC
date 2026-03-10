#include <iostream>
#include "tools.h"
using namespace std;

class Ball{
    private:
        double rayon;
        double pos_x;
        double pos_y;

        double delta_x;
        double delta_y;
        
        array <double, 2> delta_norm_max;
        int nb_bounce_max = 10;

        void change_dir(double coord){coord = -coord;}

    public:
        double getRayon(){return rayon;}
        double getPos_x(){return pos_x;}
        double getPos_y(){return pos_y;}


        double getDelta_x(){return delta_x;}
        double getDelta_y(){return delta_y;}


        //spawn une ball
        void spawn(){;}

        //detruit la ball
        void destroy(){/*destruction*/}



        //maj la position
        void update_pos(){

            //4: ball trop basse
            if (pos_y <= 0) {destroy(); return;}

            //veilles pos + nouvelle pos (delta)
            pos_x += delta_x;
            pos_y += delta_y;
        }
};