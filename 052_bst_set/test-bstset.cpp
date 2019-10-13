#include <iostream>

#include "bstset.h"

int main() {
  BstSet<int> m;
  m.printInorder();
  m.add(10);
  for (int i = 0; i < 20; ++i) {
    m.add(i);
  }
  m.remove(19);
  m.remove(16);
  m.add(16);
  m.printInorder();
  for (int i = 5; i < 15; ++i) {
    m.remove(i);
  }
  m.printInorder();

  std::cout << m.contains(100);
  return EXIT_SUCCESS;
}
