// SCommTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCommTest.h"
#include "SCommTestDlg.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CSCommTestDlg dialog

CSCommTestDlg::CSCommTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCommTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCommTestDlg)
	WT_strRXData = _T("");
	WT_strTelNum = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSCommTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCommTestDlg)
	DDX_Control(pDX, IDC_COM_PORT, WT_PortNum);
	DDX_Text(pDX, IDC_EDIT_RXDATA, WT_strRXData);
	DDX_Text(pDX, IDC_EDIT_TELNUM, WT_strTelNum);
	DDX_Control(pDX, IDC_MSCOMM1, WT_ctrlComm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSCommTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSCommTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_ATH, OnBtnAth)
	ON_BN_CLICKED(IDC_BTN_ATZ, OnBtnAtz)
	ON_BN_CLICKED(IDC_BTN_ATD, OnBtnAtd)
	ON_BN_CLICKED(IDC_BTN_ATB, OnBtnAtb)
	ON_BN_CLICKED(IDC_BTN_GETPORT, OnBtnGetport)
	ON_CBN_SELCHANGE(IDC_COM_PORT, OnSelchangeComPort)
	ON_BN_CLICKED(IDC_RADIO_NOPEO, OnRadioNopeo)
	ON_BN_CLICKED(IDC_RADIO_PEO, OnRadioPeo)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCommTestDlg message handlers

