// Lab1PripremaView.cpp : implementation of the CLab1PripremaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1Priprema.h"
#endif

#include "Lab1PripremaDoc.h"
#include "Lab1PripremaView.h"
#include "math.h"

#define DTORAD (float)(3.14/180)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CLab1PripremaView

IMPLEMENT_DYNCREATE(CLab1PripremaView, CView)

BEGIN_MESSAGE_MAP(CLab1PripremaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab1PripremaView construction/destruction

CLab1PripremaView::CLab1PripremaView() noexcept
{
	showGrid = TRUE;
	// TODO: add construction code here

}

CLab1PripremaView::~CLab1PripremaView()
{
}

BOOL CLab1PripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	

	return CView::PreCreateWindow(cs);
}

// CLab1PripremaView drawing



void CLab1PripremaView::OnDraw(CDC* pDC)
{
	CLab1PripremaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen pen1(PS_DEFAULT, 1, RGB(210, 210, 210));
	CPen* oldpen = pDC->SelectObject(&pen1);

	CBrush brush1(RGB(210, 210, 210));
	CBrush* oldbrush = pDC->SelectObject(&brush1);

	pDC -> Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	CPen penBlue5(PS_DEFAULT, 5, RGB(0, 0, 255));
	CBrush brushYeller(RGB(255, 255, 0));

	oldpen = pDC->SelectObject(&penBlue5);
	oldbrush = pDC->SelectObject(&brushYeller);

	CPoint trougaoTacke[3];
	trougaoTacke[0] = CPoint(25, 250);
	trougaoTacke[1] = CPoint(138, 137);
	trougaoTacke[2] = CPoint(138, 363);
	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);
	

	CBrush brushOrange(RGB(255, 128, 0));
	oldbrush = pDC->SelectObject(&brushOrange);
	

	
	trougaoTacke[1] = CPoint(138, 137);
	trougaoTacke[0] = CPoint(252, 137);
	trougaoTacke[2] = CPoint(252, 25);
	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);


	
	CBrush brushWhiteDiag(HS_DIAGCROSS, RGB(76, 217, 252));
	oldbrush = pDC->SelectObject(&brushWhiteDiag);

	
	trougaoTacke[0] = CPoint(138, 252);
	trougaoTacke[1] = CPoint(252, 252);
	trougaoTacke[2] = CPoint(252, 362);
	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);



	CBrush brushGreen(RGB(0, 200, 0));
	oldbrush = pDC->SelectObject(&brushGreen);


	trougaoTacke[0] = CPoint(252, 25);
	trougaoTacke[1] = CPoint(252, 252);
	trougaoTacke[2] = CPoint(475, 25);
	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);

	
	
	CBrush brushRed(RGB(255, 0, 0));
	oldbrush = pDC->SelectObject(&brushRed);


	trougaoTacke[0] = CPoint(252, 475);
	trougaoTacke[1] = CPoint(252, 252);
	trougaoTacke[2] = CPoint(475, 475);
	pDC->Polygon(trougaoTacke, 3);

	pDC->SelectObject(oldbrush);


	CBrush brushPurple(RGB(160, 40, 220));
	oldbrush = pDC->SelectObject(&brushPurple);
	
	pDC->Rectangle(139, 138, 253, 252);

	pDC->SelectObject(oldbrush);



	CBrush brushPink(RGB(250, 152, 208));
	oldbrush = pDC->SelectObject(&brushPink);

	CPoint cetvorougaoTacke[4];

	cetvorougaoTacke[0] = CPoint(138, 252);
	cetvorougaoTacke[1] = CPoint(138, 363);
	cetvorougaoTacke[2] = CPoint(252, 475);
	cetvorougaoTacke[3] = CPoint(252, 362);

	pDC->Polygon(cetvorougaoTacke, 4);

	pDC->SelectObject(oldbrush);




	CPen penBlue3(PS_DEFAULT, 3, RGB(0, 0, 255));

	oldpen = pDC->SelectObject(&penBlue3);
	pDC->SelectStockObject(NULL_BRUSH);

	DrawRegularPolygon(pDC, 89, 251, 25, 8, 0);
	DrawRegularPolygon(pDC, 220, 105, 16, 7, 15*DTORAD);
	DrawRegularPolygon(pDC, 315, 88, 30, 4, 0); //kvadrat se inicijalno crta pod 45 stepeni
	DrawRegularPolygon(pDC, 220, 287, 15, 6, 0);
	DrawRegularPolygon(pDC, 318, 415, 32, 5, 0*DTORAD);


	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);

	
	
	
	//crtanje grida
	if(showGrid)
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


// CLab1PripremaView printing

BOOL CLab1PripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1PripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1PripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab1PripremaView diagnostics

#ifdef _DEBUG
void CLab1PripremaView::AssertValid() const
{
	CView::AssertValid();
}

void CLab1PripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1PripremaDoc* CLab1PripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1PripremaDoc)));
	return (CLab1PripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab1PripremaView message handlers

void CLab1PripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'G' || nChar == 'g') 
	{
		showGrid = !showGrid;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLab1PripremaView::DrawRegularPolygon(CDC* pDC, int centerX, int centerY, int r, int numSides, float rotationAngle)
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