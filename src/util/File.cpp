/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "File.hpp"
#include "../libs/FileSize.hpp"
#include "StringUtil.hpp"

#include <dirent.h>
#include <wordexp.h>
#include <algorithm>
#include <iostream>
#include <cstdio>

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
        updateAttributes(absolutepath);
      }

      void File::updateAttributes(string absolutepath)
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
        size_t posOfDot = this->_name.find_first_of('.');
        if (posOfDot != string::npos)
        {
          this->_extension = this->_name.substr(posOfDot+1, this->_name.length());
        }
        else
        {
          this->_extension = "";
        }

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

      bool File::isExecutable() const
      {
        return this->getPermission(OTH_X) || this->getPermission(GRP_X) || this->getPermission(OTH_X);
      }

      void File::setName(string newname)
      {
        move(this->_parentFolderAbsolutePath + '/' + newname);
      }

      void File::move(string newpath)
      {
        rename(this->_absolutePath.c_str(), newpath.c_str());
        updateAttributes(newpath);
      }

      void File::setPermission(enum Permission perm, bool value)
      {
        int toSet = 0;

        switch (perm)
        {
          case USR_R:
            toSet = S_IRUSR;
            break;
          case USR_W:
            toSet = S_IWUSR;
            break;
          case USR_X:
            toSet = S_IXUSR;
            break;

          case GRP_R:
            toSet = S_IRGRP;
            break;
          case GRP_W:
            toSet = S_IWGRP;
            break;
          case GRP_X:
            toSet = S_IXGRP;
            break;

          case OTH_R:
            toSet = S_IROTH;
            break;
          case OTH_W:
            toSet = S_IWOTH;
            break;
          case OTH_X:
            toSet = S_IXOTH;
            break;

          default:
            return;
        }

        if (value)
        {
          chmod(this->_absolutePath.c_str(), this->_fileStat.st_mode | toSet);
        }
        else
        {
          chmod(this->_absolutePath.c_str(), this->_fileStat.st_mode & ~toSet);
        }

        this->updateStat();
      }

      bool File::getPermission(enum Permission perm) const
      {
        switch (perm)
        {
          case USR_R:
            return _fileStat.st_mode & S_IRUSR;
          case USR_W:
            return _fileStat.st_mode & S_IWUSR;
          case USR_X:
            return _fileStat.st_mode & S_IXUSR;

          case GRP_R:
            return _fileStat.st_mode & S_IRGRP;
          case GRP_W:
            return _fileStat.st_mode & S_IWGRP;
          case GRP_X:
            return _fileStat.st_mode & S_IXGRP;

          case OTH_R:
            return _fileStat.st_mode & S_IROTH;
          case OTH_W:
            return _fileStat.st_mode & S_IWOTH;
          case OTH_X:
            return _fileStat.st_mode & S_IXOTH;

          default:
            return false;
        }
      }

      string File::getParentFolderAbsolutePath() const
      {
        return this->_parentFolderAbsolutePath;
      }

      bool File::exists() const
      {
        return this->_exists;
      }

      FileType File::getFileType() const
      {
        string extension = this->_extension;
        STR_TOLOWER(extension);

        if (FILETYPE_EXTENSIONS_MAP.find(this->_extension) != FILETYPE_EXTENSIONS_MAP.end())
        {
          return FILETYPE_EXTENSIONS_MAP.at(this->_extension);
        }
        else
        {
          if (this->isExecutable())
          {
            return FileType("Fichier exécutable", "application-x-executable");
          }

          if (extension.empty())
          {
            return FileType("Fichier", "text-x-generic");
          }
          else
          {
            STR_TOUPPER(extension);
            return FileType("Fichier " + extension, "text-x-generic");
          }
        }
      }

      long File::getSize() const
      {
        if (this->isDirectory())
        {
          return -1;
        }

        return this->_fileStat.st_size;
      }

      string File::getFormattedSize() const
      {
        if (this->isDirectory())
        {
          return "n/a";
        }

        return convertSize(this->_fileStat.st_size);
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

      string File::getExtension() const
      {
        if (this->isDirectory())
        {
          return "";
        }
        else
        {
          return this->_extension;
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

      // FileType

      FileType::FileType(string n, string i) : _name(n), _icon(i)
      {}

      string FileType::getName()
      {
        return _name;
      }

      string FileType::getIcon()
      {
        return _icon;
      }

      // FileException

      FileException::FileException(string description) : _description(description)
      {}

      string FileException::getDescription()
      {
        return this->_description;
      }
}
