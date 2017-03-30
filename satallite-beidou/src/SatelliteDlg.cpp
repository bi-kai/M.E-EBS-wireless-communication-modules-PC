// SatelliteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Satellite.h"
#include "SatelliteDlg.h"
//#include "mscomm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
 #define WM_SHOWTASK (WM_USER +1)  //自定义消息

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
// CSatelliteDlg dialog

CSatelliteDlg::CSatelliteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSatelliteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSatelliteDlg)
	m_NumberShow = _T("");
	m_strRXData = _T("");
	m_strTXData = _T("");
	m_cnum = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bOpenPort=FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_SATELLITE);
}

void CSatelliteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSatelliteDlg)
	DDX_Control(pDX, IDC_TXINFORMATION_SHOW, m_ctrlTrsmdmitData);
	DDX_Control(pDX, IDC_RXINFORMATION_SHOW, m_ctrlReceiveData);
	DDX_Control(pDX, IDC_PORT, m_ctrlOpenPort);
	DDX_Text(pDX, IDC_PHONE_EDIT, m_NumberShow);
	DDX_Text(pDX, IDC_RXINFORMATION_SHOW, m_strRXData);
	DDX_Text(pDX, IDC_TXINFORMATION_SHOW, m_strTXData);
	DDX_Text(pDX, IDC_comingnum, m_cnum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSatelliteDlg, CDialog)
	//{{AFX_MSG_MAP(CSatelliteDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PHONEINITIAL, OnPhoneInitial)
	ON_BN_CLICKED(IDC_NUMBER1, OnNumber1)
	ON_BN_CLICKED(IDC_NUMBER2, OnNumber2)
	ON_BN_CLICKED(IDC_NUMBER3, OnNumber3)
	ON_BN_CLICKED(IDC_NUMBER4, OnNumber4)
	ON_BN_CLICKED(IDC_NUMBER5, OnNumber5)
	ON_BN_CLICKED(IDC_NUMBER6, OnNumber6)
	ON_BN_CLICKED(IDC_NUMBER7, OnNumber7)
	ON_BN_CLICKED(IDC_NUMBER8, OnNumber8)
	ON_BN_CLICKED(IDC_NUMBER9, OnNumber9)
	ON_BN_CLICKED(IDC_NUMBER10, OnNumber10)
	ON_BN_CLICKED(IDC_NUMBER0, OnNumber0)
	ON_BN_CLICKED(IDC_NUMBER12, OnNumber12)
	ON_BN_CLICKED(IDC_BTN_DELETELASTNUB, OnBtnDeletelastnub)
	ON_BN_CLICKED(IDC_PORT, OnOffPort)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CALLONOFF, OnCallOnOff)
	ON_BN_CLICKED(IDC_SHOW, OnShowOnOff)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CSQ_btn, OnCSQbtn_Click)
	ON_BN_CLICKED(IDC_RINFO_btn, OnRINFObtn_Click)
	ON_BN_CLICKED(IDC_GPStrack_btn, OnGPStrackbtn_Click)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSatelliteDlg message handlers

