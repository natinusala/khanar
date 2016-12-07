
#include "Window.hpp"
#include "Onglet.hpp"



#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"

namespace khanar{
    Onglet::Onglet(string path, string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        _propBar = new PropertiesBar(_builder);
        this->_path = path;
        this->_name = name;
        this->_wind = wind;

        this->treeview = new FileTreeView(_wind, wind , path, false);


        _propBar->setVisible(false);
    }

    Onglet::Onglet(string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        _propBar = new PropertiesBar(_builder);
        this->_name = name;
        this->_wind = wind;
        this->treeview = new FileTreeView(_wind, wind , "", true);

        _propBar->setVisible(false);
    }

    void Onglet::on_button_clicked_refresh()
      {
        this->_wind->actualiser();
      }

    void Onglet::on_button_clicked_return()
        {
          File f = File(this->_wind->getCurrentFile());
          string path = f.getParentFolderAbsolutePath();
          if( path != ""){
            this->_wind->actualiser(f.getParentFolderAbsolutePath());
          }

        }


        void Onglet::on_button_clicked_navig(GdkEventButton* button_event)
            {
              string path = this->entry->get_text();
              File f = File(path);
              if(f.isDirectory()){
                this->_wind->actualiser(path);
              }

            }

    Gtk::Box* Onglet::getContent(){

        Gtk::Box* add = nullptr;
        Gtk::Box* container = nullptr;
        Gtk::Button* actualiser = nullptr;
        Gtk::Button* retour = nullptr;
        Gtk::Button* navig = nullptr;
        Gtk::Widget* widget = this->treeview->getVbox();
        this->entry = nullptr;

        this->_builder->get_widget("box3",container);

        this->_builder->get_widget("box1",add);

        this->_builder->get_widget("actualiser", actualiser);

        this->_builder->get_widget("precedent", retour);

        this->_builder->get_widget("navig", navig);

        this->_builder->get_widget("entry1", this->entry);

        this->entry->set_activates_default(true);
        navig->set_can_default(true);
        actualiser->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_refresh));
        retour->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_return));
        //navig->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_navig));
        //navig->signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_navig));
        widget->show_all();

        container->pack_start(*widget);
        container->reorder_child(*widget,0);
        return add;
  }


  void Onglet::actualiser(){
      this->treeview->reset();
  }
  void Onglet::actualiser(string filepath){
      this->treeview->reset(filepath);
  }
  void Onglet::setPropBar(File* f){
    _propBar->updateProp(f);
  }

  string Onglet::getPath(){
      return this->treeview->getPath();

  }

  string Onglet::getName(){
      return this->_name;

  }
}

Onglet::~Onglet()
{

  delete this->_propBar;
}
