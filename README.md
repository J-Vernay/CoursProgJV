# Cours de Programmation pour le Jeu Vidéo {#mainpage}

[Voir les crédits et licences du support de cours](assets/CREDITS.md)

## Logiciels

- Téléchargez et installez **Microsoft PIX on Windows**, un logiciel qui permet de
  mesurer le temps d'exécution d'un programme : https://devblogs.microsoft.com/pix/download/
- Téléchargez et installez **Everything**, un logiciel qui permet de
  chercher parmi tous les fichiers de son ordinateur : https://www.voidtools.com/downloads/
- Téléchargez et installez **Doxygen**, un logiciel qui permet de documenter le code :
  https://www.doxygen.nl/download.html

## Préparation

Avant tout, écrivez un message "Bonjour" dans le fil de discussion https://github.com/J-Vernay/CoursProgJV/issues/1 . 👋

Cloner avec Git le dépôt GitHub.

```
git clone https://github.com/J-Vernay/CoursProgJV.git
```

Créez une branche de développement qui vous sera propre, avec votre NOM_Prenom.

```
git checkout -b NOM_Prenom
```

Modifiez le fichier `premake5.lua` pour remplacer la ligne
`project "Dino_JulienVernay"` avec votre propre nom.

Ensuite, double-cliquez sur `SetupWorkspace.bat` pour initialiser le dépôt.
Un dossier `build` a été créé, dans lequel se trouve le fichier `CoursProgJV.sln`.
Ouvrez ce fichier avec l'IDE JetBrains Rider.

Ouvrez les paramètres de JetBrains Rider avec la combinaison de touches `Ctrl+Alt+S`.

- *Build, Execution, Deployment --> Toolset and Build --> Use ReSharper build*
  doit être **décoché**.
- *Tools --> Action on Save --> Reformat and Cleanup Code* doit être **coché**,
  avec le paramètre `Profile: Reformat Code`.

Dans le volet **Solution** à gauche, faites un clic droit sur **Dino_Documentation**
et cliquez sur **Build Selected Projects**.

Une fois fini, ouvrez le fichier `docs/html/index.html` avec un navigateur Internet,
et suivez le reste du guide à partir de cette page.

<br/><br/>

Ensuite, il faut compiler le projet et le lancer en mode debugging.
En haut, sélectionner **Debug | x64-windows** et **Dino_PrenomNom**.
Puis cliquez sur le bouton vert en forme d'insecte.

Si tout se passe bien, après la compilation, vous devriez voir apparaître
la fenêtre suivante :

![Capture d'écran de l'état initial du projet](@ref initial_setup_screenshot.png)

Vérifiez que les entrées clavier/souris/manette fonctionnent bien :

* Les touches `ZQSD` doivent transformer le texte (rotation et mise à l'échelle).
* Les flèches du clavier doivent faire bouger le cercle blanc.
* Les boutons `ABXY` de la manette doivent aussi transformer le texte.
* Le stick gauche de la manette doit aussi faire bouger le cercle blanc.

Une fois ceci vérifié, c'est bon, l'environnement de développement
est opérationnel ! 😎

* [Suivre le sujet de travaux dirigés](docs/cours/travaux_diriges.md)

* [Prendre connaissance du projet du cours](docs/cours/projet_dinoranch.md)