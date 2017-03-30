// SatelliteDlg.h : header file
//
//{{AFX_INCLUDES()
//#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_SATELLITEDLG_H__A183B314_2B01_46CF_B712_00B011368B26__INCLUDED_)
#define AFX_SATELLITEDLG_H__A183B314_2B01_46CF_B712_00B011368B26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SerialPort.h"
/////////////////////////////////////////////////////////////////////////////
// CSatelliteDlg dialog

class CSatelliteDlg : public CDialog
{
// Construction
public:
	CString strdata;
	short comingflag;
	CString coming_num;
	BOOL BINIfail_WX;
	BOOL BISInitial_WX;//是否处于初始化过程中，
	BOOL bISCaller_WX;//主被叫标志，主叫TRUE,默认主叫
	BOOL bIsRecording;//录音标志，正在录音为TRUE，默认为FALSE
	BOOL bISCallProcess_WX;//是否处在通话过程中（主叫被叫），默认为FALSE
//	CString m_strConfig;//存配置信息
	void Send_AT_Command_ST(CString msg); //发送AT指令
	int nATCmdFlags;//AT指令的标志
	
	#define TMP_SIZE  2048
	char RxData[TMP_SIZE];  //接受数据暂存****
	char TxData[TMP_SIZE];  //发送字符数据****
	CSatelliteDlg(CWnd* pParent = NULL);	// standard constructor
	void ToTray();//最小化到托盘
	void DeleteTray();//删除托盘图标
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);//恢复界面

	CRect rectLarge;//全部框
	CRect rectSmall;//电话框
	CRect rectSeparator;

	int m_nBaud;       //波特率
	int m_nCom;         //串口号
	char m_cParity;    //校验
	int m_nDatabits;    //数据位
	int m_nStopbits;    //停止位
	
	CSerialPort m_Port;  //CSerialPort类对象
	CStatusBarCtrl m_StatusBar;//状态栏
// Dialog Data
	//{{AFX_DATA(CSatelliteDlg)
	enum { IDD = IDD_SATELLITE_DIALOG };
	CEdit	m_ctrlTrsmdmitData;
	CEdit	m_ctrlReceiveData;
	CButton	m_ctrlOpenPort;
	CString	m_NumberShow;
	CString	m_strRXData;
	CString	m_strTXData;
	CString	m_cnum;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSatelliteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	DWORD m_dwCommEvents;
	BOOL m_bOpenPort;
	// Generated message map functions
	//{{AFX_MSG(CSatelliteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPhoneInitial();
	afx_msg void OnNumber1();
	afx_msg void OnNumber2();
	afx_msg void OnNumber3();
	afx_msg void OnNumber4();
	afx_msg void OnNumber5();
	afx_msg void OnNumber6();
	afx_msg void OnNumber7();
	afx_msg void OnNumber8();
	afx_msg void OnNumber9();
	afx_msg void OnNumber10();
	afx_msg void OnNumber0();
	afx_msg void OnNumber12();
	afx_msg void OnBtnDeletelastnub();
	afx_msg void OnOffPort();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCallOnOff();
	afx_msg void OnShowOnOff();
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	afx_msg void OnButton2();
	afx_msg void OnCSQbtn_Click();
	afx_msg void OnRINFObtn_Click();
	afx_msg void OnGPStrackbtn_Click();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SATELLITEDLG_H__A183B314_2B01_46CF_B712_00B011368B26__INCLUDED_)
