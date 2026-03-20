#include <iostream>
#include "tools.h"
#include "constants.h"
using namespace std;

class Ball{
    private:
        Circle C;

        double delta_x;
        double delta_y;
        
        array <double, 2> delta_norm_max;
        int nb_bounce_max = 10;

        void change_dir(double coord){coord = -coord;}

        bool collisionBall(Ball B){
            /*si aux prochaines positions,
            les cercles des balles sont en intersection
            return true*/

        }

    public:
        /*double getRayon(){return rayon;}
        double getPos_x(){return pos_x;}
        double getPos_y(){return pos_y;}*/
        


        double getDelta_x(){return delta_x;}
        double getDelta_y(){return delta_y;}
        Circle getCircle(){return C;}


        //spawn une ball
        void spawn(){}

        //detruit la ball
        void destroy(){/*destruction*/}



        //maj la position
        void update_pos(){

            /*ignore ça

            double pos_x = C.getCenter().getX();
            double pos_y = C.getCenter().getY();
            //--- Collisions Arene ---
            //Toit
            if (pos_y + delta_y >= arena_size - epsil_zero){change_dir(delta_y);}
            //Murs
            if ((pos_x + delta_x <= epsil_zero) or (pos_x + delta_x >= arena_size + epsil_zero))
            {change_dir(delta_x);}
            */
           
            

           
            
            

            //ball trop basse
            if (pos_y <= 0) {destroy();} 
            else{
                //veilles pos + delta = nouvell pos
                pos_x += delta_x;
                pos_y += delta_y;
            }

            
        }
};