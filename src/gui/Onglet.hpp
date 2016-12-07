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
#include "widget/PropertiesBar.hpp"
#include "widget/FileTreeView.hpp"
#include "factory/FactoryGlade.hpp"
#include "factory/FactoryContent.hpp"



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
    class FactoryContent;
    class FileTreeView;
    class Onglet{

    private:
      RefPtr<Builder> _builder;
      string _path;
      string _name;
      Gtk::Entry* _entry;
      Gtk::SearchEntry* _search;
      Gtk::Widget* _child;
      khanar::Window *_wind;
      bool _recents = false;

      void on_button_clicked_refresh();
      void on_button_clicked_return();
      void on_button_clicked_home();
      void on_button_clicked_navig();
      void on_button_clicked_search();


      FactoryGlade* _factoryGlade;
      FactoryContent* _factoryContent;
      PropertiesBar* _propBar;
      FileTreeView* _treeView;


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
      ~Onglet();
      void actualiser();
      void actualiser(string filepath);
      void searchFiles(string search);
      Gtk::Box* getContent();
      void setPropBar(File* f);
      string getPath();
      string getName();
      Gtk::Widget* getWidget();
      string getLabel();

    };

  }
#endif
