# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : fichier header qui définie l'API d'interaction entre le moteur de jeu "XDino" et la logique de jeu
>
> `dino/x64-windows/` : Tous les fichiers qui sont pour le rendu graphique sur windows
>
> `dino/x64-linux/` : Tous les qui sont pour le rendu graphique sur linux
>
> `dino/dino_game.cpp` : fichier cpp qui implémente les fonctions principales de la logique de jeu
>
> `dino/dino_geometry.cpp` : fichier cpp qui implémente des fonctionnalités géométriques
>
> `dino/dino_draw_utils.cpp` : fichier cpp qui implémente les fonctions pour dessiner les primitives de rendu
>
> `premake5.lua` : fichier de config pour le projet, de compilation, gestion des erreurs etc...

b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des
`// COMMENTAIRE`.

```cpp

```

c) Je dirige le cercle vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcouru entre ces deux frames ?

> 6

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> déplacement vers haut gauche
> 5000px /s

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> 71,-71

f) Par quoi est définit un triangle texturé ?

> un triangle avec 3 sommet dans l'espace, des coordonnées pour les uv et une texture

g) Qu'est-ce qu'est un "draw call" ?

> un draw call c'est quand le cpu envois des informations du mat/mesh sur le gpu pour le rendu graphique

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> affiche une fenetre debug des ressources sur les textures et les vertexs buffer

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> le nomage a été fait de manière arbitraire
> ils possède des comportements différent de macro du compilateur
> permet par exemple de compiler certains bout de code ou pas en fct des config

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> tableau a taille dynamic de variable.
> un conteneur de donnée

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> C'est une référence a la donnée et si on enlève on fait une copie de la donnée en mémoire.
>

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> On echange la position des UV en U en fonction de si on va a droite ou a gauche.

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> On connait le nombre de frame par animation donc avec le time dans un update on peut faire défilé l'animation en le
> décalant
> dans la texture. Avec un calcule modulo et les bonnes conditions en fonction de l'action du joueur on a la bonne
> animaiton

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> Une classe player avec toutes les variables le concernant directement

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> On parle d'encapsulation dans une class / struct qui contient des methods membre

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> avec un foraech, ici on utilise des pointeurs directement sur les éléments dans la mémoire

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> Car les éléments dans le jeu ne sont pas responsive

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> dx = (480 - 256) /2 et dy = (360 - 192) /2

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> La premier étape est de généré toute les postions possibles pour les fleurs.
> Ensuite on peut mélanger le tableau généré. Puis on récupère les postions des fleurs.

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> External pix, external sokol, external stb, src\dino (gameplay) ,\x64 linux et windows (moteur platform)

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> Chaque fichier source a son fichier obj (source external et build obj)
> il y a obj pour chaque platforme et configuration

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

> Exe de démo, exe de debug et exe de release

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> src\External pour les .h et .cpp
> Build\obj\x64-windows\Debug Relase pour les .obj
> Build\x64-windows\DebugRelease pour les .exe

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** ...
> Etape préprocesseur qui transforme les includes/ define/ if endif / pragma
> les # → directive de précesseur
> **Fichiers `.cpp` et `.obj` :** ...
> Le compilateur vérifie les règles du cpp, transformation du code cpp en code assembleur (code natif)
> **Fichiers `.obj` et `.lib` :** ...
> archivage:  les fichiers .lib sont un archivage des .obj
> **Fichiers `.obj` et `.dll` :** ...
> edition des liens :  les .dll sont comparables au .exe mais sans 'main', ils ne peuvent donc pas être executé seuls
> **Fichiers `.obj` et `.exe` :** ...
> editions des liens : le .exe est créé à partir des .obj lorsqu'on fais une "edition des liens static"
> **Fichiers `.dll` et `.exe` :** ...
> edition de liens dynamique :  certains .exe peuvent utiliser des .dll lors de leur execution

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> On reconnait les directives de préprocesseur avec le #
> Programme qui réalise des traitements sur le code source avant qu'il ne soit réellment compilé

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> faire le lien entre les codes assembleurs contenu dans plusieurs fichiers obj.
> il produit des fichers exe et obj.

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> On peut mettre une range a chaque position de chaque dino. Ici on va utiliser 16 px, donc si on récupère
> position + 16px de range on peut savoir si on est en collision. C'est le calcule de AB

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> En récupérant la position des deux entités, on calcule la distance entre les deux centres, s'il y a chevauchement
> on les sépare dans la direction opposés. Attention, si les deux cercles ont la meme positions, cela ne marchera pas
> du fait que ça fait une division par 0.

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> On a besoin de gérer la collision entre deux entités différents, pour le moment on gère ceci directment dans leur
> class
> respective. Les dino etant pas des animaux, on duplique le code.

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> On peut utiliser une class qui gère les collisions et la faire hérité aux entités qui ont besoin de comportement
> commun
> ici on veut gérer la physique de tout entity qui ont en besoin.

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> La surcharge de l'héritage, c'est à dire la redifinition de la méthode avec le mot clé overide

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> On peut mettre en commun la méthode Draw comme methode virtuel. Ce qui permet a ceux
> qui
> en hérite de la redéfinir avec leur propre logique

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> EN utilisant le polymorphisme, on peut directement manipuler des références de dino entity qui sont des dino animal ou
> player
> pour autant la logique à l'intérieur est bien différentes.

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F5.1 .

