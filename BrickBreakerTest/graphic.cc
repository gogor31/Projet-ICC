#include <iostream>
#include <cairomm/context.h>
#include <cmath>
#include "graphic.h"
#include "graphic_gui.h"
#include "constants.h"

using namespace std;
using namespace graphic;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr){
    ptcr = &cr;
}

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

static double current_scale = 1.0; // Variable statique pour mémoriser l'échelle

void graphic_prepare_canvas(double width, double height) {
    if (!ptcr) return;

    double side = std::min(width, height);
    current_scale = side / arena_size; // On mémorise l'échelle ici

    (*ptcr)->translate((width - side) * 0.5, (height - side) * 0.5); 
    (*ptcr)->scale(current_scale, -current_scale);
    (*ptcr)->translate(0, -arena_size); 
}

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
            (*ptcr)->stroke();}
    }

void draw_arc(double x, double y, double radius, Color color) { // AI pour la rectification purement graphique de la raquette
        if (!ptcr) return;

        // On sauvegarde le contexte pour que notre masque ne casse pas le reste du jeu
        (*ptcr)->save(); 

        // 1. LA COUPURE HORIZONTALE : On crée un masque strictly positif (Y >= 0)
        // Cela va couper la raquette de manière parfaitement nette au niveau du sol.
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->clip();

        // 2. CONFIGURATION DU TRAIT
        double lw = 1.0 / current_scale; // Épaisseur du trait de la raquette
        (*ptcr)->set_line_width(lw);

        // 3. L'INNER STROKE (LE SECRET POUR NE PAS DÉBORDER SUR LE MUR)
        // On réduit le rayon de dessin de la moitié de l'épaisseur du trait.
        // Ainsi, le bord extérieur de l'encre correspondra EXACTEMENT au rayon physique (R).
        double visual_radius = radius - (lw / 2.0);

        set_color(color);

        // 4. DESSIN
        // On dessine un cercle entier. 
        // - Le masque (clip) supprimera la moitié basse pour faire l'arc.
        // - L'Inner stroke garantit que les côtés viennent effleurer le mur sans baver.
        (*ptcr)->arc(x, y, visual_radius, 0, 2 * M_PI);
        (*ptcr)->stroke();

        // On restaure le contexte pour annuler le masque
        (*ptcr)->restore(); 
    }
}


