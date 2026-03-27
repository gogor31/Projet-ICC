# Module : game

## 1. Description générale
Le module `game` est le composant central du projet (Modèle). Il est responsable de la lecture du fichier de configuration, de l'instanciation de toutes les entités du jeu et de la validation de la cohérence globale de la scène initiale.

## 2. Gestion des entités
Le module stocke et gère les collections suivantes :
* **Briques** : Un vecteur de pointeurs (`std::vector<Brick*>`) pour supporter le polymorphisme des différents types de briques (`Rainbow`, `Ball`, `Split`).
* **Balles** : Un vecteur d'objets (`std::vector<Ball>`).
* **Raquette** : Une instance unique de la classe `Paddle`.

## 3. Chargement et Parsing (`load_file`)
Le processus de lecture est séquentiel et robuste :
1. **Nettoyage** : Réinitialisation complète de la mémoire via `clear()` avant chaque chargement.
2. **Filtrage** : Utilisation d'une fonction utilitaire `get_valid_line` pour ignorer les commentaires (`#`) et les lignes blanches.
3. **Extraction** : Utilisation de `std::stringstream` pour convertir les données textuelles en types numériques (`double`, `int`).
4. **Validation par étape** : Chaque section (Header, Paddle, Bricks, Balls) est validée individuellement avant de passer à la suivante.

## 4. Validation de l'état initial
Une fois toutes les entités créées, le module effectue une série de tests de collisions statiques pour garantir que le jeu commence dans un état valide :
* **Inter-Briques** : Vérifie qu'aucune brique ne chevauche une autre.
* **Briques-Paddle** : Vérifie que la raquette n'est pas superposée à une brique.
* **Balles-Balles** : Vérifie qu'aucune balle ne se touche initialement.
* **Balles-Briques/Paddle** : Vérifie l'absence de collision entre les éléments mobiles et statiques.

## 5. Gestion de la mémoire
Le module applique le principe de responsabilité : puisque `Game` instancie les briques sur le tas (*heap*) via le mot-clé `new`, il est responsable de leur libération via `delete` dans la méthode `clear()` et dans son destructeur. Cela garantit l'absence de fuites de mémoire (*memory leaks*).