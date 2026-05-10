#include <cmath>
#include <algorithm>
#include "tools.h"
#include "graphic.h"

namespace tools {
    void Square::draw(graphic::Color color) const {
        graphic::draw_square(center.x, center.y, side, color);
    }

    void Circle::draw(graphic::Color color, bool filled) const {
        graphic::draw_circle(center.x, center.y, radius, color, filled);
    }

    double distance(const Point& p1, const Point& p2) {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    double calculer_delta_x(double radius, double x_center) {
        double r_sq = radius * radius;
        double x_sq = x_center * x_center;
        double diff = r_sq - x_sq;
        
        double delta_x = 0.0;
        
        if (diff > 0.0) {
            delta_x = std::sqrt(diff);
        } else {
            delta_x = 0.0;
        }
        
        return delta_x;
    }

    bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, double epsilon) {
        double dist_centers = distance(c1.center, c2.center);
        return (dist_centers - (c1.radius + c2.radius)) < epsilon;
    }

    bool intersects_Square_Square(const Square& s1, const Square& s2, double epsilon) {
        double dist_x = std::abs(s1.center.x - s2.center.x);
        double dist_y = std::abs(s1.center.y - s2.center.y);
        double min_dist = (s1.side + s2.side) / 2.0;
        
        return (dist_x - min_dist < epsilon) && (dist_y - min_dist < epsilon);
    }

    bool intersects_Circle_Square(const Circle& c, const Square& s, double epsilon) {
        double half_s = s.side / 2.0;
        
        double closest_x = std::max(s.center.x - half_s, 
                                    std::min(c.center.x, s.center.x + half_s));
        double closest_y = std::max(s.center.y - half_s, 
                                    std::min(c.center.y, s.center.y + half_s));
        
        Point closest_point = {closest_x, closest_y};
        return (distance(c.center, closest_point) - c.radius) < epsilon;
    }

    bool is_circle_in_square(const Circle& c, double square_side) {
        bool hors_limite_x = (c.center.x < c.radius - epsil_zero) || (c.center.x > (square_side - c.radius) + epsil_zero);
        bool hors_limite_y = (c.center.y < -epsil_zero) || (c.center.y > (square_side - c.radius) + epsil_zero);
        
        return !(hors_limite_x || hors_limite_y);
    }

    bool is_paddle_in_arena(const Circle& c, double square_side) {
        
        if (c.center.y > epsil_zero) return false;
        if (c.center.y + c.radius < epsil_zero) return false;

        double dy = std::abs(c.center.y);
        double dx = std::sqrt((c.radius * c.radius) - (dy * dy));

        double x_left  = c.center.x - dx;
        double x_right = c.center.x + dx;

        if (x_left < -epsil_zero || x_right > square_side + epsil_zero) {
            return false;
        }

        return true;
    }

    double norm(const Point& p) {
        double norm = std::sqrt(p.x * p.x + p.y * p.y);
        return norm;
    }

    Point normalize(const Point& p) {
        double n = norm(p);
        if (n < epsil_zero) {
            return {0, 0};
        }
        return {p.x / n, p.y / n};
    }

    double scalaire(const Point& v1, const Point& v2) {
        double scalaire = v1.x * v2.x + v1.y * v2.y;
        return scalaire;
    }

    Point reflect(const Point& v, const Point& n) { //? Utilité de la normale ?
    Point n_norm = normalize(n);
    double dot = scalaire(v, n_norm);
    return {v.x - 2.0 * dot * n_norm.x, v.y - 2.0 * dot * n_norm.y};
    }

    Point compute_nominal_direction(const Circle& c, const Square& s) {
        // Différence réelle entre les centres
        double diff_x = c.center.x - s.center.x;
        double diff_y = c.center.y - s.center.y;

        // Différence bornée à la moitié de la taille du carré
        double half_s = s.side / 2.0;
        double bounded_x = std::max(-half_s, std::min(diff_x, half_s));
        double bounded_y = std::max(-half_s, std::min(diff_y, half_s));

        // Direction nominale = différence réelle - différence bornée
        return {diff_x - bounded_x, diff_y - bounded_y};
    }

    Point compute_impulse(const Point& d1, double r1, const Point& c1,
                                    const Point& d2, double r2, const Point& c2) {
    double dist = distance(c1, c2);
    if (dist < epsil_zero) return d1;

    // Vecteur normal reliant les centres
    Point n = {(c1.x - c2.x) / dist, (c1.y - c2.y) / dist};

    // Vitesse nominale (projection du delta sur la normale) 
    double v1n = scalaire(d1, n);
    double v2n = scalaire(d2, n);

    // Formule de l'impulsion corrigée par les masses 
    double m1 = r1 * r1;
    double m2 = r2 * r2;
    double impulse_mag = (-v1n + v2n) * (2.0 * m2) / (m1 + m2);

    return {d1.x + impulse_mag * n.x, d1.y + impulse_mag * n.y};
    }

    Point compute_impulse_paddle(const Point& d_ball, const Point& c_ball,
                                const Point& d_paddle, const Point& c_paddle) {
        double dist = distance(c_ball, c_paddle);
        if (dist < epsil_zero) return d_ball;

        Point n = {(c_ball.x - c_paddle.x) / dist, (c_ball.y - c_paddle.y) / dist};

        double v1n = scalaire(d_ball, n);
        double v2n = scalaire(d_paddle, n);
                
        double impulse_mag = 2.0 * (-v1n + v2n);

        return {d_ball.x + impulse_mag * n.x, d_ball.y + impulse_mag * n.y};
    }

    void resolve_overlap(Point& p1, double r1, Point& p2, double r2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        double distance = std::sqrt(dx*dx + dy*dy);
        double min_dist = r1 + r2;

        if (distance < min_dist && distance > 0) {
            double overlap = min_dist - distance + epsil_zero;
            
            // Vecteur unitaire de direction (de p2 vers p1)
            double nx = dx / distance;
            double ny = dy / distance;

            // On déplace chaque point de la moitié de l'overlap
            p1.x += nx * (overlap / 2.0);
            p1.y += ny * (overlap / 2.0);
            p2.x -= nx * (overlap / 2.0);
            p2.y -= ny * (overlap / 2.0);
        }
    }


}