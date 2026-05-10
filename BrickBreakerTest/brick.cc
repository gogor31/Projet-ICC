#include <iostream>
#include "brick.h"
#include "constants.h"
#include "tools.h"
#include "message.h"

using namespace std;


bool is_square_outside_arena(double x, double y, double side) {
    double half = side / 2.0;
    bool hors_gauche = (x - half) < -tools::epsil_zero;
    bool hors_droite = (x + half) > arena_size + tools::epsil_zero;
    bool hors_haut   = (y - half) < -tools::epsil_zero;
    bool hors_bas    = (y + half) > arena_size + tools::epsil_zero;

    return (hors_gauche || hors_droite || hors_haut || hors_bas);
}



bool Brick::check() const {
    if (bounds_.side < brick_size_min) {
        cout << message::invalid_brick_size(bounds_.side);
        return false;
    }

    if (is_square_outside_arena(bounds_.center.x, bounds_.center.y, bounds_.side)) {
        cout << message::brick_outside(bounds_.center.x, bounds_.center.y);
        return false;
    }
    return true;
}

bool RainbowBrick::check() const {
    if (!Brick::check()) {
        return false;
    } 
    
    if (hit_points_ < 1 || hit_points_ > 7) {
        cout << message::invalid_hit_points(hit_points_);
        return false;
    }
    return true;
}

void RainbowBrick::draw() const {
    graphic::Color c = static_cast<graphic::Color>(hit_points_ - 1);
    bounds_.draw(c); 
}

void RainbowBrick::write(std::ostream& out) const {
    // Format : type (0) x y side hit_points 
    out << type_ << " " 
        << bounds_.center.x << " " 
        << bounds_.center.y << " " 
        << bounds_.side << " " 
        << hit_points_ << std::endl;
}

void BallBrick::draw() const {
    bounds_.draw(graphic::RED);
    tools::Circle ball_img = {bounds_.center, new_ball_radius};
    ball_img.draw(graphic::BLACK);
}

void BallBrick::write(std::ostream& out) const {
    // Format : type (1) x y side 
    out << type_ << " " 
        << bounds_.center.x << " " 
        << bounds_.center.y << " " 
        << bounds_.side << std::endl;
}

SplitBrick::SplitBrick(tools::Square s, int hp) : Brick(s, 2) {
    if (hp != -1) {
        hit_points_ = hp;
    } else {
        int levels = 1;
        double current_side = s.side;
        
        while (((current_side - split_brick_gap) / 2.0) >= brick_size_min) {
            levels++;
            current_side = (current_side - split_brick_gap) / 2.0;
        }
        hit_points_ = levels; 
    }
}

void SplitBrick::draw() const {
    draw_recursive(bounds_, hit_points_);
}

void SplitBrick::draw_recursive(tools::Square s, int hp) const {
    int depth = hit_points_ - hp;
    
    graphic::Color current_color = static_cast<graphic::Color>(depth % 7);
    
    s.draw(current_color);

    double small_s = (s.side - split_brick_gap) / 2.0;

    if (small_s >= brick_size_min && hp > 1) {
        double offset = (small_s + split_brick_gap) / 2.0;

        draw_recursive({{s.center.x - offset, s.center.y - offset}, small_s}, hp - 1);
        draw_recursive({{s.center.x + offset, s.center.y - offset}, small_s}, hp - 1);
        draw_recursive({{s.center.x - offset, s.center.y + offset}, small_s}, hp - 1);
        draw_recursive({{s.center.x + offset, s.center.y + offset}, small_s}, hp - 1);
    }
}

void SplitBrick::write(std::ostream& out) const {
    out << type_ << " " 
        << bounds_.center.x << " " 
        << bounds_.center.y << " " 
        << bounds_.side << std::endl;
}
