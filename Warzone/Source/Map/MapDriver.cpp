#include "Map/MapDriver.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <Map/Map.h>



void testLoadMaps() {
    MapLoader *mapl1=new MapLoader;
    mapl1->readFile();
}
