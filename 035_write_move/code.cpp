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

  void printIP() const {
    std::cout << id1 << "." << id2 << "." << id3 << "." << id4 << "\n";
  }

 private:
  unsigned id1, id2, id3, id4;
};

class Header {
 public:
  Header() : length(0), src(), dest() {}

  Header(unsigned char stream[]) :
      length(8),
      src(stream[0], stream[1], stream[2], stream[3]),  // first four bytes are src
      dest(stream[4], stream[5], stream[6], stream[7])  // second four are dest
  {}

  // TODO: Write copy constructor
  Header(const Header & toCopy) : {}

  // TODO: Write move constructor
  Header(Header && toMove) : {}

  // TODO: Write move assignment operator
  Header & operator=(Header && other) {}

  void printSrcDest() const {
    src.printIP();
    dest.printIP();
  }

 private:
  size_t length;
  IPAddress src;
  IPAddress dest;
};

class Packet {
 public:
  Packet(unsigned char stream[], size_t len) :
      len(len - 8),
      hdr(stream),
      data(&stream[8]) {}

  // TODO: write copy constructor
  Packet(const Packet & toCopy) : {}

  // TODO: write move constructor
  Packet(Packet && toMove) : {}

  void printInfo() const {
    hdr.printSrcDest();
    if (data != NULL) {
      std::cout << "Data: " << data << "\n";
    }
    else {
      std::cout << "Data: <empty>"
                << "\n";
    }
  }

 private:
  size_t len;
  Header hdr;
  unsigned char * data;
};

// The following code should compile and run given correct implementations
// of the copy and move constructors/operators
int main(void) {
  unsigned char raw[] = {
      0x1C, 0x13, 0x01, 0xF1, 0x1C, 0x13, 0x02, 0xF2, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x00};
  Packet temp = Packet(raw, 13);
  temp.printInfo();
  Packet && moved = Packet(std::move(temp));
  const Packet & copy = Packet(moved);
  moved.printInfo();
  temp.printInfo();
  copy.printInfo();
  return EXIT_SUCCESS;
}
