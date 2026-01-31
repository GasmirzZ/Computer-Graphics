
// Lab1View.cpp : implementation of the CLab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1.h"
#endif

#include "Lab1Doc.h"
#include "Lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab1View

IMPLEMENT_DYNCREATE(CLab1View, CView)

BEGIN_MESSAGE_MAP(CLab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab1View construction/destruction

CLab1View::CLab1View() noexcept
{
	showGrid = TRUE;
	// TODO: add construction code here

}

CLab1View::~CLab1View()
{
}

BOOL CLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab1View drawing

void CLab1View::OnDraw(CDC* pDC)
{
	CLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen grayPen(PS_SOLID, 1, RGB(210, 210, 210));
	CBrush grayBrush(RGB(210, 210, 210));

	CPen* oldpen;
	CBrush* oldbrush;

	oldpen = pDC->SelectObject(&grayPen);
	oldbrush = pDC->SelectObject(&grayBrush);

	pDC->Rectangle(0, 0, 500, 500);



	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	CPoint trougaoTacke[3];

	CPen cyanPen(PS_SOLID, 5, RGB(28, 222, 252));
	CBrush whiteBrush(HS_VERTICAL, RGB(10, 10, 230));

	oldpen = pDC->SelectObject(&cyanPen);
	oldbrush = pDC->SelectObject(&whiteBrush);

	trougaoTacke[0] = CPoint(25,25);
	trougaoTacke[1] = CPoint(13*25, 25);
	trougaoTacke[2] = CPoint(25, 13*25);

	pDC->Polygon(trougaoTacke, 3);


	pDC->SelectObject(oldbrush);

	CBrush greenBrush(RGB(10, 220, 10));

	oldbrush = pDC->SelectObject(&greenBrush);

	trougaoTacke[0] = CPoint(13 * 25, 25);
	trougaoTacke[1] = CPoint(13 * 25, 13 * 25);
	trougaoTacke[2] = CPoint(7 * 25, 7 * 25);

	pDC->Polygon(trougaoTacke, 3);

	//
	pDC->SelectObject(oldbrush);

	CBrush purpleBrush(RGB(150, 20, 200));

	oldbrush = pDC->SelectObject(&purpleBrush);

	trougaoTacke[0] = CPoint(13 * 25, 7 *25);
	trougaoTacke[1] = CPoint(13 * 25, 25);
	trougaoTacke[2] = CPoint(19 * 25, 25);

	pDC->Polygon(trougaoTacke, 3);


	pDC->SelectObject(oldbrush);

	CBrush orangeBrush(RGB(255, 150, 13));

	oldbrush = pDC->SelectObject(&orangeBrush);

	trougaoTacke[0] = CPoint(7 * 25, 13 * 25);
	trougaoTacke[1] = CPoint(13 * 25, 13 * 25);
	trougaoTacke[2] = CPoint(7 * 25, 7 * 25);

	pDC->Polygon(trougaoTacke, 3);


	pDC->SelectObject(oldbrush);

	CBrush yellowBrush(RGB(252, 252, 30));

	oldbrush = pDC->SelectObject(&yellowBrush);

	trougaoTacke[0] = CPoint(19 * 25, 7 * 25);
	trougaoTacke[1] = CPoint(19 * 25, 19 * 25);
	trougaoTacke[2] = CPoint(7 * 25, 19 * 25);

	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);

	CBrush redBrush(RGB(255, 0, 0));
	pDC->SelectObject(&redBrush);
	pDC->Rectangle(25, 13 * 25, 7 * 25, 19 * 25);

	
	pDC->SelectObject(oldbrush);
	CBrush pinkBrush(RGB(255, 166, 254));
	pDC->SelectObject(&pinkBrush);

	CPoint paralelTacke[4];
	paralelTacke[0] = CPoint(19*25, 25);
	paralelTacke[1] = CPoint(19*25, 7*25);
	paralelTacke[2] = CPoint(13*25, 13*25);
	paralelTacke[3] = CPoint(13*25, 7*25);
	pDC->Polygon(paralelTacke, 4);



	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	CPen cyanPen2(PS_SOLID, 3, RGB(28, 222, 252));
	oldpen = pDC->SelectObject(&cyanPen2);
	pDC->SelectStockObject(NULL_BRUSH);

	DrawRegularPolygon(pDC, 4.5 * 25, 4.5 * 25, 40, 4, 0);
	DrawRegularPolygon(pDC, 10.5 * 25, 7 * 25, 34, 6, 0);
	DrawRegularPolygon(pDC, 15.5 * 25, 15.5 * 25, 45, 5, 0);
	DrawRegularPolygon(pDC, 215, 11 * 25 + 5, 20, 7, 0);
	DrawRegularPolygon(pDC, 500-130, 70, 22, 8, 0);


	if (showGrid)
	{
		pDC->SelectObject(oldpen);
		CPen penGrid(PS_DEFAULT, 1, RGB(255, 255, 255));
		oldpen = pDC->SelectObject(&penGrid);
		for (int i = 0; i < 20; i++)
		{
			pDC->MoveTo(0, i * 25);
			pDC->LineTo(20 * 25, i * 25);
			pDC->MoveTo(i * 25, 0);
			pDC->LineTo(i * 25, 20 * 25);
		}
		pDC->SelectObject(oldpen);
	}

}


// CLab1View printing

BOOL CLab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab1View diagnostics

#ifdef _DEBUG
void CLab1View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Doc* CLab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Doc)));
	return (CLab1Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab1View message handlers

void CLab1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G' || nChar == 'g')
	{
		showGrid = !showGrid;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLab1View::DrawRegularPolygon(CDC* pDC, int centerX, int centerY, int r, int numSides, float rotationAngle)
{
	if (numSides < 3) return;

	CPoint* points = new CPoint[numSides];

	double angleStep = 2.0 * 3.14 / numSides;

	for (int i = 0; i < numSides; i++)
	{
		double angle = i * angleStep + rotationAngle;
		points[i].x = centerX + (int)(r * cos(angle));
		points[i].y = centerY - (int)(r * sin(angle));
	}

	pDC->Polygon(points, numSides);

	delete[] points;
}
