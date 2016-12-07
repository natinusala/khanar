/**
 * \file Window.hpp
 *
 * Header des suites de classes pour abstraire la création de la fenêtre
 */

#include "Onglet.hpp"
#include <gtkmm.h>
#include "../util/Assets.hpp"
#include "widget/FileTreeView.hpp"
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
          RefPtr<Builder> _winBuilder;
          Gtk::Window* _win;
          Gtk::Notebook* _notebook;
          vector<Onglet*> _onglets;

          //Signal handlers:
          File _clipboard;
          File _clipboardParent;
          bool _shouldDeleteClipboard;

        public:
          /**
          * \brief Constructeur
          */
          Window();
          Gtk::Window* getWindow();///< Récupération de la window Gtk qui sera affichée par l'application Gtk
          void addOnglet(string str);///< Ajoute un onglet "Fichiers Récents"
          void delOnglet(int idOnglet);///< Supprime l'onglet à l'indice idOnglet
          void addOnglet(string folder ,string title);///< Ajoute un onglet affichant le contenu de "folder"
          void labelOngletPlus(); ///< Remise en place du label d'ajout d'onglet deprecated
          void actualiser(); ///< Mise à jour de la vue avec sauvegarde des informations courantes
          void actualiser(string filepath); ///< Mise à jour de la vue avec modification du dossier courant
          void search(string search); ///< Mise à jour de la vue avec un critère de recherche
          string getCurrentFile(); ///< Récupération du chemin du dossier actuellement affiché

          void updatePropBar(File* file);///< Mise a jour de la barre de propriétés
          void updateClipboard(File file, File parent, bool shouldDeleteClipboard); ///< Mets à jour les infos du presse papiers
          File getClipboard(); ///< Récupère le fichier actuellement dans le presse papiers
          File getClipboardParent(); ///< Récupère le fichier parent du fichier actuellement dans le presse papiers
          bool getShouldDeleteClipboard(); ///< Est-ce que le fichier doit être supprimé après avoir été collé ?

    };
}
#endif
