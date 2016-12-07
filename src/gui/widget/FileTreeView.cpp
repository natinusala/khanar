#include <iostream>
#include "FileTreeView.hpp"
#include "../../util/File.hpp"

using namespace khanar;


void FileTreeView::on_button_press_actu(const Gtk::TreeModel::Path& path,Gtk::TreeViewColumn* c)
{
      Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
      int id = (*iter)[Columns.col_id];
      this->_wind->updatePropBar(&_subFiles->at(id));
}


void FileTreeView::on_button_press(GdkEventButton* button_event)
{
  if((button_event->type == 4) && (button_event->button == 3))
  {
    if (!_recents)
      _menuPopup.popup(button_event->button, button_event->time);
  }
  else if
  ((button_event->type == 5) && (button_event->button == 1))
  {
    Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
    int id = (*iter)[Columns.col_id];
    if (_subFiles->at(id).isDirectory()){
        this->_wind->actualiser(_subFiles->at(id).getAbsolutePath());
    }else{
      _subFiles->at(id).openFile();

    }
  }

  }


void FileTreeView::on_sort_ascending()
{
  this->_f->setSortStrategy(this->_f->getSortStrategy(), false);
}

void FileTreeView::on_sort_descending()
{
  this->_f->setSortStrategy(this->_f->getSortStrategy(), true);
}

void FileTreeView::on_sort_name()
{
  this->_f->setSortStrategy(File::NAME_FILESORTSTRATEGY, this->_f->isSortStrategyDescending());
}

void FileTreeView::on_sort_size()
{
  this->_f->setSortStrategy(File::SIZE_FILESORTSTRATEGY, this->_f->isSortStrategyDescending());
}

void FileTreeView::on_sort_date()
{
  this->_f->setSortStrategy(File::ACCESSTIME_FILESORTSTRATEGY, this->_f->isSortStrategyDescending());
}

void FileTreeView::on_paste()
{
  File toPaste = this->_wind->getClipboard();
  File toPasteParent = this->_wind->getClipboardParent();
  bool shouldDeleteClipboard = this->_wind->getShouldDeleteClipboard();

  if (shouldDeleteClipboard)
  {
    if (this->_f->getAbsolutePath() == toPasteParent.getAbsolutePath())
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->_parentWindow, "Erreur");
      dialog.set_secondary_text("Dossier source et de destination identiques");
      dialog.run();
    }
    else
    {
      toPaste.move(this->_f->getAbsolutePath() + "/" + toPaste.getName(), this->_f);
    }
  }
  else
  {
    if (this->_f->getAbsolutePath() == toPasteParent.getAbsolutePath())
    {
      toPaste.copy(this->_f->getAbsolutePath() + "/" + toPaste.getName() + " (copie)", this->_f);
    }
    else
    {
      toPaste.copy(this->_f->getAbsolutePath() + "/" + toPaste.getName(), this->_f);
    }

    this->_wind->updateClipboard(File(), File(), false);
  }
}

void FileTreeView::on_copy()
{
  Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toCopy = this->_subFiles->at(id);

  this->_wind->updateClipboard(toCopy, *this->_f, false);
}

void FileTreeView::on_cut()
{
  Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toCut = this->_subFiles->at(id);

  this->_wind->updateClipboard(toCut, *this->_f, true);
}

