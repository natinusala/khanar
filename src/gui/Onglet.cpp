
#include "Onglet.hpp"
#include "Window.hpp"


#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"

namespace khanar{
    Onglet::Onglet(string path, string name, khanar::Window* wind){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
        this->_path = path;
        this->_name = name;
        this->_wind = wind;
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

    Gtk::Box* Onglet::getContent(Gtk::Widget *widget){

        Gtk::Box* add = nullptr;
        Gtk::Box* container = nullptr;
        Gtk::Button* actualiser = nullptr;
        Gtk::Button* retour = nullptr;

        this->_builder->get_widget("box3",container);

        this->_builder->get_widget("box1",add);

        this->_builder->get_widget("actualiser", actualiser);

        this->_builder->get_widget("precedent", retour);

        actualiser->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_refresh));
        retour->signal_clicked().connect_notify(sigc::mem_fun(*this, &Onglet::on_button_clicked_return));
        widget->show_all();

        container->pack_start(*widget);
        container->reorder_child(*widget,0);
        return add;
  }


  void Onglet::setPropBar(File f){

    Gtk::Label* nom = nullptr;
    Gtk::Label* type = nullptr;
    Gtk::Label* taill = nullptr;
    Gtk::Label* acces = nullptr;
    Gtk::Label* modifie = nullptr;
    Gtk::Label* nomProprio = nullptr;
    Gtk::ComboBoxText* box = nullptr;
    this->_builder->get_widget("nom",nom);
    this->_builder->get_widget("type",type);
    this->_builder->get_widget("taill",taill);
    this->_builder->get_widget("acces",acces);
    this->_builder->get_widget("modifie",modifie);
    this->_builder->get_widget("nomProprietaire",nomProprio);
    this->_builder->get_widget("groupecombo",box);
    nom->set_text(f.getName());
    type->set_text(f.getFileType().getName());
    taill->set_text(to_string(f.getSize()/1000)+"ko");
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


  }

  string Onglet::getPath(){
      return this->_path;

  }

  string Onglet::getName(){
      return this->_name;

  }
}