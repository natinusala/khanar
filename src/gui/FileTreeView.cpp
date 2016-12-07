#include <iostream>
#include "FileTreeView.hpp"
#include "../util/File.hpp"

using namespace khanar;


void FileTreeView::on_button_press_actu(const Gtk::TreeModel::Path& path,Gtk::TreeViewColumn* c)
{
      Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
      int id = (*iter)[Columns.col_id];
      this->wind->updatePropBar(&subFiles->at(id));
}


void FileTreeView::on_button_press(GdkEventButton* button_event)
{
  if((button_event->type == 4) && (button_event->button == 3))
  {
    if (!recents)
      menuPopup.popup(button_event->button, button_event->time);
  }
  else if
  ((button_event->type == 5) && (button_event->button == 1))
  {
    Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
    int id = (*iter)[Columns.col_id];
    if (subFiles->at(id).isDirectory()){
        this->wind->actualiser(subFiles->at(id).getAbsolutePath());
    }else{
      subFiles->at(id).openFile();

    }
  }

  }


void FileTreeView::on_sort_ascending()
{
  this->f->setSortStrategy(this->f->getSortStrategy(), false);
}

void FileTreeView::on_sort_descending()
{
  this->f->setSortStrategy(this->f->getSortStrategy(), true);
}

void FileTreeView::on_sort_name()
{
  this->f->setSortStrategy(File::NAME_FILESORTSTRATEGY, this->f->isSortStrategyDescending());
}

void FileTreeView::on_sort_size()
{
  this->f->setSortStrategy(File::SIZE_FILESORTSTRATEGY, this->f->isSortStrategyDescending());
}

void FileTreeView::on_sort_date()
{
  this->f->setSortStrategy(File::ACCESSTIME_FILESORTSTRATEGY, this->f->isSortStrategyDescending());
}

void FileTreeView::on_paste()
{
  File toPaste = this->wind->getClipboard();
  File toPasteParent = this->wind->getClipboardParent();
  bool shouldDeleteClipboard = this->wind->getShouldDeleteClipboard();

  if (shouldDeleteClipboard)
  {
    if (this->f->getAbsolutePath() == toPasteParent.getAbsolutePath())
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->parentWindow, "Erreur");
      dialog.set_secondary_text("Dossier source et de destination identiques");
      dialog.run();
    }
    else
    {
      toPaste.move(this->f->getAbsolutePath() + "/" + toPaste.getName(), this->f);
    }
  }
  else
  {
    if (this->f->getAbsolutePath() == toPasteParent.getAbsolutePath())
    {
      toPaste.copy(this->f->getAbsolutePath() + "/" + toPaste.getName() + " (copie)", this->f);
    }
    else
    {
      toPaste.copy(this->f->getAbsolutePath() + "/" + toPaste.getName(), this->f);
    }

    this->wind->updateClipboard(File(), File(), false);
  }
}

void FileTreeView::on_copy()
{
  Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toCopy = this->subFiles->at(id);

  this->wind->updateClipboard(toCopy, *this->f, false);
}

void FileTreeView::on_cut()
{
  Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toCut = this->subFiles->at(id);

  this->wind->updateClipboard(toCut, *this->f, true);
}

