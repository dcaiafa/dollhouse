#include <stdio.h>
#include <string.h>

#include "test_utils.h"

int UsageError(const char* error) {
  fprintf(stderr, "Error: %s\n", error);
  return 1;
}

int main(int argc, char** argv) {
  const char* filter = NULL;

  for (int i = 1; i < argc; i++) {
    const char* arg = argv[i];
    const char* param = i < (argc - 1) ? argv[i + 1] : NULL;

    if (arg[0] == '-' && arg[1] == '-') {
      arg += 2;

      if (strcmp(arg, "filter") == 0) {
        if (!param) {
          return UsageError("--filter requires a value");
        }
        filter = param;
        i++;
      } else {
        return UsageError("Invalid switch");
      }
    } else {
      return UsageError("Invalid parameter");
    }
  }

  RunTests(filter);
  return 0;
}
