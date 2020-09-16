#ifndef HOUSE_HPP
#define HOUSE_HPP

#include <simlib.h>
#include <vector>


class Household : public Process
{
public:
	Household(std::vector<double> inargs, std::vector<Store*> plants, std::vector<Stat*> Stats);
	void Behavior() override;
private:
	
	Store *coal_plants; 
	Store *core_plants;
	Store *gas_plants;
	Store *renewable_plants;

	unsigned int coal_num_new;
	unsigned int core_num_new;
	unsigned int gas_num_new;
	unsigned int renewable_num_new;

	std::vector<Stat*> Stats;

	std::vector<double> inargs;
};

#endif 