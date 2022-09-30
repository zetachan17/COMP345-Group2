#include "Map/Map.h"
#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

/*int Map::getNumber()
{
	return number;
}*/


void MapLoader::readFile()
{
std::fstream mapFile;
        mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);
        //mapFile.open("002_I72_X-29.map", std::ios::in);

        Map* mapObj = new Map();//DESTRUCTOR1
        

        std::string mapText;
        std::string continentsText;
        std::string territoriesText;
        std::vector<std::string> ArrayContinents;//We declare the arrays that store the Continents and Territory data
        std::vector<std::string> ArrayTerritories;


        if (mapFile.is_open())
        {
            std::string currentLine;
            std::regex continentsRegxp("\\=[0-9]+$");
            std::regex territoriesRegxp("\\,");

            


            while (std::getline(mapFile, currentLine))
            {
                if (std::regex_search(currentLine, continentsRegxp))
                {
                    
                    std::stringstream ss(currentLine);
                    while (ss.good())
                    {
                        std::string substr;
                        std::getline(ss, substr, '=');
                        ArrayContinents.push_back(substr);

                    }
                    ArrayContinents.push_back("|");//The point of this is to add a delimiter to know when we're moving on to the data of the next Continent
                    std::cout << "" << std::endl;

                    

                    
                //Continent* contObj=new Continent()

                    
                }
                else if (std::regex_search(currentLine, territoriesRegxp))
                {
                    std::stringstream ss(currentLine);
                    while (ss.good())
                    {
                        std::string substr;
                        std::getline(ss, substr, ',');
                        ArrayTerritories.push_back(substr);
                    }

                    ArrayTerritories.push_back("|");
                    std::cout << "" << std::endl;

                }
            }

            int i = 0;
            ContinentCounter = 0;
            while (i < ArrayContinents.size()) {//This part of the code is to be able to visualize how the arrays store the data on Continents and Territories
                std::cout << "Continent: " << ArrayContinents[i] + " ";
                mapObj->ContinentIDs = ContinentCounter;//Here because we're going through all the continents,we'll assign IDs as we count them
                
                i++;
                std::cout << "Bonus: " << ArrayContinents[i] + " ";
                Continent* contObj = new Continent(ContinentCounter, ArrayContinents[i]);//DESTRUCTOR X
                i++;
                if (ArrayContinents[i].compare("|") == 0) {
                    std::cout << "\n";
                    i++;

                }
                ContinentCounter++;
            }



            i = 0;//Resetting the counter
            while (i < ArrayTerritories.size()) {

                std::cout << "Territory: " << ArrayTerritories[i];
                i = i + 3;
                std::cout << "Continent belonged: " << ArrayTerritories[i] + " ";
                i++;
                std::cout << "Adj territories: ";
                while (ArrayTerritories[i].compare("|") != 0) {
                    std::cout << ArrayTerritories[i] + " ";
                    i++;
                }
                if (ArrayTerritories[i].compare("|") == 0) {
                    std::cout << "\n";
                    i++;

                }
            }

            mapFile.close();
            std::cin.get();
        }
}

Continent::Continent(int contID,std::string contName) {
    ContinentID = contID;
    ContinentName = contName;
}

Territory::Territory(int TerrID, std::string TerrName, int ContID) {

}





