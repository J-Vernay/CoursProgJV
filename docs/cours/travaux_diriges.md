# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

**Veillez à travailler dans une branche Git à votre nom : NOM_Prenom**.

**Après chaque point, veuillez (git add + git commit) votre base de code avec dans le message le nom du point que vous
venez de faire.** (par exemple: `git add .` puis `git commit -m "0.a"`)

**A la fin de chaque séance, veuillez push votre branche sur le dépôt GitHub** (`git push`)

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers suivants :

> `xdino.h` : Déclare une API d'interaction entre le moteur XDino et le jeu, comportant des fonctions pour du rendu
> graphique, de la prise d'inputs et autre fonctions utilitaires.
>
> `xdino_win64_main.cpp` : Implémente des fonctions liés à la création de la fenêtre et le polling des inputs, en plus
> de la fonction d'entrée du programme.
>
> `xdino_win64_rdr.cpp` : Implémente des fonctions étant liées au rendu graphique via Direct3D 11.
>
> `dino_game.cpp` : Implémente la logique de jeu.
>
> `dino_geometry.cpp` : Implémente une fonction permettant de détecter si deux segments s'intersectent entre eux.
>
> `dino_draw_utils.cpp` : Implémente des fonctions utilitaires permettant de déssiner à l'écran différents éléments,
> comme du texte, des cercles et des lignes.
>
> `premake5.lua` : Utilitaire permettant de configurer la solution du projet.

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

> 6 pixels ont étés parcouru entre les deux frames.

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-300, 400) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Le cercle s'est déplacé en diagonale gauche-bas à 50000 pixels par seconde.

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

> Le cercle est à la position (170.71, 129.28) au bout d'une seconde.

f) Par quoi est définit un triangle texturé ?

> Un triangle texturé dans XDino est défini par 3 positions `DinoVec2`, 3 couleurs `DinoColor` et 3 sets de position UV,
> chaque set de position, couleur et UV représentant un des sommets du triangle.

g) Qu'est-ce qu'un "draw call" ?

> Un draw call est un appel de rendu pour la carte graphique, en passant une structure de données pour représenter ce
> qui doit être dessiné à l'écran.

h) À quoi servent les configurations Debug|Profile|Release ?
Donnez un exemple de différence dans le code.

> Ces configurations permettent de tester le jeu dans différents contextes.
> Debug n'optimise pas le code, permettant de mettre des breakpoints et de suivre précisément l'execution du programme.
> Release est une compilation du jeu "prêt à être publié", avec les optimisations activées et un build généralement plus
> léger et rapide que les autres.
> Profile est la même configuration que Release, mais implémente quelques fonctions permettant de profiler la vitesse
> d'execution du jeu en "conditions réelles".
> Un exemple de l'impact des différentes configurations peut être observé dans `xdino.h` entre les lignes 178 et 195,
> où la définition des fonctions `XDino_ProfileBegin` et `XDino_ProfileEnd` dépend de la configuration de compilation,
> notamment si on est en configuration Profile ou non.
> Si on est bien en Profile, les fonctions sont définies, sinon elles sont compilées en tant que fonctions vides.

i) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

> ...

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> On peut mettre en miroir le sprite du dinosaure en inversant les coordonnées U de la gauche et de la droite de la
> sprite.

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Comment récupérer les coordonnées UV de la bonne animation ?

> On peut récupérer les coordonnées UV de la bonne animation en tirant un index de frame via une liste de frames, et en
> le multipliant par la largeur de la sprite.

d) Implémentez la fonctionnalité F1.4 .

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper et en avoir plusieurs instances ?

