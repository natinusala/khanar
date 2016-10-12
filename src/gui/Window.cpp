/**
 * \file File.cpp
 *
 * Implémentation des classes d'abstraction des fichiers (File.hpp)
**/

#include "Window.hpp"

#include "../compiled_assets/window.glade.hex"
#include "../compiled_assets/close.glade.hex"
#include "../compiled_assets/new.glade.hex"
#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"
//#include "../compiled_assets/vboxlayout.glade.hex"

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
      this->_notebook->remove_page(0);
      labelOngletPlus();
      return this->_win;
  }


  void Window::addOnglet(Gtk::Widget* widget, string str, bool topBar, bool Prop){
    Gtk::Box* tmp = nullptr;
    Gtk::Label* label = nullptr;
    RefPtr<Builder> box_builder = Assets::buildGtkFromResource(close_glade);
    box_builder->get_widget("label",label);
    label->set_text(str);
    box_builder->get_widget("box1",tmp);
    this->_notebook->remove_page(-1);
    this->_notebook->append_page(*widget,*tmp);
    labelOngletPlus();
  }


  void Window::delOnglet(int idOnglet){
    this->_notebook->remove_page(idOnglet);
  }



  Gtk::Box* Window::getTopBar(){
    RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(topbar_glade);
    Gtk::Box* top_bar = nullptr;
    topbar_builder->get_widget("box1", top_bar);
    return top_bar;

  }



  Gtk::Box* Window::getPropBar(){
    RefPtr<Builder> topbar_builder = Assets::buildGtkFromResource(propertiesbar_glade);
    Gtk::Box* top_bar = nullptr;
    topbar_builder->get_widget("proprietes", top_bar);
    return top_bar;


  }

  void Window::labelOngletPlus(){
    Gtk::Box* tmp = nullptr;
    RefPtr<Builder> box_builder = Assets::buildGtkFromResource(new_glade);
    box_builder->get_widget("box1",tmp);
    this->_notebook->append_page(*tmp,*tmp);

  }


}
