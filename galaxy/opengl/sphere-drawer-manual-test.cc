
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include <GL/glut.h>

#include "galaxy/base/logging.h"
#include "galaxy/base/types.h"
#include "galaxy/base/vec-type.h"
#include "galaxy/context.h"
#include "galaxy/opengl/sphere-drawer.h"
#include "galaxy/stars/base/sphere.h"
#include "galaxy/ui/sdl-window.h"

using namespace galaxy;

namespace galaxy {
namespace opengl {

constexpr float kDefaultDistance = 10000;
constexpr float kInitialVelocityY = 20;

class AffineDrawer : public RendererInterface {

public:
  AffineDrawer(const float scale, Sphere *sphere, Vec3D color)
      : scale(scale), sphere(sphere), color(color), rotate(60.0){};

  void Draw() {
    glColor3f(color.x, color.y, color.z);
    glPushMatrix();
    glTranslatef(scale * sphere->center.x, scale * sphere->center.y,
                 scale * sphere->center.z);
    glRotatef(rotate, sphere->self_rotation.x, sphere->self_rotation.y,
              sphere->self_rotation.z);
    glutSolidSphere(scale * sphere->radius, 50, 50);
    glPopMatrix();
  }

  void Update() {
    const float angle = std::atan(kInitialVelocityY / kDefaultDistance);

    // Apply affine transform to the velocity
    Vec3D new_velocity;
    new_velocity.x = sphere->velocity.x * std::cos(angle) +
                     sphere->velocity.y * -std::sin(angle);
    new_velocity.y = sphere->velocity.x * std::sin(angle) +
                     sphere->velocity.y * std::cos(angle);
    new_velocity.z = sphere->velocity.z;

    Vec3D new_rotation;
    new_rotation.x = sphere->self_rotation.x * std::cos(angle) +
                     sphere->self_rotation.y * -std::sin(angle);
    new_rotation.y = sphere->self_rotation.x * std::sin(angle) +
                     sphere->self_rotation.y * std::cos(angle);
    new_rotation.z = sphere->self_rotation.z;

    sphere->velocity = new_velocity;
    sphere->self_rotation = new_rotation;

    sphere->center.x += sphere->velocity.x;
    sphere->center.y += sphere->velocity.y;
    sphere->center.z += sphere->velocity.z;
  }

protected:
  float scale;
  Sphere *sphere;
  Vec3D color;
  float rotate;
};

} // namespace opengl
} // namespace galaxy

void mod(Context *context) {
  auto window = context->GetSdlWindow();

  std::this_thread::sleep_for(std::chrono::seconds(1));
  window->SetCaption("Some other orbiting.");
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

  Sphere *the_center = context->Borrow<Sphere>();
  the_center = new (the_center) Sphere(Vec3D(0.0, 0.0, 0.0), 1000, 10000000,
                                       Vec3D(0, 0, 0), Vec3D(0.6, 0.2, 0.4));

  Vec3D center(opengl::kDefaultDistance, 0.0f, 0.0f);
  float radius = 3000;
  float mass = 1000000;
  Vec3D rotation(0.2f, 0.8f, 0.9f);
  Vec3D velocity(0.f, opengl::kInitialVelocityY, 0.0f);

  Vec3D red(0.8, 0.2, 0.1);
  Vec3D green(0.2, 0.8, 0.1);

  Sphere *orbitor = context->Borrow<Sphere>();
  orbitor = new (orbitor) Sphere(center, radius, mass, velocity, rotation);

  opengl::SphereDrawer *center_drawer = context->Borrow<opengl::SphereDrawer>();
  center_drawer =
      new (center_drawer) opengl::SphereDrawer(1.0, the_center, green);
  opengl::AffineDrawer *orbitor_drawer =
      context->Borrow<opengl::AffineDrawer>();
  orbitor_drawer = new (orbitor_drawer) opengl::AffineDrawer(1.0, orbitor, red);

  window->RegisterRenderer(center_drawer);
  window->RegisterRenderer(orbitor_drawer);
}

// Launch the SDL window without drawing anything.
int32_t main(int32_t argc, char **argv) {

  Context c(2000, 2000, 35000.0, "Some Orbiting");
  auto window = c.GetSdlWindow();
  window->Init();
  opengl::DrawerInit(argc, argv);

  std::thread my_thread(mod, &c);
  window->MainLoop();
  my_thread.join();
}
