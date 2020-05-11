#include "sdl-window.h"

#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <GL/glx.h>

#include "absl/strings/match.h"
#include "absl/strings/str_format.h"

#include "galaxy/base/logging.h"
#include "galaxy/base/types.h"

namespace galaxy {

namespace {
constexpr int32_t kNumMaxCharacters = 96;
constexpr int32_t kBaseCharacter = 32;
} // namespace

GLuint SDLWindow::static_font_base = 0;

void SDLWindow::InitFont() {
  Display *dpy;           // current X display
  XFontStruct *font_info; // current font info

  // Storage for 96 characters
  static_font_base = glGenLists(kNumMaxCharacters);

  // Get our current display long enough to get the fonts
  dpy = XOpenDisplay(NULL);

  // Get the font information
  font_info = XLoadQueryFont(
      dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");

  // If the above font didn't exist try one that should
  if (font_info == NULL) {
    font_info = XLoadQueryFont(dpy, "fixed");
    // If that font doesn't exist, something is wrong
    CHECK_NE(font_info, nullptr) << "No X font available.";
  }

  // generate the list
  glXUseXFont(font_info->fid, kBaseCharacter, kNumMaxCharacters,
              static_font_base);

  // Recover some memory
  XFreeFont(dpy, font_info);

  // close the display now that we're done with it
  XCloseDisplay(dpy);
}

void SDLWindow::KillFont() {
  glDeleteLists(static_font_base, kNumMaxCharacters);
}

void SDLWindow::TextOutBase(const char *fmt, va_list argp) {
  constexpr int32_t kMaxTextSize = 1024;
  char text[kMaxTextSize]; // Holds our string
  vsprintf(text, fmt, argp);
  glPushAttrib(GL_LIST_BIT);                     // Pushes the Display List Bits
  glListBase(static_font_base - kBaseCharacter); // Sets base character to 32
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // Draws the text
  glPopAttrib(); // Pops the Display List Bits
}

// Print GL text to the screen
void SDLWindow::TextOut(const char *fmt, ...) {
  // If there's no text, do nothing
  if (fmt == NULL) {
    return;
  }

  // Pointer to our list of elements
  va_list ap;
  // Parses The String For Variables
  va_start(ap, fmt);
  TextOutBase(fmt, ap);
  // Converts Symbols To Actual Numbers
  va_end(ap);
}

void SDLWindow::TextOut(int x, int y, const char *fmt, ...) {
  // If there's no text, do nothing
  if (fmt == NULL) {
    return;
  }

  Vec3D p = GetOGLPos(x, y);
  glRasterPos2f(static_cast<GLfloat>(p.x), static_cast<GLfloat>(p.y));

  // Pointer to our list of elements
  va_list ap;
  // Parses The String For Variables
  va_start(ap, fmt);
  TextOutBase(fmt, ap);
  // Converts Symbols To Actual Numbers
  va_end(ap);
}

// get opengl position from a screen position
// see also:  http://nehe.gamedev.net/article/using_gluunproject/16013/
Vec3D SDLWindow::GetOGLPos(int32_t x, int32_t y) {
  // TODO(Frank): Make this floating point, if possible
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLdouble winX, winY, winZ;
  GLdouble posX, posY, posZ;

  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  winX = static_cast<double>(x);
  winY = static_cast<double>(viewport[3]) - y;
  glReadPixels(x, static_cast<int32_t>(std::round(winY)), 1, 1,
               GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

  gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY,
               &posZ);

  return Vec3D(posX, posY, posZ);
}

SDLWindow::SDLWindow(int32_t width, int32_t height, float axis_len,
                     const string &caption)
    : event(), fov(axis_len), width(0), height(0), fps(0), idx_snapshot(0),
      cam_pos(0, 0, 2), cam_look_at(0, 0, 0), cam_orient(0, 1, 0),
      p_screen(NULL), font_base(0), tex_star(0), draw_stats(false),
      running(true), funcs() {

  CHECK_NE(SDL_Init(SDL_INIT_VIDEO), -1) << SDL_GetError();
  atexit(SDL_Quit);

  // Enable doublebuffer to cap FPS @ monitor refresh rate.
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // More fine grained redering quality.
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  p_screen = SDL_SetVideoMode(width, height, 16, SDL_OPENGL);
  CHECK_NE(p_screen, nullptr) << SDL_GetError();

  SetCaption(caption);

  this->width = width;
  this->height = height;

  InitGL();
}

void SDLWindow::Init() {
  // OpenGL initialization
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_POINT_SPRITE);
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.03f, 0.0f); // black background
  SetCameraOrientation(cam_orient);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

SDLWindow::~SDLWindow() {
  KillFont();
  SDL_Quit();
}

void SDLWindow::InitPointSpriteExtension() {
  const string ext =
      string(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
  CHECK(absl::StrContains(ext, "GL_ARB_point_parameters"))
      << "GL_ARB_point_parameters extension is not present";

  gl_ptr_parameter_f_ARB = reinterpret_cast<PFNGLPOINTPARAMETERFEXTPROC>(
      SDL_GL_GetProcAddress("glPointParameterfARB"));
  gl_ptr_parameter_fv_ARB = reinterpret_cast<PFNGLPOINTPARAMETERFVEXTPROC>(
      SDL_GL_GetProcAddress("glPointParameterfvARB"));

  CHECK_NE(gl_ptr_parameter_f_ARB, nullptr)
      << "GL_EXT_point_parameters function: glPointParameterfARB was not found";
  CHECK_NE(gl_ptr_parameter_fv_ARB, nullptr)
      << "GL_EXT_point_parameters function: glPointParameterfvARB was not "
         "found";

  SDL_Surface *tex;

  // texture loading taken from:
  // http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL
  tex = SDL_LoadBMP("textures/particle.bmp");
  CHECK_NE(tex, nullptr) << "can't load star texture (textures/particle.bmp).";

  // Check that the image's width is a power of 2
  const int32_t tex_w = tex->w;
  CHECK_EQ(tex_w & (tex_w - 1), 0) << "texture width is not a power of 2.";

  // Also check if the height is a power of 2
  const int32_t tex_h = tex->h;
  CHECK_EQ(tex_h & (tex_h - 1), 0) << "texture height is not a power of 2.";

  // get the number of channels in the SDL surface
  GLint num_colors = tex->format->BytesPerPixel;
  GLenum texture_format = GL_RGBA;
  CHECK(num_colors == 3 || num_colors == 4)
      << "Texture is not true color, requires RGBA or RGB";
  if (num_colors == 4) { // contains an alpha channel
    if (tex->format->Rmask == 0x000000ff) {
      texture_format = GL_RGBA;
    } else {
      texture_format = GL_BGRA;
    }
  } else if (num_colors == 3) { // no alpha channel
    if (tex->format->Rmask == 0x000000ff) {
      texture_format = GL_RGB;
    } else {
      texture_format = GL_BGR;
    }
  }

  // Have OpenGL generate a texture object handle for us
  glGenTextures(1, &tex_star);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, tex_star);

  // Set the texture's stretching properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Edit the texture object's image data using the information SDL_Surface
  // gives us
  glTexImage2D(GL_TEXTURE_2D, 0, num_colors, tex_w, tex_h, 0, texture_format,
               GL_UNSIGNED_BYTE, tex->pixels);
}

void SDLWindow::InitGL() // We call this right after our OpenGL window is
                         // created.
{
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f); // black background
  glViewport(0, 0, GetWidth(), GetHeight());

