// TODO(Frank): Don't really know how to design the interface here.
#ifndef __GALAXY_OPENGL_SPHERE_H__
#define __GALAXY_OPENGL_SPHERE_H__

#include "renderer-interface.h"

#include "galaxy/base/vec-type.h"
#include "galaxy/stars/base/sphere.h"

namespace galaxy {
namespace opengl {

void DrawerInit(int argc, char** argv);

class SphereDrawer : public RendererInterface {
public:
    // TODO(Frank): scale? color?
    SphereDrawer(const float scale, Sphere* sphere, Vec3D color);

    void Update();
    void Draw();

    SphereDrawer(const SphereDrawer& sd) { scale = sd.scale; sphere = sd.sphere; color = sd.color; rotate = sd.rotate; }

protected:
    float scale;
    Sphere* sphere;
    Vec3D color;
    float rotate;
};

}  // namespace opengl
}  // namespace galaxy

#endif  // __GALAXY_OPENGL_SPHERE_H__
