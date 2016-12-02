#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "../util/File.hpp"
#include "Window.hpp"

using namespace khanar;

class ExampleWindow
{
public:
  ExampleWindow(Gtk::Window* win, string path);
  ~ExampleWindow();
  Gtk::Box* getVbox();
protected:
  void on_button_quit();
  void on_button_press(GdkEventButton* button_event);

  void on_terminal();
  void on_create_file();
  void on_delete_file();
  void on_create_directory();

  Gtk::Window* parentWindow;

  Gtk::Menu menuPopup;

  File* f;
  vector<File>* subFiles;

  File* clipboard;
  File* clipboardParent;
  bool shouldDeleteClipboard;


  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_ico); add(m_col_name); add(m_col_number); }

    Gtk::TreeModelColumn<Glib::ustring> m_col_ico;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_number;
  };

  ModelColumns m_Columns;

  //Child widgets:
  Gtk::Box m_VBox;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

#endif //GTKMM_EXAMPLEWINDOW_H
