# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

**Veillez à travailler dans une branche Git à votre nom : NOM_Prenom**.

**Après chaque point, veuillez (git add + git commit) votre base de code avec dans le message le nom du point que vous venez de faire.** (par exemple: `git add .` puis `git commit -m "0.a"`)

**A la fin de chaque séance, veuillez push votre branche sur le dépôt GitHub** (`git push`)

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers suivants :

> `xdino.h` : Contient les déclaration des fonctionnaltiés du moteur spécifique à la plateforme
>
> `xdino_win64_main.cpp` : Implémentation du moteur pour PC
>
> `xdino_win64_rdr.cpp` : Implémentation du rendu graphique
>
> `dino_game.cpp` : Implémentation des fonctions principales de la logique de jeu
>
> `dino_geometry.cpp` : Vérifie les intersections sur un segment (collision)
>
> `dino_draw_utils.cpp` : Fonctions pour dessiner des primitives de rendu (toute plateforme)
>
> `premake5.lua` : Renseigne les informations de l'application pour la compilation 

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des `// COMMENTAIRE`.

```cpp
// Fonction appelée par le gameplay pour déterminer l'état du clavier et des manettes. //OK

// Déclaration des fonctions qui sont définies plus bas dans le fichier. //OK

// Boucle principale d'événement, qui traite les messages que le système d'exploitation nous envoit. //OK
// Tourne en boucle tant que le programme continue. //OK

// Définition des constantes. //OK

// Quand le programme se termine, on libère les ressources associés au programme. //OK

// La fenêtre graphique vient d'être construite, //OK
// on initialise des variables globales et le moteur de rendu. //OK

// Gère les événements que nous communique le système d'exploitation. //OK

// Initialisation de la fenêtre graphique. //OK

// L'utilisateur redimensionne la fenêtre. //OK
// On ajuste le moteur de rendu en conséquence. //OK

// Expose les fonctions de la base de code. //Ok

// Délègue les autres événéments à l'implémentation par défaut du système d'exploitation. //OK

// Expose les fonctions de la bibliothèque tierce PIX. //OK

// Fonction d'entrée du programme, contient le code qui sera appelé par le système d'exploitation Windows. //OK

// Le système d'exploitation demande à redessiner la fenêtre. //OK
// On demande à la logique de jeu de dessiner une frame. //OK

// Appelé par la macro DINO_CRITICAL pour afficher une popup en cas d'erreur. //OK

// L'utilisateur a demandé à détruire la fenêtre. //OK
// On notifie le système d'exploitation qu'on veut arrêter. //OK

// Expose les fonctions de la bibliothèque standard. //OK

// Variables globales, accédées dans ce fichier. //OK

// Communique avec le système d'exploitation pour créer la fenêtre graphique principale de rendu. //OK

// Expose les fonctions du système d'exploitation Windows. //OK
```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> 6 px/sec

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-300, 400) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> 50 000 px/s

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> (170 ; 130)

f) Par quoi est définit un triangle texturé ?

> 3 Vertices, une texture, une translation, une rotation et une scale

g) Qu'est-ce qu'un "draw call" ?

> Une demande d'affichage du CPU au GPU par rapport à une liste de point, leur position et une texture

h) À quoi servent les configurations Debug|Profile|Release ?
Donnez un exemple de différence dans le code.

> Débug : Pas d'optimisation facile à débuguer mais plus lent, Release : Optimisé mais compliqué à débuguer, Profile : Optimisé + PIX

i) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

> Done!


## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> En inversant l'ordre d'affichage des uv

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Comment récupérer les coordonnées UV de la bonne animation ?

> Taille du sprite * position

d) Implémentez la fonctionnalité F1.4 .

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper et en avoir plusieurs instances ?

> en créant une class regroupant les éléments d'un dino pour pouvoir en créer X dans le programme 

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> public, protected, private

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Implémentez F1.5 : Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez `std::vector` de la bibliothèque standard pour stocker ces instances.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?

> foreach 

f) Implémentez F1.6 : Utilisez `std::sort` pour que les dinosaures soient affichés de haut en bas, l'un devant l'autre.
Pour se faire, créez une fonction qui permet de comparer deux `DinoPlayer` suivant leur position verticale.
Comment créer cette fonction sans exposer publiquement la position de `DinoPlayer` ?

