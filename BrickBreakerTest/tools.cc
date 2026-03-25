#include <cmath>
#include <algorithm>
#include "tools.h"


double distance(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

// 1. Intersection Cercle-Cercle
bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, double epsilon) {
    double dist_centers = distance(c1.center, c2.center);
    return dist_centers - (c1.radius + c2.radius) < epsilon;
}

// 2. Intersection Carré-Carré
bool intersects_Square_Square(const Square& s1, const Square& s2, double epsilon) {
    double dist_x = std::abs(s1.center.x - s2.center.x);
    double dist_y = std::abs(s1.center.y - s2.center.y);
    double min_dist = (s1.side + s2.side) / 2.0;
    
    return (dist_x - min_dist < epsilon) && (dist_y - min_dist < epsilon);
}

// 3. Intersection Cercle-Carré
bool intersects_Circle_Square(const Circle& c, const Square& s, double epsilon) {
    double half_s = s.side / 2.0;
    
    // Projeter le centre du cercle sur le carré (Clamping)
    double closest_x = std::max(s.center.x - half_s, 
                                std::min(c.center.x, s.center.x + half_s));
    double closest_y = std::max(s.center.y - half_s, 
                                std::min(c.center.y, s.center.y + half_s));
    
    Point closest_point = {closest_x, closest_y};
    return distance(c.center, closest_point) - c.radius < epsilon;
}

// Vérifie si un cercle est entièrement contenu dans un carré
bool is_circle_in_square(const Circle& c, double square_side) {
    // Le centre doit être à au moins 'radius' de chaque bord
    if (c.center.x < c.radius || c.center.x > (square_side - c.radius)) return false;
    if (c.center.y < c.radius || c.center.y > (square_side - c.radius)) return false;
    return true;
}