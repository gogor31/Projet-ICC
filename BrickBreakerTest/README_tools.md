# Module : tools

## 1. Description générale
Le module `tools` constitue la bibliothèque utilitaire de base du projet. Il regroupe les structures de données géométriques simples et les fonctions de calcul nécessaires à la gestion de l'espace de jeu (collisions, inclusions et distances). 
Ce module est conçu pour être totalement indépendant et ne dépend d'aucun autre composant du projet.

## 2. Structures de données
* **`Point`** : Coordonnées cartésiennes `(x, y)` en double précision. Utilisé pour définir des positions ou des vecteurs de vitesse (`delta`).
* **`Circle`** : Défini par un centre (`Point`) et un rayon (`double`). Structure de base pour la balle.
* **`Square`** : Défini par un centre (`Point`) et une longueur de côté (`double`). Structure de base pour les briques et la raquette.

## 3. API et Fonctions utilitaires
Les fonctions utilisent systématiquement le préfixe `std::` pour la bibliothèque standard et traitent les arrondis via un paramètre `epsilon`.

| Fonction | Description |
| :--- | :--- |
| `distance` | Calcule la distance euclidienne entre deux points. |
| `intersects_Circle_Circle` | Détecte l'intersection entre deux cercles (distance < somme des rayons). |
| `intersects_Square_Square` | Détecte l'intersection entre deux carrés (AABB). |
| `intersects_Circle_Square` | Gère la collision entre un cercle et un carré par la méthode du point le plus proche (*clamping*). |
| `is_circle_in_square` | Valide qu'un cercle est entièrement contenu dans un carré de côté $L$. |

## 4. Choix d'implémentation et Optimisations
* **Performance** : La fonction `distance` évite l'utilisation de `std::pow(..., 2)` au profit de multiplications directes (`dx * dx`), plus rapides à l'exécution.
* **Précision** : Toutes les comparaisons logiques intègrent la tolérance `epsilon` pour garantir la robustesse face aux erreurs de précision des types `double`.
* **Modularité** : Le module ne contient aucune variable globale ; il se contente de fournir des outils purement mathématiques.