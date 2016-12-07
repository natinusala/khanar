#ifndef FACTORYCONTENT_HPP
#define FACTORYCONTENT_HPP

#include "Factory.hpp"
#include "../widget/WidgetPerso.hpp"
#include <string>
#include <gtkmm.h>
#include "../Window.hpp"

using namespace std;

using namespace khanar;
/**
 * \namespace khanar
 *
 * \brief Namespace du projet
 */
namespace khanar
{
  /**
  * \class FactoryContent
  *
  * Implémentation de Factory donnant des widgets générés par le code
  */
  class FactoryContent : public Factory
  {
  protected:
    Gtk::Window*& gtkWin; ///< La fenêtre Gtk::Window
    khanar::Window* khanarWind; ///< La fenêtre khanar::Window
    string contentPath; ///< Le chemin de l'onglet
    bool contentRecents; ///< Est-ce que l'onglet affiche les contenus récents ?

  public:
    /**
    * \brief Surcharge du constructeur prenant les paramètres supplémentaires nécessaires
    */
    FactoryContent(Gtk::Window*& win,khanar::Window* wind, string path, bool recents, Glib::RefPtr<Gtk::Builder> b) : Factory(b), gtkWin(win), khanarWind(wind), contentPath(path), contentRecents(recents) {}

    WidgetPerso* getWidget(string data) override;
  };
}

#endif
