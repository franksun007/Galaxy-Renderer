#include "Galaxy.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "Constants.h"
#include "Star.h"

Galaxy::Galaxy(float rad, float radCore, float deltaAng, float ex1, float ex2,
               float velInner, float velOuter, int numStars)
    : m_elEx1(ex1), m_elEx2(ex2), m_velOrigin(30), m_velInner(velInner),
      m_velOuter(velOuter), m_angleOffset(deltaAng), m_radCore(radCore),
      m_radGalaxy(rad), m_sigma(0.45), m_velAngle(0.000001),
      m_dustRenderSize(70), m_numStars(numStars), m_numDust(numStars),
      m_numH2(300), m_DustStarsRatio(1.0), m_pertN(0), m_pertAmp(0), m_time(0),
      m_timeStep(0), m_bHasDarkMatter(true), m_pos(0, 0), m_pStars(nullptr),
      m_pDust(nullptr), m_pH2(nullptr),
      distribution(std::uniform_real_distribution<float>(0.0f, 1.0f)) {}

Galaxy::~Galaxy() {
  delete[] m_pStars;
  delete[] m_pDust;
  delete[] m_pH2;
}

//------------------------------------------------------------------------
void Galaxy::Reset() {
  Reset(m_radGalaxy, m_radCore, m_angleOffset, m_elEx1, m_elEx2, m_sigma,
        m_velInner, m_velOuter, m_numStars, m_bHasDarkMatter, m_pertN,
        m_pertAmp, m_dustRenderSize, m_pos);
}

