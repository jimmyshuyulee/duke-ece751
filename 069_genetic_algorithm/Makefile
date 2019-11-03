CXXFLAGS=--std=gnu++11 -Wall -g -pedantic -pthread
CPPFILES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(CPPFILES))

main: $(OBJS)
	g++ $(CXXFLAGS) -o main $(OBJS)
clean:
	rm *.o main

.PHONY: main
