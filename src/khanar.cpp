/**
* \file khanar.cpp
* Point d'entrée de khanar
*/

#include <gtkmm.h>
#include <cstdint>
#include <iostream>
// #include "util/Assets.hpp"
#include "gui/Window.hpp"

#include "gui/Examplewindow.hpp"

#include "compiled_assets/recents.glade.hex"




using namespace std;
using namespace khanar;
using namespace Glib;
using namespace Gtk;

int main(int argc, char* argv[])
{

  Main app(argc, argv);

  try
	{

        RefPtr<Builder> recent_builder = Assets::buildGtkFromResource(recents_glade);
        RefPtr<Builder> recent_builder2 = Assets::buildGtkFromResource(recents_glade);

        Gtk::Window* win = nullptr;
        khanar::Window win_builder = khanar::Window();
        win = win_builder.getWindow();

        Gtk::Widget* chooser1 = nullptr;
        Gtk::Widget* chooser2 = nullptr;
        recent_builder->get_widget("rec", chooser1);
        recent_builder2->get_widget("rec", chooser2);

        win_builder.addOnglet("~/" ,"Test");
        //win_builder.addOnglet(chooser1 ,"Test 2");
        //win_builder.addOnglet(chooser2 ,"Test 3");

        Main::run(*win);

    return 0;
	}
	catch (BuilderError err)
	{
		cout << err.what() << endl;
    return 1;
	}
}
