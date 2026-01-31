
// Kol2022View.h : interface of the CKol2022View class
//

#pragma once
#include "DImage.h"


class CKol2022View : public CView
{
protected: // create from serialization only
	CKol2022View() noexcept;
	DECLARE_DYNCREATE(CKol2022View)

// Attributes
public:
	CKol2022Doc* GetDocument() const;
	DImage* base;
	DImage* arm1;
	DImage* arm2;
	DImage* head;
	DImage* background;
	DImage* base_shadow;
	DImage* arm1_shadow;
	DImage* arm2_shadow;
	DImage* head_shadow;
	float rotArm1 = -45;
	float rotArm2 = -90;
	float rotHead = 180;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawLampBase(CDC* pDC, bool bIsShadow);
	void DrawLampArm1(CDC* pDC, bool bIsShadow);
	void DrawLampArm2(CDC* pDC, bool bIsShadow);
	void DrawLampHead(CDC* pDC, bool bIsShadow);
	void DrawLamp(CDC* pDC, bool bIsShadow);
	void DrawLampShadow(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void Translate(CDC* pDC, float dx, float dy, bool right);
	void Rotate(CDC* pDC, float angle, bool right);
	void Scale(CDC* pDC, float sx, float sy, bool right);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2022View();
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

#ifndef _DEBUG  // debug version in Kol2022View.cpp
inline CKol2022Doc* CKol2022View::GetDocument() const
   { return reinterpret_cast<CKol2022Doc*>(m_pDocument); }
#endif

