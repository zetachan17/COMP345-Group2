#include "Map/Map.h"

#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <algorithm>
using std::cout;
using std::endl;
using std::regex;
using std::string;

bool MapLoader::readFile(string fileName)
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
                    i++;
                }

                mapObj->addContinent(contObj);

                ContinentCounter++;
                contObj = nullptr; // Avoiding dangling pointer problems at the next initialization
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
                    i++;
                }
                mapObj->addTerritory(TerrObj);
                TerritoryCounter++;
                TerrObj = nullptr; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0;
            while (i < territoriesData.size())
            { // Here we will create the array of ajdacent territories
                Territory *terr = (mapObj->getTerritoryByName(territoriesData[i]));

                i = i + 4;
                while (territoriesData[i].compare("|") != 0)
                {
                    terr->addAdjacentTerritory(mapObj->getTerritoryByName(territoriesData[i]));
                    i++;
                }
                if (territoriesData[i].compare("|") == 0)
                {
                    i++;
                }
                terr = nullptr; // Avoiding dangling pointer problems at the next initialization
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
                terr = nullptr; // Avoiding dangling pointer problems at the next initialization
            }
            for (Continent *c : continents)
            {
                cout << *c;
            }

            map = mapObj;

            mapFile.close();
            // TODO: FIND A PLACE TO DELETE THESE FUCKING POINTERS
            //  for (Territory *terr : territories)
            //  { // Memory deallocation to avoid leaks
            //      delete terr;
            //      terr = NULL;
            //  }
            //
            //  for (Continent *cont : continents)
            //  {
            //      delete cont;
            //      cont = NULL;
            //  }
            //
            //  delete mapObj;
            //
            //  mapObj = NULL; // Handling the pointers to avoid dangling pointers
        }
        return true;
    }
    else
    {
        cout << "Error: The map file could not be opened, please try again with a different map file\n";
        return false;
    }
}

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const MapLoader &mapLoader)
{
    ContinentCounter = 0;
    TerritoryCounter = 0;
}

Map *MapLoader::getMap() const
{
    return map;
}

MapLoader &MapLoader::operator=(const MapLoader &mapLoader)
{
    ContinentCounter = mapLoader.ContinentCounter;
    TerritoryCounter = mapLoader.TerritoryCounter;
    return *this;
}

Territory::Territory(int TerrID, string TerrName, int ContID)
{
    territoryId = TerrID;
    territoryName = TerrName;
    continentId = ContID;
    armyUnits = 0;
    owner = nullptr;
}
/* Important note: For the copy constructors and the overloaded assignment operators, we make it possible to have deep copies, but so far for assignment 1, we have never needed to actually use them!
This implies that even though the new keyword is written, the code is never executed so the allocation on the heap is never made. For that reason, there is no need to delete these objects and handle the pointer values (make them NULL)*/
Territory::Territory(const Territory &otherTerritory)
{
    territoryId = otherTerritory.territoryId;
    territoryName = otherTerritory.territoryName;
    continentId = otherTerritory.continentId;
    armyUnits = otherTerritory.armyUnits;
    owner = otherTerritory.owner;
    for (Territory *terri : otherTerritory.adjacentTerritories)
    {
        adjacentTerritories.push_back(terri);
    }
}

Territory::~Territory() {}

Territory &Territory::operator=(const Territory &otherTerritory)
{ // Important note: Here we define
    territoryId = otherTerritory.territoryId;
    territoryName = otherTerritory.territoryName;
    continentId = otherTerritory.continentId;
    armyUnits = otherTerritory.armyUnits;
    owner = otherTerritory.owner;
    for (Territory *adjacentTerritory : otherTerritory.adjacentTerritories)
    {
        adjacentTerritories.push_back(adjacentTerritory);
        // erritory *newTerritory = new Territory(*terri);
        // arrOfAdjTerritories.push_back(newTerritory);
    }
    return *this;
}

