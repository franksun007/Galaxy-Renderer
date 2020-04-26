#ifndef __GALAXY_RENDERER_GALAXY_H__
#define __GALAXY_RENDERER_GALAXY_H__

#include "CumulativeDistributionFunction.h"
#include "Vector.h"

#include <random>

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

  void Reset(float rad, float radCore, float deltaAng, float ex1, float ex2,
             float sigma, float velInner, float velOuter, int numStars,
             bool hasDarkMatter, int pertN, float pertAmp, float dustRenderSize,
             const Vec2D &galaxy_center);

  void Reset();

  Star *GetStars() const { return m_pStars; }
  Star *GetDust() const { return m_pDust; }
  Star *GetH2() const { return m_pH2; }

  float GetRad() const { return m_radGalaxy; }
  float GetCoreRad() const { return m_radCore; }
  float GetFarFieldRad() const { return m_radFarField; }
  float GetSigma() const { return m_sigma; }

  // Properties depending on the orbital radius

  float GetExcentricity(float rad) const;
  float GetOrbitalVelocity(float rad) const;

  float GetAngularOffset(float rad) const { return rad * m_angleOffset; }
  float GetAngularOffset() const { return m_angleOffset; }

  float GetExInner() const { return m_elEx1; }
  float GetExOuter() const { return m_elEx2; }
  float GetTimeStep() const { return m_timeStep; }
  float GetTime() const { return m_time; }
  float GetPertAmp() const { return m_pertAmp; }

  float GetDustRenderSize() const { return m_dustRenderSize; }
  int GetNumH2() const { return m_numH2; }
  int GetNumStars() const { return m_numStars; }
  int GetNumDust() const { return m_numDust; }
  float GetDustStarsRatio() const { return m_DustStarsRatio; }

  int GetPertN() const { return m_pertN; }

  void ToggleDarkMatter();

  void SingleTimeStep(float time, Galaxy &other_galaxy);

  const Vec2D &GetStarPos(int idx);

  void SetPertN(int n) { m_pertN = std::max(0, n); }
  void SetPertAmp(float amp) { m_pertAmp = std::max(0.0f, amp); }
  void SetSigma(float s) {
    m_sigma = s;
    Reset();
  }
  void SetAngularOffset(float offset) {
    m_angleOffset = offset;
    Reset();
  }
  void SetRad(float rad) {
    m_radGalaxy = rad;
    Reset();
  }
  void SetCoreRad(float rad) {
    m_radCore = rad;
    Reset();
  }
  void SetExInner(float ex) {
    m_elEx1 = ex;
    Reset();
  }
  void SetExOuter(float ex) {
    m_elEx2 = ex;
    Reset();
  }
  void SetDustRenderSize(float sz) { m_dustRenderSize = std::max(sz, 1.0f); }
  void SetNumStars(int n) {
    m_numStars = n;
    Reset();
  }
  void SetNumDust(int n) {
    m_numDust = n;
    Reset();
  }
  void SetNumH2(int n) {
    m_numH2 = n;
    Reset();
  }
  void SetDustStarsRatio(float n) {
    m_DustStarsRatio = n;
    Reset();
  }

  Vec2D GetGalaxyCenter() const { return m_pos; }
  void SetGalaxyCenter(const Vec2D &new_center) {
    m_pos = new_center;
    Reset();
  }

  float GetRandom() { return distribution(generator); }
  float GetRandomRad() { return m_cdf.ValFromProb(distribution(generator)); }

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

  int m_numStars;         ///< Total number of stars
  int m_numDust;          ///< Number of Dust Particles
  int m_numH2;            ///< Number of H2 Regions
  float m_DustStarsRatio; // The ratio between the number of dust and stars.

  int m_pertN;
  float m_pertAmp;

  float m_time;
  float m_timeStep;

  bool m_bHasDarkMatter;

public:
  int m_numberByRad[100]; // Historgramm showing distribution of stars

private:
  Vec2D m_pos;    ///< Center of the galaxy
  Star *m_pStars; ///< Pointer to an array of star data
  Star *m_pDust;  ///< Pointer to an array of dusty areas
  Star *m_pH2;

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution;

  CumulativeDistributionFunction m_cdf;
};

#endif // __GALAXY_RENDERER_GALAXY_H__
