#include <iostream>
#include "Examplewindow.hpp"
#include "../util/File.hpp"

using namespace khanar;


//TODO: Ajouter un attribut vector pour retenir les fichiers contenu dans l'interface.
//TODO Couper/copier avec this->clipboard et this->shouldDeleteClipboard

void ExampleWindow::on_button_press(GdkEventButton* button_event)
{
  if((button_event->type == 4) && (button_event->button == 3))
  {
    _menuPopup.popup(button_event->button, button_event->time);
  }
}


ExampleWindow::ExampleWindow(string path)
{
  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  this->m_VBox.set_size_request(250,250);
  this->m_VBox.set_hexpand(true);
  this->m_VBox.set_vexpand(true);
  this->m_VBox.pack_start(m_ScrolledWindow);
  //Create the Tree model:
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);

  this->f = new File(path);
  subFiles = this->f->getSubFiles();

  Gtk::TreeModel::Row row;
  for (int i = 0; i < subFiles->size(); i++)
  {
    File f = (*subFiles)[i];
    if(!f.isHidden()){
    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_ico]= f.getFileType().getIcon();
    row[m_Columns.m_col_name] = f.getName();
    string size =f.getFormattedSize();
    if (size =="n/a"){
      row[m_Columns.m_col_number] = " ";
    }else{
      row[m_Columns.m_col_number] = size;
  }
  }
  }

  auto cell = Gtk::manage(new Gtk::CellRendererPixbuf);
  m_TreeView.append_column("   ", *cell);
  auto pColumn = m_TreeView.get_column(0);
  if(pColumn)
  {
    Glib::ustring str = "icon_name";
    pColumn->add_attribute(*cell,str,m_Columns.m_col_ico);
  }
  m_TreeView.append_column("Nom", m_Columns.m_col_name);
  m_TreeView.append_column("Taille", m_Columns.m_col_number);
  //Make all the columns reorderable:
  //This is not necessary, but it's nice to show the feature.
  //You can use TreeView::set_column_drag_function() to more
  //finely control column drag and drop.
  for(guint i = 0; i < 2; i++)
  {
    auto column = m_TreeView.get_column(i);
    column->set_reorderable();
  }


  auto item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau fichier", true));
  _menuPopup.append(*item);

  item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau dossier", true));
  _menuPopup.append(*item);

  //Menu Pop up
 item = Gtk::manage(new Gtk::MenuItem("Couper", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Copier", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Coller", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Renommer", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Supprimer", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ouvrir un terminal ici", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &ExampleWindow::on_popup_terminal) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ajouter/supprimer des favoris", true));
 _menuPopup.append(*item);

 //_menuPopup.accelerate(*this);
  _menuPopup.show_all();
  m_TreeView.signal_button_press_event().connect_notify(sigc::mem_fun(*this, &ExampleWindow::on_button_press), false);
  this->m_VBox.show_all_children();
}

void ExampleWindow::on_popup_terminal()
{
  this->f->openXterm();
}

ExampleWindow::~ExampleWindow()
{
  delete this->f;
}

Gtk::Box* ExampleWindow::getVbox(){

  return &this->m_VBox;
}
