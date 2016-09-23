#include <gtkmm.h>
#include <iostream>

//Fonctions/////////////////////////////////////////////////////////////

void on_button_clicked() {
	std::cout << "Bonjour" << std::endl;
	}




//Fenêtre///////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	Gtk::Main app(argc, argv);
	Gtk::Window fenetre; 
	Gtk::VBox vb;

//Paramétrage

	fenetre.set_title("Ma belle fenetre!");
	fenetre.set_icon_from_file("icone.ico");
	fenetre.set_border_width(10);
	fenetre.add(vb);
	vb.show();
//Remplissage de la Verticale Box
	Gtk::Label l;
	vb.pack_start(l);
	l.show();
	Gtk::Entry e;
	vb.pack_start(e);
	e.show();

//Création d'un bouton
	Gtk::Button quitter(Gtk::Stock::QUIT);
	quitter.show();
	vb.pack_start(quitter);

//Création d'un second bouton
	Gtk::Button test(Gtk::Stock::OK);
	test.show();
	vb.pack_start(test);

//redimensionne la fenêtre
	fenetre.resize(480,480);

//Connexion des boutons aux signaux correspondants
	quitter.signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));

	test.signal_clicked().connect(sigc::slot(&on_button_clicked));
//Ouverture de la fenêtre
	Gtk::Main::run(fenetre);
    return 0;
}
