#include "memory.h"

#include <limits>

#include "gtest/gtest.h"

namespace galaxy {

TEST(TestMemory, test_aligned) {
  int32_t alignment;
  void *ptr;

  alignment = 16;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);

  alignment = 32;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);

  alignment = 64;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);

  alignment = 128;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);

  alignment = 256;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);

  alignment = 1024;
  ptr = AlignedMalloc(1024, alignment);
  ASSERT_EQ(reinterpret_cast<intptr_t>(ptr) % alignment, 0UL);
  AlignedFree(ptr);
}

TEST(TestMemory, test_malloc_and_free) {
  void *ptr;
  int32_t size;

  size = 1024;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);
  
  size = 12;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 31;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 41;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 12341;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 512;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 64;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 621;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 31;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 13024;
  ptr = Malloc(size);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 4;
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  ptr = Malloc(size);
  Free(ptr);

  ptr = Malloc(std::numeric_limits<uint64_t>::max());
  ASSERT_TRUE(ptr == nullptr);
}

TEST(TestMemory, test_realloc) {
  void *ptr;
  int32_t size;

  size = 1024;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 12;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 31;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 41;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 12341;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);
  
  size = 512;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 64;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 621;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 31;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 13024;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  size = 4;
  ptr = Malloc(size);
  ptr = Realloc(ptr, size * 2);
  ASSERT_TRUE(ptr != nullptr && ptr != NULL);
  Free(ptr);

  ptr = Malloc(size);
  ptr = Realloc(ptr, std::numeric_limits<uint64_t>::max());
  ASSERT_TRUE(ptr == nullptr);
}

// void *AlignedMalloc(size_t size, int minimum_alignment) {
//   void *ptr = nullptr;
//   // posix_memalign requires that the requested alignment be at least
//   // sizeof(void*). In this case, fall back on malloc which should return
//   // memory aligned to at least the size of a pointer.
//   constexpr int required_alignment = sizeof(void *);
//   if (minimum_alignment < required_alignment)
//     return Malloc(size);
//   int err = posix_memalign(&ptr, minimum_alignment, size);
//   if (err != 0) {
//     return nullptr;
//   } else {
//     return ptr;
//   }
// }

// void AlignedFree(void *aligned_memory) { Free(aligned_memory); }

// void *Malloc(size_t size) { return malloc(size); }

// void *Realloc(void *ptr, size_t size) { return realloc(ptr, size); }

// void Free(void *ptr) { free(ptr); }

} // namespace galaxy