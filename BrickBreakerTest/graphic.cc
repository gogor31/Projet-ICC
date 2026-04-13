#include <iostream>
#include <cairomm/context.h>
#include "graphic.h"
#include "graphic_gui.h"
#include "constants.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

void set_color(graphic::Color color)
{
    double r(0.), g(0.), b(0.);
    switch (color){
        case graphic::RED:    r = 1.0; break;
        case graphic::ORANGE: r = 1.0; g = 0.5; break;
        case graphic::YELLOW: r = 1.0; g = 1.0; break;
        case graphic::GREEN:  g = 1.0; break;
        case graphic::CYAN:   g = 1.0; b = 1.0; break;
        case graphic::BLUE:   b = 1.0; break;
        case graphic::PURPLE: r = 0.5; b = 1.0; break;
        case graphic::BLACK:  r = g = b = 0.0; break;
        case graphic::GREY:   r = g = b = 0.5; break;
        case graphic::WHITE:  r = g = b = 1.0; break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr){
    ptcr = &cr;
}

namespace graphic {
    void draw_arena() {
        set_color(WHITE); // Fond [cite: 267]
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->fill();
        
        set_color(GREY); // Bordure [cite: 268]
        (*ptcr)->set_line_width(0.5); 
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->stroke();
    }
    void draw_square(double x, double y, double size, Color color) {
        set_color(color);
        (*ptcr)->rectangle(x - size/2.0, y - size/2.0, size, size);
        (*ptcr)->fill();
    }
    void draw_circle(double x, double y, double radius, Color color, bool filled) {
        set_color(color);
        (*ptcr)->arc(x, y, radius, 0, 2 * M_PI);
        if (filled) {
            (*ptcr)->fill();
        } else {
            (*ptcr)->stroke();}
    }
    void draw_arc(double x, double y, double radius, Color color) {
        set_color(color);
        (*ptcr)->set_line_width(1.0); 
        (*ptcr)->arc(x, y, radius, M_PI, 2 * M_PI);
        (*ptcr)->stroke();
    }
}


