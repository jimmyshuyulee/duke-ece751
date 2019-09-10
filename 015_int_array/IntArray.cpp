#include "IntArray.h"

#include <assert.h>

#include <iostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}

IntArray::IntArray(int n) : numElements(n) {
  data = new int[n]();
}

IntArray::IntArray(const IntArray & rhs) : numElements(rhs.numElements) {
  data = new int[rhs.numElements]();
  for (int i = 0; i < rhs.numElements; i++) {
    data[i] = rhs.data[i];
  }
}

IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (this != &rhs) {
    int * temp = new int[rhs.numElements]();
    for (int i = 0; i < rhs.numElements; i++) {
      temp[i] = rhs.data[i];
    }
    delete[] data;
    data = temp;
    numElements = rhs.numElements;
  }
  return *this;
}

const int & IntArray::operator[](int index) const {
  assert(index < numElements);
  return data[index];
}

int & IntArray::operator[](int index) {
  assert(index < numElements);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (rhs.numElements != numElements) {
    return false;
  }
  for (int i = 0; i < numElements; i++) {
    if (rhs.data[i] != data[i]) {
      return false;
    }
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(rhs == *this);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  std::cout << "{" << rhs[0];
  for (int i = 1; i < rhs.size(); i++) {
    std::cout << ", " << rhs[i];
  }
  std::cout << "}";
  return s;
}
