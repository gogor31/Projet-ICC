#ifndef TOOLS_H
#define TOOLS_H

#include <gtkmm.h>
#include "graphic.h"
#include "constants.h"
#include <cairomm/context.h>

namespace tools {    
    constexpr double epsil_zero = 0.125;

    struct Point {
        double x;
        double y;
        
        Point operator+(const Point& other) const {return {x + other.x, y + other.y};}
    };

    struct Square {
        Point center;
        double side;

        void draw(graphic::Color color) const;
    };

    struct Circle {
        Point center;
        double radius;

        void draw(graphic::Color color, bool filled = true) const;
    };
    
    // --- Fonctions utilitaires ---

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
    
    bool is_paddle_in_arena(const Circle& c, double square_side);

    double norm(const Point& p);

    // Normalise un vecteur (le ramène à une norme de 1)
    Point normalize(const Point& p);

    // Calcule le produit scalaire entre deux vecteurs
    double scalaire(const Point& v1, const Point& v2);

    // Réflexion d'un vecteur v par rapport à une normale n
    Point reflect(const Point& v, const Point& n);

    // Calcule la direction nominale pour un rebond sur un carré
    Point compute_nominal_direction(const Circle& c, const Square& s);

    // Calcule le nouveau delta après un choc élastique
    Point compute_impulse(const Point& d1, double r1, const Point& c1,
                         const Point& d2, double r2, const Point& c2);

    Point compute_impulse_paddle(const Point& d_ball, const Point& c_ball,
                             const Point& d_paddle, const Point& c_paddle);
                             
    void resolve_overlap(Point& p1, double r1, Point& p2, double r2);

}
#endif