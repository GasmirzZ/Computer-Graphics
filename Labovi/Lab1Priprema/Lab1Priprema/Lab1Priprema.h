
// Lab1Priprema.h : main header file for the Lab1Priprema application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab1PripremaApp:
// See Lab1Priprema.cpp for the implementation of this class
//

class CLab1PripremaApp : public CWinApp
{
public:
	CLab1PripremaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab1PripremaApp theApp;
