
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
      Gtk::Entry* entry;
      khanar::Window* _wind;
      void on_button_clicked_refresh();
      void on_button_clicked_return();
      void on_button_clicked_navig(GdkEventButton* button_event);


    public:
      /**
      * \brief Constructeur
      * \param[in] path le dossier à ouvrir
      * \param[in] name le nom de l'onglet
      * \param[in] wind la fenêtre parent de l'onglet
      */
      Onglet(string path, string name, khanar::Window* wind);
      Gtk::Box* getContent(Gtk::Widget *widget); ///< Renvoie la Gtk::Box qui contient le contenu de l'onglet
      void setPropBar(File f);
      string getPath();
      string getName();

    };

  }
#endif
