#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <gtkmm.h>

namespace khanar
{
  /**
  * \class Assets
  *
  * Classe pour manipuler les ressources (fichiers compilés en .h avec XXD)
  */
  class Assets
  {
    public:
       static Glib::RefPtr<Gtk::Builder> buildGtkFromResource(unsigned char resource[]); ///< Méthode pour retourner un GtkBuilder depuis une ressource (char*)
  };
}

#endif
