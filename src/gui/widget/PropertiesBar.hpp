#ifndef PROPERTIESBAR_HPP
#define PROPERTIESBAR_HPP

#include <gtkmm.h>
#include "../../util/File.hpp"
#include "WidgetPerso.hpp"

 namespace khanar{

   class PropertiesBar;

class PropertiesFileObserver : public FileObserver
{
private:
  PropertiesBar* _propBar;
public:
  PropertiesFileObserver(PropertiesBar* propBar) : _propBar(propBar){}
  void fileUpdated(File* file) override;
};

class PropertiesBar : public GladeWidget
{

    private:
      File* _f = nullptr;
      Glib::RefPtr<Gtk::Builder> _builder;

      PropertiesFileObserver _fileObs = PropertiesFileObserver(this);

      Gtk::Box* _prop = nullptr;

    public:

      PropertiesBar(Glib::RefPtr<Gtk::Builder> builder);
      void updateProp(File* newFile);
      void setVisible(bool show);


    };
}
#endif