ostream &operator<<(ostream &os, const Territory &territory)
{
    os << "Territory name: " + territory.territoryName << " ID : " << territory.territoryId << " Continent ID: " << territory.continentId << "\n\n";
    return os;
}

void Territory::setArmyUnits(int units)
{
    armyUnits = std::max(units, 0);
}

int Territory::getArmyUnits() const
{
    return armyUnits;
}

void Territory::setOwner(Player *player)
{
    owner = player;
}

Player *Territory::getOwner() const
{
    return owner;
}

void Territory::addAdjacentTerritory(Territory *territory)
{
    adjacentTerritories.push_back(territory);
}

vector<Territory *> Territory::getAdjacentTerritories()
{
    return adjacentTerritories;
}

string Territory::getTerritoryName()
{
    return territoryName;
}

Continent::Continent(int continentId, string continentName, int bonus)
{
    this->continentId = continentId;
    this->continentName = continentName;
    this->bonus = bonus;
}

Continent::Continent(const Continent &continent)
{
    this->continentId = continent.continentId;
    this->continentName = continent.continentName;
    this->bonus = continent.bonus;

    for (Territory *territory : continent.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
}

Continent::~Continent() {}

Continent &Continent::operator=(const Continent &continent)
{
    this->continentId = continent.continentId;
    this->continentName = continent.continentName;
    this->bonus = continent.bonus;
    for (Territory *territory : continent.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
    // for (Territory *terri : continent.arrOfTerrInContinent)
    // {
    //     Territory *newTerritory = new Territory(terri->territoryID, terri->TerritoryName, terri->territoryID);
    //     arrOfTerrInContinent.push_back(newTerritory);
    //     terri = new Territory(terri->territoryID, terri->TerritoryName, terri->ContinentId);
    // }
    return *this;
}

void Continent::addTerritoryToContinent(Territory *territory)
{
    territories.push_back(territory);
}

ostream &operator<<(ostream &os, const Continent &continent)
{
    os << "Coninent: " + continent.continentName << "ID and Bonus are: " << continent.continentId << " and " << continent.bonus << "\n\n";
    for (Territory *y : continent.territories)
    {
        os << *y;
    }
    return os;
}

Map::Map()
{
}

// Here we define the copy constructor for the Map class
Map::Map(const Map &otherMap)
{
    numberOfContinents = otherMap.numberOfContinents;
    numberOfTerritories = otherMap.numberOfTerritories;
    continents = otherMap.continents; // These are shallow copies, gotta iterate through the array of pointers to make deep copies
    territories = otherMap.territories;
    // for (int i = 0; i < otherMap.ContinentPointerArray.size(); ++i)
    // {
    //     Continent *newContinent;
    //     newContinent = otherMap.ContinentPointerArray[i];
    //     ContinentPointerArray.push_back(newContinent);
    // }
    //
    // for (int i = 0; i < otherMap.TerritoryPointerArray.size(); ++i)
    // {
    //     Territory *newTerritory;
    //     newTerritory = otherMap.TerritoryPointerArray[i];
    //     TerritoryPointerArray.push_back(newTerritory);
    // }
}

Map::~Map()
{
    for (Continent *continent : continents)
    {
        delete continent;
        continent = nullptr;
    }
    for (Territory *territory : territories)
    {
        delete territory;
        territory = nullptr;
    }
}

// generally speaking what goes in the assignment operator is the same as the copy constructor, what might be different is checking for self assignment
Map &Map::operator=(const Map &otherMap)
{
    numberOfContinents = otherMap.numberOfContinents;
    numberOfTerritories = otherMap.numberOfTerritories;
    continents = otherMap.continents;
    territories = otherMap.territories;
    // for (int i = 0; i < otherMap.ContinentPointerArray.size(); ++i)
    // {
    //     Continent *newContinent = new Continent(*otherMap.ContinentPointerArray[i]);
    //     ContinentPointerArray.push_back(newContinent);
    // }
    //
    // for (int i = 0; i < otherMap.TerritoryPointerArray.size(); ++i)
    // {
    //     Territory *newTerritory = new Territory(*otherMap.TerritoryPointerArray[i]);
    //     TerritoryPointerArray.push_back(newTerritory);
    // }
    return *this;
}

void Map::addContinent(Continent *continent)
{
    continents.push_back(continent);
}

void Map::addTerritory(Territory *territory)
{
    territories.push_back(territory);
}
int Map::getContinentId(string continentName)
{
    for (Continent *continent : continents)
    {
        if ((continent->continentName).compare(continentName) == 0)
        {
            return continent->continentId;
        }
    }
    return -1;
}

Continent *Map::getContinentByName(string name)
{
    for (Continent *continent : continents)
    {
        if ((continent->continentName).compare(name) == 0)
        {
            return continent;
        }
    }
    return nullptr;
}

bool Map::validate()
{
    if (!isMapConnected())
    {
        cout << "Map is not connected!" << endl;
        return false;
    }
    cout << "Map is connected!" << endl;

    if (!isContinentsConnected())
    {
        cout << "Continent is not connected!" << endl;
        return false;
    }
    cout << "Continent is connected!" << endl;

    if (!territoriesBelongToOneContinent())
    {
        cout << "Territory is not belonged to only on continent!" << endl;
        return false;
    }
    cout << "All territories are belonged to only on continent!" << endl;

    cout << "Map validated!" << endl;
    return true;
}

bool Map::isMapConnected()
{
    vector<string> visited;

    for (Territory *territory : territories)
    {
        DFS(territory, visited);

        for (Territory *territory : territories)
        {
            if (find(visited.begin(), visited.end(), territory->territoryName) == visited.end())
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
                if (find(visited.begin(), visited.end(), territory->territoryName) == visited.end())
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Map::territoriesBelongToOneContinent()
{
    for (Continent *continent : continents)
    {
        for (const Territory *territory : continent->territories)
        {
            if (territory->continentId != continent->continentId)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::continentDFS(const Territory *territory, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), territory->territoryName) != visited.end())
    {
        return;
    }

    visited.push_back(territory->territoryName);

    for (const Territory *adjTerritory : territory->adjacentTerritories)
    {
        if (territory->continentId == adjTerritory->continentId)
        {
            continentDFS(adjTerritory, visited);
        }
    }
}

void Map::DFS(const Territory *territory, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), territory->territoryName) != visited.end())
    {
        return;
    }

    visited.push_back(territory->territoryName);

    for (const Territory *adjTerritory : territory->adjacentTerritories)
    {
        DFS(adjTerritory, visited);
    }
}

Territory *Map::getTerritoryByName(string name)
{
    for (Territory *x : territories)
    {
        if ((x->territoryName).compare(name) == 0)
        { // For some reason declaring the Map class as a friend works, but not declaring the function as a friend fn
            return x;
        }
    }
    return nullptr;
}

vector<Territory *> Map::getTerritories()
{
    return territories;
}

vector<Continent *> Map::getContinents()
{
    return continents;
}

vector<Territory *> Continent::getTerritories()
{
    return territories;
}

string Continent::getContinentName()
{
    return continentName;
}

int Continent::getBonus()
{
    return bonus;
}

ostream &operator<<(ostream &os, const Map &map)
{
    for (Continent *x : map.continents)
    {
        os << *x; // important to dereference the object as they are of type Continent* not Continent
    }
    for (Territory *y : map.territories)
    {
        os << *y;
    }
    return os;
}

ostream &operator<<(ostream &os, const MapLoader &mapLoader)
{
    os << "This is the printing statement for the MapLoader class!";
    return os;
}

MapLoader::~MapLoader()
{
    // Nothing in particular is needed here since the MapLoader class contains no data member that is a pointer
}
