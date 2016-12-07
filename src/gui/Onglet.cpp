
#include "Window.hpp"
#include "Onglet.hpp"



#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"

namespace khanar{
    Onglet::Onglet(string path, string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        this->_builder->get_widget("searchentry1", this->search);
        this->_path = path;
        this->_name = name;
        this->_wind = wind;

        this->treeview = new FileTreeView(_wind, wind , path, false);


        Gtk::Box* prop = nullptr;
        this->_builder->get_widget("proprietes", prop);

        prop->hide();
    }

    Onglet::Onglet(string name, khanar::Window* wind, Gtk::Window*& _wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        this->_builder->get_widget("searchentry1", this->search);
        this->_name = name;
        this->_wind = wind;
        this->treeview = new FileTreeView(_wind, wind , "", true);

        Gtk::Box* prop = nullptr;
        this->_builder->get_widget("proprietes", prop);
        recents = true;
        this->search->hide();
        prop->hide();
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
            recents =false;
            this->search->show();

          }

        }


        void Onglet::on_button_clicked_home()
          {
                this->_wind->actualiser("~");
                recents = false;
                this->search->show();

            }

        void Onglet::on_button_clicked_navig()
          {
                  File f = File(this->entry->get_text());
                  if(f.isDirectory()){
                    this->_wind->actualiser(this->entry->get_text());
                    recents = false;
                    this->search->show();
                  }

          }

          void Onglet::on_button_clicked_search()
            {
                    string tmp = this->search->get_text();
                    if(tmp!=""){
                      this->_wind->search(this->search->get_text());
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
        Gtk::Widget* widget = this->treeview->getVbox();

        this->_builder->get_widget("box3",container);

        this->_builder->get_widget("box1",add);

        this->_builder->get_widget("actualiser", actualiser);

        this->_builder->get_widget("remonter", remonter);

        this->_builder->get_widget("home", home);

        this->_builder->get_widget("entry1", this->entry);

        actualiser->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_refresh));
        remonter->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_return));
        home->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_home));
        this->entry->signal_activate().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_navig));
        this->search->signal_search_changed().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_search));
        widget->show_all();


        container->pack_start(*widget);
        container->reorder_child(*widget,0);

        this->child = add;
        return add;
  }
  Gtk::Widget* Onglet::getWidget(){
    return this->child;
  }
  string Onglet::getName(){
    return this->treeview->getName();
  }

  void Onglet::actualiser(){
      this->treeview->reset();
      this->entry->set_text(this->treeview->getPath());
      this->search->set_text("");
  }
  void Onglet::actualiser(string filepath){
      this->treeview->reset(filepath);
      this->entry->set_text(this->treeview->getPath());
      this->search->set_text("");
  }
  void Onglet::searchFiles(string search){
      this->treeview->search(search);
      this->entry->set_text("Recherche de \"" + search+ "\"");
  }

  void Onglet::setPropBar(File f){

    Gtk::Label* nom = nullptr;
    Gtk::Label* type = nullptr;
    Gtk::Label* taill = nullptr;
    Gtk::Label* acces = nullptr;
    Gtk::Label* modifie = nullptr;
    Gtk::Label* nomProprio = nullptr;
    Gtk::ComboBoxText* box = nullptr;
    Gtk::Image* icon = nullptr;


    Gtk::Box* prop = nullptr;
    this->_builder->get_widget("proprietes", prop);

    prop->show();

    this->_builder->get_widget("nom", nom);
    this->_builder->get_widget("type", type);
    this->_builder->get_widget("taill", taill);
    this->_builder->get_widget("acces", acces);
    this->_builder->get_widget("modifie", modifie);
    this->_builder->get_widget("nomProprietaire", nomProprio);
    this->_builder->get_widget("groupecombo", box);
    this->_builder->get_widget("Icone", icon);

    nom->set_text(f.getName());
    type->set_text(f.getFileType().getName());
    taill->set_text(f.getFormattedSize());
    char buffer [50];
    time_t rawtime = (time_t) f.getLastAccessTime();
    struct tm * timeinfo;
    timeinfo = localtime (&rawtime);
    strftime (buffer,20,"%d/%m/%y",timeinfo);
    char acc [50]= "Accédé : ";
    char result[100];
    strcpy(result,acc);
    strcat(result,buffer);
    acces->set_text(result);
    buffer [50];
    rawtime = (time_t) f.getLastModificationTime();
    timeinfo = localtime (&rawtime);
    strftime (buffer,20,"%d/%m/%y",timeinfo);
    char acc2 [50]= "Modifié : ";
    result[100];
    strcpy(result,acc2);
    strcat(result,buffer);
    modifie->set_text(result);

    icon->set_from_icon_name(f.getFileType().getIcon(), Gtk::BuiltinIconSize::ICON_SIZE_DIALOG);

    nomProprio->set_text(File::getUIDName(f.getUID()));
  }

  string Onglet::getPath(){
      return this->treeview->getPath();

  }
}
