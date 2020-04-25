#ifndef __GALAXY_RENDERER_CUMULATIVE_DISTRIBUTION_FUNCTION_H__
#define __GALAXY_RENDERER_CUMULATIVE_DISTRIBUTION_FUNCTION_H__

//-------------------------------------------------------------------------------------------------
#include <vector>

class CumulativeDistributionFunction {
public:
  typedef float (CumulativeDistributionFunction::*dist_fun_t)(float x);

  CumulativeDistributionFunction();
  virtual ~CumulativeDistributionFunction();

  float ProbFromVal(float fVal);
  float ValFromProb(float fVal);

  void SetupRealistic(float I0, float k, float a, float RBulge, float min,
                      float max, int nSteps);

private:
  dist_fun_t m_pDistFun;
  float m_fMin;
  float m_fMax;
  float m_fWidth;
  int m_nSteps;

  // parameters for realistic star distribution
  float m_I0;
  float m_k;
  float m_a;
  float m_RBulge;

  std::vector<float> m_vM1;
  std::vector<float> m_vY1;
  std::vector<float> m_vX1;

  std::vector<float> m_vM2;
  std::vector<float> m_vY2;
  std::vector<float> m_vX2;

  void BuildCDF(int nSteps);

  float IntensityBulge(float R, float I0, float k);
  float IntensityDisc(float R, float I0, float a);
  float Intensity(float x);
};

#endif // __GALAXY_RENDERER_CUMULATIVE_DISTRIBUTION_FUNCTION_H__
