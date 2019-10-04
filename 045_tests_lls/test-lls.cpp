#include <cassert>
#include <iostream>

#include "il.h"

using namespace std;
void testList(void) {
  // assignment
  IntList assigned;
  assigned.addFront(20);
  assigned.addFront(10);
  assigned.addBack(30);
  assigned.addBack(40);

  IntList assignee;
  assigned = assignee;
  assert(assigned.size == 0);
  assert(assigned.getSize() == 0);
  assert(assigned.head == NULL);
  assert(assigned.tail == NULL);

  // copy assignment test
  IntList l1;
  IntList cagL = l1;
  assert(cagL.head == NULL);
  assert(cagL.tail == NULL);
  assert(cagL.size == 0);
  assert(cagL.getSize() == 0);

  // copy ctor test
  IntList cpyL(l1);
  assert(cpyL.head == NULL);
  assert(cpyL.tail == NULL);
  assert(cpyL.size == 0);
  assert(cpyL.getSize() == 0);

  // test addFront()
  l1.addFront(10);  // [10]
  assert(l1.head != NULL);
  assert(l1.tail != NULL);
  assert(l1.head->prev == NULL);
  assert(l1.tail->next == NULL);
  assert(l1.head == l1.tail);
  assert(l1.size == 1);
  assert(l1.getSize() == 1);
  assert(l1[0] == 10);

  // remove only one
  bool rmvOneTrue = l1.remove(10);
  bool rmvOneFalse = l1.remove(20);
  assert(rmvOneTrue == true);
  assert(rmvOneFalse == false);
  assert(l1.size == 0);
  assert(l1.getSize() == 0);
  assert(l1.head == NULL);
  assert(l1.tail == NULL);
  l1.addFront(10);

  // test setter
  l1[0] = 100;
  assert(l1[0] == 100);  // [100]
  assert(l1.size == 1);
  assert(l1.getSize() == 1);

  // test pointer relation
  assert(l1.head->next == NULL);
  assert(l1.tail->prev == NULL);
  assert(l1.head->prev == NULL);
  assert(l1.tail->next == NULL);
  // assert(l1.head->next->prev == l1.head);
  // assert(l1.tail->prev->next == l1.tail);

  // test addBack()
  l1.addBack(20);  // [100, 20]
  assert(l1.size == 2);
  assert(l1.getSize() == 2);
  assert(l1[1] == 20);

  // test pointer relation
  assert(l1.head->next == l1.tail);
  assert(l1.tail->prev == l1.head);
  assert(l1.head->next->next == NULL);
  assert(l1.tail->prev->prev == NULL);
  assert(l1.head->next->prev == l1.head);
  assert(l1.tail->prev->next == l1.tail);

  // test remove()
  l1.addBack(30);
  l1.addBack(40);
  l1.addBack(50);  // [100, 20, 30, 40, 50]
  assert(l1.size == 5);
  assert(l1.getSize() == 5);
  assert(l1[2] == 30);
  assert(l1[3] == 40);
  assert(l1[4] == 50);
  bool rmvX = l1.remove(30);    // [100, 20, 40, 50]
  bool rmvY = l1.remove(1000);  // [100, 20, 40, 50]
  assert(rmvX == true);
  assert(rmvY == false);
  assert(l1.size == 4);
  assert(l1.getSize() == 4);
  assert(l1[2] == 40);
  assert(l1[3] == 50);

  // test remove() with same element
  l1.addFront(100);  // [100, 100, 20, 40, 50]
  l1.addBack(50);    // [100, 100, 20, 40, 50, 50]
  bool rmvA = l1.remove(100);
  bool rmvB = l1.remove(50);  // [100, 20, 40, 50]
  assert(rmvA == true);
  assert(rmvB == true);
  assert(l1.size == 4);
  assert(l1.getSize() == 4);
  assert(l1[0] == 100);
  assert(l1[1] == 20);
  assert(l1[2] == 40);
  assert(l1[3] == 50);

  // remove head and tail
  bool rmvHead = l1.remove(100);
  assert(rmvHead == true);
  l1.addFront(100);

  bool rmvTail = l1.remove(50);
  assert(rmvTail == true);
  l1.addBack(50);
  // test copy constructor
  IntList l3(l1);
  assert(l1.size == l3.size);
  assert(l1.getSize() == l3.getSize());
  assert(l1[0] == l3[0]);
  assert(l1[1] == l3[1]);
  assert(l1[2] == l3[2]);
  assert(l1[3] == l3[3]);
  assert(l3.head != NULL);
  assert(l3.tail != NULL);

  // test assignment
  IntList l4;
  l4 = l1;
  assert(l1.size == l4.size);
  assert(l1.getSize() == l4.getSize());
  assert(l1[0] == l4[0]);
  assert(l1[1] == l4[1]);
  assert(l1[2] == l4[2]);
  assert(l1[3] == l4[3]);

  // remove middle
  bool rmvMid1 = l1.remove(20);  // [100, 40, 50]
  assert(rmvMid1 == true);
  bool rmvMid2 = l1.remove(40);  // [100, 50]
  assert(rmvMid2 == true);
  bool rmvMid3 = l1.remove(9);
  assert(rmvMid3 == false);
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
