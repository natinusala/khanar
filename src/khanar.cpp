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
#include "compiled_assets/boxlayout.glade.hex"
#include "compiled_assets/recents.glade.hex"
#include "compiled_assets/panelsidebar.glade.hex"

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
		RefPtr<Builder> win_builder = Assets::buildGtkFromResource(window_glade);
    RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);
    RefPtr<Builder> topbar_builder2 = Assets::buildGtkFromResource(topbar_glade);
    RefPtr<Builder> box_builder = Assets::buildGtkFromResource(boxlayout_glade);
    RefPtr<Builder> sidebar_builder = Assets::buildGtkFromResource(panelsidebar_glade);


		Window* win = nullptr;
    //Initialisation de la fenêtre
    win_builder->get_widget("window1", win);

    Notebook* notebook = nullptr;
    //Initialisation de la boite a onglet
		win_builder->get_widget("notebook1", notebook);
    Box* box_window = nullptr;
    //Initialisation de la boite conteneur
		win_builder->get_widget("box1", box_window);
    Box* box_window2 = nullptr;
    //Initialisation de la boite conteneur test
    box_builder->get_widget("box1", box_window2);

    Box* raccourcis = nullptr;
    sidebar_builder->get_widget("Raccourcis", raccourcis);


    //Box* box_window3 = nullptr;
    //Initialisation de la boite conteneur test
    //box_builder->get_widget("box1", box_window3);

    Box* topbarbox = nullptr;
    topbar_builder->get_widget("box1", topbarbox);

    Box* topbarbox2 = nullptr;
    topbar_builder2->get_widget("box1", topbarbox2);

    Box* container = nullptr;
    box_builder->get_widget("box2", container);
    container->pack_start(*raccourcis, false, false);
    container->reorder_child(*raccourcis,0);

    notebook->remove_page(0);
    notebook->append_page(*box_window, "Récemment utilisés");

    notebook->append_page(*box_window2, "Favoris");

    box_window->pack_start(*topbarbox,true, true);
    box_window2->pack_start(*topbarbox2, false,true);
    box_window2->pack_end(*container,false, true);
    box_window2->reorder_child(*topbarbox2,0);


		Main::run(*win);
    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
