# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Ce script sert à déclarer des struct, variables et fonctions de base qui seront utilisées dans tous
> le reste du projet.
>
> `dino/x64-windows/` : Ce fichier regroupe différents scripts, chargés de faire la communication et l'affichage
> graphique sous windows.
>
> `dino/x64-linux/` : Ce fichier regroupe différents scripts, chargés de faire la communication et l'affichage graphique
> sous linux.
>
> `dino/dino_game.cpp` : Ce script contient les définissions de fonctions de base de la logique de jeu comme
> l'initialisation ou l'arrêt du jeu.
>
> `dino/dino_geometry.cpp` : Ce script contient des fonctions utilitaire pour la geometrie. Ne contient que "
> Dino_IntersectSegment" vérifiant si deux segments se croisent.
>
> `dino/dino_draw_utils.cpp` : Ce script contient des fonctions de base pour afficher des visuels dans la fenêtre.
>
> `premake5.lua` : Ce fichier donne des spécifications pour build le projet.

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

> Il a parcouru 6 pixels

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Le cercle se dirige vers la gauche et vers le bas. Cela correspond à une vitesse de 5 px/ms soit 5000px/s

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Il sera à la position (171,129)

f) Par quoi est définit un triangle texturé ?

> Il est définit par 3 vertex (DinoVertex) soit trois DinoVec2, trois coordonnées u/v sur une texture ainsi qu'une
> texture

g) Qu'est-ce qu'est un "draw call" ?

> C'est un appel fait par le CPU qui demande au GPU de faire un rendu (convertir les infos en pixels) en indiquant
> quelle géometrie et quelles textures

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> Cela permet d'afficher une fenêtre de debug montrant l'utilisation des ressources par le programme

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> Il permet de mettre a disposition des outils de debug que pour l'equipe de développement ou bien de gérer le niveau d'
> optimisation du compilateur.
> Elles sont arbitraires et créées pour les besoins du projet

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> std::vector correspond à une list d'élément de taille dynamique.

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> l'esperluette correspond à un pointeur, cela signifie que la fonction retourne un pointeur vers l'endroit où sont
> stockées les valeurs.

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> Le sprite du dinaursore peut être mit en miroir et échangeant les valeurs U des vertex de gauches avec ceux de droites

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> ((temps_écoulé * frame_per_secondes) % nombre_de_frames)*24 + U_base

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> Les variables relatives au mouvement d'un joueur peuvent être décallées au sein d'une class ou struct. Ainsi, chacune
> des variables devient propre à chaque joueur.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> On utilise l'encapsulation, une class/struct contient des method membres

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> Cette syntax est la for-range loop

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> Car le moteur garde la même résolution en permanence pour que les éléments à l'intérieur ne soient pas déformés.

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> pour Dx = (480 - 256) / 2 <br>
> pour Dy = (360 - 192) / 2

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> • La première étape consiste a récupérer toutes les positions possibles pour une fleur et de les stocker dans un std::
> vector. <br>
> • ensuite, on lance 3 boucle (une par type de fleur). <br>
> • dans chaque boucle, on choisi une case encore présente dans le std::vector => on prend un randomIndex. <br>
> • On ajoute la fleur à la position choisie. <br>
> • On enlève le position du std::vector pour éviter qu'elle soit choisie de nouveau.

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> external pix <br>
> external sokol <br>
> external stb <br>
> src\dino <br>
> src\dino <br>
> \x64 linux et windows

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> Chaque fichier cpp a un fichier obj qui lui est associé

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

> On obtient 4 fichiers différents : celui de démo à la racine, et un par configuration (Debug/Profile/Release) dans le
> dossier Build\obj\x64-windows\Debug release.

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> .h et .cpp = ligne 101 files {"src/dino/*""} <br>
> .obj = ligne 16 location "build" <br>
> .exe = targetdir "build/%{cfg.platform}/%{}cfg.buildcfg"

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** (preprocesseur) les .h aide le preprocesseur a interpréter les fichiers .cpp
>
> **Fichiers `.cpp` et `.obj` :** (compilation) les .obj sont créer par le compilateur grâce au .cpp
>
> **Fichiers `.obj` et `.lib` :** (archivage) les fichiers .lib sont un archivage des .obj
>
> **Fichiers `.obj` et `.dll` :** (edition des liens) les .dll sont comparables au .exe mais sans 'main', ils ne peuvent
> donc pas être executé seuls
>
> **Fichiers `.obj` et `.exe` :** (edition des liens) le .exe est créé à partir des .obj lorsqu'on fais une "edition des
> liens static"
>
> **Fichiers `.dll` et `.exe` :** (edition de liens dynamique) certains .exe peuvent utiliser des .dll lors de leur
> execution

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Le préprocesseur s'occupe de remplacer les '#' comme #include, #if, #pragma

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> Il se charge de regrouper tous les fichiers .obj en un seul gros fichier un ".exe". Il peut également produire des "
> .lib" ou ".dll" qui permettent de partager des fichiers précompiler pour éviter de donner accès a tous le code source
> lorsqu'on partage une base de code

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
> Si les deux cercles ont exactement la même position, alors ce calcul ne marchera pas (division par 0)

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> Car le calcul pour gérer les collisions est le même, qu'il soit pour un player ou un animal

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> On peut utiliser l'héritage de class

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> L'héritage de class et le polymorphisme

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> les fonctions "Drawn" et "Update"

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> les deux class doivent hériter de la même class parent

## 7. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Quelle méthode de `std::vector` utiliser ?

> on peut utiliser la méthode erase (en fournissant un debut et fin).

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?
Quelle méthode de `std::vector` utiliser ?

> Avec un peu d'optimisation, on peut réduire le nombre de calculs à un par segment soit 480.

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

e) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> en prenant un point situé à l'extérieur de la boucle, on trace le chemin entre celui-ci et le point que l'on souhaite
> vérifier tout en comptant le nombre de fois que l'on intersecte le contour de la boucle. Si le nombre d'intersection
> est par, le point est à l'extérieur sinon il est à l'interieur. (exception si le tracé entre les deux points est
> exactement la tangeante du tracé de la boucle)

