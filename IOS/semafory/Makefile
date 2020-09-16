CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

all: proj2
	

proj2: main.o bus.o creator.o rider.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c 
	$(CC) $(CFLAGS) -c $^ -o $@  


bus.o: bus.c 
	$(CC) $(CFLAGS) -c $^ -o $@


creator.o: creator.c 
	$(CC) $(CFLAGS) -c $^ -o $@

rider.o: rider.c 
	$(CC) $(CFLAGS) -c $^ -o $@
