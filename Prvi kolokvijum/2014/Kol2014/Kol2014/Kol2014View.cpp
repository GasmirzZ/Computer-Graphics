
// Kol2014View.cpp : implementation of the CKol2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2014.h"
#endif

#include "Kol2014Doc.h"
#include "Kol2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TORAD(x) x*3.14/180.

// CKol2014View

IMPLEMENT_DYNCREATE(CKol2014View, CView)

BEGIN_MESSAGE_MAP(CKol2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2014View construction/destruction

CKol2014View::CKol2014View() noexcept
{
	tocak = new DImage();
	tocak->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2014\\Wheel.png"));
	reno = GetEnhMetaFile(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2014\\clio.emf"));


}

CKol2014View::~CKol2014View()
{
	if (tocak)
		delete tocak;

	if (reno)
		delete reno;
}

BOOL CKol2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2014View drawing

void CKol2014View::OnDraw(CDC* pDC)
{
	CKol2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, w, h);

	CBitmap* oldbmp = memDC->SelectObject(&bmp);

	XFORM xformInit;
	memDC->GetWorldTransform(&xformInit);

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);

	//////////////////////////////////////

	CBrush plava(RGB(200, 220, 255));


	CBrush* oldbrush;
	oldbrush = memDC->SelectObject(&plava);
	memDC->Rectangle(&rect);

	float sirinaAuta = 450;
	float visinaAuta = 450 / 2.5;
	float poluprecnikTocka = 38;

	float centarAutaY =  -poluprecnikTocka/2 - visinaAuta / 2;

	DrawGround(memDC, ugaoRavni);
	Translate(memDC, 0, h, false);
	Rotate(memDC, -ugaoRavni, false);
	Translate(memDC, pomerajAuta, centarAutaY, false);
	DrawCar(memDC, 0, 0, sirinaAuta, visinaAuta);
	DrawWheel(memDC, -155, 70, poluprecnikTocka, pomerajAuta);
	DrawWheel(memDC, 135, 70, poluprecnikTocka, pomerajAuta);
	/////////////////////////////////////

	memDC->SetWorldTransform(&xformInit);
	memDC->SetGraphicsMode(oldMode);

	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCCOPY);

	memDC->SelectObject(oldbmp);
	memDC->DeleteDC();

	delete memDC;
}
void CKol2014View::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	int tocakwh = 376;
	int tocakx = 52;
	int tocaky = 15;
	CRect tocakrect(tocakx, tocaky, tocakx + tocakwh, tocaky + tocakwh);
	BYTE* bytes = tocak->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	
	Translate(pDC, x, y, false);
	Rotate(pDC, angle, false);
	tocak->DrawTransparent(pDC, tocakrect, CRect(- r,- r, r, r), boja);
	pDC->SetWorldTransform(&xform);


}

void CKol2014View::DrawCar(CDC* pDC, int x, int y, int w, int h)
{

	int left = x - w / 2;
	int right = x + w / 2;
	int top = y - h / 2;
	int bottom = y + h / 2;

	CRect rect(left, top, right, bottom);
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	Scale(pDC, -1, 1, false);
	PlayEnhMetaFile(pDC->m_hDC, reno, rect);
	pDC->SetWorldTransform(&xform);
}
void CKol2014View::DrawGround(CDC* pDC, float angle)
{
	CRect rect;
	GetClientRect(&rect);

	CPen braon(PS_SOLID, 3, RGB(140, 85, 75));
	CPen* oldpen = pDC->SelectObject(&braon);

	CBrush braonsraf(HS_FDIAGONAL, RGB(140, 85, 75));
	CBrush* oldbrush = pDC->SelectObject(&braonsraf);

	COLORREF staraBoja = pDC->SetBkColor(RGB(215, 233, 179));

	CPoint tacke[3];
	tacke[0] = CPoint(rect.left, rect.bottom);
	tacke[1] = CPoint(rect.right, rect.bottom);
	tacke[2] = CPoint(rect.right, rect.bottom - rect.Width()*tan(TORAD(angle)));

	pDC->Polygon(tacke, 3);

	pDC->SetBkColor(staraBoja);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CKol2014View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm;
	

	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = dX;
	xForm.eDy = dY;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKol2014View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xForm;
	

	float rad = TORAD(angle);

	xForm.eM11 = cos(rad);
	xForm.eM12 = sin(rad);
	xForm.eM21 = -sin(rad);
	xForm.eM22 = cos(rad);
	xForm.eDx = 0;
	xForm.eDy = 0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKol2014View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm;
	

	xForm.eM11 = sX;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = sY;
	xForm.eDx = 0;
	xForm.eDy = 0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// CKol2014View printing

BOOL CKol2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2014View diagnostics

#ifdef _DEBUG
void CKol2014View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2014Doc* CKol2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2014Doc)));
	return (CKol2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2014View message handlers

BOOL CKol2014View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
	//return CView::OnEraseBkgnd(pDC);
}

void CKol2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_LEFT)
	{
		if (pomerajAuta > 0)
		{
			pomerajAuta -= 10;
		}
	}
	if (nChar == VK_RIGHT)
	{
		pomerajAuta += 10;
	}
	if (nChar == VK_UP)
	{
		if (ugaoRavni< 80)
		{
			ugaoRavni += 10;
		}
	}
	if (nChar == VK_DOWN)
	{
		if (ugaoRavni > -10)
		{
			ugaoRavni -= 10;
		}
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
