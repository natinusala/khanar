#ifndef WIDGETPERSO_HPP
#define WIDGETPERSO_HPP

#include <gtkmm.h>

namespace khanar
{
  /**
  * \class WidgetPerso
  *
  * Abstraction d'un widget
  * Peut être une classe maison ou un widget Gtk
  */
  class WidgetPerso
  {
  protected:
    void* content; ///< Contenu du WidgetPerso (peut être une classe maison ou un widget Gtk)

  public:
    void* getContent(); ///< Renvoie le contenu du widget
  };

  class GladeWidget : public WidgetPerso
  {

  };

  class ContentWidget : public WidgetPerso
  {

  };
}

#endif
