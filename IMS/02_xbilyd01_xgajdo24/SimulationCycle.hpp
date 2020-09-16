#ifndef CYCLE_HPP
#define CYCLE_HPP

#include <simlib.h>
#include <vector>
#include "Household.hpp"

#define COAL_PRIZE 79781
#define CORE_PRIZE 74948
#define GAS_PRIZE 15075
#define RENEWABLE_PRIZE 110092


class SimulationCycle : public Process
{
public:
	SimulationCycle(std::vector<double> inargs,std::vector<Stat*> Stats);
	~SimulationCycle() override;
	void Behavior() override;
private:
	unsigned int num_households; 
	std::vector<double> inargs;

	Store *coal_plants; 
	Store *core_plants;
	Store *gas_plants;
	Store *renewable_plants;

	unsigned int coal_num;
	unsigned int core_num;
	unsigned int gas_num;
	unsigned int renewable_num;

	unsigned int coal_num_new;
	unsigned int core_num_new;
	unsigned int gas_num_new;
	unsigned int renewable_num_new;

	Stat *cost;
	std::vector<Stat*> Stats;

	
};

#endif 