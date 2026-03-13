# Travaux dirigés

Ce fichier contient le plan global du cours.
Vous devrez compléter `docs/cours/travaux_diriges.md` au fur et à mesure du cours.

## 0. Prise en main de la base de code

a) Résumez en une phrase le rôle des fichiers et dossiers suivants :

> `dino/xdino.h` : Le .h contenant les définition de fonctions des fichiers dino_game.cpp et xdino_win64_rdr.
>
> `dino/x64-windows/` : Permet la compatibilité avec l'environnement windows. Contient les fichier de base de rendering sous windows
>
> `dino/x64-linux/` : Permet la compatibilité avec l'environnement linux.
>
> `dino/dino_game.cpp` : Gère le démarage, le Update et la fermeture de la fenetre de jeu
>
> `dino/dino_geometry.cpp` : Gère la supperposition de deux pixel pour n'en afficher qu'un seul
>
> `dino/dino_draw_utils.cpp` : permet l'affichage du Debug
>
> `premake5.lua` : Permet de build le projet en fonction de l'environnement qu'il détecte

<<<<<<< HEAD
b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des `// COMMENTAIRE`. $£
=======
b) Remettez les 20 commentaires suivants aux bons endroits dans le fichier `xdino_win64_main.cpp`, à la place des
`// COMMENTAIRE`.
>>>>>>> main

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

> 6

d) Le temps entre deux frames est 10 millisecondes. Pendant ce temps,
le cercle s'est dirigé suivant le vecteur (-30, 40) (en pixels).
Dans quelle direction s'est-il déplacé ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Direction South-South-West
> (30,40)*magnitude = 50   /0.01 = 5000 pixel par secondes

e) Le cercle est à la position (100, 200). Il se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position le cercle est-il
au bout d'une seconde ?

>  (171,129)

f) Par quoi est définit un triangle texturé ?

> 3 vertex et une texture appliqué grace au coordoné UV qui lui sont associé

g) Qu'est-ce qu'est un "draw call" ?

> un appelle pour dessiner un triangle incluant géométrie et texture déjat présente dans le GPU

h) Qu'est-ce qui est affiché quand on maintient MAJ/SHIFT dans le jeu ?
Dans la sortie textuelle quand on quitte le jeu ?

> Affiche un panel de Debug

i) À quoi servent les configurations Debug|Profile|Release ? Donnez un exemple.

> Permet de différencier les vertion devlloper ou certain outil de Debug sont accesible au vertion publique.
> La vertion profiler sert a l'optimisation mais rend le Debug difficile;

j) Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
sur le même modèle que `dTime`. Il vous faudra utiliser le paramètre
`pOutSize` pour récupérer la taille en pixels du rectangle de texte
et le positionner correctement contre le bas de l'écran grâce à `translation`.

k) Qu'est-ce que le type `std::vector` ? Cherchez la page de documentation sur cppreference.
Dans quelle catégorie est située cette page ?

> C'est similaire a une List, a savoir une array dynamique

l) Que signifie l'esperluette `&` à côté du paramètre `out` dans les fonctions déclarées
dans `dino/dino_draw_utils.h` ? Que se passe-t-il si on l'enlève ?

> Elle représente la valeur du pointeur. Si on l'enlève on ne peut pas la recuperer.

## 1. Programmation du déplacement du dinosaure (F1.1-F1.4)

a) Implémentez la fonctionnalité F1.1 .
Quand le bouton de course est enfoncé, doublez la vitesse de déplacement.
Pour le rendu, il vous faudra dessiner un carré, soit deux triangles texturés.
Choisissez les coordonnées UV de telle sorte à afficher le sprite de dinosaure en haut à gauche de la texture.

b) Implémentez la fonctionnalité F1.2 .
Comment peut-on mettre en miroir le sprite du dinosaure ?

> Opérateur ternaire sur les UV pour inverser la texture du dino

c) Implémentez la fonctionnalité F1.3 .
Notez que les sprites d'une même animation sont côte à côte.
Quel calcul permet de récupérer les coordonnées UV de la bonne animation, étant donné le temps écoulé depuis le début du
jeu ?

> Incrementer le un index correspondant au nombre de frame de l'animation correspondat et faire animPos = anim.animPosStart + animFrame * 24;

d) Implémentez la fonctionnalité F1.4 ; déclenchez l'animation de dégâts en appuyant sur `btn_left`.

## 2. Introduction à la programmation orientée objet en C++ (F1.5-F1.6)

