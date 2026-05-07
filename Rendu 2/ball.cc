#include <iostream>
#include "ball.h"
#include "tools.h"
#include "constants.h"
#include "message.h"

using namespace std;

Ball::Ball(tools::Circle c, tools::Point d) : circle_(c), delta_(d) {}

bool Ball::check() const {
    // Validation de la norme de la vitesse (P5, R2)
    // Utilisation du carré de la norme pour éviter sqrt (P61)
    double v2 = delta_.x * delta_.x + delta_.y * delta_.y;
    double max_v2 = delta_norm_max * delta_norm_max;

    if (v2 > max_v2) {
        cout << message::invalid_delta(delta_.x, delta_.y);
        return false;
    }

    // Validation de la position dans l'arène via tools (P3)
    if (!tools::is_circle_in_square(circle_, arena_size)) {
        cout << message::ball_outside(circle_.center.x, 
                                            circle_.center.y);
        return false;
    }

    return true;
}
void Ball::draw() const {
    circle_.draw(graphic::BLACK, true);
}

tools::Circle Ball::get_circle_next() const { //cercle de la balle a la prochaine (potentielle) position
    return tools::Circle{circle_.center + delta_, circle_.radius};
}

