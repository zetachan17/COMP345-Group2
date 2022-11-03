#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Territory
{

private:
	int territoryID;
<<<<<<< HEAD
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
=======
	string TerritoryName;

	vector<Territory *> arrOfAdjTerritories;

	int ContinentId; // Potentially to know which continent the territory belongs to?

public:
	Territory(int TerrID, std::string TerrName, int ContID);
	Territory(const Territory &TerrObj); // Copy constructor for Territory class

	~Territory();

	Territory &operator=(const Territory &TerrObj);

	// methods
	void addAdjTerr(Territory *x);

	friend ostream &operator<<(ostream &os, const Territory &TerrObj);
	friend class Map;
	friend struct Continent;
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2
};

struct Continent
{
	int ContinentID;
	string ContinentName;
	int Bonus;
	vector<Territory *> arrOfTerrInContinent;

<<<<<<< HEAD
	Continent(int contID, string contName, int bonus);
	//Copy constructor 
	Continent(const Continent& ContObj);
=======
	// Constructors
	Continent(int contID, string contName, int bonus);
	Continent(const Continent &ContObj);
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2

	// Destructor
	~Continent();

<<<<<<< HEAD
	//assignment operator
	Continent& operator=(const Continent& ContObj);
	//Stream operator overload
	friend ostream& operator<<(ostream& os, const Continent& Contobj);
	
	//methods
	void addTerritoryToContinent(Territory* terr);
	
=======
	// assignment operator
	Continent &operator=(const Continent &ContObj);

	// methods
	void addTerritoryToContinent(Territory *terr);
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2

	friend ostream &operator<<(ostream &os, const Continent &Contobj);
};

class Map
{
private:
	vector<Continent *> ContinentPointerArray; // Here we're creating the arrays of pointers to Continent and Territory objects
	vector<Territory *> TerritoryPointerArray;
	int nbOfContinents;
	int nbOfTerritories;
<<<<<<< HEAD
	
=======
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2

public:
	// Constructors
	Map(const Map &MapObj);
	Map();

	// Destructor
	~Map();
	// Overloaded assignment operator
	Map &operator=(const Map &MapObj);

<<<<<<< HEAD
	//Stream operator overload
	friend ostream& operator<<(ostream& os, const Map& mapObjPointer);
	
	//Methods
	void addContToContVector(Continent* Cont);
	void addTerrToTerrVector(Territory* Terr);
=======
	// Methods
	void addContToContVector(Continent *Cont);
	void addTerrToTerrVector(Territory *Terr);
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2
	int getContId(string ContName);
	Continent *getContinent(string name);
	void validate();
	bool isMapConnected();
	bool isContinentsconected();
<<<<<<< HEAD
	void DFS(const Territory* Terr, vector <string>& visited);
	void continentDFS(const Territory* Terr, vector<string>& visited);
	Territory* getTerrObjByName(string TerrName);

	friend class MapLoader;
=======
	bool isBelongOneContinent();
	void DFS(const Territory *Terr, vector<string> &visited);
	void continentDFS(const Territory *Terr, vector<string> &visited);
	Territory *getTerrObjByName(string TerrName);

	friend class MapLoader;

	friend ostream &operator<<(ostream &os, const Map &mapObjPointer);
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2
};

class MapLoader
{
	int ContinentCounter;
	int TerritoryCounter;

public:
	void readFile(string fileName);
	MapLoader();
<<<<<<< HEAD
	
	//Copy Constructor
	MapLoader(const MapLoader& MapLObj);
	//Destructor
	~MapLoader();
	//Overloaded assignment operator
	MapLoader& operator=(const MapLoader& MapLObj);
	//Stream operator overload
	friend ostream& operator<<(ostream& os, const MapLoader& mapLoaderObj);

=======
	MapLoader(const MapLoader &MapLObj);
	// Destructor
	~MapLoader();
	MapLoader &operator=(const MapLoader &MapLObj);
	friend ostream &operator<<(ostream &os, const MapLoader &mapLoaderObj);
>>>>>>> c0f2fe6156401c3da3fcfcdfc06a0911223374b2
	friend class Map;
};