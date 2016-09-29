/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "File.hpp"

namespace khanar
{
      // File
      File::File(File* parent, string name)
      {
        if (parent == NULL || !parent->exists() || !parent->isDirectory())
        {
          throw FileException("Fichier parent invalide");
        }
        else
        {
          File(parent->getAbsolutePath() + "/" + name);
        }
      }

      File::File(string absolutepath)
      {
        if (absolutepath.empty())
        {
          absolutepath = "/";
        }

        size_t pos = absolutepath.find_last_of("/");
        this->_name = absolutepath.substr(pos+1);
        this->_parentFolderAbsolutePath = absolutepath.substr(0, pos);

        this->_absolutePath = absolutepath;
        this->updateStat();
      }

      void File::updateStat()
      {
        this->_fileStat = (const struct stat) {0};
        this->_exists = stat(this->getAbsolutePath().c_str(), &this->_fileStat) == 0;
      }

      string File::getName()
      {
        return this->_name;
      }

      string File::getAbsolutePath()
      {
        return this->_absolutePath;
      }

      string File::getParentFolderAbsolutePath()
      {
        return this->_parentFolderAbsolutePath;
      }

      bool File::exists()
      {
        return this->_exists;
      }

      bool File::isDirectory()
      {
        return this->_fileStat.st_mode & S_IFDIR;
      }

      bool File::isHidden()
      {
        return this->_name[0] == '.';
      }

      // FileException

      FileException::FileException(string description)
      {
        this->_description = description;
      }

      string FileException::getDescription()
      {
        return this->_description;
      }
}