  SDLWindow::InitFont();
  InitPointSpriteExtension();
}

void SDLWindow::SaveToTGA(int idx) {
  if (idx == -1) {
    idx_snapshot++;
  } else {
    idx_snapshot = idx;
  }

  string filename = absl::StrFormat("frame_%05d.tga", idx_snapshot);
  SaveToTGA(filename);
}

void SDLWindow::SaveToTGA(const std::string &name) {
  constexpr int32_t kNumRgbChannels = 3;
  using std::ios;

  int nSize = GetWidth() * GetHeight() * kNumRgbChannels;

  GLubyte pixels[nSize];
  glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);

  const std::string &filename = name;

  std::fstream file(filename.c_str(), ios::out | ios::binary | ios::trunc);
  char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char header[6] = {static_cast<int8_t>(GetWidth() % 256),
                    static_cast<int8_t>(GetWidth() / 256),
                    static_cast<int8_t>(GetHeight() % 256),
                    static_cast<int8_t>(GetHeight() / 256),
                    24,
                    0};
  file.write(TGAheader, sizeof(TGAheader));
  file.write(header, sizeof(header));

  // convert to BGR format
  for (int i = 0; i < nSize; i += kNumRgbChannels) {
    std::swap(pixels[i + 0], pixels[i + 2]);
  }

  file.write(reinterpret_cast<char *>(pixels), nSize);
  file.close();
}

void SDLWindow::ScaleAxis(float scale) {
  fov *= scale;
  AdjustCamera();
}

Vec3D SDLWindow::GetCamPos() const { return cam_pos; }
Vec3D SDLWindow::GetCamOrient() const { return cam_orient; }
Vec3D SDLWindow::GetCamLookAt() const { return cam_look_at; }
void SDLWindow::SetCameraOrientation(const Vec3D &orient) {
  cam_orient = orient;
  AdjustCamera();
}
void SDLWindow::SetCamera(const Vec3D &pos, const Vec3D &look_at,
                          const Vec3D &orient) {
  cam_orient = orient;
  cam_pos = pos;
  cam_look_at = look_at;
  AdjustCamera();
}

