
#include "TabContent.hpp"


#include "../compiled_assets/topbar.glade.hex"
#include "../compiled_assets/propertiesbar.glade.hex"
#include "../compiled_assets/vboxlayout.glade.hex"

namespace khanar{
    TabContent::TabContent(){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
    }

    TabContent::TabContent(Gtk::Widget *widget){
        this->_builder = Assets::buildGtkFromResource(topbar_glade);
    }

    Gtk::Box* TabContent::getContent(Gtk::Widget *widget){
        Gtk::Box* add = nullptr;
        Gtk::Box* container = nullptr;

        this->_builder->get_widget("box3",container);
        this->_builder->get_widget("box1",add);
        container->pack_start(*widget);
        container->reorder_child(*widget,0);
        return add;
  }
}
