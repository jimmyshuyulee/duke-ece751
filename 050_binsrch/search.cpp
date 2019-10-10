#include <iostream>

#include "function.h"
int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (high - low < 2) {
    return low;
  }
  int mid = (high + low) / 2;
  if (f->invoke(mid) > 0) {
    return binarySearchForZero(f, low, mid);
  }
  else {
    return binarySearchForZero(f, mid, high);
  }
}
