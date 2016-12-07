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
#include "FileTreeView.hpp"



#ifndef ONGLET_HPP
#define ONGLET_HPP


using namespace Gtk;
using namespace Glib;
using namespace std;
using namespace khanar;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class Window;
    class FileTreeView;
    class Onglet{

    private:
      RefPtr<Builder> _builder;
      string _path;
      string _name;
      Gtk::Entry* entry;
      FileTreeView* treeview;
      khanar::Window *_wind;
      void on_button_clicked_refresh();
      void on_button_clicked_return();
      void on_button_clicked_navig(GdkEventButton* button_event);


    public:
      /**
      * \brief Constructeur
      * \param[in] path le dossier à ouvrir
      * \param[in] name le nom de l'onglet
      * \param[in] wind la fenêtre parent de l'onglet
      * \param[in] win la fenêtre parent Gtk
      */
      Onglet(string path, string name, khanar::Window* wind, Gtk::Window*& win);
      Onglet( string name, khanar::Window* wind, Gtk::Window*& win);
      void actualiser();
      void actualiser(string filepath);
      Gtk::Box* getContent();
      void setPropBar(File f);
      string getPath();
      string getName();

    };

  }
#endif
