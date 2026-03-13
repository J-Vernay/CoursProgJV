# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Fichier header qui stock les déclaration des fonctions et des variables globales
>
> `dino/x64-windows/` Dossier contenant les fichiers qui permetent l'implémentation de la fenetre et des visuels du jeu
> sur windows
>
> `dino/x64-linux/` Dossier contenant les fichiers qui permetent l'implémentation de la fenetre et des visuels du jeu
> sur linux
>
> `dino/dino_game.cpp` : Fichier qui implémente les fonctions principale de la logique du jeu
>
> `dino/dino_geometry.cpp` : Fichier avec une helpers methode qui verifie que 2 segments ont une intersection
>
> `dino/dino_draw_utils.cpp` : Fichier qui possède divers fonctions qui permettent de dessiner dans le moteur
>
> `premake5.lua` : Fichier qui donne des indications pour bien executer la build

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

> La distance parcouru est de 6 pixels (300 * 0.02f)

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> en bas à gauche (x negatif et y positif), à une vitesse de 5000 pixels par secondes

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Il se trouvera en (171, 129)

f) Par quoi est définit un triangle texturé ?

> Par 3 sommets, ayant des informations de position, une texture, d'uv et de normal.

g) Qu'est-ce qu'est un "draw call" ?

> Le CPU qui envoie les infos de géométries et de texture à la ram pour pouvoir afficher.

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> une fenetre de debug avec diverses infos (vertexBuffer, texture, allocation de mémoire)

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> Cela permet d'avoir plusieurs mods de compilation, cela permet d'ajouter et de retirer des features ou modifier des
> configurations. Elles sont arbitraires et créent en fonction des besoins du projet.

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> un tableau à taille dynamique de différent types, dans ce cas ce sont des vectors

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> Elle signifie que l'on va utiliser une reference de valeur, sans elle cela signifiera qu'on utilisera la valeur en
> elle meme (et donc en faire une copie)

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> En inversant les UV.

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> UFrame = ((temps x VitesseFrame) % NbFrame) x 24 + Ubase. Le V permet de changer le dino affiché (dans ce cas-ci c'est
> la couleur)

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> Créer une struct DinoPlayer qui possède les variables et créer une variable de cette struct pour stocker les infos de
> chaque player dans une variable chacun.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> On appelle ça une fonction membre (ou une methode), on l'utilise grace a l'encapsulation. On peut limiter la
> modification grace aux specificateurs d'acces (public, private). Cela permet de proteger les données, garantir que les
> modifications viennent de fonctions prévus,
> etc.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> c'est for (DinoPlayer& player : g_players). Elle s'appelle une boucle for.

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> Car on définit la taille de l'ecran en pixel à l'avance, si on dépasse la taille ce n'est plus affiché

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> dx = (renderingW - rectangleW) / 2 , dy = (rdrH - rectangleH) / 2

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> 1) Generer toutes les positions possibles
> 2) Melanger aleatoirement le tableau
> 3) Recuperer les positions des fleurs

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> Les 6 dossiers sont : "external\pix, external\sokol, external\stb,(Pour le code externe qui est utilisé pour le
> moteur) src\dino (le gameplay), src\dino\x64_linux, src\dino\x64_windows (moteur plateforme)"

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> Ils sont dans build/obj et src/external. Chaque src à son fichier obj. La suite de leur path dépend de la platforme et
> de la configuration.

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

