// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : graphic.h
// Description : Bibliothèque d'encapsulation graphique pour l'environnement Cairo.
//               Définit les palettes chromatiques et l'interface de rendu.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

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

    // Configure la matrice de base pour cadrer l'arène logique à l'écran
    void graphic_prepare_canvas(double width, double height);

    // ==========================================
    // FONCTIONS DE DESSIN DES ENTITÉS
    // ==========================================

    // Dessine le fond blanc et la bordure grise délimitant l'arène de jeu
    void draw_arena();
    
    // Rentre un carré plein centré sur (x,y)
    void draw_square(double x, double y, double size, Color color);
    
    // Rentre un disque plein ou évidé centré sur (x,y)
    void draw_circle(double x, double y, double radius, Color color, bool filled = true);
    
    // Dessine l'arc de cercle de la raquette avec un masque de collision strict
    void draw_arc(double x, double y, double radius, Color color);
}

#endif