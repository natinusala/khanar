#ifndef FILETREEVIEW_HPP
#define FILETREEVIEW_HPP

#include <gtkmm.h>
#include "../../util/File.hpp"
#include "../Window.hpp"
#include "WidgetPerso.hpp"

 namespace khanar{

   class FileTreeView;
   /**
   * \class OngletFileObserver
   *
   * Interface du pattern Observer pour observer la mise à jour des fichiers
   * Ici, le sujet est FileTreeView
   */
   class OngletFileObserver : public FileObserver
   {
   private:
     FileTreeView* _fileTreeView;

   public:
     OngletFileObserver(FileTreeView* fileTreeView) : _fileTreeView(fileTreeView)
     {}

     void fileUpdated(File* file) override;
   };

   /**
   * \class FileTreeView
   *
   * Implémentation concrète de ContentWidget. Correspond a une vue arborescente d'une directory.
   *
   */
class FileTreeView : public ContentWidget
{
public:
  /**
  * \brief Constructeur prenant une fenetre Gtk, une fenetre native, un chemin et un booleen en paramètres
  * \param[in] win la fenêtre Gtk à partir de laquelle on créé les menus
  * \param[in] wind la fenetre khanar a partir de laquelle on met à jour la vue
  * \param[in] path le chemin versle dossier à afficher
  * \param[in] recents Booléen permettant de savoir si il faut afficher le dossier courant ou les fichiers récents
  */
  FileTreeView(Gtk::Window*& win,khanar::Window* wind, string path, bool recents);
  ~FileTreeView();///Destructeur de la classe
  Gtk::Box* getVbox(); /// Méthode de récupération du widget Gtk principal du FileTreeView
  void reset(); ///Mise à jour de la vue
  void reset(string filepath); ///Mise à jour de la vue en changeant le sujet File
  void search(string search); ///Mise à jour de la vue en fonction d'un critère de recherche
  string getPath(); ///Retourne le chemin absolu du fichier courant
  string getName(); /// Retourne le nom du fichier courant
private:
  void on_button_quit(); ///Handler du clic sur quitter
  void on_button_press(GdkEventButton* button_event); /// Handler du clic sur le TreeView
  void on_button_press_actu(const Gtk::TreeModel::Path& path,Gtk::TreeViewColumn* c); /// Handler du clic sur le bouton d'actualisation
  void on_terminal(); ///Handler de l'ouverture d'un terminal
  void on_create_file(); /// Handler de la création d'un fichier
  void on_delete_file(); /// Handler de la supression d'un fichier
  void on_create_directory(); /// Handler de la création d'un dossier
  void on_rename(); /// Handler du changement de nom d'un fichier/dossier
  void on_cut(); /// Handler de coupe
  void on_copy(); /// Handler de copie
  void on_paste(); /// Handler de collage
  void on_sort_ascending(); /// Modifie le tri vers la manière ascendante
  void on_sort_descending(); /// Modifie le tri vers la manière descendante
  void on_sort_name(); /// Modifie le tri vers un tri nominal
  void on_sort_size(); /// Modifie le tri vers un tri en fonction de la taille
  void on_sort_date(); /// Modifie le tri vers un tri en fonction de la date

  OngletFileObserver _fileObs = OngletFileObserver(this);
  Gtk::Window* _parentWindow;
  khanar::Window* _wind;
  Gtk::Menu _menuPopup;
  File* _f;
  vector<File>* _subFiles;
  vector<File>* _subFilesSearch;
  bool _recents;


  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    {add(col_id); add(col_ico); add(col_name); add(col_number); }

    Gtk::TreeModelColumn<int> col_id;
    Gtk::TreeModelColumn<Glib::ustring> col_ico;
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<Glib::ustring> col_number;
  };

  ModelColumns Columns;

  //Child widgets:
  Gtk::Box _VBox;

  Gtk::ScrolledWindow _scrolledWindow;
  Gtk::TreeView _treeView;
  Glib::RefPtr<Gtk::ListStore> _refTreeModel;
};
}
#endif
