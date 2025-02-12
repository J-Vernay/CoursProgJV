workspace "CoursProgJV"

  -- Les plateformes correspondent à quel type de PC ou console.
  platforms { "x64-windows" }

  -- Les configurations changent la manière dont les fichiers C++ sont compilés,
  -- par exemple en changeant l'état du préprocesseur, en demandant des optimisations,
  -- ou l'ajout de checks de debug.
  configurations { "Debug", "Profile", "Release" }

  -- Quels standards utiliser pour les langages de programmation.
  cdialect "C17"
  cppdialect "C++20"

  -- Où sont générés les fichiers dy système de build (MSBuild)
  location "build"

  -- Où sont générés les binaires (exécutables et bibliothèques natives)
  targetdir "build/%{cfg.platform}/%{cfg.buildcfg}"

  -- Les #include <truc> vont aussi chercher dans le dossier src.
  includedirs { "src", "external" }
  
  -- On demande au compilateur de nous remonter ce qu'il trouve suspect,
  -- et de provoquer une erreur de compilation dès qu'il trouve quelque chose.
  warnings "Default"
  flags { "FatalWarnings" }

  -- Génère des informations de débogage, qui vont servir aux debuggers et profilers.
  symbols "On"

  -- Particularités pour Windows 64-bits
  filter "platforms:x64-windows"
    architecture "x86_64"
    defines { "_CRT_SECURE_NO_WARNINGS", "XDINO_X64_WINDOWS=1" }
    flags { "NoIncrementalLink" }
    editandcontinue "Off"
  filter {}

  -- Particularités quand on veut se mettre en débogage
  filter "configurations:Debug"
    defines { "XDINO_DEBUG=1"}
  filter {}

  -- Particularités quand on veut se mettre en configuration de performance mais avoir les outils de debug
  filter "configurations:Profile"
    -- Active la macro standard NDEBUG, qui désactive les assert()
    defines { "NDEBUG", "XDINO_PROFILE=1" }
    -- Le compilateur va optimiser le code assembleur, pour le rendre plus rapide,
    -- mais sera plus difficile à déboguer vu que la correspondance C++/Assembleur est brisée.
    optimize "On"
  filter {}

  -- Particularités quand on veut faire l'exe final, optimisé et sans outils de debug.
  filter "configurations:Release"
    defines { "NDEBUG", "XDINO_RELEASE=1" }
    optimize "On"
  filter {}

project "Dino_Documentation"

  -- Projet qui n'implique pas la compilation (ici, c'est la documentation).
  kind "Utility"
  -- Inclut tous les fichiers qui sont dans le dossier docs.
  files { "docs/*", "docs/cours/**", "docs/images/**", "README.md", "assets/CREDITS.md" }

  filter 'files:docs/Doxyfile'
    buildcommands {
      "{CHDIR} %[docs]%",
      "doxygen.exe"
    }
    buildoutputs { "docs/html/index.html" }
  filter {}

  -- On change la date de dernière modification du fichier, de telle sorte
  -- que l'on relance toujours le prebuild quand on veut build ce projet.
  postbuildcommands {
    "{TOUCH} %[docs/Doxyfile]"
  }

project "Dino_External"

  -- Compilation des bibliothèques tierces.
  kind "StaticLib"
  -- Compile tous les fichiers qui sont dans le dossier (et sous-dossiers) external
  files { "external/**.cpp", "external/**.h" }

-- Remplacer le nom du projet avec son propre prénom + nom.
project "Dino_LIClement"

  -- On fait une application graphique (en opposition à une application console = purement texte)
  kind "WindowedApp"
  -- Compile tous les fichiers qui sont directement dans le dossier src
  files { "src/dino/*" }

  links { "Dino_External" }

  filter "platforms:x64-windows"
    -- Et sur Windows, on veut compiler les fichiers dans le dossier x64-windows
    files { "src/dino/x64-windows/*" }
    -- On veut également utiliser les API systèmes Windows
    links { "d3d11.lib", "d3dcompiler.lib", "dxguid.lib", "Xinput.lib" }
  filter {}

  postbuildcommands {
    "{COPYDIR} %[assets] %[%{prj.location}/assets]",
  }

  -- Autoriser PIX quand on est sur Windows en mode "Profile"
  filter { "platforms:x64-windows", "configurations:Profile" }
    defines { "USE_PIX" }
    includedirs { "external/pix/Include/WinPixEventRuntime" }
    links { "external/pix/bin/x64/WinPixEventRuntime.lib"}
    postbuildcommands {
      "{COPYFILE} %[external/pix/bin/x64/WinPixEventRuntime.dll] %[%{prj.location}]"
    }
  filter {}