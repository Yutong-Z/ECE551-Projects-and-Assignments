#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <exception>
#include <iostream>
#include <stdexcept>

#include "map.h"
template<typename K, typename V>
class BstMap : public Map<K, V> {
 private:
  class Node {
   public:
    K k;
    V v;
    Node * left;
    Node * right;
    Node(K k, V v, Node * l, Node * r) : k(k), v(v), left(l), right(r) {}
  };
  Node * root;
  Node * clone(Node * curr);
  void destory(Node * curr);
  void print(Node * curr) {
    if (curr != NULL) {
      print(curr->left);
      std::cout << "(" << curr->k << ", " << curr->v << ") ";
      print(curr->right);
    }
  }

 public:
  BstMap() : root(NULL){};
  BstMap(const BstMap & rhs);
  BstMap & operator=(const BstMap & rhs);
  virtual void add(const K & key, const V & value);
  virtual const V & lookup(const K & key) const throw(std::invalid_argument);
  virtual void remove(const K & key);
  virtual ~BstMap<K, V>();
  void printTree() {
    print(root);
    std::cout << "\n";
  }
};

// destructor helper
template<typename K, typename V>
void BstMap<K, V>::destory(typename BstMap<K, V>::Node * curr) {
  if (curr != NULL) {
    destory(curr->left);
    destory(curr->right);
    delete curr;
  }
}

// destructor
template<typename K, typename V>
BstMap<K, V>::~BstMap() {
  destory(root);
  root = NULL;
}

// copy constructor helper
template<typename K, typename V>
typename BstMap<K, V>::Node * BstMap<K, V>::clone(typename BstMap<K, V>::Node * curr) {
  if (curr == NULL) {
    return NULL;
  }
  typename BstMap<K, V>::Node * newNode =
      new typename BstMap<K, V>::Node(curr->k, curr->v, NULL, NULL);
  newNode->left = clone(curr->left);
  newNode->right = clone(curr->right);
  return newNode;
}

// copy constructor
template<typename K, typename V>
BstMap<K, V>::BstMap(const BstMap<K, V> & rhs) {
  root = clone(rhs.root);
}

// = operator
template<typename K, typename V>
BstMap<K, V> & BstMap<K, V>::operator=(const BstMap & rhs) {
  if (&rhs != this) {
    BstMap<K, V> temp(rhs);
    std::swap(temp.root, root);
  }
  return *this;
}

// add
template<typename K, typename V>
void BstMap<K, V>::add(const K & key, const V & value) {
  Node ** curr = &root;
  while (*curr != NULL && key != (*curr)->k) {
    if (key < (*curr)->k) {
      curr = &(*curr)->left;
    }
    else {
      curr = &(*curr)->right;
    }
  }
  if (*curr == NULL) {
    *curr = new Node(key, value, NULL, NULL);
  }
  else {
    (*curr)->v = value;
  }
}

// lookup
template<typename K, typename V>
const V & BstMap<K, V>::lookup(const K & key) const throw(std::invalid_argument) {
  Node * curr = root;
  while (curr != NULL) {
    if (key < curr->k) {
      curr = curr->left;
    }
    else if (key > curr->k) {
      curr = curr->right;
    }
    else {
      return curr->v;
    }
  }
  throw std::invalid_argument("Invaild Key!\n");
}

// remove
template<typename K, typename V>
void BstMap<K, V>::remove(const K & key) {
  Node ** curr = &root;
  while (*curr != NULL && key != (*curr)->k) {
    if (key < (*curr)->k) {
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
    (*curr)->k = (*temp)->k;
    (*curr)->v = (*temp)->v;
    Node * lTemp = (*temp)->left;
    delete *temp;
    *temp = lTemp;
  }
}
#endif
