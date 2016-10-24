#ifndef TREEVIEWLIB_HPP
#define TREEVIEWLIB_HPP

#include <gtkmm.h>
#include <string>

using namespace std;

namespace khanar{

  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    {add(m_col_ico); add(m_col_name); add(m_col_number); };

    Gtk::TreeModelColumn<Glib::ustring> m_col_ico;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<short> m_col_number;
  };


class TreeViewLib
{
public:
  TreeViewLib(string path);
  Gtk::Widget* getVBox();

protected:
  //Signal handlers:
  void on_button_quit();

  //Tree model columns:


  ModelColumns m_Columns;

  //Child widgets:
  Gtk::Box m_VBox;


  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  Gtk::ButtonBox m_ButtonBox;
  Gtk::Button m_Button_Quit;
};


}

#endif
