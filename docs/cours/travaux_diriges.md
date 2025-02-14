# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

**Veillez à travailler dans une branche Git à votre nom : NOM_Prenom**.

**Après chaque point, veuillez (git add + git commit) votre base de code avec dans le message le nom du point que vous
venez de faire.** (par exemple: `git add .` puis `git commit -m "0.a"`)

**A la fin de chaque séance, veuillez push votre branche sur le dépôt GitHub** (`git push`)

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers suivants :

> `xdino.h` : Contient toutes les fonctions du moteur de jeu qui sont spécifiques à la plateforme (PS5, Xbox, PC, etc.).
>
> `xdino_win64_main.cpp` : L'implémentation du moteur de jeu pour la plateforme PC (Windows).
>
> `xdino_win64_rdr.cpp` : La partie rendu pour la plateforme.
>
> `dino_game.cpp` : Il contient les 3 fonctions de base et donc la logique du jeu.
>
> `dino_geometry.cpp` : Fonctions pour gérer les collisions.
>
> `dino_draw_utils.cpp` : Fonctions d'aide pour au rendu graphique, commune à toutes les platefomes.
>
> `premake5.lua` : Configure la compilation du projet, comment on passe des fichiers souces à l'exécutable.

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place
des `// COMMENTAIRE`.

```cpp
// Fonction appelée par le gameplay pour déterminer l'état du clavier et des manettes.

// Déclaration des fonctions qui sont définies plus bas dans le fichier.

// Boucle principale d'événement, qui traite les messages que le système d'exploitation nous envoit.
// Tourne en boucle tant que le programme continue.

// Définition des constantes.

// Quand le programme se termine, on libère les ressources associés au programme.

// La fenêtre graphique vient d'être construite,
// on initialise des variables globales et le moteur de rendu.

// Gère les événements que nous communique le système d'exploitation.

// Initialisation de la fenêtre graphique.

// L'utilisateur redimensionne la fenêtre.
// On ajuste le moteur de rendu en conséquence.

// Expose les fonctions de la base de code.

// Délègue les autres événéments à l'implémentation par défaut du système d'exploitation.

// Expose les fonctions de la bibliothèque tierce PIX.

// Fonction d'entrée du programme, contient le code qui sera appelé par le système d'exploitation Windows.

// Le système d'exploitation demande à redessiner la fenêtre.
// On demande à la logique de jeu de dessiner une frame.

// Appelé par la macro DINO_CRITICAL pour afficher une popup en cas d'erreur.

// L'utilisateur a demandé à détruire la fenêtre.
// On notifie le système d'exploitation qu'on veut arrêter.

// Expose les fonctions de la bibliothèque standard.

// Variables globales, accédées dans ce fichier.

// Communique avec le système d'exploitation pour créer la fenêtre graphique principale de rendu.

// Expose les fonctions du système d'exploitation Windows.
```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> Le pixel parcourt 6 pixels entre les deux frames. Vitesse / distance = temps.

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-300, 400) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Il faut faire la racine carré de -300 au carré plus 400 au carré pour obtenir la distance parcourue. Donc 50 000
> pixels par seconde.

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Comme la direction indiquée n'est pas plus précise, la vitesse de déplacement est de 100 pixels par seconde avec comme
> direction (1, 1), en haut à droite. Le vecteur normalisé (1,1) donne le vecteur (0.707, 0.707). Nous pouvons
> multiplier
> notre vecteur de déplacement (0.707, 0.707) par la vitesse de 100 pixels par seconde. Le déplacement est donc de 70.7
> pixels en x et 70.7 pixels en y. La position finale est donc (100 + 70.7, 200 + 70.7) = (170.7, 270.7).

f) Par quoi est définit un triangle texturé ?

> Il est définit par vertice, texture, translation, rotation et scale.

g) Qu'est-ce qu'un "draw call" ?

> C'est une demande de dessin du CPU (processeur) à la carte graphique (GPU).

h) À quoi servent les configurations Debug|Profile|Release ?
Donnez un exemple de différence dans le code.

> Debug est pour le développement, il correspond au code que nous avons fait pas à pas sans aucune optimisation, Profile
> est une couche intermédiaire qui a les optimisations, en revanche on ajoute Pix pour faire du Profiling, enfin
> Release c'est le code également mais de manère optimisée.

i) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

> Réponse à la question dans le script dino_game.cpp au commentaire // NOM Prénom.

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
Comment récupérer les coordonnées UV de la bonne animation ?

> ...

d) Implémentez la fonctionnalité F1.4 .

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper et en avoir plusieurs instances ?

> Via la création d'une struct.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> Ces fonctions sont des méthodes. Elles permettent de limiter a modification d'un type de données à ces fonctions, via
> l'utilisation de classe. Cette limitation est appelé l'encapsulation. Cela permet de protéger les données et de ne pas
> les exposer directement.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Implémentez F1.5 : Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez `std::vector` de la bibliothèque standard pour stocker ces instances.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?

