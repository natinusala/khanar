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
		RefPtr<Builder> win_builder = Assets::buildGtkFromResource(window_glade);
    RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);
    RefPtr<Builder> box_builder = Assets::buildGtkFromResource(boxlayout_glade);


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


	

    Box* topbarbox = nullptr;
    topbar_builder->get_widget("box1", topbarbox);

    notebook->remove_page(0);
    notebook->append_page(*box_window, "lolMdr Khanar Fdp");

    notebook->append_page(*box_window2, "Mange tes morts");

    box_window->pack_start(*topbarbox,true, true);
    box_window2->pack_start(label,true,true);


		Main::run(*win);
    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
