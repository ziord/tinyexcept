#ifndef TINYEXCEPT_DEFS_H
#define TINYEXCEPT_DEFS_H

#include "tinyexcept_def.h"

extern Exception BadAlloc;
extern Exception BadDataRequest;

unsigned int get_data(unsigned int x);
void* allocate_mem(long long size);

#endif