// recordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "record.h"
#include "recordDlg.h"

#include "DeCodeWav.h"
#include "CWaveFile.h"

#define DllExport __declspec(dllexport)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

extern WAVEFORMATEX wfm;
extern WriteParams params;

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
// CRecordDlg dialog

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_record =FALSE;
	m_pause = FALSE;
	m_play = FALSE;
	m_end = TRUE;

	c_time = 0;//当前时间
	c_runtime = 0;//运行时间
	c_OverallTime = 0;//总时间
	nTimeLength = 0;
	nPlayTime = 0;
	bDirectoryExist = FALSE;

	m_szDestFileName = _T("");
	m_lTime=0;
	m_strFile = _T("source.wav");
	m_dir="D:\\Record\\";

}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REC_START, OnRec)
	ON_BN_CLICKED(IDC_PLAY_START, OnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg message handlers

BOOL CRecordDlg::OnInitDialog()
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

	params.pwf=&wfm;
	((CWnd *)(this->GetDlgItem(IDC_STOP)))->EnableWindow(FALSE);
	PathSave();



		
		 
		 GetLocalTime(&CurTime);
	 
			 
			 sYear.Format("%d",CurTime.wYear);
			 if (CurTime.wMonth>=10)//创建日期目录-----年月日
			 {
				sMonth.Format("%d",CurTime.wMonth);
			 } 
			 else
			 {
				 sMonth.Format("0%d",CurTime.wMonth);
			 }
			 if (CurTime.wDay>=10)
			 {
				sDay.Format("%d",CurTime.wDay);
			 } 
			 else
			 {
				sDay.Format("0%d",CurTime.wDay);
			 }	
			 sDayCompare = sDay;
			 sDate =  sYear+ sMonth + sDay;

			this->m_dir += sDate;
			PathSave();//存储路径改变
			this->bDirectoryExist = TRUE;

		OnRec();//调用程序，即开始录音

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRecordDlg::OnPaint() 
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
HCURSOR CRecordDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CRecordDlg::OnRec() 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("Record start.");
	m_record = TRUE;
	m_end = FALSE;
	if(!m_pause || !m_end)
	{
	}
	UpdateData(TRUE);

	if(m_record)
	{
	//	CString strPath;
	//	GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
	//	strPath.ReleaseBuffer(); 
	//	m_szSrcFileName=strPath+"\\"+m_strFile; 

		 
//		 SYSTEMTIME CurTime;
		 GetLocalTime(&CurTime);
		
		 if (CurTime.wDay>=10)
		 {
			sDayCompare.Format("%d",CurTime.wDay);
		 } 
		 else
		 {
			sDayCompare.Format("0%d",CurTime.wDay);
		 }	

		 if (sDay == sDayCompare)
		 {
			this->bDirectoryExist=TRUE;
//			AfxMessageBox(sDayCompare+"&");
		 }
		else
		{
			this->bDirectoryExist=FALSE;
			this->m_dir="D:\\Record\\";
			sDay = sDayCompare;
//			AfxMessageBox(sDayCompare+"*");			
		}
//		AfxMessageBox(sDay);
			 
		if (this->bDirectoryExist==FALSE)//判断目录是否存在，若不存在，则创建
		{
		
			 sDate =  sYear+ sMonth + sDay;

			this->m_dir += sDate;
			PathSave();//存储路径改变
			this->bDirectoryExist = TRUE;
		}

	//	GetLocalTime(&CurTime);




		CString sHour,sMinute;//创建文件名
		CString sTime;
		if (CurTime.wHour>=10)//创建文件名----时分
		 {
			sHour.Format("%d",CurTime.wHour);
		 } 
		 else
		 {
			 sHour.Format("0%d",CurTime.wHour);
		 }
		if (CurTime.wMinute>=10)
		 {
			sMinute.Format("%d",CurTime.wMinute);
		 } 
		 else
		 {
			 sMinute.Format("0%d",CurTime.wMinute);
		 }	 
		 sTime = sHour+sMinute;

		
		m_szDestFileName= this->m_dir+"\\"+sTime+".mp3";


		LoadMp3DllFunc();
		PrepareFormat();
		CreateWaveFile((LPSTR)(LPCSTR)m_strFile,&params);
	//	PrepareEncodeMp3 ( m_szDestFileName, ENUM_FILE_CHANNEL_COMMON );
		PrepareEncodeMp3 ( m_szDestFileName, ENUM_FILE_CHANNEL_LEFT );

		SetTimer(11,1000,NULL);
		c_time = CTime::GetCurrentTime();
		m_lTime=0;
		RecordStart();

	}

	return TRUE;
}

void CRecordDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
	m_play = TRUE;
	m_record = FALSE;
	m_end = FALSE;	
	nPlayTime = 0;
	nTimeLength = GetTimeLength(m_strFile);

	((CWnd *)(this->GetDlgItem(IDC_REC_START)))->EnableWindow(FALSE);
	((CWnd *)(this->GetDlgItem(IDC_STOP)))->EnableWindow(TRUE);
	((CWnd *)(this->GetDlgItem(IDC_PLAY_START)))->EnableWindow(FALSE);

	LPSTR pData;
	LONG len;
	UpdateData(TRUE);
	if(!m_pause)
	{
		c_OverallTime = 0;
		SetTimer(11,1000,NULL);
		c_time = CTime::GetCurrentTime();
		int bPlay = PlayStart(m_strFile,&wfm,&pData,&len);
		if(bPlay == 0)
		{
			AfxMessageBox("文件播放不成功。");
			OnStop();
		}
	}
}

