#include <vector>
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::vector;

class Territory
{
private:
	int territoryId;
	string territoryName;
	vector<Territory*> adjacentTerritories;
	int continentId; 

public:
	// constructors
	Territory(int territoryId, string territoryName, int continentId);
	Territory(const Territory &territory); 

	//destructor
	~Territory();

	// assignment operator
	Territory &operator=(const Territory &territory);

	// stream output operator
	friend ostream& operator<<(ostream& os, const Territory& territory);

	// adds a territory to the list of adjacent territories
	void addAdjacentTerritory(Territory *territory);

	// getters
	int getTerritoryId();
	string getTerritoryName();
	vector<Territory*> getAdjacentTerritories();
	int getContinentId();

	friend class Map;
	friend class Continent;
};

class Continent
{
private:
	int continentId;
	string continentName;
	int bonus;
	vector<Territory*> territories;

public:
	// constructors
	Continent(int continentId, string continentName, int bonus);
	Continent(const Continent &continent);

	// destructor
	~Continent();

	// assignment operator
	Continent &operator=(const Continent &continent);

	// stream output operator
	friend ostream& operator<<(ostream& os, const Continent& continent);

	// adds a territory to the list of territory in this continent
	void addTerritoryToContinent(Territory *territory);

	friend class Map;
	friend class Territory;

};

class Map
{
private:
	vector<Continent*> continents; 
	vector<Territory*> territories;
	int numberOfContinents;
	int numberOfTerritories;

public:
	// constructors
	Map(const Map &map);
	Map();

	// destructor
	~Map();

	// assignment operator
	Map &operator=(const Map &map);

	// output stream operator
	friend ostream& operator<<(ostream& os, const Map& map);

	// REQUIRED - validates the map is a connected graph, continents are connected subgraphs and each territory belongs
	// to one and only one continent
	void validate();
	
private:
	// adds continent to the map
	void addContinent(Continent *continent);

	// adds territory to the map
	void addTerritory(Territory *territory);

	// gets the id of the continent matching the passed name or returns -1
	int getContinentId(string continentName);

	// gets the continent matching the passed name
	Continent* getContinentByName(string continentName);

	// gets the territory maching the passed name
	Territory* getTerritoryByName(string territoryName);

	// verifies that the map is a connected graph
	bool isMapConnected();

	// verifies that the continents are connected subgraphs
	bool isContinentsConnected();

	// 
	bool isBelongOneContinent();
	void DFS(const Territory *territory, vector<string> &visited);
	void continentDFS(const Territory *territory, vector<string> &visited);
	
	friend class MapLoader;
};

class MapLoader
{
	int ContinentCounter;
	int TerritoryCounter;

public:
	// constructors
	MapLoader();
	MapLoader(const MapLoader &mapLoader);

	// destructor
	~MapLoader();

	// assignment operator
	MapLoader& operator=(const MapLoader &mapLoader);

	// output stream operator
	friend ostream& operator<<(ostream &os, const MapLoader &mapLoader);

	// --additional functions--
	void readFile(string fileName);
	
	friend class Map;
};