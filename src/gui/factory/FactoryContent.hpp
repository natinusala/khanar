#ifndef FACTORYCONTENT_HPP
#define FACTORYCONTENT_HPP

#include "Factory.hpp"
#include "../widget/WidgetPerso.hpp"
#include <string>
#include <gtkmm.h>
#include "../Window.hpp"

using namespace std;

using namespace khanar;

namespace khanar
{
  class FactoryContent : public Factory
  {
  protected:
    Gtk::Window*& gtkWin;
    khanar::Window* khanarWind;
    string contentPath;
    bool contentRecents;

  public:
    FactoryContent(Gtk::Window*& win,khanar::Window* wind, string path, bool recents, Glib::RefPtr<Gtk::Builder> b) : Factory(b), gtkWin(win), khanarWind(wind), contentPath(path), contentRecents(recents) {}

    WidgetPerso* getWidget() override;
    WidgetPerso* getWidget(string data) override;
  };
}

#endif
