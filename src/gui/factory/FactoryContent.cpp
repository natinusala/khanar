#include "FactoryContent.hpp"
#include "../widget/WidgetPerso.hpp"
#include "../widget/FileTreeView.hpp"
#include <string>

using namespace khanar;

WidgetPerso* FactoryContent::getWidget()
{
  return nullptr;
}

WidgetPerso* FactoryContent::getWidget(string data)
{
  if (data == "FileTreeView")
  {
      return new FileTreeView(gtkWin, khanarWind, contentPath, contentRecents);
  }

  return nullptr;
}
