#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>

class Thing {
  std::string name;

 public:
  Thing(std::string n) : name(n) { std::cout << "Create a thing: " << name << std::endl; }
  std::string getName() { return name; }
  void setName(std::string new_name) { name = new_name; }
  void printName(const char * nm) { std::cout << nm << " is " << name << std::endl; }
  ~Thing() { std::cout << "Delete a thing: " << name << std::endl; }
};

std::unique_ptr<Thing> foo() {
  return std::unique_ptr<Thing>(std::make_unique<Thing>("C"));
}

void print_up(const char * nm, std::unique_ptr<Thing> & up) {
  if (up == nullptr) {
    std::cout << nm << " is nullptr\n";
  }
  else {
    up->printName(nm);
  }
}

void other_print(const char * nm, std::unique_ptr<Thing> up) {
  if (up == nullptr) {
    std::cout << nm << " is nullptr\n";
  }
  else {
    up->printName(nm);
  }
}

int main() {
  std::unique_ptr<Thing> p1(std::make_unique<Thing>("A"));
  auto p2(std::make_unique<Thing>("B"));  //what type is p2?
  p1->printName("p1");
  p2 = std::move(p1);
  print_up("p1", p1);
  print_up("p2", p2);
  Thing * p3 = p2.release();
  print_up("p3", p2);
  print_up("p2", p2);
  p1.reset(p3);
  print_up("p1", p2);
  print_up("p3", p2);
  p2 = foo();
  print_up("p2", p2);
  other_print("other p2", std::move(p2));
  print_up("p2", p2);
  std::cout << "done!" << std::endl;
  return 0;
}
