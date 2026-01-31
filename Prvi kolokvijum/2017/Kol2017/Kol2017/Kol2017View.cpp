
// Kol2017View.cpp : implementation of the CKol2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2017.h"
#endif

#include "Kol2017Doc.h"
#include "Kol2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(X) 3.14*X/180.

// CKol2017View

IMPLEMENT_DYNCREATE(CKol2017View, CView)

BEGIN_MESSAGE_MAP(CKol2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2017View construction/destruction

CKol2017View::CKol2017View() noexcept
{
	body = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	leg3 = new DImage();
	back2 = new DImage();
	body->Load(CString("Body.png"));
	leg1->Load(CString("Leg1.png"));
	leg2->Load(CString("Leg2.png"));
	leg3->Load(CString("Leg3.png"));
	back2->Load(CString("Back2.jpg"));

}

CKol2017View::~CKol2017View()
{
	if (body)
		delete body;
	if (leg1)
		delete leg1;
	if (leg2)
		delete leg2;
	if (leg3)
		delete leg3;
	if (back2)
		delete back2;
}

BOOL CKol2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2017View drawing

void CKol2017View::OnDraw(CDC* pDC)
{
	CKol2017Doc* pDoc = GetDocument();
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
	XFORM old;
	memdc->GetWorldTransform(&old);

	DrawScene(memdc, rect);


	memdc->SetWorldTransform(&old);
	memdc->SetGraphicsMode(prevmode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldmap);
	memdc->DeleteDC();
	delete memdc;

	// TODO: add draw code for native data here
}

void CKol2017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	int w = back2->Width();
	int h = back2->Height();
	back2->Draw(pDC, CRect(0, 0, w, h), CRect(rect.left - pozadinaX, rect.top - pozadinaY, rect.left - pozadinaX + w, rect.top - pozadinaY + h));

	Translate(pDC, 100+pomeraj, 100, false);
	Scale(pDC, scale, scale, false);
	DrawImgTransparent(pDC, body);
	
	//zadnje noge
	DrawLeg(pDC, ugaoZadnje, 72 - 28, 168);
	DrawLeg(pDC, -ugaoZadnje, 72 - 28, 168);
	
	DrawLeg(pDC, ugaoPrednje, 270, 168);
	DrawLeg(pDC, -ugaoPrednje, 270, 168);
	//prednje noge

	pDC->SetWorldTransform(&old);

}

void CKol2017View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	int w = pImage->Width();
	int h = pImage->Height();
	pImage->DrawTransparent(pDC, CRect(0, 0, w, h), CRect(0, 0, w, h), boja);
}

void CKol2017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	{
		Translate(pDC, dx, dy, false);
		Translate(pDC, 28, 28, false);
		Rotate(pDC, alpha, false);
		Translate(pDC, -28, -28, false);
		DrawImgTransparent(pDC, leg1);

		Translate(pDC, 0, leg1->Height() - 28 - 29, false);
		Translate(pDC, 29, 29, false);
		Rotate(pDC, -alpha, false);
		Translate(pDC, -29, -29, false);
		DrawImgTransparent(pDC, leg2);

		Translate(pDC, -35, leg2->Height() - 35, false);
		DrawImgTransparent(pDC, leg3);
	}
	pDC->SetWorldTransform(&old);
}

void CKol2017View::LoadIdentity(CDC* pDC)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}

void CKol2017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2017View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rot = TORAD(angle);

	XFORM xform;
	xform.eM11 = cos(rot);
	xform.eM12 = sin(rot);
	xform.eM21 = -sin(rot);
	xform.eM22 = cos(rot);
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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


// CKol2017View printing

BOOL CKol2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2017View diagnostics

#ifdef _DEBUG
void CKol2017View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2017Doc* CKol2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2017Doc)));
	return (CKol2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2017View message handlers

BOOL CKol2017View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}

void CKol2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'A')
	{
		if(pozadinaX >= 200)
			pozadinaX -= 200;	
	}
	if (nChar == 'D')
	{
		pozadinaX += 200;
	}
	if (nChar == 'W')
	{
		pozadinaY += 200;
	}
	if (nChar == 'S')
	{
		pozadinaY -= 200;
	}
	if (nChar == VK_RIGHT)
	{
		count++;
		pomeraj += 10;
		if (count == 1)
		{
			ugaoPrednje += 10;
			ugaoZadnje += 10;

		}
		if (count == 2)
		{
			ugaoPrednje += 10;
			ugaoZadnje += 10;

		}
		if (count == 3)
		{
			ugaoPrednje -= 10;
			ugaoZadnje += 10;

		}
		if (count == 4)
		{
			ugaoPrednje -= 10;
			ugaoZadnje += 10;

		}
		if (count == 5)
		{
			ugaoPrednje -= 10;
			ugaoZadnje -= 10;
		}
		if (count == 6)
		{
			ugaoPrednje -= 10;
			ugaoZadnje -= 10;
		}
		if (count == 7)
		{
			ugaoPrednje += 10;
			ugaoZadnje -= 10;
		}
		if (count == 8)
		{
			ugaoPrednje += 10;
			ugaoZadnje -= 10;
			count = 0;
		}
	}
	if (nChar == VK_UP)
	{
		scale += 0.1;
	}
	if (nChar == VK_DOWN)
	{
		scale -= 0.1;
	}


	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
