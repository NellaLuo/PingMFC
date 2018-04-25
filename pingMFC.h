// pingMFC.h : main header file for the PINGMFC application
//

#if !defined(AFX_PINGMFC_H__2C31A02C_FE12_4C42_B299_FFF944CE14B1__INCLUDED_)
#define AFX_PINGMFC_H__2C31A02C_FE12_4C42_B299_FFF944CE14B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPingMFCApp:
// See pingMFC.cpp for the implementation of this class
//

class CPingMFCApp : public CWinApp
{
public:
	CPingMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPingMFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINGMFC_H__2C31A02C_FE12_4C42_B299_FFF944CE14B1__INCLUDED_)
