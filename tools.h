#ifndef TOOLS_H
#define TOOLS_H

constexpr double epsil_zero = 0.125;

struct Point {
    double x;
    double y;

    double getX(){return x;}
    double getY(){return y;}
};

struct Square {
    Point center;
    double side;
};

struct Circle {
    Point center;
    double radius;

    Point getCenter(){return center;}
};

// Calcul de distance entre deux points
double distance(Point p1, Point p2);

// Vérifie si un point est contenu dans un cercle
bool is_point_in_circle(Point p, Circle c);

// Vérifie si un point est contenu dans un carré
bool is_point_in_square(Point p, Square s);

// Détection de collisions (intersections)
bool intersects(Circle c1, Circle c2);
bool intersects(Square s1, Square s2);
bool intersects(Circle c, Square s);

#endif
