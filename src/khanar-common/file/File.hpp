/**
 * \file File.hpp
 *
 * Header des suites de classes pour abstraire les fichiers
 * et les dossiers, en suivant le pattern Decorator
 */

#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <memory>

using namespace std;

/**
 * \namespace khanar_common_file
 *
 * Namespace de la classe d'abstraction des fichiers
 */
namespace khanar
{
    /**
     * \class FileComponent
     *
     * Wrapper autour des fonctions de manipulation de fichiers
     * de C/C++
     */
    class File
    {
        //TODO Ajouter les attributs

        private:
           string _name;
           string _parentFolderAbsolutePath;

        public:
           /**
           * \brief Constructeur prenant le fichier parent et le nom du fichier actuel
           * \param[in] parent le fichier parent
           * \param[in] name le nom du fichier actuel
           */
           File(File* parent, string name);

           /**
           * \brief Constructeur prenant le chemin absolu du fichier
           * \param[in] string le chemin absolu du fichier
           */
           File(string absolutepath);

           string getName();  ///< Récupère le nom du fichier
           string getParentFolderAbsolutePath(); ///< Récupère le chemin absolu vers le dossier parent
           string getAbsolutePath(); ///< Récupère le chemin absolu vers le fichier (parentAbsolutePath + name)
    };
}

#endif
