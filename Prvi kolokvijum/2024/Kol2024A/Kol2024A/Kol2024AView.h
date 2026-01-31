
// Kol2024AView.h : interface of the CKol2024AView class
//

#pragma once
#include "DImage.h"


class CKol2024AView : public CView
{
protected: // create from serialization only
	CKol2024AView() noexcept;
	DECLARE_DYNCREATE(CKol2024AView)

// Attributes
public:
	CKol2024ADoc* GetDocument() const;
	DImage* background;
	DImage* basket;
	DImage* arm;
	float offset = 0;
	float alpha = 0;
	float beta = 0;
	float angle = 0;
	float step = 5;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawCarousel(CDC* pDC, int h, int r, float offset, float alpha, float beta, float angle);
	void DrawPlatform(CDC* pDC, int l, int r, float angle);
	void DrawBasketCouple(CDC* pDC, int l, int r, float angle);
	void DrawBasket(CDC* pDC, int r);
	void DrawArm(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
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
	virtual ~CKol2024AView();
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

#ifndef _DEBUG  // debug version in Kol2024AView.cpp
inline CKol2024ADoc* CKol2024AView::GetDocument() const
   { return reinterpret_cast<CKol2024ADoc*>(m_pDocument); }
#endif

