
// Kol2017View.h : interface of the CKol2017View class
//

#pragma once
#include "DImage.h"

class CKol2017View : public CView
{
protected: // create from serialization only
	CKol2017View() noexcept;
	DECLARE_DYNCREATE(CKol2017View)

// Attributes
public:
	CKol2017Doc* GetDocument() const;
	DImage* body;
	DImage* leg1;
	DImage* leg2;
	DImage* leg3;
	DImage* back2;
	int pozadinaX = 0;
	int pozadinaY = 0;
	float ugaoPrednje = 0;
	float ugaoZadnje = -20;
	int count = 0;
	float scale = 1;
	int pomeraj = 0;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawScene(CDC* pDC, CRect rect);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
	void LoadIdentity(CDC* pDC);
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
	virtual ~CKol2017View();
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

#ifndef _DEBUG  // debug version in Kol2017View.cpp
inline CKol2017Doc* CKol2017View::GetDocument() const
   { return reinterpret_cast<CKol2017Doc*>(m_pDocument); }
#endif

