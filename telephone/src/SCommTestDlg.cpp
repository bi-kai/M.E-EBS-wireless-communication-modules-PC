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
	
	
	DWORD style=WS_VISIBLE|WS_CHILD;	//�ؼ��ɼ�,�Ӵ���
	if(WT_ctrlComm.GetPortOpen())
		WT_ctrlComm.SetPortOpen(FALSE);
	OnBtnGetport();
	WT_ctrlComm.SetInBufferSize(1024);
	WT_ctrlComm.SetOutBufferSize(1024); //��������������Ĵ�С

	if(WT_ctrlComm.GetPortOpen())
		SetDlgItemText(IDC_BTN_OPEN,"�رմ���");//�紮���Ѿ�����ť��ʾ"�رմ���"����
	else
		SetDlgItemText(IDC_BTN_OPEN,"�򿪴���");	
	((CButton*)GetDlgItem(IDC_RADIO_NOPEO))->SetCheck(TRUE);	//��ʼʱ��������ֵ����Ч

	SetTimer(NULL,1000,NULL);	//����ʱ��
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
//�¼���Ӧ
void CSCommTestDlg::OnOnComm() 
{
	UpdateData(TRUE);
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	char rxdata[2048];	//����BYTE����(�ַ�����)
	CString strtemp;
	if(WT_ctrlComm.GetCommEvent() == 2)	//ʱ��ֵΪ2��ʾ���ջ����������ַ�
	{
		char *str_calloff = "ATS2";	//�һ���ʾ
		char *str_ring = "ATRing";	//����,��ʾ������ʾ
		char *str_callon = "ATCID";	//������ʾ,��ʾ�绰����	
		char *str_sigvoice = "ATN";	//������ʾ,һ�δ�һλ
		char *str_listen = "ATZ";	//ժ����ʾ,���к����ʱ�����ȡ
		
		variant_inp = WT_ctrlComm.GetInput();	//��������
		safearray_inp = variant_inp;//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len = safearray_inp.GetOneDimSize();	//�õ���Ч���ݳ���
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������,�����rxdata������
		rxdata[len] = '\0';	//���ַ�����ת�����ַ�����ʽ
		CString temp = rxdata;	
		char *charpoint = temp.GetBuffer(temp.GetLength());	//���ַ���rxdataת��Ϊchar *��ʽ

		//����Ƿ�ΪDTMF�ź�
		if(strncmp(rxdata,str_sigvoice,3) == 0)
			DoDTMF(charpoint);
		//����Ƿ�Ϊ������ʾ
		else if(strncmp(rxdata,str_callon,5) == 0 || (rxdata[0] >= '0' && rxdata[0] <= '9'))
			DoATCIDAndNum(charpoint,str_callon);
		//����Ƿ�Ϊ�һ���ʾ
		else if(strncmp(rxdata,str_calloff,4) == 0)
			DoATH(charpoint);
		//����Ƿ�Ϊ������ʾ��
		else if(strncmp(rxdata,str_ring,6) == 0)
			DoATRing(charpoint);
		//����Ƿ�ժ��
		else if(strncmp(rxdata,str_listen,3) == 0)
			GetTelTime();
	}
	UpdateData(FALSE);//���±༭������
	((CEdit*)GetDlgItem(IDC_EDIT_RXDATA))->SetSel(WT_strRXData.GetLength(), WT_strRXData.GetLength(),FALSE); //ʹ���ָ��༭��ĵ�ǰλ��
}
//DTMF�źŴ�����
void CSCommTestDlg::DoDTMF(char *str_sigvoice)
{
	char *lpOutBuffer = str_sigvoice+3;
	WT_strRXData += lpOutBuffer;	//��ʾ��������
}
//�һ�ָ�����
void CSCommTestDlg::DoATH(char *str_calloff)
{
	WT_strRXData += "call-off";	//�Է��ѹһ�
	char lpOutBuffer[] = {'A','T','H','\r','\n'};//�����ϴ�ATHָ����йһ�
	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));	//��������
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(false);
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(true);
}
//ATCID�źŴ�����
void CSCommTestDlg::DoATCIDAndNum(char *charpoint,char *str_callon)
{
	CString str_time;
	if(strncmp(charpoint,str_callon,5) == 0)
	{
		char *lpOutBuffer = charpoint + 5;
		WT_strRXData += lpOutBuffer;	//��ʾ�������
		WT_str_show = lpOutBuffer;
	}
	else
	{
		WT_strRXData += charpoint;
		WT_str_show += charpoint;
		WT_Time = CTime::GetCurrentTime();	//��ȡ��ǰʱ��
		str_time = WT_Time.Format("%H%M%S");	//�ַ�(��'\0')����Ϊ6
		if(WT_sel_NoPeo)	//����ֵ��RecordTelTime�ĸ�ʽ
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
//���崦����
void CSCommTestDlg::DoATRing(char *str_ring)
{
	WT_strRXData += "Ring";
	PlaySound((LPCSTR)IDR_WAVE2,NULL,SND_ASYNC|SND_RESOURCE);
}
//�������
void CSCommTestDlg::OnBtnClear() 
{
	WT_strRXData.Empty();
	UpdateData(FALSE);
}
//�򿪴���/�رմ�����Ӧ����
void CSCommTestDlg::OnBtnOpen() 
{
	CString str;
	GetDlgItemText(IDC_BTN_OPEN,str);
	if(str == "�򿪴���")
	{
		if(WT_ctrlComm.GetPortOpen())
			WT_ctrlComm.SetPortOpen(FALSE);
		WT_ctrlComm.SetInBufferSize(1024);
		WT_ctrlComm.SetOutBufferSize(1024); //��������������Ĵ�С

		if(!WT_ctrlComm.GetPortOpen())	//��������ǹرյ�,��ִ�д򿪴���
			WT_ctrlComm.SetPortOpen(TRUE);
		else
			AfxMessageBox("cannot open serial port !!");
		WT_ctrlComm.SetSettings("9600,n,8,1"); //������9600����У�飬8������λ��1��ֹͣλ
		WT_ctrlComm.SetRThreshold(1);	//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
	
		WT_ctrlComm.SetInputMode(1);//�Զ����Ʒ�ʽ��д����   ,0���ַ�����ʽ
		WT_ctrlComm.SetInputLen(0);  //���õ�ǰ���������ݳ���Ϊ0
		WT_ctrlComm.GetInput();//��Ԥ���������������������	
		SetDlgItemText(IDC_BTN_OPEN, "�رմ���");
		GetDlgItem(IDC_COM_PORT)->EnableWindow(false);
	}
	else if(str == "�رմ���")
	{
		if(WT_ctrlComm.GetPortOpen())
			WT_ctrlComm.SetPortOpen(FALSE);
		SetDlgItemText(IDC_BTN_OPEN, "�򿪴���");
		GetDlgItem(IDC_COM_PORT)->EnableWindow(true);		
	}
}
//�һ���������
void CSCommTestDlg::sendATH()
{
	CString str = "ATH";
	int len = strlen(str);	//�ַ���str�������ַ�

	char *lpOutBuffer = (char *)malloc((len+2)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer,str,len);	//���ַ���str���Ƹ�lpOutBuffer
	lpOutBuffer[len] = '\r';
	lpOutBuffer[len+1] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));	//��������
	free(lpOutBuffer);
}
//�һ���ť��Ӧ����
void CSCommTestDlg::OnBtnAth() 
{
	sendATH();
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(true);	//ժ����ť��Ч
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(false);	//�һ���ťʧЧ
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(false);	//���Ű�ťʧЧ
	GetDlgItem(IDC_BTN_ATB)->EnableWindow(false);
}
//ժ����������
void CSCommTestDlg::sendATZ()
{
	CString str = "ATZ";
	int len = strlen(str);//�ַ���str�������ַ�

	char *lpOutBuffer = (char *)malloc((len+2)*sizeof(char));
	if(lpOutBuffer == NULL)
		MessageBox("Allocated failed!");
	memcpy(lpOutBuffer,str,len);	//���ַ���str���Ƹ�lpOutBuffer
	lpOutBuffer[len] = '\r';
	lpOutBuffer[len+1] = '\n';

	WT_ctrlComm.SetOutput(COleVariant(lpOutBuffer));//��������
	free(lpOutBuffer);
}
//ժ����ť��Ӧ����
void CSCommTestDlg::OnBtnAtz() 
{
	sendATZ();
	GetDlgItem(IDC_BTN_ATH)->EnableWindow(true);	//�һ���ť��Ч
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(true);	//���Ű�ť��Ч
	GetDlgItem(IDC_BTN_ATB)->EnableWindow(true);
	GetDlgItem(IDC_BTN_ATZ)->EnableWindow(false);	//ժ����ťʧЧ
}
//������������
void CSCommTestDlg::sendATD()
{
	UpdateData(TRUE);	//��ȡ�༭������

	CString str;
	str = WT_strTelNum;
	int len = strlen(str);	//�ַ���str�������ַ�

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
//�绰���ϲ���
void CSCommTestDlg::OnBtnAtd() 
{
	sendATD();
	GetDlgItem(IDC_BTN_ATD)->EnableWindow(false);
}
//������������
void CSCommTestDlg::sendATB()
{
	UpdateData(TRUE);	//��ȡ�༭������

	CString str;
	str = WT_strTelNum;
	int len = strlen(str);	//�ַ���str�������ַ�

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
//���Ű�ť��Ӧ����
void CSCommTestDlg::OnBtnAtb() 
{
	sendATB();
}
//�Զ������˿ں�
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
//��ȡ�˿ںź���
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
//�����˵���Ӧ����
void CSCommTestDlg::OnSelchangeComPort() 
{
	CUIntArray ports;
	EnumerateSerialPorts(ports);
	int curport = WT_PortNum.GetCurSel();
	WT_ctrlComm.SetCommPort(ports.ElementAt(curport));
}
//Radio button��ť����ֵ��
void CSCommTestDlg::OnRadioNopeo() 
{
	GetDlgItem(IDC_EDIT_TXDATA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ATB)->ShowWindow(SW_HIDE);
	WT_sel_NoPeo = true;	//ѡ������ֵ�ص�radio button;
	WT_sel_Peo = false;
}
//Radio button��ť�˹�ֵ��
void CSCommTestDlg::OnRadioPeo() 
{
	GetDlgItem(IDC_EDIT_TXDATA)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATH)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_ATB)->ShowWindow(SW_SHOW);
	WT_sel_Peo = true;	//ѡ���˹�ֵ�ص�radio button
	WT_sel_NoPeo = false;
}
//�ⲿ�ӿں���
void GetTelTime()
{
	CString temp;
	CSCommTestDlg cd;
	temp = cd.RecordTelTime;
	char *charpoint = temp.GetBuffer(temp.GetLength());
}
//��ȡϵͳ��ǰʱ�亯��
void CSCommTestDlg::OnTimer(UINT nIDEvent) 
{
	CString str;
	WT_Time = CTime::GetCurrentTime();	//��ȡ��ǰʱ��
	str = WT_Time.Format("%H:%M:%S");	//�ַ�(��'\0')����Ϊ8
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText("��ǰʱ�䣺"+str);
	CDialog::OnTimer(nIDEvent);
}
