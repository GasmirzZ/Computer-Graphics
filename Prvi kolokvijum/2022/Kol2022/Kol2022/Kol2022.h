
// Kol2022.h : main header file for the Kol2022 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2022App:
// See Kol2022.cpp for the implementation of this class
//

class CKol2022App : public CWinApp
{
public:
	CKol2022App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2022App theApp;
