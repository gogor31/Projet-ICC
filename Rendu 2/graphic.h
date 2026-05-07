#ifndef GRAPHIC_H
#define GRAPHIC_H

namespace graphic {

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

    void draw_arena();
    void draw_square(double x, double y, double size, Color color);
    void draw_circle(double x, double y, double radius, Color color, bool filled = true);
    void draw_arc(double x, double y, double radius, Color color);

}

#endif
