#include <iostream>

#include "bstmap.h"

int main() {
  BstMap<int, int> m;
  m.printInorder();
  for (int i = 0; i < 20; ++i) {
    m.add(i, i);
  }
  m.remove(20);
  m.remove(17);
  m.add(17, 17);
  m.remove(18);
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
