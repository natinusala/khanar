/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "Window.hpp"

namespace khanar
{
  Window::Window()
  {
    this->_win_builder = Assets::buildGtkFromResource(window_glade);
  }
  Gtk::Window*::getWindow(){
      Window* win = nullptr;
      this->win_builder->getwidget("window1", win);
      return win;
  }
}
