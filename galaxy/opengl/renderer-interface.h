#ifndef __GALAXY_OPENGL_RENDERER_INTERFACE_H__
#define __GALAXY_OPENGL_RENDERER_INTERFACE_H__

namespace galaxy {

class RendererInterface {
public:
  virtual void Update() = 0;
  virtual void Draw() = 0;
};

} // namespace galaxy

#endif // __GALAXY_OPENGL_RENDERER_INTERFACE_H__