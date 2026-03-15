# Compilation sous Linux

Il y a plusieurs méthodes pour compiler avec Linux.

> **NOTE:** Pour l'instant en linux natif, seul le clavier fonctionne, pas les manettes.
> S'il faut vraiment tester les manettes, il vaut mieux utiliser la méthode 3 qui passe par Wine.

## 1. En utilisant Premake et Ninja

Il faut installer les packages `premake` et `ninja`.

```
premake5 ninja
ninja -C build Dino_JulienVernay_Debug_x64-linux
./build/x64-linux/Debug/Dino_JulienVernay
```

- La 1ère étape n'est nécessaire que quand l'architecture du projet change (nouveaux fichiers...)
- La 2ème étape est la compilation en elle-même.
- La 3ème étape est l'exécution.

> **ATTENTION:** En modifiant des headers, il arrive que ninja n'arrive pas à détecter
> qu'il faut recompiler, ou cela peut entrainer des erreurs bizarres de mémoire.
> Dans ce cas-là, supprimer le dossier `build` entier et recommencer.

## 2. Une seule grosse commande bourrine avec g++

```
mkdir build
g++ src/dino/*.cpp src/dino/x64-linux/*.cpp external/stb/stb.cpp -Isrc -Iexternal -std=c++20 -DXDINO_DEBUG=1 -DXDINO_X64_LINUX=1 -lGL  -lX11 -lXi -lXcursor -ldl -lpthread -lm -g -o build/Dino
./build/Dino
```

## 3. Compilation sous Linux, mais pour faire un exe Windows

Il faut le package `mingw-w64-gcc` pour compiler, et le package `wine` pour tester sous Linux.

```
mkdir build
x86_64-w64-mingw32-g++ src/dino/*.cpp src/dino/x64-windows/*.cpp external/stb/stb.cpp -Isrc -Iexternal -std=c++20 -DXDINO_DEBUG=1 -DXDINO_X64_WINDOWS=1 -ld3d11 -ld3dcompiler -ldxguid -lxinput -static -o build/Dino.exe
wine ./build/Dino.exe
```