void FileTreeView::on_create_directory()
{
  Gtk::Dialog dialog = Dialog("Créer un dossier", *this->_parentWindow, true);

  dialog.add_button("Créer", RESPONSE_OK);
  dialog.add_button("Annuler", RESPONSE_CANCEL);

  Gtk::Label label = Label("Entrez le nom du nouveau dossier :");
  dialog.get_content_area()->pack_start(label);

  Gtk::TextView text = TextView();
  dialog.get_content_area()->pack_start(text);

  dialog.show_all_children();

  int result = dialog.run();

  if (result == RESPONSE_OK)
  {
    //Création du nouveau fichier
    string newFileName = text.get_buffer()->get_text();
    File newFile = File(this->_f, newFileName);
    if (!newFile.exists() || (newFile.exists() && !newFile.isDirectory()))
    {
      newFile.createNewDirectory(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH, this->_f);
      //this->wind->actualiser(); //TODO Observateur
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->_parentWindow, "Erreur");
      dialog.set_secondary_text("Le dossier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::on_terminal()
{
  this->_f->openXterm();
}

void FileTreeView::on_delete_file()
{
  Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toDelete = this->_subFiles->at(id);

  Gtk::MessageDialog dialog = MessageDialog(*this->_parentWindow, "Etes-vous sûr ?");
  dialog.set_secondary_text("Etes-vous sûr de vouloir supprimer " + toDelete.getName() + (toDelete.isDirectory() ? " et tout ce qu'il contient" : "") + " ?");

  dialog.add_button("Non", RESPONSE_NO);
  int result = dialog.run();

  if (result == RESPONSE_OK)
  {
    toDelete.removeFile();
  }
}

void FileTreeView::on_rename()
{
  Gtk::TreeModel::iterator iter = _treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toRename = this->_subFiles->at(id);

  Gtk::Dialog dialog = Dialog("Renommer", *this->_parentWindow, true);

  dialog.add_button("Renommer", RESPONSE_OK);
  dialog.add_button("Annuler", RESPONSE_CANCEL);

  Gtk::Label label = Label("Entrez le nouveau nom du fichier :");
  dialog.get_content_area()->pack_start(label);

  Gtk::TextView text = TextView();
  dialog.get_content_area()->pack_start(text);
  text.get_buffer()->set_text(toRename.getName());

  dialog.show_all_children();

  int result = dialog.run();

  if (result == RESPONSE_OK)
  {
    //Vérification que le nouveau fichier n'existe pas déjà
    string newFileName = text.get_buffer()->get_text();
    File newFile = File(this->_f, newFileName);
    if (!newFile.exists())
    {
      toRename.setName(newFileName);
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->_parentWindow, "Erreur");
      dialog.set_secondary_text("Le fichier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::on_create_file()
{
  Gtk::Dialog dialog = Dialog("Créer un fichier", *this->_parentWindow, true);

  dialog.add_button("Créer", RESPONSE_OK);
  dialog.add_button("Annuler", RESPONSE_CANCEL);

  Gtk::Label label = Label("Entrez le nom du nouveau fichier :");
  dialog.get_content_area()->pack_start(label);

  Gtk::TextView text = TextView();
  dialog.get_content_area()->pack_start(text);

  dialog.show_all_children();

  int result = dialog.run();

  if (result == RESPONSE_OK)
  {
    //Création du nouveau fichier
    string newFileName = text.get_buffer()->get_text();
    File newFile = File(this->_f, newFileName);
    if (!newFile.exists())
    {
      newFile.createNewFile(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH, this->_f);
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->_parentWindow, "Erreur");
      dialog.set_secondary_text("Le fichier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::reset(){

  if (this->_subFiles != nullptr)
  {
    for (int i = 0; i < this->_subFiles->size(); i++)
    {
      this->_subFiles->at(i).unsubscribeObserver(&_fileObs);
    }
  }
        _refTreeModel->clear();
        if (_recents){
          _subFiles = this->_f->getRecentFiles();
        } else
        {
          _subFiles = this->_f->getSubFiles(true);
        }



        for (int i = 0; i < this->_subFiles->size(); i++)
        {
          this->_subFiles->at(i).subscribeObserver(&_fileObs);
        }

      Gtk::TreeModel::Row row;
      for (int i = 0; i < _subFiles->size(); i++)
      {
        File f = _subFiles->at(i);
        if(!f.isHidden()){
          row = *(_refTreeModel->append());
          row[Columns.col_id]=i;
          row[Columns.col_ico]= f.getFileType().getIcon();
          row[Columns.col_name] = f.getName();
          string size =f.getFormattedSize();
          if (size =="n/a")
          {
            row[Columns.col_number] = " ";
          }
          else
          {
            row[Columns.col_number] = size;
          }
        }
      }


}

void FileTreeView::reset(string filepath){
  if (this->_f != nullptr)
    this->_f->unsubscribeObserver(&_fileObs);

  if (this->_subFiles != nullptr)
  {
    for (int i = 0; i < this->_subFiles->size(); i++)
    {
      this->_subFiles->at(i).unsubscribeObserver(&_fileObs);
    }
  }
        _refTreeModel->clear();
        FileSortStrategy oldStrategy = this->_f->getSortStrategy();
        bool oldOrder = this->_f->isSortStrategyDescending();

        this->_f = new File(filepath);
        _recents = false;

        this->_f->subscribeObserver(&_fileObs);

        this->_f->setSortStrategy(oldStrategy, oldOrder);

        _subFiles = this->_f->getSubFiles(true);

        for (int i = 0; i < this->_subFiles->size(); i++)
          {
            this->_subFiles->at(i).subscribeObserver(&_fileObs);
          }



      Gtk::TreeModel::Row row;
      for (int i = 0; i < _subFiles->size(); i++)
      {
        File f = (*_subFiles)[i];
        if(!f.isHidden()){
          row = *(_refTreeModel->append());
          row[Columns.col_id]=i;
          row[Columns.col_ico]= f.getFileType().getIcon();
          row[Columns.col_name] = f.getName();
          string size =f.getFormattedSize();
          if (size =="n/a"){
            row[Columns.col_number] = " ";
          }else{
            row[Columns.col_number] = size;
          }
        }
      }


}

void FileTreeView::search(string search){
        _refTreeModel->clear();
        if (_subFilesSearch != nullptr)
        {
          delete _subFilesSearch;
          _subFilesSearch = nullptr;
        }

        _subFilesSearch = this->_f->search(search);

        if (_subFilesSearch == nullptr)
          return;

      Gtk::TreeModel::Row row;
      for (int i = 0; i < _subFilesSearch->size(); i++)
      {
        File f = _subFilesSearch->at(i);
        if(!f.isHidden()){
          row = *(_refTreeModel->append());
          row[Columns.col_id]=i;
          row[Columns.col_ico]= f.getFileType().getIcon();
          row[Columns.col_name] = f.getName();
          string size =f.getFormattedSize();
          if (size =="n/a"){
            row[Columns.col_number] = " ";
          }else{
            row[Columns.col_number] = size;
          }
        }
      }


}



FileTreeView::FileTreeView(Gtk::Window*& win,khanar::Window* wind, string path, bool recents)
{
  this->content = this;

  this->_parentWindow = win;
  this->_wind = wind;
  this->_recents = recents;

  _scrolledWindow.add(_treeView);

  _scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  this->_VBox.set_size_request(250,250);
  this->_VBox.set_hexpand(true);
  this->_VBox.set_vexpand(true);
  this->_VBox.pack_start(_scrolledWindow);

  _refTreeModel = Gtk::ListStore::create(Columns);
  _treeView.set_model(_refTreeModel);

  this->_f = new File(path);
  _f->subscribeObserver(&_fileObs);

  if (_recents){
    _subFiles = File::getRecentFiles();
  }
  else {
    _subFiles = this->_f->getSubFiles();
  }


  Gtk::TreeModel::Row row;
  for (int i = 0; i < _subFiles->size(); i++)
  {
    File f = _subFiles->at(i);
    f.subscribeObserver(&_fileObs);
    if(!f.isHidden()){
      row = *(_refTreeModel->append());
      row[Columns.col_id]=i;
      row[Columns.col_ico]= f.getFileType().getIcon();
      row[Columns.col_name] = f.getName();
      string size =f.getFormattedSize();
      if (size =="n/a"){
        row[Columns.col_number] = " ";
      }else{
        row[Columns.col_number] = size;
      }
    }
  }

  auto cell = Gtk::manage(new Gtk::CellRendererPixbuf);
  _treeView.append_column("   ", *cell);
  auto pColumn = _treeView.get_column(0);
  if(pColumn)
  {
    Glib::ustring str = "icon_name";
    pColumn->add_attribute(*cell,str,Columns.col_ico);
  }
  _treeView.append_column("Nom", Columns.col_name);
  _treeView.append_column("Taille", Columns.col_number);

  for(guint i = 0; i < 2; i++)
  {
    auto column = _treeView.get_column(i);
    column->set_reorderable();
  }


  auto item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau fichier", true));
  item->signal_activate().connect_notify(
     sigc::mem_fun(*this, &FileTreeView::on_create_file) );
  _menuPopup.append(*item);

  item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau dossier", true));
  item->signal_activate().connect_notify(
     sigc::mem_fun(*this, &FileTreeView::on_create_directory) );
  _menuPopup.append(*item);

  item = Gtk::manage(new Gtk::SeparatorMenuItem());
  _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Couper", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_cut) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Copier", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_copy) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Coller ici", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_paste) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Renommer", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_rename) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Supprimer", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_delete_file) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
_menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ouvrir un terminal ici", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_terminal) );
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ajouter/supprimer des favoris", true));
 _menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 _menuPopup.append(*item);

 Gtk::Menu* sortMenu = Gtk::manage(new Gtk::Menu());

 item = Gtk::manage(new Gtk::MenuItem("Par nom", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_sort_name) );
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Par taille", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_sort_size) );
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Par date de dernière modification", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_sort_date) );
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ascendant", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_sort_ascending ));
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Descendant", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_sort_descending ));
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Trier", true));
 item->set_submenu(*sortMenu);
 _menuPopup.append(*item);

  _menuPopup.show_all();
  _treeView.signal_button_press_event().connect_notify(sigc::mem_fun(*this, &FileTreeView::on_button_press), false);
  _treeView.set_activate_on_single_click(true);
  _treeView.signal_row_activated().connect_notify(sigc::mem_fun(*this, &FileTreeView::on_button_press_actu));

  this->_VBox.show_all_children();
}

FileTreeView::~FileTreeView()
{
  delete this->_f;
}

Gtk::Box* FileTreeView::getVbox(){

  return &this->_VBox;
}

void OngletFileObserver::fileUpdated(khanar::File *file)
{
  _fileTreeView->reset();
}

string FileTreeView::getPath(){
  if(!_recents){
    return this->_f->getAbsolutePath();
  }
  return ("Fichiers récents");
}

string FileTreeView::getName(){
  if(!_recents){
    return this->_f->getName();
  }
  return ("Fichiers récents");
}
