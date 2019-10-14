#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>

class AVLTree
{
 private:
  class AVLNode
  {
   public:
    int value;
    int height;
    AVLNode * left;
    AVLNode * right;
    int getLeftChildHeight() const { return left == nullptr ? -1 : left->height; }
    int getRightChildHeight() const { return right == nullptr ? -1 : right->height; }
    void updateHeight() { height = 1 + std::max(getLeftChildHeight(), getRightChildHeight()); }
    AVLNode(int ivalue) : value(ivalue), height(0), left(nullptr), right(nullptr) {}
    AVLNode(int ivalue, AVLNode * ileft, AVLNode * iright) :
        value(ivalue),
        left(ileft),
        right(iright) {
      updateHeight();
    }
  };

  AVLNode * root;
  AVLNode * leftRotate(AVLNode * node);
  /*  This rotation does
   *           1                 2
   *            \               / \
   *             2        ->   1   3
   *              \
   *               3
   * where 1's node is passed in, and it returns 2's node.
   */
  AVLNode * rightRotate(AVLNode * node);
  /*  This rotation does
   *         3              2
   *        /              / \
   *       2         ->   1   3
   *      /      
   *     1         
   * where 3's node is passed in, and it returns 2's node
   */
  AVLNode * leftRightRotate(AVLNode * node);
  /*  This rotation does
   *         3              2
   *        /              / \
   *       1         ->   1   3
   *        \     
   *         2      
   * where 3's node is passed in, and it returns 2's node
   */
  AVLNode * rightLeftRotate(AVLNode * node);
  /*  This rotation does
   *           1                 2
   *            \               / \
   *             3        ->   1   3
   *            /  
   *           2    
   * where 1's node is passed in, and it returns 2's node
   */

 public:
  AVLTree() : root(nullptr) {}
  void insertNode(int val);
  void deleteNode(int val);
  template<typename R>
  R preOrderFold(std::function<R(const R &, int)> const & f, const R & initial) {
    // Write your code here
  }
  ~AVLTree() { deleteTree(root); }

 private:
  AVLNode * insertHelper(AVLNode * curr, int val);
  AVLNode * deleteHelper(AVLNode * curr, int val);
  void deleteTree(AVLNode * root);
};
#endif
