#all: d6r 
	#@sudo ./d6r -s fd17:625c:f037:2:e595:9a81:d562:528  #   2001:4860:4860::8888
	

d6r : d6r.o
	@g++ -o d6r d6r.o -lpcap


d6r.o: d6r.cpp
	@g++ -c d6r.cpp 

test:
	
