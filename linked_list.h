#ifndef DOLLHOUSE_LINKED_LIST
#define DOLLHOUSE_LINKED_LIST

#include <stdlib.h>
#include <stdio.h>

struct LinkNode {
  LinkNode* previous;
  LinkNode* next;

  LinkNode() : previous(NULL), next(NULL) {}

  LinkNode(LinkNode* previous, LinkNode* next)
      : previous(previous),
        next(next) {}

  void InsertBefore(LinkNode* n) {
    this->next = n;
    this->previous = n->previous;
    n->previous->next = this;
    n->previous = this;
  }

  void InsertAfter(LinkNode* n) {
    this->next = n->next;
    this->previous = n;
    n->next->previous = this;
    n->next = this;
  }

  void RemoveFromList() {
    this->previous->next = this->next;
    this->next->previous = this->previous;
    this->previous = NULL;
    this->next = NULL;
  }

  bool in_list() {
    return this->previous != NULL;
  }
};

class LinkedList {
 public:
  LinkedList() : root_(&root_, &root_) {
  }

  void PushFront(LinkNode* n) {
    n->InsertAfter(&root_);
  }

  void PushBack(LinkNode* n) {
    n->InsertBefore(&root_);
  }

  LinkNode* PopFront() {
    LinkNode* n = root_.next;
    n->RemoveFromList();
    return n;
  }

  LinkNode* PopBack() {
    LinkNode* n = root_.previous;
    n->RemoveFromList();
    return n;
  }

  LinkNode* head() {
    return root_.next;
  }

  LinkNode* tail() {
    return root_.previous;
  }

  const LinkNode* end() {
    return &root_;
  }

  bool empty() const {
    return root_.next == &root_;
  }

 private:
  LinkNode root_;
};

#endif  // DOLLHOUSE_LINKED_LIST
