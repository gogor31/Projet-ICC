# Module : ball

## 1. Description générale
Le module `ball` définit le comportement et les propriétés de la balle. Une balle est une entité mobile caractérisée par sa forme géométrique (un cercle) et un vecteur de déplacement (vitesse).

## 2. Structure de données
* **`Ball` (Classe)** : 
    * **Attributs privés** :
        * `Circle circle_` : Définit la position du centre $(x, y)$ et le rayon de la balle.
        * `Point delta_` : Définit le vecteur de vitesse $(\Delta x, \Delta y)$, représentant le déplacement de la balle à chaque pas de temps.

## 3. Méthodes de validation (`check`)
La méthode `check()` assure que la balle est dans un état initial conforme aux lois de l'arène avant le début de la simulation.

1. **Validation de la vitesse** : 
    * La norme du vecteur de déplacement ne doit pas dépasser la constante `delta_norm_max`.
    * **Optimisation** : Le test est effectué sur les carrés ($v_x^2 + v_y^2 > \text{max}^2$) pour éviter l'appel coûteux à la racine carrée (`std::sqrt`).
2. **Validation de l'arène** :
    * La balle doit être intégralement contenue dans l'aire de jeu $[0, \text{arena\_size}]$.
    * Cette vérification est déléguée à la fonction utilitaire `is_circle_in_square` du module `tools`, garantissant qu'aucune partie du cercle ne dépasse les bords.

## 4. Choix d'implémentation
* **Délégation** : Le module s'appuie fortement sur le module `tools` pour les structures de données (`Circle`, `Point`) et la logique géométrique, respectant ainsi le principe de séparation des responsabilités.
* **Performance** : Suppression de toute utilisation de `std::pow` au profit de multiplications directes, conformément aux standards de performance exigeants du projet.
* **Communication** : En cas d'erreur de vitesse ou de position, le module utilise les fonctions dédiées de `message.h` pour garantir un retour utilisateur précis et normé.