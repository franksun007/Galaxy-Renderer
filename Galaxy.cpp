#include "Galaxy.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

//------------------------------------------------------------------------
#include "Constants.h"
#include "CumulativeDistributionFunction.h"
#include "Star.h"

//------------------------------------------------------------------------
float rnd_spread(float v, float o) {
  return (v + (2 * o * (float)rand() / RAND_MAX - o));
}

//------------------------------------------------------------------------
Galaxy::Galaxy(float rad, float radCore, float deltaAng, float ex1,
               float ex2, float velInner, float velOuter, int numStars)
    : m_elEx1(ex1), m_elEx2(ex2), m_velOrigin(30), m_velInner(velInner),
      m_velOuter(velOuter), m_angleOffset(deltaAng), m_radCore(radCore),
      m_radGalaxy(rad), m_sigma(0.45), m_velAngle(0.000001),
      m_dustRenderSize(70), m_numStars(numStars), m_numDust(numStars),
      m_numH2(300), m_pertN(0), m_pertAmp(0), m_time(0), m_timeStep(0),
      m_bHasDarkMatter(true), m_pos(0, 0), m_pStars(nullptr), m_pDust(nullptr),
      m_pH2(nullptr) {}

//------------------------------------------------------------------------
Galaxy::~Galaxy() {
  delete[] m_pStars;
  delete[] m_pDust;
  delete[] m_pH2;
}

//------------------------------------------------------------------------
void Galaxy::Reset() {
  Reset(m_radGalaxy, m_radCore, m_angleOffset, m_elEx1, m_elEx2, m_sigma,
        m_velInner, m_velOuter, m_numStars, m_bHasDarkMatter, m_pertN,
        m_pertAmp, m_dustRenderSize);
}

//------------------------------------------------------------------------
void Galaxy::Reset(float rad, float radCore, float deltaAng, float ex1,
                   float ex2, float sigma, float velInner, float velOuter,
                   int numStars, bool hasDarkMatter, int pertN, float pertAmp,
                   float dustRenderSize) {
  m_elEx1 = ex1;
  m_elEx2 = ex2;
  m_velInner = velInner;
  m_velOuter = velOuter;
  m_elEx2 = ex2;
  m_angleOffset = deltaAng;
  m_radCore = radCore;
  m_radGalaxy = rad;
  m_radFarField =
      m_radGalaxy *
      2; // there is no science behind this threshold it just looks nice
  m_sigma = sigma;
  m_numStars = numStars;
  m_numDust = numStars / 2;
  m_time = 0;
  m_dustRenderSize = dustRenderSize;
  m_bHasDarkMatter = hasDarkMatter;
  m_pertN = pertN;
  m_pertAmp = pertAmp;

  for (int i = 0; i < 100; ++i)
    m_numberByRad[i] = 0;

  InitStars(m_sigma);
}

void Galaxy::ToggleDarkMatter() {
  m_bHasDarkMatter ^= true;
  Reset();
}