b) Implémentez la fonctionnalité F5.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> std::vector::erase() pour supprimer les points dont le timestamp est trop vieux

c) Implémentez la fonctionnalité F5.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> (119 × 118) / 2 = 7021 comparaisons par joueur
> × 4 joueurs = 28 084 comparaisons au total
> C'est toujours la méthode std::vector::erase()

d) Implémentez la fonctionnalité F5.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> Si le point est a l'intérieur il va couper le contour du lasso sinon le point est l'extérieur

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 32go
> 18 millaird d'octets sont adressables
> On peut pas lire ou écrire partout, meme si en théorie on peut. On peut seulement sur les la mémoire que l'os
> a allouée au programme

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Allouer de la mémoire veut dire qu'on réserve dans la ram des octets pour stocker de l'information.
> relativement oui, il faut faire un appel de l'os pour trouver un bloc de mémoire assez grand pour stocker l'
> information.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> un type est une séquence d'octets, comment interpréter une variable
> Un pointeur est une adresse mémoire en plus du type
> réinterpréter un pointeur c'est le fait de lire les octets en mémoire mais avec un type différent

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> Dino color fait 12 octets (struc : 4, uint8_t rgba : 1 1 1 1 doc 4 et uint32_t rgba_t : 4 octets)
> DinoVertex fait 32 octets ( Struc : 16 octets, dinovec2 8, uint16_t u v : 2 2, dinocolor : 4 octet)

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> Zone mémoire unique et collaspe (toutes les données sont a la chaine)
> Comme les données sont collé, et comme on connait leur taille en mémoire, il suffit de prendre le début et de
> décaller a chaque fois, ici on décale de 16 pour avoir l'élement suivant.
> il faut que le type soit le meme sinon cela peut vite devenir compliquer pour avoir accès à l'élement position x dans
> la mémoire

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Variable globale : Lors de la compilation l'OS va alouer de la mémoire dans la ram physique, cela est allouer avant
> l'appel de la fonction main
> de meme pour les variables locals elles sont alloué par l'os avant la fonction main, elles sont sous allouer dans le
> stack
> std::vector est un tableau dynamique, il est égallement alloué en mémoire via l'OS
> il est alloué avant chaque variables local et globale et libéré en dernier

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> type de return + mot clé operator + symbole (+ * -) + parametre
> puis return { a.x * b, a.y* b}

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> On a pas besoin de la spécifier car la valeur par défaut est déjà blanc (255 *4)
> or la multiplication serait de 1 sur chaque canal

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

> On remplace Init() par un constructeur
> La méthode std::vector qui construit sans copie est emplace_back(),
> elle transmet les arguments directement au constructeur de l'objet

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> Il est automatiquement appelé par le compilateur pour libéré la mémoire

e) Créer une classe `DinoVertexBuffer` qui accepte les mêmes arguments que `XDino_CreateVertexBuffer()`
et garde le `vbufID` dans un membre privé, avec une fonction `Get()` qui retourne ce `vbufID`.
Faites en sorte que `XDino_DestroyVertexBuffer()` soit appelé automatiquement.
Remplacez les usages des vertex buffers qui sont créés à chaque frame.

f) Que se passe-t-il lorsqu'on copie un `DinoVertexBuffer` vers un autre ? Comment le prévenir ?

> SI on détruit un objet, le deuxeime étant une copie, va pointer sur une valeur détruite.
> pour prévenir, on peut supprimé le constructeur par copie et l'opérateur d'assignement
> par copie avec un = delete, alors le compilateur va refusé la tentative de copie.

=========== NE PAS FAIRE ===========

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

=========== FIN DE NE PAS FAIRE ===========

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

## 11. Profiling

a) Compilez le jeu avec la configuration "Profile", puis lancez le jeu.

b) Prenez une capture PIX. ( https://devblogs.microsoft.com/pix/download/ )

* Dans **File > Settings > Symbol / PDB Options > PDB Search Paths**, ajoutez
  `srv*c:\symcache*https://msdl.microsoft.com/download/symbols` (même s'il y a un texte d'erreur).
* Dans **Local Machine > Capture > Attach** (à gauche), cherchez le nom de l'exécutable (`Dino_PrenomNom.exe`), et
  cliquez dessus.
* Décochez la case **For GPU Capture**, puis appuyez sur le bouton **Attach**.
* Dans **Capture**, cochez **CPU Samples** avec **8k / sec**, puis appuyez sur **Start Capture**.
* Jouez pendant quelques secondes.
* Cliquez sur **Stop Capture**.
* Allez dans l'onglet en haut **Timing 1.wpix**, puis dans l'onglet **Timeline**

c) En haut à gauche de la fenêtre, vous pouvez voir **Reload Symbols**. Cliquez dessus.
Cette étape télécharge la première fois environ 250 MiB de données dans le dossier `C:\symcache`.
Dépendamment de la connexion, cela peut prendre plusieurs minutes.

En utilisant Everything, quel type de fichiers se trouvent dans `C:\symcache` ? Quel est leur rôle ?

> ...

d) Sélectionnez une large bande dans la timeline. En bas à droite, sélectionnez **Item Type = Sampled functions**.

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

> **Créer un vertex buffer de 6 points :** ...
>
> **Drawcall d'un animal :** ...
>
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
