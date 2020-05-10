#ifndef __GALAXY_BASE_CONSTANTS_H__
#define __GALAXY_BASE_CONSTANTS_H__

#include <cmath>

namespace galaxy {
namespace constants {

constexpr float kParsecToKilometer = 3.08567758129e13;
constexpr float kSeccondsPerYear = 365.25 * 86400;
constexpr float kDegreeToRadian = M_PI / 180.0;
constexpr float kRadianToDegree = 180.0 / M_PI;
constexpr float kPi = M_PI;
// TODO(Frank): Maybe float is not enough.
constexpr float kGravitationalConstant = 6.674 * 0.00000000001;

} // namespace constants
} // namespace galaxy

#endif // __GALAXY_BASE_CONSTANTS_H__
