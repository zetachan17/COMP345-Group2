#include "Map/Map.h"
#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

void MapLoader::readFile()
{
    std::fstream mapFile;
    mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);
    //mapFile.open("002_I72_X-29.map", std::ios::in);

    std::string mapText;
    std::string continentsText;
    std::string territoriesText;
    std::vector<std::string> ArrayContinents;//We declare the arrays that store the Continents and Territory data
    std::vector<std::string> ArrayTerritories;
    vector<Continent*> ContinentPointerArray;//Here we're creating the arrays of pointers to Continent and Territory objects
    vector<Territory*> TerritoryPointerArray;

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
            //Above we're going to do some checks on the file before we create the map object

        }

        Map* mapObj = new Map();//DESTRUCTOR1

        int i = 0;
        ContinentCounter = 0;

        while (i < ArrayContinents.size()) 
        {//This part of the code is to be able to visualize how the arrays store the data on Continents and Territories
            std::cout << "Continent: " << ArrayContinents[i] + " ";
            mapObj->nbOfContinents = ContinentCounter;//Here because we're going through all the continents,we'll assign IDs as we count them

            i++;
            std::cout << "Bonus: " << ArrayContinents[i] + " ";
            Continent* contObj = new Continent(ContinentCounter, ArrayContinents[i-1],stoi(ArrayContinents[i]));//DESTRUCTOR X
            i++;
            if (ArrayContinents[i].compare("|") == 0)
            {
                std::cout << "\n";
                i++;
            }
            //cout << *contObj;
            mapObj->addContToContVector(contObj);
                
            ContinentCounter++;
        }

        i = 0;//Resetting the counter
        TerritoryCounter = 0;
        while (i < ArrayTerritories.size())
        {
            std::cout << "Territory: " << ArrayTerritories[i];
            mapObj->nbOfTerritories = TerritoryCounter;
            i = i + 3;
            std::cout << " Continent belonged: " << ArrayTerritories[i] + " ";
               
                
            Territory* TerrObj = new Territory(TerritoryCounter, ArrayTerritories[i - 3], mapObj->getContId(ArrayTerritories[i]));
               
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
            mapObj->addTerrToTerrVector(TerrObj);
            TerritoryCounter++;
        }
            
        i = 0;
        while (i<ArrayTerritories.size()) {//Here we will create the array of ajdacent territories 
            Territory* terr = (mapObj->getTerrObjByName(ArrayTerritories[i]));
                
            //cout << "For territ: " << ArrayTerritories[i]<<"\n";
            i = i + 4;
            while (ArrayTerritories[i].compare("|") != 0) {
                //cout << *(mapObj->getTerrObjByName(ArrayTerritories[i]));
                terr->addAdjTerr(mapObj->getTerrObjByName(ArrayTerritories[i]));
                i++;
            }
            if (ArrayTerritories[i].compare("|") == 0) {
                i++;
            }
        }

        i = 0;
        while (i < ArrayTerritories.size()) {//Here we create the array of territories for each continent
            Territory* terr = (mapObj->getTerrObjByName(ArrayTerritories[i]));
            
            i = i + 3;//This is to access the name of the continent in the Territories data
            Continent* continent = (mapObj->getContinent(ArrayTerritories[i]));

            continent->addTerritoryToContinent(terr);
            while (ArrayTerritories[i].compare("|") != 0) {
                i++;
            }
            if (ArrayTerritories[i].compare("|") == 0) {
                i++;
            }
        }
        
        for (Continent* c : ContinentPointerArray) {
            cout << *c;
        }
        
        if (mapObj->validate())
        {
            cout << *mapObj;
        }

        mapFile.close();
        //std::cin.get();
    }
}



Territory::Territory(int TerrID, string TerrName, int ContID) {
    territoryID = TerrID;
    TeritorryName = TerrName;
    ContinentId = ContID;
}

Territory::Territory(const Territory& TerrObj) {
    territoryID = TerrObj.territoryID;
    TeritorryName = TerrObj.TeritorryName;
    ContinentId = TerrObj.ContinentId;
    for (Territory* terri : arrOfAdjTerritories) {
        terri = new Territory(terri->territoryID, terri->TeritorryName, terri->ContinentId);
    }
}

ostream& operator<<(ostream & os, const Territory& TerrObj) {
    os << "Territory name: " + TerrObj.TeritorryName << " ID : " << TerrObj.territoryID << " Continent ID: " << TerrObj.ContinentId<<"\n\n";
    return os;
}

void Territory::addAdjTerr(Territory* Terr) {
    arrOfAdjTerritories.push_back(Terr);
}

Continent::Continent(int contID,string contName, int bonus) {
    ContinentID = contID;
    ContinentName = contName;
    Bonus = bonus;
}

Continent::Continent(const Continent& ContObj) {
    ContinentID = ContObj.ContinentID;
    ContinentName = ContObj.ContinentName;
    Bonus = ContObj.Bonus;
    for (Territory* terri : arrOfTerrInContinent) {
        terri = new Territory(terri->territoryID, terri->TeritorryName, terri->ContinentId);
    }
}

