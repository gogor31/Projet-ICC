# Module : brick

## 1. Description générale
Le module `brick` gère les différents types d'obstacles statiques de l'arène. Il utilise une hiérarchie de classes pour factoriser le comportement commun (position, taille, collision) tout en permettant des comportements spécifiques pour chaque type de brique.

## 2. Hiérarchie des classes
* **`Brick` (Classe de base)** : 
    * Contient les propriétés géométriques (`Square bounds_`).
    * Implémente la validation générique (taille minimale et inclusion dans l'arène).
* **`RainbowBrick` (Hérite de `Brick`)** : 
    * Possède un compteur de points de vie (`hits_points_`).
    * Ajoute une validation sur le nombre de coups requis (entre 1 et 7).
* **`BallBrick` et `SplitBrick` (Héritent de `Brick`)** :
    * Briques spéciales déclenchant des événements lors de leur destruction. Pour le Rendu 1, elles suivent les contraintes de base de `Brick`.

## 3. Méthodes de validation (`check`)
La validation des briques est structurée de manière incrémentale :

1. **Validation Géométrique** : 
    * Vérifie que le côté de la brique est supérieur à `brick_size_min`.
    * Vérifie que l'intégralité du carré est comprise dans les bornes $[0, \text{arena\_size}]$.
2. **Validation Spécifique** :
    * La méthode `RainbowBrick::check()` appelle d'abord `Brick::check()` (réutilisation de code) avant de valider ses propres attributs.

## 4. Intégration des messages d'erreur
Le module est totalement intégré avec le module `message`. En cas d'invalidité (taille, position hors arène, ou points de vie incorrects), la brique signale l'erreur précise via `std::cout` avant d'interrompre le chargement du jeu.

## 5. Choix de conception
* **Héritage** : Permet de traiter toutes les briques de manière uniforme dans le module `Game` (via un vecteur de pointeurs de base si nécessaire pour le polymorphisme futur).
* **Encapsulation** : Les données géométriques sont protégées, garantissant que seule la méthode `check()` peut valider l'intégrité d'une brique après son instanciation.