BOOL CSatelliteDlg::OnInitDialog()
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
	bIsRecording = FALSE;
	nATCmdFlags = 0;
	bISCallProcess_WX = FALSE;
	bISCaller_WX = TRUE; 
	BISInitial_WX = FALSE;
	BINIfail_WX = FALSE;
	strdata = "";

	memset(&RxData,0,TMP_SIZE);
	memset(&TxData,0,TMP_SIZE);

	m_StatusBar.EnableAutomation();//状态栏
	m_StatusBar.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0);
	int width[]={180,360};//右队齐,开始的长度.
	m_StatusBar.SetParts(2,&width[0]);

	CString StatusText;
	StatusText = "串口未连接";
	m_StatusBar.SetText(StatusText,0,0);
	StatusText = "CSQ:";
	m_StatusBar.SetText(StatusText,0,1);
	m_StatusBar.ShowWindow(SW_SHOW);

	m_nBaud=9600;
	m_nCom=1;
	m_cParity='N';
	m_nDatabits=8;
	m_nStopbits=1;
	coming_num="";
	comingflag=0;
	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	//if (m_Port.InitPort(this, 1, 9600,'N',8,1,dwCommEvents,512))
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		m_Port.StartMonitoring();		
		//m_ctrlIconOpenoff.SetIcon(m_hIconOff);		
		//"当前状态：串口打开，无奇偶校验，8数据位，1停止位");
		StatusText = "串口已连接";
		m_StatusBar.SetText(StatusText,0,0);
		m_StatusBar.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ctrlOpenPort.SetWindowText("打开串口");
		AfxMessageBox("没有发现此串口");
	}

	this->GetDlgItem(IDC_NUMBER10)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_NUMBER12)->EnableWindow(FALSE);

	GetWindowRect(&rectLarge);

	GetDlgItem(IDC_NORMALSHOW)->GetWindowRect(&rectSeparator);
	rectSmall.left=rectSeparator.left;
	rectSmall.top=rectLarge.top;
	rectSmall.right=rectSeparator.right+35;
	rectSmall.bottom=rectLarge.bottom;
	SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE|SWP_NOZORDER);

	GetDlgItem(IDC_SHOWBIG)->GetWindowRect(&rectSeparator);
	rectLarge.left=rectSeparator.left;
	rectLarge.top=rectLarge.top;
	rectLarge.right=rectSeparator.right+50;
	rectLarge.bottom=rectLarge.bottom;



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSatelliteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_MINIMIZE)
	{
		ToTray();	//最小化到托盘
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSatelliteDlg::OnPaint() 
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
HCURSOR CSatelliteDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

 //在桌面右下角显示托盘图标
void CSatelliteDlg::ToTray()
{  
     NOTIFYICONDATA nid;    
     nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);  
     nid.hWnd=this->m_hWnd;     
	 nid.uID=IDI_SATELLITE; //IDR_MAINFRAME;  
     nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
     nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称  
     nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SATELLITE));    
     strcpy(nid.szTip,"程序名称");    //信息提示条    
     Shell_NotifyIcon(NIM_ADD,&nid);    //在托盘区添加图标    
     ShowWindow(SW_HIDE);    //隐藏主窗口
}

//恢复界面函数
LRESULT CSatelliteDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
   if(wParam!=IDI_SATELLITE)    
                   return 1;    
   switch(lParam)  
   {          
      case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”  
		 {
			 LPPOINT lpoint=new tagPOINT;  
			 ::GetCursorPos(lpoint);//得到鼠标位置      
			 CMenu menu;      
			 menu.CreatePopupMenu();//声明一个弹出式菜单        
			 //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口将程序结束。    
			 menu.AppendMenu(MF_STRING,WM_DESTROY,"关闭");             //确定弹出式菜单的位置    
			 menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);             //资源回收        
			 HMENU hmenu=menu.Detach();      
			 menu.DestroyMenu();          
			 delete lpoint;      
		 }           
			break;        
      case WM_LBUTTONDBLCLK://双击左键的处理    
		{  
			this->ShowWindow(SW_SHOW);//显示主窗口            
		   DeleteTray();      
		}       
	       break;        
    default:           
		   break;  

   }    
    return 0;
}

//删除托盘图标函数
void CSatelliteDlg::DeleteTray()
{
    NOTIFYICONDATA nid;  
    nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);  
    nid.hWnd=this->m_hWnd;  
    nid.uID=IDI_SATELLITE;  //IDR_MAINFRAME;  
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
    nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称
    nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SATELLITE));  
    strcpy(nid.szTip,"程序名称");    //信息提示条为“计划任务提醒”  
    Shell_NotifyIcon(NIM_DELETE,&nid);    //在托盘区删除图标
}

