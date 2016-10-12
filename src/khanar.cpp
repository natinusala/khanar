/**
* \file khanar.cpp
* Point d'entrée de khanar
*/

#include <gtkmm.h>
#include <cstdint>
#include <iostream>
// #include "util/Assets.hpp"
#include "gui/Window.hpp"

/*#include "compiled_assets/window.glade.hex"
#include "compiled_assets/topbar.glade.hex"/*
#include "compiled_assets/boxlayout.glade.hex"
*/#include "compiled_assets/recents.glade.hex"/*
#include "compiled_assets/panelsidebar.glade.hex"*/

using namespace std;
using namespace khanar;
using namespace Glib;
using namespace Gtk;

int main_khanar(int argc, char* argv[])
{
  //Ouverture de la fenêtre principale
  Main app(argc, argv);

  try
	{
    //Création des constructeurs glade
      /*RefPtr<Builder> win_builder = Assets::buildGtkFromResource(window_glade);
        RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);
        RefPtr<Builder> topbar_builder2 = Assets::buildGtkFromResource(topbar_glade);
      /*  RefPtr<Builder> box_builder = Assets::buildGtkFromResource(boxlayout_glade);
        RefPtr<Builder> sidebar_builder = Assets::buildGtkFromResource(panelsidebar_glade);*/
        RefPtr<Builder> recent_builder = Assets::buildGtkFromResource(recents_glade);
        RefPtr<Builder> recent_builder2 = Assets::buildGtkFromResource(recents_glade);

        Gtk::Window* win = nullptr;
        khanar::Window win_builder = khanar::Window();
        win = win_builder.getWindow();

        Gtk::Widget* chooser1 = nullptr;
        Gtk::Widget* chooser2 = nullptr;
        recent_builder->get_widget("rec", chooser1);
        recent_builder2->get_widget("rec", chooser2);

        win_builder.addOnglet(chooser1, "Test", true, true);
        win_builder.addOnglet(chooser2, "Test 2", false, false);
        Main::run(*win);
    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
