#include <string>
#include <fstream>
#include <iostream>

void testLoadMaps();

int main()
{
    testLoadMaps();
}

void testLoadMaps() {
    std::fstream mapFile;
    mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);
    if(mapFile.is_open())
    {
        std::string mapText;
        while(std::getline(mapFile, mapText))
        {
            std::cout << mapText << std::endl;
        }
        mapFile.close();
    }
}