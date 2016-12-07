#include "FactoryGlade.hpp"
#include "../widget/WidgetPerso.hpp"
#include "../widget/PropertiesBar.hpp"
#include <string>

using namespace khanar;

WidgetPerso* FactoryGlade::getWidget(string data)
{
  if (data == "PropertiesBar")
  {
      return new PropertiesBar(this->builder);
  }

  return nullptr;
}
