#include <cassert>
#include <iostream>

#include "ll.h"

using namespace std;

void testList() {
  // add elements, default constructor
  LinkedList<int> a = LinkedList<int>();
  assert(a.head == NULL && a.tail == NULL && a.size == 0);

  // add head to empty
  a.addFront(20);
  assert(a.head == a.tail && a.head != NULL);
  assert(a.head->prev == NULL && a.head->next == NULL);
  assert(a[0] == 20);

  a.addFront(10);
  a.addBack(30);
  a.addBack(40);
  assert(a.tail != NULL && a.head != NULL);
  assert(a.size == 4 && a.getSize() == 4);
  assert(a.tail->data == 40 && a.head->data == 10);

  // remove from head
  assert(a.remove(10) == true && a.remove(100) == false);
  assert(a.head->data == 20 && a.tail->data == 40);
  assert(a.head->prev == NULL);
  assert(a.size == 3 && a.getSize() == 3);

  //remove from middle
  assert(a.remove(30) == true);
  assert(a.head->next->data == 40 && a.tail->prev->data == 20);
  assert(a.size == 2 && a.getSize() == 2);

  // remove from tail
  assert(a.remove(40) == true);
  assert(a.head->data == 20 && a.tail == a.head);
  assert(a.tail->next == NULL);
  assert(a.size == 1 && a.getSize() == 1);

  // remove head from single node
  assert(a.remove(20));
  assert(a.head == NULL && a.tail == NULL);
  // add back to empty
  a.addBack(200);
  assert(a.tail == a.head && a.tail->data == 200);
  assert(a.tail->prev == NULL && a.tail->next == NULL);
  // remove tail from single node
  assert(a.remove(200));
  assert(a.head == NULL && a.tail == NULL);

  a.addFront(20);
  a.addFront(10);
  a.addBack(30);
  a.addBack(40);

  // initialize d as a copy of a
  LinkedList<int> d(a);
  assert(d.head != a.head && d.tail != a.tail);
  assert(d.head->data == a.head->data && d.tail->data == a.tail->data);
  assert(d.head->next != a.head->next && d.head->next->data == a.head->next->data);
  assert(d.tail->prev != a.tail->prev && d.tail->prev->data == a.tail->prev->data);

  // deep copy
  d = a;
  assert(d.head != a.head && d.tail != a.tail);
  assert(d.head->data == a.head->data && d.tail->data == a.tail->data);
  assert(d.head->next != a.head->next && d.head->next->data == a.head->next->data);
  assert(d.tail->prev != a.tail->prev && d.tail->prev->data == a.tail->prev->data);

  // assign empty to a
  LinkedList<int> b;
  a = b;
  // 10 copy assignment
  // 13 not clear
  assert(a.head == NULL && a.tail == NULL);
  assert(a.size == 0 && a.getSize() == 0);

  // initialize c from empty b
  // 11 copy constructor
  LinkedList<int> c(b);
  assert(c.head == NULL && c.tail == NULL);
  assert(c.size == 0 && c.getSize() == 0);

  int alen = a.getSize();
  assert(a.size == alen);
  cout << "The length of a is: " << alen << endl;
  for (int i = 0; i < alen; i++) {
    cout << a[i] << " ";
  }
  cout << endl;

  return;
}

int main() {
  testList();
  return EXIT_SUCCESS;
}