a) Comment transformer les différentes variables globales qui représentent l'état du dinosaure
pour les regrouper ? L'appliquer.

> Faire une struct et mettre les variable dedans

b) Créez les fichiers `dino_player.h` et `dino_player.cpp` dans le dossier `src/dino`,
pour y déplacer le code concernant les dinosaures.
N'oubliez pas de relancer le script `SetupWorkspace.bat`.

c) En C++, quel terme utilise-t-on pour une fonction qui est associée à un type de données ?
Quel outil permet de limiter la modification d'un type de données à ce genre de fonctions ?
Comment appelle-t-on cette limitation ? Quel intérêt ?

> Une fonction membre
> Les déclaration d'accès (public/private)
> l'encapsulation.
> Elle permet de séparer les donné accessible depuis d'autre script des donné spécifique qui ne doivent pas etre modifié ailleur.

d) Appliquez ces outils pour créer la classe `DinoPlayer` en rendant privées les données
qui représentent le dinosaure.

e) Créez quatre dinosaures, c'est-à-dire quatre instances de la classe `DinoPlayer`.
Utilisez pour cela le type `std::vector<DinoPlayer>` de la bibliothèque standard.
Quelle syntaxe permet d'itérer sur tous les éléments d'un tableau, sans manipuler d'indices de cases ?
Comment s'appelle cette syntaxe ?

> for (int value : PlayerList)
la range-based for loop

## 3. Programmation du terrain

a) Elargissez la fenêtre du jeu. Pourquoi y a-t-il de l'espace inutilisé sur les côtés de l'écran ?

> parcque le moteur ne deforme pas les sprite quand la fenetre change de taille

b) Forcez la résolution du rendu à 480 pixels de long par 360 pixels de haut.

c) On veut positionner un rectangle de taille 256x192 pixels au centre d'un rectangle de 480x360 pixels.
Quel calcul faire pour obtenir la taille des marges en haut, à gauche, à droite et en bas ?

> (480 - 256) / 2 et(360-192)/2 pour x et y

d) Implémentez la fonctionnalité F2.1 . Mettez votre code dans des fichiers `dino_terrain.h` et `dino_terrain.cpp`
qui contiendront la logique du terrain.

e) Implémentez la fonctionnalité F2.2 .

f) Implémentez la fonctionnalité F2.3 . Faites en sorte qu'il y ait toujours exactement 10 fleurs de chaque espèce.
Expliquez les étapes de votre algorithme.

> 1 mettre l'image dans le gpu
> 2 coordoné des fleur dans l'image rensigné dans une liste (manuellement)
> 3 créer une list de la longeur de 30 avec 1à fleur de chaque espèce
>
> créer une grille sous forme de list contenant tout les case
> melanger cette liste
> prendre les 30 premiere case de cette liste
>
>mettre 10 fleur de chaque espece au coordoné

## 4. Comprendre la compilation des fichiers C++

Dans Everything, vérifier que **Recherche > Respecter le chemin** est activé.

a) Cherchez `CoursProgJV *.h|*.cpp`. Quels sont les 6 dossiers du projet à contenir des fichiers Header et C++ ?
Répartissez ces dossiers en 3 catégories.

> External pix
> external sokol
> external stb
> src\dino
> src\dino (gameplay)
> \x64 linux et windows (moteur platform)

c) Cherchez `CoursProgJV *.cpp|*.obj`. Que remarquez-vous à propos des fichiers `.obj` ? Notez leurs dossiers.

> Ils sont tous dans le dossier Debug
> 

d) Compilez le projet en changeant de configuration (Debug/Profile/Release).
Cherchez `CoursProgJV !tools *.exe`. Quel(s) fichier(s) obtenez-vous ? Notez leurs dossiers.

>.h et .cpp  = src\  et external\
> .obj et .exe = Build\obj\x64-windows\Debug release

e) Dans le fichier `premake5.lua`, quelles lignes font références aux fichiers et chemins observés plus tôt ?

