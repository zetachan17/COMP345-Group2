#include "Map/Map.h"
#include <string>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

bool MapLoader::readFile(string fileName)
{
    std::fstream mapFile;
    mapFile.open(fileName, std::ios::in);
    // mapFile.open("002_I72_X-29.map", std::ios::in);

    std::vector<std::string> ArrayContinents; // We declare the arrays that store the Continents and Territory data
    std::vector<std::string> ArrayTerritories;
    vector<Continent *> ContinentPointerArray; // Here we're creating the arrays of pointers to Continent and Territory objects
    vector<Territory *> TerritoryPointerArray;

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
                ArrayContinents.push_back("|"); // The point of this is to add a delimiter to know when we're moving on to the data of the next Continent
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
            // Above we're going to do some checks on the file before we create the map object
        }
        if (ArrayContinents.size() == 0)
        {
            cout << " There are no continents in this map! Please try another map";
            mapFile.close();
        }
        else if (ArrayTerritories.size() == 0)
        {
            cout << " There are no Territories in this map! Please try another map";
        }
        else
        {

            Map *mapObj = new Map(); // DESTRUCTOR1
            
            int i = 0;
            ContinentCounter = 0;

            while (i < ArrayContinents.size())
            { // This part of the code is to be able to visualize how the arrays store the data on Continents and Territories
                //std::cout << "Continent: " << ArrayContinents[i] + " ";
                mapObj->nbOfContinents = ContinentCounter; // Here because we're going through all the continents,we'll assign IDs as we count them

                i++;
                //std::cout << "Bonus: " << ArrayContinents[i] + " ";
                Continent *contObj = new Continent(ContinentCounter, ArrayContinents[i - 1], stoi(ArrayContinents[i])); // DESTRUCTOR X
                i++;

                if (ArrayContinents[i].compare("|") == 0)
                {
                    //std::cout << "\n";
                    i++;
                }
                // cout << *contObj;
                mapObj->addContToContVector(contObj);

                ContinentCounter++;
                contObj = nullptr; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0; // Resetting the counter
            TerritoryCounter = 0;
            while (i < ArrayTerritories.size())
            {
                //std::cout << "Territory: " << ArrayTerritories[i];
                mapObj->nbOfTerritories = TerritoryCounter;
                i = i + 3;
                //std::cout << " Continent belonged: " << ArrayTerritories[i] + " ";

                Territory *TerrObj = new Territory(TerritoryCounter, ArrayTerritories[i - 3], mapObj->getContId(ArrayTerritories[i])); //    Destructor!

                i++;
                //std::cout << "Adj territories: ";
                while (ArrayTerritories[i].compare("|") != 0)
                {
                    //std::cout << ArrayTerritories[i] + " ";
                    i++;
                }
                if (ArrayTerritories[i].compare("|") == 0)
                {
                    //std::cout << "\n";
                    i++;
                }
                mapObj->addTerrToTerrVector(TerrObj);
                TerritoryCounter++;
                TerrObj = nullptr; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0;
            while (i < ArrayTerritories.size())
            { // Here we will create the array of ajdacent territories
                Territory *terr = (mapObj->getTerrObjByName(ArrayTerritories[i]));

                // cout << "For territ: " << ArrayTerritories[i]<<"\n";
                i = i + 4;
                while (ArrayTerritories[i].compare("|") != 0)
                {
                    // cout << *(mapObj->getTerrObjByName(ArrayTerritories[i]));
                    terr->addAdjTerr(mapObj->getTerrObjByName(ArrayTerritories[i]));
                    i++;
                }
                if (ArrayTerritories[i].compare("|") == 0)
                {
                    i++;
                }
                terr = nullptr; // Avoiding dangling pointer problems at the next initialization
            }

            i = 0;
            while (i < ArrayTerritories.size())
            { // Here we create the array of territories for each continent

                Territory *terr = (mapObj->getTerrObjByName(ArrayTerritories[i]));

                i = i + 3; // This is to access the name of the continent in the Territories data
                Continent *continent = (mapObj->getContinent(ArrayTerritories[i]));

                continent->addTerritoryToContinent(terr);
                while (ArrayTerritories[i].compare("|") != 0)
                {
                    i++;
                }
                if (ArrayTerritories[i].compare("|") == 0)
                {
                    i++;
                }
                terr = nullptr; // Avoiding dangling pointer problems at the next initialization
            }
            for (Continent *c : ContinentPointerArray)
            {
                cout << *c;
            }

            map = mapObj;
            
            mapFile.close();

            //TODO: FIND A PLACE TO DELETE THESE FKING POINTERS
            // for (Territory *terr : TerritoryPointerArray)
            // { // Memory deallocation to avoid leaks
            //     delete terr;
            //     terr = nullptr;
            // }
            //
            // for (Continent *cont : ContinentPointerArray)
            // {
            //     delete cont;
            //     cont = nullptr;
            // }
            //
            // delete mapObj;
            // mapObj = nullptr; // Handling the pointers to avoid dangling pointers
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

MapLoader::MapLoader(const MapLoader &MapLObj)
{
    ContinentCounter = 0;
    TerritoryCounter = 0;
}

Map* MapLoader::getMap()
{
    return map;
}

MapLoader &MapLoader::operator=(const MapLoader &MapLObj)
{
    ContinentCounter = MapLObj.ContinentCounter;
    TerritoryCounter = MapLObj.TerritoryCounter;
    return *this;
}

Territory::Territory(int TerrID, string TerrName, int ContID)
{
    territoryID = TerrID;
    TerritoryName = TerrName;
    ContinentId = ContID;
}
/* Important note: For the copy constructors and the overloaded assignment operators, we make it possible to have deep copies, but so far for assignment 1, we have never needed to actually use them!
This implies that even though the new keyword is written, the code is never executed so the allocation on the heap is never made. For that reason, there is no need to delete these objects and handle the pointer values (make them NULL)*/
Territory::Territory(const Territory &TerrObj)
{
    territoryID = TerrObj.territoryID;
    TerritoryName = TerrObj.TerritoryName;
    ContinentId = TerrObj.ContinentId;
    for (Territory *terri : TerrObj.arrOfAdjTerritories)
    {
        arrOfAdjTerritories.push_back(terri);
    }
}

Territory::~Territory() {}

Territory &Territory::operator=(const Territory &TerrObj)
{ // Important note: Here we define
    territoryID = TerrObj.territoryID;
    TerritoryName = TerrObj.TerritoryName;
    ContinentId = TerrObj.ContinentId;
    for (Territory *terri : TerrObj.arrOfAdjTerritories)
    {
        arrOfAdjTerritories.push_back(terri);
        //erritory *newTerritory = new Territory(*terri);
        //arrOfAdjTerritories.push_back(newTerritory);
    }
    return *this;
}

ostream &operator<<(ostream &os, const Territory &TerrObj)
{
    os << "Territory name: " + TerrObj.TerritoryName << " ID : " << TerrObj.territoryID << " Continent ID: " << TerrObj.ContinentId << "\n\n";
    return os;
}

void Territory::addAdjTerr(Territory *Terr)
{
    arrOfAdjTerritories.push_back(Terr);
}

vector<Territory*> Territory::getAdjacentTerritories()
{
    return arrOfAdjTerritories;
}

string Territory::getTerritoryName()
{
    return TerritoryName;
}

Continent::Continent(int contID, string contName, int bonus)
{
    ContinentID = contID;
    ContinentName = contName;
    Bonus = bonus;
}

Continent::Continent(const Continent &ContObj)
{
    ContinentID = ContObj.ContinentID;
    ContinentName = ContObj.ContinentName;
    Bonus = ContObj.Bonus;
    for (Territory *terri : arrOfTerrInContinent)
    {
        terri = new Territory(terri->territoryID, terri->TerritoryName, terri->ContinentId);
    }
}

Continent::~Continent() {}

Continent &Continent::operator=(const Continent &ContObj)
{
    ContinentID = ContObj.ContinentID;
    ContinentName = ContObj.ContinentName;
    Bonus = ContObj.Bonus;
    for (Territory *terri : arrOfTerrInContinent)
    {
        terri = new Territory(terri->territoryID, terri->TerritoryName, terri->ContinentId);
    }
    
    // for (Territory *terri : ContObj.arrOfTerrInContinent)
    // {
    //     Territory *newTerritory = new Territory(terri->territoryID, terri->TerritoryName, terri->territoryID);
    //     arrOfTerrInContinent.push_back(newTerritory);
    //     terri = new Territory(terri->territoryID, terri->TerritoryName, terri->ContinentId);
    // }
    
    return *this;
}

void Continent::addTerritoryToContinent(Territory *terr)
{
    arrOfTerrInContinent.push_back(terr);
}

ostream &operator<<(ostream &os, const Continent &Contobj)
{
    os << "Coninent: " + Contobj.ContinentName << "ID and Bonus are: " << Contobj.ContinentID << " and " << Contobj.Bonus << "\n\n";
    for (Territory *y : Contobj.arrOfTerrInContinent)
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
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    // These are shallow copies, gotta iterate through the array of pointers to make deep copies
    ContinentPointerArray = MapObj.ContinentPointerArray; 
    TerritoryPointerArray = MapObj.TerritoryPointerArray;
    
    // for (int i = 0; i < MapObj.ContinentPointerArray.size(); ++i)
    // {
    //     Continent *newContinent;
    //     newContinent = MapObj.ContinentPointerArray[i];
    //     ContinentPointerArray.push_back(newContinent);
    // }
    //
    // for (int i = 0; i < MapObj.TerritoryPointerArray.size(); ++i)
    // {
    //     Territory *newTerritory;
    //     newTerritory = MapObj.TerritoryPointerArray[i];
    //     TerritoryPointerArray.push_back(newTerritory);
    // }
}

Map::~Map()
{
    for (Continent *cont : ContinentPointerArray)
    {
        delete cont;
        cont = nullptr;
    }
    for (Territory *terr : TerritoryPointerArray)
    {
        delete terr;
        terr = nullptr;
    }
}

Map &Map::operator=(const Map &MapObj)
{
    // generally speaking what goes in the assignment operator is the same as the copy constructor, what might be different is checking for self assignment
    nbOfContinents = MapObj.nbOfContinents;
    nbOfTerritories = MapObj.nbOfTerritories;
    ContinentPointerArray = MapObj.ContinentPointerArray;
    TerritoryPointerArray = MapObj.TerritoryPointerArray;
    
    // for (int i = 0; i < MapObj.ContinentPointerArray.size(); ++i)
    // {
    //     Continent *newContinent = new Continent(*MapObj.ContinentPointerArray[i]);
    //     ContinentPointerArray.push_back(newContinent);
    // }
    //
    // for (int i = 0; i < MapObj.TerritoryPointerArray.size(); ++i)
    // {
    //     Territory *newTerritory = new Territory(*MapObj.TerritoryPointerArray[i]);
    //     TerritoryPointerArray.push_back(newTerritory);
    // }

    return *this;
}

void Map::addContToContVector(Continent *Cont)
{
    ContinentPointerArray.push_back(Cont);
}

void Map::addTerrToTerrVector(Territory *Terr)
{
    TerritoryPointerArray.push_back(Terr);
}
int Map::getContId(string ContName)
{
    for (Continent *x : ContinentPointerArray)
    {
        if ((x->ContinentName).compare(ContName) == 0)
        {
            return x->ContinentID;
        }
    }
    return -1;
}

Continent *Map::getContinent(string name)
{
    for (Continent *x : ContinentPointerArray)
    {
        if ((x->ContinentName).compare(name) == 0)
        {
            return x;
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
    
    if (!isContinentsconected())
    {
        cout << "Continent is not connected!" << endl;
        return false;
    }
    cout << "Continent is connected!" << endl;
    
    if (!isBelongOneContinent())
    {
        cout << "Terrtoriey does not belong to only on continent!" << endl;
        return false;
    }
    cout << "All terrtories belong to only on continent!" << endl;
    
    cout << "Map validated!" << endl;
    return true;
}

bool Map::isMapConnected()
{
    vector<string> visited;

    for (Territory *territory : TerritoryPointerArray)
    {
        DFS(territory, visited);

        for (Territory *territory : TerritoryPointerArray)
        {
            if (find(visited.begin(), visited.end(), territory->TerritoryName) == visited.end())
            {
                return false;
            }
        }
    }

    return true;
}

bool Map::isContinentsconected()
{
    for (Continent *continent : ContinentPointerArray)
    {
        vector<Territory *> &continentTerritories = continent->arrOfTerrInContinent;

        for (Territory *territory : continent->arrOfTerrInContinent)
        {
            vector<string> visited;
            continentDFS(territory, visited);

            for (Territory *territory : continentTerritories)
            {
                if (find(visited.begin(), visited.end(), territory->TerritoryName) == visited.end())
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
    for (Continent *continent : ContinentPointerArray)
    {
        for (const Territory *territory : continent->arrOfTerrInContinent)
        {
            if (territory->ContinentId != continent->ContinentID)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::continentDFS(const Territory *Terr, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), Terr->TerritoryName) != visited.end())
    {
        return;
    }

    visited.push_back(Terr->TerritoryName);

    for (const Territory *adjTerritory : Terr->arrOfAdjTerritories)
    {
        if (Terr->ContinentId == adjTerritory->ContinentId)
        {
            continentDFS(adjTerritory, visited);
        }
    }
}

void Map::DFS(const Territory *Terr, vector<string> &visited)
{
    if (find(visited.begin(), visited.end(), Terr->TerritoryName) != visited.end())
    {
        return;
    }

    visited.push_back(Terr->TerritoryName);

    for (const Territory *adjTerritory : Terr->arrOfAdjTerritories)
    {
        DFS(adjTerritory, visited);
    }
}


Territory *Map::getTerrObjByName(string TerrName)
{
    for (Territory *x : TerritoryPointerArray)
    {
        if ((x->TerritoryName).compare(TerrName) == 0)
        { // For some reason declaring the Map class as a friend works, but not declaring the function as a friend fn
            return x;
        }
    }
    return nullptr;
}

vector<Territory*> Map::getTerritories()
{
    return TerritoryPointerArray;
}

ostream &operator<<(ostream &os, const Map &mapObjPointer)
{
    for (Continent *x : mapObjPointer.ContinentPointerArray)
    {
        os << *x; // important to dereference the object as they are of type Continent* not Continent
    }
    for (Territory *y : mapObjPointer.TerritoryPointerArray)
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
