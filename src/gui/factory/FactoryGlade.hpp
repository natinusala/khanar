#ifndef FACTORYGLADE_HPP
#define FACTORYGLADE_HPP

#include "Factory.hpp"
#include "../widget/WidgetPerso.hpp"
#include <string>

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
  * \class FactoryGlade
  *
  * Implémentation de Factory donnant des widgets générés par Glade
  */
  class FactoryGlade : public Factory
  {
  public:
    FactoryGlade(Glib::RefPtr<Gtk::Builder> b) : Factory(b) {}

    WidgetPerso* getWidget(string data) override;
  };
}

#endif
