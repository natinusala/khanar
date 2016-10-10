/**
* \file khanar.cpp
* Point d'entrée de khanar
*/

#include <gtkmm.h>
#include <cstdint>
#include <iostream>
// #include "util/Assets.hpp"
#include "util/Window.hpp"

/*#include "compiled_assets/window.glade.hex"*/
#include "compiled_assets/topbar.glade.hex"/*
#include "compiled_assets/boxlayout.glade.hex"
#include "compiled_assets/recents.glade.hex"
#include "compiled_assets/panelsidebar.glade.hex"*/

using namespace std;
using namespace khanar;
using namespace Glib;
using namespace Gtk;

int main(int argc, char* argv[])
{
  //Ouverture de la fenêtre principale
  Main app(argc, argv);

  try
	{
    //Création des constructeurs glade
      /*RefPtr<Builder> win_builder = Assets::buildGtkFromResource(window_glade);*/
        RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);
      /*  RefPtr<Builder> topbar_builder2 = Assets::buildGtkFromResource(topbar_glade);
        RefPtr<Builder> box_builder = Assets::buildGtkFromResource(boxlayout_glade);
        RefPtr<Builder> sidebar_builder = Assets::buildGtkFromResource(panelsidebar_glade);*/

        Gtk::Window* win = nullptr;
        khanar::Window win_builder = khanar::Window();
        win = win_builder.getWindow();

        Box* topbarbox = nullptr;
        topbar_builder->get_widget("box1", topbarbox);

        win_builder.addOnglet(topbarbox, "test");
        Main::run(*win);
    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