> On peut créer une classe pour regrouper les données et la logique du dinosaure.
> En utilisant la classe on peut créer plusieurs instances du dinosaure.

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> On appelle ces fonctions des fonctions membres.
> Les modificateurs de visibilité permettent de contrôler l'accès des variables membres et donc
> de limiter la modification d'un type de données uniquement aux fonctions membres.
> L'intérêt est de pouvoir faire en sorte qu'un programmeur puisse mettre à disposition de ses collègues uniquement
> ce qui est sensé modifier un type de donnée d'une manière contrôlée, et éviter qu'un programmeur qui ne connait
> pas précisément le fonctionnement d'un type de donnée ne crée des bugs en accédant d'une manière non prévue à ce
> dernier.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Implémentez F1.5 : Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez `std::vector` de la bibliothèque standard pour stocker ces instances.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?

> ````for (Type nom : tableau)````

f) Implémentez F1.6 : Utilisez `std::sort` pour que les dinosaures soient affichés de haut en bas, l'un devant l'autre.
Pour se faire, créez une fonction qui permet de comparer deux `DinoPlayer` suivant leur position verticale.
Comment créer cette fonction sans exposer publiquement la position de `DinoPlayer` ?

> On peut implémenter une fonction `static` accessible publiquement dans `DinoPlayer` qui compare internement les
> positions verticales de deux instances de `DinoPlayer`. Ainsi on obtient une comparaison sans exposer publiquement la
> position de `DinoPlayer`.

g) Que retournent, et à quoi servent, `.begin()` et `.end()` dans l'utilisation de `std::sort` ?

> `.begin()` et `.end()` retournent des itérateurs, type de données permettant d'itérer au travers des élements d'une
> structure de données de la librairie standard. Dans le contexte de `std::sort` ces itérateurs permettent de définir la
> portée de données que la fonction va trier.

## 3. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h`. Quels sont les 4 dossiers du projet à contenir des fichiers C++ ?

> `src\dino`
>
> `src\dino\x64-windows`
>
> `external\stb`
>
> `external\pix\Include\WinPixEventRuntime`

b) Cherchez `CoursProgJV *.cpp`. Quels sont les 3 dossiers du projet à contenir des fichiers C++ ?

> `src\dino`
>
> `src\dino\x64-windows`
>
> `external\stb`

c) Cherchez `CoursProgJV *.obj`. Que remarquez-vous des noms des fichiers concernés ? Notez leur chemin.

> Ce sont les noms des différents fichiers `.cpp` du projet.
>
> `build\obj\x64-windows\Debug\Dino_MatysGueroult`
>
> `build\obj\x64-windows\Debug\Dino_External`

d) Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leur chemin.

> `Dino_JulienVernay.exe`
>
> `build\x64-windows\Debug\Dino_MatysGueroult.exe`

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> Pour les chemins des fichiers `.h`, les lignes 22, 86, 94, 100 et 112 y font référence.
>
> Pour les chemins des fichiers `.cpp` les lignes 22, 86, 94 et 100 y font référence.
>
> Pour les chemins des fichiers `.obj`, la ligne 16 y fait référence.
>
> Pour les chemins des fichiers `.exe`, la ligne 19 y fait référence.

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** les fichiers `.h` déclarent ce que les fichiers `.cpp` implémentent.
>
> **Fichiers `.cpp` et `.obj` :** les fichiers `.obj` sont les versions compilées (mais pas linkées) des
> fichiers `.cpp`.
>
> **Fichiers `.obj` et `.lib` :** les fichiers `.lib` contiennent un ou plusieurs fichiers `.obj`, prêts à être linkés
> statiquement avec d'autres fichiers `.obj`.
>
> **Fichiers `.obj` et `.dll` :** les fichiers `.dll` contiennent un ou plusieurs fichiers `.obj`,
> exposant des fonctions provenant des `.obj` à un linkage dynamique avec un processus.
>
> **Fichiers `.obj` et `.exe` :** les fichiers `.obj` sont linkés entre eux pour produire un fichier `.exe`.
>
> **Fichiers `.dll` et `.exe` :** les fichiers `.dll` sont séparés des fichiers `.exe` mais peuvent être linkés entre
> eux durant l'execution du fichier `.exe`.

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> Le préprocesseur execute toutes les instructions préprocesseur se trouvant dans le code source à compiler, remplaçant
> les macros par leur valeur définie, et vérifiant les conditions, incluant ou non du code en fonction de leur résultat.
> Cela permet d'avoir un code source changeant en fonction du contexte de la compilation, sans avoir à maintenir
> plusieurs code source différents.

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

