#include <iostream>
#include "brick.h"
#include "constants.h"
#include "tools.h"
#include "message.h"

using namespace std;


bool is_square_in_arena(double x, double y, double side) {
    double half = side / 2.0;
    bool hors_gauche = (x - half) < 0.0;
    bool hors_droite = (x + half) > arena_size;
    bool hors_haut   = (y - half) < 0.0;
    bool hors_bas    = (y + half) > arena_size;
    
    return (hors_gauche || hors_droite || hors_haut || hors_bas);
}



bool Brick::check() const {
    if (bounds_.side < brick_size_min) {
        cout << message::invalid_brick_size(bounds_.side);
        return false;
    }

    if (is_square_in_arena(bounds_.center.x, bounds_.center.y, bounds_.side)) {
        cout << message::brick_outside(bounds_.center.x, bounds_.center.y);
        return false;
    }
    return true;
}

bool RainbowBrick::check() const {
    if (!Brick::check()) {
        return false;
    } 
    
    if (hits_points_ < 1 || hits_points_ > 7) {
        cout << message::invalid_hit_points(hits_points_);
        return false;
    }
    return true;
}

void Rainbow_brick::draw() const {
    graphic::Color c = static_cast<graphic::Color>(hit_points_ - 1);
    bounds_.draw(c); 
}

void Ball_brick::draw() const {
    bounds_.draw(graphic::RED);
    Circle ball_img = {bounds_.center, new_ball_radius};
    ball_img.draw(graphic::BLACK);
}

void Split_brick::draw() const {
    bounds_.draw(graphic::RED); // Fond rouge
    
    double small_side = (bounds_.side - split_brick_gap) / 2.0;

    if (small_side >= brick_size_min) {
        double offset = (small_side + split_brick_gap) / 2.0;
        
        double corners_x[4] = {bounds_.center.x - offset, bounds_.center.x + offset, 
                               bounds_.center.x - offset, bounds_.center.x + offset};
        double corners_y[4] = {bounds_.center.y - offset, bounds_.center.y - offset, 
                               bounds_.center.y + offset, bounds_.center.y + offset};

        for (int i = 0; i < 4; ++i) {
            Square small_sq = {{corners_x[i], corners_y[i]}, small_side};
            small_sq.draw(graphic::ORANGE);
        }
    }
}