> .h et .cpp  = ligne 101   files {"src/dino/*""}
> 
> .obj  = ligne 16  location "build"
> 
>.exe = targetdir "build/%{cfg.platform}/%{}cfg.buildcfg"

f) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` : les .h aide le preprocesseur a interpréter les fichiers .cpp
>
> **Fichiers `.cpp` et `.obj` : le compilateur créer les objet avec les cpp
>
> **Fichiers `.obj` et `.lib` : Les lib sont des archivage des obj
>
> **Fichiers `.obj` et `.dll` : prochedes .exe mais pas executable seul
>
> **Fichiers `.obj` et `.exe` : "edition des lien static" créer le .exe grace au obj
>
> **Fichiers `.dll` et `.exe` :LEs .exe peuvent utiliser les dll

g) Quel est le rôle du préprocesseur ? Comment reconnait-on les directives de préprocesseur ?

> verifier pour les erreur de syntaxe et les opération de type invalide

h) Quel est le rôle de l'éditeur de liens ? Quels sont les deux types de fichiers qu'il peut produire ? Quelle
différence majeure ?

>Compile tout en un .exe et peut faire des lib et dll pour avoir des fichier partageable en gardant le code secret. 




## 5. Programmation des animaux 

a) Implémentez la fonctionnalité F3.1 .

b) Implémentez la fonctionnalité F3.2 .

c) Implémentez la fonctionnalité F3.3 .

## 6. Physique de jeu

a) Implémentez la fonctionnalité F4.1 .

b) Implémentez la fonctionnalité F4.2 .

c) Comment détecter si deux cercles à des positions données sont en collision ?

> Utiliser le radius et claculer la distance. si distance < RadiusA + RadiusB, ils sont en collision

d) Comment repousser deux cercles en collision de façon minimale et qu'il ne soient plus en collision ?
Quel cas particulier n'est pas résoluble ?

> appliquer un vecteur inverse tant que la collision se fait, plus la collision est proche, plus le vecteur inverse est fort. si deux entité on exactement la meme coordonée, la collision ne se fait pas.

e) Implémentez la fonctionnalité F4.3 .

f) Implémentez la fonctionnalité F4.4 . Pourquoi y a-t-il duplication de code ?

> Parcque il ont la ememe fonction mais dansd deux class différentes.

g) Quelle fonctionnalité du C++ permet de dédupliquer la logique commune entre `DinoPlayer` et `DinoAnimal` ?
L'appliquer dans la base de code.

> l'héritage de class.

h) Quelle fonctionnalité du C++ permet de gérer différemment un point de logique commune,
comme la réaction à un événement du type "limite du terrain" ? L'appliquer dans la base de code.

> Un fonction abstract.

i) Quelles méthodes de classes pourraient être mises en commune suivant le même principe ?
L'appliquer dans la base de code.

> GetPosition et SetPosition. 

j) Implémentez la fonctionnalité F4.5. Cela implique de trier un tableau qui peut contenir à la fois des `DinoPlayer` et
des `DinoAnimal`. Comment faire ?

> heritage de class

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

## 8. S'intéresser à la mémoire

a) Sur votre machine, combien de RAM est disponible ?
Dans un programme 64-bits, combien d'octets sont adressables ? À quels octets peut-on lire et écrire ?

> 32 au total
> la quantité max d'octet addressable par un programme 64 bit est 18 446 744 073 709 551 616 octet, soit, 18 hexaoctet
> 
> 

b) Que veut dire "allouer de la mémoire" sur un ordinateur moderne ?
Est-ce une opération coûteuse ?

> Attribuer un espace de la RAM pour y stocker des donné
> Le cout d'une allocation mémoire est proportionnel a la taille de mémoire a alouer.

c) En C++, à quoi correspond un type ? À quoi correspond un pointeur ?
Que veut dire réinterpréter un pointeur ?

> Un type définit comment interpréter une zone mémoire. 
> Un pointeur est une variable contenant une adresse mémoire. 
> Réinterpréter un pointeur  c'est lire les octets à cette adresse en leur appliquant un type différents.

d) Quelle est la taille du type `DinoColor` ? du type `DinoVertex` ?

> DinoColor = 4 octet
> 
> DinoVertex = 16 octet

e) Que représente un `std::vector` ? Comment pourrait-il être représenté en mémoire ?
Comment connaître la position en mémoire d'un élément étant donné son indice ?
Quelle limitation cela entraîne-t-il ?

> std::vector

h) Quand et qui alloue la mémoire pour les variables globales ?
Quand et qui alloue la mémoire pour les variables locales ?
Quand et qui alloue la mémoire des `std::vector` ?

> Variable gloabal = dans le fichier .exe, connue a la compilation
> Variable Local = dans la stack. généré par le compilateur pour sous-allouer dans la mémoire.
> vector = taille dinamiqye = allocation par l'OS.

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
