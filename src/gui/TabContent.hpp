/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */


#include <gtkmm.h>
#include "../util/Assets.hpp"
#include "../util/File.hpp"
#include <cstdint>
#include <iostream>

#ifndef TABCONTENT_HPP
#define TABCONTENT_HPP


using namespace Gtk;
using namespace Glib;
using namespace std;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class Window;
    class TabContent{

    private:
      RefPtr<Builder> _builder;
      string path;
      string name;
      khanar::Window* wind;


    public:
      TabContent(string path, string name, khanar::Window* wind);
      Gtk::Box* getContent(Gtk::Widget *widget);
      void setPropBar(File f);
      string getPath();
      string getName();





    };

  }
#endif
