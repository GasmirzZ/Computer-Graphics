
// Kol2023.h : main header file for the Kol2023 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2023App:
// See Kol2023.cpp for the implementation of this class
//

class CKol2023App : public CWinApp
{
public:
	CKol2023App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2023App theApp;