void Continent::addTerritoryToContinent(Territory* terr) {
    arrOfTerrInContinent.push_back(terr);
}

ostream& operator<<(ostream& os, const Continent& Contobj) {
    os << "Coninent: " + Contobj.ContinentName << "ID and Bonus are: " << Contobj.ContinentID << " and "<< Contobj.Bonus <<"\n\n";
    for (Territory* y : Contobj.arrOfTerrInContinent) {
        os << *y;
    }
    return os;
}

Map::Map() {


}

Map::Map(const Map& MapObj) {//Here we define the copy constructor for the Map class
    Cont = new Continent(*(MapObj.Cont));
    Terr = new Territory(*(MapObj.Terr));
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    ContinentPointerArray = MapObj.ContinentPointerArray;//These are shallow copies, gotta iterate through the array of pointers to make deep copies
    TerritoryPointerArray = MapObj.TerritoryPointerArray;
}

Map& Map::operator=(const Map& MapObj) {//generally speaking what goes in the assignment operator is the same as the copy constructor, what might be different is checking for self assignment
    Cont = new Continent(*(MapObj.Cont));
    Terr = new Territory(*(MapObj.Terr));
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    ContinentPointerArray = MapObj.ContinentPointerArray;
    TerritoryPointerArray = MapObj.TerritoryPointerArray;
    
    return *this;
}

void Map::addContToContVector(Continent* Cont) {
    ContinentPointerArray.push_back(Cont);
}

void Map::addTerrToTerrVector(Territory* Terr) {
    TerritoryPointerArray.push_back(Terr);
}

int Map::getContId(string ContName) {
    for (Continent* x : ContinentPointerArray) {
        if ((x->ContinentName).compare(ContName) == 0) {
            return x->ContinentID;
        }
    }
    //TODO:
    //add default return
}

Continent* Map::getContinent(string name) {
    for (Continent* x : ContinentPointerArray) {
        if ((x->ContinentName).compare(name) == 0) {
            return x;
        }
    }
    //TODO:
    //add default return
}

bool Map::validate()
{
    if (!isMapConnected())
        return false;
    if (!isContinentConected())
        return false;
    if (!isBelongOneContinent())
        return false;

    return true;
}

bool Map::isMapConnected()
{
    for (Territory* territory : TerritoryPointerArray)
    {
        vector <string> visited;
        DFS(territory, visited);

        for (Territory* territory : TerritoryPointerArray)
        {
            if (find(visited.begin(), visited.end(), territory->TeritorryName) == visited.end())
            {
                return false;
            }
        }
    }

    return true;
}

bool Map::isContinentConected()
{
    for (Continent* continent : ContinentPointerArray)
    {
        vector <Territory*>& continentTerritories = continent->arrOfTerrInContinent;

        for (Territory* territory : continent->arrOfTerrInContinent)
        {
            vector <string> visited;
            continentDFS(territory, visited);

            for (Territory* territory : continentTerritories)
            {
                if (find(visited.begin(), visited.end(), territory->TeritorryName) == visited.end())
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Map::isBelongOneContinent()
{
    for (Continent* continent : ContinentPointerArray)
    {
        for (const Territory* territory : continent->arrOfTerrInContinent)
        {
            if (territory->ContinentId != continent->ContinentID)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::DFS(const Territory* Terr, vector <string>& visited)
{
    if (find(visited.begin(), visited.end(), Terr->TeritorryName) != visited.end())
    {
        return;
    }
        
    visited.push_back(Terr->TeritorryName);

    for (const Territory* adjTerritory : Terr->arrOfAdjTerritories)
    {
        DFS(adjTerritory, visited);
    }
}

void Map::continentDFS(const Territory* Terr, vector<string>& visited)
{
    if (find(visited.begin(), visited.end(), Terr->TeritorryName) != visited.end())
    {
        return;
    }

    visited.push_back(Terr->TeritorryName);

    for (const Territory* adjTerritory : Terr->arrOfAdjTerritories)
    {
        if (Terr->ContinentId == adjTerritory->ContinentId)
        {
            continentDFS(adjTerritory, visited);
        }
    }
}

Territory* Map::getTerrObjByName(string TerrName) {
    for (Territory* x : TerritoryPointerArray) {
        if ((x->TeritorryName).compare(TerrName) == 0) {//For some reason declaring the Map class as a friend works, but not declaring the function as a friend fn
            return x;
        }
    }
    //TODO:
    //add default return
}


ostream& operator<<(ostream & os, const Map & mapObjPointer) {
    for (Continent* x : mapObjPointer.ContinentPointerArray) {
        os << *x;//important to dereference the object as they are of type Continent* not Continent
    }
    for (Territory* y : mapObjPointer.TerritoryPointerArray) {
        os << *y;
    }
    return os;
}







