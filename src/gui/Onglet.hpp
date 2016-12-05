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

#ifndef ONGLET_HPP
#define ONGLET_HPP


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
    class Onglet{

    private:
      RefPtr<Builder> _builder;
      string _path;
      string _name;
      khanar::Window* _wind;
      void on_button_clicked_refresh();
      void on_button_clicked_return();


    public:
      Onglet(string path, string name, khanar::Window* wind);
      Gtk::Box* getContent(Gtk::Widget *widget);
      void setPropBar(File f);
      string getPath();
      string getName();

    };

  }
#endif
