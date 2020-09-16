#include "simulation.hpp"
using namespace std;
#include <unistd.h>
SimulationCycle::SimulationCycle(vector <double> inargs,vector<Stat*> Stats)
{
	this->num_households = inargs[NUM_HOUSEHOLDS];
	this->inargs = inargs;
	
	this->coal_plants = new Store(inargs[COAL_MAX_NEW]);
	this->core_plants = new Store(inargs[CORE_MAX_NEW]);
	this->gas_plants = new Store (inargs[GAS_MAX_NEW]);
	this->renewable_plants = new Store(inargs[RENEWABLE_MAX_NEW]);

	this->coal_num = inargs[COAL_MAX]; 
	this->core_num = inargs[CORE_MAX];
	this->gas_num = inargs[GAS_MAX];
	this->renewable_num = inargs[RENEWABLE_MAX];

	this->coal_num_new = inargs[COAL_MAX_NEW]; 
	this->core_num_new = inargs[CORE_MAX_NEW];
	this->gas_num_new = inargs[GAS_MAX_NEW];
	this->renewable_num_new = inargs[RENEWABLE_MAX_NEW];
	
	cost = Stats[COST_STAT];
	this->Stats = Stats;
}	

void SimulationCycle::Behavior()
{
	//spocitat cenu postaveni elektraren
	
	//pocet elektraren k postaveni (prepocitano na cloveka)
	int coal_diff = this->coal_num_new - this->coal_num;
	int core_diff = this->core_num_new - this->core_num;
	int gas_diff = this->gas_num_new - this->gas_num;
	int renewable_diff = this->renewable_num_new - this->renewable_num;
	
	double coal_cost = coal_diff * COAL_PRIZE;
	double core_cost = core_diff * CORE_PRIZE;
	double gas_cost = gas_diff * GAS_PRIZE;
	double renewable_cost = renewable_diff * RENEWABLE_PRIZE;
	double total_cost = coal_cost + core_cost + gas_cost + renewable_cost;
	//ulozeni hodnoy do statistik
	(*cost)(total_cost);
	

	
	//spocitat procenta jednotlivych elektraren
	int total = this->coal_num_new + this->core_num_new + this->gas_num_new + this->renewable_num_new;
	this->inargs[COAL_PERCENTAGE] = (this->coal_num_new / (double)total)*100;
	this->inargs[CORE_PERCENTAGE] = (this->core_num_new / (double)total)*100;
	this->inargs[GAS_PERCENTAGE] = (this->gas_num_new / (double)total)*100;
	this->inargs[RENEWABLE_PERCENTAGE] = (this->renewable_num_new / (double)total)*100;
	//vygenerovani domacnosti
	int households = this->num_households;
	while(households>0)
	{
		vector<Store*> plants;
		plants.push_back(this->coal_plants);
		plants.push_back(this->core_plants);
		plants.push_back(this->gas_plants);
		plants.push_back(this->renewable_plants);
		srand (time(NULL));
		(new Household(this->inargs, plants, Stats))->Activate();
		
		households--;
	}
}

SimulationCycle::~SimulationCycle()
{
	//cost->Output();
	//fuel_cost->Output();
}