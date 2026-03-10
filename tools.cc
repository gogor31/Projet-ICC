#include <iostream>
#include "tools.h"
#include <cmath>
#include <algorithm>
#include "ball.cc" //!: il faudra mettre .h
#include "game.cc" //!: il faudra mettre .h
#include <fstream>


using namespace std; 

double distance(Point p1, Point p2){
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Vérifie si un point est contenu dans un cercle
bool is_point_in_circle(Point p, Circle c) {
    // Un point est dans le cercle si sa distance au centre 
    // est inférieure au rayon (en tenant compte d'epsil_zero)
    return distance(p, c.center) < (c.radius - epsil_zero);
}

// Vérifie si un point est contenu dans un carré
bool is_point_in_square(Point p, Square s) {
    double half_side = s.side / 2.0;
    // Test d'inclusion selon la formule : min + epsil < val < max
    bool inside_x = (p.x > (s.center.x - half_side + epsil_zero)) && 
                    (p.x < (s.center.x + half_side - epsil_zero));
    bool inside_y = (p.y > (s.center.y - half_side + epsil_zero)) && 
                    (p.y < (s.center.y + half_side - epsil_zero));
    return inside_x && inside_y;
}

bool intersects(Circle c1, Circle c2) {
    // Distance entre les centres < somme des rayons
    return distance(c1.center, c2.center) < (c1.radius + c2.radius);
}

bool intersects(Square s1, Square s2) {
    double dist_x = abs(s1.center.x - s2.center.x);
    double dist_y = abs(s1.center.y - s2.center.y);
    double min_dist = (s1.side + s2.side) / 2.0;
    
    return (dist_x < min_dist) && (dist_y < min_dist);
}

bool intersects(Circle c, Square s) {
    // 1. Trouver les bornes du carré
    double half_s = s.side / 2.0;
    
    // 2. Projeter le centre du cercle sur le carré (Clamping)
    double closest_x = max(s.center.x - half_s, 
                        min(c.center.x, s.center.x + half_s));
    double closest_y = max(s.center.y - half_s, 
                        min(c.center.y, s.center.y + half_s));
    
    // 3. Calculer la distance entre le centre du cercle et ce point proche
    Point closest_point = {closest_x, closest_y};
    return distance(c.center, closest_point) < c.radius;
}


//collision cs (circle square)
void collisionCS(Ball b){;}

//collision cc (circle circle)
void collisionCC(Ball a,Ball b){;}

//lire les fichiers test
void readGame(string filename){
    ifstream file("tests/test00.txt"); //open file

    if (!file.is_open()) {cout << "Error opening file" << endl;}

    //converti le .txt en string dans content
    string line;
    string content;
    while (getline(file,line)) {content += line + "\n";}

    file.close(); //close file 

    //vvv lire les donnees du fichier
    

}