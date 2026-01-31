
// Lab1PripremaView.h : interface of the CLab1PripremaView class
//

#pragma once


class CLab1PripremaView : public CView
{
protected: // create from serialization only
	CLab1PripremaView() noexcept;
	DECLARE_DYNCREATE(CLab1PripremaView)

// Attributes
public:
	CLab1PripremaDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab1PripremaView();
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
	void DrawRegularPolygon(CDC* pDC, int centerX, int centerY, int r, int numSides, float rotationAngle);

private:
	BOOL showGrid;
};

#ifndef _DEBUG  // debug version in Lab1PripremaView.cpp
inline CLab1PripremaDoc* CLab1PripremaView::GetDocument() const
   { return reinterpret_cast<CLab1PripremaDoc*>(m_pDocument); }
#endif

