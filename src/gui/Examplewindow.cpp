#include <iostream>
#include "Examplewindow.hpp"
#include "../util/File.hpp"


//TODO: Ajouter un attribut vector pour retenir les fichiers contenu dans l'interface.

ExampleWindow::ExampleWindow(){
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

  khanar::File f = khanar::File("/home/");
  vector<khanar::File> *subFiles = f.getSubFiles();

  Gtk::TreeModel::Row row;
  for (int i = 0; i < subFiles->size(); i++)
  {
    khanar::File f = (*subFiles)[i];
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

  this->m_VBox.show_all_children();

}

ExampleWindow::ExampleWindow(Glib::ustring path)
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

  khanar::File f = khanar::File(path);
  vector<khanar::File> *subFiles = f.getSubFiles();

  Gtk::TreeModel::Row row;
  for (int i = 0; i < subFiles->size(); i++)
  {
    khanar::File f = (*subFiles)[i];
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

  this->m_VBox.show_all_children();
}

Gtk::Box* ExampleWindow::getVbox(){

  return &this->m_VBox;
}
