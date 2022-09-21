#include <string>
#include <fstream>
#include <iostream>
#include <regex>

void testLoadMaps();

int main()
{
    testLoadMaps();
}

void testLoadMaps() {
    std::fstream mapFile;
    mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);

    std::string mapText;
    
    if(mapFile.is_open())
    {
        std::string readText;
        while(std::getline(mapFile, readText))
        {
            std::regex emptylineRegxp("^$");
            if (std::regex_match(readText, emptylineRegxp))
            {
                break;
            }
            
            mapText += readText;
        }
        mapFile.close();
    }

    std::cout << mapText << std::endl;
}