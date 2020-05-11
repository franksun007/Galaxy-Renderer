
#include <chrono>
#include <iostream>
#include <thread>

#include "galaxy/base/logging.h"
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

  Vec3D c(0.2, 0.8, 0.1);
  Sphere the_center(Vec3D(0.0, 0.0, 0.0), 1000, 100000, Vec3D(0, 0, 0), Vec3D(0, 1, 0));
  window->DrawFunc([=] {
    LOG(DEBUG) << the_center.center.to_string();
    opengl::DrawSphere(1.0, the_center, c); 
  });

  Vec3D center(10000.0f, 0.0f, 0.0f);
  float radius = 1000;
  float mass = 0.1;
  Vec3D rotation(0.0f, 1.0f, 0.0f);
  Vec3D velocity(1.0f, 0.0f, 0.0f);

  // TODO(Frank): Really bad resources management here. Using invalid memory.
  Sphere a(center, radius, mass, velocity, rotation); 
  Vec3D red(0.8, 0.2, 0.1);
  // TODO(Frank): This won't work since you cannot change the parameters any more.
  window->DrawFunc([=] {
    LOG(DEBUG) << a.center.to_string();
    opengl::DrawSphere(1.0, a, red); 
  });

  // while (true) {
  //   a.center.x += a.velocity.x;
  //   a.center.y += a.velocity.y;
  //   a.center.z += a.velocity.z;
  // }

  // window->RenderNumFrames(600);
}

// Launch the SDL window without drawing anything.
int32_t main(int32_t argc, char **argv) {
  auto window = SDLWindow(2000, 2000, 35000.0, "Some Randon Caption");
  window.Init();

  opengl::DrawerInit(argc, argv);

  std::thread my_thread(mod, &window);
  window.MainLoop();
  my_thread.join();
}
