
// Lab3PripremaView.h : interface of the CLab3PripremaView class
//

#pragma once


class CLab3PripremaView : public CView
{
protected: // create from serialization only
	CLab3PripremaView() noexcept;
	DECLARE_DYNCREATE(CLab3PripremaView)

// Attributes
public:
	CLab3PripremaDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = FALSE);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = FALSE);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = FALSE);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = FALSE);
	void DrawGrid(CDC* pDC);
	void DrawPiece(CDC* pDC, CString piece, bool blue = FALSE);
	void FilterBits(CBitmap* map, int count, bool red);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab3PripremaView();
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
};

#ifndef _DEBUG  // debug version in Lab3PripremaView.cpp
inline CLab3PripremaDoc* CLab3PripremaView::GetDocument() const
   { return reinterpret_cast<CLab3PripremaDoc*>(m_pDocument); }
#endif

