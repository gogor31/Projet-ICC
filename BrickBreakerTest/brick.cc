#include <iostream>
#include "brick.h"
#include "constants.h"
#include "tools.h"
#include "message.h"
#include "graphic.h"

using namespace std;

// ==========================================
// FONCTIONS UTILITAIRES LOCALES
// ==========================================

bool is_square_outside_arena(const tools::Square& s) {
    double half = s.side * 0.5;
    bool out_left   = (s.center.x - half) < -tools::epsil_zero;
    bool out_right  = (s.center.x + half) > arena_size + tools::epsil_zero;
    bool out_bottom = (s.center.y - half) < -tools::epsil_zero;
    bool out_top    = (s.center.y + half) > arena_size + tools::epsil_zero;

    return (out_left || out_right || out_top || out_bottom);
}

// ==========================================
// MÉTHODES DE LA CLASSE DE BASE (BRICK)
// ==========================================

bool Brick::check() const {
    if (bounds_.side < brick_size_min) {
        cout << message::invalid_brick_size(bounds_.side);
        return false;
    }

    if (is_square_outside_arena(bounds_)) {
        cout << message::brick_outside(bounds_.center.x, bounds_.center.y);
        return false;
    }
    return true;
}

// ==========================================
// MÉTHODES DE RAINBOWBRICK (TYPE 0)
// ==========================================

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
    graphic::draw_square(bounds_.center.x, bounds_.center.y, bounds_.side, c); 
}

void RainbowBrick::write(std::ostream& out) const {
    out << type_ << " " 
        << bounds_.center.x << " " 
        << bounds_.center.y << " " 
        << bounds_.side << " " 
        << hit_points_ << std::endl;
}

// ==========================================
// MÉTHODES DE BALLBRICK (TYPE 1)
// ==========================================

void BallBrick::draw() const {
    graphic::draw_square(bounds_.center.x, bounds_.center.y, bounds_.side, graphic::RED);
    graphic::draw_circle(bounds_.center.x, bounds_.center.y, new_ball_radius, graphic::BLACK, true);
}

void BallBrick::write(std::ostream& out) const {
    out << type_ << " " 
        << bounds_.center.x << " " 
        << bounds_.center.y << " " 
        << bounds_.side << std::endl;
}

// ==========================================
// MÉTHODES DE SPLITBRICK (TYPE 2)
// ==========================================

SplitBrick::SplitBrick(tools::Square s, int hp) : Brick(s, 2) {
    if (hp != -1) {
        hit_points_ = hp;
    } else {
        int levels = 1;
        double current_side = s.side;
        
        while (((current_side - split_brick_gap) * 0.5) >= brick_size_min) {
            levels++;
            current_side = (current_side - split_brick_gap) * 0.5;
        }
        hit_points_ = levels; 
    }
}

void SplitBrick::draw() const {
    draw_recursive(bounds_, hit_points_);
}

void SplitBrick::draw_recursive(tools::Square s, int hp) const {
    int color_idx = (hit_points_ - hp) % 7;
    graphic::Color current_color = static_cast<graphic::Color>(color_idx);
    
    graphic::draw_square(s.center.x, s.center.y, s.side, current_color);

    const double small_s = (s.side - split_brick_gap) * 0.5;

    if (small_s >= brick_size_min && hp > 1) {
        const double offset = (small_s + split_brick_gap) * 0.5;

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