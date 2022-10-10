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

		~Territory();

		Territory& operator=(const Territory& TerrObj);
		

		//methods
		void addAdjTerr(Territory* x);


		friend ostream& operator<<(ostream& os, const Territory& TerrObj);
		friend class Map;
		friend struct Continent;
};

struct Continent {
	int ContinentID;
	string ContinentName;
	int Bonus;
	vector<Territory*> arrOfTerrInContinent;

	//Constructors
	Continent(int contID, string contName, int bonus);
	Continent(const Continent& ContObj);

	//Destructor
	~Continent();

	//assignment operator
	Continent& operator=(const Continent& ContObj);

	//methods
	void addTerritoryToContinent(Territory* terr);
	
	friend ostream& operator<<(ostream& os, const Continent& Contobj);
	
	

};


class Map
{
private:
	vector<Continent*> ContinentPointerArray;//Here we're creating the arrays of pointers to Continent and Territory objects
	vector<Territory*> TerritoryPointerArray;
	int nbOfContinents;
	int nbOfTerritories;
	/*Continent* Cont;
	Territory* Terr;*/
	

public:
	//Constructors
	Map(const Map& MapObj);
	Map();

	//Destructor
	~Map();
	//Overloaded assignment operator
	Map& operator=(const Map& MapObj);

	//Methods
	void addContToContVector(Continent* Cont);
	void addTerrToTerrVector(Territory* Terr);
	int getContId(string ContName);
	Continent* getContinent(string name);
	void validate();
	bool isMapConnected();
	bool isContinentsconected();
	void DFS(const Territory* Terr, vector <string>& visited);
	void continentDFS(const Territory* Terr, vector<string>& visited);
	Territory* getTerrObjByName(string TerrName);

	

	friend class MapLoader;

	friend ostream& operator<<(ostream& os, const Map& mapObjPointer);

};

class MapLoader {
	int ContinentCounter;
	int TerritoryCounter;
	

public:
	
	void readFile(string fileName);
	MapLoader();
	MapLoader(const MapLoader& MapLObj);
	//Destructor
	~MapLoader();
	MapLoader& operator=(const MapLoader& MapLObj);
	friend ostream& operator<<(ostream& os, const MapLoader& mapLoaderObj);
	friend class Map;
	
};