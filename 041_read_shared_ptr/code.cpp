#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>
class Thing
{
  std::string name;

 public:
  Thing(std::string n) : name(n) { std::cout << "Create a thing: " << name << std::endl; }
  std::string getName() { return name; }
  void setName(std::string new_name) { name = new_name; }
  void printName() { std::cout << name << std::endl; }
  ~Thing() { std::cout << "Delete a thing: " << name << std::endl; }
};

void f1(std::shared_ptr<Thing> & sp) {
  std::cout << "sp& with " << sp->getName() << " with count " << sp.use_count() << "\n";
}

void f2(std::shared_ptr<Thing> sp) {
  std::cout << "sp with " << sp->getName() << " with count " << sp.use_count() << "\n";
}

void g1(std::weak_ptr<Thing> & wp) {
  if (std::shared_ptr<Thing> sp = wp.lock()) {
    std::cout << "wp& has " << sp->getName() << " with count " << sp.use_count() << "\n";
  }
  else {
    std::cout << "nothing there\n";
  }
}

void g2(std::weak_ptr<Thing> wp) {
  if (std::shared_ptr<Thing> sp = wp.lock()) {
    std::cout << "wp has " << sp->getName() << " with count " << sp.use_count() << "\n";
  }
  else {
    std::cout << "nothing there\n";
  }
}

void fish(std::shared_ptr<Thing> p) {
  p->setName(p->getName() + " fish");
}

int main() {
  std::shared_ptr<Thing> p1(std::make_shared<Thing>("red"));
  f1(p1);
  f2(p1);
  std::weak_ptr<Thing> wp1(p1);
  g1(wp1);
  fish(p1);
  g2(wp1);
  std::shared_ptr<Thing> p2(p1);
  f1(p1);
  g1(wp1);
  p1 = std::make_shared<Thing>("blue");
  f2(p1);
  f2(p2);
  g2(wp1);
  p2 = std::make_shared<Thing>("cat");
  g2(wp1);
  return 0;
}
