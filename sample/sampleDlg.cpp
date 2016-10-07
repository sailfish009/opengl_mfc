
// sampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sample.h"
#include "sampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
#include <mutex>
#include <condition_variable>
#include <thread>
std::mutex g_mutex;
std::condition_variable g_cv;
bool CsampleDlg::b_display_start;
COpenGL *CsampleDlg::m_vw;
HGLRC CsampleDlg::context;
void display_proc(bool* b_start, UINT8 max_ch);
FILE *fp = nullptr;
uint8_t buf[2048*540];
/////////////////////////////////////////////////////////////////////////////


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsampleDlg dialog



CsampleDlg::CsampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAMPLE_DIALOG, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_vw = new COpenGL;
}

void CsampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_CLOSE()
  ON_BN_CLICKED(IDOK, &CsampleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CsampleDlg message handlers

BOOL CsampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

  //fp = fopen("nv12.yuv", "rb");

  RECT temp_rect;
  GetClientRect(&temp_rect);

  CRect rect(0, 0, temp_rect.right, temp_rect.bottom - 40);

  m_vw->Create(NULL,   //CWnd default
    NULL,   //has no name
    WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
    rect,
    this,   //this is the parent
    0);     //this should really be a different number... check resource.h



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CsampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CsampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CsampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsampleDlg::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(context);

  //if (fp != nullptr) fclose(fp);
  if (m_vw) delete m_vw;

  CDialogEx::OnClose();
}


void CsampleDlg::OnBnClickedOk()
{
  switch (b_display_start)
  {
  case true:
  {
    //fseek(fp, 0, SEEK_SET);
    {
      std::lock_guard<std::mutex> lock(g_mutex);
      b_display_start = false;
    }
    g_cv.notify_one();
    GetDlgItem(IDOK)->SetWindowTextW(L"Start");
  }
  break;
  default:
  {
    b_display_start = true;
    display(&b_display_start, 1);
    GetDlgItem(IDOK)->SetWindowTextW(L"Stop");
  }
  break;
  }
}

void CsampleDlg::display(bool* b_start, UINT8 max_ch) {
  std::thread display_thread(display_proc, std::ref(b_start), max_ch);
  display_thread.detach();
}

void display_proc(bool* b_start, UINT8 max_ch) {
  CsampleDlg::context =
    CsampleDlg::m_vw->create_context(::GetDC(CsampleDlg::m_vw->m_hWnd));

  CsampleDlg::m_vw->initshader();

  std::unique_lock<std::mutex> lock(g_mutex);
  while (*b_start) {
    //fread(buf, 1, 2048 * 540, fp);
    CsampleDlg::m_vw->render(buf);
    SwapBuffers(::GetDC(CsampleDlg::m_vw->m_hWnd));
    g_cv.wait_for(lock, std::chrono::milliseconds(33));
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(CsampleDlg::context);
}