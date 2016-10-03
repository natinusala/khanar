/**
* \file khanar.cpp
* Point d'entrée de khanar
*/

#include <gtkmm.h>
#include <cstdint>
#include <iostream>
#include "util/Assets.hpp"

#include "compiled_assets/window.glade.hex"

using namespace std;
using namespace khanar;

int main(int argc, char* argv[])
{
  //Ouverture de la fenêtre principale
  Gtk::Main app(argc, argv);

  try
	{
		Glib::RefPtr<Gtk::Builder> builder = Assets::buildGtkFromResource(window_glade);
		Gtk::Window* win = nullptr;
		builder->get_widget("window1", win);
		Gtk::Main::run(*win);
    return 0;
	}
	catch (Gtk::BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
