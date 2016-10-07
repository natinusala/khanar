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
#include <vector>

using namespace std;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class File;

    typedef bool (*FileSortStrategy)(File const& a, File const& b);  ///< Critère de tri pour les fichiers a et b

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

           FileSortStrategy _sortStrategy;

           vector<File> _subFiles;
           bool _subFilesCreated;

           struct stat _fileStat; ///< "Statistiques" du fichier, par stat.h (contient des données utiles)
           bool _exists; ///< Défini par stat() et lors de la suppression/création du fichier à la main

           bool _sortDescending;

           void updateStat();
           void updateSubFiles();
           void sortSubFiles();

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

           string getName() const;  ///< Récupère le nom du fichier
           string getParentFolderAbsolutePath() const; ///< Récupère le chemin absolu vers le dossier parent
           string getAbsolutePath() const; ///< Récupère le chemin absolu vers le fichier (parentAbsolutePath + name)

           bool isDirectory() const; ///< Indique si le fichier est un dossier
           bool exists() const; ///< Renvoie si le fichier existe ou non
           bool isHidden() const; ///< Renvoie si le fichier est caché ou non (commence par un '.')

           long getSize(); ///< Renvoie la taille du fichier en octets (ou -1 si c'est un dossier)
           string getFormattedSize(); ///< Renvoie la taille du fichier formattée dans un String

           vector<File>* getSubFiles(); ///< Si le fichier est un dossier, renvoie la liste des sous dossiers, NULL sinon

           void setSortStrategy(FileSortStrategy strategy, bool descending); ///< Change la stratégie de tri des fichiers du dossier ; une stratégie ici est un critère de tri et un ordre

           //Stratégies de tri
           static bool NAME_FILESORTSTRATEGY(File const& a, File const& b)
           {
             return a.getName() < b.getName();
           }  ///< Tri alphabétique par nom

           static bool SIZE_FILESORTSTRATEGY(File const& a, File const& b)
           {
             return a.getName() < b.getName();
           }  ///< Tri alphabétique par taille
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
