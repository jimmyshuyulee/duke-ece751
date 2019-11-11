CFLAGS= -Wall -Werror -std=gnu++11 -pedantic -ggdb3
sim-dijkstra-start:studentinterface.o
	g++ $(CFLAGS) -o sim-dijkstra-start studentinterface.cpp -L . -lcars
studentinterface.o:studentinterface.cpp studentinterface.hpp
	g++ $(CFLAGS) -c studentinterface.cpp
test:test.cpp studentinterface.o
	g++ $(CFLAGS) -o test test.cpp studentinterface.o

.PHONY: clean
clean:
	rm -f test sim-dijkstra-start *.o
