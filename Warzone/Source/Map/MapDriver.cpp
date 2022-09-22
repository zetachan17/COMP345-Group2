#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include "Map/MapDriver.h"

void testLoadMaps() {
    std::fstream mapFile;
    mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);
    //mapFile.open("002_I72_X-29.map", std::ios::in);
    
    std::string mapText;
    std::string continentsText;
    std::string territoriesText;
    
    if(mapFile.is_open())
    {
        std::string currentLine;
        std::regex maptagRegxp("(\\[Map\\])");
        std::regex continentsRegxp("[a-zA-Z\\s]*\\=[0-9]+");
        std::regex territoriesRegxp("[a-zA-Z0-9\\s\\_\\!]*\\,[0-9]+\\,[0-9]+\\,[a-zA-Z0-9\\,\\s\\_\\!]*");

        std::regex continentNameRegxp("[a-zA-Z\\s]*");
        std::regex continentScoreRegxp("[0-9]+");
        
        while(std::getline(mapFile, currentLine))
        {
            if (std::regex_match(currentLine, continentsRegxp))
            {
                std::smatch continentName;
                std::smatch continentScore;
                
                std::regex_search(currentLine, continentName, continentNameRegxp);
                std::cout << continentName.str(0) << std::endl;

                std::regex_search(currentLine, continentScore, continentScoreRegxp);
                std::cout << continentScore.str(0) << std::endl;
            }
            else if (std::regex_match(currentLine, territoriesRegxp))
            {
                 std::cout << currentLine << std::endl;
            }
        }
        
        mapFile.close();
    }
    
}
