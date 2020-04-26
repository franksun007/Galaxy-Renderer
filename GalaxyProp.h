#ifndef __GALAXY_RENDERER_GALAXY_PROP_H__
#define __GALAXY_RENDERER_GALAXY_PROP_H__

//-------------------------------------------------------------------------------------------
/** \brief Encapsulates the data needed to define the galaxies shape. */
class GalaxyProp {

  GalaxyProp(float radDisk, float radCore, float deltaAng, float exInner,
             float exOuter, float velInner, float velOuter);

public:
  // Excentricity of the orbital ellipses
  float m_excInner; // Excentricity of the innermost ellipse
  float m_excOuter; // Excentricity of the outermost ellipse

  // Orbital Velocities
  float m_velOrigin; // Velovity at the innermost core in km/s
  float m_velInner;  // Velocity at the core edge in km/s
  float m_velOuter;  // Velocity at the edge of the disk in km/s

  // Shape of the density wave
  float m_angleOffset;    // Angular of the orbits offset per parsec
  float m_angleOffsetInc; // Increment of the angular offset
  float m_velAngle;       // Angular velocity of the density waves

  // Galaxy radius
  float m_radCore;     // Radius of the inner core
  float m_radGalaxy;   // Radius of the galaxy
  float m_radFarField; // The radius after which all density waves must have
                       // circular shape
};

#endif // __GALAXY_RENDERER_GALAXY_PROP_H__
