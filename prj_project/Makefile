CFLAGS= -Wall -Werror -std=c++11 -pedantic -ggdb3
TASKDIR= .

workflow: workflow.cpp graph.hpp task.hpp logging.hpp parser.hpp
	g++ $(CFLAGS) -o workflow workflow.cpp

test: $(TASKDIR)/task_test.cpp task.hpp
	g++ $(CFLAGS) -o task_test task_test.cpp

graph-test: $(TASKDIR)/graph_test.cpp graph.hpp
	g++ $(CFLAGS) -o graph_test graph_test.cpp

.PHONY: clean
clean:
	rm -f test  *.o
