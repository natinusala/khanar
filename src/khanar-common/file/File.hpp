/**
 * \file File.hpp
 *
 * Header des suites de classes pour abstraire les fichiers
 * et les dossiers, en suivant le pattern Composite
 */

#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <memory>

using namespace std;

/**
 * \namespace khanar_common_file
 *
 * Namespace des classes d'abstraction des fichiers
 */
namespace khanar_common_file
{
    /**
     * \class FileComponent
     *
     * Classe mère du pattern, représente un fichier -ou- un dossier
     * Représente le Component dans le pattern
     */
    class FileComponent
    {
        //TODO Ajouter les permissions

        private:
           string _name;
           string _absolutePath;

        public:
           /**
           * \brief Constructeur prenant le fichier parent et le nom du fichier actuel
           * \param[in] parent le fichier parent
           * \param[in] name le nom du fichier actuel
           */
           FileComponent(shared_ptr<FileComponent> parent, string name);

           /**
           * \brief Constructeur prenant le chemin absolu du fichier
           * \param[in] string le chemin absolu du fichier
           */
           FileComponent(string absolutepath);

           string getName();  ///< Récupère le nom du fichier
           string getAbsolutePath(); ///< Récupère le chemin absolu vers le fichier

           virtual bool isFolder() = 0; ///< Renvoie si le fichier est un dossier
    };

    /**
     * \class Folder
     *
     * Classe représentant le Composite dans le pattern
     * contient une liste de fihiers (FileComponent)
    */
    class Folder : public FileComponent
    {
      //TODO Override isFolder, remplir les sous fichiers dans le constructeur
    };

    /**
     * \class File
     *
     * Classe représentant la "feuille" dans le pattern
     * contient un fichier simple
    */
    class File : public FileComponent
    {
      //TODO Méthodes pour ouvrir, déterminer les attributs du fichier, les propriétés
    };

}

#endif
