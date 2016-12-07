#include "PropertiesBar.hpp"

using namespace khanar;

PropertiesBar::PropertiesBar(Glib::RefPtr<Gtk::Builder> builder) : _builder(builder)
{
  if (builder)
    builder->get_widget("proprietes", _prop);

  this->content = this;
}

void PropertiesBar::updateProp(khanar::File* newFile)
{
      if (this->_f != nullptr)
      {
        this->_f->unsubscribeObserver(&_fileObs);
      }

      this->_f = newFile;

      this->_f->subscribeObserver(&_fileObs);

      Gtk::Label* nom = nullptr;
      Gtk::Label* type = nullptr;
      Gtk::Label* taill = nullptr;
      Gtk::Label* acces = nullptr;
      Gtk::Label* modifie = nullptr;
      Gtk::Label* nomProprio = nullptr;
      Gtk::ComboBoxText* box = nullptr;
      Gtk::Image* icon = nullptr;

      this->setVisible(true);

      this->_builder->get_widget("nom", nom);
      this->_builder->get_widget("type", type);
      this->_builder->get_widget("taill", taill);
      this->_builder->get_widget("acces", acces);
      this->_builder->get_widget("modifie", modifie);
      this->_builder->get_widget("nomProprietaire", nomProprio);
      this->_builder->get_widget("groupecombo", box);
      this->_builder->get_widget("Icone", icon);

      nom->set_text(_f->getName());
      type->set_text(_f->getFileType().getName());
      taill->set_text(_f->getFormattedSize());
      char buffer [50];
      time_t rawtime = (time_t) _f->getLastAccessTime();
      struct tm * timeinfo;
      timeinfo = localtime (&rawtime);
      strftime (buffer,20,"%d/%m/%y",timeinfo);
      char acc [50]= "Accédé : ";
      char result[100];
      strcpy(result,acc);
      strcat(result,buffer);
      acces->set_text(result);
      buffer [50];
      rawtime = (time_t) _f->getLastModificationTime();
      timeinfo = localtime (&rawtime);
      strftime (buffer,20,"%d/%m/%y",timeinfo);
      char acc2 [50]= "Modifié : ";
      result[100];
      strcpy(result,acc2);
      strcat(result,buffer);
      modifie->set_text(result);

      icon->set_from_icon_name(_f->getFileType().getIcon(), Gtk::BuiltinIconSize::ICON_SIZE_DIALOG);

      nomProprio->set_text(File::getUIDName(_f->getUID()));
}

void PropertiesBar::setVisible(bool show)
{
  if (show)
    _prop->show();
  else
    _prop->hide();
}

void PropertiesFileObserver::fileUpdated(File* file)
{
  _propBar->updateProp(file);
}
