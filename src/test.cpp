#include <iostream>
#include <regex>
#include "util/File.hpp"

using namespace khanar;
using namespace std;

#include <stdio.h>
#include <unistd.h>

int main()
{
  try
  {

    File file = File("/");
    cout << file.exists() << endl;
    file.openXterm();

    /**File file = File("/home/natinusala/Bureau");

    vector<File> v = file.search("(.*?)png");

    for (int i = 0; i < v.size(); i++)
    {
      cout << v.at(i).getName() << endl;
    }*/

    /*vector<File> mounted = File::getMountedVolumes();
    for (int i = 0; i < mounted.size(); i++)
    {
      File f = mounted.at(i);
      cout << f.getName() << " : " << f.getAbsolutePath() << endl;
    }*/

    /*char path[4096];
    FILE *fp = popen("fdisk -l | grep \"Disk /\" | awk '{print $2};' | sed 's/://'", "r");
    while(fgets(path, sizeof(path) -1,fp) != NULL)
    //your code
    pclose(fp);*/


    /*cout << file.getAbsolutePath() << endl;
    cout << file.getParentFolderAbsolutePath() << endl;
    cout << file.getName() << endl;
    cout << file.isDirectory() << endl;
    cout << file.exists() << endl;*/



    /*vector<File>* subFiles = file.getSubFiles();

    for (int i = 0; i < subFiles->size(); i++)
    {
      File f = (*subFiles)[i];

      if (!f.isHidden())
        cout << f.getName() << " - " << f.getFormattedSize() << " (" << f.getSize() << "o)" << endl;
    }*/
  }
  catch (FileException e)
  {
    cout << e.getDescription() << endl;
  }
  catch (regex_error e)
  {
    cout << e.what() << endl;
  }
}
