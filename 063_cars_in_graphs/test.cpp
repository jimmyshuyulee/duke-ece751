#include "studentinterface.hpp"

int main(void) {
  Graph * g = readGraph("testing_input.txt");
  vector<intersection_id_t> path = dijkstra(g, 2, 4);
  for (auto n : path) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
