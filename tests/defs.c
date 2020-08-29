#include "defs.h"

Exception BadAlloc = {"Bad allocation size"};
Exception BadDataRequest = {"Bad data request"};

unsigned int get_data(unsigned int x) {
    if (x > 0 && x < 50) {
        return x & 1u;
    } else {
        raise(BadDataRequest);
    }
}

void *allocate_mem(long long size) {
    void *mem = malloc(size);
    if (mem) return mem;
    else {
        raise(BadAlloc);
    }
}