void CRecordDlg::OnPause() 
{
	// TODO: Add your control notification handler code here
		if(!m_record && !m_play)
		return ;

	if (!m_pause) //暂停第1下
	{
		KillTimer(11);
		c_runtime = c_OverallTime;
		SetDlgItemText(IDC_PAUSE,"continue");
		m_pause = TRUE;
		if(m_record)
		{
			RecordPause();//这里加个Record暂停
		}else if(m_play)
		{
			PlayPause();//这里加个Play暂停
		}		
	}
	else//继续第2下
	{
		SetTimer(11,1000,NULL);
		c_time = CTime::GetCurrentTime();
		SetDlgItemText(IDC_PAUSE,"Pause");
		m_pause = FALSE;

		if(m_record)
		{
			RecordPauseStart();//录音继续
		}else if(m_play)
		{
			PlayPauseStart();//播放继续
		}		
	}	
}

void CRecordDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	m_end =TRUE;
	KillTimer(11);
	
	if(m_pause)
	{
		((CWnd*)(this->GetDlgItem(IDC_PAUSE)))->SetWindowText("Pause");
			m_pause = FALSE;
	}
	if(m_record)
	{
		RecordEnd();//停止录音，关闭输入设备
		m_record = FALSE;
	//	Sleep(100);
		((CWnd *)(this->GetDlgItem(IDC_REC_START)))->EnableWindow(TRUE);
		((CWnd *)(this->GetDlgItem(IDC_STOP)))->EnableWindow(FALSE);
		((CWnd *)(this->GetDlgItem(IDC_PLAY_START)))->EnableWindow(TRUE);
		((CWnd *)GetDlgItem(IDC_SHOW))->SetWindowText("00:00:00");
		UpdateData(FALSE);

	}else if(m_play)
	{		
		PlayEnd(); //停止回放，关闭输出设备
		m_play = FALSE;
	//	Sleep(100);
		((CWnd *)(this->GetDlgItem(IDC_REC_START)))->EnableWindow(TRUE);
		((CWnd *)(this->GetDlgItem(IDC_STOP)))->EnableWindow(FALSE);
		((CWnd *)(this->GetDlgItem(IDC_PLAY_START)))->EnableWindow(TRUE);
		((CWnd *)GetDlgItem(IDC_SHOW))->SetWindowText("00:00:00");
		UpdateData(FALSE);
	}
}


void CRecordDlg::PrepareFormat()
{
	wfm.wFormatTag=WAVE_FORMAT_PCM;  //PCM编码
//	wfm.nChannels=2; //双声道
	wfm.nChannels = 1;
//	wfm.nSamplesPerSec= 44100;//采样频率，每秒采集次数
	wfm.nSamplesPerSec= 8000;
	wfm.wBitsPerSample=16; //采样位，模拟信号转数字信号的精准度
	wfm.nBlockAlign= wfm.nChannels * wfm.wBitsPerSample / 8;
	wfm.nAvgBytesPerSec=wfm.nSamplesPerSec * wfm.nBlockAlign;//每秒数据量
	wfm.cbSize=0;//PCM编码时，此处为0
}

void CRecordDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

	CString str;
	if(m_record)
	{
		c_OverallTime = CTime::GetCurrentTime()- c_time +c_runtime;//时间 差
		str.Format("%02d:%02d:%02d",c_OverallTime.GetHours(),c_OverallTime.GetMinutes(),c_OverallTime.GetSeconds());
		((CWnd *)GetDlgItem(IDC_SHOW))->SetWindowText(str);
		if (c_OverallTime.GetHours() == 1)//一个小时存一下
//		if(c_OverallTime.GetMinutes() == 2)
//			if(c_OverallTime.GetSeconds() == 31)
		{
			OnStop();
			OnRec();
		}
		UpdateData(FALSE);
	}
	if(m_play) 
	{	
		if(nPlayTime< nTimeLength)
		{
			++nPlayTime;
			str.Format("%02d:%02d:%02d",nPlayTime/3600,(nPlayTime%3600)/60,(nPlayTime%3600)%60);
			this->GetDlgItem(IDC_SHOW)->SetWindowText(str);
			UpdateData(FALSE);
		}
		if(nPlayTime == nTimeLength)
		{
			this->GetDlgItem(IDC_REC_START)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_PLAY_START)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_SHOW)->SetWindowText("00:00:00");
			UpdateData(FALSE);
			KillTimer(11);
		}else 
		{	
		}
	}
}

void CRecordDlg::PathSave()
{
	CreateDirectory(this->m_dir,NULL);       
	//如果需要复杂的自动化生成文件名，可能需要提取日期数据加在文件名中
	this->GetDlgItem(IDC_PATH_SHOW)->SetWindowText(m_dir);
}

void CRecordDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_record || m_play)
	{
		OnStop();
	}
}


void CRecordDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_record || m_play)
	{
		OnStop();
	}
	CDialog::OnClose();
}


