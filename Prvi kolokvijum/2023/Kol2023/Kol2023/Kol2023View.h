
// Kol2023View.h : interface of the CKol2023View class
//

#pragma once
#include "DImage.h"


class CKol2023View : public CView
{
protected: // create from serialization only
	CKol2023View() noexcept;
	DECLARE_DYNCREATE(CKol2023View)

// Attributes
public:
	CKol2023Doc* GetDocument() const;
	DImage* glava;
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* podkolenica;
	DImage* podlaktica;
	DImage* pozadina;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	float nadlakticaRot;
	float podlakticaRot;
	float sakaRot;
	float ceoRobotRot;
	float robotSX;
	float robotSY;
	float step;


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawBackgorund(CDC* pDC);
	void DrawFigure(CDC* pDC);
	void DrawHalf(CDC* pDC);
	void DrawHead(CDC* pDC);
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
	virtual ~CKol2023View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Kol2023View.cpp
inline CKol2023Doc* CKol2023View::GetDocument() const
   { return reinterpret_cast<CKol2023Doc*>(m_pDocument); }
#endif

