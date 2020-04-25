#ifndef __GALAXY_RENDERER_CONSTANTS_H__
#define __GALAXY_RENDERER_CONSTANTS_H__

#include <cmath>

namespace constants {

// Convert parsec to kilometre.
constexpr float PC_TO_KM = 3.08567758129e13;

// Seconds per year.
constexpr float SEC_PER_YEAR = 365.25 * 86400;

// Deg to radian conversion faktor.
constexpr float DEG_TO_RAD = M_PI / 180.0;

// Radian to deg conversion faktor.
constexpr float RAD_TO_DEG = 180.0 / M_PI;

}; // namespace constants

#endif // __GALAXY_RENDERER_CONSTANTS_H__
