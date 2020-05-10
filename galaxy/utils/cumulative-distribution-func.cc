#include "cumulative-distribution_func.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

namespace galaxy {

CumulativeDistributionFunc::CumulativeDistributionFunc()
    : v_m1(), v_y1(), v_x1(), v_m2(), v_y2(), v_x2() {}

void CumulativeDistributionFunc::SetupRealistic(float i_0, float k, float a,
                                                float r_bulge, float min,
                                                float max, int n_steps) {
  this->i_0 = i_0;
  this->k = k;
  this->a = a;
  this->r_bulge = r_bulge;

  this->f_min = min;
  this->f_max = max;

  this->n_steps = n_steps;

  InitializeCDF(n_steps);
}

void CumulativeDistributionFunc::InitializeCDF(int n_steps) {
  const float bin_size = (f_max - f_min) / n_steps;
  float x = 0;
  float y = 0;

  v_m1.clear();
  v_y1.clear();
  v_x1.clear();
  v_m2.clear();
  v_y2.clear();
  v_x2.clear();

  // Simpson rule for approximating the integral of a distribution function
  v_y1.push_back(0.0);
  v_x1.push_back(0.0);
  for (int i = 0; i < n_steps; i += 2) {
    x = (i + 2) * bin_size;
    y +=
        bin_size / 3 *
        (CumulativeDistributionFunc::Intensity(f_min + i * bin_size) +
         4 * CumulativeDistributionFunc::Intensity(f_min + (i + 1) * bin_size) +
         CumulativeDistributionFunc::Intensity(f_min + (i + 2) * bin_size));

    v_m1.push_back((y - v_y1.back()) / (2 * bin_size));
    v_x1.push_back(x);
    v_y1.push_back(y);

  }
  v_m1.push_back(0.0);

  // TODO(Frank): Marcos for assert
  // all arrays must have the same length
  if (v_m1.size() != v_x1.size() || v_m1.size() != v_y1.size()) {
    throw std::runtime_error(
        "CumulativeDistributionFunc::InitializeCDF: array size mismatch!");
  }

  // normalize
  for (int i = 0; i < v_y1.size(); ++i) {
    v_y1[i] /= v_y1.back();
    v_m1[i] /= v_y1.back();
  }

  v_x2.push_back(0.0);
  v_y2.push_back(0.0);

  float p = 0;
  const float bin_size_prob = 1.0 / n_steps;
  for (int i = 1, k = 0; i < n_steps; ++i) {
    p = static_cast<float>(i) * bin_size_prob;

    for (; v_y1[k + 1] <= p; ++k) {
    }

    y = v_x1[k] + (p - v_y1[k]) / v_m1[k];

    v_m2.push_back((y - v_y2.back()) / bin_size_prob);
    v_x2.push_back(p);
    v_y2.push_back(y);
  }
  v_m2.push_back(0.0);

  // all arrays must have the same length
  if (v_m2.size() != v_x2.size() || v_m2.size() != v_y2.size()) {
    throw std::runtime_error(
        "CumulativeDistributionFunc::InitializeCDF: array size mismatch!");
  }
}

CumulativeDistributionFunc::~CumulativeDistributionFunc() {}

float CumulativeDistributionFunc::ProbabilityFromValue(float val) {
  if (val < f_min || val > f_max) {
    throw std::runtime_error("Value out of range");
  }

  const float bin_size = 2 * ((f_max - f_min) / n_steps);
  const int32_t i = static_cast<int32_t>(std::round((val - f_min) / bin_size));
  const float remainder = val - i * bin_size;

  assert(i >= 0 && i < v_m1.size());
  return v_y1[i] + v_m1[i] * remainder;
}

float CumulativeDistributionFunc::ValueFromProbability(float prob) {
  if (prob < 0 || prob > 1) {
    throw std::runtime_error("Probability out of range");
  }

  const float bin_size = 1.0 / (v_y2.size() - 1);

  const int32_t i = static_cast<int32_t>(std::round(prob / bin_size));
  const float remainder = prob - i * bin_size;

  assert(i >= 0 && i < (int)v_m2.size());
  return v_y2[i] + v_m2[i] * remainder;
}

float CumulativeDistributionFunc::IntensityBulge(float r, float i_0, float k) {
  return i_0 * exp(-k * pow(r, 0.25));
}

float CumulativeDistributionFunc::IntensityDisc(float r, float i_0, float a) {
  return i_0 * exp(-r / a);
}

float CumulativeDistributionFunc::Intensity(float x) {
  if (x < r_bulge) {
    return IntensityBulge(x, i_0, k);
  } else {
    return IntensityDisc(x - r_bulge, IntensityBulge(r_bulge, i_0, k), a);
  }
}

} // namespace galaxy