#include <vector>
#include <string>
#include <iostream>
using namespace std;


class Map
{

};

class MapLoader {


public:
	
	void readFile();


	
};

struct Continents {
	int ContinentID;
	std::string ContinentName;
	Continents(int contID, std::string contName);

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

