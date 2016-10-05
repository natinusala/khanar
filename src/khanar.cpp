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
using namespace Glib;
using namespace Gtk;

int main(int argc, char* argv[])
{
  //Ouverture de la fenêtre principale
  Main app(argc, argv);

  try
	{

		
		RefPtr<Builder> win_builder = Assets::buildGtkFromResource(window_glade);

		Window* win = nullptr;
		win_builder->get_widget("window1", win);

    Notebook* notebook = nullptr;
		win_builder->get_widget("notebook1", notebook);
    Box* box_window = nullptr;
		win_builder->get_widget("box1", box_window);
    Box box_window2 = Box();
    RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);

	Label label = Label("Mdrcopter");

    Box* topbarbox = nullptr;
    topbar_builder->get_widget("box1", topbarbox);
    
    notebook->remove_page(0);
    notebook->append_page(*box_window, "lolMdr Khanar Fdp");
    box_window2.pack_start(label,true, true);

    notebook->append_page(box_window2, "Mange tes morts");

    box_window->pack_start(*topbarbox,true, true);
 


		Main::run(*win);
    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
