#ifndef __GALAXY_RENDERER_VECTOR_H__
#define __GALAXY_RENDERER_VECTOR_H__

struct Vec2D {
public:
  Vec2D(const double x_ = 0, const double y_ = 0) : x(x_), y(y_) {}
  double x;
  double y;
};

struct Vec3D {
public:
  Vec3D(const double x_ = 0, const double y_ = 0, const double z_ = 0)
      : x(x_), y(y_), z(z_) {}
  double x;
  double y;
  double z;
};

#endif // __GALAXY_RENDERER_VECTOR_H__
