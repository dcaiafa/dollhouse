#ifndef DOLLHOUSE_UTILS_H_
#define DOLLHOUSE_UTILS_H_

#include <stddef.h>

#pragma GCC diagnostic ignored "-Winvalid-offsetof"

#define CONTAINER_OF(ptr, type, member) ( \
    reinterpret_cast<type*>( \
        reinterpret_cast<char*>(ptr) - offsetof(type, member)));

#endif  // DOLLHOUSE_UTILS_H_