> L'éditeur de lien détermine le positionnement des différents `.obj` dans un executable, résolvant les adresses de
> fonctions et de données, et produisant du code executable par le système d'opération. L'éditeur de liens peut produire
> des fichiers `.exe` ou `.dll`, les `.exe` étant des fichiers directement executables pas le système d'opération,
> les `.dll` nécessitant d'être linké dynamiquement à un processus pour être executés.

## 4. Programmation du terrain

a) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

b) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> Marge: (tailleTotale - tailleInterieur) / 2
>
> Marge horizontale: (480 - 256) / 2
>
> Marge verticale: (360 - 192) / 2

c) Implémentez la fonctionnalité F2.1 .

d) Implémentez la fonctionnalité F2.2 . Profitez-en pour créer les fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain. Pour vous faciliter la vie, ajoutez une fonction utilitaire
dans `dino_draw_utils.cpp`
qui ajoute un sprite à un drawcall avec les bonnes positions et UV.

e) Combien de triangles avez-vous besoin pour dessiner le terrain complet (océan compris) ?

> (4 + 16 + 16 + 12 + 12 + 1) * 2 = 122. L'océan étant dessiné avec le clear de l'écran, et le centre du terrain étant
> dessiné avec un seul rectangle, il ne reste plus qu'à dessiner la bordure du terrain qui prends 120 triangles. En
> comptant le centre du terrain la scène entière prends 122 triangles à dessiner.

f) Implémentez la fonctionnalité F2.3 .

g) Implémentez la fonctionnalité F2.4 .

## 5. Programmation des animaux

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 31.7 Go sont disponibles sur ma machine. Sont adressables 18 446 744 073 709 551 616 octets. On ne peut théoriquement
> adresser que les octets alloués à notre processus.

d) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Pour allouer de la mémoire sur un ordinateur moderne, le processus intéragit avec la librairie standard, qui gère un
> espace de mémoire virtuel, pour obtenir des bouts de mémoire. Physiquement seulement une petite partie de la mémoire
> virtuelle est allouée, et ce n'est seulement que quand une allocation est trop grande pour l'espace déjà alloué que la
> librairie standard alloue plus de mémoire (en blocs de quelques kilo-octets) en intéragissant avec le système
> d'opération.
> L'opération est relativement peu couteuse la plupart du temps, mais peut demander un peu plus de temps si un gros bloc
> de mémoire doit être alloué. Généralement si on alloue pas trop régulièrement de mémoire il ne devrait réellement y
> avoir de problèmes de performance.

e) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Un type correspond à une déclaration de la manière d'interpreter et d'intéragir avec un bloc de données.
> Un pointeur correspond à la position d'un bloc de données dans la mémoire.
> Réinterpréter un pointeur veut dire qu'on change le type, donc l'interprétation, du bloc de données référencé par le
> pointeur.

f) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> `DinoColor` occupe 4 octets. `DinoVertex` occupe 16 octets.

g) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> `std::vector` représente un tableau de données à taille dynamique.
>
> En mémoire il est représenté par un bloc contigu de mémoire contenant les données et un pointeur vers le début du
> bloc.
>
> Pour connaître la position en mémoire d'un élément, on prend le pointeur du premier élément, et on y ajoute l'indice
> donné multiplié par la taille en mémoire du type.

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Les variables globales sont alloués au début du processus dans le secteur "data".
>
> Les variables locales sont alloués à chaque fois qu'une fonction est appelée, dans le "stack".
>
> La mémoire des `std::vector` est allouée à la création de ces derniers, ainsi que quand la mémoire déjà allouée pour
> le `std::vector` est remplie, le tout étant situé dans la "heap".

