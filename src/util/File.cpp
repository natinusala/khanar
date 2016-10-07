/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "File.hpp"

#include <dirent.h>
#include <wordexp.h>
#include <algorithm>
#include <iostream>

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
          //Formattage du nom
          if (name.back() == '/')
          {
            name = name.substr(0, name.length()-1);
          }

          File(parent->getAbsolutePath() + "/" + name);
        }
      }

      File::File(string absolutepath)
      {
        if (absolutepath.empty())
        {
          absolutepath = "/";
        }

        //Formattage du nom
        if (absolutepath.back() == '/')
        {
          absolutepath = absolutepath.substr(0, absolutepath.length()-1);
        }

        //Expansion du nom (par exemple transformer "~" en "/home/user")
        wordexp_t exp_result;
        wordexp(absolutepath.c_str(), &exp_result, 0);
        absolutepath = string(exp_result.we_wordv[0]);

        //Construction
        size_t pos = absolutepath.find_last_of("/");
        this->_name = absolutepath.substr(pos+1);
        this->_parentFolderAbsolutePath = absolutepath.substr(0, pos);
        this->_sortStrategy = &NAME_FILESORTSTRATEGY;
        this->_subFilesCreated = false;
        this->_sortDescending = false;

        this->_absolutePath = absolutepath;
        this->updateStat();
      }

      void File::updateStat()
      {
        this->_fileStat = (const struct stat) {0};
        this->_exists = stat(this->getAbsolutePath().c_str(), &this->_fileStat) == 0;
      }

      string File::getName() const
      {
        return this->_name;
      }

      string File::getAbsolutePath() const
      {
        return this->_absolutePath;
      }

      string File::getParentFolderAbsolutePath() const
      {
        return this->_parentFolderAbsolutePath;
      }

      bool File::exists() const
      {
        return this->_exists;
      }

      long File::getSize()
      {
        return this->_fileStat.st_size;
      }

      void File::setSortStrategy(FileSortStrategy strategy, bool descending)
      {
        this->_sortStrategy = strategy;
        this->_sortDescending = descending;
        this->sortSubFiles();
      }

      bool File::isDirectory() const
      {
        return this->_fileStat.st_mode & S_IFDIR;
      }

      bool File::isHidden() const
      {
        return this->_name[0] == '.';
      }

      void File::updateSubFiles()
      {
        if (!this->isDirectory())
        {
          return;
        }

        //Construction
        vector<File> list;

        DIR* dir = opendir(this->_absolutePath.c_str());

        if (dir == NULL)
        {
          throw FileException("Impossible de lister les fichiers du dossier " + this->_absolutePath);
        }

        struct dirent* fichier = readdir(dir);

        while (fichier != NULL)
        {
          list.push_back(File(this->_absolutePath + "/" + string(fichier->d_name)));
          fichier = readdir(dir);
        }

        closedir(dir);

        //Tri
        this->_subFiles = list;
        this->_subFilesCreated = true;

        sortSubFiles();
      }

      void File::sortSubFiles()
      {
        if (!this->isDirectory() || !this->_subFilesCreated)
        {
          return;
        }

        if (this->_sortDescending)
        {
          sort(_subFiles.rbegin(), _subFiles.rend(), this->_sortStrategy);
        }
        else
        {
          sort(_subFiles.begin(), _subFiles.end(), this->_sortStrategy);
        }
      }

      vector<File>* File::getSubFiles()
      {
        if (!this->isDirectory())
        {
          return NULL;
        }

        if (!this->_subFilesCreated)
          updateSubFiles();

        return (&this->_subFiles);
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
