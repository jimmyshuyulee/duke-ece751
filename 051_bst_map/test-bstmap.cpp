#include <iostream>

#include "bstmap.h"

int main() {
  BstMap<int, int> m;
  m.printInorder();
  m.add(10, 10);
  for (int i = 0; i < 20; ++i) {
    m.add(i, i);
  }
  m.remove(19);
  m.remove(16);
  m.add(16, 16);
  m.printInorder();
  for (int i = 5; i < 15; ++i) {
    m.remove(i);
  }
  m.printInorder();

  try {
    std::cout << m.lookup(100);
  }
  catch (const std::invalid_argument & e) {
    std::cout << "Not found " << e.what() << "...\n";
  }
  return EXIT_SUCCESS;
}
