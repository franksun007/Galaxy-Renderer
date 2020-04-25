#ifndef __GALAXY_RENDERER_GALAXY_H__
#define __GALAXY_RENDERER_GALAXY_H__

#include "Vector.h"

// Forward declarations
class Star;

//------------------------------------------------------------------------
/** \brief A class to encapsulate the geometric details of a spiral galaxy. */
class Galaxy {
public:
  Galaxy(float rad = 15000, float radCore = 6000, float deltaAng = 0.019,
         float ex1 = 0.8, float ex2 = 1, float velInner = 200,
         float velOuter = 300, int numStars = 20000);
  ~Galaxy();

  void Reset(float rad, float radCore, float deltaAng, float ex1,
             float ex2, float sigma, float velInner, float velOuter,
             int numStars, bool hasDarkMatter, int pertN, float pertAmp,
             float dustRenderSize);

  void Reset();

  Star *GetStars() const;
  Star *GetDust() const;
  Star *GetH2() const;

  float GetRad() const;
  float GetCoreRad() const;
  float GetFarFieldRad() const;
  float GetSigma() const;

  // Properties depending on the orbital radius

  float GetExcentricity(float rad) const;
  float GetOrbitalVelocity(float rad) const;

  float GetAngularOffset(float rad) const;
  float GetAngularOffset() const;

  float GetExInner() const;
  float GetExOuter() const;
  float GetTimeStep() const;
  float GetTime() const;
  float GetDustRenderSize() const;
  int GetNumStars() const;
  int GetNumDust() const;
  int GetNumH2() const;
  int GetPertN() const;
  float GetPertAmp() const;

  void ToggleDarkMatter();

  void SingleTimeStep(float time);

  const Vec2D &GetStarPos(int idx);

  void SetPertN(int n);
  void SetPertAmp(float amp);
  void SetSigma(float sigma);
  void SetAngularOffset(float offset);
  void SetCoreRad(float rad);
  void SetRad(float rad);
  void SetExInner(float ex);
  void SetExOuter(float ex);
  void SetDustRenderSize(float sz);
  void SetNumStars(int n) { m_numStars = n; Reset(); }

private:
  void InitStars(float sigma);

  // Parameters needed for defining the general structure of the galaxy

  float m_elEx1; ///< Excentricity of the innermost ellipse
  float m_elEx2; ///< Excentricity of the outermost ellipse

  float m_velOrigin; ///< Velovity at the innermost core in km/s
  float m_velInner;  ///< Velocity at the core edge in km/s
  float m_velOuter;  ///< Velocity at the edge of the disk in km/s

  float m_angleOffset; ///< Angular offset per parsec

  float m_radCore;     ///< Radius of the inner core
  float m_radGalaxy;   ///< Radius of the galaxy
  float m_radFarField; ///< The radius after which all density waves must have
                        ///< circular shape
  float m_sigma;       ///< Distribution of stars
  float m_velAngle;    ///< Angular velocity of the density waves

  float m_dustRenderSize;

  int m_numStars; ///< Total number of stars
  int m_numDust;  ///< Number of Dust Particles
  int m_numH2;    ///< Number of H2 Regions

  int m_pertN;
  float m_pertAmp;

  float m_time;
  float m_timeStep;

  bool m_bHasDarkMatter;

public:
  int m_numberByRad[100]; ///< Historgramm showing distribution of stars

private:
  Vec2D m_pos;    ///< Center of the galaxy
  Star *m_pStars; ///< Pointer to an array of star data
  Star *m_pDust;  ///< Pointer to an array of dusty areas
  Star *m_pH2;
};

#endif // __GALAXY_RENDERER_GALAXY_H__
