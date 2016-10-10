/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */

#include "../compiled_assets/window.glade.hex"
#include <gtkmm.h>
#include "../util/Assets.hpp"
#include <cstdint>
#include <iostream>

#ifndef WINDOW_HPP
#define WINDOW_HPP

using namespace Gtk;
using namespace Glib;
using namespace std;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class Window
    {
        private:
          RefPtr<Builder> _win_builder;

        public:
          Window();
          getWindow();
    };
}
#endif
