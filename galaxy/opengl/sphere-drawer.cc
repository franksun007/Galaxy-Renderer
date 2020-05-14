
#include "sphere-drawer.h"

#include "galaxy/base/logging.h"

#include <memory>
#include <random>

#include <GL/gl.h>
#include <GL/glut.h>

namespace galaxy {
namespace opengl {
namespace {
constexpr int32_t kDefaultNumMesh = 20;
constexpr float kDefaultRotate = 60.0;
} // namespace

void DrawerInit(int argc, char **argv) {
  glutInit(&argc, argv);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

SphereDrawer::SphereDrawer(const float scale, const Sphere& sphere, const Vec3D& color)
    : scale(scale), sphere(sphere), color(color), rotate(kDefaultRotate){};
  
SphereDrawer* SphereDrawer::Create(void* desired_memory, const float scale, const Sphere& sphere, const Vec3D& color) {
  if (desired_memory != nullptr) {
    return new (desired_memory) SphereDrawer(scale, sphere, color);
  } else {
    return new SphereDrawer(scale, sphere, color);
  }
}

void SphereDrawer::Draw() {
  glColor3f(color.x, color.y, color.z);
  glPushMatrix();
  // TODO(Frank): Optimize this portion
  glTranslatef(scale * sphere.center.x, scale * sphere.center.y,
               scale * sphere.center.z);
  glRotatef(rotate, sphere.self_rotation.x, sphere.self_rotation.y,
            sphere.self_rotation.z);
  glutSolidSphere(scale * sphere.radius, kDefaultNumMesh, kDefaultNumMesh);
  glPopMatrix();
}

void SphereDrawer::Update() {
  rotate += 1.0f;
  sphere.center.x += sphere.velocity.x;
  sphere.center.y += sphere.velocity.y;
  sphere.center.z += sphere.velocity.z;
}

} // namespace opengl
} // namespace galaxy
