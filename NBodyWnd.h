
#ifndef __GALAXY_RENDERER_NBODYWND_H__
#define __GALAXY_RENDERER_NBODYWND_H__

#include <fstream>
#include <stdint.h>

//--- Framework ----------------------------------------------------------------
#include "Galaxy.h"
#include "SDLWnd.h"

//------------------------------------------------------------------------------
/** \brief Main window of th n-body simulation. */
class NBodyWnd : public SDLWindow {
public:
  NBodyWnd(const int sz_w, const int sz_h, const std::string caption);
  virtual ~NBodyWnd();

  virtual void Render();
  virtual void OnProcessEvents(uint8_t type);

  void Init(int num);

private:
  enum EDisp {
    dspNONE = 0,
    dspAXIS = 1 << 0,
    dspSTARS = 1 << 1,
    dspSTAT = 1 << 2,
    dspPAUSE = 1 << 3,
    dspHELP = 1 << 4,
    dspROI = 1 << 5,
    dspDENSITY_WAVES = 1 << 6,
    dspRADII = 1 << 7,
    dspVELOCITY = 1 << 8,
    dspDUST = 1 << 9,
    dspH2 = 1 << 10
  };

  struct Color {
    float r;
    float g;
    float b;
  };

  NBodyWnd(const NBodyWnd &orig);

  void DrawStars();
  void DrawDust();
  void DrawH2();
  void DrawStat();
  void DrawHelp();
  void DrawGalaxyRadii();
  void DrawCenterOfMass();
  void DrawDensityWaves(int num, float rad);
  void DrawVelocity();
  void DrawEllipsis(float a, float b, float angle);
  Color ColorFromTemperature(float temp) const;

  int m_camOrient; ///< Index of the camera orientation to use
  int m_starRenderType;
  float m_roi;     ///< Radius of the region of interest
  uint32_t m_flags; ///< The display flags
  bool m_bDumpImage;

  Galaxy m_galaxy;

  // Star color management
  int m_colNum;
  float m_t0, m_t1, m_dt;
  Color m_col[200];
};

#endif // __GALAXY_RENDERER_NBODYWND_H__
