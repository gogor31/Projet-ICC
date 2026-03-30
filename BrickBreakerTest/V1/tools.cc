#include <cmath>
#include <algorithm>
#include "tools.h"

/*
TODO: - Implémenter les fonctions de collision (collisionCS, collisionCC, etc.)
TODO: - Supprimer le namespace std global et utiliser std:: explicitement
TODO: - Vérifier que toutes les fonction ont une utilié
*/
using namespace std; 

double distance(const Point& p1, const Point& p2){
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

// 1. Intersection Cercle-Cercle
bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, double epsilon) {
    double dist_centers = distance(c1.center, c2.center);
    // Distance entre les centres < somme des rayons
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
    
    // Calculer la distance entre le centre du cercle et ce point proche
    Point closest_point = {closest_x, closest_y};
    return distance(c.center, closest_point) - c.radius < epsilon;
}

// Vérifie si un point est contenu dans un cercle
bool is_point_in_circle(const Point& p, const Circle& c, double epsilon) {
    // Un point est dans le cercle si sa distance au centre 
    // est inférieure au rayon (en tenant compte d'epsil_zero)
    return distance(p, c.center) < (c.radius - epsilon);
}

// Vérifie si un point est contenu dans un carré
bool is_point_in_square(const Point& p, const Square& s, double epsilon) {
    double half_side = s.side / 2.0;
    
    double min_x = s.center.x - half_side;
    double max_x = s.center.x + half_side;
    double min_y = s.center.y - half_side;
    double max_y = s.center.y + half_side;

    // Test d'inclusion en resserrant les bords de la valeur d'epsilon
    bool inside_x = (p.x > (min_x + epsilon)) && (p.x < (max_x - epsilon));
    bool inside_y = (p.y > (min_y + epsilon)) && (p.y < (max_y - epsilon));
    
    return inside_x && inside_y;
}
