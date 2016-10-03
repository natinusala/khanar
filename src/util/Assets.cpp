#include "Assets.hpp"

#include <cstdint>
#include <string>
#include <gtkmm.h>

using namespace std;

namespace khanar
{
  Glib::RefPtr<Gtk::Builder> Assets::buildGtkFromResource(unsigned char resource[])
  {
    string glade_str = string(reinterpret_cast<char*>(resource)); //passer de unsigned char* à char*
    return Gtk::Builder::create_from_string(glade_str);
  }
}
