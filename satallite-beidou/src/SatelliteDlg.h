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
	BOOL BISInitial_WX;//�Ƿ��ڳ�ʼ�������У�
	BOOL bISCaller_WX;//�����б�־������TRUE,Ĭ������
	BOOL bIsRecording;//¼����־������¼��ΪTRUE��Ĭ��ΪFALSE
	BOOL bISCallProcess_WX;//�Ƿ���ͨ�������У����б��У���Ĭ��ΪFALSE
//	CString m_strConfig;//��������Ϣ
	void Send_AT_Command_ST(CString msg); //����ATָ��
	int nATCmdFlags;//ATָ��ı�־
	
	#define TMP_SIZE  2048
	char RxData[TMP_SIZE];  //���������ݴ�****
	char TxData[TMP_SIZE];  //�����ַ�����****
	CSatelliteDlg(CWnd* pParent = NULL);	// standard constructor
	void ToTray();//��С��������
	void DeleteTray();//ɾ������ͼ��
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);//�ָ�����

	CRect rectLarge;//ȫ����
	CRect rectSmall;//�绰��
	CRect rectSeparator;

	int m_nBaud;       //������
	int m_nCom;         //���ں�
	char m_cParity;    //У��
	int m_nDatabits;    //����λ
	int m_nStopbits;    //ֹͣλ
	
	CSerialPort m_Port;  //CSerialPort�����
	CStatusBarCtrl m_StatusBar;//״̬��
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
