// SCommTestDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_SCOMMTESTDLG_H__478DB428_4E9E_4B77_8BAA_90C3130C29F9__INCLUDED_)
#define AFX_SCOMMTESTDLG_H__478DB428_4E9E_4B77_8BAA_90C3130C29F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSCommTestDlg dialog
void GetTelTime();	//外部接口函数--全局函数声明
class CSCommTestDlg : public CDialog
{
// Construction
public:
	CSCommTestDlg(CWnd* pParent = NULL);	// standard constructor
	CString RecordTelTime;

// Dialog Data
	//{{AFX_DATA(CSCommTestDlg)
	enum { IDD = IDD_SCOMMTEST_DIALOG };
	CComboBox	WT_PortNum;
	CString	WT_strRXData;
	CString	WT_strTelNum;
	CMSComm	WT_ctrlComm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCommTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	BOOL WT_sel_NoPeo;	//radio button的bool型变量
	BOOL WT_sel_Peo;
	CTime WT_Time;		//CTime变量,获取当前时间
	CString WT_str_show;
	// Generated message map functions
	//{{AFX_MSG(CSCommTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnComm();
	afx_msg void OnBtnClear();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnAth();
	afx_msg void OnBtnAtz();
	afx_msg void OnBtnAtd();
	afx_msg void OnBtnAtb();
	afx_msg void OnBtnGetport();
	afx_msg void OnSelchangeComPort();
	afx_msg void OnRadioNopeo();
	afx_msg void OnRadioPeo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void sendATH();	//挂机驱动函数
	afx_msg void sendATZ();	//摘机驱动函数
	afx_msg void sendATD();	//拨号驱动函数
	afx_msg void sendATB();	//传号驱动函数
	afx_msg void DoDTMF(char *str_sigvoice);	//DTMF信号处理函数
	afx_msg void DoATH(char *str_sigvoice);		//挂机指令处理函数
	afx_msg void DoATCIDAndNum(char *charpoint,char *str_callon);	//ATCID信号处理函数
	afx_msg void DoATRing(char *str_ring);	//振铃处理函数
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOMMTESTDLG_H__478DB428_4E9E_4B77_8BAA_90C3130C29F9__INCLUDED_)
