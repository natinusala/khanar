#include <iostream>
#include "TreeViewLib.hpp"
#include "../util/File.hpp"

namespace khanar{
TreeViewLib::TreeViewLib(string path)
{
  m_ScrolledWindow.add(m_TreeView);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_VBox.pack_start(m_ScrolledWindow);
  //m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

  //Create the Tree model:
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);


  khanar::File f = File(path);
  vector<File> *subFiles = f.getSubFiles();
  for (int i = 0; i < subFiles->size(); i++)
  {
    File f = (*subFiles)[i];
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_ico]= f.getFileType().getIcon();
    row[m_Columns.m_col_name] = f.getName();
    row[m_Columns.m_col_number] = f.getSize();
  }
  //Add the TreeView's view columns:
  //This number will be shown with the default numeric formatting.
  auto cell = Gtk::manage(new Gtk::CellRendererPixbuf);
  int cols_count = m_TreeView.append_column("Ico", *cell);
  auto pColumn = m_TreeView.get_column(cols_count - 1);
  if(pColumn)
  {
    Glib::ustring str = "icon_name";
    pColumn->add_attribute(*cell,str,m_Columns.m_col_ico);
  }
  m_TreeView.append_column("Name", m_Columns.m_col_name);
  m_TreeView.append_column_numeric("Formatted number", m_Columns.m_col_number,
          "%010d" /* 10 digits, using leading zeroes. */);

  //Display a progress bar instead of a decimal number:


  //Make all the columns reorderable:
  //This is not necessary, but it's nice to show the feature.
  //You can use TreeView::set_column_drag_function() to more
  //finely control column drag and drop.
  for(guint i = 0; i < 2; i++)
  {
    auto column = m_TreeView.get_column(i);
    column->set_reorderable();
  }

  //show_all_children();
}


Gtk::Widget* TreeViewLib::getVBox(){

  return &this->m_VBox;
}



}
