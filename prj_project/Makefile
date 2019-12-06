CFLAGS= -Wall -Werror -std=c++11 -pedantic -ggdb3
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TESTDIR= .

workflow: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	g++ -c $(CFLAGS) $<

test: $(TESTDIR)/task_test.cpp task.hpp
	g++ $(CFLAGS) -o task_test task_test.cpp

graph-test: $(TESTDIR)/graph_test.cpp graph.hpp
	g++ $(CFLAGS) -o graph_test graph_test.cpp

.PHONY: clean
clean:
	rm -f *.o

task.o: task.hpp
graph.o: graph.hpp task.hpp logging.hpp
workflow.o: task.hpp graph.hpp logging.hpp parser.hpp
