# Projet DinoRanch

DinoRanch est un projet de jeu 2D d'arcade de 1 à 4 joueurs en local.
Le jeu est en vue du dessus. Les joueurs incarnent chacun un dinosaure.
Les dinosaures doivent capturer des animaux en tournant autour.
Les animaux capturés donnent des points. Plus la prise est grosse, plus il y a de points.
La partie s'arrête quand le chronomètre arrive à sa fin, et les points sont décomptés.

[TOC]

## Fonctionnalités

Chaque fonctionnalité est détaillée par des étapes de plus en plus difficiles.
Des indications sont données pour accélérer le développement sur les aspects non-programmation.

### F1. Les dinosaures

1. Un dinosaure est affiché et peut être déplacé (directions + bouton de course `btn_right`).
2. L'affichage du dinosaure est mis en miroir s'il se déplace vers la gauche (= échanger les positions U).
3. L'affichage du dinosaure est animé suivant ses déplacements.
4. Le dinosaure peut se prendre des dégâts (= immobilisation 3 secondes + animation).
5. 4 dinosaures sont affichables et individuellement déplaçables.

Les images de dinosaures sont dans la spritesheet `dinosaurs.png`, disposées sur une grille
de **24x24 pixels**.

![Texture des dinosaures](@ref dinosaurs.png)

Les animations consistent à enchaîner en boucle différents sprites.
Dans la spritesheet, ces sprites correspondent à des positions U :

- **U=(0, 24, 48, 72)** pour l'animation quand le dinosaure reste sur place. (8 images par seconde)
- **U=(96, 120, 144, 168, 192, 216)** pour l'animation de marche. (8 images par seconde)
- **U=(336, 360, 384)** pour l'animation de dégâts. (8 images par seconde)
- **U=(432, 456, 480, 504, 528, 552)** pour l'animation de course. (16 images par seconde)

Les dinosaures de différentes couleurs correspondent à des positions V :

- **V=0** pour le dinosaure bleu
- **V=24** pour le dinosaure rouge
- **V=48** pour le dinosaure jaune
- **V=72** pour le dinosaure vert

### F2. Le terrain

1. L'océan est affiché sur toute la fenêtre, et une zone centrale de terrain de **256x192 pixels**.
2. Le terrain peut avoir au hasard une des quatre saisons.
3. Des fleurs sont positionnées sur le terrain aléatoirement à chaque début de partie.
4. *(BONUS)* La frontière terrain/océan est dessinée avec le tileset et animée.

Les images du terrain sont dans le tileset `terrain.png`.

![Texture du terrain](@ref terrain.png)

Les tuiles du tileset font une taille de **16x16 pixels**.
Le terrain de **256x192 pixels** correspond donc à **16x12 tuiles**.
Les arbres font une taille de **48x72 pixels**.

La spritesheet est composée de :

- **UV = (0,0)** pour la couleur de l'océan
- **UV = (16,0)** pour la couleur du terrain
- **UV = (0,16)** pour la tuile du coin en haut à gauche
- **UV = (32,16)** pour la tuile du coin en haut à droite
- **UV = (0,48)** pour la tuile du coin en bas à gauche
- **UV = (32,48)** pour la tuile du coin en bas à droite
- **UV = (16,16)** pour la tuile à répéter sur la frontière en haut
- **UV = (0,32)** pour la tuile à répéter sur la frontière à gauche
- **UV = (32,32)** pour la tuile à répéter sur la frontière à droite
- **UV = (16,48)** pour la tuile à répéter sur la frontière en bas
- **UV = (32,0)** pour la fleur 1
- **UV = (48,0)** pour la fleur 2
- **UV = (64,0)** pour la fleur 3
- **UV = (de 48,16 à 80,64)** pour l'arbre 1
- **UV = (de 48,64 à 80,112)** pour l'arbre 2
- **UV = (de 48,112 à 80,160)** pour l'arbre 3
- **UV = (de 48,160 à 80,208)** pour l'arbre 4

Pour obtenir les animations des tuiles de terrain, il faut ajouter dans l'ordre **V += (0, 48, 96, 144)**.

Pour obtenir les saisons, il faut ajouter dans l'ordre **U += (0, 80, 160, 240)**.

### F3. Les animaux

1. Des animaux apparaissent au hasard sur le terrain (1 par seconde).
2. Les animaux se déplacent aléatoirement et sont correctement animés.
3. Pendant l'apparition, les animaux passent d'invisible à transparent à opaque.

Les images d'animaux sont dans la spritesheet `animals.png`.

