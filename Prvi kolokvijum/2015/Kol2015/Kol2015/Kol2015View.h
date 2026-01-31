
// Kol2015View.h : interface of the CKol2015View class
//

#pragma once
#include "DImage.h"

class CKol2015View : public CView
{
protected: // create from serialization only
	CKol2015View() noexcept;
	DECLARE_DYNCREATE(CKol2015View)

// Attributes
public:
	CKol2015Doc* GetDocument() const;
	DImage* blue;
	int pacmanOrientation = -1;
	float angleParam = 1;
	int pomerajx = 0;
	int pomerajy = 0;
	int pacmanx;
	int pacmany;
	int ghostx;
	int ghosty;
	int pacmanr;
	int ghostr;
	bool kraj = false;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawBackground(CDC* pDC);
	void DrawEnd(CDC* pDC, CRect rect);
	void DrawGhost(CDC* pDC, CRect rect);
	void DrawPacman(CDC* pDC, CRect rect, float angle);
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
	virtual ~CKol2015View();
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

#ifndef _DEBUG  // debug version in Kol2015View.cpp
inline CKol2015Doc* CKol2015View::GetDocument() const
   { return reinterpret_cast<CKol2015Doc*>(m_pDocument); }
#endif

