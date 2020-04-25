#ifndef __GALAXY_RENDERER_VECTOR_H__
#define __GALAXY_RENDERER_VECTOR_H__

struct Vec2D {
public:
  Vec2D(const float x_ = 0, const float y_ = 0) : x(x_), y(y_) {}
  float x;
  float y;
};

struct Vec3D {
public:
  Vec3D(const float x_ = 0, const float y_ = 0, const float z_ = 0)
      : x(x_), y(y_), z(z_) {}
  float x;
  float y;
  float z;
};

#endif // __GALAXY_RENDERER_VECTOR_H__