BOOL CSCommTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CSCommTestDlg sample = new CSCommTestDlg;
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
	
	
	DWORD style=WS_VISIBLE|WS_CHILD;	//控件可见,子窗口
	if(WT_ctrlComm.GetPortOpen())
		WT_ctrlComm.SetPortOpen(FALSE);
	OnBtnGetport();
	WT_ctrlComm.SetInBufferSize(1024);
	WT_ctrlComm.SetOutBufferSize(1024); //设置输出缓冲区的大小

	if(WT_ctrlComm.GetPortOpen())
		SetDlgItemText(IDC_BTN_OPEN,"关闭串口");//如串口已经打开则按钮显示"关闭串口"字体
	else
		SetDlgItemText(IDC_BTN_OPEN,"打开串口");	
	((CButton*)GetDlgItem(IDC_RADIO_NOPEO))->SetCheck(TRUE);	//初始时设置无人值守生效

	SetTimer(NULL,1000,NULL);	//设置时间
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSCommTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSCommTestDlg::OnPaint() 
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
HCURSOR CSCommTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CSCommTestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSCommTestDlg)
	ON_EVENT(CSCommTestDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnComm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//事件响应
void CSCommTestDlg::OnOnComm() 
{
	UpdateData(TRUE);
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	char rxdata[2048];	//设置BYTE数组(字符数组)
	CString strtemp;
	if(WT_ctrlComm.GetCommEvent() == 2)	//时间值为2表示接收缓冲区内有字符
	{
		char *str_calloff = "ATS2";	//挂机提示
		char *str_ring = "ATRing";	//振铃,显示来电提示
		char *str_callon = "ATCID";	//来电提示,显示电话号码	
		char *str_sigvoice = "ATN";	//传号提示,一次传一位
		char *str_listen = "ATZ";	//摘机提示,进行号码和时间的提取
		
		variant_inp = WT_ctrlComm.GetInput();	//读缓冲区
		safearray_inp = variant_inp;//VARIANT型变量转换为ColeSafeArray型变量
		len = safearray_inp.GetOneDimSize();	//得到有效数据长度
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组,存放在rxdata数组中
		rxdata[len] = '\0';	//将字符数组转换成字符串形式
		CString temp = rxdata;	
		char *charpoint = temp.GetBuffer(temp.GetLength());	//将字符串rxdata转换为char *形式

		//检测是否为DTMF信号
		if(strncmp(rxdata,str_sigvoice,3) == 0)
			DoDTMF(charpoint);
		//检测是否为来电提示
		else if(strncmp(rxdata,str_callon,5) == 0 || (rxdata[0] >= '0' && rxdata[0] <= '9'))
			DoATCIDAndNum(charpoint,str_callon);
		//检测是否为挂机提示
		else if(strncmp(rxdata,str_calloff,4) == 0)
			DoATH(charpoint);
		//检测是否为来电提示音
		else if(strncmp(rxdata,str_ring,6) == 0)
			DoATRing(charpoint);
		//检测是否摘机
		else if(strncmp(rxdata,str_listen,3) == 0)
			GetTelTime();
	}
	UpdateData(FALSE);//更新编辑框内容
	((CEdit*)GetDlgItem(IDC_EDIT_RXDATA))->SetSel(WT_strRXData.GetLength(), WT_strRXData.GetLength(),FALSE); //使光标指向编辑框的当前位置
}
//DTMF信号处理函数
void CSCommTestDlg::DoDTMF(char *str_sigvoice)
{
	char *lpOutBuffer = str_sigvoice+3;
	WT_strRXData += lpOutBuffer;	//显示所传号码
}
//挂机指令处理函数
void CSCommTestDlg::DoATH(char *str_calloff)
{
	WT_strRXData += "call-off";	//对方已挂机
	char lpOutBuffer[] = {'A','T','H','\r','\n'};//接着上传ATH指令进行挂机
	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));	//发送数据
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(false);
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(true);
}
//ATCID信号处理函数
void CSCommTestDlg::DoATCIDAndNum(char *charpoint,char *str_callon)
{
	CString str_time;
	if(strncmp(charpoint,str_callon,5) == 0)
	{
		char *lpOutBuffer = charpoint + 5;
		WT_strRXData += lpOutBuffer;	//显示来电号码
		WT_str_show = lpOutBuffer;
	}
	else
	{
		WT_strRXData += charpoint;
		WT_str_show += charpoint;
		WT_Time = CTime::GetCurrentTime();	//获取当前时间
		str_time = WT_Time.Format("%H%M%S");	//字符(除'\0')长度为6
		if(WT_sel_NoPeo)	//无人值守RecordTelTime的格式
		{
			RecordTelTime = WT_str_show + " " + str_time + "NoPeople";
			MessageBox(RecordTelTime);
			sendATZ();
		}
		else
		{
			RecordTelTime = WT_str_show + " " + str_time + "People";
		}
	}
}
//振铃处理函数
void CSCommTestDlg::DoATRing(char *str_ring)
{
	WT_strRXData += "Ring";
	PlaySound((LPCSTR)IDR_WAVE2,NULL,SND_ASYNC|SND_RESOURCE);
}
//清除窗口
void CSCommTestDlg::OnBtnClear() 
{
	WT_strRXData.Empty();
	UpdateData(FALSE);
}
//打开串口/关闭串口响应函数
void CSCommTestDlg::OnBtnOpen() 
{
	CString str;
	GetDlgItemText(IDC_BTN_OPEN,str);
	if(str == "打开串口")
	{
		if(WT_ctrlComm.GetPortOpen())
			WT_ctrlComm.SetPortOpen(FALSE);
		WT_ctrlComm.SetInBufferSize(1024);
		WT_ctrlComm.SetOutBufferSize(1024); //设置输出缓冲区的大小

		if(!WT_ctrlComm.GetPortOpen())	//如果串口是关闭的,则执行打开串口
			WT_ctrlComm.SetPortOpen(TRUE);
		else
			AfxMessageBox("cannot open serial port !!");
		WT_ctrlComm.SetSettings("9600,n,8,1"); //波特率9600，无校验，8个数据位，1个停止位
		WT_ctrlComm.SetRThreshold(1);	//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	
		WT_ctrlComm.SetInputMode(1);//以二进制方式读写数据   ,0是字符串格式
		WT_ctrlComm.SetInputLen(0);  //设置当前接收区数据长度为0
		WT_ctrlComm.GetInput();//先预读缓冲区以清除残留数据	
		SetDlgItemText(IDC_BTN_OPEN, "关闭串口");
		GetDlgItem(IDC_COM_PORT)->EnableWindow(false);
	}
	else if(str == "关闭串口")
	{
		if(WT_ctrlComm.GetPortOpen())
			WT_ctrlComm.SetPortOpen(FALSE);
		SetDlgItemText(IDC_BTN_OPEN, "打开串口");
		GetDlgItem(IDC_COM_PORT)->EnableWindow(true);		
	}
}
//挂机驱动函数
void CSCommTestDlg::sendATH()
{
	CString str = "ATH";
	int len = strlen(str);	//字符串str包含的字符

	char *lpOutBuffer = (char *)malloc((len+2)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer,str,len);	//将字符串str复制给lpOutBuffer
	lpOutBuffer[len] = '\r';
	lpOutBuffer[len+1] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));	//发送数据
	free(lpOutBuffer);
}
//挂机按钮响应函数
void CSCommTestDlg::OnBtnAth() 
{
	sendATH();
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(true);	//摘机按钮生效
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(false);	//挂机按钮失效
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(false);	//拨号按钮失效
	GetDlgItem(IDC_BTN_ATB)->EnableWindow(false);
}
//摘机驱动函数
void CSCommTestDlg::sendATZ()
{
	CString str = "ATZ";
	int len = strlen(str);//字符串str包含的字符

	char *lpOutBuffer = (char *)malloc((len+2)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer,str,len);	//将字符串str复制给lpOutBuffer
	lpOutBuffer[len] = '\r';
	lpOutBuffer[len+1] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));//发送数据
	free(lpOutBuffer);
}
//摘机按钮响应函数
void CSCommTestDlg::OnBtnAtz() 
{
	sendATZ();
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(true);	//挂机按钮生效
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(true);	//拨号按钮生效
	GetDlgItem(IDC_BTN_ATB)->EnableWindow(true);
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(false);	//摘机按钮失效
}
//拨号驱动函数
void CSCommTestDlg::sendATD()
{
	UpdateData(TRUE);	//读取编辑框内容

	CString str;
	str = WT_strTelNum;
	int len = strlen(str);	//字符串str包含的字符

	char *lpOutBuffer = (char *)malloc((len+5)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer+3,str,len);
	lpOutBuffer[0] = 'A';
	lpOutBuffer[1] = 'T';
	lpOutBuffer[2] = 'D';
	lpOutBuffer[len+3] = '\r';
	lpOutBuffer[len+4] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));
	free(lpOutBuffer);
}
//电话线上拨号
void CSCommTestDlg::OnBtnAtd() 
{
	sendATD();
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(false);
}
//传号驱动函数
void CSCommTestDlg::sendATB()
{
	UpdateData(TRUE);	//读取编辑框内容

	CString str;
	str = WT_strTelNum;
	int len = strlen(str);	//字符串str包含的字符

	char *lpOutBuffer = (char *)malloc((len+5)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer+3,str,len);
	lpOutBuffer[0] = 'A';
	lpOutBuffer[1] = 'T';
	lpOutBuffer[2] = 'B';
	lpOutBuffer[len+3] = '\r';
	lpOutBuffer[len+4] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));
	free(lpOutBuffer);
}
//传号按钮响应函数
void CSCommTestDlg::OnBtnAtb() 
{
	sendATB();
}
//自动搜索端口号
void EnumerateSerialPorts(CUIntArray& ports)
{
	ports.RemoveAll();

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bGetVer = GetVersionEx(&osvi);

	if(bGetVer && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL,szDevices,65535);
		if(dwChars)
		{
			int i=0;
			for(;;)
			{
				TCHAR *pszCurrentDevice = &szDevices[i];
				int nLen = _tcslen(pszCurrentDevice);
				if(nLen > 3 && _tcsnicmp(pszCurrentDevice,_T("COM"),3) == 0)
				{
					int nPort = _ttoi(&pszCurrentDevice[3]);
					ports.Add(nPort);
				}
				while(szDevices[i] != _T('\0'))
					i++;
				i++;
				if(szDevices[i] == _T('\0'))
					break;
			}
		}
		else
			TRACE(_T("Failed in call to QueryDosDevice,GetLastError:%d\n"),GetLastError());
	}
	else
	{}
}
//获取端口号函数
void CSCommTestDlg::OnBtnGetport() 
{
	CUIntArray ports;
	CString s;
	EnumerateSerialPorts(ports);
	for(int i=0; i<ports.GetSize(); i++)
	{
		CString res = " ";
		s.Format("%d",ports.ElementAt(i));
		res = "COM" + s;
		WT_PortNum.AddString(res);
	}
	WT_PortNum.SetCurSel(0);
}
//下拉菜单响应函数
void CSCommTestDlg::OnSelchangeComPort() 
{
	CUIntArray ports;
	EnumerateSerialPorts(ports);
	int curport = WT_PortNum.GetCurSel();
	WT_ctrlComm.SetCommPort(ports.ElementAt(curport));
}
//Radio button按钮无人值守
void CSCommTestDlg::OnRadioNopeo() 
{
	GetDlgItem(IDC_EDIT_TXDATA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATB)->ShowWindow(SW_HIDE);
	WT_sel_NoPeo = true;	//选中无人值守的radio button;
	WT_sel_Peo = false;
}
//Radio button按钮人工值守
void CSCommTestDlg::OnRadioPeo() 
{
	GetDlgItem(IDC_EDIT_TXDATA)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATH)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATB)->ShowWindow(SW_SHOW);
	WT_sel_Peo = true;	//选中人工值守的radio button
	WT_sel_NoPeo = false;
}
//外部接口函数
void GetTelTime()
{
	CString temp;
	CSCommTestDlg cd;
	temp = cd.RecordTelTime;
	char *charpoint = temp.GetBuffer(temp.GetLength());
}
//提取系统当前时间函数
void CSCommTestDlg::OnTimer(UINT nIDEvent) 
{
	CString str;
	WT_Time = CTime::GetCurrentTime();	//获取当前时间
	str = WT_Time.Format("%H:%M:%S");	//字符(除'\0')长度为8
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText("当前时间："+str);
	CDialog::OnTimer(nIDEvent);
}
