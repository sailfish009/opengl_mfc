#include "stdafx.h"
#include "OpenGL.h"


COpenGL::COpenGL()
{
}


COpenGL::~COpenGL()
{
}


BEGIN_MESSAGE_MAP(COpenGL, CWnd)
  ON_WM_CREATE()
END_MESSAGE_MAP()

HGLRC COpenGL::create_context(HDC hdc)
{
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
    1,                                // version number
    PFD_DRAW_TO_WINDOW |              // support window
    PFD_SUPPORT_OPENGL |              // support OpenGL
    PFD_DOUBLEBUFFER,                 // double buffered
    PFD_TYPE_RGBA,                    // RGBA type
    24,                               // 24-bit color depth
    0, 0, 0, 0, 0, 0,                 // color bits ignored
    0,                                // no alpha buffer
    0,                                // shift bit ignored
    0,                                // no accumulation buffer
    0, 0, 0, 0,                       // accum bits ignored
    32,                               // 32-bit z-buffer
    0,                                // no stencil buffer
    0,                                // no auxiliary buffer
    PFD_MAIN_PLANE,                   // main layer
    0,                                // reserved
    0, 0, 0                           // layer masks ignored
  };

  int  iPixelFormat;

  if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
  {
    return 0;
  }

  // make that match the device context's current pixel format
  if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
  {
    return 0;
  }

  HGLRC ret = wglCreateContext(hdc);
  wglMakeCurrent(hdc, ret);

  return ret;
}


int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  Add your specialized creation code here

  return 0;
}

void COpenGL::initshader()
{
}

void COpenGL::render(unsigned char * buf)
{
}


