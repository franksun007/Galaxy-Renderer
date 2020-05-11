
#include <chrono>
#include <iostream>
#include <thread>

#include "galaxy/base/types.h"
#include "galaxy/base/vec-type.h"
#include "galaxy/ui/sdl-window.h"
#include "galaxy/stars/base/sphere.h"
#include "galaxy/opengl/sphere-drawer.h"

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


  // opengl::SphereDrawer drawer;

  Vec3D center(0.0f, 0.0f, 0.0f);
  float radius = 10000;
  float mass = 0.1;
  Sphere a(center, radius, mass); 

  Vec3D red(0.8, 0.2, 0.1);

  window->DrawFunc([&] { opengl::DrawSphere(1.0, a, red); });
  
}

// Launch the SDL window without drawing anything.
int32_t main(int32_t argc, char **argv) {
  auto window = SDLWindow(500, 500, 35000.0, "Some Randon Caption");
  window.Init();

  opengl::DrawerInit(argc, argv);

  std::thread my_thread(mod, &window);
  window.MainLoop();
  my_thread.join();
}