f) Implémentez F5.6 et F5.7 via une logique commune, comme mentionné dans (6.h).

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 32Go de RAM
> Il y a 18.446.744.073.709.551.616 adresses mémoires différentes, c'est à dire un espace
> addressable de 18 milliards Go, mais cette valeur est limitée par la RAM physique
> présente dans l'ordinateur

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Le programmedemande à l'OS de l'espace dans la mémoir pour y stocker des valeurs qui lui
> sont/seront utiles. L'OS cherche un emplacement dans la RAM, "Map" (récupère l'adresse où il a stocké) puis retourne
> l'adresse (pointeur) au programme.
> Une allocation mémoire est d'autant plus couteuse que la taille demandée augmente.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Les types correspondent à une abstraction des octets pour les rendre plus facilement manipulables dans le code. Il
> définissent comment interpréter une séquence d'octets. <br>
> Un pointeur correspond à une adresse mémoire (dynamique) et un Type (statique)
> Réinterpréter un pointeur signifie lire la valeur stocké à son adresse mais en utilisant le prisme d'un autre Type

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor fait 4 octets <br>
> DinoVertex fait 16 octets

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> std::vector représente un liste de plusieurs éléments de même type, cela est représenté en mémoir par une longue
> chaine d'octets dont la taille est égale à : Taille = TailleElement * NombreElement. <br>
> Ainsi, pour retrouver un élément à un indice donné, il suffit d'aller à l'emplacement du vector et de se décaler de
> indice*TailleElement <br>
> Cela signifie que pour changer la taille d'un std::vector (augmenter), il faut déplacer tous les éléments pour trouver
> un nouvel
> espace mémoire d'une taille adéquate.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Variables globales : L'OS alloue une suffisamment grande pour stocker toutes les variables globales avant d'appeler la
> fonction "main" du programme <br>
> Variables locales : L'OS alloue un espace nécessaire pour la "stack" avant d'appeler la fonction "main". Le
> compilateur génère ensuite du code pour sous-allouer les variables locales dans le stack. <br>
> Les std::vector ont des tailles dynamiques, ce qui signifie qu'ils ont besoin d'allocation mémoire pendant l'éxecution
> du programme. Il y a bien discussion avec L'OS.

## 9. Fonctions cachées

a) Surcharger les opérateurs `+` et `*` pour que l'on puisse additionner deux `DinoVec2` ensemble,
et que l'on puisse multiplier un `DinoVec2` avec un `float`. Quelle syntaxe est utilisée ?

> On doit utiliser la syntaxe "operator"

b) Quand on affiche un sprite, on crée un `std::vector<DinoVertex>`, et on spécifie les positions et UV.
Pourquoi n'a-t-on pas besoin de spécifier la couleur de modulation du sprite ?

> Il n'y a pas besoin de le préciser car la struct admet des valeurs par défaut. Ainsi, lors de la compilation, le
> compilateur génère un constructor par défaut qui admet ces valeurs renseignées.

c) Remplacez les méthodes `Init()` de `DinoPlayer`, `DinoAnimal` et `DinoLasso`.
Quelle méthode de `std::vector` prend en paramètres les arguments de construction
et crée une instance sans faire de copie ?

> On utilise la méthode "emplace_back()". Les arguments transférés au constructeur sont placés entre parenthèse.

d) `std::vector<DinoVertex>` contient une allocation mémoire. Pourquoi n'y a-t-il pas besoin
d'appeler explicitement une méthode équivalente à `Shut()/Destroy()` ?

> Car il a une méthode "destructor" qui est appelée implicitement dès que la variable n'est plus accessible. Cette
> méthode s'occupe notamment de libérer la mémoire allouée.

e) Créer une classe `DinoVertexBuffer` qui accepte les mêmes arguments que `XDino_CreateVertexBuffer()`
et garde le `vbufID` dans un membre privé, avec une fonction `Get()` qui retourne ce `vbufID`.
Faites en sorte que `XDino_DestroyVertexBuffer()` soit appelé automatiquement.
Remplacez les usages des vertex buffers qui sont créés à chaque frame.

f) Que se passe-t-il lorsqu'on copie un `DinoVertexBuffer` vers un autre ? Comment le prévenir ?

> Car lorsqu'on copie un copie égalment le VbufId ce qui fait que lors de la destruction, on appelle deux fois la
> destruction d'un même buffer

g) Appliquez les mêmes outils pour enlever les méthodes `Shut()` des classes `DinoPlayer`, `DinoAnimal` et `DinoLasso`
(s'il y en a).

h) Implémentez en debug le fait que le terrain est réinitialisé en appuyant
sur le bouton `btn_up` (touche Z du clavier).

i) Quelle difficulté a-t-on pour remplacer `Init()` et `Shut()` de `DinoTerrain` ?
Comment changer le code ?

> DinoTerrain étant une variable globale sont constructor et destructor sont appelé avant, et après l'initialisation du
> moteur graphie. Ainsi, les fonction d'envoie de texture au GPU par exemple ne peuvent pas fonctionner
> Une méthode est de passer DinoTerrein en "std::optional"

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
