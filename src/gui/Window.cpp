/**
 * \file Window.cpp
 *
 * Implémentation des classes d'abstraction de l'interface graphique
**/

#include "Window.hpp"
#include "../util/File.hpp"

#include "../compiled_assets/window.glade.hex"
#include "../compiled_assets/close.glade.hex"
#include "../compiled_assets/new.glade.hex"


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

  void Window::updateClipboard(khanar::File file, khanar::File parent, bool shouldDeleteClipboard)
  {
    this->_clipboard = file;
    this->_clipboardParent = parent;
    this->_shouldDeleteClipboard = shouldDeleteClipboard;
  }

  void Window::updatePropBar(khanar::File file)
  {
    int page =  this->_notebook->get_current_page();
    this->_onglets.at(page)->setPropBar(file);
  }

  File Window::getClipboard()
  {
    return this->_clipboard;
  }

  File Window::getClipboardParent()
  {
    return this->_clipboardParent;
  }

  bool Window::getShouldDeleteClipboard()
  {
    return this->_shouldDeleteClipboard;
  }


  void Window::addOnglet(string str){
    Onglet* onglet = new Onglet(str, this, _win);

    Gtk::Box *add = onglet->getContent();
    this->_onglets.push_back(onglet);
    Gtk::Box* tmp = nullptr;
    Gtk::Label* label = nullptr;
    RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
    tab_builder->get_widget("label",label);
    label->set_text(str);
    tab_builder->get_widget("box1",tmp);


    this->_notebook->remove_page(-1);
    this->_notebook->append_page(*add,*tmp);
    this->_notebook->set_current_page(-1);
    actualiser();
    labelOngletPlus();


  }
  void Window::addOnglet(string filepath, string title){
      Onglet* onglet = new Onglet(filepath, title, this, _win);

      Gtk::Box *add = onglet->getContent();
      this->_onglets.push_back(onglet);
      Gtk::Box* tmp = nullptr;
      Gtk::Label* label = nullptr;
      RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
      tab_builder->get_widget("label",label);
      label->set_text(title);
      tab_builder->get_widget("box1",tmp);


      this->_notebook->remove_page(-1);
      this->_notebook->append_page(*add,*tmp);

      actualiser();
      labelOngletPlus();



  }



  void Window::delOnglet(int idOnglet){
    this->_notebook->remove_page(idOnglet);
  }



  void Window::labelOngletPlus(){
    Gtk::Box* tmp = nullptr;
    RefPtr<Builder> box_builder = Assets::buildGtkFromResource(new_glade);
    box_builder->get_widget("box1",tmp);
    this->_notebook->append_page(*tmp,*tmp);
  }

  void Window::actualiser(){
      int page = this->_notebook->get_current_page();
      this->_onglets.at(page)->actualiser();
  }

  void Window::actualiser(string filepath){
    int page = this->_notebook->get_current_page();
    this->_onglets.at(page)->actualiser(filepath);

  }


string Window::getCurrentFile(){
    int page =  this->_notebook->get_current_page();
    string path = this->_onglets.at(page)->getPath();
    return path;
  }
}
