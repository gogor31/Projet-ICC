// ============================================================================
// École Polytechnique Fédérale de Lausanne (EPFL)
// Cours : Programmation orientée objet / Projet C++
// 
// Fichier : brick.h
// Description : Hiérarchie de classes pour la gestion des briques.
//               Gère le polymorphisme, la destruction et le dessin récursif.
//
// Auteur(s) : Legio Ilhan (N° SCPIER : 397526), Nadav Solomon (N° SCPIER : 398747)  
// Date : Mai 2026
// ============================================================================

#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "graphic.h"

// ==========================================
// CLASSE DE BASE ABSTRAITE
// ==========================================

class Brick {
public:
    // Destructeur virtuel obligatoire pour éviter les fuites de mémoire
    virtual ~Brick() = default; 

    // Valide les dimensions de la brique et sa présence dans l'arène
    virtual bool check() const; 

    // Méthodes virtuelles pures imposant un comportement spécifique à chaque type de brique
    virtual void draw() const = 0;
    virtual void write(std::ostream& out) const = 0;
    virtual int get_hit_points() const = 0;
    virtual bool hit() = 0; // Applique un impact et retourne true si la brique est détruite

    const tools::Square& get_bounds() const { return bounds_; }
    int get_type() const { return type_; } 
    bool is_dead() const { return dead_; }
    void mark_as_dead() { dead_ = true; }

protected:  
    // Constructeur protégé pour empêcher l'instanciation directe de la classe de base
    Brick(tools::Square s, int type) : bounds_(s), type_(type) {}
    
    tools::Square bounds_; 
    int type_;
    bool dead_ = false;
};

// ==========================================
// CLASSES DÉRIVÉES
// ==========================================

// Brique standard multicolore sensible aux points de vie (HP)
class RainbowBrick : public Brick {
public:
    RainbowBrick(tools::Square s, int hp) : Brick(s, 0), hit_points_(hp) {}
    
    bool check() const override;
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return hit_points_; }
    bool hit() override {
        --hit_points_;
        return (hit_points_ <= 0);
    }

private:
    int hit_points_;
};

// Brique spéciale libérant une nouvelle balle à sa destruction
class BallBrick : public Brick {
public:
    BallBrick(tools::Square s) : Brick(s, 1) {}
    
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return 1; }
    bool hit() override { return true; }
};

// Brique fractale imbriquée se dessinant de manière récursive
class SplitBrick : public Brick {
public: 
    SplitBrick(tools::Square s, int hp = -1);
    
    void draw() const override;
    void write(std::ostream& out) const override;
    int get_hit_points() const override { return hit_points_; }
    bool hit() override { return true; }

private:
    // Procédure récursive dessinant les sous-briques imbriquées
    void draw_recursive(tools::Square s, int hp) const;
    int hit_points_;
};

#endif