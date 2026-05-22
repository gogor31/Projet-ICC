// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : ball.h
// Description : Définition de la classe Ball représentant une balle mobile 
//               et gérant sa cinématique ainsi que son cycle de vie.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "graphic.h"

class Ball {
public:
    // ==========================================
    // CONSTRUCTEURS
    // ==========================================

    Ball(tools::Circle c, tools::Point d);

    // ==========================================
    // ACCESSEURS ET MUTATEURS
    // ==========================================

    double get_pos_x() { return circle_.center.x; }
    
    double get_pos_y() { return circle_.center.y; }
    
    double get_radius() { return circle_.radius; }
    
    const tools::Circle& get_circle() const { return circle_; }
    
    const tools::Point& get_delta() const { return delta_; }
    
    tools::Circle get_circle_next() const;

    // Modifie le vecteur de déplacement en s'assurant qu'il respecte la vitesse maximale
    void set_delta(tools::Point new_delta);
    
    void set_center(tools::Point p);
    
    void mark_as_dead();
    
    bool is_dead() const;

    // ==========================================
    // VÉRIFICATIONS ET LIMITES
    // ==========================================

    // Valide la conformité de la balle 
    bool check() const;

    // ==========================================
    // CINÉMATIQUE ET SIMULATION
    // ==========================================

    // Applique le déplacement basé sur le vecteur delta actuel
    void move();
    
    // Inverse la composante horizontale de la vitesse
    void reverse_dx();
    
    // Inverse la composante verticale de la vitesse
    void reverse_dy();
    
    // Sauvegarde la position actuelle avant la résolution d'éventuelles collisions
    void backup_position();
    
    // Restaure la position sauvegardée en cas de collision invalide ou de blocage
    void restore_position();

    // ==========================================
    // AFFICHAGE
    // ==========================================

    // Dessine le disque représentant la balle
    void draw() const;

private:
    tools::Circle circle_;    
    tools::Point  delta_;     
    tools::Point old_center_; 
    bool dead_ = false;
};

#endif