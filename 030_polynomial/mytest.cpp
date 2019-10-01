#include <cstdlib>

#include "poly.hpp"

int main(void) {
  Polynomial<int> p1;
  std::cout << "default poly: " << p1 << "\n";
  p1.addTerm(4, 4);
  p1.addTerm(2, 3);
  p1.addTerm(-7, 2);
  p1.addTerm(-9, 0);
  std::cout << "p1: " << p1 << "\n";
  std::cout << "-p1: " << -p1 << "\n";
  Polynomial<float> p2;
  p2.addTerm(6, 6);
  p2.addTerm(-3, 3);
  p2.addTerm(4, 0);
  std::cout << "p2: " << p2 << "\n";
  std::cout << "p1 + p2: " << p1 + p2 << "\n";
  std::cout << "p1 * p2: " << p1 * p2 << "\n";
  std::cout << "p1 - p2: " << p1 - p2 << "\n";
  Polynomial<Polynomial<int> > p3;
  p3.addTerm(p1, 0);
  p3.addTerm(p2, 1);
  std::cout << "p3: " << p3 << "\n";
  /*
  Polynomial<Polynomial<int> > p4;
  p4.addTerm(p1, 1);
  p4.addTerm(p2, 2);
  std::cout << "p4: " << p4 << "\n";
  std::cout << "p3 + p4: " << p3 + p4 << "\n";
  std::cout << "p3 * p4: " << p3 * p4 << "\n";
  std::cout << "p3 - p4: " << p3 - p4 << "\n";
*/
  //need to test:
  //==, !=
  return EXIT_SUCCESS;
}
