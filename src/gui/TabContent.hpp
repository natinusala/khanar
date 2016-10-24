/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */


#include <gtkmm.h>
#include "../util/Assets.hpp"
#include <cstdint>
#include <iostream>

#ifndef TABCONTENT_HPP
#define TABCONTENT_HPP

using namespace Gtk;
using namespace Glib;
using namespace std;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class TabContent{

    private:
      RefPtr<Builder> _builder;


    public:
      TabContent();
      TabContent(Gtk::Widget *widget);
      Gtk::Box* getContent(Gtk::Widget *widget);




    };

  }
#endif
