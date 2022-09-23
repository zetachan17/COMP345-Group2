#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
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
        std::regex continentsRegxp("\\=[0-9]+$");
        std::regex territoriesRegxp("\\,");
        
        while(std::getline(mapFile, currentLine))
        {
            if (std::regex_search(currentLine, continentsRegxp))
            {
                std::vector<std::string> result;
                std::stringstream ss(currentLine);
                while (ss.good())
                {
                    std::string substr;
                    std::getline(ss, substr, '=');
                    result.push_back(substr);
                }
                std::cout << "Continent: " << result[0] << " Score: " << result[1];
               
                std::cout << "" <<std::endl;
            }
            else if (std::regex_search(currentLine, territoriesRegxp))
            {
               std::vector<std::string> result;
               std::stringstream ss(currentLine);
               while (ss.good())
               {
                   std::string substr;
                   std::getline(ss, substr, ',');
                   result.push_back(substr);
               }
               std::cout << "Territory: " << result[0] << " Continent belonged: " << result[3] << " Neighbour: ";
                
               for (int i= 4; i != result.size(); ++i)
                   std::cout << result[i] << ' ';
               
               std::cout << "" <<std::endl;
            }
        }
        mapFile.close();
    }
    
}
