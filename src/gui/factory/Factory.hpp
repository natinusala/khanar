#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "../widget/WidgetPerso.hpp"
#include <string>

using namespace std;

using namespace khanar;

namespace khanar
{
  /**
  * \class FileType
  *
  * Interface servant de base aux Factory du projet
  */
  class Factory
  {
  protected:
    Glib::RefPtr<Gtk::Builder> builder; ///< Gtk::Builder qui contient les éléments de l'interface graphique

  public:

    /**
    * \brief Constructeur par défaut de la factory
    * \param[in] b Le Gtk::Builder qui contient les éléments de l'interface graphique
    */
    Factory(Glib::RefPtr<Gtk::Builder> b) : builder(b){}

    virtual WidgetPerso* getWidget(string data) = 0; ///< Méthode de la factory pour créer un widget
  };
}

#endif