//------------------------------------------------------------------------
void Galaxy::InitStars(float sigma) {
  delete[] m_pDust;
  m_pDust = new Star[m_numDust];

  delete[] m_pStars;
  m_pStars = new Star[m_numStars];

  delete[] m_pH2;
  m_pH2 = new Star[m_numH2 * 2];

  // The first three stars can be used for aligning the
  // camera with the galaxy rotation.

  // First star ist the black hole at the centre
  m_pStars[0].m_a = 0;
  m_pStars[0].m_b = 0;
  m_pStars[0].m_angle = 0;
  m_pStars[0].m_theta = 0;
  m_pStars[0].m_velTheta = 0;
  m_pStars[0].m_center = Vec2D(0, 0);
  m_pStars[0].m_velTheta =
      GetOrbitalVelocity((m_pStars[0].m_a + m_pStars[0].m_b) / 2.0);
  m_pStars[0].m_temp = 6000;

  // second star is at the edge of the core area
  m_pStars[1].m_a = m_radCore;
  m_pStars[1].m_b = m_radCore * GetExcentricity(m_radCore);
  m_pStars[1].m_angle = GetAngularOffset(m_radCore);
  m_pStars[1].m_theta = 0;
  m_pStars[1].m_center = Vec2D(0, 0);
  m_pStars[1].m_velTheta =
      GetOrbitalVelocity((m_pStars[1].m_a + m_pStars[1].m_b) / 2.0);
  m_pStars[1].m_temp = 6000;

  // third star is at the edge of the disk
  m_pStars[2].m_a = m_radGalaxy;
  m_pStars[2].m_b = m_radGalaxy * GetExcentricity(m_radGalaxy);
  m_pStars[2].m_angle = GetAngularOffset(m_radGalaxy);
  m_pStars[2].m_theta = 0;
  m_pStars[2].m_center = Vec2D(0, 0);
  m_pStars[2].m_velTheta =
      GetOrbitalVelocity((m_pStars[2].m_a + m_pStars[2].m_b) / 2.0);
  m_pStars[2].m_temp = 6000;

  // cell width of the histogramm
  float dh = (float)m_radFarField / 100.0;

  // Initialize the stars
  CumulativeDistributionFunction cdf;
  cdf.SetupRealistic(1.0,               // Maximalintensität
                     0.02,              // k (bulge)
                     m_radGalaxy / 3.0, // disc skalenlänge
                     m_radCore,         // bulge radius
                     0,                 // start der intensitätskurve
                     m_radFarField,     // ende der intensitätskurve
                     1000);             // Anzahl der stützstellen
  for (int i = 3; i < m_numStars; ++i) {
    float rad = cdf.ValFromProb((float)rand() / (float)RAND_MAX);

    m_pStars[i].m_a = rad;
    m_pStars[i].m_b = rad * GetExcentricity(rad);
    m_pStars[i].m_angle = GetAngularOffset(rad);
    m_pStars[i].m_theta = 360.0 * ((float)rand() / RAND_MAX);
    m_pStars[i].m_velTheta = GetOrbitalVelocity(rad);
    m_pStars[i].m_center = Vec2D(0, 0);
    m_pStars[i].m_temp = 6000 + (4000 * ((float)rand() / RAND_MAX)) - 2000;
    m_pStars[i].m_mag = 0.3 + 0.2 * (float)rand() / (float)RAND_MAX;

    int idx = (int)std::min(
        1.0 / dh * (m_pStars[i].m_a + m_pStars[i].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;
  }

  // Initialise Dust
  float x, y, rad;
  for (int i = 0; i < m_numDust; ++i) {
    if (i % 4 == 0) {
      rad = cdf.ValFromProb((float)rand() / (float)RAND_MAX);
    } else {
      x = 2 * m_radGalaxy * ((float)rand() / RAND_MAX) - m_radGalaxy;
      y = 2 * m_radGalaxy * ((float)rand() / RAND_MAX) - m_radGalaxy;
      rad = sqrt(x * x + y * y);
    }

    m_pDust[i].m_a = rad;
    m_pDust[i].m_b = rad * GetExcentricity(rad);
    m_pDust[i].m_angle = GetAngularOffset(rad);
    m_pDust[i].m_theta = 360.0 * ((float)rand() / RAND_MAX);
    m_pDust[i].m_velTheta =
        GetOrbitalVelocity((m_pDust[i].m_a + m_pDust[i].m_b) / 2.0);
    m_pDust[i].m_center = Vec2D(0, 0);

    // I want the outer parts to appear blue, the inner parts yellow. I'm
    // imposing the following temperature distribution (no science here it just
    // looks right)
    m_pDust[i].m_temp = 5000 + rad / 4.5;

    m_pDust[i].m_mag = 0.015 + 0.01 * (float)rand() / (float)RAND_MAX;
    int idx =
        (int)std::min(1.0 / dh * (m_pDust[i].m_a + m_pDust[i].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;
  }

  // Initialise Dust
  for (int i = 0; i < m_numH2; ++i) {
    x = 2 * (m_radGalaxy) * ((float)rand() / RAND_MAX) - (m_radGalaxy);
    y = 2 * (m_radGalaxy) * ((float)rand() / RAND_MAX) - (m_radGalaxy);
    rad = sqrt(x * x + y * y);

    int k1 = 2 * i;
    m_pH2[k1].m_a = rad;
    m_pH2[k1].m_b = rad * GetExcentricity(rad);
    m_pH2[k1].m_angle = GetAngularOffset(rad);
    m_pH2[k1].m_theta = 360.0 * ((float)rand() / RAND_MAX);
    m_pH2[k1].m_velTheta =
        GetOrbitalVelocity((m_pH2[k1].m_a + m_pH2[k1].m_b) / 2.0);
    m_pH2[k1].m_center = Vec2D(0, 0);
    m_pH2[k1].m_temp = 6000 + (6000 * ((float)rand() / RAND_MAX)) - 3000;
    m_pH2[k1].m_mag = 0.1 + 0.05 * (float)rand() / (float)RAND_MAX;
    int idx =
        (int)std::min(1.0 / dh * (m_pH2[k1].m_a + m_pH2[k1].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;

    // Create second point 100 pc away from the first one
    int dist = 1000.0;
    int k2 = 2 * i + 1;
    m_pH2[k2].m_a = (rad + dist);
    m_pH2[k2].m_b = (rad /*+ dist*/) * GetExcentricity(rad /*+ dist*/);
    m_pH2[k2].m_angle = GetAngularOffset(rad);
    m_pH2[k2].m_theta = m_pH2[k1].m_theta;
    m_pH2[k2].m_velTheta = m_pH2[k1].m_velTheta;
    m_pH2[k2].m_center = m_pH2[k1].m_center;
    m_pH2[k2].m_temp = m_pH2[k1].m_temp;
    m_pH2[k2].m_mag = m_pH2[k1].m_mag;
    idx = (int)std::min(1.0 / dh * (m_pH2[k2].m_a + m_pH2[k2].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;
  }
}

//------------------------------------------------------------------------
float Galaxy::GetSigma() const { return m_sigma; }

//------------------------------------------------------------------------
void Galaxy::SetDustRenderSize(float sz) {
  m_dustRenderSize = std::max(sz, 1.0f);
}

//------------------------------------------------------------------------
void Galaxy::SetSigma(float s) {
  m_sigma = s;
  Reset();
}

//------------------------------------------------------------------------
Star *Galaxy::GetStars() const { return m_pStars; }

//------------------------------------------------------------------------
Star *Galaxy::GetDust() const { return m_pDust; }

//------------------------------------------------------------------------
Star *Galaxy::GetH2() const { return m_pH2; }

//------------------------------------------------------------------------
float Galaxy::GetDustRenderSize() const { return m_dustRenderSize; }

//------------------------------------------------------------------------
float Galaxy::GetRad() const { return m_radGalaxy; }

//------------------------------------------------------------------------
float Galaxy::GetCoreRad() const { return m_radCore; }

//------------------------------------------------------------------------
float Galaxy::GetFarFieldRad() const { return m_radFarField; }

//------------------------------------------------------------------------
void Galaxy::SetAngularOffset(float offset) {
  m_angleOffset = offset;
  Reset();
}

//------------------------------------------------------------------------
/** \brief Returns the orbital velocity in degrees per year.
    \param rad Radius in parsec
*/
float Galaxy::GetOrbitalVelocity(float rad) const {
  float vel_kms(0); // velovity in kilometer per seconds

  // Realistically looking velocity curves for the Wikipedia models.
  struct VelocityCurve {
    static float MS(float r) {
      float d = 2000;   // Dicke der Scheibe
      float rho_so = 1; // Dichte im Mittelpunkt
      float rH = 2000; // Radius auf dem die Dichte um die Hälfte gefallen ist
      return rho_so * exp(-r / rH) * (r * r) * M_PI * d;
    }

    static float MH(float r) {
      float rho_h0 = 0.15; // Dichte des Halos im Zentrum
      float rC = 2500;     // typische skalenlänge im Halo
      return rho_h0 * 1 / (1 + pow(r / rC, 2)) * (4 * M_PI * pow(r, 3) / 3);
    }

    // Velocity curve with dark matter
    static float v(float r) {
      float MZ = 100;
      float G = 6.672e-11;
      return 20000 * sqrt(G * (MH(r) + MS(r) + MZ) / r);
    }

    // velocity curve without dark matter
    static float vd(float r) {
      float MZ = 100;
      float G = 6.672e-11;
      return 20000 * sqrt(G * (MS(r) + MZ) / r);
    }
  };

  if (m_bHasDarkMatter) {
    //  with dark matter
    vel_kms = VelocityCurve::v(rad);
  } else {
    // without dark matter:
    vel_kms = VelocityCurve::vd(rad);
  }

  // Calculate velocity in degree per year
  float u = 2 * M_PI * rad * constants::PC_TO_KM;       // Umfang in km
  float time = u / (vel_kms * constants::SEC_PER_YEAR); // Umlaufzeit in Jahren

  return 360.0 / time; // Grad pro Jahr
}

//------------------------------------------------------------------------
float Galaxy::GetExcentricity(float r) const {
  if (r < m_radCore) {
    // Core region of the galaxy. Innermost part is round
    // excentricity increasing linear to the border of the core.
    return 1 + (r / m_radCore) * (m_elEx1 - 1);
  } else if (r > m_radCore && r <= m_radGalaxy) {
    return m_elEx1 +
           (r - m_radCore) / (m_radGalaxy - m_radCore) * (m_elEx2 - m_elEx1);
  } else if (r > m_radGalaxy && r < m_radFarField) {
    // excentricity is slowly reduced to 1.
    return m_elEx2 +
           (r - m_radGalaxy) / (m_radFarField - m_radGalaxy) * (1 - m_elEx2);
  } else
    return 1;
}

//------------------------------------------------------------------------
float Galaxy::GetAngularOffset(float rad) const {
  return rad * m_angleOffset;
}

//------------------------------------------------------------------------
float Galaxy::GetAngularOffset() const { return m_angleOffset; }

//------------------------------------------------------------------------
float Galaxy::GetExInner() const { return m_elEx1; }

//-----------------------------------------------------------------------
float Galaxy::GetExOuter() const { return m_elEx2; }

//-----------------------------------------------------------------------
int Galaxy::GetPertN() const { return m_pertN; }

//-----------------------------------------------------------------------
float Galaxy::GetPertAmp() const { return m_pertAmp; }

//-----------------------------------------------------------------------
void Galaxy::SetPertN(int n) { m_pertN = std::max(0, n); }

//-----------------------------------------------------------------------
void Galaxy::SetPertAmp(float amp) { m_pertAmp = std::max(0.0f, amp); }

//-----------------------------------------------------------------------
void Galaxy::SetRad(float rad) {
  m_radGalaxy = rad;
  Reset();
}

//-----------------------------------------------------------------------
void Galaxy::SetCoreRad(float rad) {
  m_radCore = rad;
  Reset();
}

//-----------------------------------------------------------------------
void Galaxy::SetExInner(float ex) {
  m_elEx1 = ex;
  Reset();
}

//-----------------------------------------------------------------------
void Galaxy::SetExOuter(float ex) {
  m_elEx2 = ex;
  Reset();
}

//-----------------------------------------------------------------------
float Galaxy::GetTimeStep() const { return m_timeStep; }

//-----------------------------------------------------------------------
float Galaxy::GetTime() const { return m_time; }

//-----------------------------------------------------------------------
void Galaxy::SingleTimeStep(float time) {
  m_timeStep = time;
  m_time += time;

  Vec2D posOld;
  #pragma omp parallel for
  for (int i = 0; i < m_numStars; ++i) {
    m_pStars[i].m_theta += (m_pStars[i].m_velTheta * time);
    posOld = m_pStars[i].m_pos;
    m_pStars[i].CalcXY(m_pertN, m_pertAmp);

    Vec2D b =
        Vec2D(m_pStars[i].m_pos.x - posOld.x, m_pStars[i].m_pos.y - posOld.y);
    m_pStars[i].m_vel = b;
  }

  #pragma omp parallel for
  for (int i = 0; i < m_numDust; ++i) {
    m_pDust[i].m_theta += (m_pDust[i].m_velTheta * time);
    posOld = m_pDust[i].m_pos;
    m_pDust[i].CalcXY(m_pertN, m_pertAmp);
  }

  #pragma omp parallel for
  for (int i = 0; i < m_numH2 * 2; ++i) {
    m_pH2[i].m_theta += (m_pH2[i].m_velTheta * time);
    posOld = m_pDust[i].m_pos;
    m_pH2[i].CalcXY(m_pertN, m_pertAmp);
  }
}

//-----------------------------------------------------------------------
const Vec2D &Galaxy::GetStarPos(int idx) {
  if (idx >= m_numStars)
    throw std::runtime_error("index out of bounds.");

  return m_pStars[idx].m_pos; // GetPos();
}

/*
//-----------------------------------------------------------------------
Vec2D Galaxy::GetStarPos(int idx)
{
  if (idx>=m_numStars)
    throw std::runtime_error("index out of bounds.");

  float &a = m_pStars[idx].m_a,
         &b = m_pStars[idx].m_b,
         &theta = m_pStars[idx].m_theta;
  const Vec2D &p = m_pStars[idx].m_center;

//  float beta  = m_angle - m_pStars[idx].m_angle,
  float beta  = -m_pStars[idx].m_angle,
         alpha = theta * constants::DEG_TO_RAD;

  // temporyries to save cpu time
  float cosalpha = cos(alpha),
         sinalpha = sin(alpha),
         cosbeta = cos(beta),
         sinbeta = sin(beta);

  Vec2D pos(p.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
            p.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta));
  return pos;
}
*/

//-----------------------------------------------------------------------
int Galaxy::GetNumH2() const { return m_numH2; }

//-----------------------------------------------------------------------
int Galaxy::GetNumStars() const { return m_numStars; }

//-----------------------------------------------------------------------
int Galaxy::GetNumDust() const { return m_numDust; }
