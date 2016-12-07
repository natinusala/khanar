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
 * \brief Namespace du projet
 */
namespace khanar
{
    class Window;
    class FactoryContent;
    class FileTreeView;


    /**
    * \class Onglet
    *
    * Encapsulation des éléments d'interface d'un onglet
    */
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
      /**
      * \brief Constructeur
      * \param[in] name le nom de l'onglet
      * \param[in] wind la fenêtre parent de l'onglet
      * \param[in] win la fenêtre parent Gtk
      */
      Onglet( string name, khanar::Window* wind, Gtk::Window*& win);
      ~Onglet();///< Destructeur de la classe
      void actualiser(); ///< Méthode de mise à jour de la vue avec les informations courantes
      void actualiser(string filepath);///< Méthode de mise à jour de la vue en modifiant le dossier courant
      void searchFiles(string search); ///< Méthode de mise a joure de la vue en fonction d'un critère de recherche
      Gtk::Box* getContent(); ///< Méthode de récupération du contenu de l'onglet pour l'afficher dans la fenêtre
      void setPropBar(File* f); ///< Méthode de mise à jour de l'affichage en détail d'un fichier
      string getPath(); ///< Récupération du chemin absolu du fichier courant
      string getName(); ///< Récupération du nom du fichier courant
      Gtk::Widget* getWidget();///< Méthode Factory de récupération du widget
      string getLabel();

    };

  }
#endif
