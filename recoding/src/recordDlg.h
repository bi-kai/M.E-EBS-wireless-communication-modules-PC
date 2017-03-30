// recordDlg.h : header file
//

#if !defined(AFX_RECORDDLG_H__C2AB81C9_D91F_4C84_8D24_78EDFFCF6B33__INCLUDED_)
#define AFX_RECORDDLG_H__C2AB81C9_D91F_4C84_8D24_78EDFFCF6B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg dialog

class CRecordDlg : public CDialog
{
// Construction
public:
	CRecordDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordDlg)
	enum { IDD = IDD_RECORD_DIALOG };
		// NOTE: the ClassWizard will add data members here 
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation



	// Generated message map functions
	//{{AFX_MSG(CRecordDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnRec();
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
	HICON m_hIcon;
	BOOL m_record;//是否开始录音
	BOOL m_pause;//是否暂停
	BOOL m_end;//是否结束
	BOOL m_play; //m_play表示是否正在播放

	CTime c_time;
	CTimeSpan c_runtime;
	CTimeSpan c_OverallTime;
	int nTimeLength;//WAV文件播放时间总长
	int nPlayTime;//已播放时长



	CString	m_szDestFileName;
	long	m_lTime;
	CString	m_strFile; 
public:
	CString sDate;
	BOOL bDirectoryExist;//目录是否已经创建
	CString m_dir;//目录文件夹
	void PathSave();
	void PrepareFormat();
	SYSTEMTIME CurTime;
	CString sYear;
	CString sMonth;
	CString sDay;
	CString sDayCompare;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDDLG_H__C2AB81C9_D91F_4C84_8D24_78EDFFCF6B33__INCLUDED_)
