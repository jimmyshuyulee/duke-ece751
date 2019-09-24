#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main(void) {
  // Invoke an inline lambda
  []() {
    std::cout << "Hello World! This is a lambda!"
              << "\n";
  }();

  // Invoke an inline lambda with a capture
  int myVal = 5;
  [&myVal]() {
    std::cout << "Here's another lambda that captures myVal: " << myVal << "\n";
    myVal++;
  }();
  std::cout << "myVal is now " << myVal << "\n";

  // Define a func that is a lambda
  auto lambdaFunc = [&myVal]() {
    std::cout << "This is lambda func!"
              << "\n";
    myVal *= 2;
  };

  // Invoke a defined lambda manually with a capture
  auto myFunc = [myVal]() {
    std::cout
        << "This is an auto-defined lambda that can be explicitly invoked that captures "
           "myVal: "
        << myVal << "\n";
  };
  myFunc();
  lambdaFunc();

  // Use std::transform to double values in a vector
  std::vector<int> v({3, 5, 7, 9});
  std::transform(v.begin(), v.end(), v.begin(), [](int i) -> int { return i * 2; });
  for (auto num : v) {
    std::cout << num << "\n";
  }

  // Use std::transform to convert string to lowercase
  std::string s("WORLD");
  std::transform(
      s.begin(), s.end(), s.begin(), [](char c) -> char { return std::tolower(c); });
  std::cout << "Hello " << s << "\n";
  return EXIT_SUCCESS;
}
