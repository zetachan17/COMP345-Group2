#include "Map/Map.h"

#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
using std::cout;
using std::endl;
using std::string;
using std::regex;


void MapLoader::readFile(string fileName)
{

    std::fstream mapFile;
    mapFile.open(fileName, std::ios::in);
    // mapFile.open("002_I72_X-29.map", std::ios::in);

    // arrays that will store the Continents and Territory string data
    vector<string> continentsData; 
    vector<string> territoriesData;
    
    // arrays of pointers to Continent and Territory objects
    vector<Continent *> continents; 
    vector<Territory *> territories;

    if (mapFile.is_open())
    {
        string currentLine;
        regex continentsRegex("\\=[0-9]+$");
        regex territoriesRegex("\\,");

        while (getline(mapFile, currentLine))
        {
            if (std::regex_search(currentLine, continentsRegex))
            {
                std::stringstream ss(currentLine);
                while (ss.good())
                {
                    std::string substr;
                    std::getline(ss, substr, '=');
                    continentsData.push_back(substr);
                }
                continentsData.push_back("|"); // The point of this is to add a delimiter to know when we're moving on to the data of the next Continent
            }

            else if (std::regex_search(currentLine, territoriesRegex))
            {
                std::stringstream ss(currentLine);
                while (ss.good())
                {
                    std::string substr;
                    std::getline(ss, substr, ',');
                    territoriesData.push_back(substr);
                }

                territoriesData.push_back("|");
            }
            // Above we're going to do some checks on the file before we create the map object
        }
        if (continentsData.size() == 0)
        {
            cout << " There are no continents in this map! Please try another map";
            mapFile.close();
        }
        else if (territoriesData.size() == 0)
        {
            cout << " There are no Territories in this map! Please try another map";
        }
        else
        {

            Map *mapObj = new Map(); // DESTRUCTOR1

            int i = 0;
            ContinentCounter = 0;

            while (i < continentsData.size())
            { // This part of the code is to be able to visualize how the arrays store the data on Continents and Territories
                std::cout << "Continent: " << continentsData[i] + " ";
                mapObj->numberOfContinents = ContinentCounter; // Here because we're going through all the continents,we'll assign IDs as we count them

                i++;
                std::cout << "Bonus: " << continentsData[i] + " ";
                Continent *contObj = new Continent(ContinentCounter, continentsData[i - 1], stoi(continentsData[i])); // DESTRUCTOR X
                i++;

                if (continentsData[i].compare("|") == 0)
                {
                    std::cout << "\n";
                    i++;
                }
                // cout << *contObj;
                mapObj->addContinent(contObj);

                ContinentCounter++;
                contObj = NULL; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0; // Resetting the counter
            TerritoryCounter = 0;
            while (i < territoriesData.size())
            {
                std::cout << "Territory: " << territoriesData[i];
                mapObj->numberOfTerritories = TerritoryCounter;
                i = i + 3;
                std::cout << " Continent belonged: " << territoriesData[i] + " ";

                Territory *TerrObj = new Territory(TerritoryCounter, territoriesData[i - 3], mapObj->getContinentId(territoriesData[i])); //    Destructor!

                i++;
                std::cout << "Adj territories: ";
                while (territoriesData[i].compare("|") != 0)
                {
                    std::cout << territoriesData[i] + " ";
                    i++;
                }
                if (territoriesData[i].compare("|") == 0)
                {
                    std::cout << "\n";
                    i++;
                }
                mapObj->addTerritory(TerrObj);
                TerritoryCounter++;
                TerrObj = NULL; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0;
            while (i < territoriesData.size())
            { // Here we will create the array of ajdacent territories
                Territory *terr = (mapObj->getTerritoryByName(territoriesData[i]));

                // cout << "For territ: " << territoriesData[i]<<"\n";
                i = i + 4;
                while (territoriesData[i].compare("|") != 0)
                {
                    // cout << *(mapObj->getTerrObjByName(territoriesData[i]));
                    terr->addAdjacentTerritory(mapObj->getTerritoryByName(territoriesData[i]));
                    i++;
                }
                if (territoriesData[i].compare("|") == 0)
                {
                    i++;
                }
                terr = NULL; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0;
            while (i < territoriesData.size())
            { // Here we create the array of territories for each continent

                Territory *terr = (mapObj->getTerritoryByName(territoriesData[i]));

                i = i + 3; // This is to access the name of the continent in the Territories data
                Continent *continent = (mapObj->getContinentByName(territoriesData[i]));

                continent->addTerritoryToContinent(terr);
                while (territoriesData[i].compare("|") != 0)
                {
                    i++;
                }
                if (territoriesData[i].compare("|") == 0)
                {
                    i++;
                }
                terr = NULL; // Avoiding dangling pointer problems at the next initialization
            }
            for (Continent *c : continents)
            {
                cout << *c;
            }

            cout << *mapObj;

            mapObj->validate();
            mapFile.close();
            std::cin.get();

            for (Territory *terr : territories)
            { // Memory deallocation to avoid leaks
                delete terr;
                terr = NULL;
            }

            for (Continent *cont : continents)
            {
                delete cont;
                cont = NULL;
            }

            delete mapObj;

            mapObj = NULL; // Handling the pointers to avoid dangling pointers
        }
    }
    else
    {
        cout << "Error: The map file could not be opened, please try again with a different map file";
    }
}

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const MapLoader &MapLObj)
{
    ContinentCounter = 0;
    TerritoryCounter = 0;
}

MapLoader &MapLoader::operator=(const MapLoader &MapLObj)
{
    ContinentCounter = MapLObj.ContinentCounter;
    TerritoryCounter = MapLObj.TerritoryCounter;
    return *this;
}

Territory::Territory(int TerrID, string TerrName, int ContID)
{
    id = TerrID;
    name = TerrName;
    continentId = ContID;
}
/* Important note: For the copy constructors and the overloaded assignment operators, we make it possible to have deep copies, but so far for assignment 1, we have never needed to actually use them!
This implies that even though the new keyword is written, the code is never executed so the allocation on the heap is never made. For that reason, there is no need to delete these objects and handle the pointer values (make them NULL)*/
Territory::Territory(const Territory &TerrObj)
{
    id = TerrObj.id;
    name = TerrObj.name;
    continentId = TerrObj.continentId;
    for (Territory *terri : TerrObj.adjacentTerritories)
    {
        adjacentTerritories.push_back(terri);
    }
}

Territory::~Territory() {}

Territory &Territory::operator=(const Territory &TerrObj)
{ // Important note: Here we define
    id = TerrObj.id;
    name = TerrObj.name;
    continentId= TerrObj.continentId;
    for (Territory *terri : TerrObj.adjacentTerritories)
    {
        adjacentTerritories.push_back(terri);
    }
    return *this;
}

ostream &operator<<(ostream &os, const Territory &TerrObj)
{
    os << "Territory name: " + TerrObj.name << " ID : " << TerrObj.id << " Continent ID: " << TerrObj.continentId << "\n\n";
    return os;
}

void Territory::addAdjacentTerritory(Territory *Terr)
{
    adjacentTerritories.push_back(Terr);
}

vector<Territory*> Territory::getAdjacentTerritories()
{
    return adjacentTerritories;
}

string Territory::getName()
{
    return name;
}

Continent::Continent(int contID, string contName, int bonus)
{
    this->id = contID;
    this->name = contName;
    this->bonus = bonus;
}

Continent::Continent(const Continent &continent)
{
    this->id = continent.id;
    this->name = continent.name;
    this->bonus = continent.bonus;

    for (Territory *territory : continent.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
}

Continent::~Continent() {}

Continent &Continent::operator=(const Continent & continent)
{
    this->id = continent.id;
    this->name = continent.name;
    this->bonus = continent.bonus;
    for (Territory *territory : continent.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
    return *this;
}

void Continent::addTerritoryToContinent(Territory *terr)
{
    territories.push_back(terr);
}

ostream &operator<<(ostream &os, const Continent &Contobj)
{
    os << "Coninent: " + Contobj.name << "ID and Bonus are: " << Contobj.id << " and " << Contobj.bonus << "\n\n";
    for (Territory *y : Contobj.territories)
    {
        os << *y;
    }
    return os;
}

Map::Map()
{
}

Map::Map(const Map &MapObj)
{ // Here we define the copy constructor for the Map class
    numberOfContinents = MapObj.numberOfContinents;
    numberOfTerritories = MapObj.numberOfTerritories;
    continents = MapObj.continents; // These are shallow copies, gotta iterate through the array of pointers to make deep copies
    territories = MapObj.territories;
}

Map::~Map()
{
    for (Continent *cont : continents)
    {
        delete cont;
        cont = NULL;
    }
    for (Territory *terr : territories)
    {
        delete terr;
        terr = NULL;
    }
}

Map &Map::operator=(const Map &MapObj)
{ // generally speaking what goes in the assignment operator is the same as the copy constructor, what might be different is checking for self assignment
    numberOfContinents = MapObj.numberOfContinents;
    numberOfTerritories = MapObj.numberOfTerritories;
    continents = MapObj.continents;
    territories = MapObj.territories;

    return *this;
}

void Map::addContinent(Continent *Cont)
{
    continents.push_back(Cont);
}

void Map::addTerritory(Territory *Terr)
{
    territories.push_back(Terr);
}
int Map::getContinentId(string ContName)
{
    for (Continent *x : continents)
    {
        if ((x->name).compare(ContName) == 0)
        {
            return x->id;
        }
    }
    return -1;
}

Continent *Map::getContinentByName(string name)
{
    for (Continent *x : continents)
    {
        if ((x->name).compare(name) == 0)
        {
            return x;
        }
    }
    return nullptr;
}

void Map::validate()
{
    if (!isMapConnected())
        cout << "Map is not connected!" << endl;
    else if (!isContinentsConnected())
        cout << "Continent is not connected!" << endl;
    else if (!isBelongOneContinent())
        cout << "Terrtory does npt belong to only on continent!" << endl;
    else
        cout << "Map validated!" << endl;
}

bool Map::isMapConnected()
{
    vector<string> visited;

    for (Territory *territory : territories)
    {
        DFS(territory, visited);

        for (Territory *territory : territories)
        {
            if (find(visited.begin(), visited.end(), territory->name) == visited.end())
            {
                return false;
            }
        }
    }

    return true;
}

bool Map::isContinentsConnected()
{
    for (Continent *continent : continents)
    {
        vector<Territory *> &continentTerritories = continent->territories;

        for (Territory *territory : continent->territories)
        {
            vector<string> visited;
            continentDFS(territory, visited);

            for (Territory *territory : continentTerritories)
            {
                if (find(visited.begin(), visited.end(), territory->name) == visited.end())
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
    for (Continent *continent : continents)
    {
        for (const Territory *territory : continent->territories)
        {
            if (territory->continentId != continent->id)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::continentDFS(const Territory *Terr, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), Terr->name) != visited.end())
    {
        return;
    }

    visited.push_back(Terr->name);

    for (const Territory *adjTerritory : Terr->adjacentTerritories)
    {
        if (Terr->continentId == adjTerritory->continentId)
        {
            continentDFS(adjTerritory, visited);
        }
    }
}

void Map::DFS(const Territory *Terr, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), Terr->name) != visited.end())
    {
        return;
    }

    visited.push_back(Terr->name);

    for (const Territory *adjTerritory : Terr->adjacentTerritories)
    {
        DFS(adjTerritory, visited);
    }
}

Territory *Map::getTerritoryByName(string TerrName)
{
    for (Territory *x : territories)
    {
        if ((x->name).compare(TerrName) == 0)
        { // For some reason declaring the Map class as a friend works, but not declaring the function as a friend fn
            return x;
        }
    }
    return nullptr;
}

ostream &operator<<(ostream &os, const Map &mapObjPointer)
{
    for (Continent *x : mapObjPointer.continents)
    {
        os << *x; // important to dereference the object as they are of type Continent* not Continent
    }
    for (Territory *y : mapObjPointer.territories)
    {
        os << *y;
    }
    return os;
}

ostream &operator<<(ostream &os, const MapLoader &mapLoaderObj)
{
    os << "This is the printing statement for the MapLoader class!";
    return os;
}

MapLoader::~MapLoader()
{
    // Nothing in particular is needed here since the MapLoader class contains no data member that is a pointer
}
