# Module : project (Main)

## 1. Description générale
Le module `project` contient la fonction `main()`, point d'entrée unique de l'exécutable. Son rôle est de gérer l'interface en ligne de commande, d'initialiser le contrôleur principal (`Game`) et de rapporter le statut final de l'exécution à l'utilisateur.

## 2. Flux d'exécution
Le programme suit un cycle de vie linéaire pour le Rendu 1 :
1. **Analyse des arguments** : Vérification que l'utilisateur a fourni exactement un argument (le chemin vers le fichier de configuration `.txt`).
2. **Initialisation** : Instanciation de l'objet `Game` qui centralise toutes les données du modèle.
3. **Chargement** : Appel de la méthode `game.load_file()`. Cette étape déclenche en cascade la lecture, le parsing et la validation de toutes les entités (balles, briques, raquette).
4. **Verdict** : 
    * Si le chargement réussit sans collision ni erreur de format : affichage de `message::success()`.
    * Si une erreur est détectée : arrêt immédiat avec un code de retour `1`.

## 3. Gestion des paramètres (argc/argv)
Conformément aux standards de programmation système :
* **`argc`** : Utilisé pour valider que le nombre de paramètres est correct (attendu : 2, incluant le nom de l'exécutable).
* **`argv[1]`** : Récupère le nom du fichier cible pour le transmettre au chargeur du module `Game`.

## 4. Gestion des erreurs et flux de sortie
* **Succès** : Le message de réussite est envoyé sur le flux de sortie standard `std::cout`.
* **Échecs** : En cas d'erreur lors du parsing ou de la validation des données, les messages d'erreurs spécifiques sont gérés par les sous-modules, et le `main` assure la terminaison propre du processus avec un code d'erreur.

## 5. Conformité au Rendu 1
Ce module respecte strictement l'architecture "ligne de commande" imposée par la donnée, sans aucune dépendance à une interface graphique, et utilise exclusivement les messages fournis par le module `message`.