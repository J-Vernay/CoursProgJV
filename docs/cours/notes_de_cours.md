# Notes de cours

Le cours utilise comme fil rouge le [projet DinoRanch](projet_dinoranch.md).
Le design du jeu est décomposé en points unitaires.
Certains de ces points sont un support pour dispenser le cours.
D'autres sont l'occasion de laisser les élèves en autonomie.

J'ai mesuré le temps qu'il me fallait pour remplir chaque point.
Il va de soi que les élèves auront besoin de plus de temps.

[TOC]


## Déroulé du cours

Le cours est décomposé en 6 séances de 3 heures.

### Séance 1

1. 10 minutes présentation du cours
2. 10 minutes mise en place de la base de code
3. 10 minutes présentation du C++.
2. 1 heure pour F1.1 à F1.4, avec explication des triangles texturés.
-- pause 15 min --
3. 30 minutes pour explication de structures/classes pour F1.5.
4. 15 minutes pour F1.6.
5. 15 minutes pour F1.7.

### Séance 2

1. 20 minutes pour explication compilation C++
2. 20 minutes pour F2.1
3. 40 minutes pour F2.2
-- pause 15 minutes --
4. 40 minutes F2.3
5. 40 minutes F3.1

### Séance 3

1. 20 minutes pour la mémoire ?
2. 1h pour minimum remplacement viable pour std::vector ?
-- pause 15 minutes
3. 20 minutes pour constructeur/copie/destructeur ?
4. 1h pour peaufiner et remplacer les usages de std::vector

### Séance 4

1. 1h pour F3.2 + F3.3
2. 30min pour F4.1
-- pause 15 minutes
3. 15 minutes opérateurs custom
4. 45 minutes implémenter +-* sur DinoVec2 et changer les usages
5. 15min pour F4.2

### Séance 5
1. 1h30 pour F4.3 et F4.4
-- pause 15 minutes
2. 1h pour F5.1-F5.4


## Durées indicatives

### F1. Les dinosaures

F1.1 | 16 minutes | Un dinosaure est affiché et peut être déplacé (directions + bouton de course `btn_right`).
F1.2 |  3 minutes | L'affichage du dinosaure est mis en miroir s'il se déplace vers la gauche (= échanger les positions U).
F1.3 | 16 minutes | L'affichage du dinosaure est animé suivant ses déplacements.
F1.4 |  6 minutes | Le dinosaure peut se prendre des dégâts (= immobilisation 3 secondes + animation).
F1.5 | 26 minutes | 4 dinosaures sont affichables et individuellement déplaçables.
F1.6 |  7 minutes | Tous les dinosaures sont affichés avec un seul draw call.
F1.7 | 13 minutes | Les dinosaures peuvent être devant les autres et être affichés suivant lequel est devant.

### F2. Le terrain

F2.1 | 10 minutes | L'océan est affiché sur toute la fenêtre, et une zone centrale de terrain de **256x192 pixels**.
F2.2 | 30 minutes | La frontière terrain/océan est dessinée avec le tileset et animée.
F2.3 | 20 minutes | Des fleurs sont positionnées sur le terrain aléatoirement à chaque début de partie.

### F3. Les animaux

F3.1 | 20 minutes | Des animaux apparaissent au hasard sur le terrain (1 par seconde).
F3.2 | 30 minutes | Les animaux se déplacent aléatoirement et sont correctement animés.
F3.3 |  6 minutes | Pendant l'apparition, les animaux passent d'invisible à transparent à opaque.

### F4. Lasso

F4.1 | 17 minutes | Des suites de points dessinées correspondant aux positions passées des dinosaures, aux couleurs des dinosaures.
F4.2 |  3 minutes | Les suites de points sont tronquées à une longueur maximale.
F4.3 | 25 minutes | Quand deux segments se coupent et sont du même joueur, la boucle est retirée du lasso.
F4.4 | 35 minutes | Quand un joueur passe par dessus le lasso d'un autre joueur, le début du lasso est détruit jusqu'à l'intersection.

### F5. Interactions

F5.1 |  6 minutes | Les dinosaures ne peuvent pas sortir des limites du terrain.
F5.2 | 10 minutes | Les animaux ne peuvent pas sortir des limites du terrain.
F5.3 | 13 minutes | Quand les dinosaures sont en collision (distance < 16 pixels), ils se repoussent.
F5.4 |  5 minutes | Les animaux se repoussent entre eux, et aussi les animaux et les dinosaures entre eux.

### F6. Scoring et chronomètre

### F7. Menu
