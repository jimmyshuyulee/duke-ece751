#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdlib>
#include <exception>
#include <iostream>

//YOUR CODE GOES HERE
template<typename T>
class LinkedList {
 private:
  class itemNotFoundException : public std::exception {
    virtual const char * what() const throw() { return "testing"; }
  };

  class Node {
   public:
    T data;
    Node * next;
    Node * prev;
    Node() : data(0), next(NULL), prev(NULL){};
    Node(T d) : data(d), next(NULL), prev(NULL){};
  };

  Node * head;
  Node * tail;
  int size;
  void linkHandler(Node * n1, Node * n2) {
    if (n1 != NULL) {
      n1->next = n2;
    }
    if (n2 != NULL) {
      n2->prev = n1;
    }
  }
  void print() {
    for (Node * p = head; p != NULL; p = p->next) {
      std::cout << p->data << "  ";
    }
  }

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}

  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
    for (Node * p = rhs.head; p != NULL; p = p->next) {
      (*this).addBack(p->data);
    }
  }

  LinkedList & operator=(const LinkedList & rhs) {
    if (this != &rhs) {
      LinkedList temp(rhs);
      std::swap(temp.head, head);
      std::swap(temp.tail, tail);
      size = temp.size;
    }
    return *this;
  }

  ~LinkedList() {
    while (head != NULL) {
      Node * temp = head;
      head = head->next;
      delete temp;
    }
  }

  void addFront(const T & item) {
    Node * temp = new Node(item);
    linkHandler(temp, head);
    if (head == NULL) {
      tail = temp;
    }
    head = temp;
    ++size;
  }

  void addBack(const T & item) {
    Node * temp = new Node(item);
    linkHandler(tail, temp);
    if (tail == NULL) {
      head = temp;
    }
    tail = temp;
    ++size;
  }

  bool remove(const T & item) {
    Node * current = head;
    while (current != NULL && current->data != item) {
      current = current->next;
    }
    if (current == NULL) {
      return false;
    }

    linkHandler(current->prev, current->next);
    if (current == head) {
      head = head->next;
    }
    if (current == tail) {
      tail = tail->prev;
    }
    delete current;
    --size;
    return true;
  }

  T & operator[](int index) {
    if (index < 0 || index >= size) {
      throw itemNotFoundException();
    }
    Node * p = head;
    for (int i = 0; i < index; ++i) {
      p = p->next;
    }
    return p->data;
  }

  const T & operator[](int index) const {
    if (index < 0 || index >= size) {
      throw itemNotFoundException();
    }
    Node * p = head;
    for (int i = 0; i < index; ++i) {
      p = p->next;
    }
    return p->data;
  }

  int find(const T & item) {
    Node * p = head;
    for (int i = 0; i < size; ++i) {
      if (p->data == item) {
        return i;
      }
      p = p->next;
    }
    return -1;
  }

  int getSize() const { return size; }
  friend void testList(void);
};

#endif
