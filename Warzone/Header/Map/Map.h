#include <vector>
#include <string>
#include <iostream>
using namespace std;





class Territory {
	
	private:
	int territoryID;
	string TeritorryName;

	
	vector<Territory*> arrOfAdjTerritories;
	int ContinentId;//Potentially to know which continent the territory belongs to?
	

	public:
		Territory(int TerrID, std::string TerrName, int ContID);
		Territory(const Territory& TerrObj);//Copy constructor for Territory class

		//methods
		void addAdjTerr(Territory* x);

		friend ostream& operator<<(ostream& os, const Territory& TerrObj);
		friend class Map;
		



};

struct Continent {
	int ContinentID;
	string ContinentName;
	int Bonus;
	vector<Territory*> arrOfTerrInContinent;

	//Constructors
	Continent(int contID, string contName, int bonus);
	
	Continent(const Continent& ContObj);

	//methods
	
	friend ostream& operator<<(ostream& os, const Continent& Contobj);
	

};


class Map
{
private:
	vector<Continent*> ContinentPointerArray;//Here we're creating the arrays of pointers to Continent and Territory objects
	vector<Territory*> TerritoryPointerArray;
	int nbOfContinents;
	int nbOfTerritories;
	Continent* Cont;
	Territory* Terr;
	

public:
	//Constructors
	Map(const Map& MapObj);
	Map();
	//Overloaded assignment operator
	Map& operator=(const Map& MapObj);

	//Methods
	void addContToContVector(Continent* Cont);
	void addTerrToTerrVector(Territory* Terr);
	int getContId(string ContName);
	Territory* getTerrObjByName(string TerrName);

	

	friend class MapLoader;

	friend ostream& operator<<(ostream& os, const Map& mapObjPointer);

};

class MapLoader {
	int ContinentCounter;
	int TerritoryCounter;
	

public:
	
	void readFile();

	friend class Map;
	
};