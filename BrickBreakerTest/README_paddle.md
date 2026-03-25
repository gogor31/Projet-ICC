# Module : paddle

## 1. Description générale
Le module `paddle` définit la raquette contrôlée par le joueur. Contrairement aux autres éléments, la raquette est modélisée par un cercle dont le centre se situe en dehors de l'arène (y < 0). Seule l'intersection du cercle avec le demi-plan supérieur (y > 0) est visible et utilisable pour faire rebondir la balle.

## 2. Structure de données
* **`Paddle` (Classe)** : 
    * Attribut : `Circle circle_` (centre et rayon).
    * La raquette est caractérisée par un centre dont la coordonnée `y` doit être négative pour que seule une calotte circulaire apparaisse dans l'arène.

## 3. Méthodes de validation (`check`)
La validation de la raquette est cruciale car elle doit garantir que la partie "jouable" se situe bien dans les limites horizontales de l'arène.

1. **Validation Verticale** : 
    * Le centre doit être sous l'axe des abscisses (`y <= 0`).
    * Le sommet du cercle (`y + radius`) doit être strictement positif, sinon la raquette serait totalement invisible.
2. **Validation Horizontale (Points d'intersection)** :
    * Le module calcule les deux points d'intersection $x_1$ et $x_2$ du cercle avec l'axe $y=0$ (la base de l'arène).
    * Formule utilisée : $x = x_{centre} \pm \sqrt{R^2 - y_{centre}^2}$.
    * Ces deux points doivent impérativement se trouver dans l'intervalle $[0, \text{arena\_size}]$.

## 4. Choix d'implémentation
* **Géométrie analytique** : L'utilisation de l'équation du cercle pour déterminer la largeur visible de la raquette permet une précision parfaite pour la gestion des rebonds futurs.
* **Robustesse** : Le code vérifie que le discriminant ($R^2 - y^2$) est positif avant d'extraire la racine carrée pour éviter toute erreur de calcul (`NaN`).
* **Optimisation** : Les calculs de carrés sont effectués par multiplication directe pour minimiser les appels aux fonctions coûteuses de la bibliothèque `<cmath>`.