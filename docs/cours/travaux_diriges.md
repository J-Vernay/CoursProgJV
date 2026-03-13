# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Fichier contenant les définitions du moteur XDino.
>
> `dino/x64-windows/` : Dossier contenant les fichiers permettant au moteur de fonctionner correctement sur Windows.
>
> `dino/x64-linux/` : Dossier contenant les fichiers permettant au moteur de fonctionner correctement sur Linux.
>
> `dino/dino_game.cpp` : Fichier contenant la logique principale du jeu.
>
> `dino/dino_geometry.cpp` : Fichier contenant un booléens retournant si des segments sont intersec ou non.
>
> `dino/dino_draw_utils.cpp` : Fichier permetant l'implementation graphique de base du moteur.
>
> `premake5.lua` : Fichier contenant des indications pour la bonne exécution de la build.

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des
`// COMMENTAIRE`.

```cpp
// Fonction appelée par le gameplay pour déterminer l'état du clavier et des manettes.

// Déclaration des fonctions qui sont définies plus bas dans le fichier.

// Boucle principale d'événement, qui traite les messages que le système d'exploitation nous envoit.
// Tourne en boucle tant que le programme continue.

// Définition des constantes.

// Quand le programme se termine, on libère les ressources associés au programme.

// La fenêtre graphique vient d'être construite,
// on initialise des variables globales et le moteur de rendu.

// Foncfion qui gère les événements que nous communique le système d'exploitation.

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

// Fonction qui communique avec le système d'exploitation
// pour créer la fenêtre graphique principale de rendu.

// Expose les fonctions du système d'exploitation Windows.
```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> Il a parcouru 6 pixel (300 * 0.02).

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Il a parcouru 50 pixels en 10 ms donc 5000 pixels par seconde..


e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Il se toruvera en environ (171,129).

f) Par quoi est définit un triangle texturé ?

> Par trois vertices, ayant des informations de position, de couleur, de coordonnées UV, de normale.

g) Qu'est-ce qu'est un "draw call" ?

