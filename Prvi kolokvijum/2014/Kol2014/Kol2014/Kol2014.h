
// Kol2014.h : main header file for the Kol2014 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2014App:
// See Kol2014.cpp for the implementation of this class
//

class CKol2014App : public CWinApp
{
public:
	CKol2014App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2014App theApp;
