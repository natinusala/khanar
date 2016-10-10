/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "Window.hpp"

#include "../compiled_assets/window.glade.hex"


namespace khanar
{
  Window::Window()
  {
    this->_win_builder = Assets::buildGtkFromResource(window_glade);
    this->_win = nullptr;
    this->_notebook=nullptr;
    this->_win_builder->get_widget("window1", _win);
    this->_win_builder->get_widget("notebook1", _notebook);
  }
  Gtk::Window* Window::getWindow(){

      return this->_win;
  }
  void Window::addOnglet(Gtk::Widget* widget, string str){
      this->_notebook->append_page(*widget,str);
  }

}
