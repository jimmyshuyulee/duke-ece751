#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

std::string sayHello() {
  return "Hello World";
}

std::string sayHi() {
  return "Hi World";
}

void printReference(const std::string & s) {
  std::cout << "Called lvalue reference: " << s << "\n";
}

void printReference(std::string && s) {
  std::cout << "Called mutable rvalue reference: " << s << "\n";
}

int main(void) {
  std::string foobar("foobar");
  printReference(sayHello());
  printReference(foobar);
  printReference(sayHi());

  const std::string & hi = sayHi();
  std::string && hello = sayHello();
  printReference(hi);
  printReference(hello);
  printReference(std::move(hello));
  return EXIT_SUCCESS;
}