BEGIN_EVENTSINK_MAP(CSatelliteDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSatelliteDlg)
//	ON_EVENT(CSatelliteDlg, IDC_MSCOMM1, 1 /* OnComm */, OnComm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//串口事件消息处理函数
LONG CSatelliteDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
     COleSafeArray safearray_inp;
	 int nIndex = 0;//索引
     CString strtemp;
		 strtemp.Format("%c",ch);

		 int nLen=m_ctrlReceiveData.GetWindowTextLength();
		 m_ctrlReceiveData.SetSel(nLen, nLen);//设定选中接收框中的字符，选择末尾，即无字符被选中
		 m_ctrlReceiveData.ReplaceSel(strtemp);//将str内容替换到选中的字符处，即将str内容添加到接收框字符串末尾
		 nLen+=strtemp.GetLength();		 
		 m_strRXData+=strtemp;
		 
		strdata += strtemp;

		if(nATCmdFlags == 100)
		{
			if(strdata.Find("OK")!=-1)
			{
				KillTimer(59);
				SetTimer(50,1000,NULL);	
				nATCmdFlags = 2;
				strdata = "";
			}
		}

		if ((strdata.Find("OK")!=-1)&&(strdata.GetLength()<=3))
		{
			 if ((bISCallProcess_WX == TRUE) && (bISCaller_WX == TRUE))
			{
				ShellExecute(NULL,"open","record.exe",NULL,NULL,SW_SHOWNORMAL);//开始录音
				m_StatusBar.SetText("通话中",0,0);
				m_StatusBar.ShowWindow(SW_SHOW);
			 }
			 strdata="";
		}
		else if (strdata.Find("RING")!=-1) //这个得测试下
		{	
			int i = strdata.Find("RING");
			strdata = strdata.Mid(i,strdata.GetLength()-i);
			//i = strdata.Find("OK");
			if (i!=-1)
			{
				bISCaller_WX = FALSE;
				if (bISCallProcess_WX == FALSE)
				{
					bISCallProcess_WX = TRUE;
					this->GetDlgItem(IDC_NUMBER10)->EnableWindow(TRUE);
					this->GetDlgItem(IDC_NUMBER12)->EnableWindow(TRUE);
					this->GetDlgItem(IDC_CALLONOFF)->SetWindowText("接听");	
				}
				m_StatusBar.SetText("来电",0,0);
				m_StatusBar.ShowWindow(SW_SHOW);
				strdata = "";
			}
		}
		 else if (strdata.Find("CLIP:") != -1)//监听来电
		 {
			int i = strtemp.Find("\"");
			strdata = strdata.Mid(i,strdata.GetLength()-i);
			//i = strdata.Find("OK");
			if (i!=-1&&comingflag==0)
			{
				m_cnum="";
				comingflag=1;
			}
			else if(i!=-1&&comingflag==1)
			{	
				comingflag=0;
				UpdateData(FALSE);
				strdata="";
			}
			if (comingflag==1&&i==-1)
			{
				 UpdateData(TRUE);
				 m_cnum += strtemp;
				 UpdateData(FALSE);
			}
		 }
		else if (strdata.Find("CSQ")!=-1)
		{
			CString str3;
			int	n=strtemp.FindOneOf(":");
			int	k=strtemp.FindOneOf(",");
			str3 = strtemp.Mid(n+2,k-(n+2));
			int j = 0;
			j = atoi(str3);
			SetTimer(52,60000,NULL);//15s查询CSQ，但是在通话过程中不查
			strdata="";
	 //分别加一个卫星电话信号和是否定位的显示框
/*			if ((j<=24)&&(j>0))
			{
				this->GetDlgItem(IDC_WXCSQ)->SetWindowText("卫星信号强度: 差");
			} 
			else if ((j>=24)&&(j<=28))
			{
				this->GetDlgItem(IDC_WXCSQ)->SetWindowText("卫星信号强度: 中");
			}
			else
			{
				if (strtemp.Find("99,")!=-1)
				{
					this->GetDlgItem(IDC_WXCSQ)->SetWindowText("卫星信号未知");
				}
				else
				{
					this->GetDlgItem(IDC_WXCSQ)->SetWindowText("卫星信号强度: 好");
				}
			}*/
			UpdateData(false);
		}
		else if (strdata.Find("RINFO")!=-1)
		{//暂不做什么
			int i = strdata.Find("RINFO");
			strdata = strdata.Mid(i,strdata.GetLength()-i);
			i = strdata.Find("OK");
			if ((i!=-1)||(strdata.Find("RINFO: 2")!=-1))
			{
				i = strdata.Find("2");
				if (i!=-1)
				{
					m_StatusBar.SetText("区域信息成功",0,1);  //最重要的通话标志
					m_StatusBar.ShowWindow(SW_SHOW);
				} 
				else
				{
					m_StatusBar.SetText("区域信息不明",0,1);
					m_StatusBar.ShowWindow(SW_SHOW);
				}
				strdata="";
			}
		}
		else if (strdata.Find("+GPSTRACKD")!=-1)
		{ //先回OK
			int i = strdata.Find("78");
			int j = strdata.Find("48");
			if (i!=-1)
			{
				m_StatusBar.SetText("GPS已定位",0,1);
				m_StatusBar.ShowWindow(SW_SHOW);
				strdata="";
			}
			else if(j!=-1)
			{
				m_StatusBar.SetText("GPS未定位",0,1);
				m_StatusBar.ShowWindow(SW_SHOW);
				strdata="";	
			}
		}
		else if (strdata.Find("+RCIPH:")!=-1)
		{//更新状态栏，已播出
			m_StatusBar.SetText("拨号中",0,0);
			m_StatusBar.ShowWindow(SW_SHOW);
			strdata="";
		}
		else if(strdata.Find("ERROR")!=-1)//初始化返回错误
		{	
			if (BISInitial_WX == TRUE)
			{
				BINIfail_WX = FALSE;
				strdata = "";
			} 
			else
			{
				int i = strdata.Find("OK");
				if (i!=-1)
				{
					CString stra;
					stra.Format("%d",nATCmdFlags);
					AfxMessageBox("AT command "+stra+" error.");
					strdata="";
				}
			}
		
		}
		else if((strdata.Find("OK")!=-1)&&(10 == nATCmdFlags))
		{
			nATCmdFlags = 0;
			BISInitial_WX = FALSE;
			strdata = "";
		}
		else if (strdata.Find("CME ERROR")!=-1)//这里还要改！！！！！！！！！！！！！！
		{
			CString str;
			this->GetDlgItemText(IDC_CALLONOFF,str);
			if (str=="挂机")
			{
				OnCallOnOff();
			}
			AfxMessageBox(strdata);
			strdata ="";
		}
		else if ((strdata.Find("NO CARRIER")!=-1)||(strdata.Find("BUSY")!=-1)||(strdata.Find("NO ANSWER")!=-1))
		{
			CString str;
			this->GetDlgItemText(IDC_CALLONOFF,str);
			if (str=="挂机")
			{
				OnCallOnOff();
			}
			AfxMessageBox("关闭");
			if (bIsRecording)//未关闭就关闭
			{
				bIsRecording = FALSE;
				//关闭录音
				AfxMessageBox("关闭录音2");
				HWND hWnd = ::FindWindow(NULL, _T("record11"));//要改，不是record11
				if (NULL != hWnd) {
					::SendMessage(hWnd, WM_CLOSE, 0, 0);
				}
			}
			strdata="";			
		}

		if(m_ctrlReceiveData.GetLineCount()>400)
		{
			m_strRXData.Empty();
			m_strRXData="***The length of the text is too long, Emptied Automaticly!***";
			UpdateData(FALSE);
		}
	//UpdateData(FALSE); //更新编辑框内容	
	return 0;
}

