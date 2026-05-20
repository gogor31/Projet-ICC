#ifndef GRAPHIC_H
#define GRAPHIC_H

namespace graphic {

    // ==========================================
    // ENUMERATION DES COULEURS DU JEU
    // ==========================================

    enum Color
    {
        RED,
        ORANGE,
        YELLOW,
        GREEN,
        CYAN,
        BLUE,
        PURPLE,
        BLACK,
        GREY,
        WHITE,
    };

    // ==========================================
    // CONFIGURATION ET INITIALISATION DU CANVAS
    // ==========================================

    void graphic_prepare_canvas(double width, double height);

    // ==========================================
    // FONCTIONS DE DESSIN DES ENTITÉS
    // ==========================================

    void draw_arena();
    
    void draw_square(double x, double y, double size, Color color);
    
    void draw_circle(double x, double y, double radius, Color color, bool filled = true);
    
    void draw_arc(double x, double y, double radius, Color color);
}

#endif