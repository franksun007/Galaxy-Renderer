// TODO(Frank): Don't really know how to design the interface here.
#ifndef __GALAXY_OPENGL_SPHERE_H__
#define __GALAXY_OPENGL_SPHERE_H__

#include "renderer-interface.h"

#include "galaxy/base/macros.h"
#include "galaxy/base/vec-type.h"
#include "galaxy/stars/base/sphere.h"

namespace galaxy {
namespace opengl {

void DrawerInit(int argc, char **argv);

class SphereDrawer : public RendererInterface {
public:
  static SphereDrawer* Create(void *desired_memory, const float scale, const Sphere& sphere, const Vec3D& color);

  void Update();
  void Draw(); 

private:
  SphereDrawer(const float scale, const Sphere& sphere, const Vec3D& color);

  float scale;
  Sphere sphere;
  Vec3D color;
  float rotate;

  DISALLOW_COPY_AND_ASSIGN(SphereDrawer);
};

} // namespace opengl
} // namespace galaxy

#endif // __GALAXY_OPENGL_SPHERE_H__
