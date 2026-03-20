#ifndef TOOLS_H
#define TOOLS_H

constexpr double epsil_zero = 0.125;

struct Point {
    double x;
    double y;

    double getX() const {return x;}
    double getY() const {return y;}
};

struct Square {
    Point center;
    double side;
};

struct Circle {
    Point center;
    double radius;

    Point getCenter() const {return center;}
};

// Calcul de distance entre deux points
double distance(const Point& p1, const Point& p2);

// Détection de collisions (intersections)
bool intersects_Circle_Circle(const Circle& c1, const Circle& c2);
bool intersects_Square_Square(const Square& s1, const Square& s2);
bool intersects_Circle_Square(const Circle& c, const Square& s);

// Vérifie si un point est contenu dans un cercle
bool is_point_in_circle(const Point& p, const Circle& c);

// Vérifie si un point est contenu dans un carré
bool is_point_in_square(const Point& p, const Square& s);


#endif
