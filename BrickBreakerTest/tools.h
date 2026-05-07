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

    class Clock{
    public:
        Clock();
        virtual ~Clock() = default;

        bool tick();

        double elapsed_seconds() const;
        void reset();
        // pause/play des ticks
        void pause()  { timeout_conn.block(); }
        void resume() { timeout_conn.unblock(); }

    private:
        Glib::Timer timer;
        sigc::connection timeout_conn;
        double elapsed_sec;
        int dt_ = dt;
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
}
#endif