i) Implémentez la fonctionnalité F3.3. Cela implique de trier un tableau qui peut contenir à la fois
des `DinoPlayer` et des `DinoAnimal`. Comment faire ?

> On peut faire en sorte que `DinoPlayer` et `DinoAnimal` hérite tous deux d'une même classe `DinoActor`, comme ça on
> peut stocker toutes les instances des joueurs et animaux dans le même `std::vector`.

j) Implémentez la fonctionnalité F3.4.

## 6. Programmation des lassos

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 en limitant à 2 secondes d'historique.
Qu'est-ce que la fonctionnalité implique pour les octets en mémoire qui représente le lasso ?

> La quantité d'octets nécessaires à la représentation du lasso se retrouve limité à un nombre fixe sitôt que le lasso
> atteint sa taille maximum.

c) Implémentez la fonctionnalité F4.3 . Combien d'intersections de segments sont calculés (en comptant les 4 joueurs) ?

> Avec mon implémentation, sont calculés 468 intersections dans le pire des cas (qui est le plus commun des cas).

d) Implémentez la fonctionnalité F4.4 , tout en faisant que les instances de la classe `DinoPlayer` n'ont pas besoin d'
interagir entre elles.

## 7. Interactions

a) Implémentez la fonctionnalité F5.1 .

b) Implémentez la fonctionnalité F5.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> On prends la distance entre les deux positions centrales des cercles et on la compare à la somme des radius des deux
> cercles. Si la distance est plus petite alors les cercles sont en collision.

d) Comment repousser deux cercles en collision pour qu'ils ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> En prenant le vecteur normal entre les centres des deux cercles on peut bouger chacun des cercles sur ce vecteur mais
> dans deux directions opposées, de manière à ce que chaque cercle soit bougé de la moitié de la distance de pénetration
> entre les deux cercles. On ne peut pas résoudre le cas où les deux points centraux sont précisément au même endroit.

e) Implémentez la fonctionnalité F5.3 .

f) Implémentez la fonctionnalité F5.4 . Pourquoi y a-t-il duplication de code ?

> Théoriquement parce que on a implémenté 3 fois la même logique de collision pour différent types de données.

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> L'héritage est une fonctionnalité qui permettrait de rassembler la logique de collision dans une seule
> classe `DinoActor`.

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> Les fonctions virtuelles permetted cela, en changeant le comportement de la fonction dans les classes héritant de la
> classe qui déclare cette fonction.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> Les méthodes `update()` et `draw()` peuvent ainsi être également mises en commun.

j) Comment détecter qu'une position est à l'intérieur d'un contour fermé définis par des segments ?

> En comptant le nombre d'intersections entre le contour fermé et un segment formé par la position testée et un point en
> dehors du contour. Si le nombre d'intersections est impair la position est dans la forme, sinon elle est en dehors.

k) Implémentez F5.5 et F5.6 via une logique commune, comme mentionné par (g).

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

> On trouve des fichiers `.pdb`, ce sont des fichiers symboles qui répertorient les noms des fonctions et autres membres
> de multiples librairies système et standard. En mettant un nom sur ces éléments on peut donc plus facilement analyser
> le comportement d'un executable.

d) Sélectionnez une large bande dans la timeline. En bas à droite, sélectionnez **Items to show = Sampled functions**.

Quelle part représente `Dino_GameFrame` relativement à `WinMain` ? Quelle autre fonction prend du temps ?

> `Dino_GameFrame` prends environ 32.43% du temps d'execution de `WinMain`. `XDino_Win64_EndDraw` prends la majeure
> partie du reste du temps, avec 64.45% du temps d'execution de `WinMain`.

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

