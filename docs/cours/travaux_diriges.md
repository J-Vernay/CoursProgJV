# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Fichier déclarant les fonctions du gameplay ainsi que les interfaces pour le rendu graphique, les
> entrées et la gestion mémoire.
>
> `dino/x64-windows/` : Dossier contenant l'implémentation spécifique à Windows du moteur du jeu permettant la gestion
> de la fenêtre, du rendu et des fichiers nécessaires pour faire fonctionner le moteur sur x64 Windows.
>
> `dino/x64-linux/` : Dossier contenant l'implémentation spécifique à Linux du moteur du jeu permettant la gestion de
> la fenêtre, du rendu et des fichiers nécessaires pour faire fonctionner le moteur sur x64 Linux.
>
> `dino/dino_game.cpp` : Fichier contenant l'implémentation de la logique de jeu, l'initialisation du jeu, l'update de
> l'état des objets, la gestion des entrées du joueur...
>
> `dino/dino_geometry.cpp` : Fichier contenant les fonctions géométriques utilitaires utilisées dans la logique du jeu
> pour la détection de collisions etc.
>
> `dino/dino_draw_utils.cpp` : Fichier contenant des fonctions pour générer les sommets (DinoVertex) des formes à
> dessiner.
>
> `premake5.lua` : Fichier qui décrit comment générer le projet, solutions et configurations, les chemins, fichiers
> sources à compiler, dépendances externes...

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

// Fonction qui gère les événements que nous communique le système d'exploitation.

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

> Une distance de 6px a été parcouru entre ces deux frames.

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Le cercle s'est déplacé dans la direction bas-gauche. Cela correspond à une vitesse de 5000 pixels par seconde.

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Le cercle est à la position (171;129) après une seconde.

f) Par quoi est définit un triangle texturé ?

> Un triangle texturé est définit par 3 vertex : DinoVertex. Cela correspond à 3 sommets (positions dans l'espace) et 3
> coordonnées de texture (UV) associées à ces sommets.

g) Qu'est-ce qu'est un "draw call" ?

