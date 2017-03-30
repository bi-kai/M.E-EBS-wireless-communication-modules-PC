// ¼��Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "¼��.h"
#include "¼��Dlg.h"
#include "vfw.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
HWND g_hWndMCIWnd;    //��Ϊ���ô�����ȥ��ȫ�ֺ���

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
		MCIWndClose(g_hWndMCIWnd);    //�����Ϊ�˴�һ��������֮ǰ�ر���ǰ������,Ӧ���ȼ�һ���жϺ���.
 g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //�������Ҫ˵�������һ������,�����NULL,���½�һ��,�����һ���ļ���·��,�ʹ���.���Ƕ�Ҫʹ�������MCIWndNew�����ڴ�
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
	MCIWndClose(g_hWndMCIWnd);    //�����Ϊ�˴�һ��������֮ǰ�ر���ǰ������,Ӧ���ȼ�һ���жϺ���.
 g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //�������Ҫ˵�������һ������,�����NULL,���½�һ��,�����һ���ļ���·��,�ʹ���.���Ƕ�Ҫʹ�������MCIWndNew�����ڴ�
 MCIWndNew(g_hWndMCIWnd,"waveaudio");    

}

void CMyDlg::OnBUTTONRec() 
{
	// TODO: Add your control notification handler code here
	if(MCIWndCanRecord(g_hWndMCIWnd))    //�������ж��Ƿ����¼��
 { 
  MCIWndRecord(g_hWndMCIWnd);    //¼�����������,�ܼ�
 }
 else
 {
  AfxMessageBox("error #01");    //������󱨸��������,��������,���������û�����.
   }

}

void CMyDlg::OnBUTTONRecStop() 
{
	// TODO: Add your control notification handler code here
	MCIWndStop(g_hWndMCIWnd);    //����,����ô��

}

void CMyDlg::OnBUTTONPlay() 
{
	// TODO: Add your control notification handler code here
	if(MCIWndCanPlay(g_hWndMCIWnd))    //�����ж���û�п��Բ��ŵĶ���
 {
  MCIWndSeek(g_hWndMCIWnd,1);    //˵һ�������,������ȷ�������￪ʼ����.û��Ҳ���Ծ��Ǵ�ͷ��ʼ����.�����m_Int���������벥�ſ�ʼ���,����ӻ������Ͻ���һ������,��������,�ʹ����￪ʼ������.����������Ǳ�����!
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
  MCIWndSave(g_hWndMCIWnd, "a");    //�����е�����Ҫ˵˵��,MSDN����д,���MCIWndSave����ֱ�ӱ����ļ�,�ڶ�����������һ��-1�ͻ��Զ��򿪱���������ѡ��,����������N�ö�û�㶨,(Ҳ����ֿ���,˭�㶨��mail����һ��)�����򿪱�������,���Ƿ���һ���ɹ�ֵ!��~!����ֱ�ӱ���Ϊ�ļ�,�ļ���ֻҪ����һλ��������,����������������һ��Ȧ,�ȱ����һ��a,��������ΪҪ������ļ���,����ڽ�������ɾ�������a.
  CopyFile("a",m_Path,FALSE);

 }
 else
  AfxMessageBox("error Can`t save");
}



void CMyDlg::OnRestart() 
{
	// TODO: Add your control notification handler code here
	MCIWndClose(g_hWndMCIWnd);
		MCIWndClose(g_hWndMCIWnd);    //�����Ϊ�˴�һ��������֮ǰ�ر���ǰ������,Ӧ���ȼ�һ���жϺ���.
    g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,WS_CAPTION,NULL);    //�������Ҫ˵�������һ������,�����NULL,���½�һ��,�����һ���ļ���·��,�ʹ���.���Ƕ�Ҫʹ�������MCIWndNew�����ڴ�
    MCIWndNew(g_hWndMCIWnd,"waveaudio");    
}
