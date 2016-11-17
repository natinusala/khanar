#include <iostream>
#include <regex>
#include "util/File.hpp"
#include <gtkmm.h>

using namespace khanar;
using namespace std;

#include <stdio.h>
#include <unistd.h>

#include "libs/json/json.h"

int main(int argc, char* argv[])
{
  try
  {
    Gtk::Main app(argc, argv);

    vector<gid_t> recentFiles = File("/home/natinusala/Bureau/15134771_1188114111280865_9218025161381243426_n.jpg").getGroupList();
    for (int i = 0; i < recentFiles.size(); i++)
    {
      cout << File::getGIDName(recentFiles.at(i)) << endl;
    }
    //cout << f.isInFavorites() << endl;


    /*Json::Value root;

    root["prout"] = "lel";

    cout << root << endl;*/

    /*File file = File("/");
    cout << file.exists() << endl;
    file.openXterm();*/

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
