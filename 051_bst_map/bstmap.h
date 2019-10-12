#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <iostream>
#include <stdexcept>

#include "map.h"
template<typename K, typename V>
class BstMap : public Map<K, V> {
  class Node {
   public:
    K key;
    V value;
    Node * left;
    Node * right;
    Node() : key(K()), value(V()), left(NULL), right(NULL) {}
    Node(K k, V v) : key(k), value(v), left(NULL), right(NULL) {}
  };
  Node * root;

  Node ** search(const K & key) const {
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

 public:
  BstMap() : root(NULL) {}
  virtual void add(const K & key, const V & value) {
    Node ** p = search(key);
    if (*p == NULL) {
      *p = new Node(key, value);
    }
    else {
      (*p)->value = value;
    }
  }

  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node ** n = search(key);
    if (*n == NULL) {
      throw std::invalid_argument("Invalid Argument");
    }
    else {
      return (*n)->value;
    }
  }

  virtual void remove(const K & key) {
    Node ** n = search(key);
    Node ** p = n;
    if (*n != NULL) {
      if ((*n)->left == NULL) {
        p = &(*p)->right;
        std::swap(*n, *p);
      }
      else if ((*n)->right == NULL) {
        p = &(*p)->left;
        std::swap(*n, *p);
      }
      else {
        p = &(*p)->right;
        while ((*p)->left != NULL) {
          p = &(*p)->left;
        }
        std::swap((*n)->key, (*p)->key);
        std::swap((*n)->value, (*p)->value);
        n = p;
        p = &(*p)->right;
        std::swap(*n, *p);
      }
      delete *p;
    }
  }

  virtual ~BstMap<K, V>() { removeAll(root); }
  void printInorder() { printInorder(root); }
  void printInorder(Node * n) {
    if (n != NULL) {
      printInorder(n->left);
      std::cout << "(" << n->key << ", " << n->value << ") ";
      printInorder(n->right);
    }
  }
};
#endif
