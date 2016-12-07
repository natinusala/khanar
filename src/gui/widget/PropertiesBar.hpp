#ifndef PROPERTIESBAR_HPP
#define PROPERTIESBAR_HPP

#include <gtkmm.h>
#include "../../util/File.hpp"
#include "WidgetPerso.hpp"

/**
 * \namespace khanar
 *
 * \brief Namespace du projet
 */
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

/**
* \class PropertiesBar
*
* Encapsulation des éléments d'interface du panneau des propriétés
* du bas de la fenêtre
*/
class PropertiesBar : public GladeWidget
{
    private:
      File* _f = nullptr;
      Glib::RefPtr<Gtk::Builder> _builder;

      PropertiesFileObserver _fileObs = PropertiesFileObserver(this);

      Gtk::Box* _prop = nullptr;

    public:
      /**
      * \brief Constructeur
      * \param[in] builder Le Gtk::Builder qui contient les éléments
      * de l'interface à construire
      */
      PropertiesBar(Glib::RefPtr<Gtk::Builder> builder);

      void updateProp(File* newFile); ///< Mets à jour le panneau avec les données de ce fichier
      void setVisible(bool show); ///< Affiche ou cache le panneau
    };
}
#endif
