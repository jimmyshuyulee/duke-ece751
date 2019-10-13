#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <iostream>
#include <stdexcept>

#include "set.h"
template<typename T>
class BstSet : public Set<T> {
  class Node {
   public:
    T key;
    Node * left;
    Node * right;
    Node() : key(T()), left(NULL), right(NULL) {}
    Node(T k) : key(k), left(NULL), right(NULL) {}
  };
  Node * root;

  Node ** search(const T & key) const {
    Node ** current = const_cast<Node **>(&root);
    while (*current != NULL) {
      if ((*current)->key == key) {
        break;
      }
      else if (key < (*current)->key) {
        current = &(*current)->left;
      }
      else {
        current = &(*current)->right;
      }
    }
    return current;
  }

  void removeAll(Node * current) {
    if (current == NULL) {
      return;
    }
    removeAll(current->left);
    removeAll(current->right);
    delete (current);
  }

  void copyAll(Node ** tree, Node * c) {
    if (c == NULL) {
      return;
    }
    *tree = new Node(c->key);
    copyAll(&(*tree)->left, c->left);
    copyAll(&(*tree)->right, c->right);
  }

 public:
  BstSet() : root(NULL) {}
  BstSet(const BstSet & rhs) : root(NULL) { copyAll(&root, rhs.root); }
  const BstSet & operator=(const BstSet & rhs) {
    if (&rhs != this) {
      BstSet temp(rhs);
      std::swap(root, temp.root);
    }
    return *this;
  }

  virtual void add(const T & key) {
    Node ** p = search(key);
    if (*p == NULL) {
      *p = new Node(key);
    }
  }

  virtual bool contains(const T & key) const {
    Node ** n = search(key);
    return (*n != NULL);
  }

  virtual void remove(const T & key) {
    Node ** n = search(key);
    Node * p = *n;
    if (*n != NULL) {
      if ((*n)->left == NULL) {
        *n = p->right;
      }
      else if ((*n)->right == NULL) {
        *n = p->left;
      }
      else {
        Node ** temp = &(*n)->right;
        while ((*temp)->left != NULL) {
          temp = &(*temp)->left;
        }
        (*n)->key = (*temp)->key;
        p = *temp;
        *temp = p->right;
      }
      delete p;
    }
  }

  virtual ~BstSet<T>() { removeAll(root); }
  void printInorder() { printInorder(root); }
  void printInorder(Node * n) {
    if (n != NULL) {
      printInorder(n->left);
      std::cout << n->key << " ";
      printInorder(n->right);
    }
  }
};
#endif
