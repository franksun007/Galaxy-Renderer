#ifndef __GALAXY_UI_SDL_WINDOW_H__
#define __GALAXY_UI_SDL_WINDOW_H__

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_opengl.h>

#include "galaxy/base/types.h"
#include "galaxy/base/vec-type.h"

namespace galaxy {

class SDLWindow {
public:
  SDLWindow(int32_t width, int32_t height, float axis_len,
            const string &caption);
  virtual ~SDLWindow();
  void Init();
  void MainLoop();
  void ExitMainLoop();
  void SetCaption(const string &caption);

  int32_t GetWidth() const;
  int32_t GetHeight() const;
  Vec3D GetCamPos() const;
  Vec3D GetCamOrient() const;
  Vec3D GetCamLookAt() const;
  float GetFOV() const;
  int32_t GetFPS() const;

  void SetDrawStats(bool draw) { draw_stats = draw; }

  virtual void Render();

protected:
  virtual void PollEvents();
  virtual void OnProcessEvents(uint8_t type);
  virtual void DrawStats();

  // Camera setup
  void SetCameraOrientation(const Vec3D &orientation);
  void SetCamera(const Vec3D &pos, const Vec3D &look_at, const Vec3D &orient);
  void AdjustCamera();

  // Basic graphics functionality
  void DrawAxis(const Vec2D &origin);
  void SaveToTGA(const string &name);
  void SaveToTGA(int idx = -1);

  // misc
  void ScaleAxis(float scale);
  SDL_Surface *Surface();
  SDL_Event event;

  static void InitFont();
  static void KillFont();
  static void TextOut(const char *fmt, ...);
  static void TextOut(int x, int y, const char *fmt, ...);
  static void TextOutBase(const char *fmt, va_list argp);
  static Vec3D GetOGLPos(int32_t x, int32_t y);

  static GLuint static_font_base;

  int32_t fov;    // Length of an axis
  int32_t width;  // Width of the window in pixel
  int32_t height; // Height of the window in pixel
  int32_t fps;
  int32_t idx_snapshot;

  Vec3D cam_pos;     // Position of the camera
  Vec3D cam_look_at; // Point at which the camera is aimed
  Vec3D cam_orient;  // orientation of the camera (rotation as it aims at its
                     // target)

  SDL_Surface *p_screen;
  GLuint font_base;
  GLuint tex_star;

  // function pointer for point sprite extension
  PFNGLPOINTPARAMETERFARBPROC gl_ptr_parameter_f_ARB;
  PFNGLPOINTPARAMETERFVARBPROC gl_ptr_parameter_fv_ARB;

  volatile bool draw_stats;
  volatile bool running;

private:
  void InitGL();
  void InitPointSpriteExtension();
};

} // namespace galaxy

#endif // __GALAXY_UI_SDL_WINDOW_H__
