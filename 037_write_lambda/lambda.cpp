#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

// TODO 1: Write a lambda function and use it with std::sort
// to sort the elements in the vector by absolute value.
// Preconditions is that std::abs is defined on T.
template<typename T>
void sortByAbsolute(std::vector<T> & toSort) {
  std::sort(toSort.begin(), toSort.end(), [](T i, T j) -> bool {
    return std::abs(i) < std::abs(j);
  });
}

// TODO 2: Write a lambda function and use it with std::transform
// to convert every other letter starting at 0 in a string to uppercase
void everyOtherUpper(std::string & s) {
  std::transform(
      s.begin(), s.end(), s.begin(), [](char c) -> char { return std::toupper(c); });
}

// TODO 3: Write a lambda function and use it with std::transform
// to multiply each element of the vector by 2 and add 1
// storing the result in an answer vector and returning it.
std::vector<int> doublePlusOne(const std::vector<int> & src) {
  std::vector<int> ans(src);
  std::transform(
      ans.begin(), ans.end(), ans.begin(), [](int i) -> int { return i * 2 + 1; });
  return ans;
}

// TODO 4: This function takes a vector of Ts, and a function that turns
// Ts into strings.   That is, it gives some string representation (which may
// not be the "normal" one) of a T.   The function you are writing,
// uses the passed in function f to transform each element of the passed
// in vector (src) to a string, and prints those results, one per line.
// For example, if T were  Matrix, and f were
// [](const Matrix & m){ std::stringstream ss;
//      ss<< m.width << "x" << m.height <<"\n";
//      return ss.str();
//    });
// and src held a 3x5 and a 6x2 matrix, then this function would print
//
// 3x5
// 6x2
//
// because f turns a matrix into (width)x(height) as a string.
template<typename T>
void printFofVector(const std::vector<T> & src, std::function<std::string(const T &)> f) {
  //std::vector<std::string> toPrint;
  //for (int i = 0; i < src.size(); ++i) {
  //  toPrint.push_back(f(src[i]));
  //}
  for (auto e : src) {
    std::cout << f(e);
  }
}

// an example function like the one above
template<typename T>
std::string format(const T & value) {
  std::stringstream ss;
  ss << "Value: " << value << "\n";
  return ss.str();
}

int main(void) {
  std::vector<int> data({-25, -10, 16, 4, 0, 1535343, -2235356, 9879, 1, -2});
  sortByAbsolute(data);
  std::function<std::string(const int &)> print = format<int>;
  printFofVector(data, print);

  std::string s("hello world");
  everyOtherUpper(s);
  std::cout << s << "\n";

  std::vector<int> src({5, 10, 15, 20});
  std::vector<int> transformed = doublePlusOne(src);
  printFofVector(transformed, print);

  return EXIT_SUCCESS;
}
