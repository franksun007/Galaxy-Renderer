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
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

std::default_random_engine engine;

void UpdateColor(const Vec3D& color) {
    if (PREDICT_FALSE(r + g + b == 0)) {
        r = static_cast<uint8_t>(std::round(color.x * 255));
        g = static_cast<uint8_t>(std::round(color.y * 255));
        b = static_cast<uint8_t>(std::round(color.z * 255));
    } else {
        int32_t rand = engine() % 100;
        r = (rand + r) % 255;
        g = (rand - 1 + g) % 255;
        b = (rand + 1 + b) % 255;
    }
}

}  // namespace

void DrawerInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DrawSphere(const float scale, const Sphere& sphere, const Vec3D& color) {

    LOG(DEBUG) << Vec3D(r, g, b).to_string();

    UpdateColor(color);
    glColor3f(r / 255.0, g / 255.0, b / 255.0);
    glPushMatrix();
    // TODO(Frank): Optimize this portion
    glTranslatef(scale * sphere.center.x, scale * sphere.center.y, scale * sphere.center.z);
    glRotatef(rotate, rotate / 60.0f, 0, 0);
    glRotatef(rotate, 0, rotate / 60.0f, rotate / 60.0f);   
    glutSolidSphere(scale * sphere.radius, kDefaultNumMesh, kDefaultNumMesh);
    glPopMatrix();

    rotate += 3.0f;
}

}  // namespace opengl
}  // namespace galaxy

