
// Kol2019View.cpp : implementation of the CKol2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2019.h"
#endif

#include "Kol2019Doc.h"
#include "Kol2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(X) X*3.14/180.

// CKol2019View

IMPLEMENT_DYNCREATE(CKol2019View, CView)

BEGIN_MESSAGE_MAP(CKol2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2019View construction/destruction

CKol2019View::CKol2019View() noexcept
{
	// TODO: add construction code here
	body1 = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	background = new DImage();
	body1->Load(CString("body1.png")); //iza arm1
	arm1->Load(CString("arm1.png")); //napred
	arm2->Load(CString("arm2.png")); //iza arm1
	leg1->Load(CString("leg1.png")); 
	leg2->Load(CString("leg2.png"));
	background->Load(CString("background.jpg"));
}

CKol2019View::~CKol2019View()
{
	if (body1)
		delete body1;
	if (arm1)
		delete arm1;
	if (arm2)
		delete arm2;
	if (leg1)
		delete leg1;
	if (leg2)
		delete leg2;
	if (background)
		delete background;
}

BOOL CKol2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2019View drawing

void CKol2019View::OnDraw(CDC* pDC)
{
	CKol2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memdc = new CDC();
	memdc->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldmap = memdc->SelectObject(&bmp);

	int prevmode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM xform;
	memdc->GetWorldTransform(&xform);

	DrawBackground(memdc, rect);

	DrawTransformer(memdc);
	




	memdc->SetWorldTransform(&xform);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SetGraphicsMode(prevmode);
	memdc->SelectObject(oldmap);
	memdc->DeleteDC();

	delete memdc;
}


void CKol2019View::DrawTransformer(CDC* pDC)

{
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	Translate(pDC, 250, 600, false);
	DrawLeg2(pDC);
	Translate(pDC, 35, 60, false); //vracamo se u leg2 na tacku rot
	Rotate(pDC, -angleLeg2, false);
	DrawLeg1(pDC);
	Translate(pDC, 237, 125, false);
	Rotate(pDC, angleBody, false);
	DrawBody1(pDC);
	Translate(pDC, 212, 85, false);

	XFORM xform2;
	pDC->GetWorldTransform(&xform2);
	Rotate(pDC, -angleArm1, false);
	Translate(pDC, -34, -31, false);
	Translate(pDC, 210, 102, false);
	DrawArm2(pDC);
	pDC->SetWorldTransform(&xform2);
	DrawArm1(pDC);
	pDC->SetWorldTransform(&xform);

}




void CKol2019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	CRect rect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, rect, rect, boja);
}

void CKol2019View::DrawBackground(CDC* pDC, CRect rc)
{
	background->Draw(pDC, CRect(0, 0, background->Width(), background->Height()), CRect(rc.left, rc.top, rc.left + background->Width(), rc.top + background->Height()));
}

void CKol2019View::DrawArm1(CDC* pDC)
{
	// posto se crta preko arm2 racunacemo da je prethodnop postavljen koord sistem na tacku rotacije zajednicku
	Rotate(pDC, -angleArm1, false);
	Translate(pDC, -34, -31, false);
	DrawImgTransparent(pDC, arm1);
}

void CKol2019View::DrawArm2(CDC* pDC)
{
	Rotate(pDC, -180-angleArm2, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, arm2);
}

void CKol2019View::DrawLeg1(CDC* pDC)
{
	Rotate(pDC, -angleLeg1, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, leg1);
}

void CKol2019View::DrawLeg2(CDC* pDC)
{
	Rotate(pDC, angleLeg2, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, leg2);
}

void CKol2019View::DrawBody1(CDC* pDC)
{
	Rotate(pDC, angleBody, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, body1);
}

void CKol2019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CKol2019View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

// CKol2019View printing

BOOL CKol2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2019View diagnostics

#ifdef _DEBUG
void CKol2019View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2019Doc* CKol2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2019Doc)));
	return (CKol2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2019View message handlers

BOOL CKol2019View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}

void CKol2019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'Q')
	{
		angleArm1 -= 90;
		angleLeg1 -= 35;
	}
	if (nChar == 'A')
	{
		angleArm1 += 90;
		angleLeg1 += 35;
	}
	if (nChar == 'T')
		angleArm2 -= 90;
	if (nChar == 'G')
		angleArm2 += 90;
	if (nChar == 'R')
		angleLeg2-= 35;
	if (nChar == 'F')
		angleLeg2 += 35;
	if (nChar == 'W')
		angleBody -= 2.5;
	if (nChar == 'S')
		angleBody += 2.5;
	if (nChar == VK_LEFT)
	{
		if(count < 3)
			count++;
		angleArm1 = count * 90; // korak 90
		angleArm2 = count * 90; //90
		angleLeg1 = count * 35; //35
		angleLeg2 = count * 35; //35
		angleBody = count * 2.5;
	}
	if (nChar == VK_RIGHT)
	{
		if(count > 0)
			count--;
		angleArm1 = count * 90; // korak 90
		angleArm2 = count * 90; //90
		angleLeg1 = count * 35; //35
		angleLeg2 = count * 35; //35
		angleBody = count * 2.5;
	}


	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
