// record.h : main header file for the RECORD application
//

#if !defined(AFX_RECORD_H__95E9CB52_2C5F_4EA3_8A7E_9135B1713758__INCLUDED_)
#define AFX_RECORD_H__95E9CB52_2C5F_4EA3_8A7E_9135B1713758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRecordApp:
// See record.cpp for the implementation of this class
//

class CRecordApp : public CWinApp
{
public:
	CRecordApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRecordApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORD_H__95E9CB52_2C5F_4EA3_8A7E_9135B1713758__INCLUDED_)
