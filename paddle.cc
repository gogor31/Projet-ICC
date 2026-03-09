#include "paddle.h"
#include "constants.h"
#include "message.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Paddle::Paddle(Circle c, int r) : circle_(c), radius_(r)
{

    // Vérification de la position y
    if ((c.center.y > 0) || (c.center.y + r <= 0))
    {
        cout << paddle_outside(c.center.x, c.center.y) << endl;
        exit(FAILURE);
    }
    // Vérification de la position x PAS JUSTE 
    if (c + r < 0 || c + r > arena_size){ //Verif du centre et pas des intersection 
        cout<<MES_ERR;
        exit(FAILURE);
    }
}