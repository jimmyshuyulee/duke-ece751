#include <iostream>

#include "il.h"

using namespace std;

void testList(void) {
  auto printList = [](IntList il) {
    for (int i = 0; i < il.size; ++i) {
      cout << il[i] << " ";
    }
  };

  IntList a;
  printList(a);
  a.remove(0);
  a.addFront(1);
  a.addFront(3);
  a.addFront(4);
  a.addBack(2);
  a.remove(1);
  a.remove(4);
  cout << a.head->data;
  cout << a.tail->data;
  IntList b = a;
  printList(b);
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
