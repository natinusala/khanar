/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */


#include <gtkmm.h>
#include "../util/Assets.hpp"
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


        public:
          Window();
          Gtk::Window* getWindow();
          void addOnglet(Gtk::Widget* widget ,string str , bool topBar, bool Prop);
          void addOnglet(string str , bool topBar, bool Prop);
          void delOnglet(int idOnglet);
          void setPropInfo(string name, string type); //TO DO -> Ajouter les arguments correspondant au changement d'informations d'une barre de propriétés
          Gtk::Box* getTopBar();
          Gtk::Box* getPropBar();
    };
}
#endif