> Un draw call est une instruction envoyée par le CPU au GPU pour demander le rendu d'un objet graphique avec une
> géométrie et une texture.

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> Quand on maintient cette touche en jeu les ressources actuellement chargées en mémoire dans le moteur graphique sont
> affichées. Dans la sortie textuelle quand on quitte le jeu on y retrouve les ressources qui ont été libérées.

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> Deub, profile, release sont des modes de compilation et d'exécution du programme. Exemple : Lancer le jeu en Debug
> pour vérifier pourquoi le visuel du personnage ne bouge pas.

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> `std::vector` est un tableau de la bibliothèque standard C++ qui a une taille dynamique. Il stocke des éléments à la
> suite.

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> L'esperluette `&` indique une référence. Si l'esperluette `&` est enlevée, out serait passé par valeur, ce qui
> signifie qu'une copie du vecteur serait créée. Coût en performarmance possible car une copie est créée à chaque appel.

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> Pour mettre en miroir le sprite du dinosaure on peut directement toucher aux UV en inversant les coordonnées U des
> vertices du carré.

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> `frameIndex = (timeSinceStart * framesPerSecond) % nbFrames` puis `u = frameIndex * largeur_sprite`.

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> On peut créer une classe Dino qui contiendra toutes les variables globales qui définissent l'état du dinosaure.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> En C++ on appelle une fonction associée à un type de donnée `une méthode`. Pour limiter la modification d'un type de
> données à ce genre de fonction on utilise `private`. Cette limitation est appellée `l'encapsulation`. L'intérêt
> est d'éviter les modifications non désirées, contrôler l'accès et protéger les données.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> foreach, code :
> for (type variableNom : nomTableau)
> {
> //Code à executer
> }
>
> Cette syntaxe est appelée : `range-based for loop`.

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> La résolution du rendu n'est pas adapté, actuellement il reste au format 1:1 (carré) peu importe les modifications
> apportées à la fenêtre.

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> Il faut calculer la différence de taille et la répartir de chaque côté pour obtenir la bonne taille des marges.
> Horizontalement : marge = (largeur_fenetre - largeur_rectangle) / 2 = (480 - 256) / 2 = 224 / 2 = 112.
> Verticalement : marge = (hauteur_fenetre - hauteur_rectangle) / 2 = (360 - 192) / 2 = 168 / 2 = 84.

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> On implemente la grille des tuiles pour placer les fleurs sans superpositions (TILE_SIZE / TILES_X / TILES_Y).
> Ensuite on vient créer la liste de toutes les cellules disponibles, qui représentent les positions possibles pour les
> fleurs.
> On mélange les cellules avant d'attribuer les fleurs.
> Pour chaque type de fleurs (3 types au total), on génère 10 instances.
> On créer une fleur (position, type de fleur).
> Pour chaque fleur, on lui donne la cellule suivante dans la liste mélangée (pour éviter les superpositions) et on lui
> donne son type.
> On répète 10 fois pour chaque type.
> Ensuite on applique l'offset pour les UV correspondants à la saison désirée.

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h`. Quels sont les 4 dossiers du projet à contenir des fichiers C++ ?

> external pix
> external sokol
> external stb
> src\dino
> \x64 linux
> \x64 windows

c) Cherchez `CoursProgJV *.obj`. Que remarquez-vous des noms des fichiers concernés ? Notez leur chemin.

> Chaque fichier `.cpp` a son fichier `.obj` associé.

d) Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leur chemin.

> Un `.exe` de la démo à la racine.
> Un `.exe` par configuration (Debug/Profile/Release) dans le dossier Build\obj\x64-windows\Debug release.

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> Pour les `.h` et .cpp, ligne 101 : files {"src/dino/*""}
> Pour les `.obj`, ligne 16 : location "build"
> Pour les `.exe`, ligne 19 : targetdir "build/%{cfg.platform}/%{}cfg.buildcfg"

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Les fichiers `.h` contiennent des déclarations (fonctions, classes, constantes) que le
> préprocesseur inclut dans les fichiers `.cpp` via #include.
> Cela permet au compilateur de connaître les interfaces avant la compilation.
>
> **Fichiers `.cpp` et `.obj` :** Les fichiers `.cpp` sont compilés par le compilateur pour produire des fichiers objets
> .obj.
>
> **Fichiers `.obj` et `.lib` :** Les fichiers `.lib` sont des archives de fichiers `.obj`.
>
> **Fichiers `.obj` et `.dll` :** Les fichiers `.dll` sont des bibliothèques dynamiques créées à partir de `.obj`.
>
> **Fichiers `.obj` et `.exe` :** L’éditeur de liens combine les `.obj` pour produire un fichier exécutable `.exe`.
>
> **Fichiers `.dll` et `.exe` :** Un `.exe` peut utiliser des `.dll` à l’exécution pour partager du code ou réduire la
> taille du programme.

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Le rôle du préprocesseur est de préparer le code source avant la compilation proprement dite.
> Le préprocesseur traite les directives commençant par #, telles que :
> #include
> #define
> Etc.

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> L’éditeur de liens regroupe les fichiers objets `.obj` et les bibliothèques pour produire des fichiers `.exe` ou
> des bibliothèques (`.dll` ou `.lib`) qui permettent de partager du code sans distribuer le code source complet
> contrairement au `.exe`.

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> Deux cercles à des positions données sont en collision si la distance entre leurs centres est inférieur ou égal à la
> somme de leurs rayons. Il faut donc vérifier cette distance.

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> Pour que deux cercles ne soient plus en collision, il faut les déplacer le long de la normale qui relie leurs centres.
> On ne peut pas résoudre le cas où les centres des cercles sont exactement au même point.

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> Il y a de la duplication de code car la logique pour gérer les collisions est la même pour les dinosaures et les
> animaux.

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> On peut dédupliquer la logique en utilisant de l'héritage de class.

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> En C++ on peut utiliser l'héritage de class et le polymorphisme.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> On peut mettre en commun "Drawn" et "Update".

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> Il faut faire hériter ces deux class du même parent.

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> On utilise la méthode : erase(begin(), end()) pour supprimer les points les plus anciens. (FIFO : First In, First Out)
> pour le lasso.

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> NbJoueur * LongueurLasso = 4*120 = 480 intersections. On utilise toujours std::vector::erase().

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> On récupère la position d'un point qui on sait est en dehors de la forme quoi qu'il arrive et on trace un segment
> entre ce > point et la position qu'on veut détecter.
> Si le segment n'a aucune intersection, ou s'il en a un nombre pair, alors on sait qu'on est à l'extérieur, et
> inversement.
>
> Un cas très rare peut arriver où le segment serait une tangente du tracé et aurait donc un nombre d'intersections
> impair alors que la position qu'on veut détecter est en dehors du tracé. (on pourrait réduire les probabilités en
> prenant plusieurs points en dehors de la forme)

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> Sur ma machine, je possède 32 Go de RAM. Dans un programme 64 bits, l’espace d’adressage théorique est de 2^64
> adresses. Un programme ne peut lire ou écrire que dans les zones de mémoire virtuelle qui lui sont allouées par le
> système d’exploitation. La mémoire réellement utilisée peut être limitée par la RAM physique.

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Allouer de la mémoire signifie qu’un programme demande au système d’exploitation de réserver une zone de mémoire pour
> y stocker des données.
> Le système d’exploitation réserve un espace dans la mémoire virtuelle du processus et renvoie au programme l’adresse
> de
> début de cette zone sous forme de pointeur.
> L’allocation a un coût et elle peut devenir plus coûteuse lorsque la taille demandée est grande ou lorsque les
> allocations sont fréquentes.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> En C++, un type décrit comment une suite d’octets doit être interprétée et manipulée par le programme (taille en
> mémoire, opérations possibles, représentation des données). Un pointeur est une variable contenant l’adresse mémoire
> d’une donnée d’un certain type. Réinterpréter un pointeur signifie traiter l’adresse pointée comme si elle contenait
> un objet d’un autre type, on interprète les mêmes octets en utilisant un type différent.

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor fait 4 octets et DinoVertex fait 16 octets.

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> std::vector représente une liste de plusieurs éléments de même type.
> En mémoire, les éléments sont placés côte à côte, comme dans un tableau dynamique.
> L’adresse d’un élément d’indice i peut être calculée par :
> addr(i) = begin + i × sizeof(T)
> où begin est l’adresse du premier élément et T le type stocké.
> Cela impose que tous les éléments aient la même taille. De plus, lorsque la capacité du std::vector est
> dépassée, il doit allouer un nouveau bloc mémoire plus grand et copier tous les éléments, ce qui peut être coûteux.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Variables globales :
> La mémoire des variables globales est déterminée à la compilation et lors du chargement du programme, le système
> d’exploitation réserve ces zones mémoire avant l’exécution de main.
> Variables locales :
> Les variables locales sont allouées à l’appel de la fonction, et le compilateur génère du code qui réserve
> l’espace nécessaire à l’entrée de chaque fonction.
> std::vector :
> Un std::vector est alloué pendant l’exécution du programme via l’allocateur C++ (new / malloc).

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> En C++, on surcharge les opérateurs avec la syntaxe operator+ et operator*.

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> La couleur possède une valeur par défaut (DinoColor_WHITE). Donc lorsque l’on crée les sommets pour un sprite sans
> préciser la couleur, celle-ci est automatiquement blanche, ce qui signifie aucune modulation de la texture.

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

> Syntaxe du constructeur, la méthode est pareil que le type, et n'as pas de retour :
> class DinoPlayer {
> public :
> DinoPlayer(int idxPlayer); // Si pas d'argument, constructeur par defaut.
> // Le constructeur est une méthode avec le nom du type
> // Le constructeur n'a pas de type de retour.
> };
> DinoPlayer::DinoPlayer(){
> }
>
> La méthode est : emplace_back(). Par exemple :
> g_Players.resize(4) // Ne compile plus car pas de constructeur par défaut.
> g_Player.emplace_back(XXX); // XXX = Arguments transférés au constructeur de l'élément.
> Elle construit l’objet directement dans le std::vector en utilisant les arguments fournis, ce qui évite une copie
> intermédiaire.

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> Implicitement pour std::vector, le compilateur appelle le destructeur automatiquement et la mémoire qu'il contient est
> libérée automatiquement, aucun appel manuel n’est nécessaire.
> En C++, quand une variable est détruite (quand elle n'est plus accessible), le compilateur appelle automatiquement le
> destructeur.

e) Créer une classe `DinoVertexBuffer` qui accepte les mêmes arguments que `XDino_CreateVertexBuffer()`
et garde le `vbufID` dans un membre privé, avec une fonction `Get()` qui retourne ce `vbufID`.
Faites en sorte que `XDino_DestroyVertexBuffer()` soit appelé automatiquement.
Remplacez les usages des vertex buffers qui sont créés à chaque frame.

f) Que se passe-t-il lorsqu'on copie un `DinoVertexBuffer` vers un autre ? Comment le prévenir ?

> Si on copie un DinoVertexBuffer vers un autre, les deux objets contiennent le même vbufID et leurs destructeurs vont
> appeler XDino_DestroyVertexBuffer() deux fois, donc une double destruction se produit. On peut le prévenir en
> empêchant la copie.

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
