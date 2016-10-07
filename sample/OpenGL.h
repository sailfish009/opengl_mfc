#pragma once
class COpenGL : public CWnd
{
public:
  COpenGL();
  virtual ~COpenGL();
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
  HGLRC create_context(HDC hdc);
  void render(unsigned char * buf);
  void initshader();
};

