# Modalités d'évaluation

**Veillez à travailler dans une branche Git à votre nom : NOM_Prenom**.

**Après chaque point, veuillez (git add + git commit) votre base de code avec dans le message le nom du point que vous venez de faire.** (par exemple: `git add .` puis `git commit -m "0.a"`)

**A la fin de chaque séance, veuillez push votre branche sur le dépôt GitHub** (`git push`)

La note du cours est composée de 3 rendus :

## travaux_diriges.md (30%)

Au fur et à mesure du cours, vous serez amené à remplir un mini-compte rendu
dans le fichier `travaux_diriges.md`, à l'emplacement des `...`.
Vous n'êtes pas obligé de faire des phrases si vos notes sont claires en quelques mots.

Cette note sert à mesurer l'assiduité au long du cours,
et à aller chercher les réponses proactivement.

## Examen papier (30%)

Le jeudi 27 février au matin, vous réaliserez un examen sur papier
qui reprend ce qui a été vu le long du cours. `travaux_diriges.md`
contient une partie des questions papier.

## Rendu du logiciel (40%)

Une fois le cours terminé, des fonctionnalités du jeu vous seront laissés
à faire en autonomie. Il est conseillé de passer dessus au plus environ une journée.
Le projet est à rendre d'ici deux semaines plus tard, sous la forme d'un `git push` de votre branche.
Je prendrai l'état du dossier tel qu'il aura été le vendredi 28 février à minuit.

Seront jugés le bon fonctionnement des fonctionnalités (50%), et l'état global de la base de code (50%) :

* Code commenté de telle sorte à pouvoir générer la documentation des classes.
* Séparation des responsabilités via encapsulation par différentes classes.
* Pas de fonctions de 200 lignes qui touchent à tout.
* Code lisible (variables bien nommées)
* Minimiser les variables globales accédées partout.

Les fonctionnalités laissées en autonomie sont :

* F1.4 - **[BONUS]** Le dinosaure peut se prendre des dégâts (= immobilisation 3 secondes + animation).
* F2.2 ; F2.3 ; F2.4 - **[BONUS]** Terrain animé, avec des fleurs, et des saisons différentes
* F3.4 - **[BONUS]** Pendant l'apparition, les animaux passent d'invisible à transparent à opaque.
* F4.4 - **[NECESSAIRE]** Quand un joueur passe par dessus le lasso d'un autre joueur, le début du lasso est détruit jusqu'à l'intersection.
* F5.5 - **[BONUS]** Quand un dinosaure est dans une boucle de lasso, il se prend des dégâts
* F5.6 - **[NECESSAIRE]** Quand des animaux sont dans une boucle de lasso, ils disparaissent.
* F6 - **[NECESSAIRE]** Scoring et chronomètre - autonomie complète
* F7 - **[NECESSAIRE]** Lobby et flow du jeu - autonomie complète 

Les fonctionnalités **[NECESSAIRE]** font partie de l'évaluation.
Les fonctionnalités **[BONUS]** sont considérées comme du polish, et représentent
1.5 points bonus sur la note totale (1 point pour le cosmétique, 0.5 point pour gestion des dégâts).
