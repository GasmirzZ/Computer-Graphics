
// Kol2024AView.cpp : implementation of the CKol2024AView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2024A.h"
#endif

#include "Kol2024ADoc.h"
#include "Kol2024AView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TORAD(X) X*3.14/180.


// CKol2024AView

IMPLEMENT_DYNCREATE(CKol2024AView, CView)

BEGIN_MESSAGE_MAP(CKol2024AView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2024AView construction/destruction

CKol2024AView::CKol2024AView() noexcept
{
	// TODO: add construction code here
	background = new DImage();
	basket = new DImage();
	arm = new DImage();
	background->Load(CString("Background.jpg"));
	basket->Load(CString("Basket.png"));
	arm->Load(CString("Arm.png"));
}

CKol2024AView::~CKol2024AView()
{
	delete background;
	delete basket;
	delete arm;
}

BOOL CKol2024AView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2024AView drawing

void CKol2024AView::OnDraw(CDC* pDC)
{
	CKol2024ADoc* pDoc = GetDocument();
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

	int oldmode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	memdc->GetWorldTransform(&old);
	//CRTAJ
	DrawBackground(memdc);
	//DrawArm(memdc);
	//Translate(memdc, 500, 300, false);
	//DrawBasket(memdc, 50);
	//DrawBasketCouple(memdc, 190, 55, 30);
	//DrawPlatform(memdc, 190, 55, 90);
	DrawCarousel(memdc, 182, 50, offset, alpha, beta, angle);



	memdc->SetWorldTransform(&old);
	memdc->SetGraphicsMode(oldmode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);
	
	memdc->SelectObject(oldbmp);
	memdc->DeleteDC();
	delete memdc;

}

void CKol2024AView::DrawCarousel(CDC* pDC, int h, int r, float offset, float alpha, float beta, float angle)
{
	CRect rect;
	GetClientRect(&rect);
	int x = rect.Width() / 2;
	int y = rect.Height() - r;
	
	Translate(pDC, x + offset, y, false);
	Rotate(pDC, offset, false);
	DrawBasket(pDC, r);

	Rotate(pDC, -offset, false);
	Rotate(pDC, 180 + alpha, false);
	DrawArm(pDC);

	Translate(pDC, 0, h, false);
	Rotate(pDC, beta, false);
	DrawArm(pDC);
	
	Translate(pDC, 0, h, false);
	DrawPlatform(pDC, h, r, angle);
	
}

void CKol2024AView::DrawPlatform(CDC* pDC, int l, int r, float angle)
{
	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, - 2 * angle, false);

	for (int i = 0; i < 6; i++)
	{
		Rotate(pDC, 60, false);
		Translate(pDC, 2 * r, 0, false);
		DrawBasketCouple(pDC, l, r, 0);
		Translate(pDC, -2 * r, 0, false);
	}
}

void CKol2024AView::DrawBasketCouple(CDC* pDC, int l, int r, float angle)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	
	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	
	//PRVI I TRECI TRANSLATE SE ANULIRAJU
	Translate(pDC, 0.8 * r, 0, false); //leva tacka rot - osovina
	Translate(pDC, l, 0, false);//desna tacka rot - osovina
	Translate(pDC, -0.8 * r, 0, false); //centar desne
	DrawBasket(pDC, r);

	Translate(pDC, 0.8 * r, 0, false); //desna tacka rot - osovina
	Translate(pDC, -l, 0, false);//leva tacka rot - osovina - tu je pocetak od arm
	Rotate(pDC, -90, false);
	DrawArm(pDC);
	
	pDC->SetWorldTransform(&old);
}

void CKol2024AView::DrawBasket(CDC* pDC, int r)
{
	float x = basket->Width();
	float y = basket->Height();
	CFont font;
	font.CreateFont(0.9 * r, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
	CFont* oldfont = pDC->SelectObject(&font);

	XFORM old;
	pDC->GetWorldTransform(&old);
	
	Scale(pDC, 0.675, 0.675, false);
	Translate(pDC, -x / 2, -y / 2, false);
	
	DrawImgTransparent(pDC, basket);
	
	pDC->SetWorldTransform(&old);
	Rotate(pDC, -30, false);
	COLORREF oldcolor = pDC->SetTextColor(RGB(0, 0, 255));
	int oldmode = pDC->SetBkMode(TRANSPARENT);
	CSize textsize = pDC->GetTextExtent(L"R_G");
	pDC->TextOut(-textsize.cx/2, -textsize.cy/2, L"R_G");
	pDC->SetWorldTransform(&old);

	pDC->SelectObject(oldfont);
	pDC->SetTextColor(oldcolor);
	pDC->SetBkMode(oldmode);


}

void CKol2024AView::DrawArm(CDC* pDC)
{
	Translate(pDC, -10, -10, false);
	DrawImgTransparent(pDC, arm);
	Translate(pDC, 10, 10, false);
}

void CKol2024AView::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int x = rect.Width() - background->Width();
	int left = x / 2;
	int right = rect.Width() - x / 2;
	int bottom = rect.bottom;
	int top = rect.bottom - background->Height();

	background->Draw(pDC, CRect(0, 0, background->Width(), background->Height()), CRect(left, top, right, bottom));
}

void CKol2024AView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect rect;
	GetClientRect(&rect);

	BYTE* bytes = pImage->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), boja);

}


void CKol2024AView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2024AView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = TORAD(angle);

	XFORM xform;
	xform.eM11 = cos(rad);
	xform.eM12 = sin(rad);
	xform.eM21 = -sin(rad);
	xform.eM22 = cos(rad);
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKol2024AView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

// CKol2024AView printing

BOOL CKol2024AView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2024AView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2024AView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2024AView diagnostics

#ifdef _DEBUG
void CKol2024AView::AssertValid() const
{
	CView::AssertValid();
}

void CKol2024AView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2024ADoc* CKol2024AView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2024ADoc)));
	return (CKol2024ADoc*)m_pDocument;
}
#endif //_DEBUG


// CKol2024AView message handlers

BOOL CKol2024AView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
	//return CView::OnEraseBkgnd(pDC);
}

void CKol2024AView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'Q' || nChar == 'q')
		offset += step;
	if (nChar == 'W' || nChar == 'w')
		offset -= step;
	if (nChar == 'A' || nChar == 'a')
		alpha += step;
	if (nChar == 'S' || nChar == 's')
		alpha -= step;
	if (nChar == 'D' || nChar == 'd')
		beta += step;
	if (nChar == 'F' || nChar == 'f')
		beta -= step;
	if (nChar == 'E' || nChar == 'e')
		angle += step;
	if (nChar == 'R' || nChar == 'r')
		angle -= step;
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
