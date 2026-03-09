#include <iostream>
#include "tools.h"
using namespace std;

class Ball{
    private:
        array <double, 2> pos;
        double rayon;
        array <double, 2> delta;
        array <double, 2> delta_norm_max;
        int nb_bounce_max = 10;

        void change_dir(double coord){coord = -coord;}

    public:
        array <double, 2> getPos(){return pos;}

        double getRayon(){return rayon;}

        array <double, 2> getDelta(){return delta;}

        //spawn une ball
        void spawn(){;}

        //detruit la ball
        void destroy(){/*destruction*/}



        //maj la position
        void update_pos(){

            //1: collision avec mur ou brique

            //2: collision avec le paddle

            //3: collision avec balle

            //4: ball trop basse
            if (pos[1] <= 0) {destroy(); return;}

            //veilles pos + nouvelle pos (delta)
            pos[0] += delta[0];
            pos[1] += delta[1];
        }
};