#include "Map/Map.h"
#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>


 void MapLoader::readFile(string fileName)
{
    
        std::fstream mapFile;
        mapFile.open("001_I72_Ghtroc 720.map", std::ios::in);
        //mapFile.open("002_I72_X-29.map", std::ios::in);

        
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
                }
            }
            if (ArrayContinents.size() == 0) {
                cout << " There are no continents in this map! Please try another map";
                mapFile.close();
            }
            else if (ArrayTerritories.size() == 0) {
                cout << " There are no Territories in this map! Please try another map";
            }
            else {

                Map* mapObj = new Map();

                int i = 0;
                ContinentCounter = 0;

                while (i < ArrayContinents.size())
                {
                    mapObj->nbOfContinents = ContinentCounter;//Here because we're going through all the continents,we'll assign IDs as we count them

                    i++;
                    
                    Continent* contObj = new Continent(ContinentCounter, ArrayContinents[i - 1], stoi(ArrayContinents[i]));
                    i++;
                    if (ArrayContinents[i].compare("|") == 0)
                    {
                        std::cout << "\n";
                        i++;
                    }
                    
                    mapObj->addContToContVector(contObj);

                    ContinentCounter++;
                    contObj = NULL;//Avoiding dangling pointer problems at the next initialization
                }

                i = 0;//Resetting the counter
                TerritoryCounter = 0;
                while (i < ArrayTerritories.size())
                {
                    
                    mapObj->nbOfTerritories = TerritoryCounter;
                    i = i + 3;
                    Territory* TerrObj = new Territory(TerritoryCounter, ArrayTerritories[i - 3], mapObj->getContId(ArrayTerritories[i]));//    Destructor!
                    i++;
                    while (ArrayTerritories[i].compare("|") != 0) {
                        i++;
                    }
                    if (ArrayTerritories[i].compare("|") == 0) {
                        i++;
                    }
                    mapObj->addTerrToTerrVector(TerrObj);
                    TerritoryCounter++;
                    TerrObj = NULL;//Avoiding dangling pointer problems at the next initialization
                }
                i = 0;
                while (i < ArrayTerritories.size()) {//Here we will create the array of ajdacent territories 
                    Territory* terr = (mapObj->getTerrObjByName(ArrayTerritories[i]));
                    i = i + 4;
                    while (ArrayTerritories[i].compare("|") != 0) {
           
                        terr->addAdjTerr(mapObj->getTerrObjByName(ArrayTerritories[i]));
                        i++;
                    }
                    if (ArrayTerritories[i].compare("|") == 0) {
                        i++;
                    }
                    terr = NULL;//Avoiding dangling pointer problems at the next initialization
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
                    terr = NULL;//Avoiding dangling pointer problems at the next initialization
                }
                

                if (mapObj->isContinentsconected()) {
                    cout << "The continents are connected subgraphs!"<<"\n";
                }if (mapObj->isMapConnected()) {
                    cout << "The map is a connected graph!"<<"\n";
                }
                mapFile.close();
                std::cin.get();

                for (Territory* terr : TerritoryPointerArray) {//Memory deallocation to avoid leaks
                    delete terr;
                    terr = NULL;
                }

                for (Continent* cont : ContinentPointerArray) {
                    delete cont;
                    cont = NULL;
                }

                delete mapObj;

                mapObj = NULL;//Handling the pointers to avoid dangling pointers
            }
        }
        else {
        cout << "Error: The map file could not be opened, please try again with a different map file";
        }
    

    }



/* Important note: For the copy constructors and the overloaded assignment operators, we make it possible to have deep copies, but so far for assignment 1, we have never needed to actually use them!
This implies that even though the new keyword is written, the code is never executed so the allocation on the heap is never made. For that reason, there is no need to delete these objects and handle the pointer values (make them NULL)*/

//Territory class implementation

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

Territory::~Territory() {
    for (Territory* terr : arrOfAdjTerritories) {
        delete terr;
        terr = NULL;
    }
}