> créer une fonction public qui modifie la valeur privée

g) Que retournent, et à quoi servent, `.begin()` et `.end()` dans l'utilisation de `std::sort` ?

>

## 3. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h`. Quels sont les 4 dossiers du projet à contenir des fichiers C++ ?

> src/sino -> nos headers
> ser/pix -> fichiers extrenes pour accéder à PIX
> extern/stb -> fichier utilisé par le moteur
> src/dino/x64-windows -> fichiers headers porpre à la plateforme

b) Cherchez `CoursProgJV *.cpp`. Quels sont les 3 dossiers du projet à contenir des fichiers C++ ?

> src/dino -> nos headers
> src/dino/x64-windows -> fichiers headers porpre à la plateforme
> externals/stb -> fichier utilisé par le moteur

c) Cherchez `CoursProgJV *.obj`. Que remarquez-vous des noms des fichiers concernés ? Notez leur chemin.

> Chaques CPP avec un fichier OBJ corresponant
> build/obj/x64-windows/Debug
> x64-winows -> Spécifique à la plateforme
> Debug -> Spécifique à la configuration

d) Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leur chemin.

> build/x64-windows/Debug/Dino
> x64-winows -> Spécifique à la plateforme
> Debug -> Spécifique à la configuration


e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> build/%{cfg.platform}/%/{cfg.builfcfg}

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Préprocesseur
>
> **Fichiers `.cpp` et `.obj` :** Compilation
>
> **Fichiers `.obj` et `.lib` :** Compilation + Archive
>
> **Fichiers `.obj` et `.dll` :** Compilation + Edition de lien
>
> **Fichiers `.obj` et `.exe` :** Edition de lien
>
> **Fichiers `.dll` et `.exe` :** le .exe lui même

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Le Préprocesseur fait le lien entre les include et les fichiers. On reconnais les directives grâce à #

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle différence majeure ?

> .exe et .dll, le .exe possède une entrée, alors que le .dll non

## 4. Programmation du terrain

a) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

b) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> (480 - 256) / 2

c) Implémentez la fonctionnalité F2.1 .

d) Implémentez la fonctionnalité F2.2 . Profitez-en pour créer les fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain. Pour vous faciliter la vie, ajoutez une fonction utilitaire dans `dino_draw_utils.cpp`
qui ajoute un sprite à un drawcall avec les bonnes positions et UV.

e) Combien de triangles avez-vous besoin pour dessiner le terrain complet (océan compris) ?

> 4 (sans les bordures)

f) Implémentez la fonctionnalité F2.3 .

g) Implémentez la fonctionnalité F2.4 .

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 32Gb, 16 000 000 000, seulement les adresse qui lui sont alloué 

d) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Allouer est le fait de reserver un espace de ram pour un programme, c'est relativement couteux

e) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> La façon d'interpréter une suite de bits, un pointeur c'est l'adresse dans la mémoire, récupérer la valeur stocké à cette adresse

f) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> 32 bits, 128 bits

g) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> Une liste de valeur, un poiteur de début + la taille des éléments qu'il contient, pointeur 0 + size de l'élément qu'il contient * l'index, la taille max

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> OS au lancement de l'app
> Le compilateur lorsque l'on rentre dans une fonction/class
> dynamiquement, cela change en fonction de si variable global/local

i) Implémentez la fonctionnalité F3.3. Cela implique de trier un tableau qui peut contenir à la fois
des `DinoPlayer` et des `DinoAnimal`. Comment faire ?

> ...

j) Implémentez la fonctionnalité F3.4.

## 6. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Qu'est-ce que la fonctionnalité implique pour les octets en mémoire qui représente le lasso ?

> ...

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?

> ...

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'interagir entre elles.

## 7. Interactions

a) Implémentez la fonctionnalité F5.1 .

b) Implémentez la fonctionnalité F5.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> ...

d) Comment repousser deux cercles en collision pour qu'ils ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> ...

e) Implémentez la fonctionnalité F5.3 .

f) Implémentez la fonctionnalité F5.4 . Pourquoi y a-t-il duplication de code ?

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

j) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> ...

k) Implémentez F5.5 et F5.6 via une logique commune, comme mentionné par (g).

> ...

## 8. Profiling

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

