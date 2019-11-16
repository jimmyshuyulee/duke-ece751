#include "studentinterface.hpp"

int main(void) {
  std::cout << "Testing readGraph" << std::endl;
  Graph * g = readGraph("graph2.txt");
  g->printGraph();
  std::cout << std::endl;

  std::cout << "Testing dijkstra" << std::endl;

  std::cout << "Testing dijkstraAtStart" << std::endl;
  vector<intersection_id_t> path = dijkstraAtStart(g, 2, 4);
  for (auto n : path) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