void FileTreeView::on_create_directory()
{
  Gtk::Dialog dialog = Dialog("Créer un dossier", *this->parentWindow, true);

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
    File newFile = File(this->f, newFileName);
    if (!newFile.exists() || (newFile.exists() && !newFile.isDirectory()))
    {
      newFile.createNewDirectory(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH, this->f);
      //this->wind->actualiser(); //TODO Observateur
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->parentWindow, "Erreur");
      dialog.set_secondary_text("Le dossier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::on_terminal()
{
  this->f->openXterm();
}

void FileTreeView::on_delete_file()
{
  Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toDelete = this->subFiles->at(id);

  Gtk::MessageDialog dialog = MessageDialog(*this->parentWindow, "Etes-vous sûr ?");
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
  Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
  int id = (*iter)[Columns.col_id];

  File toRename = this->subFiles->at(id);

  Gtk::Dialog dialog = Dialog("Renommer", *this->parentWindow, true);

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
    File newFile = File(this->f, newFileName);
    if (!newFile.exists())
    {
      toRename.setName(newFileName);
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->parentWindow, "Erreur");
      dialog.set_secondary_text("Le fichier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::on_create_file()
{
  Gtk::Dialog dialog = Dialog("Créer un fichier", *this->parentWindow, true);

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
    File newFile = File(this->f, newFileName);
    if (!newFile.exists())
    {
      newFile.createNewFile(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH, this->f);
    }
    else
    {
      Gtk::MessageDialog dialog = MessageDialog(*this->parentWindow, "Erreur");
      dialog.set_secondary_text("Le fichier existe déjà.");
      dialog.run();
    }
  }
}

void FileTreeView::reset(){

  if (this->subFiles != nullptr)
  {
    for (int i = 0; i < this->subFiles->size(); i++)
    {
      this->subFiles->at(i).unsubscribeObserver(&fileObs);
    }
  }
        refTreeModel->clear();
        if (recents){
          subFiles = this->f->getRecentFiles();
        } else
        {
          subFiles = this->f->getSubFiles(true);
        }



        for (int i = 0; i < this->subFiles->size(); i++)
        {
          this->subFiles->at(i).subscribeObserver(&fileObs);
        }

      Gtk::TreeModel::Row row;
      for (int i = 0; i < subFiles->size(); i++)
      {
        File f = subFiles->at(i);
        if(!f.isHidden()){
          row = *(refTreeModel->append());
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
  if (this->f != nullptr)
    this->f->unsubscribeObserver(&fileObs);

  if (this->subFiles != nullptr)
  {
    for (int i = 0; i < this->subFiles->size(); i++)
    {
      this->subFiles->at(i).unsubscribeObserver(&fileObs);
    }
  }
        refTreeModel->clear();
        FileSortStrategy oldStrategy = this->f->getSortStrategy();
        bool oldOrder = this->f->isSortStrategyDescending();

        this->f = new File(filepath);

        this->f->subscribeObserver(&fileObs);

        this->f->setSortStrategy(oldStrategy, oldOrder);

        subFiles = this->f->getSubFiles(true);

        for (int i = 0; i < this->subFiles->size(); i++)
          {
            this->subFiles->at(i).subscribeObserver(&fileObs);
          }



      Gtk::TreeModel::Row row;
      for (int i = 0; i < subFiles->size(); i++)
      {
        File f = (*subFiles)[i];
        if(!f.isHidden()){
          row = *(refTreeModel->append());
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


  this->parentWindow = win;
  this->wind = wind;
  this->recents = recents;

  scrolledWindow.add(treeView);

  scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  this->VBox.set_size_request(250,250);
  this->VBox.set_hexpand(true);
  this->VBox.set_vexpand(true);
  this->VBox.pack_start(scrolledWindow);

  refTreeModel = Gtk::ListStore::create(Columns);
  treeView.set_model(refTreeModel);

  this->f = new File(path);
  f->subscribeObserver(&fileObs);

  if (recents){
    subFiles = File::getRecentFiles();
  }
  else {
    subFiles = this->f->getSubFiles();
  }


  Gtk::TreeModel::Row row;
  for (int i = 0; i < subFiles->size(); i++)
  {
    File f = subFiles->at(i);
    f.subscribeObserver(&fileObs);
    if(!f.isHidden()){
      row = *(refTreeModel->append());
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
  treeView.append_column("   ", *cell);
  auto pColumn = treeView.get_column(0);
  if(pColumn)
  {
    Glib::ustring str = "icon_name";
    pColumn->add_attribute(*cell,str,Columns.col_ico);
  }
  treeView.append_column("Nom", Columns.col_name);
  treeView.append_column("Taille", Columns.col_number);

  for(guint i = 0; i < 2; i++)
  {
    auto column = treeView.get_column(i);
    column->set_reorderable();
  }


  auto item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau fichier", true));
  item->signal_activate().connect_notify(
     sigc::mem_fun(*this, &FileTreeView::on_create_file) );
  menuPopup.append(*item);

  item = Gtk::manage(new Gtk::MenuItem("Créer un nouveau dossier", true));
  item->signal_activate().connect_notify(
     sigc::mem_fun(*this, &FileTreeView::on_create_directory) );
  menuPopup.append(*item);

  item = Gtk::manage(new Gtk::SeparatorMenuItem());
  menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Couper", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_cut) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Copier", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_copy) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Coller ici", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_paste) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Renommer", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_rename) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Supprimer", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_delete_file) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ouvrir un terminal ici", true));
 item->signal_activate().connect_notify(
    sigc::mem_fun(*this, &FileTreeView::on_terminal) );
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Ajouter/supprimer des favoris", true));
 menuPopup.append(*item);

 item = Gtk::manage(new Gtk::SeparatorMenuItem());
 menuPopup.append(*item);

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
 menuPopup.append(*item);

  menuPopup.show_all();
  treeView.signal_button_press_event().connect_notify(sigc::mem_fun(*this, &FileTreeView::on_button_press), false);
  treeView.set_activate_on_single_click(true);
  treeView.signal_row_activated().connect_notify(sigc::mem_fun(*this, &FileTreeView::on_button_press_actu));

  this->VBox.show_all_children();
}

FileTreeView::~FileTreeView()
{
  delete this->f;
}

Gtk::Box* FileTreeView::getVbox(){

  return &this->VBox;
}

void OngletFileObserver::fileUpdated(khanar::File *file)
{
  _fileTreeView->reset();
}

string FileTreeView::getPath(){
  return this->f->getAbsolutePath();
}
