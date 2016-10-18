# khanar

Gestionnaire de fichiers en C++, dans le cadre d'un projet de développement de L3 Informatique à l'Université de Nantes.

##Dépendances

 * gtkmm (`libgtkmm-3.0-dev`)
 * `cmake`
 * `doxygen` pour la documentation
 * `xxd` pour convertir les ressources en .h
 * `xterm`

##Compilation
```
git clone https://github.com/natinusala/khanar.git
cd khanar
mkdir build
cd build
cmake ../
make
```

Si des dépendances manquent, CMake vous le fera savoir et ne génèrera pas de `Makefile`. Une fois la compilation terminée, l'exécutable `khanar` sera présent dans le dossier `khanar/build`.

##Documentation

La documentation se fait avec Doxygen ; après l'avoir installé, exécutez simplement le script `generate_doxygen.sh` présent dans le dossier `khanar`. La documentation sera générée au format HTML dans le dossier `khanar/doc/html` (ouvrez `index.html`).