//------------------------------------------------------------------------
void Galaxy::Reset(float rad, float radCore, float deltaAng, float ex1,
                   float ex2, float sigma, float velInner, float velOuter,
                   int numStars, bool hasDarkMatter, int pertN, float pertAmp,
                   float dustRenderSize, const Vec2D &galaxy_center) {
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
  m_numDust = static_cast<int>(std::round(m_DustStarsRatio * m_numStars));
  m_time = 0;
  m_dustRenderSize = dustRenderSize;
  m_bHasDarkMatter = hasDarkMatter;
  m_pertN = pertN;
  m_pertAmp = pertAmp;
  m_pos = galaxy_center;

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

  constexpr int32_t kInitialStarTemporature = 6000;
  // First star is the black hole at the centre
  m_pStars[0].m_a = 0;
  m_pStars[0].m_b = 0;
  m_pStars[0].m_angle = 0;
  m_pStars[0].m_theta = 0;
  m_pStars[0].m_velTheta = 0;
  m_pStars[0].m_center = m_pos;
  m_pStars[0].m_velTheta =
      GetOrbitalVelocity((m_pStars[0].m_a + m_pStars[0].m_b) / 2.0);
  m_pStars[0].m_temp = kInitialStarTemporature;
  m_pStars[0].m_temp = 0;

  // second star is at the edge of the core area
  m_pStars[1].m_a = m_radCore;
  m_pStars[1].m_b = m_radCore * GetExcentricity(m_radCore);
  m_pStars[1].m_angle = GetAngularOffset(m_radCore);
  m_pStars[1].m_theta = 0;
  m_pStars[1].m_center = m_pos;
  m_pStars[1].m_velTheta =
      GetOrbitalVelocity((m_pStars[1].m_a + m_pStars[1].m_b) / 2.0);
  m_pStars[1].m_temp = kInitialStarTemporature;
  m_pStars[1].m_temp = 0;

  // third star is at the edge of the disk
  m_pStars[2].m_a = m_radGalaxy;
  m_pStars[2].m_b = m_radGalaxy * GetExcentricity(m_radGalaxy);
  m_pStars[2].m_angle = GetAngularOffset(m_radGalaxy);
  m_pStars[2].m_theta = 0;
  m_pStars[2].m_center = m_pos;
  m_pStars[2].m_velTheta =
      GetOrbitalVelocity((m_pStars[2].m_a + m_pStars[2].m_b) / 2.0);
  m_pStars[2].m_temp = kInitialStarTemporature;
  m_pStars[2].m_temp = 0;

  // cell width of the histogramm
  float dh = (float)m_radFarField / 100.0;

  m_cdf.SetupRealistic(1.0,               // Maximalintensität
                       0.02,              // k (bulge)
                       m_radGalaxy / 3.0, // disc skalenlänge
                       m_radCore,         // bulge radius
                       0,                 // start der intensitätskurve
                       m_radFarField,     // ende der intensitätskurve
                       1000);             // Anzahl der stützstellen
  for (int i = 3; i < m_numStars; ++i) {
    float rad = m_cdf.ValFromProb(distribution(generator));

    m_pStars[i].m_a = rad;
    m_pStars[i].m_b = rad * GetExcentricity(rad);
    m_pStars[i].m_angle = GetAngularOffset(rad);
    m_pStars[i].m_theta = 360.0 * (distribution(generator));
    m_pStars[i].m_velTheta = GetOrbitalVelocity(rad);
    m_pStars[i].m_center = m_pos;
    m_pStars[i].m_temp = kInitialStarTemporature +
                         (kInitialStarTemporature * (distribution(generator))) -
                         kInitialStarTemporature;
    m_pStars[i].m_mag = 0.3 + distribution(generator);

    int idx = (int)std::min(
        1.0 / dh * (m_pStars[i].m_a + m_pStars[i].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;
  }

  // Initialise Dust
  float x, y, rad;
  for (int i = 0; i < m_numDust; ++i) {
    if (i % 4 == 0) {
      rad = m_cdf.ValFromProb(distribution(generator));
    } else {
      x = 2 * m_radGalaxy * (distribution(generator)) - m_radGalaxy;
      y = 2 * m_radGalaxy * (distribution(generator)) - m_radGalaxy;
      rad = sqrt(x * x + y * y);
    }

    m_pDust[i].m_a = rad;
    m_pDust[i].m_b = rad * GetExcentricity(rad);
    m_pDust[i].m_angle = GetAngularOffset(rad);
    m_pDust[i].m_theta = 360.0 * (distribution(generator));
    m_pDust[i].m_velTheta =
        GetOrbitalVelocity((m_pDust[i].m_a + m_pDust[i].m_b) / 2.0);
    m_pDust[i].m_center = m_pos;

    // I want the outer parts to appear blue, the inner parts yellow. I'm
    // imposing the following temperature distribution (no science here it just
    // looks right)
    m_pDust[i].m_temp = 5000 + rad / 4.5;

    m_pDust[i].m_mag = 0.015 + 0.01 * distribution(generator);
    int idx =
        (int)std::min(1.0 / dh * (m_pDust[i].m_a + m_pDust[i].m_b) / 2.0, 99.0);
    m_numberByRad[idx]++;
  }

  // Initialise H2
  for (int i = 0; i < m_numH2; ++i) {
    x = 2 * (m_radGalaxy) * (distribution(generator)) - (m_radGalaxy);
    y = 2 * (m_radGalaxy) * (distribution(generator)) - (m_radGalaxy);
    rad = sqrt(x * x + y * y);

    int k1 = 2 * i;
    m_pH2[k1].m_a = rad;
    m_pH2[k1].m_b = rad * GetExcentricity(rad);
    m_pH2[k1].m_angle = GetAngularOffset(rad);
    m_pH2[k1].m_theta = 360.0 * (distribution(generator));
    m_pH2[k1].m_velTheta =
        GetOrbitalVelocity((m_pH2[k1].m_a + m_pH2[k1].m_b) / 2.0);
    m_pH2[k1].m_center = m_pos;
    m_pH2[k1].m_temp = 6000 + (6000 * (distribution(generator))) - 3000;
    m_pH2[k1].m_mag = 0.1 + 0.05 * distribution(generator);
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
      float rH = 2000;  // Radius auf dem die Dichte um die Hälfte gefallen ist
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

// TODO(Frank): Scaleup to more galaxy
void Galaxy::SingleTimeStep(float time, Galaxy &other_galaxy) {
  m_timeStep = time;
  m_time += time;

  int stars_in_my_galaxy = 0;
  int dust_in_my_galaxy = 0;
  int h2_in_my_galaxy = 0;

  Vec2D posOld;
#pragma omp parallel for
  for (int i = 0; i < m_numStars; ++i) {
    m_pStars[i].m_theta += (m_pStars[i].m_velTheta * time);
    posOld = m_pStars[i].m_pos;

    m_pStars[i].CalcXY(m_pertN, m_pertAmp);
    m_pStars[i].UpdateGalaxyIfCloser(other_galaxy, 0);

    stars_in_my_galaxy += m_pStars[i].m_center == m_pos;

    Vec2D b =
        Vec2D(m_pStars[i].m_pos.x - posOld.x, m_pStars[i].m_pos.y - posOld.y);
    m_pStars[i].m_vel = b;
  }

#pragma omp parallel for
  for (int i = 0; i < m_numDust; ++i) {
    m_pDust[i].m_theta += (m_pDust[i].m_velTheta * time);
    posOld = m_pDust[i].m_pos;
    m_pDust[i].CalcXY(m_pertN, m_pertAmp);
    m_pDust[i].UpdateGalaxyIfCloser(other_galaxy, 1);
    dust_in_my_galaxy += m_pDust[i].m_center == m_pos;
  }

#pragma omp parallel for
  for (int i = 0; i < m_numH2 * 2; ++i) {
    m_pH2[i].m_theta += (m_pH2[i].m_velTheta * time);
    posOld = m_pDust[i].m_pos;
    m_pH2[i].CalcXY(m_pertN, m_pertAmp);
    m_pH2[i].UpdateGalaxyIfCloser(other_galaxy, 2);
    h2_in_my_galaxy += m_pH2[i].m_center == m_pos;
  }

  std::cout << "Galaxy " << m_pos.to_string() << " contains: ("
            << stars_in_my_galaxy << ", " << dust_in_my_galaxy << ", "
            << h2_in_my_galaxy << ")" << std::endl;
}

//-----------------------------------------------------------------------
const Vec2D &Galaxy::GetStarPos(int idx) {
  if (idx < 0 || idx >= m_numStars) {
    throw std::runtime_error("index out of bounds.");
  }

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