Territory& Territory::operator=(const Territory& TerrObj) { 
    territoryID = TerrObj.territoryID;
    TeritorryName = TerrObj.TeritorryName;
    ContinentId = TerrObj.ContinentId;
    for (Territory* terri : arrOfAdjTerritories) {
        terri = new Territory(terri->territoryID, terri->TeritorryName, terri->ContinentId);
    }
    return *this;
}


ostream& operator<<(ostream & os, const Territory& TerrObj) {
    os << "Territory name: " + TerrObj.TeritorryName << " ID : " << TerrObj.territoryID << " Continent ID: " << TerrObj.ContinentId<<"\n\n";
    return os;
}

void Territory::addAdjTerr(Territory* Terr) {
    arrOfAdjTerritories.push_back(Terr);
}

//Continent class implementation

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

Continent::~Continent() {
    for (Territory* terr:arrOfTerrInContinent) {
        delete terr;
        terr = NULL;
    }
}

Continent& Continent::operator=(const Continent& ContObj) {
    ContinentID = ContObj.ContinentID;
    ContinentName = ContObj.ContinentName;
    Bonus = ContObj.Bonus;
    for (Territory* terri : arrOfTerrInContinent) {
        terri = new Territory(terri->territoryID, terri->TeritorryName, terri->ContinentId);
    }
    return *this;
}
ostream& operator<<(ostream& os, const Continent& Contobj) {
    os << "Coninent: " + Contobj.ContinentName << "ID and Bonus are: " << Contobj.ContinentID << " and "<< Contobj.Bonus <<"\n\n";
    for (Territory* y : Contobj.arrOfTerrInContinent) {
        os << *y;
    }
    return os;
}

void Continent::addTerritoryToContinent(Territory* terr) {
    arrOfTerrInContinent.push_back(terr);
}

//Map Class implementation

Map::Map() {
}

Map::Map(const Map& MapObj) {//Here we define the copy constructor for the Map class
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    ContinentPointerArray = MapObj.ContinentPointerArray;//These are shallow copies, gotta iterate through the array of pointers to make deep copies
    TerritoryPointerArray = MapObj.TerritoryPointerArray;

}
Map::~Map() {
    for (Continent* cont:ContinentPointerArray) {
        delete cont;
        cont = NULL;
    }
    for (Territory* terr:TerritoryPointerArray) {
        delete terr;
        terr = NULL;
    }
}
Map& Map::operator=(const Map& MapObj) {//generally speaking what goes in the assignment operator is the same as the copy constructor, what might be different is checking for self assignment
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    ContinentPointerArray = MapObj.ContinentPointerArray;
    TerritoryPointerArray = MapObj.TerritoryPointerArray;
    
    return *this;
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
}

Continent* Map::getContinent(string name) {
    for (Continent* x : ContinentPointerArray) {
        if ((x->ContinentName).compare(name) == 0) {
            return x;
        }
    }
}

void Map::validate()
{
    if (isMapConnected())
    {
        cout << "YAS" << endl;
    }
    else
    {
        cout << "NA" << endl;
    }
}

bool Map::isMapConnected()
{
    vector <string> visited;

    for (Territory* territory : TerritoryPointerArray)
    {
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

bool Map::isContinentsconected()
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

Territory* Map::getTerrObjByName(string TerrName) {
for (Territory* x : TerritoryPointerArray) {
    if ((x->TeritorryName).compare(TerrName) == 0) {//For some reason declaring the Map class as a friend works, but not declaring the function as a friend fn
        return x;
    }
}

}

//MapLoader class implementation

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const MapLoader& MapLObj)
{
    ContinentCounter = 0;
    TerritoryCounter = 0;
}

MapLoader::~MapLoader() {
    //Nothing in particular is needed here since the MapLoader class contains no data member that is a pointer
}

MapLoader& MapLoader::operator=(const MapLoader& MapLObj)
{
    ContinentCounter = MapLObj.ContinentCounter;
    TerritoryCounter = MapLObj.TerritoryCounter;
    return *this;
}

ostream& operator<<(ostream& os, const MapLoader& mapLoaderObj)
{
    os << "This is the printing statement for the MapLoader class!";
    return os;
}














