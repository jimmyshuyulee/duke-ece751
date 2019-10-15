#include "avl_tree.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>

AVLTree::AVLNode * AVLTree::leftRotate(AVLTree::AVLNode * node) {
  AVLNode * temp = node->right;
  node->right = temp->left;
  temp->left = node;
  node->updateHeight();
  temp->updateHeight();
  return temp;
}
/*  This rotation does
   *           1                 2
   *            \               / \
   *             2        ->   1   3
   *              \
   *               3
   * where 1's node is passed in, and it returns 2's node.
   */
AVLTree::AVLNode * AVLTree::rightRotate(AVLNode * node) {
  AVLNode * temp = node->left;
  node->left = temp->right;
  temp->right = node;
  node->updateHeight();
  temp->updateHeight();
  return temp;
}

/*  This rotation does
   *         3              2
   *        /              / \
   *       2         ->   1   3
   *      /      
   *     1         
   * where 3's node is passed in, and it returns 2's node
   */
AVLTree::AVLNode * AVLTree::leftRightRotate(AVLNode * node) {
  node->left = leftRotate(node->left);
  return rightRotate(node);
}
/*  This rotation does
   *         3              2
   *        /              / \
   *       1         ->   1   3
   *        \     
   *         2      
   * where 3's node is passed in, and it returns 2's node
   */
AVLTree::AVLNode * AVLTree::rightLeftRotate(AVLNode * node) {
  node->right = rightRotate(node->right);
  return leftRotate(node);
}
/*  This rotation does
   *           1                 2
   *            \               / \
   *             3        ->   1   3
   *            /  
   *           2    
   * where 1's node is passed in, and it returns 2's node
   */

void AVLTree::insertNode(int val) {
  root = insertHelper(root, val);
}
void AVLTree::deleteNode(int val) {
  root = deleteHelper(root, val);
}

AVLTree::AVLNode * AVLTree::insertHelper(AVLNode * curr, int val) {
  if (curr == NULL) {
    return new AVLNode(val);
  }

  if (val < curr->value) {
    curr->left = insertHelper(curr->left, val);
  }
  else if (val > curr->value) {
    curr->right = insertHelper(curr->right, val);
  }
  curr->updateHeight();

  if (curr->getLeftChildHeight() - curr->getRightChildHeight() > 1) {
    curr = (curr->getLeftChildHeight() > curr->getRightChildHeight())
               ? rightRotate(curr)
               : leftRightRotate(curr);
  }
  else if (curr->getRightChildHeight() - curr->getLeftChildHeight() > 1) {
    curr = (curr->getRightChildHeight() > curr->getLeftChildHeight())
               ? leftRotate(curr)
               : rightLeftRotate(curr);
  }
  curr->updateHeight();
  return curr;
}

AVLTree::AVLNode * AVLTree::deleteHelper(AVLNode * curr, int val) {
  if (curr == NULL) {
    return curr;
  }

  if (val < curr->value) {
    curr->left = deleteHelper(curr->left, val);
  }
  else if (val > curr->value) {
    curr->right = deleteHelper(curr->right, val);
  }
  else {
    if (curr->left == NULL || curr->right == NULL) {
      AVLNode * temp = (curr->left == NULL) ? curr->right : curr->left;
      std::swap(curr, temp);
      delete temp;
    }
    else {
      AVLNode * p = curr->right;
      while (p->left != NULL) {
        p = p->left;
      }
      std::swap(curr->value, p->value);
      curr->right = deleteHelper(curr->right, val);
    }
  }

  if (curr != NULL) {
    curr->updateHeight();
    if (curr->getLeftChildHeight() - curr->getRightChildHeight() > 1) {
      curr = (curr->left->getLeftChildHeight() > curr->left->getRightChildHeight())
                 ? rightRotate(curr)
                 : leftRightRotate(curr);
    }
    else if (curr->getRightChildHeight() - curr->getLeftChildHeight() > 1) {
      curr = (curr->right->getRightChildHeight() > curr->right->getLeftChildHeight())
                 ? leftRotate(curr)
                 : rightLeftRotate(curr);
    }
    curr->updateHeight();
  }
  return curr;
}

void AVLTree::deleteTree(AVLNode * root) {
  if (root != NULL) {
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
  }
}

void AVLTree::copyTree(AVLNode ** root, AVLNode * c) {
  if (c == NULL) {
    return;
  }
  *root = new AVLNode(c->value);
  copyTree(&(*root)->left, c->left);
  copyTree(&(*root)->right, c->right);
  c->updateHeight();
}

