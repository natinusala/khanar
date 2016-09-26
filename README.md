# khanar

Gestionnaire de fichiers en C++, dans le cadre d'un projet de développement de L3 Informatique à l'Université de Nantes.

##Dépendances

 * gtkmm (`libgtkmm-3.0-dev`)
 * `cmake`
 
##Compilation
```
git clone https://github.com/natinusala/khanar.git
cd khanar
mkdir build_release
cd build_release
cmake ../
make
```

Si des dépendances manquent, CMake vous le fera savoir et ne génèrera pas de `Makefile`. Une fois la compilation terminée, l'exécutable `khanar` sera présent dans le dossier `khanar/build_release/src`.
