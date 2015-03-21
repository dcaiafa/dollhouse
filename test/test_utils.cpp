#include "test_utils.h"

#include <stdio.h>
#include <string.h>

static unsigned long g_now = 0;

LinkedList* g_tests = NULL;

unsigned long micros() {
  return g_now;
}

void SetTimeForTesting(unsigned long new_time) {
  g_now = new_time;
}

void AdvanceTimeForTesting(unsigned long delta) {
  g_now += delta;
}

static bool MatchFilter(const char* filter, const char* test_name) {
  if (!filter) {
    return true;
  }
  for (size_t i = 0; ; i++) {
    if (filter[i] == '*') {
      return true;
    }
    if (filter[i] != test_name[i]) {
      return false;
    }
    if (filter[i] == '\0') {
      break;
    }
  }
  return true;
}

void RunTests(const char* filter) {
  printf("Running tests:\n");
  for (LinkNode* n = g_tests->head(); n != g_tests->end(); n = n->next) {
    Test* test = Test::FromLinkNode(n);
    if (!MatchFilter(filter, test->name())) {
      continue;
    }
    printf("  %s : ", test->name());
    if (test->Run()) {
      printf("OK\n");
    } else {
      printf("FAILED\n");
    }
  }
}
