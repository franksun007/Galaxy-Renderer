
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "sdl-window.h"

#include "galaxy/base/types.h"
#include "galaxy/base/vec-type.h"

using namespace galaxy;

void mod(SDLWindow *window) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  window->SetCaption("Some other random");
  Vec2D xy(window->GetWidth(), window->GetHeight());
  std::cout << "Window Size: " << xy.to_string() << std::endl;
  std::cout << "Camera Position: " << window->GetCamPos().to_string()
            << std::endl;
  std::cout << "Camera Orientation: " << window->GetCamOrient().to_string()
            << std::endl;
  std::cout << "Camera LookAt: " << window->GetCamLookAt().to_string()
            << std::endl;
  std::cout << std::endl;
  std::cout << "FOV: " << window->GetFOV() << std::endl;
  std::cout << "FPS: " << window->GetFPS() << std::endl;
  std::cout << std::endl;

  window->SetDrawStats(true);
}

// Launch the SDL window without drawing anything.
int32_t main(int32_t argc, char **argv) {
  std::mutex mu;
  auto window = SDLWindow(500, 500, 35000.0, "Some Randon Caption", &mu);
  window.Init();

  std::thread my_thread(mod, &window);
  window.MainLoop();
  my_thread.join();
}
