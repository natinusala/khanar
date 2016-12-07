#include <iostream>
#include "FileTreeView.hpp"
#include "../util/File.hpp"

using namespace khanar;


//TODO Couper/copier avec this->clipboard et this->shouldDeleteClipboard


void FileTreeView::on_button_press_actu(const Gtk::TreeModel::Path& path,Gtk::TreeViewColumn* c)
{
      Gtk::TreeModel::iterator iter = treeView.get_selection()->get_selected();
      int id = (*iter)[Columns.col_id];
      if (! subFiles->at(id).isDirectory()){
          this->wind->updatePropBar(subFiles->at(id));
      }

}


void FileTreeView::on_button_press(GdkEventButton* button_event)
{
  if((button_event->type == 4) && (button_event->button == 3))
  {
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
  //TODO Mise à jour (l'observateur sur f s'en charge)
}

void FileTreeView::on_sort_descending()
{
  this->f->setSortStrategy(this->f->getSortStrategy(), true);
  //TODO Mise à jour (l'observateur sur f s'en charge)
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
      toPaste.move(this->f->getAbsolutePath() + "/" + toPaste.getName());
    }
  }
  else
  {
    if (this->f->getAbsolutePath() == toPasteParent.getAbsolutePath())
    {
      toPaste.copy(this->f->getAbsolutePath() + "/" + toPaste.getName() + " (copie)");
    }
    else
    {
      toPaste.copy(this->f->getAbsolutePath() + "/" + toPaste.getName());
    }

    //TODO Mettre à jour (l'observateur le fait)

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
      newFile.createNewDirectory(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      this->wind->actualiser();
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
    this->wind->actualiser(); // Test : Dégage si observeur fonctionne
    //TODO Mettre à jour (l'observateur s'en charge normalement)
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
      this->wind->actualiser();//test: degage si onbserveur fonctionne
      //TODO Mettre à jour (l'observateur le fait normalement)
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
      newFile.createNewFile(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      this->wind->actualiser();//test : degage si observeur fonctionne
      //TODO Mettre à jour (ici l'observateur ne marchera pas)
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
        refTreeModel->clear();
        subFiles = this->f->getSubFiles();


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

void FileTreeView::reset(string filepath){
        refTreeModel->clear();
        this->f = new File(filepath);
        subFiles = this->f->getSubFiles();


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



FileTreeView::FileTreeView(Gtk::Window*& win,khanar::Window* wind, string path)
{


  this->parentWindow = win;
  this->wind = wind;

  scrolledWindow.add(treeView);

  scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  this->VBox.set_size_request(250,250);
  this->VBox.set_hexpand(true);
  this->VBox.set_vexpand(true);
  this->VBox.pack_start(scrolledWindow);

  refTreeModel = Gtk::ListStore::create(Columns);
  treeView.set_model(refTreeModel);

  this->f = new File(path);

  if (path ==""){
    subFiles = this->f->getRecentFiles();
  }
  else {
    subFiles = this->f->getSubFiles();
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
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Par taille", true));
 sortMenu->append(*item);

 item = Gtk::manage(new Gtk::MenuItem("Par date de dernière modification", true));
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
