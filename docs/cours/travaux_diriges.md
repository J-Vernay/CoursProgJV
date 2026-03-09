# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Gère le lien entre le jeu et le moteur custom "XDino".
>
> `dino/x64-windows/` : Contient l'ensemble des dépendances liées à Windows.
>
> `dino/x64-linux/` : Contient l'ensemble des dépendances liées à Linux.
>
> `dino/dino_game.cpp` : Implémente la logique principale du jeu. Utilise les fonctions d'initialisation et de fermeture
> du programme, ainsi que l'"Update" pour gérer la génération des frames.
>
> `dino/dino_geometry.cpp` : Implémente une fonction qui vérifie et retourne vrai si deux segments AB et CD se coupent.
>
> `dino/dino_draw_utils.cpp` : Implémente les fonctions qui permettent de dessiner à l'écran. Une pour dessiner un
> cercle, une autre pour dessiner une boite de dialogue, avec ou sans texte, et enfin une pour dessiner un ensemble de
> segments reliants un ensemble de points entre eux.
>
> `premake5.lua` : Renseigne l'ensemble des paramètres et configurations ("Système de Build") de la solution, tels que
> le dossier de Build, les plateformes acceptées, ou encore l'ensemble des configuration possibles (Debug / Profile /
> Release).

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des
`// COMMENTAIRE`.

```cpp
// X Fonction appelée par le gameplay pour déterminer l'état du clavier et des manettes.

// X Déclaration des fonctions qui sont définies plus bas dans le fichier.

// X Boucle principale d'événement, qui traite les messages que le système d'exploitation nous envoit.
// X Tourne en boucle tant que le programme continue.

// X Définition des constantes.

// X Quand le programme se termine, on libère les ressources associés au programme.

// X La fenêtre graphique vient d'être construite,
// X on initialise des variables globales et le moteur de rendu.

// X Fonction qui gère les événements que nous communique le système d'exploitation.

// X Initialisation de la fenêtre graphique.

// X L'utilisateur redimensionne la fenêtre.
// X On ajuste le moteur de rendu en conséquence.

// X Expose les fonctions de la base de code.

// X Délègue les autres événéments à l'implémentation par défaut du système d'exploitation.

// X Expose les fonctions de la bibliothèque tierce PIX.

// X Fonction d'entrée du programme, contient le code qui sera appelé par le système d'exploitation Windows.

// X Le système d'exploitation demande à redessiner la fenêtre.
// X On demande à la logique de jeu de dessiner une frame.

// X Appelé par la macro DINO_CRITICAL pour afficher une popup en cas d'erreur.

// X L'utilisateur a demandé à détruire la fenêtre.
// X On notifie le système d'exploitation qu'on veut arrêter.

// X Expose les fonctions de la bibliothèque standard.

// X Variables globales, accédées dans ce fichier.

// X Fonction qui communique avec le système d'exploitation
// X pour créer la fenêtre graphique principale de rendu.

// X Expose les fonctions du système d'exploitation Windows.
```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> ...

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> ...

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> ...

f) Par quoi est définit un triangle texturé ?

> ...

g) Qu'est-ce qu'est un "draw call" ?

