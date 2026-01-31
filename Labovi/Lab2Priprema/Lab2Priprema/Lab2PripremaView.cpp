
// Lab2PripremaView.cpp : implementation of the CLab2PripremaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab2Priprema.h"
#endif

#include "Lab2PripremaDoc.h"
#include "Lab2PripremaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(x) x*3.14/180 


// CLab2PripremaView

IMPLEMENT_DYNCREATE(CLab2PripremaView, CView)

BEGIN_MESSAGE_MAP(CLab2PripremaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab2PripremaView construction/destruction

CLab2PripremaView::CLab2PripremaView() noexcept
{
	showGrid = TRUE;
	angle1 = 0; //ceo kaktus
	angle2 = 0; //jedna elipsa
	// TODO: add construction code here

}

CLab2PripremaView::~CLab2PripremaView()
{
}

BOOL CLab2PripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab2PripremaView drawing

void CLab2PripremaView::OnDraw(CDC* pDC)
{
	CLab2PripremaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawFigure(pDC);
	
}


// CLab2PripremaView printing

BOOL CLab2PripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab2PripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab2PripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab2PripremaView diagnostics

#ifdef _DEBUG
void CLab2PripremaView::AssertValid() const
{
	CView::AssertValid();
}

void CLab2PripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab2PripremaDoc* CLab2PripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2PripremaDoc)));
	return (CLab2PripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab2PripremaView message handlers

void CLab2PripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == 'G')
		showGrid = !showGrid;
	if (nChar == 'A')
		angle1 -= 6;
	if (nChar == 'D')
		angle1 += 6;
	if (nChar == VK_LEFT)
		angle2 -= 6;
	if (nChar == VK_RIGHT)
		angle2 += 6;
	Invalidate();
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLab2PripremaView::DrawCircle(CDC* pDC,int xcenter, int ycenter, int radius)
{
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush greenBrush(RGB(0, 204, 0));
	CPen* oldpen = pDC->SelectObject(&blackPen);
	CBrush* oldbrush = pDC->SelectObject(&greenBrush);
	pDC->Ellipse(xcenter - radius / 2, ycenter - radius / 2, xcenter + radius / 2, ycenter + radius / 2);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CLab2PripremaView::DrawPot(CDC* pDC)
{

	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brownBrush(RGB(222, 148, 0));

	CPen *oldpen = pDC->SelectObject(&blackPen);
	CBrush *oldbrush = pDC->SelectObject(&brownBrush);

	CPoint tacke[4];
	CPoint tacke2[4];
	tacke[0] = CPoint(8 * 25 + 10, 500);
	tacke[1] = CPoint(500 - 8 * 25 - 10, 500);
	tacke[2] = CPoint(500 - 8 * 25, 500 - 2 * 25);
	tacke[3] = CPoint(8 * 25, 500 - 2 * 25);
	tacke2[0] = CPoint(7 * 25 + 15, 500 - 2 * 25);
	tacke2[1] = CPoint(7 * 25 + 15, 500 - 2 * 25 - 20);
	tacke2[2] = CPoint(500 - (7 * 25 + 15), 500 - 2 * 25 - 20);
	tacke2[3] = CPoint(500 - (7 * 25 + 15), 500 - 2 * 25);

	pDC->Polygon(tacke, 4);
	pDC->Polygon(tacke2, 4);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CLab2PripremaView::DrawGrid(CDC* pDC)
{
	CPen gridPen(PS_SOLID, 1, RGB(220, 220, 220));
	CPen *oldpen = pDC->SelectObject(&gridPen);

	if (showGrid)
	{
		for (int i = 0; i < 21; i++)
		{
			pDC->MoveTo(0, i * 25);
			pDC->LineTo(500, i * 25);
			pDC->MoveTo(i * 25, 0);
			pDC->LineTo(i * 25, 500);
		}
	}

	pDC->SelectObject(oldpen);

}

void CLab2PripremaView::DrawCactusElement(CDC* pDC, int x, int y, HENHMETAFILE* file, int partCount, int selfRot, int sirina, int visina)
{
	XFORM xFormOld;
	pDC->GetWorldTransform(&xFormOld);
	Translate(pDC, x, y);
	Rotate(pDC, TORAD(selfRot));
	Scale(pDC, 4-partCount, 1);

	pDC->PlayMetaFile(*file, CRect(CPoint(-sirina/2, -visina/2), CPoint(sirina/2, visina/2)));
	pDC->SetWorldTransform(&xFormOld);
}



void CLab2PripremaView::DrawFigure(CDC* pDC) 
{
	//inicijalni kvadrat 500x500
	XFORM xFormOld;
	CPen bluePen(PS_SOLID, 1, RGB(135, 206, 235));
	CBrush blueBrush(RGB(135, 206, 235));
	CPen* oldpen = pDC->SelectObject(&bluePen);
	CBrush* oldbrush = pDC->SelectObject(&blueBrush);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	HENHMETAFILE lightPart = GetEnhMetaFile(_T("cactus_part_light.emf"));
	HENHMETAFILE darkPart = GetEnhMetaFile(_T("cactus_part.emf"));

	if (lightPart == NULL || darkPart == NULL)  AfxMessageBox(_T("Greska pri ucitavanju emf datoteka"));

	int prevmode = pDC->SetGraphicsMode(GM_ADVANCED);

	//deo koji se rotira za ceo kaktus
	int savedDC1 = pDC->SaveDC();
	Translate(pDC, 10 * 25, 17 * 25); //tacka rotacije
	Rotate(pDC, TORAD(angle1));
	Translate(pDC, -250, -425);
	DrawCactusElement(pDC, 10*25, 500-112, &lightPart);
	DrawCactusElement(pDC, 10*25, 12 * 25 + 12, &darkPart , 3);
	DrawCactusElement(pDC, 11*25 + 2, 13*25 - 2, &darkPart, 3, 45);
	DrawCactusElement(pDC, 9 * 25 - 2, 13*25 -2, &darkPart, 3, -45);
	DrawCactusElement(pDC, 6 * 25 + 10, 12 * 25 - 2, &darkPart, 2, -90);
	DrawCactusElement(pDC, 8 * 25 - 2, 10 * 25 + 10, &darkPart, 2);
	int savedDC2 = pDC->SaveDC();
	Translate(pDC, 12 * 25 + 2, 12 * 25 - 2); //tacka rotacije
	Rotate(pDC, TORAD(angle2));
	Translate(pDC, -(12 * 25 + 2), -(12 * 25 - 2));
	DrawCactusElement(pDC, 12 * 25 + 2, 10 * 25 + 10, &lightPart, 2);
	pDC->RestoreDC(savedDC2);
	DrawCactusElement(pDC, 500 - (6 * 25 + 10), 12 * 25 - 2, &darkPart, 2, 90);
	DrawCactusElement(pDC, 8 * 25 - 2, 7 * 25 + 10, &darkPart);
	DrawCactusElement(pDC, 16 * 25 + 2, 11 * 25 - 2, &darkPart, 2, 45);
	DrawCactusElement(pDC, 16 * 25 + 2, 13 * 25 - 2, &darkPart, 2, 135);

	
	DrawCircle(pDC, 250, 14*25, 20);
	DrawCircle(pDC, 8 * 25 - 2, 12 * 25 - 2, 20);
	DrawCircle(pDC, 12 * 25 + 2, 12 * 25 - 2, 20);
	DrawCircle(pDC, 8*25 - 2, 9*25 - 2, 20);
	DrawCircle(pDC, 15*25 + 2, 12 * 25 - 2, 20);
	pDC->RestoreDC(savedDC1);
	//do ovde se rotira za ceo kaktus

	DrawCircle(pDC, 250, 425, 20);
	DrawPot(pDC);
	DrawGrid(pDC);

	WriteCactus(pDC);




	pDC->SetGraphicsMode(prevmode);

	DeleteEnhMetaFile(lightPart);
	DeleteEnhMetaFile(darkPart);
}

void CLab2PripremaView::WriteCactus(CDC* pDC)
{
	CFont font;
	font.CreateFont(35, 13, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	int savedDC = pDC->SaveDC();
	pDC->SetBkMode(TRANSPARENT);
	Translate(pDC, 475, 25);
	Rotate(pDC, TORAD(90));
	pDC->SelectObject(font);
	pDC->TextOut(0,-4, L"Kaktusss");
	pDC->SetTextColor(RGB(248, 250, 7));
	pDC->TextOut(-2, -2, L"Kaktusss");
	pDC->RestoreDC(savedDC);


}

void CLab2PripremaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = 1.0;
	xForm.eM12 = 0.0;
	xForm.eM21 = 0.0;
	xForm.eM22 = 1.0;
	xForm.eDx = dX;
	xForm.eDy = dY;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab2PripremaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{

	XFORM xForm{};
	xForm.eM11 = sX;
	xForm.eM12 = 0.0;
	xForm.eM21 = 0.0;
	xForm.eM22 = sY;
	xForm.eDx = 0.0;
	xForm.eDy = 0.0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab2PripremaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = cos(angle);
	xForm.eM12 = sin(angle);
	xForm.eM21 = -sin(angle);
	xForm.eM22 = cos(angle);
	xForm.eDx = 0.0;
	xForm.eDy = 0.0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}



