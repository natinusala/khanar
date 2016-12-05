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


  void Window::addOnglet(string str){
    Onglet onglet = Onglet("", str, this);
    FileTreeView *widget = new FileTreeView(this->_win,this, "");

    Gtk::Box *add = onglet.getContent(widget->getVbox());
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
    this->_notebook->set_current_page(this->_onglets.size()-1);
    actualiser();

  }
  void Window::addOnglet(string filepath, string title){
      //TODO Récupérer la liste des fichier du répèrtoire à afficher
      FileTreeView *widget = new FileTreeView(this->_win,this, filepath);
      Onglet onglet = Onglet(filepath, title, this);

      Gtk::Box *add = onglet.getContent(widget->getVbox());
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
      this->_notebook->set_current_page(this->_onglets.size()-1);
      actualiser();
      //onglet.setPropBar(File("/home/killy/Téléchargements/cunei.ttf"));

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
    int page =  this->_notebook->get_current_page();
    string path = this->_onglets.at(page).getPath();
    FileTreeView *widget = new FileTreeView(this->_win,this, path);
    Onglet onglet = Onglet(path, this->_onglets.at(page).getName(), this);
    _onglets.erase(_onglets.begin() + page);
    _onglets.insert(_onglets.begin() + page, onglet);
    Gtk::Box *add = this->_onglets.at(page).getContent(widget->getVbox());
    Gtk::Box* tmp = nullptr;
    Gtk::Label* label = nullptr;
    RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
    tab_builder->get_widget("label",label);
    label->set_text(this->_onglets.at(page).getName());
    tab_builder->get_widget("box1",tmp);


    this->_notebook->remove_page(page);
    this->_notebook->insert_page(*add ,*tmp, page);

    this->_notebook->set_current_page(page);


  }

  void Window::actualiser(string filepath){
    int page =  this->_notebook->get_current_page();
    string path = filepath;
    FileTreeView *widget = new FileTreeView(this->_win,this, path);
    Onglet onglet = Onglet(path, this->_onglets.at(page).getName(), this);
    _onglets.erase(_onglets.begin() + page);
    _onglets.insert(_onglets.begin() + page, onglet);
    Gtk::Box *add = this->_onglets.at(page).getContent(widget->getVbox());
    Gtk::Box* tmp = nullptr;
    Gtk::Label* label = nullptr;
    RefPtr<Builder> tab_builder = Assets::buildGtkFromResource(close_glade);
    tab_builder->get_widget("label",label);
    label->set_text(this->_onglets.at(page).getName());
    tab_builder->get_widget("box1",tmp);


    this->_notebook->remove_page(page);
    this->_notebook->insert_page(*add ,*tmp, page);
    this->_notebook->set_current_page(page);


  }


string Window::getCurrentFile(){
    int page =  this->_notebook->get_current_page();
    string path = this->_onglets.at(page).getPath();
    return path;
  }
}
