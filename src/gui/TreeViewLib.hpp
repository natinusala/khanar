#ifndef TREEVIEWLIB_HPP
#define TREEVIEWLIB_HPP

#include <gtkmm.h>
#include <string>

using namespace std;

namespace khanar{

class TreeViewLib : public Gtk::Window
{
public:
  TreeViewLib(Glib::ustring path);

protected:
  //Signal handlers:
  void on_button_quit();
  bool on_button_press(GdkEventButton* button_event);

  Gtk::Menu _menuPopup;

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_ico); add(m_col_name); add(m_col_number); }

    Gtk::TreeModelColumn<Glib::ustring> m_col_ico;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<short> m_col_number;
  };

  ModelColumns m_Columns;

  //Child widgets:
  Gtk::VBox m_VBox;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

}

#endif