![Texture des animaux](@ref animals.png)

Chaque sprite fait une taille de **32x32 pixels**.

Les animations consistent à enchaîner en boucle 4 sprites,
suivant les positions **U=(0, 32, 64, 96)**.

Les différentes animations correspondent à des positions V :

- **V = 0** pour marcher sur le côté (à mettre en miroir pour aller vers la droite)
- **V = 32** pour marcher vers le bas
- **V = 64** pour marcher vers le haut

Les différents animaux correspondent à des décalage de positions U :

- **U += 0 ou 128** pour les deux types de cochon.
- **U += 256 ou 384** pour les deux types de vache.
- **U += 512 ou 640** pour les deux types de mouton.
- **U += 768 ou 896** pour les deux types d'autruche.

### F4. Physique de jeu

1. Les dinosaures ne peuvent pas sortir des limites du terrain.
2. Les animaux ne peuvent pas sortir des limites du terrain. Quand ils atteignent le bord
   du terrain, ils prennent une nouvelle direction aléatoirement.
3. Quand les dinosaures sont en collision (distance < 16 pixels), ils se repoussent.
4. Les animaux se repoussent entre eux, et aussi les animaux et les dinosaures entre eux.
5. Les dinosaures et les animaux sont affichés les uns derrière les autres, suivant leur position verticale.

### F5. Lasso

1. Des suites de points sont dessinées correspondant aux positions passées
   des dinosaures, aux couleurs des dinosaures.
2. Les suites de points sont tronquées à une longueur maximale de deux secondes.
3. Quand deux segments se coupent et sont du même joueur, la boucle
   est retirée du lasso (mais la partie avant la boucle existe toujours).
4. Quand un joueur passe par dessus le lasso d'un autre joueur, le début du lasso est détruit jusqu'à l'intersection.
5. Quand un dinosaure est dans une boucle de lasso,
   il se prend des dégâts (= immobilisation 3 secondes + animation).
6. Quand des animaux sont dans une boucle de lasso, ils disparaissent.

### F6. Flow du jeu

1. Un chronomètre de 60 secondes est affiché en haut de l'écran (centré horizontalement) et décroit.
2. Plus le chronomètre est bas, plus les animaux apparaissent vite.
3. En cours de partie, n'importe quel joueur peut mettre en pause avec `start`.
4. En pause, les animaux, joueurs, et le chronomètre, ne bougent pas.
5. Avant la première partie, les joueurs sont dans l'état de "lobby"
   et peuvent se connecter (`start`) ou se déconnecter (`select`).
6. Un arbre de chaque saison est affiché sur le lobby ; n'importe quel joueur
   peut en entourer un pour lancer la partie avec un terrain correspondant à cette saison.
7. Les joueurs ne peuvent se connecter/déconnecter que sur le lobby.
   Les joueurs ne peuvent mettre en pause que quand le jeu est en cours.
8. Quand le chronomètre expire, on est de retour dans l'état "lobby".
   Les arbres sont translucides pendant 5 secondes et ne peuvent pas être sélectionnés.

### F7. Scoring

Quand le lasso fait une boucle, le score donné au joueur dépend des animaux :
pour chaque type d'animaux (vache, autruche, cochon et mouton), le premier animal de ce type
rapporte 10 points, puis 20 points, puis 30 points, etc. Par exemple, faire une boucle de lasso
contenant 4 vaches et 2 autruches rapporte **(10 + 20 + 30 + 40) + (10 + 20) = 130 points** au joueur.

Quand les animaux disparaissent, une petite notification contenant le texte `+10`, `+20`, etc, apparaît
à leur emplacement, avec le texte colorié suivant le joueur qui a fermé la boucle.

Sur le côté gauche de l'écran, le score des 4 joueurs est affiché, chacun suivant la couleur du dinosaure.
Ce score reste visible pendant le temps où les joueurs sont sur le lobby.

### F8. Menu d'options

Quand la partie est mis en pause, l'écran de pause continue d'afficher le jeu en arrière-plan,
et propose les choix suivants :

- Recommencer : fait réapparaître les joueurs sur le même terrain, avec le chronomètre réinitialisé.
- Retour au lobby : finit la partie, les scores ne sont plus affichés.
- Chrono : avec les flèches de gauche/droite, le chronomètre peut être avancé/réculé de 10 secondes.
- Reprendre : continue la partie, permet de sortir de la pause.

Sur l'écran de pause, tous les joueurs peuvent contrôler le menu.
L'écran de pause est contrôle avec les boutons `dpad` et `btn_right`.
