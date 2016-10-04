#include <iostream>
#include "util/File.hpp"

using namespace khanar;
using namespace std;

int main()
{
  File file = File("~/");

  cout << file.getAbsolutePath() << endl;
  cout << file.getParentFolderAbsolutePath() << endl;
  cout << file.getName() << endl;
  cout << file.isDirectory() << endl;
  cout << file.exists() << endl;

  vector<File> subFiles = file.getSubFiles();

  for (int i = 0; i < subFiles.size(); i++)
  {
    File f = subFiles[i];

    if (!f.isHidden())
      cout << f.getName() << endl;
  }
}
