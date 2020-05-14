#ifndef __GALAXY_CONTEXT_H__
#define __GALAXY_CONTEXT_H__

#include <memory>
#include <mutex>
#include <unordered_map>

#include "absl/strings/str_format.h"

#include "interface-object-registry.h"
#include "galaxy/base/logging.h"
#include "galaxy/base/memory.h"
#include "galaxy/ui/sdl-window.h"

namespace galaxy {

class Context {

struct AllocatedMemory {
  void * ptr = nullptr;
  bool requested_aligned = false;
  int32_t requested_aligned_size = 0;
  size_t used_size = 0;
  size_t total_size = 0; 

  string to_string() const {
    return absl::StrFormat("[0x%X, RA: %d, AS: %d, used: %d, total: %d]", reinterpret_cast<const uintptr_t>(ptr), (const int32_t)(requested_aligned), requested_aligned_size, used_size, total_size);
  }
};

public:
  Context(int32_t width, int32_t height, float axis_len, const string &caption)
      : mu(), sdl_window(std::make_unique<SDLWindow>(width, height, axis_len,
                                                     caption)), memory_management() {}
  ~Context() {
    for ( auto it = memory_management.begin(); it != memory_management.end(); ++it ) {
      LOG(DEBUG) << it->second.ptr;
      Free(it->second.ptr);
    }
  }

  SDLWindow *GetSdlWindow() { return sdl_window.get(); }
  std::mutex *GetLock() { return &mu; }

  template <typename T, INTERFACE_OBJECT object> 
  T *Allocate() {
    if (object == INTERFACE_OBJECT::SphereRenderer) {
    LOG(DEBUG) << 1;
    }
    
    std::lock_guard<std::mutex> guard(mu);

    void* ret_ptr = nullptr;

    const int32_t requested_size = sizeof(T);

    auto got = memory_management.find(object);
    if (got == memory_management.end()) {
      AllocatedMemory amem;
      const size_t total_size = static_cast<size_t>(std::round(requested_size * 1.5));
      amem.ptr = Malloc(total_size);
      CHECK_NOTNULL(amem.ptr);
      amem.total_size = total_size;
      amem.used_size = requested_size;
      memory_management.emplace(object, amem);
      ret_ptr = amem.ptr;
      LOG(DEBUG) << amem.to_string();
    } else {
      AllocatedMemory& amem = got->second;
      const size_t new_used_size = amem.used_size + requested_size;
      if (new_used_size > amem.total_size) {
        size_t new_size = static_cast<size_t>(std::round(new_used_size * 1.5));
        amem.ptr = Realloc(amem.ptr, new_size);
        CHECK_NOTNULL(amem.ptr);
        amem.total_size = new_size;
        LOG(DEBUG) << amem.to_string();
      }
      ret_ptr = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(amem.ptr) + amem.used_size);
      amem.used_size += requested_size;
    } 

    LOG(DEBUG) << ret_ptr;

    return reinterpret_cast<T *>(ret_ptr);
  }

private:
  std::mutex mu;
  std::unique_ptr<SDLWindow> sdl_window;

  std::unordered_map<INTERFACE_OBJECT, AllocatedMemory> memory_management;
};

} // namespace galaxy

#endif // __GALAXY_CONTEXT_H__