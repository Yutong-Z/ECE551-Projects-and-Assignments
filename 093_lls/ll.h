#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdlib>
#include <exception>

template<typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node * next;
    Node * prev;
    Node(const T & d) : data(d), next(NULL), prev(NULL){};
    Node(const T & d, Node * n, Node * p) : data(d), next(n), prev(p){};
  };
  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0){};
  void addFront(const T & item) {
    head = new Node(item, head, NULL);
    if (tail == NULL) {
      tail = head;
    }
    else {
      head->next->prev = head;
    }
    size++;
  }

  void addBack(const T & item) {
    tail = new Node(item, NULL, tail);
    if (head == NULL) {
      head = tail;
    }
    else {
      tail->prev->next = tail;
    }
    size++;
  }

  bool remove(const T & item) {
    Node ** curr = &head;
    while (*curr != NULL && (*curr)->data != item) {
      curr = &(*curr)->next;
    }
    if (*curr == NULL) {
      return 0;
    }
    else {
      Node * temp = *curr;
      if ((*curr)->next != NULL) {
        (*curr)->next->prev = (*curr)->prev;
      }
      else {  // removed item is the last item
        tail = NULL;
      }
      *curr = (*curr)->next;
      delete temp;
      size--;
      return 1;
    }
  }

  T & operator[](int index) {
    assert(index >= 0 && index < size);
    Node * curr = head;
    int i = 0;
    while (i != index) {
      i++;
      curr = curr->next;
    }
    return curr->data;
  }

  const T & operator[](int index) const {
    assert(index >= 0 && index < size);
    Node * curr = head;
    int i = 0;
    while (i != index) {
      i++;
      curr = curr->next;
    }
    return curr->data;
  }

  int find(const T & item) {
    for (int i = 0; i < size; i++) {
      if ((*this)[i] == item) {
        return i;
      }
    }
    return -1;
  }

  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(rhs.size) {
    head = new Node(rhs[0], NULL, NULL);
    Node * curr = head;
    for (int i = 1; i < rhs.size; i++) {
      curr->next = new Node(rhs[i], NULL, curr);
      curr = curr->next;
    }
    tail = curr;
  }

  LinkedList & operator=(const LinkedList & rhs) {
    if (this != &rhs) {
      Node * temp = new Node(rhs[0], NULL, NULL);
      Node * curr = temp;
      for (int i = 1; i < rhs.size; i++) {
        curr->next = new Node(rhs[i], NULL, curr);
        curr = curr->next;
      }
      while (head != NULL) {
        Node * temp1 = head->next;
        delete head;
        head = temp1;
      }
      head = temp;
      tail = curr;
      size = rhs.size;
    }
    return *this;
  }

  ~LinkedList() {
    while (head != NULL) {
      Node * temp = head->next;
      delete head;
      head = temp;
    }
  }

  int getSize() const { return size; }
};

#endif