void CSatelliteDlg::OnPhoneInitial() 
{
	// TODO: Add your control notification handler code here
	//可以考虑用ini存初始化信息//加握手
	nATCmdFlags = 100;
	BISInitial_WX = TRUE;
	BINIfail_WX = TRUE;
	Send_AT_Command_ST("AT");
	SetTimer(59,5000,NULL);
}

void CSatelliteDlg::Send_AT_Command_ST(CString msg)
{
	msg+="\r\n";
	m_strTXData += msg;
	int nLentx = 0;
	if(m_Port.m_hComm==NULL)
	{
		AfxMessageBox("串口没有打开，请打开串口");
		return;
	}
	else
	{
		//m_ctrlComm.SetOutput(COleVariant(Array));//发送数据
		m_Port.WriteToPort((LPCTSTR)msg);	//发送数据
		m_strTXData += "\r\n";
		CString strtemp = msg+"\r\n";
		nLentx=m_ctrlTrsmdmitData.GetWindowTextLength();
		m_ctrlTrsmdmitData.SetSel(nLentx, nLentx);//设定选中接收框中的字符，选择末尾，即无字符被选中
		m_ctrlTrsmdmitData.ReplaceSel(strtemp);//将str内容替换到选中的字符处，即将str内容添加到接收框字符串末尾
		//UpdateData(FALSE);
	}	                        
}

void CSatelliteDlg::OnNumber1() //1
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER1,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber2() //2 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER2,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber3() //3
{
	// TODO: Add your control notification handler code here
		CString str;
	GetDlgItemText(IDC_NUMBER3,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber4() //4
{
	// TODO: Add your control notification handler code here
		CString str;
	GetDlgItemText(IDC_NUMBER4,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber5() //5
{
	// TODO: Add your control notification handler code here
		CString str;
	GetDlgItemText(IDC_NUMBER5,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber6() //6
{
	// TODO: Add your control notification handler code here
		CString str;
	GetDlgItemText(IDC_NUMBER6,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber7() //7
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER7,str);
	m_NumberShow += str;
	UpdateData(FALSE);		
}

void CSatelliteDlg::OnNumber8() //8
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER8,str);
	m_NumberShow += str;
	UpdateData(FALSE);		
}

void CSatelliteDlg::OnNumber9() //9
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER9,str);
	m_NumberShow += str;
	UpdateData(FALSE);		
}

void CSatelliteDlg::OnNumber10() //*
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER10,str);
	m_NumberShow += str;
	UpdateData(FALSE);		
}


