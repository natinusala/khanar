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
#include <fstream>
#include <cstring>
#include <grp.h>
#include <unistd.h>
#include <regex>
#include <pwd.h>
#include <mntent.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include "../libs/json/json.h"

namespace khanar
{
      // File
      string File::FAVORITES_DIRECTORY = "~/.config/khanar/favorites.json";

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

      void File::notifyObservers()
      {
        for (int i = 0; i < this->_observers.size(); i++)
        {
          this->_observers.at(i)->fileUpdated(this);
        }
      }

      void File::subscribeObserver(FileObserver* observer)
      {
        this->_observers.push_back(observer);
      }

      void File::unsubscribeObserver(FileObserver* observer)
      {
        this->_observers.erase(std::remove(this->_observers.begin(), this->_observers.end(), observer), this->_observers.end());
      }

      vector<File> File::getFavorites()
      {
        vector<File> v;

        File favorites = File(File::FAVORITES_DIRECTORY);

        if (!favorites.exists())
          favorites.createNewFile(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        Json::Value root;
        Json::Reader().parse(favorites.read(), root);

        if (root.type() == Json::arrayValue)
        {
          for (int i = 0; i < root.size(); i++)
          {
            v.push_back(File(root[i].asString()));
          }
        }

        return v;
      }

      void File::addToFavorites()
      {
        vector<File> v = File::getFavorites();
        v.push_back(*this);

        File::updateFavorites(v);
      }

      void File::removeFromFavorites()
      {
        vector<File> v = File::getFavorites();
        v.erase(std::remove(v.begin(), v.end(), *this), v.end());

        File::updateFavorites(v);
      }

      bool File::isInFavorites()
      {
        vector<File> v = File::getFavorites();

        for (int i = 0; i < v.size(); i++)
        {
          if (v.at(i) == *this)
            return true;
        }

        return false;
      }

      void File::updateFavorites(vector<File> newFavorites)
      {
        File favorites = File(File::FAVORITES_DIRECTORY);

        if (!favorites.exists())
          favorites.createNewFile(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        Json::Value root = Json::Value(Json::arrayValue);

        for (int i = 0; i < newFavorites.size(); i++)
        {
          root.append(newFavorites.at(i).getAbsolutePath());
        }

        string json = Json::FastWriter().write(root);
        favorites.write(json);
      }

      vector<File> File::getMountedVolumes()
      {
        FILE* mtab = setmntent("/etc/mtab", "r");
        struct mntent* m;
        struct mntent mnt;
        char strings[4096];

        vector<File> result;

        while ((m = getmntent_r(mtab, &mnt, strings, sizeof(strings))))
        {
          struct statfs fs;
          if (mnt.mnt_dir != NULL && statfs(mnt.mnt_dir, &fs) == 0)
          {
            string name = string(mnt.mnt_fsname); // /dev/machin (utilisé pour trier les volumes voulus)
            string dir = string(mnt.mnt_dir);     // /media/machin/truc (chemin final du File retourné)
            string prefix = "/dev/";

            if (!name.compare(0, prefix.size(), prefix) && dir != "/")
            {
              result.push_back(File(dir));
            }
          }
        }

        endmntent(mtab);

        return result;
      }

      void File::openXterm() const
      {
        if (!this->isDirectory())
        {
          throw FileException("Ce fichier n'est pas un dossier.");
        }

        if (fork() == 0)
        {
          string command = "cd \"" + this->_absolutePath + "\" && xterm";
          system(command.c_str());
          exit(0);
        }
      }

      void File::write(string data)
      {
        ofstream file = ofstream(this->_absolutePath);
        if (file.is_open())
        {
          file << data;
          file.close();
          this->updateStat();
        }
      }

      string File::read()
      {
        std::ostringstream os;
        ifstream file = ifstream(this->_absolutePath);
        if (file.is_open())
        {
          os << file.rdbuf();
          file.close();
        }
        return os.str();
      }

      void File::updateAttributes(string absolutepath)
      {
        if (absolutepath.empty())
        {
          absolutepath = "/";
        }

        //Formattage du nom
        if (absolutepath.back() == '/' && absolutepath != "/")
        {
          absolutepath = absolutepath.substr(0, absolutepath.length()-1);
        }

        //Expansion du nom (par exemple transformer "~" en "/home/user")
        wordexp_t exp_result;
        if (wordexp(absolutepath.c_str(), &exp_result, 0) == 0)
        {
          absolutepath = string(exp_result.we_wordv[0]);
        }


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
        this->notifyObservers();
      }

      void File::setUID(uid_t const& uid)
      {
        chown(this->_absolutePath.c_str(), uid, -1);
        this->updateStat();
      }

      void File::setGID(gid_t const& gid)
      {
        chown(this->_absolutePath.c_str(), -1, gid);
        this->updateStat();
      }

      vector<File> File::search(string expression)
      {
        if (!this->isDirectory())
        {
          throw FileException("Le fichier n'est pas un dossier");
        }
        this->updateSubFiles();

        vector<File> result;
        regex r = regex(expression);

        for (int i = 0; i < this->_subFiles.size(); i++)
        {
          File f = this->_subFiles.at(i);

          if (regex_match(f.getName(), r))
          {
            result.push_back(f);
          }
        }

        return result;
      }

      string File::getName() const
      {
        return this->_name;
      }

      long File::getLastAccessTime() const
      {
        return this->_fileStat.st_atime;
      }

      void File::createNewFile(mode_t mode)
      {
        if (this->_exists)
        {
          throw FileException("Le fichier existe déjà");
        }

        File parent = File(this->_parentFolderAbsolutePath);
        vector<File> toCreate;

        while (!parent.exists())
        {
          toCreate.push_back(parent);
          parent = File(parent.getParentFolderAbsolutePath());
        }

        for (int i = toCreate.size()-1; i >= 0; i--)
        {
          mkdir(toCreate.at(i).getAbsolutePath().c_str(), mode);
        }

        fstream fs;
        fs.open(this->_absolutePath, ios::out);
        fs.close();

        this->updateStat();
      }

      unsigned File::getUID() const
      {
        return this->_fileStat.st_uid;
      }

      unsigned File::getGID() const
      {
        return this->_fileStat.st_gid;
      }

      string File::getGIDName(gid_t const& gid)
      {
        return string(getgrgid(gid)->gr_name);
      }

      string File::getUIDName(uid_t const& uid)
      {
        return string(getpwuid(uid)->pw_name);
      }

      long File::getLastModificationTime() const
      {
        return this->_fileStat.st_mtime;
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

      void File::removeFile()
      {
        if (this->isDirectory())
        {
          this->updateSubFiles();
          for (int i = 0; i < this->_subFiles.size(); i++)
          {
            this->_subFiles.at(i).removeFile();
          }
        }

        remove(this->_absolutePath.c_str());
        this->updateStat();
      }

      File File::copy(string newpath) const
      {
        ifstream src = ifstream(this->_absolutePath, ios::binary);
        ofstream dst = ofstream(newpath, ios::binary);

        dst << src.rdbuf();

        return File(newpath);
      }

      void File::move(string newpath)
      {
        rename(this->_absolutePath.c_str(), newpath.c_str());
        updateAttributes(newpath);
      }

      void File::setPermission(enum Permission const& perm, bool const& value)
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

      bool File::getPermission(enum Permission const& perm) const
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

      void File::setSortStrategy(FileSortStrategy const& strategy, bool const& descending)
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
          if (strcmp(fichier->d_name, ".") != 0 && strcmp(fichier->d_name, ".."))
          {
            list.push_back(File(this->_absolutePath + "/" + string(fichier->d_name)));
          }
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

        this->notifyObservers();
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
