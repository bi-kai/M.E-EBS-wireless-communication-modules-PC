// 录音Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "录音.h"
#include "录音Dlg.h"
#include "vfw.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
HWND g_hWndMCIWnd;    //因为懒得传来传去就全局好了

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONInit, OnBUTTONInit)
	ON_BN_CLICKED(IDC_BUTTONRec, OnBUTTONRec)
	ON_BN_CLICKED(IDC_BUTTONRecStop, OnBUTTONRecStop)
	ON_BN_CLICKED(IDC_BUTTONPlay, OnBUTTONPlay)
	ON_BN_CLICKED(IDC_BUTTONSave, OnBUTTONSave)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
		MCIWndClose(g_hWndMCIWnd);    //这个是为了打开一个声音的之前关闭以前的声音,应该先加一个判断好了.
 g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //创建句柄要说的是最后一个参数,如果是NULL,就新建一个,如果是一个文件的路径,就打开它.但是都要使用下面的MCIWndNew来开内存
 MCIWndNew(g_hWndMCIWnd,"waveaudio");    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnBUTTONInit() 
{
	// TODO: Add your control notification handler code here
	MCIWndClose(g_hWndMCIWnd);    //这个是为了打开一个声音的之前关闭以前的声音,应该先加一个判断好了.
 g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //创建句柄要说的是最后一个参数,如果是NULL,就新建一个,如果是一个文件的路径,就打开它.但是都要使用下面的MCIWndNew来开内存
 MCIWndNew(g_hWndMCIWnd,"waveaudio");    

}

void CMyDlg::OnBUTTONRec() 
{
	// TODO: Add your control notification handler code here
	if(MCIWndCanRecord(g_hWndMCIWnd))    //这里是判断是否可以录音
 { 
  MCIWndRecord(g_hWndMCIWnd);    //录音就这个函数,很简单
 }
 else
 {
  AfxMessageBox("error #01");    //这个错误报告很垃圾的,不建议用,不过测试用还可以.
   }

}

void CMyDlg::OnBUTTONRecStop() 
{
	// TODO: Add your control notification handler code here
	MCIWndStop(g_hWndMCIWnd);    //别怀疑,就这么简单

}

void CMyDlg::OnBUTTONPlay() 
{
	// TODO: Add your control notification handler code here
	if(MCIWndCanPlay(g_hWndMCIWnd))    //这里判断有没有可以播放的东西
 {
  MCIWndSeek(g_hWndMCIWnd,1);    //说一下这里吧,这里是确定从哪里开始播放.没有也可以就是从头开始播放.里面的m_Int是用来传入播放开始点的,比如从滑动条上接收一个数字,传到这里,就从这里开始播放了.但这个方法是笨方法!
  MCIWndPlay(g_hWndMCIWnd);
 }
 else
  AfxMessageBox("error no wave");

}

void CMyDlg::OnBUTTONSave() 
{
	// TODO: Add your control notification handler code here
	CString m_Path;
 CFileDialog wav(FALSE,NULL,NULL,OFN_ALLOWMULTISELECT,"wav File (*.mp3)|*.mp3");
 if(wav.DoModal() == IDOK)
 {
  m_Path = wav.GetPathName();
 }
 else
  return;
 if(MCIWndCanSave(g_hWndMCIWnd))
 {
  MCIWndSave(g_hWndMCIWnd, "a");    //这里有点问题要说说了,MSDN里面写,这个MCIWndSave可以直接保存文件,第二个参数传入一个-1就会自动打开保存框给我们选择,但是我试了N久都没搞定,(也请高手看看,谁搞定了mail给我一份)它不打开保存框给我,但是返回一个成功值!靠~!还有直接保存为文件,文件名只要大于一位就是乱码,所以我在这里绕了一个圈,先保存成一个a,拷贝改名为要保存的文件名,最后在结束里面删除掉这个a.
  CopyFile("a",m_Path,FALSE);

 }
 else
  AfxMessageBox("error Can`t save");
}



void CMyDlg::OnRestart() 
{
	// TODO: Add your control notification handler code here
	MCIWndClose(g_hWndMCIWnd);
		MCIWndClose(g_hWndMCIWnd);    //这个是为了打开一个声音的之前关闭以前的声音,应该先加一个判断好了.
    g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //创建句柄要说的是最后一个参数,如果是NULL,就新建一个,如果是一个文件的路径,就打开它.但是都要使用下面的MCIWndNew来开内存
    MCIWndNew(g_hWndMCIWnd,"waveaudio");    
}
