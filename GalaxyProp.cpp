#include "GalaxyProp.h"

//-------------------------------------------------------------------------------------------
GalaxyProp::GalaxyProp(float radDisk, float radCore, float deltaAng,
                       float exInner, float exOuter, float velInner,
                       float velOuter)
    : m_excInner(exInner), m_excOuter(exOuter), m_velOrigin(),
      m_velInner(velInner), m_velOuter(velOuter), m_angleOffset(deltaAng),
      m_angleOffsetInc(0), m_velAngle(0), m_radCore(radCore),
      m_radGalaxy(radDisk), m_radFarField(radDisk * 2) {}
