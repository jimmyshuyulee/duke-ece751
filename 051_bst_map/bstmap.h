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

  void copyAll(Node * tree, Node * c) {
    if (c == NULL) {
      return;
    }
    tree = new Node(c->key, c->value);
    copyAll(tree->left, c->left);
    copyAll(tree->right, c->right);
  }

  Node * removeNode(Node * root, K key) {
    if (root == NULL) {
      return root;
    }

    if (key < root->key) {
      root->left = removeNode(root->left, key);
    }
    else if (key > root->key) {
      root->right = removeNode(root->right, key);
    }
    else {
      if (root->left == NULL) {
        Node * temp = root->right;
        delete root;
        return temp;
      }
      else if (root->right == NULL) {
        Node * temp = root->left;
        delete root;
        return temp;
      }
      Node * temp = root->right;
      while (temp->left != NULL) {
        temp = temp->left;
      }
      root->key = temp->key;
      root->right = removeNode(root->right, temp->key);
    }
    return root;
  }

 public:
  BstMap() : root(NULL) {}
  BstMap(const BstMap & rhs) : root(NULL) { copyAll(root, rhs.root); }
  const BstMap & operator=(const BstMap & rhs) {
    if (&rhs != this) {
      BstMap temp(rhs);
      std::swap(root, temp.root);
    }
    return *this;
  }

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
    root = removeNode(root, key);
    /*
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
        (*n)->value = (*temp)->value;
        p = *temp;
        *temp = p->right;
      }
      delete p;
    }
*/
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
