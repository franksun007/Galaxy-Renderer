#ifndef __GALAXY_UTILS_CUMULATIVE_DISTRIBUTION_FUNC_H__
#define __GALAXY_UTILS_CUMULATIVE_DISTRIBUTION_FUNC_H__

#include <vector>

class CumulativeDistributionFunc {
public:
  CumulativeDistributionFunc();
  ~CumulativeDistributionFunc();

  float ProbabilityFromValue(const float val);
  float ValueFromProbability(const float prob);

  void SetupRealistic(float i_0, float k, float a, float r_bulge, float min,
                      float max, int n_steps);

private:
  float f_min;
  float f_max;
  float f_width;
  int n_steps;

  // Surface brightness distribution
  // https://beltoforion.de/article.php?a=spiral_galaxy_renderer
  float i_0;
  float k;
  float a;
  float r_bulge;

  std::vector<float> v_m1;
  std::vector<float> v_y1;
  std::vector<float> v_x1;

  std::vector<float> v_m2;
  std::vector<float> v_y2;
  std::vector<float> v_x2;

  void InitializeCDF(int n_steps);

  float IntensityBulge(float r, float i_0, float k);
  float IntensityDisc(float r, float i_0, float a);
  float Intensity(float x);
};

#endif // __GALAXY_UTILS_CUMULATIVE_DISTRIBUTION_FUNC_H__