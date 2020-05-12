#ifndef __GALAXY_CONTEXT_H__
#define __GALAXY_CONTEXT_H__

#include <memory>
#include <mutex>

#include "galaxy/base/logging.h"
#include "galaxy/base/memory.h"
#include "galaxy/ui/sdl-window.h"

namespace galaxy {

class Context {
public:
  Context(int32_t width, int32_t height, float axis_len, const string &caption)
      : mu(), sdl_window(std::make_unique<SDLWindow>(width, height, axis_len,
                                                     caption, &mu)) {}
  ~Context() {
    for (auto mem : memory_management) {
      if (mem != nullptr) {
        Free(mem);
      }
    }
  }

  SDLWindow *GetSdlWindow() { return sdl_window.get(); }
  std::mutex *GetLock() { return &mu; }

  template <typename T> T *Borrow() {
    void *ptr = Malloc(sizeof(T));
    CHECK_NOTNULL(ptr);
    {
      std::lock_guard<std::mutex> guard(mu);
      memory_management.push_back(ptr);
    }
    return reinterpret_cast<T *>(ptr);
  }

  void *AlignedBorrow(int32_t size, int32_t alignment) {
    void *ptr = AlignedMalloc(size, alignment);
    CHECK_NOTNULL(ptr);
    {
      std::lock_guard<std::mutex> guard(mu);
      memory_management.push_back(ptr);
    }
    return ptr;
  }

private:
  std::mutex mu;
  std::unique_ptr<SDLWindow> sdl_window;

  std::vector<void *> memory_management;
};

} // namespace galaxy

#endif // __GALAXY_CONTEXT_H__