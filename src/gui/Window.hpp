/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */

#include "TabContent.hpp"
#include <gtkmm.h>
#include "../util/Assets.hpp"
#include "Examplewindow.hpp"
#include <cstdint>
#include <iostream>



#ifndef WINDOW_HPP
#define WINDOW_HPP

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
    class Window
    {
        private:
          RefPtr<Builder> _win_builder;
          Gtk::Window* _win;
          Gtk::Notebook* _notebook;
          vector<TabContent> _onglets;
          //Signal handlers:


        public:
          Window();
          Gtk::Window* getWindow();
          void addOnglet(Gtk::Widget* widget ,string str );
          void delOnglet(int idOnglet);
          void setPropInfo(string nom, string type, string icon, string Dateacces, string Datemodif, string proprietaire,string DroitProprietaire, string Groupe, string DroitGroupe, string DroitAutre ); //TO DO -> Ajouter les arguments correspondant au changement d'informations d'une barre de propriétés
          void addOnglet(string folder ,string title);
          void resetPropInfo();
          void labelOngletPlus();
          void actualiser();


    };
}
#endif
