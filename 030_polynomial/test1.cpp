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
  Polynomial<int> p2;
  p2.addTerm(6, 6);
  p2.addTerm(-3, 3);
  p2.addTerm(4, 0);
  std::cout << "p2: " << p2 << "\n";
  std::cout << "p1 + p2: " << p1 + p2 << "\n";
  std::cout << "p1 * p2: " << p1 * p2 << "\n";
  std::cout << "p1 - p2: " << p1 - p2 << "\n";
  Polynomial<int> p3;
  p3.addTerm(4, 0);
  p3.addTerm(5, 1);
  std::cout << "p3: " << p3 << "\n";
  Polynomial<int> p4;
  p4.addTerm(3, 0);
  p4.addTerm(2, 1);
  std::cout << "p4: " << p4 << "\n";
  std::cout << "p3 * p4: " << p3 * p4 << "\n";
  p3 += p4;
  std::cout << "p3v2: " << p3 << "\n";
  p3 *= p4;
  std::cout << "p3v3: " << p3 << "\n";
  p3 -= p4;
  std::cout << "p3v4: " << p3 << "\n";
  p4 *= 3;
  std::cout << "p4v2: " << p4 << "\n";
  Polynomial<int> p5;
  p5.addTerm(6, 1);
  p5.addTerm(9, 0);
  std::cout << "p5: " << p5 << "\n";
  std::cout << "p5 == p4? " << (p5 == p4) << "\n";
  std::cout << "p5 == p5? " << (p5 == p5) << "\n";
  std::cout << "p5 == p1? " << (p5 == p1) << "\n";

  //need to test:
  //==, !=
  return EXIT_SUCCESS;
}
