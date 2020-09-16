#include "simulation.hpp"
using namespace std;
Household::Household(vector <double> inargs, vector<Store*> plants, std::vector<Stat*> Stats)
{
	this->inargs = inargs;

	this->coal_plants = plants[0];
	this->core_plants = plants[1];
	this->gas_plants = plants[2];
	this->renewable_plants = plants[3];

	this->coal_num_new = inargs[COAL_MAX_NEW]; 
	this->core_num_new = inargs[CORE_MAX_NEW];
	this->gas_num_new = inargs[GAS_MAX_NEW];
	this->renewable_num_new = inargs[RENEWABLE_MAX_NEW];

	this->Stats = Stats;
}	

void Household::Behavior()
{	
	
	while(1)
	{
		double coal_percent = this->inargs[COAL_PERCENTAGE]; 
		double core_percent = this->inargs[CORE_PERCENTAGE];
		double gas_percent = this->inargs[GAS_PERCENTAGE];
		double renewable_percent = this->inargs[RENEWABLE_PERCENTAGE];

		float rand_num;

		double co2_produced;
		int choice;
		double fuel_used;
		while(1)
		{
			rand_num = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100-1)));
			if(rand_num <= coal_percent)
			{
				if(this->coal_plants->Full())
					continue;
				else
				{
					Enter(*this->coal_plants,1);
					choice = 1;
					co2_produced = 6650;//kg
					fuel_used = 7978;//kg
					break;
				}
			}
			else if(rand_num <= coal_percent + core_percent)
			{
				if(this->core_plants->Full())
					continue;
				else
				{
					Enter(*this->core_plants,1);
					choice = 2;
					co2_produced = 0;//kg		
					fuel_used = 0.00738;//kg
					break;
				}
			}
			else if(rand_num <= coal_percent + core_percent + gas_percent)
			{
				if(this->gas_plants->Full())
					continue;
				else
				{
					Enter(*this->gas_plants,1);
					choice = 3;
					co2_produced = 565;//kg
					fuel_used = 308;
					break;
				}
			}
			else
			{
				if(this->renewable_plants->Full())
					continue;
				else
				{
					Enter(*this->renewable_plants,1);
					choice = 4;
					co2_produced = 0;
					fuel_used = 0;
					break;
				}
			}

		}	
			
		Wait(1);//1 rok
		
		//vratit elektrarnu
		if(choice == 1)
		{
			(*this->Stats[COAL_STAT])(fuel_used);
			Leave(*this->coal_plants,1);
		}
		else if(choice == 2)
		{
			(*this->Stats[CORE_STAT])(fuel_used);
			Leave(*this->core_plants,1);
		}
		else if(choice == 3)
		{
			(*this->Stats[GAS_STAT])(fuel_used);
			Leave(*this->gas_plants,1);
		}
		else if(choice == 4)
		{
			(*this->Stats[RENEWABLE_STAT])(fuel_used);
			Leave(*this->renewable_plants,1);
		}


		(*this->Stats[CO2_STAT])(co2_produced);
	}
	
}