void CSatelliteDlg::OnNumber0() //0
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER0,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnNumber12() //  #
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_NUMBER12,str);
	m_NumberShow += str;
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnBtnDeletelastnub() //用户按错，删除前一个字符
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	m_NumberShow.Delete(m_NumberShow.GetLength()-1,1);//或者newStr = oldStr.substring(0,oldStr.length() - 1);
	UpdateData(FALSE);	
}

void CSatelliteDlg::OnOffPort() 
{
	// TODO: Add your control notification handler code here
	CString StatusText;
	m_bOpenPort=!m_bOpenPort;
	if(m_bOpenPort)  //关闭串口
	{	
		m_ctrlOpenPort.SetWindowText("打开串口");
		m_Port.ClosePort();//关闭串口
		StatusText = "串口未连接";
		m_StatusBar.SetText(StatusText,0,0);
	}
	else  //打开串口
	{
		m_ctrlOpenPort.SetWindowText("关闭串口");
		
		if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
		{
			m_Port.StartMonitoring();
			//"当前状态：串口打开，无奇偶校验，8数据位，1停止位");

		StatusText = "串口已连接";
		m_StatusBar.SetText(StatusText,0,0);
		m_StatusBar.ShowWindow(SW_SHOW);
		}
		else
		{			
			StatusText ="串口未连接";
			m_StatusBar.SetText(StatusText,0,0);
			m_StatusBar.ShowWindow(SW_SHOW);
			AfxMessageBox("没有发现此串口或被占用");
		}
	}
}

void CSatelliteDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(BISInitial_WX == TRUE) //继续初始化
	{
			if (nIDEvent==50)
			{
				switch(nATCmdFlags)
				{
					case 1:
						KillTimer(50);
						Send_AT_Command_ST("ATE0");  //关回显
						nATCmdFlags = 2;
						SetTimer(50,1000,NULL);
						break;
					case 2:
						KillTimer(50);
						Send_AT_Command_ST("AT+CLIP=1");  //开来电显示
						nATCmdFlags = 3;
						SetTimer(50,1000,NULL);
						break;
					case 3:
						KillTimer(50);
						Send_AT_Command_ST("AT+GPSSERVICE=1");//开GPS				
						nATCmdFlags = 4;
						SetTimer(50,1000,NULL);
						break;
					case 4:
						KillTimer(50);
						Send_AT_Command_ST("AT+CLVL=4");  //设置loudspeaker声音为4，最大
						nATCmdFlags = 5;
						SetTimer(50,1000,NULL);
						break;
					case 5:
						KillTimer(50);
						Send_AT_Command_ST("AT+CMEE=1");  //错误返回设置：设为返回错误代号
						nATCmdFlags = 6;
						SetTimer(50,1000,NULL);
						break;
					case 6:
						KillTimer(50);
						Send_AT_Command_ST("AT+CSQ");//查询信号质量
						nATCmdFlags = 7;
						SetTimer(50,1000,NULL);
						break;
					case 7:
						KillTimer(50);
						Send_AT_Command_ST("AT+CREG=1");  //查询注册状态的设置，返回状态代号
						nATCmdFlags = 8;
						SetTimer(50,1000,NULL);
						break;
					case 8:
						KillTimer(50);
						Send_AT_Command_ST("AT+GPSTRACK=1");//GPS追踪				
						nATCmdFlags = 9;
						SetTimer(50,1000,NULL);
						break;
					case 9:
						KillTimer(50);
						Send_AT_Command_ST("AT+RINFO");//区域信息
						nATCmdFlags = 10;
		//				SetTimer(50,1000,NULL);
						break;
					case 100:
						nATCmdFlags = 1;
						SetTimer(50,1000,NULL);
						break;
					default :			
						break;
				}
		}
		else
		{
			KillTimer(50);
			SetTimer(55,10000,NULL);
		}
	} 
	else if(nIDEvent==51)
	{
		KillTimer(51);
		nATCmdFlags = 10;
		Send_AT_Command_ST("AT+RINFO");//区域信息
		BINIfail_WX = FALSE;
		BISInitial_WX = FALSE;		
	}
	else if(nIDEvent==52)
	{
		if (bISCallProcess_WX==FALSE)//不在通话过程中
		{
			nATCmdFlags = 7;
			Send_AT_Command_ST("AT+CSQ");//查询信号质量
			SetTimer(51,1000,NULL);
		}
	}
	else if (nIDEvent == 55)
	{
		KillTimer(55);
		bISCallProcess_WX = TRUE;
		BINIfail_WX = TRUE;
		nATCmdFlags = 100;
		SetTimer(50,1000,NULL);	
	}
	else if (nIDEvent == 59)
	{
		BISInitial_WX = TRUE;
		BINIfail_WX = TRUE;
		HWND hWnd = ::FindWindow(NULL, _T("record11"));//关闭录音
		if (NULL != hWnd) {
			::SendMessage(hWnd, WM_CLOSE, 0, 0);
							}
		nATCmdFlags = 100;
		Send_AT_Command_ST("AT");
	}
	CDialog::OnTimer(nIDEvent);
}

