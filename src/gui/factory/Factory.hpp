#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "../widget/WidgetPerso.hpp"
#include <string>

using namespace std;

using namespace khanar;

namespace khanar
{
  class Factory
  {
  protected:
    Glib::RefPtr<Gtk::Builder> builder;

  public:

    Factory(Glib::RefPtr<Gtk::Builder> b) : builder(b){}

    virtual WidgetPerso* getWidget() = 0;
    virtual WidgetPerso* getWidget(string data) = 0;
  };
}

#endif