> Une instruction donnée à la carte graphique par le processeur pour dessiner des choses à l'écran.

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> On a des informations sur les textures, vertexBuffer et allocations de mémoires

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> Cela sert à avoir des mods de compilation différents.
> par exemple en ajoutant ou retirant des features ou en modifiant des configurations

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> Il s'agit d'un tableau à taille dynamique dont on peu spécifier le type
> par exemple : std::vector<DinoVertex>

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> Elle signifie que l'on va utiliser une reference de la valeur,
> sans l'esperluette cela signifie que l'on utilisera la valeur en elle même (et donc en créer une copie)

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> Il faut inverser la position des UV en fonction de l'input du joueur.

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> ((temps écoulé * vitesse d'animation) % nombre de frame) * décalage en pixel + position de départ de u.

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> On a créé un struct DinoPlayer regroupant toutes les variables associées à celui-ci.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> On va utiliser l'encapsulation, une certaine classe qui va contenir toutes les méthodes dont elle a besoin

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> for (Type& typeName : liste/array )
>   typeName.Update();

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> Parce que l'on définit la taille de l'écran en pixel à l'avance, de ce fait si on dépasse cette limite, 
> ce n'est plus affiché.

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> On définit le nombre de tile posable dans cette aire, ensuite, on calcule la position du corner en haut à gauche,
> à partir de là, il n'y a qu'à poser les tiles.

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> En premiere étape, on crée une liste de toutes les positions possibles.
> En second, on la mélange.
> Ensuite, pour chaque espèce de plante, on prend 10 des premieres positions.
> Et on peux les afficher (en fonction de leur saison on décale les uv etc...).

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> Les 6 dossiers sont : "external\pix, external\sokol, external\stb,(Pour le code externe qui est utilisé pour le
> moteur)
> src\dino (le gameplay), src\dino\x64_linux, src\dino\x64_windows (moteur plateforme)"

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> Chaque source à son fichier obj

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

> On Obtien 3 fichiers  : ce sont tous des exécutables du projet, ils ont tous le même nom, mais on les trouves dans les
> dossiers suivant :
> src/ external
> build/obj/x64 windows/debug
> build/x64-windows/release

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

>On a la ligne 93 pour la compilation des bibliothèques externes : 
>   files { "external/**.cpp", "external/**.h" }
> 
> On a la ligne 107 pour la build windows:
>    files { "src/dino/x64-windows/*" }
> 
> On a la ligne 1015 pour la build linux :
>     files { "src/dino/x64-linux/*" }
> 
> Les particularités du débogguage sont définies plus haut dans le fichier.

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** ...
> Le h est un fichier de déclaration, et le cpp est un fichier de définition dans lequel la logique des méthodes est implémenté.
> **Fichiers `.cpp` et `.obj` :** ...
> Le fichier en .obj est un fichier cpp compilé, il contient donc le code en binaire du .cpp, mais il n'est pas tout a fait complet.
> **Fichiers `.obj` et `.lib` :** ...
> Le fichier .Lib contient des fichiers .obj, c'est le linker qui ira piocher dedans lorsqu'il en aura besoin.
> **Fichiers `.obj` et `.dll` :** ...
> Le fichier .dll contient également des fichiers .obj, mais celle-ci n'est pas intégré dans l'exécutable final.
> **Fichiers `.obj` et `.exe` :** ...
> Ces deux fichiers sont lié par un linker qui va s'occuper des différentes liaisons entre les fichiers et combler les trous.
> **Fichiers `.dll` et `.exe` :** ...
> Le lien entre les deux dépend du projet, certains projets chargent la .dll au démarrage uniquement et d'autres non.

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Il s'agit d'une étape qui se déroule avant la compilation, il ne s'agit de code en c++, mais de la manipulation de texte afin de donner des directives spécifiques au compilateur.
> Une directive de préprocesseur se reconnait avec le character `#`  devant l'instruction.

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> Comme dit précédemment, il intervient avant la création du .exe et s'occupe de correctement lier les fichiers du projet entre eux (pour résoudre les références etc..).
> Il peut produire un exécutable et des bibliothèques (.lib / .dll).

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

> Car on essaye d'implémenter la même fonctionnalité sur des types différents

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> L'héritage permet de créer des fonctions communes à toutes les classes qui en hérite

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> Le polymorphisme permet ce genre de chose, on peut par exemple override une fonction si l'on veut un fonctionnement
> précis pour une classe ayant le même héritage.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> Les méthodes gérant les collisions et les limites de terrain sont des méthodes parfaites pour utiliser ce principe.

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> On va créer un tableau de pointeur, et acceder aux references de ses éléments pour appeler les méthodes dont on a
> besoin.

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> On vas utiliser .erase pour supprimer la dernière position de la liste si elle contient plus de 120 positions.

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> On a 12 vérifications par frame
> On utilisera la méthode .size() pour le pas calculer toutes les positions du lasso.

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> On va tracer une ligne entre un élément du jeu (par exemple un joueur) et un point hors de la zone de jeu,
> Ensuite, au moment de valider une boucle de lasso, on vérifie le nombre de collisions entre cette ligne et le lasso, 
> En fonction du nombre cela détermine si le joueur est dans le lasso ou non.

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> On ne peut acceder qu'à la RAM physique, si le GPU accede à une partie de la RAM non associé a la RAM physique, le
> programme crash.

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> C'est une demande à l'Os de pouvoir mapper une donnée dans l'espace adressable, après cette demande, l'os renvois une
> adresse libre à utiliser.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Les types sont des abstractions, nn type correspond à la place dans la mémoire dont la donnée a besoin pour y être
> stockée.
> C'est le travail du compilateur d'interpreter les types dans la mémoire.
>
> Un pointeur correspond à l'adresse (hexadécimal) de la mémoire dans laquelle la donnée est stockée.
> Cela signifie acceder à l'emplacement d'une séquence d'octet dans la mémoire et l'interpreter avec un type différent
> que celui ce base..

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor : 4 octets
> DinoVertex : 16 octets : 1 Vec2 (2octets), 2 uint16 (10 octets) et 1 couleur (4 octets)

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> Un std::vector est un tableau dynamique, il contient donc trois informations : 
> - Un pointeur qui dirrige vers le début du tableau en mémoire.
> - Une taille, qui donne le nombre d'éléments que contient ce tableau.
> - Une capacité, qui donne le nombre d'éléments que le tableau peut acceuillir avant de devoir réallouer de l'espace.
>Pour connaitre la position d'un élément, avec son indice, il faut connaitre son type ensuite,
>on peut prendre l'adresse donnée par le pointeur et lui ajouter autant de fois que l'indice, l'espace que prend ce type dans la mémoire.
>adresse(i) = adresse_debut + i × taille_d_un_element
> 
> La limiation de cela est que si on dépasse la capcité max du tableau, il faut changer l'emplacement de son contenu dans la mémoire, ce qui peut être une action très lourde.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> C'est l'OS qui va se charger d'allouer de l'espace. Il le fera avant l'execution du main() pendant l'exécution du .exe.
> Pour les variables locales, le compilateur va générer des instructions pour gérer la "stack", ces instructions sont générée l'entrée du block et détruite à la fin.
> Cela va dépendre de s'il s'agit d'une variable locale ou globale, dans les deux cas, il n'y a pas besoin de libéré la mémoire, puisque ces données sont automatiquement détruites.

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> ...

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> ...

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> ...

e) Créer une classe `DinoVertexBuffer` qui accepte les mêmes arguments que `XDino_CreateVertexBuffer()`
et garde le `vbufID` dans un membre privé, avec une fonction `Get()` qui retourne ce `vbufID`.
Faites en sorte que `XDino_DestroyVertexBuffer()` soit appelé automatiquement.
Remplacez les usages des vertex buffers qui sont créés à chaque frame.

f) Que se passe-t-il lorsqu'on copie un `DinoVertexBuffer` vers un autre ? Comment le prévenir ?

> ...

g) Appliquez les mêmes outils pour enlever les méthodes `Shut()` des classes `DinoPlayer`, `DinoAnimal` et `DinoLasso`
(s'il y en a).

h) Implémentez en debug le fait que le terrain est réinitialisé en appuyant
sur le bouton `btn_up` (touche Z du clavier).

i) Quelle difficulté a-t-on pour remplacer `Init()` et `Shut()` de `DinoTerrain` ?
Comment changer le code ?

> ...

j) Pour le test, créer dynamiquement un `DinoAnimal` (en dehors de la logique de jeu).
Convertissez le pointeur en `DinoEntity`, puis détruisez l'instance via le pointeur `DinoEntity`.
Passe-t-on dans le destructeur de `DinoAnimal` ? Pourquoi ? Comment y remédier ?

> ...

## 10. Flow de jeu

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

## 11. Profiling

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
