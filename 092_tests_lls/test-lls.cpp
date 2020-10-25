#include <assert.h>

#include "il.h"

void testList(void) {
  // test default constructor
  IntList * il1 = new IntList();
  assert(il1->head == NULL && il1->tail == NULL && il1->size == 0);
  int s = 0;
  assert(il1->getSize() == s);
  assert(il1->find(1) == -1);

  // test addFront
  il1->addFront(1);
  s++;
  assert(il1->head->data == 1);
  assert(il1->head->prev == NULL && il1->tail->next == NULL);
  assert(il1->head == il1->tail);
  assert(il1->size == s);
  assert((*il1)[0] == 1);
  assert(il1->getSize() == s);

  // test addBack
  il1->addBack(2);
  s++;
  assert(il1->tail->data == 2 && il1->head->data == 1);
  assert(il1->tail->next == NULL && il1->head->prev == NULL);
  assert(il1->head->next->next == NULL && il1->tail->prev->prev == NULL);
  assert(il1->head->next == il1->tail && il1->tail->prev == il1->head);
  assert((*il1)[0] == 1 && (*il1)[1] == 2);
  assert(il1->size == s);
  assert(il1->getSize() == s);

  // test find
  assert(il1->find(3) == -1);
  assert(il1->find(2) == 1);
  assert(il1->find(1) == 0);

  // test remove il1: 1->2
  il1->addBack(9);
  il1->remove(9);
  assert(il1->head->next->next == NULL);
  assert(il1->remove(12) == 0);  // remove something not exits in list
  assert(il1->remove(1) == 1);
  s--;
  assert(il1->head != NULL);
  assert(il1->head == il1->tail && il1->head->next == NULL);
  assert(il1->size == s);
  assert(il1->remove(2) == 1);  // remove the only one element of list
  assert(il1->head == NULL && il1->tail == NULL);
  assert(il1->size == 0);
  s--;
  assert(il1->getSize() == s);
  assert(il1->remove(2) == 0);  //remove from an empty list

  // remove the first appearance
  il1->addBack(1);
  il1->addBack(2);
  il1->addBack(1);
  il1->addBack(12);

  assert(il1->remove(1) == 1);
  assert((*il1)[0] == 2 && (*il1)[1] == 1 && (*il1)[2] == 12);
  assert(il1->find(1) == 1);
  assert(il1->getSize() == 3);

  assert(il1->remove(1) == 1);
  assert((*il1)[0] == 2 && (*il1)[1] == 12);
  assert(il1->find(1) == -1);
  assert(il1->getSize() == 2);

  delete il1;

  // test copy constructor
  IntList * il2 = new IntList();
  il2->addFront(1);
  il2->addBack(2);    // il2: 1->2
  IntList il3(*il2);  // il3: 1->2
  assert((*il2)[0] == il3[0] && (*il2)[1] == il3[1]);
  assert(il2->head->data == il3.head->data && il2->tail->data == il3.tail->data);
  assert(il2->size == il3.size);
  (*il2)[0] = 3;  // il2 change to: 3->2
  assert(il2->head->data == 3);
  assert((*il2)[0] == 3 && il3[0] == 1);
  delete il2;
  assert(il3[1] == 2);
  assert(il3.head->next->next == NULL && il3.head->next == il3.tail &&
         il3.tail == il3.tail->prev->next);

  // test = assignment operator
  IntList il4;
  il4 = il3;  // il4: 1->2
  assert(il4[0] == il3[0] && il4[1] == il3[1]);
  assert(il4.head->data == il3.head->data && il4.tail->data == il3.tail->data);
  assert(il4.size == il3.size);
  assert(il4[0] == 1 && il4[1] == 2);
  assert(il4.head->next->next == NULL && il4.head->next == il4.tail &&
         il4.tail == il4.tail->prev->next);
  il4[0] = 3;
  assert(il4[0] == 3 && il3[0] == 1);

  // test destructor
  il4.~IntList();
  il3.~IntList();
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
