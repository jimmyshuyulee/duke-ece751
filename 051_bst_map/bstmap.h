#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <stdexcept>

template<typename K, typename V>
class BstMap : public Map {
  class Node {
    K key;
    V value;
    Node * left;
    Node * right;

   public:
    Node() : data(0), left(NULL), right(NULL) {}
    Node(K k, V v) : key(k), value(v), left(NULL), right(NULL) {}
  };
  Node * root;
  Node * search(const & key) {
    Node ** current = &root;
    while (*current != NULL) {
      if ((*current)->key == key) {
        return *current;
      }
      else if (key < *current->value) {
        *current = (*current)->left;
      }
      else {
        *current = (*current)->right;
      }
    }
    return NULL;
  }
  void removeAll(Node * current) {
    if (current == NULL) {
      return;
    }
    remove(current->left);
    remove(current->right);
    delete (current);
  }

 public:
  BstMap() : root(NULL) {}
  virtual void add(const K & key, const V & value) {
    Node ** current = &root;
    while (*current != NULL) {
      if (key == *current->value) {
        (*current)->value = value;
        return;
      }
      else if (key < *current->value) {
        *current = (*current)->left;
      }
      else {
        *current = (*current)->right;
      }
    }
    *current = new Node(key, value)
  }

  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node * n = search(key) if (n == NULL) { throw std::invalid_argument; }
    else {
      return n->value;
    }
  }

  virtual void remove(const K & key) {
    Node ** n = &search(key);
    if (*n != NULL) {
      if ((*n)->left == NULL) {
        std::swap(*n, (*n)->right);
      }
      else if ((*n)->right == NULL) {
        std::swap(*n, (*n)->left);
      }
      else {
        Node * temp = (*n)->right;
        while (temp->left != NULL) {
          temp = temp->left
        }
        std::swap(temp, *n);
      }
      delete *n;
    }
  }
  virtual ~Map<K, V>() { removeAll(root); }
};
#endif