void CSatelliteDlg::OnCallOnOff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nCallSatelliteFlags = 0;
	CString str;
	this->GetDlgItemText(IDC_CALLONOFF,str);
	if (str == "拨号")
	{
		nCallSatelliteFlags = 1;
	} 
	else if(str == "挂机")
	{
		nCallSatelliteFlags = 3;
	}
	else if(str == "接听")
	{
		nCallSatelliteFlags = 2;	
	}
	//无虚拟摘机
	//判断
	switch (nCallSatelliteFlags)
	{
		case 1:
			bISCallProcess_WX = TRUE;
			bISCaller_WX = TRUE;
			this->GetDlgItem(IDC_NUMBER10)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_NUMBER12)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_CALLONOFF)->SetWindowText("挂机");
			nATCmdFlags = 15;
			Send_AT_Command_ST("ATD"+m_NumberShow+";");//拨号
			break;
		case 2:	
			this->GetDlgItem(IDC_CALLONOFF)->SetWindowText("挂机");
			nATCmdFlags = 16;
			Send_AT_Command_ST("ATA");
			ShellExecute(NULL,"open","record.exe",NULL,NULL,SW_SHOWNORMAL);	//启动录音
			m_StatusBar.SetText("通话中",0,0);
			m_StatusBar.ShowWindow(SW_SHOW);
			break;
		case 3:
			bISCallProcess_WX = FALSE;			
			this->GetDlgItem(IDC_NUMBER10)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_NUMBER12)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_CALLONOFF)->SetWindowText("拨号");
			nATCmdFlags = 17;
			Send_AT_Command_ST("ATH");
			m_StatusBar.SetText("串口已连接",0,0);
			m_StatusBar.ShowWindow(SW_SHOW);
			break;
	//		ShellExecute(NULL,"open","record.exe",NULL,NULL,SW_SHOWNORMAL);	//启动录音
		default:
			break;
	}
}

void CSatelliteDlg::OnShowOnOff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	this->GetDlgItemText(IDC_SHOW,str);
	if (str == "显示开")
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),SWP_NOMOVE|SWP_NOZORDER);
		this->GetDlgItem(IDC_SHOW)->SetWindowText("显示关");
	} 
	else if(str == "显示关")
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
		this->GetDlgItem(IDC_SHOW)->SetWindowText("显示开");
	}	
}

void CSatelliteDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_strRXData.Empty();
	m_strTXData.Empty();
	UpdateData(FALSE);
}

void CSatelliteDlg::OnCSQbtn_Click() 
{
	// TODO: Add your control notification handler code here
	if(m_Port.m_hComm==NULL)
	{
		AfxMessageBox("串口没有打开，请打开串口");
		return;
	}
	else
	{
		CString msg = "AT+CSQ\r\n";
		m_Port.WriteToPort((LPCTSTR)msg);	//发送数据
	}
}

void CSatelliteDlg::OnRINFObtn_Click() 
{
	// TODO: Add your control notification handler code here
	if(m_Port.m_hComm==NULL)
	{
		AfxMessageBox("串口没有打开，请打开串口");
		return;
	}
	else
	{
		CString msg = "AT+RINFO\r\n";
			m_Port.WriteToPort((LPCTSTR)msg);	//发送数据
	}
}

void CSatelliteDlg::OnGPStrackbtn_Click() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if(m_Port.m_hComm==NULL)
	{
		AfxMessageBox("串口没有打开，请打开串口");
		return;
	}
	else
	{
		CString msg = "AT+GPSTRACK=1\r\n";
		m_Port.WriteToPort((LPCTSTR)msg);	//发送数据
	}
}
