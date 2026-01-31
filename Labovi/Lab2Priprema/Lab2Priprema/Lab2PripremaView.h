
// Lab2PripremaView.h : interface of the CLab2PripremaView class
//

#pragma once


class CLab2PripremaView : public CView
{
protected: // create from serialization only
	CLab2PripremaView() noexcept;
	DECLARE_DYNCREATE(CLab2PripremaView)

// Attributes
public:
	CLab2PripremaDoc* GetDocument() const;

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
	virtual ~CLab2PripremaView();
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
	void DrawCircle(CDC* pDC, int xcenter, int ycenter, int radius);

	void DrawPot(CDC* pDC);

	void DrawGrid(CDC* pDC);

	void DrawCactusElement(CDC* pDC, int x, int y, HENHMETAFILE* file, int partCount = 1, int selfRot = 0, int sirina = 20, int visina = 75);


	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);

	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);

	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);

	void DrawFigure(CDC* pDC);

	void WriteCactus(CDC* pDC);

private:
	bool showGrid;
	int angle1;
	int angle2;
	
};

#ifndef _DEBUG  // debug version in Lab2PripremaView.cpp
inline CLab2PripremaDoc* CLab2PripremaView::GetDocument() const
   { return reinterpret_cast<CLab2PripremaDoc*>(m_pDocument); }
#endif

