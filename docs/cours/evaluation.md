# Modalités d'évaluation

**Veillez à travailler dans une branche Git à votre nom : NOM_Prenom**.

**Après chaque point, veuillez (git add + git commit) votre base de code avec dans le message le nom du point qde
travaux dirigés que vous venez de faire.** (par exemple: `git add .` puis `git commit -m "0.a"`)

**A la fin de chaque séance, veuillez push votre branche sur le dépôt GitHub** (`git push`)

La note du cours est composée de 3 rendus :

## travaux_diriges.md (30%)

Au fur et à mesure du cours, vous serez amené à remplir un mini-compte rendu
dans le fichier `travaux_diriges.md`, à l'emplacement des `...`.
Vous n'êtes pas obligé de faire des phrases si vos notes sont claires en quelques mots.

Cette note sert à mesurer l'assiduité au long du cours,
et à aller chercher les réponses proactivement.

## Examen papier (30%)

Vous réaliserez un examen sur papier le mardi 24 mars de 10h à 11h.
Cet examen reprend ce qui a été vu le long du cours,
que ce soit dans le code écrit en cours (dans la branche VERNAY_Julien sur GitHub),
dans les questions de `travaux_diriges.md`, et dans les notes de cours (en PDF dans le dépôt).

## Rendu du logiciel (40%)

Une fois le cours terminé, des fonctionnalités du jeu vous seront laissés
à faire en autonomie. Il est conseillé de passer dessus au plus environ une journée.
Le projet est à rendre d'ici deux semaines plus tard, sous la forme d'un `git push` de votre branche.
Je prendrai l'état du dossier tel qu'il aura été le vendredi 27 mars à minuit.

Seront jugés le bon fonctionnement des fonctionnalités (50%), et l'état global de la base de code (50%) :

* Code commenté de telle sorte à pouvoir générer la documentation des classes.
* Séparation des responsabilités via encapsulation par différentes classes.
* Pas de fonctions de 200 lignes qui touchent à tout.
* Code lisible (variables bien nommées)
* Les seules variables globales autorisées sont dans `dino_game.cpp`.
* Architecture du code clair :
    * Chaque variable a un unique "parent" responsable de sa durée de vie.
    * Une variable ne devrait pas avoir besoin de connaître son "parent".
    * De préférence, passer les informations via les paramètres de fonctions
      et valeurs de retour des fonctions/méthodes (plutôt que callbacks et variables globales/membres).
