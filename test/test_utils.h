#ifndef DOLLHOUSE_TEST_TEST_UTILS_H_
#define DOLLHOUSE_TEST_TEST_UTILS_H_

#include <stddef.h>
#include <stdio.h>

#include "linked_list.h"
#include "utils.h"

void SetTimeForTesting(unsigned long new_time);
void AdvanceTimeForTesting(unsigned long delta);

extern LinkedList* g_tests;

typedef bool (*TestFunc)();

struct Test {
 public:
  Test(const char* name, TestFunc func)
      : func_(func),
        name_(name) {
    if (!g_tests) {
      g_tests = new LinkedList;
    }
    g_tests->PushBack(&link_node_);
  }

  ~Test() {
    if (link_node_.in_list()) {
      link_node_.RemoveFromList();
    }
  }

  static Test* FromLinkNode(LinkNode* link_node) {
    return CONTAINER_OF(link_node, Test, link_node_);
  }

  bool Run() {
    return func_();
  }

  const char* name() const {
    return name_;
  }

 private:
  LinkNode link_node_;
  const char* name_;
  TestFunc func_;
};

#define TEST(x) \
    bool Test_##x(); \
    Test test_obj_##x(#x, Test_##x); \
    bool Test_##x()

#define ASSERT(x) { \
  bool result = (x); \
  if (!result) { \
    fprintf(stderr, "%s(%d): Failed: %s\n", __FILE__, __LINE__, #x); \
    return false; \
  } \
}

void RunTests(const char* filter);

#endif  // DOLLHOUSE_TEST_TEST_UTILS_H_
