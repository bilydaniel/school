#include"simulation.hpp"
using namespace std;

class TestProcess : public Process {
 public:
  void Behavior() {
    Wait(1);
   
    Wait(5);
  }
};

int main()
{
	ifstream infile;
	string line;
	
	vector<double> inargs(14,0);
	
	
	string y;
	int x;
	//otevreni souboru
	infile.open("input.txt");
	if (!infile) 
	{
        cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    //precteni vstupnich dat
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    string a;
	    double b;
	    if (!(iss >> a >> b))  
	    	continue; 
	    if(b < 0)
	    {
	    	cerr << "Wrong value in input file";
        	exit(1); // terminate with error
	    }
	    

	    if(a.compare("num_households") == 0)
	    	inargs[NUM_HOUSEHOLDS] = b;
	    else if(a.compare("coal_max") == 0)
	    	inargs[COAL_MAX] = b;
	    else if(a.compare("core_max") == 0)
	    	inargs[CORE_MAX] = b;
	    else if(a.compare("gas_max") == 0)
	    	inargs[GAS_MAX] = b;
	    else if(a.compare("renewable_max") == 0)
	    	inargs[RENEWABLE_MAX] = b;
	    else if(a.compare("coal_max_new") == 0)
	    	inargs[COAL_MAX_NEW] = b;
	    else if(a.compare("core_max_new") == 0)
	    	inargs[CORE_MAX_NEW] = b;
	    else if(a.compare("gas_max_new") == 0)
	    	inargs[GAS_MAX_NEW] = b;
	    else if(a.compare("renewable_max_new") == 0)
	    	inargs[RENEWABLE_MAX_NEW] = b;
	    else if(a.compare("simulation_len") == 0)
	    	inargs[SIMULATION_LEN] = b;

	}
	
    infile.close();	

    long int total_places = inargs[COAL_MAX_NEW] + inargs[CORE_MAX_NEW] + inargs[GAS_MAX_NEW] + inargs[RENEWABLE_MAX_NEW];
    if(inargs[NUM_HOUSEHOLDS] > total_places)
    {
    	cerr<< "! Nedostatek elektraren !" << endl;
    	exit(1);
    }


    //inicializace simulace
    Init(SIMULATION_START_TIME, inargs[SIMULATION_LEN]); 
    Stat *co2,*cost,*coal_fuel, *core_fuel, *gas_fuel, *renewable_fuel;
    std::vector<Stat*> Stats(6);

    Stats[CO2_STAT] = co2 = new Stat("Mnozstvi vytvoreneho CO2[kg]");
    Stats[COST_STAT] = cost = new Stat("Cena vystavby elektraren[kc]");
    Stats[COAL_STAT] = coal_fuel = new Stat("Mnozstvi pouziteho hnedeho uhli[kg]");
    Stats[CORE_STAT] = core_fuel = new Stat("Mnozstvi pouziteho uranu[kg]");//TODO uranium????
    Stats[GAS_STAT] = gas_fuel = new Stat("Mnozstvi pouziteho plynu[m^3]");
    Stats[RENEWABLE_STAT] = renewable_fuel = new Stat("Mnozstvi pouziteho paliva pro elektratny obnovitelnych zdroju");//TODO??
    //vytvoreni a naplanovani simulacniho cyklu
	(new SimulationCycle(inargs, Stats))->Activate();
	//spusteni simulace
	Run();

	//SIMLIB_statistics.Output();

	cost->Output();
	cout << "Celkova cena[kc]: " << cost->Sum() << endl;
	co2->Output();
	cout << "Celkove vyprodukovane CO2[kg]: " << co2->Sum() << endl;
	coal_fuel->Output();
	cout << "Celkove pouzite hnede uhli[kg]: " << coal_fuel->Sum() << endl;
	core_fuel->Output();
	cout << "Celkove pouzity uran[kg]: " << core_fuel->Sum() << endl;
	gas_fuel->Output();
	cout << "Celkove pouzity plyn[m^3]: " << gas_fuel->Sum() << endl;
	renewable_fuel->Output();
    cout << "*****************" << endl;
    cout << "END OF SIMULATION" << endl;
	return 0;
}