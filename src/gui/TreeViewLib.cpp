#include <iostream>
#include "TreeViewLib.hpp"
#include <gtkmm.h>
#include "../util/File.hpp"
#include "../compiled_assets/vboxlayout.glade.hex"
#include "../util/Assets.hpp"

using namespace Gtk;
using namespace Glib;
using namespace std;

namespace khanar{
TreeViewLib::TreeViewLib(Glib::ustring path)
{
 add(m_VBox);
  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
 m_ScrolledWindow.add(m_TreeView);

 //Only show the scrollbars when they are necessary:
 m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

 m_VBox.pack_start(m_ScrolledWindow);

 //Create the Tree model:
 m_refTreeModel = Gtk::ListStore::create(m_Columns);
 m_TreeView.set_model(m_refTreeModel);

 //Add the TreeView's view columns:
 //This number will be shown with the default numeric formatting.
 khanar::File f = File(path);
 vector<File> *subFiles = f.getSubFiles();

 Gtk::TreeModel::Row row;
 for (int i = 0; i < subFiles->size(); i++)
 {
   File f = (*subFiles)[i];
   row = *(m_refTreeModel->append());
   cout << f.getFileType().getIcon() <<endl;
   cout << f.getName()<<endl;
   row[m_Columns.m_col_ico]= f.getFileType().getIcon();
   row[m_Columns.m_col_name] = f.getName();
   row[m_Columns.m_col_number] = f.getSize();

 }

 auto cell = Gtk::manage(new Gtk::CellRendererPixbuf);
 m_TreeView.append_column("Ico", *cell);
 auto pColumn = m_TreeView.get_column(0);
 if(pColumn)
 {
   Glib::ustring str = "icon_name";
   pColumn->add_attribute(*cell,str,m_Columns.m_col_ico);
 }
 m_TreeView.append_column("Name", m_Columns.m_col_name);
 m_TreeView.append_column_numeric("Size", m_Columns.m_col_number,"%010d");

 for(guint i = 0; i < 2; i++)
 {
   auto column = m_TreeView.get_column(i);
   column->set_reorderable();
 }
}

}