> On obtient 3 excutables (demo, debug et release) ils sont l'un à al racine de cours prog et les 2 autres dans
> build\x64-windows\debug et release respectivement.
>

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> On a la ligne 93 pour la compilation des bibliothèques externes : files { "external/.cpp", "external/.h" }
> On a la ligne 107 pour la build windows: files { "src/dino/x64-windows/" }
> On a la ligne 1015 pour la build linux : files { "src/dino/x64-linux/" }
> Les particularités du débogguage sont définies plus haut dans le fichier.

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Le h est un fichier de déclaration, et le cpp est un fichier de définition dans lequel
> la logique des méthodes est implémenté.
>
> **Fichiers `.cpp` et `.obj` :** Le fichier en .obj est un fichier cpp compilé, il contient donc le code en binaire du
> .cpp, mais il n'est pas tout a fait complet
>
> **Fichiers `.obj` et `.lib` :** Le fichier .Lib contient des fichiers .obj, c'est le linker qui ira piocher dedans
> lorsqu'il en aura besoin.
>
> **Fichiers `.obj` et `.dll` :** Le fichier .dll contient également des fichiers .obj, mais celle-ci n'est pas intégré
> dans l'exécutable final.
>
> **Fichiers `.obj` et `.exe` :**  Ces deux fichiers sont lié par un linker qui va s'occuper des différentes liaisons
> entre les fichiers et combler les trous.
>
> **Fichiers `.dll` et `.exe` :**  Le lien entre les deux dépend du projet, certains projets chargent la .dll au
> démarrage uniquement et d'autres non.

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Il agit avant la compilation, il ne s'agit de code en c++, mais de la manipulation de texte afin de donner des
> directives spécifiques au compilateur. Une directive de préprocesseur se reconnait avec le character # devant
> l'instruction.

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> Comme le préprocesseur, il intervient avant la création du .exe et s'occupe de correctement lier les fichiers du
> projet entre eux (pour résoudre les références etc..). Il peut produire un exécutable et des bibliothèques (.lib /
> .dll).

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> Il suffit de vérifier la distance entre les deux et de la comparer à leur rayons

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> On prends la direction entre les deux cercles que l'on divise en deux, chacun des deux cercle est repoussé de la
> direction opposé à l'autre.
> d = ((Ra+rb) - ab) / pas eu le temps de noter
> Le risque est la division par zero si les 2 sont parfaitement au meme endroit

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> On essaye d'implémenter la même fonctionnalité sur des types différents

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> L'héritage permet de créer des fonctions communes à toutes les classes qui en hérite

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> Le polymorphisme permet ce genre de chose, on peut par exemple override une fonction si l'on veut un fonctionnement
> précis pour une classe ayant le même héritage.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> Les méthodes gérant les collisions et les limites de terrain sont des méthodes qu'on peut utiliser ce principe.

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> Créer un tableau de pointeur de dino Entity, et acceder aux references de ses éléments pour appeler les méthodes dont
> on a besoin.

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> On va utiliser .erase pour supprimer la dernière position de la liste si elle contient plus de Lasso_time * update per
> seconde (soit 60 x 2 = 120) positions.

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> On a 12 vérifications par frame On utilisera la méthode .size() pour ne pas calculer toutes les positions du lasso.

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> On va tracer une ligne entre un élément du jeu (par exemple un joueur) et un point hors de la zone de jeu, Ensuite, au
> moment de valider une boucle de lasso, on vérifie le nombre de collisions entre cette ligne et le lasso, En fonction
> du
> nombre cela détermine si le joueur est dans le lasso ou non.

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 32Go de RAM Il y a 18.446.744.073.709.551.616 adresses mémoires différentes, c'est à dire un espace addressable de 18
> milliards Go, mais cette valeur est limitée par la RAM physique présente dans l'ordinateur

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Le programmedemande à l'OS de l'espace dans la mémoir pour y stocker des valeurs qui lui sont/seront utiles. L'OS
> cherche un emplacement dans la RAM, "Map" (récupère l'adresse où il a stocké) puis retourne l'adresse (pointeur) au
> programme. Une allocation mémoire est d'autant plus couteuse que la taille demandée augmente.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Les types correspondent à une abstraction des octets pour les rendre plus facilement manipulables dans le code. Il
> définissent comment interpréter une séquence d'octets.
> Un pointeur correspond à une adresse mémoire (dynamique) et un Type (statique) Réinterpréter un pointeur signifie lire
> la valeur stocké à son adresse mais en utilisant le prisme d'un autre Type

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor : 4 octets (union dans uint32_t)
> DinoVertex : 16 octets (8 + 2 + 2 + 4)

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> std::vector représente un liste de plusieurs éléments de même type, cela est représenté en mémoir par une longue
> chaine d'octets dont la taille est égale à : Taille = TailleElement * NombreElement.
> Ainsi, pour retrouver un élément à un indice donné, il suffit d'aller à l'emplacement du vector et de se décaler de
> indice*TailleElement
> Cela signifie que pour changer la taille d'un std::vector (augmenter), il faut déplacer tous les éléments pour trouver
> un nouvel espace mémoire d'une taille adéquate.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Les variables globales sont connues à la compilation, l'os va allouer cette zone avant d'appeler la fonction main.
> Les variables locales (stack) allouée par l'os avant la fonction main
> Les std::Vector ont une taille dynamique qui signifie une allocation mémoire via l'OS

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> DinoVec2 operator+(DinoVec2 a, DinoVec2 b);
> DinoVec2 operator*(DinoVec2 a, float b);
> DinoVec2 operator*(float a, DinoVec2 b);

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> Car elles sont de base à {255, 255, 255, 255} en paramètre par défaut dans le constructeur. Ce dernier est appelé
> implicitement quand on declare un DinoVertex.

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

> emplace_back

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> En symétrie du constructeur en c++, quand une variable est détruite, le compilateur appelle automatiquemen le
> destructeur. Le compilateur appelle donc implictement le destructeur de std::vector

e) Créer une classe `DinoVertexBuffer` qui accepte les mêmes arguments que `XDino_CreateVertexBuffer()`
et garde le `vbufID` dans un membre privé, avec une fonction `Get()` qui retourne ce `vbufID`.
Faites en sorte que `XDino_DestroyVertexBuffer()` soit appelé automatiquement.
Remplacez les usages des vertex buffers qui sont créés à chaque frame.

f) Que se passe-t-il lorsqu'on copie un `DinoVertexBuffer` vers un autre ? Comment le prévenir ?

> Le jeu crash instantanément car les 2 DinoVertexBuffer essayent de detruire le vertexBuffer et donc essaye de detruire
> quelque chose de vide

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
