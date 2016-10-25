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


  void Window::addOnglet(Gtk::Widget* widget, string str){
    TabContent onglet = TabContent();

    Gtk::Box *add = onglet.getContent(widget);
    this->_onglets.push_back(onglet);
    Gtk::Box* tmp = nullptr;
    Gtk::Label* label = nullptr;
    RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
    tab_builder->get_widget("label",label);
    label->set_text(str);
    tab_builder->get_widget("box1",tmp);


    this->_notebook->remove_page(-1);
    this->_notebook->append_page(*add,*tmp);
    labelOngletPlus();

    onglet.setPropBar(File("/home/killy/Téléchargements/cunei.ttf"));

  }
  void Window::addOnglet(string filepath, string title){
      //TODO Récupérer la liste des fichier du répèrtoire à afficher

      TreeViewLib tvl(filepath);

      TabContent onglet = TabContent();

      Gtk::Box *add = onglet.getContent(tvl.getVBox());
      this->_onglets.push_back(onglet);
      Gtk::Box* tmp = nullptr;
      Gtk::Label* label = nullptr;
      RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
      tab_builder->get_widget("label",label);
      label->set_text(title);
      tab_builder->get_widget("box1",tmp);


      this->_notebook->remove_page(-1);
      this->_notebook->append_page(*add,*tmp);
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



}
