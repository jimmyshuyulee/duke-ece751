#ifndef __NODE_H__
#define __NODE_H__
#include <stdint.h>

#include <cstdlib>
#include <queue>
#include <vector>
#define NO_SYM 999

class Node {
 public:
  unsigned sym;
  uint64_t freq;
  Node * left;
  Node * right;
  Node(unsigned s, uint64_t f) : sym(s), freq(f), left(NULL), right(NULL) {}
  Node(Node * l, Node * r) : sym(NO_SYM), freq(l->freq + r->freq), left(l), right(r) {}
  ~Node() {
    if (left != NULL) {
      delete left;
    }
    if (right != NULL) {
      delete right;
    }
  }
};
class NodePtrCompare : std::binary_function<Node *, Node *, bool> {
 public:
  bool operator()(Node * const & x, Node * const & y) const { return x->freq > y->freq; }
};
typedef std::priority_queue<Node *, std::vector<Node *>, NodePtrCompare> priority_queue_t;

Node * buildTree(uint64_t * counts);
#endif
