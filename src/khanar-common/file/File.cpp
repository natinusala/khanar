/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "File.hpp"

namespace khanar
{
      //TODO Vérifier si les paramètres ne sont pas vides

      File::File(File* parent, string name)
      {
        //TODO Vérifier que le parent est un dossier
        this->_name = name;

        if (parent != NULL)
        {
          this->_parentFolderAbsolutePath = parent->getAbsolutePath();
        }
        else
        {
          this->_parentFolderAbsolutePath = "";
        }
      }

      File::File(string absolutepath)
      {
        size_t pos = absolutepath.find_last_of("/");
        this->_name = absolutepath.substr(pos+1);
        this->_parentFolderAbsolutePath = absolutepath.substr(0, pos);
      }

      string File::getName()
      {
        return this->_name;
      }

      string File::getAbsolutePath()
      {
        return this->_parentFolderAbsolutePath + "/" + this->_name;
      }

      string File::getParentFolderAbsolutePath()
      {
        return this->_parentFolderAbsolutePath;
      }
}
