# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : contient toutes les déclarations des fonctions utilisées dans le fichier xdino.cpp (il s'agit des
> logiques du jeu comme l'appel de chaque frames, le chargement de sprites , etc..)
>
> `dino/x64-windows/` : il s'agit du dossier qui contient les fichiers qui gerent la relation entre leu jeu et windows (
> ouverture de fenetre, etc...)
>
> `dino/x64-linux/` : il s'agit du dossier qui contient les fichiers qui gerent la relation entre leu jeu et linux (
> ouverture de fenetre, etc...)
>
> `dino/dino_game.cpp` : il s'agit du fichier qui contient la logique de toutes les fonctions déclarées dans xdino.h, il
> gere donc la logique du jeu.
>
> `dino/dino_geometry.cpp` : ce fichier contient une seule fonction (un booleen nommé Dino_IntersectSegment) qui vérifie
> si les deux segments [AB] et [CD] ont une intersection.
>
> `dino/dino_draw_utils.cpp` : ce fichier contient trois fonctions qui permettent de dessiner des formes et textes en
> pixels
>
> `premake5.lua` : il s'agit du fichier qui défini comment le projet doit etre interpreté par l'orditnateur pour la
> build, on y retrouve des informations comme la version du langage, de windows, etc...

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des
`// COMMENTAIRE`.

```cpp

// Fonction qui gère les événements que nous communique le système d'exploitation.

```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> 6

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> il s'est déplacé en bas a gauche a une vitesse de 5000 px/s

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> en 171,129

f) Par quoi est définit un triangle texturé ?

> 3 sommets (chacun a une position a l'ecran et une coordonné UV) ainsi qu'une texture

g) Qu'est-ce qu'est un "draw call" ?

> c'est le CPU qui dit a la ram quelle geometrie et quelle texture appliquer sur chaque triangle texturé au GPU et qui
> demande de réaliser l'opération d'application des textures sur les triangles

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> une fenetre de debug qui montre notemment que une fuite de memoire se produit

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> elles servent a compiler notre base de code differemment, le nommage est arbitraire

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> c'est un container selon la documentation, il s'agit d'un type de donnée (je ne suis pas sur de savoir le definir
> proprement) c'est un tableau a taille dynamique

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> c'est pour dire qu'on passe la reference de la case mémoire de la donnée pour s'assurer qu'on édite bien la donee en
> question plutot qu'en créer une copie

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> en inversant les UVs sur le triangle de position

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> (timeSinceStart * animSpeed) % frameCount) * spriteSizeInPxl + ubase

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> en utilisant une struct qui contient toutes les variables nécéssaires

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> on utilise le therme "fonction membre"
> on utilise des outils qui limitent l'accessibilité a ces fonctions comme private et public
> ca permet d'indiquer la portée d'une fonction, de ne pas l'appeler dans un contexte qui ne serait pas adapté.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> for range

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> parce qu'on force la résolution a etre en 4/3 plutot qu'en 16/9

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> ...

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> on commence par faire une liste de toutes les positions de flaurs possible sous forme de vecteur 2,
> mon mélange ensuite cette liste
> on prend les dix premiers elements de cette liste et on y instantie 1 flaur de type 1
> pour les dix elements suivants de la liste, on prend la flaur 2 etc

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> external pix
> external solcol
> extrenal stb
>
> src dino // gameplay
>
> src dino x64windows
> src dino x64linux

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> tout les fichiers obj sont dans le dossier build/obj, la suite de leurs path dépend de la plateforme et de la
> configuration que l'on utilise
> chaque source a son fichier obj qui lui est lié

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

> ...

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> ...

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** préprocesseur
>
> **Fichiers `.cpp` et `.obj` :** compilateur
>
> **Fichiers `.obj` et `.dll` :** linkage static
>
> **Fichiers `.obj` et `.lib` :** archivage
>
> **Fichiers `.obj` et `.exe` :** linkage static
>
> **Fichiers `.dll` et `.exe` :** linkage dynamique

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> il sert a assimiler toutes les substitutions afin de pouvoir compiler le code, c'est la compilation conditionnelle
> il sert aussi a remplacer les includes par les morceaux de code qu'ils remplacent

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> l'editeur de lien peut produire des fichiers .dll et des fichiers .exe
>
> la difference majeure entre les deux types de fichiers est que le dll est unique et nedépend pas d la recompilation,
> ce qui permet de mettre a jour le dll sans changer le .exe

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> en compartant la distance entre chacun des cercles et en verifiant si la magnitude au carré du vecteur résultant est
> plus petite que la taille d'une collision

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> on cherche le vecteur qui sépare les deux cercles,on le divise en deux, le multiplie par -1 et on l'additionne a la
> position des deux cercles. a noter que ce calcule n'est pas possible si les deux cercles sont parfaitement supperposés

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> parce que les dino collident entre eux de la meme maniere que les animaux doivent le faire, il ne s'agit pas de
> fonction membre mais elles ne fonctionnent qu'avec leurs propre classe

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> l'héritage pour rendre commune les classes (entity)

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> la surcharge (override) des fonctions communes. ca s'appelle le polymorphysme.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> les collisions entres les entités

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> on créé un std::vector d'entities et on créé un algorythme de tri qui les met dans l'ordre selon m-pos.y
> (on va devoir recréer une methode draw dans la classe entity)
> ensuite on Draw les elements a l'ecran dans l'ordre du tableau

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> m_lassoPos.erase(m_lassoPos.begin());

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> comme on ne check la collision qu'avec le drnier segment du lasso, on appelle la fonction "CheckCollision"
> NombreDeJoueur * LongueurDeChaqueLasso (en l'occurence 4*120 = 480)

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> on calcule le nombre d'intersection entre le contour et le segment défini par "position qu'on sait a l'exterieur" et "
> position checkée"
> si ce nombre d'intersection est pair (0 compris) la position checkée est a l'exterieur du contour, sinon a l'interieur

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> mon pc possede 32Go de ram en tout et dispose en moyenne sans programe trop gourmand de 16Go disponibles
> 18 446 744 073 709 551 615 octets (2 exposant 64) sont adressables sur un processeur 64 bits
> on ne peut pas lire ni ecrire a des espaces memoires qui ne sont pas liés a des emplacements de ram physique

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> reserver un espace mémoire afin que la donée soit sauvgardée et qu'il ne soit plus considéré comme disponnible (on le
> malloc)
> non cette opération n'est pas couteuse

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> un type reprenente la facon dont l'ordinateur doit interpreter une sequence d'octets
> un pointeur est une adresse memoire (dynamique) + un type (static)
> cela signifie changer le type contenu dans un pointeur pour changer l'interpretation de la sequence d'octet qu'il
> contient

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor a une taille de 16 octets, soit 64bits

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> ...

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> les variables globales sont connues a la compilation, le fichier exe connait le nombre d'octets a allouer aux
> vairables globales => l'os va donc l'allouer avant d'apeler la fonction main
> (stack) allouées par l'OS avant la fonction main, le compilateur va ensuite générer du code pour sous alouer les
> variables locales dans la stack
> taille dynamique => allocation memeoire par l'OS

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> NomDeClass Opéraor + (type other), NomDeClass Opérator * (type other), etc ...

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> parce que c'est spécifié dans son constructeur par défault

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

> emplace_back

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> parce que std::vector possede son propre Destroyer

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
