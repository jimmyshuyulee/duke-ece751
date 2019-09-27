#include <cstdlib>

#include "poly.hpp"

int main(void) {
  Polynomial<double> p1;
  p1.addTerm(4, 4);
  p1.addTerm(2, 3);
  p1.addTerm(-7, 2);
  p1.addTerm(-9, 0);
  std::cout << "p1: " << p1 << "\n";
  double z1 = p1.findZero(-1.5, 5, 0.0001);
  std::cout << "First zero: " << z1 << "\n";
  double z2 = p1.findZero(2, 10, 0.0001);
  std::cout << "Second zero: " << z2 << "\n";
  try {
    double zbogus = p1.findZero(0, 20, 0.00001);
    std::cout << "Should not have gotten: " << zbogus << "\n";
  }
  catch (convergence_failure<double> & cf) {
    std::cout << "Failed to converge. Last x was " << cf.value << "\n";
  }
  try {
    double zbogus = p1.findZero(0.0001, 20, 0.00001);
    std::cout << "Should not have gotten: " << zbogus << "\n";
  }
  catch (convergence_failure<double> & cf) {
    std::cout << "Failed to converge. Last x was " << cf.value << "\n";
  }
  double z3 = p1.findZero(0.0001, 35, 0.00001);
  std::cout << "But will converge to " << z3 << " with more steps\n";
  return EXIT_SUCCESS;
}