void SDLWindow::AdjustCamera() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float l = fov / 2.0;
  glOrtho(-l, l, -l, l, -l, l);
  gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_look_at.x, cam_look_at.y,
            cam_look_at.z, cam_orient.x, cam_orient.y, cam_orient.z);
  glMatrixMode(GL_MODELVIEW);
}

void SDLWindow::SetCaption(const std::string &caption) {
  SDL_WM_SetCaption(caption.c_str(), NULL);
}

float SDLWindow::GetFOV() const { return fov; }
int SDLWindow::GetFPS() const { return fps; }
void SDLWindow::DrawAxis(const Vec2D &origin) {
  glColor3f(static_cast<GLfloat>(0.3f), static_cast<GLfloat>(0.3f),
            static_cast<GLfloat>(0.3f));

  float s = std::pow(10, static_cast<int32_t>(std::round(log10(fov / 2.0))));
  float l = fov / 100;
  float p = 0;

  glPushMatrix();
  glTranslated(origin.x, origin.y, 0);

  for (int i = 0; p < fov; ++i) {
    p += s;

    if (i % 2 == 0) {
      glRasterPos2f(p - l, -4 * l);
      TextOut("%2.0f", p);
    } else {
      glRasterPos2f(p - l, 2 * l);
      TextOut("%2.0f", p);
    }

    glBegin(GL_LINES);
    glVertex3f(p, -l, 0);
    glVertex3f(p, l, 0);

    glVertex3f(-p, -l, 0);
    glVertex3f(-p, 0, 0);
    glVertex3f(-l, p, 0);
    glVertex3f(0, p, 0);
    glVertex3f(-l, -p, 0);
    glVertex3f(0, -p, 0);
    glEnd();
  }

  glBegin(GL_LINES);
  glVertex3f(static_cast<GLfloat>(-fov), 0, 0);
  glVertex3f(static_cast<GLfloat>(fov), 0, 0);
  glVertex3f(0, static_cast<GLfloat>(-fov), 0);
  glVertex3f(0, static_cast<GLfloat>(fov), 0);
  glEnd();

  glPopMatrix();
}

// Main render loop
// Handles Keyevents advances the time and renders the galaxy.
void SDLWindow::MainLoop() {
  int32_t ct = 0;
  float dt = 0;
  time_t t1(time(NULL)), t2;

  while (running) {
    Render();
    PollEvents();
    ++ct;

    t2 = time(NULL);
    dt = difftime(t2, t1);
    if (dt > 1) {
      fps = static_cast<int32_t>(std::round(static_cast<float>(ct) / dt));
      ct = 0;
      t1 = t2;
    }
  }
}

SDL_Surface *SDLWindow::Surface() { return p_screen; }
int32_t SDLWindow::GetWidth() const { return width; }
int32_t SDLWindow::GetHeight() const { return height; }

void SDLWindow::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SetCamera(cam_pos, cam_look_at, cam_orient);
  if (draw_stats) {
    DrawStats();
  }

  for (const auto& func : funcs) {
    func();
  }

  SDL_GL_SwapBuffers();
}

void SDLWindow::DrawStats() {
  float x0 = 10, y0 = 20, dy = 20;
  int line = 0;

  LOG(DEBUG) << "Drawing Stats";

  glColor3f(1, 1, 1);
  TextOut(x0, y0 + dy * line++, "FPS:                %d", GetFPS());
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  TextOut(x0, y0 + dy * line++, "Time:               %s", std::ctime(&now));
  TextOut(x0, y0 + dy * line++, "FoV:                %1.2f pc", fov);
  TextOut(x0, y0 + dy * line++, "Window Size:        (%d, %d)", GetHeight(),
          GetHeight());
  TextOut(x0, y0 + dy * line++, "Camera Position:    %s",
          cam_pos.to_string().c_str());
  TextOut(x0, y0 + dy * line++, "Camera Orientation: %s",
          cam_orient.to_string().c_str());
  TextOut(x0, y0 + dy * line++, "Camera LookAt:      %s",
          cam_look_at.to_string().c_str());
}

void SDLWindow::ExitMainLoop() { running = false; }
void SDLWindow::OnProcessEvents(uint8_t type) {}
void SDLWindow::PollEvents() {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      ExitMainLoop();
      break;

    default:
      OnProcessEvents(event.type);
      break;
    } // switch event type
  }
}

} // namespace galaxy