> ...

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> ...

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> ...

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> ...

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> ...

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> ...

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> ...

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> ...

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> ...

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> ...

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> ...

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> ...

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> ...

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h`. Quels sont les 4 dossiers du projet à contenir des fichiers C++ ?

> ...

b) Cherchez `CoursProgJV *.cpp`. Quels sont les 3 dossiers du projet à contenir des fichiers C++ ?

> ...

c) Cherchez `CoursProgJV *.obj`. Que remarquez-vous des noms des fichiers concernés ? Notez leur chemin.

> ...

d) Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leur chemin.

> ...

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> ...

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** ...
>
> **Fichiers `.cpp` et `.obj` :** ...
>
> **Fichiers `.obj` et `.lib` :** ...
>
> **Fichiers `.obj` et `.dll` :** ...
>
> **Fichiers `.obj` et `.exe` :** ...
>
> **Fichiers `.dll` et `.exe` :** ...

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> ...

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> ...

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> ...

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> ...

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> ...

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> ...

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> ...

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> ...

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> ...

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> ...

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> ...

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> ...

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 7. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> ...

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> ...

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> ...

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> ...

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> ...

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> ...

## 8. Notre propre implémentation de std::vector

Nous allons remplacer l'usage de `std::vector<DinoVec2>` dans la gestion des lassos.

a) Allouez une zone mémoire avec `XDino_MemAlloc()` pour stocker un point.
Quel est le type de retour ? Pourquoi la zone mémoire ne peut pas être manipulée telle quelle ?

> ...

b) Faites en sorte de pouvoir ajouter des points à la liste, en allouant une zone mémoire plus grande
et en copiant l'ancien contenu dans la nouvelle zone mémoire.
Pourquoi l'usage mémoire ne fait que grandir ?

> ...

c) Utilisez `XDino_MemFree()` pour éviter les fuites mémoire lors du redimensionnement du tableau.

d) Si l'on ajoute 20 points au tableau, combien de `DinoVec2` sont copiés en tout ?
Combien d'allocation système, c'est-à-dire des appels à `XDino_MemAlloc()` sont faits ?

> ...

e) Limiter le nombre de copie et d'allocation système en implémentant une "capacité",
Quelle est la différence entre la taille et la capacité de notre tableau ?

> ...

f) Isoler la logique de gestion du tableau dans une classe à part `DinoArrayVec2`.

g) Que se passe-t-il lorsqu'une fonction contenant une variable locale de type `DinoArrayVec2`
retourne ? Quel mécanisme C++ permet de s'assurer que cela ne se produise pas ?

> ...

h) À l'inverse, quel mécanisme C++ permet de s'assurer que `DinoArrayVec2` peut toujours être détruit,
même si la fonction d'initialisation n'est pas explicitement appelée ?

> ...

i) Que se passe-t-il lorsqu'on copie un `DinoArrayVec2` vers un autre ?
Quelles fonctions C++ doivent être définies pour se prémunir du problème ?

> ...

j) Itérez sur tous les `DinoVec2` d'un `DinoArray<DinoVec2>` avec une boucle for-range,
pour décaler chaque point du lasso d'une petite distance avant de dessigner le lasso,
pour imiter un effet gribouillage. Quelles méthodes devez-vous implémenter pour que la syntaxe
de la boucle for-range soit acceptée par le compilateur ?

k) Utilisez la syntaxe des template `template<typename T>` pour transformer la classe
`DinoArrayVec2` en classe générique `DinoArray`.
Pourquoi l'implémentation de `DinoArray` doit forcément être dans un fichier header ?

> ...

l) Remplacez les usages de `std::vector<DinoVertex>` en `DinoArray<DinoVertex>`.

m) Remplacez les usages de `std::vector<DinoPlayer>` en `DinoArray<DinoPlayer>`.
Quel problème se pose ? Fixer l'implémentation de `DinoArray` pour utiliser placement-new.

> ...

## 9. Flow de jeu

a) Implémentez F6.1 et F6.2 .

b) Implémentez F6.3.

c) Implémentez F6.4.

d) Implémentez F6.5.

e) Implémentez F6.6.

h) Implémentez F6.7.

j) Implémentez F6.8.

k) Sous quelle forme peut-on réorganiser toute cette logique ? Pourquoi ?

> ...

l) Utilisez le polymorphisme pour gérer l'état actuel du jeu.
Qu'est-ce que cela entraîne nécessairement pour le stockage ?

> ...

m) Utilisez `std::unique_ptr<T>`. En quoi son usage est pratique ?

> ...

n) Que veut dire `std::move` ?

> ...

o) Implémentez des move-operators sur `DinoArray`.
Comment démontrez-vous qu'ils fonctionnent ?

> ...

## 10. Profiling

a) Compilez le jeu avec la configuration "Profile", puis lancez le jeu.

b) Prenez une capture PIX. ( https://devblogs.microsoft.com/pix/download/ )

* Dans **File > Settings > PDB Search Paths**, ajoutez `srv*c:\symcache*https://msdl.microsoft.com/download/symbols` .
* Dans **Connect > Local Machine > Attach**, cherchez le nom de l'exécutable (`Dino_PrenomNom.exe`), et clique dessus.
* Décochez la case **For GPU Capture**, puis appuyez sur le bouton **Attach**.
* Dans **Start Timing Capture**, cochez **CPU Samples** avec **8k / sec**, puis appuyez sur l'icône de chronomètre.
* Jouez pendant quelques secondes.
* Recliquez sur l'icône de chronomètre.
* Allez dans l'onglet en haut **Timing 1.wpix**, puis dans l'onglet **Timeline**

c) En haut à droite de la fenêtre, vous pouvez voir **Loading symbols**.
Cette étape télécharge la première fois environ 250 MiB de données dans le dossier `C:\symcache`.
Dépendamment de la connexion, cela peut prendre plusieurs minutes.

En utilisant Everything, quel type de fichiers se trouvent dans `C:\symcache` ? Quel est leur rôle ?

> ...

d) Sélectionnez une large bande dans la timeline. En bas à droite, sélectionnez **Items to show = Sampled functions**.

Quelle part représente `Dino_GameFrame` relativement à `WinMain` ? Quelle autre fonction prend du temps ?

> ...

e) Zoomez sur la timeline (`Ctrl + Molette souris`), jusqu'à voir les frames individuellement.
Double-cliquez sur un rectangle **Frame**. Vous devriez revoir les observations de la question précédente.

f) Double-cliquez sur un rectangle **Frame**, puis cliquez en bas à gauche sur **Graph Duration in Metrics View**.
Quelle est le temps moyen d'une frame ? Le temps minimum ? Le temps maximum ?

> ...

g) Les rectangles dans la timeline PIX sont créés dans le code par des appels à XDino_ProfileBegin()
et XDino_ProfileEnd() . Ces appels de fonctions peuvent être imbriqués. Utilisez ces fonctions
pour avoir une meilleure idée des performances du code, et répondre aux questions suivantes :

**Temps passé en moyenne pour...**

> **Lire les entrées claviers/manette :** ...
>
> **Le calcul des DinoDrawCall :** ...
>
> **La logique de jeu (excluant lire les entrées et drawcalls):** ...
>
> **Résoudre les collisions :** ...
>
> **Calculer les intersections de lasso :** ...
>
> **Le calcul des DinoDrawCall des animaux :** ...
>
> **Le calcul des DinoDrawCall des lasso :** ...
