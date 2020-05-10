
#include "memory.h"

#include <cstdlib>

namespace galaxy {

void *AlignedMalloc(size_t size, int32_t minimum_alignment) {
  void *ptr = nullptr;
  // posix_memalign requires that the requested alignment be at least
  // sizeof(void*). In this case, fall back on malloc which should return
  // memory aligned to at least the size of a pointer.
  constexpr int required_alignment = sizeof(void *);
  if (minimum_alignment < required_alignment)
    return Malloc(size);
  int err = posix_memalign(&ptr, minimum_alignment, size);
  if (err != 0) {
    return nullptr;
  } else {
    return ptr;
  }
}

void AlignedFree(void *aligned_memory) { Free(aligned_memory); }

void *Malloc(size_t size) { return malloc(size); }

void *Realloc(void *ptr, size_t size) { return realloc(ptr, size); }

void Free(void *ptr) { free(ptr); }

} // namespace galaxy