#include <vector>
#include <string>
#include <iostream>
using namespace std;





class Territory {
	
	private:
	int territoryID;
	string TeritorryName;
	vector<Territory*> arrOfAdjTerritories;
	int ContinentId;
	

	public:
		Territory(int TerrID, std::string TerrName, int ContID);
		//Copy constructor 
		Territory(const Territory& TerrObj);
		//Destructor
		~Territory();
		//Assignment operator overload
		Territory& operator=(const Territory& TerrObj);
		//Stream operator overload
		friend ostream& operator<<(ostream& os, const Territory& TerrObj);

		//methods
		void addAdjTerr(Territory* x);
		friend class Map;
		friend struct Continent;
};

struct Continent {
	int ContinentID;
	string ContinentName;
	int Bonus;
	vector<Territory*> arrOfTerrInContinent;

	Continent(int contID, string contName, int bonus);
	//Copy constructor 
	Continent(const Continent& ContObj);

	//Destructor
	~Continent();

	//assignment operator
	Continent& operator=(const Continent& ContObj);
	//Stream operator overload
	friend ostream& operator<<(ostream& os, const Continent& Contobj);
	
	//methods
	void addTerritoryToContinent(Territory* terr);
	

};


class Map
{
private:
	vector<Continent*> ContinentPointerArray;//Here we're creating the arrays of pointers to Continent and Territory objects
	vector<Territory*> TerritoryPointerArray;
	int nbOfContinents;
	int nbOfTerritories;
	

public:
	//Constructors
	Map(const Map& MapObj);
	Map();

	//Destructor
	~Map();
	//Overloaded assignment operator
	Map& operator=(const Map& MapObj);

	//Stream operator overload
	friend ostream& operator<<(ostream& os, const Map& mapObjPointer);
	
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
};

class MapLoader {
	int ContinentCounter;
	int TerritoryCounter;
	

public:
	
	void readFile(string fileName);
	MapLoader();
	
	//Copy Constructor
	MapLoader(const MapLoader& MapLObj);
	//Destructor
	~MapLoader();
	//Overloaded assignment operator
	MapLoader& operator=(const MapLoader& MapLObj);
	//Stream operator overload
	friend ostream& operator<<(ostream& os, const MapLoader& mapLoaderObj);

	friend class Map;
	
};