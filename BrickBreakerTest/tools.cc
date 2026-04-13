#include <cmath>
#include <algorithm>
#include "tools.h"
#include "graphic.h"


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
    bool hors_limite_x = (c.center.x < c.radius) || (c.center.x > (square_side - c.radius));
    bool hors_limite_y = (c.center.y < c.radius) || (c.center.y > (square_side - c.radius));
    
    return !(hors_limite_x || hors_limite_y);
}
