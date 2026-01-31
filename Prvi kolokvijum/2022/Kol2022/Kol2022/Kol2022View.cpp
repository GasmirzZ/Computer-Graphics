
// Kol2022View.cpp : implementation of the CKol2022View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2022.h"
#endif

#include "Kol2022Doc.h"
#include "Kol2022View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TORAD(X) X*3.14/180.

// CKol2022View

IMPLEMENT_DYNCREATE(CKol2022View, CView)

BEGIN_MESSAGE_MAP(CKol2022View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2022View construction/destruction

CKol2022View::CKol2022View() noexcept
{
	// TODO: add construction code here
	base = new DImage();
	base->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\base.png"));
	arm1 = new DImage();
	arm1->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\arm2.png"));
	head = new DImage();
	head->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\head.png"));
	background = new DImage();
	background->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\background.jpg"));
	base_shadow = new DImage();
	base_shadow->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\base_shadow.png"));
	arm1_shadow = new DImage();
	arm1_shadow->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\arm1_shadow.png"));
	arm2_shadow = new DImage();
	arm2_shadow->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\arm2_shadow.png"));
	head_shadow = new DImage();
	head_shadow->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2022\\head_shadow.png"));
}

CKol2022View::~CKol2022View()
{
	delete base;
	delete arm1;
	delete arm2;
	delete head;
	delete background;
	delete base_shadow;
	delete arm1_shadow;
	delete arm2_shadow;
	delete head_shadow;
}

BOOL CKol2022View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2022View drawing

void CKol2022View::OnDraw(CDC* pDC)
{
	CKol2022Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memdc = new CDC();
	memdc->CreateCompatibleDC(pDC);
	
	CBitmap memmap;
	memmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* oldmap = memdc->SelectObject(&memmap);

	int oldmode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM xform;
	memdc->GetWorldTransform(&xform);

	DrawBackground(memdc);
	DrawLampShadow(memdc);
	DrawLamp(memdc, true);

	memdc->SetWorldTransform(&xform);
	memdc->SetGraphicsMode(oldmode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldmap);
	memdc->DeleteDC();
	delete memdc;
}

void CKol2022View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect rect(0, 0, pImage->Width(), pImage->Height());
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	pImage->DrawTransparent(pDC, rect, rect, boja);


	//delete bytes;
}

void CKol2022View::DrawLampBase(CDC* pDC, bool bIsShadow)
{
	
	if (bIsShadow)
	{
		DrawImgTransparent(pDC, base);
	}
	else
	{
		DrawImgTransparent(pDC, base_shadow);
	}
	Translate(pDC, 100, -35, false);
	
}

//ZOVI PRVO CRTANJE POSTOLJE-RUKA1-GLAVA-RUKA2
void CKol2022View::DrawLampArm1(CDC* pDC, bool bIsShadow)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, rotArm1, false);
	Translate(pDC, -58, -61, false);
	if (bIsShadow)
	{
		DrawImgTransparent(pDC, arm1);
	}
	else
	{
		DrawImgTransparent(pDC, arm1_shadow);
	}
	Translate(pDC, 309, 61, false);
}

void CKol2022View::DrawLampArm2(CDC* pDC, bool bIsShadow)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	Rotate(pDC, rotArm2, false);
	Translate(pDC, -36, -40, false);
	if (bIsShadow)
	{
		DrawImgTransparent(pDC, arm2);
	}
	else
	{
		DrawImgTransparent(pDC, arm2_shadow);
	}
	pDC->SetWorldTransform(&old);
}

void CKol2022View::DrawLampHead(CDC* pDC, bool bIsShadow)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	Rotate(pDC, rotArm2, false);
	Translate(pDC, 272, 40, false); //pomeramo se po arm2 koji jos nije nacrtan
	Translate(pDC, -36, -40, false); //ovo mora posto smo u tacki rotacije a ne u pocetnoj da bi gornji translate bio validan
	Rotate(pDC, rotHead, false);
	Translate(pDC, -178, -100, false);
	if (bIsShadow)
	{
		DrawImgTransparent(pDC, head);
	}
	else
	{
		DrawImgTransparent(pDC, head_shadow);
	}
	pDC->SetWorldTransform(&old);
	
}

void CKol2022View::DrawLamp(CDC* pDC, bool bIsShadow)
{
	if(bIsShadow)
	{
		CRect rect;
		GetClientRect(&rect);
		Translate(pDC, rect.Width() / 2, 5 * rect.Height() / 8, false); //nabadane koordinate 
	}
	DrawLampBase(pDC, bIsShadow);
	DrawLampArm1(pDC, bIsShadow);
	DrawLampHead(pDC, bIsShadow);
	DrawLampArm2(pDC, bIsShadow);
}

void CKol2022View::DrawLampShadow(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	CRect rect;
	GetClientRect(&rect);
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	Translate(pDC, rect.Width() / 2, 5 * rect.Height() / 8, false); //nabadane koordinate
	//Translate(pDC, base->Width()/2, base->Height()/2, false);
	Translate(pDC, 100, -35, false);
	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);
	Translate(pDC, -4*100, -35, false);
	//Translate(pDC, -4*35, -100, false);
	//Translate(pDC, -4*35, -4*100, false);
	/*pDC->LineTo(100, 0);
	pDC->LineTo(100, 100);
	pDC->MoveTo(-100, -100);*/
	//Translate(pDC, -100, 0, false);
	DrawLamp(pDC, false);
	pDC->SetWorldTransform(&old);
}


void CKol2022View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CRect rect2(0, 0, background->Width(), background->Height()); //za sliku
	int pomerajX = (rect.Width() - rect2.Width())/2;
	int pomerajY = (rect.Height() - rect2.Height());
	background->Draw(pDC, rect2, CRect(pomerajX, pomerajY, pomerajX + background->Width(), pomerajY + background->Height()));
}

void CKol2022View::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM xform;

	xform.eM11 = 1;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = 1;
	xform.eDx = dx;
	xform.eDy = dy;

	pDC->ModifyWorldTransform(&xform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2022View::Rotate(CDC* pDC, float angle, bool right)
{
	XFORM xform;

	float rot = TORAD(angle);

	xform.eM11 = cos(rot);
	xform.eM12 = sin(rot);
	xform.eM21 = -sin(rot);
	xform.eM22 = cos(rot);
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKol2022View::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM xform;

	xform.eM11 = sx;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = sy;
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// CKol2022View printing

BOOL CKol2022View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2022View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2022View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2022View diagnostics

#ifdef _DEBUG
void CKol2022View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2022View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2022Doc* CKol2022View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2022Doc)));
	return (CKol2022Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2022View message handlers

BOOL CKol2022View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


	return true;
	//return CView::OnEraseBkgnd(pDC);
}

void CKol2022View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == '1')
	{
		rotArm1 -= 10;
	}
	if (nChar == '2')
	{
		rotArm1 += 10;

	}
	if (nChar == '3')
	{
		rotArm2 -= 10;
	}
	if (nChar == '4')
	{
		rotArm2 += 10;
	}
	if (nChar == '5')
	{
		rotHead -= 10;
	}
	if (nChar == '6')
	{
		rotHead += 10;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
