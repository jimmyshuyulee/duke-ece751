#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

class IPAddress {
 public:
  IPAddress() : id1(0), id2(0), id3(0), id4(0) {}

  IPAddress(unsigned n1, unsigned n2, unsigned n3, unsigned n4) :
      id1(n1),
      id2(n2),
      id3(n3),
      id4(n4) {}

  IPAddress(const IPAddress & toCopy) :
      id1(toCopy.id1),
      id2(toCopy.id2),
      id3(toCopy.id3),
      id4(toCopy.id4) {}

  IPAddress(IPAddress && toMove) :
      id1(toMove.id1),
      id2(toMove.id2),
      id3(toMove.id3),
      id4(toMove.id4) {
    toMove.id1 = 0;
    toMove.id2 = 0;
    toMove.id3 = 0;
    toMove.id4 = 0;
  }

  IPAddress & operator=(IPAddress && other) {
    if (this != &other) {
      id1 = other.id1;
      id2 = other.id2;
      id3 = other.id3;
      id4 = other.id4;
      other.id1 = 0;
      other.id2 = 0;
      other.id3 = 0;
      other.id4 = 0;
    }
    return *this;
  }

  void printIP() const { std::cout << id1 << "." << id2 << "." << id3 << "." << id4 << "\n"; }

 private:
  unsigned id1, id2, id3, id4;
};

int main(void) {
  IPAddress src = IPAddress(128, 3, 12, 54);
  IPAddress dest = IPAddress(128, 3, 12, 55);
  std::cout << "src: ";
  src.printIP();
  std::cout << "dest: ";
  dest.printIP();
  IPAddress && moved = IPAddress(std::move(src));
  const IPAddress & copied = IPAddress(dest);
  std::cout << "src: ";
  src.printIP();
  std::cout << "dest: ";
  dest.printIP();
  std::cout << "moved: ";
  moved.printIP();
  std::cout << "copied: ";
  copied.printIP();
  return EXIT_SUCCESS;
}
