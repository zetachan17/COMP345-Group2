#include <vector>
#include <string>
#include <iostream>
using namespace std;


class Map
{
private:
	vector<Continent*>ContinentPointerArray;//Here we're creating the arrays of pointers to Continent and Territory objects
	vector<Territory*>TerritoryPointerArray;
	static int ContinentIDs;
	static int TerritoryId;

public:
	Map(const Map& CopyOfMap);
	Map();

	friend class MapLoader;

};

class MapLoader {
	int ContinentCounter;
	

public:
	
	void readFile();

	friend class Map;
	
};

struct Continent {
	int ContinentID;
	std::string ContinentName;
	Continent(int contID, std::string contName);
	Continent(const Continent& CopyOfCont);

};

class Territory {
	
	private:
	int teritorryID;
	std::string TeritorryName;
	
	vector<Territory*> arrOfAdjTerritories;
	int* IDOfContTerritoryBelongsTo;//Potentially to know which continent the territory belongs to?

	public:
		Territory(int TerrID, std::string TerrName, int ContID);
		Territory(const Territory& CopyOfTerritory);//Copy constructor for Territory class


	



};

