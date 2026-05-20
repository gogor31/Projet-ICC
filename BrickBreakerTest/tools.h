#ifndef TOOLS_H
#define TOOLS_H

namespace tools {    
    constexpr double epsil_zero = 0.125;

    // ==========================================
    // STRUCTURES DE DONNÉES
    // ==========================================

    struct Point {
        double x;
        double y;
        
        Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    };

    struct Square {
        Point center;
        double side;
    };

    struct Circle {
        Point center;
        double radius;
    };
    
    // ==========================================
    // UTILITAIRES MATHÉMATIQUES ET SCALAIRES
    // ==========================================

    double clamp(double value, double min, double max);

    double norm(const Point& p);

    Point normalize(const Point& p);

    double scalaire(const Point& v1, const Point& v2);

    void clamp_vector(Point& v, double max_norm);

    // ==========================================
    // DISTANCES ET GÉOMÉTRIE INFÉRIEURE
    // ==========================================

    double distance_carre(const Point& p1, const Point& p2);

    double distance(const Point& p1, const Point& p2);

    // ==========================================
    // DÉTECTION DES INTERSECTIONS (COLLISIONS)
    // ==========================================

    bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, double epsilon = epsil_zero);
                                    
    bool intersects_Square_Square(const Square& s1, const Square& s2, double epsilon = epsil_zero);
                                    
    bool intersects_Circle_Square(const Circle& c, const Square& s, double epsilon = epsil_zero);

    // ==========================================
    // VALIDATION DES LIMITES DE L'ARÈNE
    // ==========================================

    bool is_circle_in_square(const Circle& c, double square_side);
    
    bool is_paddle_in_arena(const Circle& c, double square_side);

    // ==========================================
    // PHYSIQUE, RÉFLEXION ET IMPULSIONS
    // ==========================================

    Point reflect(const Point& v, const Point& n);

    Point compute_nominal_direction(const Circle& c, const Square& s);

    Point compute_impulse(const Point& d1, double r1, const Point& c1,
                          const Point& d2, double r2, const Point& c2);

    Point compute_impulse_paddle(const Point& d_ball, const Point& c_ball,
                                 const Point& d_paddle, const Point& c_paddle);
                                 
    void resolve_overlap(Point& p1, double r1, Point& p2, double r2);
}
#endif