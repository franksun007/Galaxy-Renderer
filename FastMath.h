#ifndef __GALAXY_RENDERER_FAST_MATH_H__
#define __GALAXY_RENDERER_FAST_MATH_H__

//------------------------------------------------------------------------------------
#include <cmath>

namespace FastMath {

// TODO(Frank): Factor out inline to ALWAYS_INLINE

//--------------------------------------------------------------------------------
// Helligkeitsverteilung einer elliptischen Galaxie als Funktion des Radius
// nach Freeman
static inline double IntensityDisk(double r, double i0, double a) { return i0 * exp(-r / a); }

//--------------------------------------------------------------------------------
// Intensitätsverteilung im Zentralbereich
static inline double IntensityBulge(double r, double i0, double k) {
  return i0 * exp(-k * pow(r, 0.25));
}

//--------------------------------------------------------------------------------
// Intensitätsverteilung Scheibe und Zentralbereich
static inline double Intensity(double r, double r_bulge, double i0, double a, double k) {
  return (r < r_bulge)
             ? IntensityBulge(r, i0, k)
             : IntensityDisk(r - r_bulge, IntensityBulge(r_bulge, i0, k), a);
}
//------------------------------------------------------------------------------------
/** \brief Fast math functions using table lookup or other optimizations. */
class FastMath {
public:
  static void init(int num = 10000);
  static void release();

  static double nvzz(double m, double s);
  static double sin(double v);
  static double cos(double v);
  static double sqr(double v);

  static void self_test();

private:
  static int s_num;
  static double s_da;
  static double *s_sin;
  static double *s_cos;
};

}  // namespace FastMath

#endif  // __GALAXY_RENDERER_FAST_MATH_H__
