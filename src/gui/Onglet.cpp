
#include "Window.hpp"
#include "Onglet.hpp"



#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"

namespace khanar{
    Onglet::Onglet(string path, string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        this->_builder->get_widget("searchentry1", this->_search);
        _factoryGlade = new FactoryGlade(_builder);
        _factoryContent = new FactoryContent(_wind, wind, path, false, _builder);
        _propBar = (PropertiesBar*)_factoryGlade->getWidget("PropertiesBar")->getContent();

        this->_path = path;
        this->_name = name;
        this->_wind = wind;

        this->_treeView = (FileTreeView*) _factoryContent->getWidget("FileTreeView")->getContent();


        _propBar->setVisible(false);
    }

    Onglet::Onglet(string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        this->_builder->get_widget("searchentry1", this->_search);
        _factoryGlade = new FactoryGlade(_builder);
        _factoryContent = new FactoryContent(_wind, wind, string(""), true, _builder);
        _propBar = (PropertiesBar*)_factoryGlade->getWidget("PropertiesBar")->getContent();
        this->_name = name;
        this->_wind = wind;

        this->_treeView = (FileTreeView*) _factoryContent->getWidget("FileTreeView")->getContent();

        Gtk::Box* prop = nullptr;
        this->_builder->get_widget("proprietes", prop);
        this->_recents = true;
        this->_search->hide();

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
            this->_recents =false;
            this->_search->show();

          }

        }


        void Onglet::on_button_clicked_home()
          {
                this->_wind->actualiser("~");
                this->_recents = false;
                this->_search->show();

            }

        void Onglet::on_button_clicked_navig()
          {
                  File f = File(this->_entry->get_text());
                  if(f.isDirectory()){
                    this->_wind->actualiser(this->_entry->get_text());
                    this->_recents = false;
                    this->_search->show();
                  }

          }

          void Onglet::on_button_clicked_search()
            {
                    string tmp = this->_search->get_text();
                    if(tmp!=""){
                      this->_wind->search(this->_search->get_text());
                    }else{
                      this->_wind->actualiser();
                    }

            }


    Gtk::Box* Onglet::getContent(){

        Gtk::Box* add = nullptr;
        Gtk::Box* container = nullptr;
        Gtk::Button* actualiser = nullptr;
        Gtk::Button* remonter = nullptr;
        Gtk::Button* home = nullptr;
        Gtk::Button* navig = nullptr;
        Gtk::Widget* widget = this->_treeView->getVbox();

        this->_builder->get_widget("box3",container);

        this->_builder->get_widget("box1",add);

        this->_builder->get_widget("actualiser", actualiser);

        this->_builder->get_widget("remonter", remonter);

        this->_builder->get_widget("home", home);

        this->_builder->get_widget("entry1", this->_entry);

        actualiser->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_refresh));
        remonter->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_return));
        home->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_home));
        this->_entry->signal_activate().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_navig));
        this->_search->signal_search_changed().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_search));
        widget->show_all();


        container->pack_start(*widget);
        container->reorder_child(*widget,0);

        this->_child = add;
        return add;
  }
  Gtk::Widget* Onglet::getWidget(){
    return this->_child;
  }
  string Onglet::getName(){
    return this->_treeView->getName();
  }

  void Onglet::actualiser(){
      this->_treeView->reset();
      this->_entry->set_text(this->_treeView->getPath());
      this->_search->set_text("");
      _propBar->setVisible(false);
  }
  void Onglet::actualiser(string filepath){
      this->_treeView->reset(filepath);
      this->_entry->set_text(this->_treeView->getPath());
      this->_search->set_text("");
      _propBar->setVisible(false);
  }
  void Onglet::searchFiles(string search){
      this->_treeView->search(search);
      this->_entry->set_text("Recherche de \"" + search+ "\"");
  }

  void Onglet::setPropBar(File* f){
    _propBar->updateProp(f);
  }

  string Onglet::getPath(){
      return this->_treeView->getPath();

  }
}

Onglet::~Onglet()
{
  delete this->_factoryContent;
  delete this->_factoryGlade;
  delete this->_propBar;
}
