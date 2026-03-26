/*
 * Fichier: tools.h
 * Auteur:  Ilhan Salvatore Legio
 */

#ifndef TOOLS_H
#define TOOLS_H

namespace brickbreaker {

    // Constante pour les tolérances de collision (E12, R2)
    constexpr double epsil_zero = 0.125;

    struct Point {
        double x;
        double y;

        Point operator+(Point& other) const {return {x + other.x, y + other.y};}
    };

    struct Square {
        Point center;
        double side;
    };

    struct Circle {
        Point center;
        double radius;
    };

    // --- Fonctions utilitaires exportées (O34) ---

    // Calcul de la distance euclidienne entre deux points
    double distance(const Point& p1, const Point& p2);
    
    // Calcule l'écart horizontal entre le centre et l'intersection avec l'axe X
    double calculer_delta_x(double radius, double x_center);

    // Détection d'intersections avec une marge d'erreur epsilon
    bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, 
                                  double epsilon = epsil_zero);
                                  
    bool intersects_Square_Square(const Square& s1, const Square& s2, 
                                  double epsilon = epsil_zero);
                                  
    bool intersects_Circle_Square(const Circle& c, const Square& s, 
                                  double epsilon = epsil_zero);

    // Vérifie si un cercle est intégralement contenu dans une arène carrée
    bool is_circle_in_square(const Circle& c, double square_side);

} // namespace brickbreaker

#endif