
// Kol2014View.h : interface of the CKol2014View class
//

#pragma once
#include "DImage.h"


class CKol2014View : public CView
{
protected: // create from serialization only
	CKol2014View() noexcept;
	DECLARE_DYNCREATE(CKol2014View)

// Attributes
public:
	CKol2014Doc* GetDocument() const;
	HENHMETAFILE reno;
	DImage* tocak;
	float ugaoRavni = 10;
	float pomerajAuta = 225;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void DrawGround(CDC* pDC, float angle);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2014View();
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

#ifndef _DEBUG  // debug version in Kol2014View.cpp
inline CKol2014Doc* CKol2014View::GetDocument() const
   { return reinterpret_cast<CKol2014Doc*>(m_pDocument); }
#endif

