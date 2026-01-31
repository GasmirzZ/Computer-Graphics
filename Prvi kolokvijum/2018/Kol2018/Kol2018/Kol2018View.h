
// Kol2018View.h : interface of the CKol2018View class
//

#pragma once
#include "DImage.h"


class CKol2018View : public CView
{
protected: // create from serialization only
	CKol2018View() noexcept;
	DECLARE_DYNCREATE(CKol2018View)

// Attributes
public:
	CKol2018Doc* GetDocument() const;
	DImage* bager;
	DImage* arm1;
	DImage* arm2;
	DImage* pozadina;
	HENHMETAFILE viljuska;
	float angle1 = -90;
	float angle2 = -90;
	float angle3 = -90;
	float pomeraj = 0;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawExcavator(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawBody(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2018View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kol2018View.cpp
inline CKol2018Doc* CKol2018View::GetDocument() const
   { return reinterpret_cast<CKol2018Doc*>(m_pDocument); }
#endif

