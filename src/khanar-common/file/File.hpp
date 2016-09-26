/**
 * \file File.hpp
 *
 * Suites de classes pour abstraire les fichiers
 * et les dossiers, en suivant le pattern Composite
**/

#ifndef FILE_HPP
#define FILE_HPP

/**
 * \namespace khanar_common_file
 *
 * Namespace des classes d'abstraction des fichiers
**/
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

    };

    /**
     * \class Folder
     *
     * Classe représentant le Composite dans le pattern
     * contient une liste de fihiers (FileComponent)
    **/
    class Folder : public FileComponent
    {

    };

    /**
     * \class File
     *
     * Classe représentant la "feuille" dans le pattern
     * contient un fichier simple
    **/
    class File : public FileComponent
    {

    };

}

#endif
