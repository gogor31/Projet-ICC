// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : paddle.cc
// Description : Implémentation des mouvements mécaniques et des validations
//               géométriques du paddle.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526)
// Date : Mai 2026
// ============================================================================

#include <iostream>
#include <cmath>
#include "paddle.h"
#include "tools.h"
#include "constants.h"
#include "message.h"
#include "graphic.h"
#include "brick.h"

using namespace std;

// ==========================================
// CONSTRUCTEURS
// ==========================================

Paddle::Paddle(tools::Circle c) : circle_(c), active(true) {} 

// ==========================================
// ACCESSEURS ET MUTATEURS
// ==========================================

void Paddle::set_x(double x) {
    circle_.center.x = x;
}

void Paddle::set_delta(tools::Point d) {
    delta_ = d;
}

tools::Circle Paddle::get_future_circle(double x) const {
    tools::Circle future = circle_;
    future.center.x = x;
    return future;
}

// ==========================================
// VÉRIFICATIONS ET LIMITES
// ==========================================

bool Paddle::check() const {
    if(!active) return true;
    
    if (!tools::is_paddle_in_arena(circle_, arena_size)) {
        std::cout << message::paddle_outside(circle_.center.x, circle_.center.y);
        return false;
    }
    return true;
}

bool Paddle::check_position(double next_x) const {
    tools::Circle future_circle = circle_;
    future_circle.center.x = next_x;
    return tools::is_paddle_in_arena(future_circle, arena_size);
}

// ==========================================
// DYNAMIQUE ET SIMULATION
// ==========================================

void Paddle::move(double target_x, const std::vector<std::unique_ptr<Brick>>& bricks) {
    if (!active) return;

    const double current_x = circle_.center.x;
    double move_x = target_x - current_x;
    
    // Limitation de la vitesse maximale du paddle
    move_x = tools::clamp(move_x, -delta_norm_max, delta_norm_max);
    
    double next_x = current_x + move_x;

    // Calcul de la demi-largeur de l'intersection du disque avec la base de l'arène
    double dy = std::abs(circle_.center.y);
    double val = (circle_.radius * circle_.radius) - (dy * dy);
    double semi_width = std::sqrt(std::max(0.0, val));
    
    // Contraint le paddle à rester à l'intérieur des murs latéraux
    next_x = tools::clamp(next_x, semi_width, arena_size - semi_width);

    tools::Circle next_circle = circle_;
    next_circle.center.x = next_x;

    // Détection d'une collision bloquante avec une brique
    bool collision_brick = false;
    for (const auto& b : bricks) {
        if (tools::intersects_Circle_Square(next_circle, b->get_bounds(), tools::epsil_zero)) {
            collision_brick = true;
            break;
        }
    }

    // Application du mouvement ou calcul par dichotomie en cas d'obstacle
    if (!collision_brick) {
        circle_.center.x = next_circle.center.x;
        delta_ = { move_x, 0.0 }; 
    } else {
        // Recherche dichotomique pour approcher la brique au plus près sans intersection
        double low = 0.0;
        double high = move_x;
        
        for (int i = 0; i < 5; ++i) { 
            double mid = low + (high - low) * 0.5;
            next_circle.center.x = current_x + mid;
            
            bool hit = false;
            for (const auto& b : bricks) {
                if (tools::intersects_Circle_Square(next_circle, b->get_bounds(), tools::epsil_zero)) {
                    hit = true;
                    break;
                }
            }
            
            if (hit) {
                high = mid; 
            } else {
                low = mid;  
            }
        }
        
        circle_.center.x = current_x + low;
        delta_ = { 0.0, 0.0 };
    }
}

// ==========================================
// AFFICHAGE
// ==========================================

void Paddle::draw() const {
    if (!active) return;

    graphic::draw_arc(circle_.center.x, circle_.center.y, circle_.radius, graphic::BLACK);
}