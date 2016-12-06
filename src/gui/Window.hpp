/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */

#include "Onglet.hpp"
#include <gtkmm.h>
#include "../util/Assets.hpp"
#include "FileTreeView.hpp"
#include <cstdint>
#include <iostream>



#ifndef WINDOW_HPP
#define WINDOW_HPP

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
    class Window
    {
        private:
          RefPtr<Builder> _win_builder;
          Gtk::Window* _win;
          Gtk::Notebook* _notebook;
          vector<Onglet> _onglets;
          //Signal handlers:

          File _clipboard;
          File _clipboardParent;
          bool _shouldDeleteClipboard;

        public:
          Window();
          Gtk::Window* getWindow();
          void addOnglet(string str );
          void delOnglet(int idOnglet);
          void setPropInfo(string nom, string type, string icon, string Dateacces, string Datemodif, string proprietaire,string DroitProprietaire, string Groupe, string DroitGroupe, string DroitAutre ); //TO DO -> Ajouter les arguments correspondant au changement d'informations d'une barre de propriétés
          void addOnglet(string folder ,string title);
          void resetPropInfo();
          void labelOngletPlus();
          void actualiser();
          void actualiser(string filepath);
          string getCurrentFile();

          void updatePropBar(File file);
          void updateClipboard(File file, File parent, bool shouldDeleteClipboard); ///< Mets à jour les infos du presse papiers
          File getClipboard(); ///< Récupère le fichier actuellement dans le presse papiers
          File getClipboardParent(); ///< Récupère le fichier parent du fichier actuellement dans le presse papiers
          bool getShouldDeleteClipboard(); ///< Est-ce que le fichier doit être supprimé après avoir été collé ?

    };
}
#endif
