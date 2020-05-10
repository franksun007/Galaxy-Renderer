
#ifndef __GALAXY_BASE_MEMORY_H__
#define __GALAXY_BASE_MEMORY_H__

#include "types.h"

namespace galaxy {
// Aligned allocation/deallocation. `minimum_alignment` must be a power of 2
// and a multiple of sizeof(void*).
void *AlignedMalloc(size_t size, int32_t minimum_alignment);
void AlignedFree(void *aligned_memory);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void Free(void *ptr);
} // namespace galaxy

#endif // __GALAXY_BASE_MEMORY_H__