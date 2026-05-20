#include <iostream>
#include <cairomm/context.h>
#include <cmath>
#include "graphic.h"
#include "graphic_gui.h"
#include "constants.h"

using namespace std;
using namespace graphic;

// ==========================================
// VARIABLES STATIQUES ET CONTEXTE CAIRO
// ==========================================

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

static double current_scale = 1.0; 

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr){
    ptcr = &cr;
}

// ==========================================
// CONFIGURATION ET INITIALISATION DU CANVAS
// ==========================================

void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

void graphic_prepare_canvas(double width, double height) {
    if (!ptcr) return;

    double side = std::min(width, height);
    current_scale = side / arena_size; 

    (*ptcr)->translate((width - side) * 0.5, (height - side) * 0.5); 
    (*ptcr)->scale(current_scale, -current_scale);
    (*ptcr)->translate(0, -arena_size); 
}

// ==========================================
// FONCTIONS DE DESSIN DES ENTITÉS
// ==========================================

namespace graphic {

    void draw_arena() {
        if (!ptcr) return;

        set_color(WHITE); 
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->fill();
        
        set_color(GREY); 
        (*ptcr)->set_line_width(0.5 / current_scale); 
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->stroke();
    }

    void draw_square(double x, double y, double size, Color color) {
        if (!ptcr) return;
        set_color(color);

        double top_left_x = x - (size * 0.5);
        double top_left_y = y - (size * 0.5);
        (*ptcr)->rectangle(top_left_x, top_left_y, size, size);
        (*ptcr)->fill();
    }

    void draw_circle(double x, double y, double radius, Color color, bool filled) {
        if (!ptcr) return;
        set_color(color);

        (*ptcr)->arc(x, y, radius, 0, 2 * M_PI);

        if (filled) {
            (*ptcr)->fill();
        } else {
            (*ptcr)->stroke();
        }
    }

    void draw_arc(double x, double y, double radius, Color color) { 
        if (!ptcr) return;

        (*ptcr)->save(); 

        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->clip();

        double lw = 1.0 / current_scale; 
        (*ptcr)->set_line_width(lw);

        double visual_radius = radius - (lw / 2.0);

        set_color(color);

        (*ptcr)->arc(x, y, visual_radius, 0, 2 * M_PI);
        (*ptcr)->stroke();

        (*ptcr)->restore(); 
    }
}