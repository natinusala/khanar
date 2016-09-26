/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "File.hpp"

namespace khanar_common_file
{
      /* FileComponent */

      //TODO Tester les constructeurs

      FileComponent::FileComponent(shared_ptr<FileComponent> parent, string name)
      {
        this->_name = name;

        if (parent != NULL)
        {
          this->_absolutePath = parent->_absolutePath + name;
        }
        else
        {
          this->_absolutePath = "/" + name;
        }
      }

      FileComponent::FileComponent(string absolutepath)
      {
        this->_absolutePath = absolutepath;
        size_t pos = absolutepath.find_last_of("/");
        this->_name = absolutepath.substr(pos+1);
      }

      string FileComponent::getName()
      {
        return this->_name;
      }

      string FileComponent::getAbsolutePath()
      {
        return this->_absolutePath;
      }
}
