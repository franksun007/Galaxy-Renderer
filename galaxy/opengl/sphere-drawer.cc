// TODO(Frank): Don't really know how to design the interface here.

#include "sphere-drawer.h"

#include "galaxy/base/logging.h"

#include <random>

#include <GL/gl.h>
#include <GL/glut.h>

namespace galaxy {
namespace opengl {
namespace {
constexpr int32_t kDefaultNumMesh = 20;

int32_t rotate = 60.0;
}  // namespace

void DrawerInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DrawSphere(const float scale, const Sphere& sphere, const Vec3D& color) {
    glColor3f(color.x, color.y, color.z);
    glPushMatrix();
    // TODO(Frank): Optimize this portion
    LOG(DEBUG) << sphere.center.to_string();
    glTranslatef(scale * sphere.center.x, scale * sphere.center.y, scale * sphere.center.z);
    glRotatef(rotate, sphere.self_rotation.x, sphere.self_rotation.y, sphere.self_rotation.z);
    glutSolidSphere(scale * sphere.radius, kDefaultNumMesh, kDefaultNumMesh);
    glPopMatrix();
    rotate += 3.0f;
}

}  // namespace opengl
}  // namespace galaxy

