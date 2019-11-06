#include <stdlib.h>

#include <iostream>
#include <string>

class D;

class A {
 public:
  int x;
  A() { std::cout << "Construct A: this = " << this << "\n"; }
  virtual void printMe() { std::cout << "An A this= " << this << "\n"; }
  virtual void aThing() {
    std::cout << "A thing this = " << this << "\n";
    printMe();
  }
  virtual ~A() { std::cout << "Destruct A this= " << this << " \n"; }
};

class B : virtual public A {
 public:
  int y;
  int z;
  B() { std::cout << "Construct B: this = " << this << "\n"; }
  virtual void printMe() { std::cout << "A B this= " << this << "\n"; }
  virtual void bThing() {
    std::cout << "B thing this = " << this << "\n";
    printMe();
  }
  virtual ~B() { std::cout << "Destruct B this= " << this << " \n"; }
};

class C : virtual public A {
 public:
  int f1;
  C() { std::cout << "Construct C: this = " << this << "\n"; }
  virtual void printMe() { std::cout << "A C this= " << this << "\n"; }
  virtual void cThing() {
    std::cout << "C thing this = " << this << "\n";
    printMe();
  }
  virtual ~C() { std::cout << "Destruct C this= " << this << " \n"; }
};

class D : public C, public B {
 public:
  int f2;
  int f3;
  D() { std::cout << "Construct D: this = " << this << "\n"; }
  virtual void printMe() { std::cout << "A D this= " << this << "\n"; }
  virtual ~D() { std::cout << "Destruct D this= " << this << " \n"; }
};

int main() {
  std::cout << "sizeof A:" << sizeof(A) << "\n";
  std::cout << "sizeof B:" << sizeof(B) << "\n";
  std::cout << "sizeof C:" << sizeof(C) << "\n";
  std::cout << "sizeof D:" << sizeof(D) << "\n";
  D d1;  //assume &d1 = 0x1000
  d1.aThing();
  d1.bThing();
  d1.cThing();
  B * bptr = &d1;
  std::cout << "B:" << bptr << "\n";
  bptr->bThing();
  bptr->aThing();
  C * cptr = &d1;
  std::cout << "C:" << cptr << "\n";
  cptr->cThing();
  cptr->aThing();
  A * aptr = &d1;
  std::cout << "A:" << aptr << "\n";
  aptr->aThing();

  return EXIT_SUCCESS;
}
