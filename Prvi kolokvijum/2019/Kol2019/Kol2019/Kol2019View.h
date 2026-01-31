
// Kol2019View.h : interface of the CKol2019View class
//

#pragma once
#include "DImage.h"


class CKol2019View : public CView
{
protected: // create from serialization only
	CKol2019View() noexcept;
	DECLARE_DYNCREATE(CKol2019View)

// Attributes
public:
	CKol2019Doc* GetDocument() const;
	DImage* body1;
	DImage* arm1;
	DImage* arm2;
	DImage* leg1;
	DImage* leg2;
	DImage* background;
	float angleArm1 = 0; // korak 90
	float angleArm2 = 0; //90
	float angleLeg1 = 0; //35
	float angleLeg2 = 0; //35
	float angleBody = 0;  //2.5
	int count = 0;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawTransformer(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2019View();
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

#ifndef _DEBUG  // debug version in Kol2019View.cpp
inline CKol2019Doc* CKol2019View::GetDocument() const
   { return reinterpret_cast<CKol2019Doc*>(m_pDocument); }
#endif

