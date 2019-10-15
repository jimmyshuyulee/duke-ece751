#include <iostream>

#include "avl_tree.hpp"

int main(void) {
  AVLTree a;
  a.insertNode(3);
  a.insertNode(5);
  a.insertNode(1);
  a.insertNode(6);
  a.insertNode(7);
  a.insertNode(4);
  a.insertNode(2);
  a.deleteNode(3);
  int i = a.preOrderFold<int>(
      [](const int & sum, int data) -> int {
        std::cout << data << std::endl;
        return data;
      },
      0);
  return EXIT_SUCCESS;
}
