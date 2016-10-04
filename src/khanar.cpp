/**
* \file khanar.cpp
* Point d'entrée de khanar
*/

#include <gtkmm.h>
#include <cstdint>
#include <iostream>
#include "util/Assets.hpp"

#include "compiled_assets/window.glade.hex"
#include "compiled_assets/topbar.glade.hex"

using namespace std;
using namespace khanar;

int main_khanar(int argc, char* argv[])
{
  //Ouverture de la fenêtre principale
  Gtk::Main app(argc, argv);

  try
	{
		Glib::RefPtr<Gtk::Builder> win_builder = Assets::buildGtkFromResource(window_glade);

		Gtk::Window* win = nullptr;
		win_builder->get_widget("window1", win);

    Gtk::Notebook* notebook = nullptr;
		win_builder->get_widget("notebook1", notebook);

    Glib::RefPtr<Gtk::Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);

    Gtk::Box* topbarbox = nullptr;
    topbar_builder->get_widget("box1", topbarbox);

    notebook->append_page(*topbarbox);

		Gtk::Main::run(*win);
    return 0;
	}
	catch (Gtk::BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
