
// Kol2023View.cpp : implementation of the CKol2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2023.h"
#endif

#include "Kol2023Doc.h"
#include "Kol2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TORAD(x) x*3.14/180.
// CKol2023View

IMPLEMENT_DYNCREATE(CKol2023View, CView)

BEGIN_MESSAGE_MAP(CKol2023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol2023View construction/destruction

CKol2023View::CKol2023View() noexcept
{
	nadlakticaRot = 0;
	podlakticaRot = 0;
	sakaRot = 0;
	ceoRobotRot = 0;
	robotSX = 1;
	robotSY = 1;
	step = 5;
	glava = new DImage();
	glava->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\glava.png"));
	podlaktica = new DImage();
	podlaktica->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\\podlaktica.png"));
	nadlaktica = new DImage();
	nadlaktica->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\nadlaktica.png"));
	nadkolenica = new DImage();
	nadkolenica->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\nadkolenica.png"));
	podkolenica = new DImage();
	podkolenica->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\podkolenica.png"));
	saka = new DImage();
	saka->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\saka.png"));
	stopalo = new DImage();
	stopalo->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\stopalo.png"));
	telo = new DImage();
	telo->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\telo.png"));
	pozadina = new DImage();
	pozadina->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2023\\pozadina.jpg"));

}

CKol2023View::~CKol2023View()
{
	delete glava;
	delete nadkolenica;
	delete nadlaktica;
	delete podkolenica;
	delete podlaktica;
	delete saka;
	delete stopalo;
	delete telo;
	delete pozadina;
}

BOOL CKol2023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2023View drawing

void CKol2023View::OnDraw(CDC* pDC)
{
	CKol2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memdc = new CDC();
	memdc->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldbmp = memdc->SelectObject(&bmp);

	int prevMode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM xformInit;
	memdc->GetWorldTransform(&xformInit);
	
	//////////////////////////////////////////////////////////

	//crtanje ovde sad
	DrawBackgorund(memdc);

	Translate(memdc, pozadina->Width() / 2, pozadina->Height() / 2, false);
	Rotate(memdc, ceoRobotRot, false);
	Scale(memdc, robotSX, robotSY, false);
	Translate(memdc, -pozadina->Width() / 2, -pozadina->Height() / 2, false);
	DrawFigure(memdc);

	//////////////////////////////////////////////////////////

	memdc->SetWorldTransform(&xformInit);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldbmp);
	memdc->DeleteDC();

	delete memdc;
}

void CKol2023View::DrawBackgorund(CDC* pDC)
{
	CRect rect(0, 0, pozadina->Width(), pozadina->Height());
	pozadina->Draw(pDC, rect, rect);
}

void CKol2023View::DrawFigure(CDC* pDC)
{
	
	XFORM xold;
	//telo leva strana
	pDC->GetWorldTransform(&xold);
	Translate(pDC, -telo->Width(), -telo->Height(), false);
	Translate(pDC, pozadina->Width() / 2, pozadina->Height() / 2, false);
	DrawHalf(pDC);
	pDC->SetWorldTransform(&xold);

	//telo desna strana
	pDC->GetWorldTransform(&xold);
	Translate(pDC, telo->Width(), -telo->Height(), false);
	Translate(pDC, pozadina->Width() / 2, pozadina->Height() / 2, false);
	Scale(pDC, -1, 1, false);
	DrawHalf(pDC);
	pDC->SetWorldTransform(&xold);
	
	DrawHead(pDC);
}

void CKol2023View::DrawHalf(CDC* pDC)
{
	//XFORM xold;
	////telo
	//pDC->GetWorldTransform(&xold);
	//Translate(pDC, -telo->Width(), -telo->Height(), false);
	//Translate(pDC, pozadina->Width()/2, pozadina->Height() / 2, false);
	DrawImgTransparent(pDC, telo);

	XFORM xpreruke;
	pDC->GetWorldTransform(&xpreruke);
	//nadlaktica
	Translate(pDC, -10, 30, false);
	Translate(pDC, 35, 35, false);
	Rotate(pDC, nadlakticaRot, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nadlaktica);
	
	//podlaktica
	Translate(pDC, -8, 134, false);
	Translate(pDC, 30, 33, false);
	Rotate(pDC, podlakticaRot, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, podlaktica);

	//saka
	Translate(pDC, 5, 137, false);
	Translate(pDC, 25, 3, false);
	Rotate(pDC, sakaRot, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&xpreruke);

	//nadkolenica
	Translate(pDC, 32, 242, false);
	DrawImgTransparent(pDC, nadkolenica);

	//podkolenica
	Translate(pDC, 5, 147, false);
	DrawImgTransparent(pDC, podkolenica);

	//stopalo
	Translate(pDC, 5, 232, false);
	DrawImgTransparent(pDC, stopalo);


	//pDC->SetWorldTransform(&xold);
}

void CKol2023View::DrawHead(CDC* pDC)
{
	XFORM xold;
	//telo
	pDC->GetWorldTransform(&xold);
	Translate(pDC, -glava->Width()/2, -glava->Height()/2, false);
	Translate(pDC, pozadina->Width() / 2, telo->Height() / 2, false);
	DrawImgTransparent(pDC, glava);


	pDC->SetWorldTransform(&xold);
}

void CKol2023View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);
	CRect rect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CKol2023View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = 1;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = 1;
	xform.eDx = dX;
	xform.eDy = dY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2023View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = TORAD(angle);

	XFORM xform;
	xform.eM11 = cos(radAngle);
	xform.eM12 = sin(radAngle);
	xform.eM21 = -sin(radAngle);
	xform.eM22 = cos(radAngle);
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2023View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = sX;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = sY;
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// CKol2023View printing

BOOL CKol2023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2023View diagnostics

#ifdef _DEBUG
void CKol2023View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2023Doc* CKol2023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2023Doc)));
	return (CKol2023Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2023View message handlers

void CKol2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'A' && sakaRot + step <= 30)
		sakaRot += step;
	if (nChar == 'S' && sakaRot - step >= -10)
		sakaRot -= step;
	if (nChar == 'D' && podlakticaRot + step <= 80)
		podlakticaRot += step;
	if (nChar == 'F' && podlakticaRot - step >= -10)
		podlakticaRot -= step;
	if (nChar == 'G' && nadlakticaRot + step <= 90)
		nadlakticaRot += step;
	if (nChar == 'H' && nadlakticaRot - step >= -10)
		nadlakticaRot -= step;

	if (nChar == '1')
	{
		ceoRobotRot -= 3 * step;
		robotSX -= 0.1;
		robotSY -= 0.1;
	}
	if (nChar == '2')
	{
		ceoRobotRot += 3 * step;
		robotSX += 0.1;
		robotSY += 0.1;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CKol2023View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