> La syntaxe qui permet d'itérer sur tous les éléments d'un tableau sans manipuler d'indices de cases est la boucle for(
> type obj : list). Par exemple : for (DinoPlayer& player : players) { ... }.

f) Implémentez F1.6 : Utilisez `std::sort` pour que les dinosaures soient affichés de haut en bas, l'un devant l'autre.
Pour se faire, créez une fonction qui permet de comparer deux `DinoPlayer` suivant leur position verticale.
Comment créer cette fonction sans exposer publiquement la position de `DinoPlayer` ?

> Via la création d'une fonction qui return un bool. IsAbove.

g) Que retournent, et à quoi servent, `.begin()` et `.end()` dans l'utilisation de `std::sort` ?

> .begin() retourne le premier élément du vecteur et .end() retourne le dernier élément du vecteur. Ils servent à
> définir la plage de tri.

## 3. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h`. Quels sont les 4 dossiers du projet à contenir des fichiers C++ ?

> src/dino c'est nos fichiers headers.
> external/pix/... ce sont nos fichiers externes et qui permettent d'accéder aux fonctionnalités de Pix.
> external/stb ce sont encore des fichiers externes utilisés par le moteur.
> src/dino/x64-windows ce sont des fichiers headers propre à la plateforme.

b) Cherchez `CoursProgJV *.cpp`. Quels sont les 3 dossiers du projet à contenir des fichiers C++ ?

> src/dino c'est nos fichiers sources.
> src/dino/x64-windows ce sont des fichiers sources propre à la plateforme.
> external/stb implémentation du code externe par le moteur.
> Remarque : nous n'avons pas de fichier sources pour Pix ?

c) Cherchez `CoursProgJV *.obj`. Que remarquez-vous des noms des fichiers concernés ? Notez leur chemin.

> Chaque fichier CPP a un fichier OBJ correspondant.
> build/obj/x64-windows/Debug/Dino_EnzoDufour
> x64-windows c'est spécifique à la plateforme.
> Debug c'est spécifique à la configuration.

d) Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leur chemin.

> build/bin/x64-windows/Debug/Dino_EnzoDufour.exe
> x64-windows c'est spécifique à la plateforme.
> Debug c'est spécifique à la configuration.

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> C'est lui qui définit comment nous passons des fichiers sources à l'exécutable final.
> build/%{cfg.buildcfg} c'est le dossier contenant l'exe.
> includedirs{ "src", "external" }
> #include <dino/dino_player.h>
>
> files { "external/**.cpp", "external **.h"}
> files { "src/dino/**.cpp", "src/dino/**.h"}
> files { "src/dino/x64-windows/**.cpp", "src/dino/x64-windows/**.h"}
> Ils disent où on va hercher nos .cpp et nos .h.

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** .h est une déclaration de ce que l'on va faire dans le .cpp.
>
> **Fichiers `.cpp` et `.obj` :** .obj est une standardisation du .cpp.
>
> **Fichiers `.obj` et `.lib` :** Nous partons des fichiers .obj pour créer les .lib. Un .lib est une bibliothèque de
> .obj
>
> **Fichiers `.obj` et `.dll` :** C'est le même lien que pour .obj et .lib mais pour .dll c'est quelqu'un qui a déjà
> fait la compilation et en plus il a fait l'édition des liens.
>
> **Fichiers `.obj` et `.exe` :** .obj est une standardisation du .cpp. .exe est le résultat final de la compilation.
>
> **Fichiers `.dll` et `.exe` :** .exe va permettre de directement appeler des fonctions de la .dll.

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Nous les reconnaissons via le #. Il permet d'importer des fonctions et des valeurs.

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> Il permet de lier les fichiers .obj entre eux pour créer un .exe. Il peut produire des .lib et des .dll. La différence

## 4. Programmation du terrain

a) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

b) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> Il faut faire pour X : (renderSize.x - terrainSize.x) / 2 et pour Y : (renderSize.y - terrainSize.y) / 2.

c) Implémentez la fonctionnalité F2.1 .

d) Implémentez la fonctionnalité F2.2 . Profitez-en pour créer les fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain. Pour vous faciliter la vie, ajoutez une fonction utilitaire
dans `dino_draw_utils.cpp`
qui ajoute un sprite à un drawcall avec les bonnes positions et UV.

e) Combien de triangles avez-vous besoin pour dessiner le terrain complet (océan compris) ?

> Nous avons besoin de quatre triangles au total.

f) Implémentez la fonctionnalité F2.3 .

g) Implémentez la fonctionnalité F2.4 .

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> La RAM correspond à la mémoire, sur ma machie j'ai 32 GB RAM physique. En 64 bits, on peut adresser 2^64 octets dans
> l'espace adressable, soit 16 000 000 000 GB adressables.

d) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Allouer de la mémoire c'est fait par le système d'exploitation (OS), c'est discuter entre l'OS, le programme, la RAM
> et le CPU. Cela permet de bloquer la mémoire pour un programme et cela permet au logiciel qu'aucun autre
> programme que lui-même ne touche à cet espace. C'est une opération qui dure 16 000 microsecondes environ pour un jeu à
> 60 FPS. Nous pouvons faire environ 5 000 allocations si nous ne faisons que ça.

e) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Un type est un outil de langage de programmation pour garder l'information de comment interpréter ne suite de bits et
> quelles opérations sont autorisées. Le typag statique : le compilateur connait les types associés aux emplacements
> mémoire, donc il peut détecter les erreurs logiques. Un pointeur est une adresse mémoire. Réinterpréter un pointeur
> c'est changer le fait de dire au compilateur "fait moi confiance, je sais ce que je fais" en mettant devant la valeur
> du pointeur le type que l'on veut. Par exemple : int* pA = &a et float* pB (float*)&A.

f) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> La taille de DinoColor est de 4 octets soit 32 bits et la taille de DinoVertex est de 16 octets soit 128. Une
> structure est un ensemble de variable. Un union est un ensemble de variable qui partage la même adresse mémoire qui au
> cours de l'exécution, va y stocker des types différents. Via Rider, en passant sur le type, nous pouvons voir la
> taille ou bien via sizeof(type).

g) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> std::vector est un tableau dynamique. Il est représenté en mémoire par un pointeur vers le début du tableau, pour la
> valeur suivante il fait le pointeur du début + sizeof(int32) * indice. Pour connaître la position en mémoire d'un
> élément étant donné son indice, il faut faire le pointeur du début + sizeof(int32) * indice.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> L'OS commence le programme .exe et dans le fichier exe, il y a la taille nécessaire pour les variables globales. Il
> crée une correspondance dans notre espace mémoire pour les variables globales, au tout début. L'OS alloue la stack (
> pile en français). Les variables locales sont allouées à chaque appel de fonction. La stack ne fait qu'une seule
> grosse allocation, elle est allouée à chaque appel de fonction et nous "grignotons" dedans, à chaque return nus
> rendons la mémoire. Les std::vector sont alloués à chaque appel de fonction, c'est un tableau contigu. Il y a une
> limitation, nous sommes obligés de stocker des éléments du même type.

i) Implémentez la fonctionnalité F3.3. Cela implique de trier un tableau qui peut contenir à la fois
des `DinoPlayer` et des `DinoAnimal`. Comment faire ?

> Mettre de côté

j) Implémentez la fonctionnalité F3.4.

> Mettre de côté

## 6. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Qu'est-ce que la fonctionnalité implique pour les octets en mémoire qui représente le lasso ?

> ...

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?

> ...

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

## 7. Interactions

a) Implémentez la fonctionnalité F5.1 .

b) Implémentez la fonctionnalité F5.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> Admettons que nous leur donnions un rayon de 8 pixels, alors dans le cas ou la distance AB est supérieur à 16 pixels (
> deux fois le rayon) alors ils ne collisionnent pas. En revanche, si AB est inférieur à 16 pixels alors ils
> collisionnent. AB = racine de dx au carré + dy au carré. AB = racine carré de (A.x - B.x) au carré + (A.y - B.y) au
> carré. Donc si cela est inférieur à deux fois le rayon alors ils collisionnent.

d) Comment repousser deux cercles en collision pour qu'ils ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> Pour résoudre cela, il faut repousser A et B de telle sorte qu'ils ne soient plus en collision donc un vecteur V et
> -V. V = alpha AB. Nous voulons que A - V et B + V soit égal à 2 * rayon. Soit 2alpha AB + AB = 2r soit alpha = 2r -
> AB / 2AB. V = alpha AB = (2r - AB / 2AB) AB. Le cas particulier est lorsque AB = 0 (division par 0 qui pose un
> problème), c'est-à-dire que les deux cercles soient piles au même point. Dans ce cas, nous ne pouvons pas les
> repousser. Nous allons faire une vérification s'il y a ce cas et si oui, alors nous ne faisons rien.

e) Implémentez la fonctionnalité F5.3 .

f) Implémentez la fonctionnalité F5.4 . Pourquoi y a-t-il duplication de code ?

> Il y a une duplication du code puisqu'elles n'ont pas de classe mère commune, nous ne pouvons pas généraliser le code.

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> Nous pouvons utiliser le principe de classe dérivée. Pour notifier le fiat qu'une classe dérive il faut mettre :
> public DinoEntity {}; C'est le principe de l'héritage.

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> Le fait d'avoir une fonction virtuelle. Cela permet de définir une fonction dans la classe mère et de la redéfinir (
> override) dans les classes filles.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> Pour les méthodes Draw et Update.

j) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> Via la méthode Dino_IntersectSegment de la classe dino_geometry. Un lasso est une suite de points appelé Polyligne ou
> ligne cassée. Pour l'implémenter nous allons utiliser std:: vector<DinoVec2> pour chaque joueur.

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

