
// sampleDlg.h : header file
//

#pragma once

#include "OpenGL.h"

// CsampleDlg dialog
class CsampleDlg : public CDialogEx
{
// Construction
public:
	CsampleDlg(CWnd* pParent = NULL);	// standard constructor
  static HGLRC context;
  static COpenGL *m_vw;
  static bool b_display_start;
  void display(bool* b_start, UINT8 max_ch);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnClose();
  afx_msg void OnBnClickedOk();
};
