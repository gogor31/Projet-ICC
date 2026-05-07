# Module : Makefile

## 1. Description générale
Le `Makefile` est l'outil d'automatisation de la compilation du projet. Il définit les règles nécessaires pour transformer les fichiers sources C++ (`.cc`) en fichiers objets (`.o`), puis pour les lier afin de générer l'exécutable final `project`.

## 2. Configuration du compilateur
Le fichier utilise les variables standard suivantes pour garantir la portabilité et la qualité du code :
* **`CXX`** : Utilise le compilateur `g++`.
* **`CXXFLAGS`** : 
    * `-std=c++17` : Assure la compatibilité avec la norme C++ 2017.
    * `-Wall -Wextra -pedantic` : Active tous les avertissements pour garantir un code rigoureux et conforme aux standards.
    * `-O2` : Active les optimisations de niveau 2 pour améliorer les performances à l'exécution.

## 3. Structure de compilation
La compilation est divisée en deux étapes :
1. **Compilation séparée** : Chaque module (`game`, `ball`, `brick`, `paddle`, `tools`, `message`) est compilé individuellement en un fichier `.o`. Les dépendances vers les fichiers d'en-tête (`.h`) sont explicitement déclarées pour forcer la recompilation d'un module si sa définition change.
2. **Édition de liens (Linking)** : Les fichiers objets sont regroupés pour produire l'exécutable `project`.

## 4. Commandes disponibles
Le `Makefile` propose les cibles suivantes en ligne de commande :
* **`make`** : Compile l'intégralité du projet et génère l'exécutable `project`.
* **`make clean`** : Supprime tous les fichiers objets (`.o`) et l'exécutable. Cette commande est utile pour "nettoyer" le répertoire ou forcer une compilation complète à partir de zéro.

## 5. Gestion des dépendances
Une attention particulière a été portée à la hiérarchie des inclusions. Par exemple :
* Le module `game.o` dépend de presque tous les autres headers, car il orchestre la création de toutes les entités.
* Le module `tools.o` est minimaliste, reflétant son rôle de bibliothèque utilitaire indépendante.