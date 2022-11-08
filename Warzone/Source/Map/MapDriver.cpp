#include "Map/MapDriver.h"
#include <Map/Map.h>
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
#include <regex>
#include <sstream>
#include <string>

void testLoadMaps()
{
    string fileName = "0";

    while (fileName.compare("null") != 0)
    {
        cout << "Please enter the name of the file you'd like to load, if there is no other file you would like to try, enter null : ";
        getline(cin, fileName);
        MapLoader *mapl1 = new MapLoader;
        mapl1->readFile(fileName.c_str());
    }
}
