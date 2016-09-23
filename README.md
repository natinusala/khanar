# untitled-file-manager

Gestionnaire de fichiers en C++, dans le cadre d'un projet de développement de L3 Informatique à l'Université de Nantes.

##Dépendances

 * gtkmm (`libgtkmm-3.0-dev`)
 * `cmake`
 
##Compilation
```
git clone https://github.com/natinusala/untitled-file-manager.git
cd untitled-file-manager
mkdir build_release
cd build_release
cmake ../
make
```

Si des dépendances manquent, CMake vous le fera savoir et ne génèrera pas de `Makefile`. Une fois la compilation terminée, l'exécutable `untitled-file-manager` sera présent dans le dossier `build_release`.
