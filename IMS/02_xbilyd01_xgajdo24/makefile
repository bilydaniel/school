CC=g++
CFLAGS=
DEPS = simulation.hpp


	


%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simulation: simulation.o SimulationCycle.o Household.o
	$(CC) -o simulation simulation.o SimulationCycle.o Household.o -lsimlib -lm
