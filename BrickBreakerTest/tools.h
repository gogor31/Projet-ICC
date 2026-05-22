// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : tools.h
// Description : Bibliothèque d'utilitaires géométriques, physiques et mathématiques
//               pour la gestion des collisions et des mouvements de l'arène.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#ifndef TOOLS_H
#define TOOLS_H

namespace tools {    
    constexpr double epsil_zero = 0.125;

    // ==========================================
    // STRUCTURES DE DONNÉES
    // ==========================================

    struct Point {
        double x;
        double y;
        
        Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    };

    struct Square {
        Point center;
        double side;
    };

    struct Circle {
        Point center;
        double radius;
    };
    
    // ==========================================
    // UTILITAIRES MATHÉMATIQUES ET SCALAIRES
    // ==========================================

    // Borne une valeur entre un minimum et un maximum
    double clamp(double value, double min, double max);

    // Calcule la norme d'un vecteur
    double norm(const Point& p);

    // Retourne le vecteur unitaire.
    Point normalize(const Point& p);

    // Calcule le produit scalaire
    double scalaire(const Point& v1, const Point& v2);

    // Limite la norme d'un vecteur à max_norm si elle la dépasse
    void clamp_vector(Point& v, double max_norm);

    // ==========================================
    // DISTANCES ET GÉOMÉTRIE INFÉRIEURE
    // ==========================================

    // Calcule la distance au carré
    double distance_carre(const Point& p1, const Point& p2);

    // Calcule la distance entre deux points
    double distance(const Point& p1, const Point& p2);

    // ==========================================
    // DÉTECTION DES INTERSECTIONS 
    // ==========================================

    // Vérifie la collision entre deux cercles
    bool intersects_Circle_Circle(const Circle& c1, const Circle& c2, double epsilon = epsil_zero);
                                    
    // Vérifie la collision entre deux carrés alignés sur les axes
    bool intersects_Square_Square(const Square& s1, const Square& s2, double epsilon = epsil_zero);
                                    
    // Vérifie la collision entre un cercle et un carré
    bool intersects_Circle_Square(const Circle& c, const Square& s, double epsilon = epsil_zero);

    // ==========================================
    // VALIDATION DES LIMITES DE L'ARÈNE
    // ==========================================

    // Détermine si un cercle est strictement à l'intérieur d'un carré
    bool is_circle_in_square(const Circle& c, double square_side);
    
    // Vérifie si le paddle reste valide au niveau de l'arène
    bool is_paddle_in_arena(const Circle& c, double square_side);

    // ==========================================
    // PHYSIQUE, RÉFLEXION ET IMPULSIONS
    // ==========================================

    // Calcule le vecteur réfléchi par rapport à une normale 'n'
    Point reflect(const Point& v, const Point& n);

    // Calcule le vecteur reliant le point le plus proche du carré au centre du cercle
    Point compute_nominal_direction(const Circle& c, const Square& s);

    // Calcule la nouvelle vitesse d'une bille après un choc élastique avec une autre
    Point compute_impulse(const Point& d1, double r1, const Point& c1,
                          const Point& d2, double r2, const Point& c2);

    // Calcule la nouvelle vitesse de la balle après rebond sur la raquette
    Point compute_impulse_paddle(const Point& d_ball, const Point& c_ball,
                                 const Point& d_paddle, const Point& c_paddle);
                                 
    // Sépare physiquement deux cercles en cas d'interpénétration
    void resolve_overlap(Point& p1, double r1, Point& p2, double r2);
}
#endif