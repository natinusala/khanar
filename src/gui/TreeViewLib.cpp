#include <iostream>
#include "TreeViewLib.hpp"
#include "../util/File.hpp"

namespace khanar{
TreeViewLib::TreeViewLib(string path)
{
  m_ScrolledWindow.add(m_TreeView);
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_VBox.pack_start(m_ScrolledWindow);
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);


  khanar::File f = File(path);
  vector<File> *subFiles = f.getSubFiles();

Gtk::TreeModel::Row row;
  for (int i = 0; i < subFiles->size(); i++)
  {
    File f = (*subFiles)[i];
    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_ico]= f.getFileType().getIcon();
    row[m_Columns.m_col_name] = f.getName();
    row[m_Columns.m_col_number] = f.getSize();
  }

  auto cell = Gtk::manage(new Gtk::CellRendererPixbuf);
  int cols_count = m_TreeView.append_column("Ico", *cell);
  auto pColumn = m_TreeView.get_column(cols_count - 1);
  if(pColumn)
  {
    Glib::ustring str = "icon_name";
    pColumn->add_attribute(*cell,str,m_Columns.m_col_ico);
  }
  m_TreeView.append_column("Name", m_Columns.m_col_name);
  m_TreeView.append_column_numeric("Size", m_Columns.m_col_number,"%010d");

  for(guint i = 0; i < 3; i++)
  {
    auto column = m_TreeView.get_column(i);
    column->set_reorderable();
  }

  this->m_VBox.show_all();
}


Gtk::Widget* TreeViewLib::getVBox(){

  return &this->m_VBox;
}



}
