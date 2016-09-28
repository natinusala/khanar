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
#include <sys/stat.h>

using namespace std;

/**
 * \namespace khanar_common_file
 *
 * Namespace de la classe d'abstraction des fichiers
 */
namespace khanar
{
    /**
     * \class File
     *
     * Wrapper autour des fonctions de manipulation de fichiers
     * de C/C++
     */
    class File
    {
        //TODO Ajouter les attributs

        private:
           string _name;
           string _absolutePath;
           string _parentFolderAbsolutePath;

           struct stat _fileStat; ///< "Statistiques" du fichier, par stat.h (contient des données utiles)
           bool _exists; ///< Défini par stat() et lors de la suppression/création du fichier à la main

           void updateStat();

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

           bool isDirectory(); ///< Indique si le fichier est un dossier
           bool exists(); ///< Renvoie si le fichier existe ou non
           bool isHidden(); ///< Renvoie si le fichier est caché ou non (commence par un .)
    };

    /**
    * \class FileException
    *
    * Classe servant à reporter des erreurs sur les fichiers (entrée/sortie, opérations impossibles...)
    */
    class FileException
    {
      private:
        string _description;

      public:
        /**
        * \brief Constructeur de l'exception
        * \param[in] description la description de l'erreur
        */
        FileException(string description);

        string getDescription(); ///< Renvoie la description de l'exception
    };
}

#endif
