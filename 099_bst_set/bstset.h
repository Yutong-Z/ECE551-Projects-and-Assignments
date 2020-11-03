#ifndef __BSTSET_H__
#define __BSTSET_H__
#include <iostream>

#include "set.h"
template<typename T>
class BstSet : public Set<T> {
 private:
  class Node {
   public:
    T key;
    Node * left;
    Node * right;
    Node(T data) : key(data), left(NULL), right(NULL){};
  };
  Node * root;
  Node * clone(Node * curr) {
    if (curr == NULL) {
      return NULL;
    }
    Node * newNode = new Node(curr->key);
    newNode->left = clone(curr->left);
    newNode->right = clone(curr->right);
    return newNode;
  }
  void destory(Node * curr) {
    if (curr != NULL) {
      destory(curr->left);
      destory(curr->right);
      delete curr;
    }
  }
  void print(Node * curr) {
    if (curr != NULL) {
      print(curr->left);
      std::cout << curr->key << " ";
      print(curr->right);
    }
  };

 public:
  BstSet() : root(NULL){};
  BstSet(const BstSet & rhs) { root = clone(rhs.root); }
  BstSet & operator=(const BstSet & rhs) {
    if (&rhs != this) {
      BstSet temp(rhs);
      std::swap(temp.root, root);
    }
    return *this;
  }
  virtual void add(const T & key) {
    Node ** curr = &root;
    while (*curr != NULL && key != (*curr)->key) {
      if (key < (*curr)->key) {
        curr = &(*curr)->left;
      }
      else {
        curr = &(*curr)->right;
      }
    }
    if (*curr == NULL) {
      *curr = new Node(key);
    }
  }
  virtual bool contains(const T & key) const {
    Node * curr = root;
    while (curr != NULL) {
      if (key < curr->key) {
        curr = curr->left;
      }
      else if (key > curr->key) {
        curr = curr->right;
      }
      else {
        return 1;
      }
    }
    return 0;
  }
  virtual void remove(const T & key) {
    Node ** curr = &root;
    while (*curr != NULL && key != (*curr)->key) {
      if (key < (*curr)->key) {
        curr = &(*curr)->left;
      }
      else {
        curr = &(*curr)->right;
      }
    }
    if (*curr == NULL) {
      return;
    }
    if ((*curr)->left == NULL) {
      Node * temp = (*curr)->right;
      delete *curr;
      *curr = temp;
    }
    else if ((*curr)->right == NULL) {
      Node * temp = (*curr)->left;
      delete *curr;
      *curr = temp;
    }
    else {
      Node ** temp = &(*curr)->left;
      while ((*temp)->right != NULL) {
        temp = &(*temp)->right;
      }
      (*curr)->key = (*temp)->key;
      Node * temptemp = (*temp)->left;
      delete *temp;
      *temp = temptemp;
    }
  }
  virtual ~BstSet<T>() {
    destory(root);
    root = NULL;
  }
  void printTree() {
    print(root);
    std::cout << "\n";
  }
};

#endif
