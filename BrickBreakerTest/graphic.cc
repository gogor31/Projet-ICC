// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : graphic.cc
// Description : Implémentation des primitives Cairo, gestion des pointeurs 
//               de contexte statiques et normalisation de l'épaisseur des traits.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

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

// Pointeur vers le contexte actif maintenu par GTK
static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// Facteur d'échelle mémémorisé pour adapter dynamiquement l'épaisseur des contours textuels
static double current_scale = 1.0; 

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr){
    ptcr = &cr;
}

// ==========================================
// CONFIGURATION ET INITIALISATION DU CANVAS
// ==========================================

// Traduit l'énumération métier Color en canaux RGB natifs pour le moteur Cairo
void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:    r = 1.0;             break;
    case ORANGE: r = 1.0; g = 0.5;    break;
    case YELLOW: r = 1.0; g = 1.0;    break;
    case GREEN:  g = 1.0;             break;
    case CYAN:   g = 1.0; b = 1.0;    break;
    case BLUE:   b = 1.0;             break;
    case PURPLE: r = 0.5; b = 1.0;    break;
    case BLACK:  r = g = b = 0.0;     break;
    case GREY:   r = g = b = 0.5;     break;
    case WHITE:  r = g = b = 1.0;     break;
    default:                          break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

void graphic_prepare_canvas(double width, double height) {
    if (!ptcr) return;

    double side = std::min(width, height);
    current_scale = side / arena_size; 

    // Aligne le repère cartésien au centre et inverse l'axe vertical Y pour correspondre au modèle physique
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
        // Compensation d'échelle pour garantir une épaisseur de trait uniforme à l'écran
        (*ptcr)->set_line_width(0.5 / current_scale); 
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->stroke();
    }

    void draw_square(double x, double y, double size, Color color) {
        if (!ptcr) return;
        set_color(color);

        // centre de masse vers coin supérieur-gauche Cairo
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

        // Restreint le tracé aux frontières strictes de l'arène
        (*ptcr)->rectangle(0, 0, arena_size, arena_size);
        (*ptcr)->clip();

        double lw = 1.0 / current_scale; 
        (*ptcr)->set_line_width(lw);

        // Correction visuelle pour caler précisément le tracé sur le bord
        double visual_radius = radius - (lw / 2.0);

        set_color(color);

        (*ptcr)->arc(x, y, visual_radius, 0, 2 * M_PI);
        (*ptcr)->stroke();

        (*ptcr)->restore(); 
    }
}