
// Lab2View.h : interface of the CLab2View class
//

#pragma once


class CLab2View : public CView
{
protected: // create from serialization only
	CLab2View() noexcept;
	DECLARE_DYNCREATE(CLab2View)

// Attributes
public:
	CLab2Doc* GetDocument() const;

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
	virtual ~CLab2View();
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

#ifndef _DEBUG  // debug version in Lab2View.cpp
inline CLab2Doc* CLab2View::GetDocument() const
   { return reinterpret_cast<CLab2Doc*>(m_pDocument); }
#endif

