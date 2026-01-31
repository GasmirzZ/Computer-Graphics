
// Kol2024A.h : main header file for the Kol2024A application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2024AApp:
// See Kol2024A.cpp for the implementation of this class
//

class CKol2024AApp : public CWinApp
{
public:
	CKol2024AApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2024AApp theApp;
