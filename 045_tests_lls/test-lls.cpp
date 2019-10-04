#include <cassert>
#include <iostream>

#include "il.h"

using namespace std;

void testList(void) {
  IntList l1;
  l1.addFront(1);
  assert(l1.head->data == 1);
  assert(l1.tail->data == 1);
  assert(l1.tail == l1.head);
  assert(l1.size == 1);
  assert(l1.getSize() == 1);
  l1.addBack(2);
  assert(l1.head->data == 1);
  assert(l1.tail->data == 2);
  assert(l1.tail != l1.head);
  assert(l1.head->next != l1.tail);

  IntList l2(l1);
  /*
  a.remove(0);
  a.addFront(1);
  a.addFront(3);
  a.addFront(4);
  a.addBack(2);
  a.remove(1);
  a.remove(4);
  IntList b = a;
*/
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
