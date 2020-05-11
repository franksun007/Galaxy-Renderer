// TODO(Frank): Don't really know how to design the interface here.
#ifndef __GALAXY_OPENGL_SPHERE_H__
#define __GALAXY_OPENGL_SPHERE_H__

#include "galaxy/base/vec-type.h"
#include "galaxy/stars/base/sphere.h"

namespace galaxy {
namespace opengl {

// class SphereDrawer {
// public:
//     SphereDrawer() {};

//     // TODO(Frank): scale? color?
void DrawerInit(int argc, char** argv);
void DrawSphere(const float scale, const Sphere& sphere, const Vec3D& color);
// };

}  // namespace opengl
}  // namespace galaxy

#endif  // __GALAXY_OPENGL_SPHERE_H__
