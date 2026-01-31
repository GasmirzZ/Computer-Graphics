
// Kol2015View.cpp : implementation of the CKol2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2015.h"
#endif

#include "Kol2015Doc.h"
#include "Kol2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(X) X*3.14/180.

// CKol2015View

IMPLEMENT_DYNCREATE(CKol2015View, CView)

BEGIN_MESSAGE_MAP(CKol2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2015View construction/destruction

CKol2015View::CKol2015View() noexcept
{
	// TODO: add construction code here
	blue = new DImage();
	blue->Load(CString("blue.png"));
}

CKol2015View::~CKol2015View()
{
	if (blue)
		delete blue;
}

BOOL CKol2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2015View drawing

void CKol2015View::OnDraw(CDC* pDC)
{
	CKol2015Doc* pDoc = GetDocument();
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


	int oldmode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	memdc->GetWorldTransform(&old);
	
	
	DrawBackground(memdc);
	DrawPacman(memdc, CRect(350, 350, 450, 450), 15);
	DrawGhost(memdc, CRect(100, 120, 196, 216));
	DrawEnd(memdc, rect);
	
	memdc->SetWorldTransform(&old);
	memdc->SetGraphicsMode(oldmode);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldmap);
	memdc->DeleteDC();
	delete memdc;
}

void CKol2015View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	blue->Draw(pDC, CRect(0, 0, blue->Width(), blue->Height()), rect);
}

void CKol2015View::DrawEnd(CDC* pDC, CRect rect)
{
	if (((ghostx - pacmanx) * (ghostx - pacmanx)) + ((ghosty - pacmany) * (ghosty - pacmany)) <= (ghostr + pacmanr) * (ghostr + pacmanr))
	{
		
		CFont font;
		
		font.CreateFont(72, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		int oldmode = pDC->SetBkMode(TRANSPARENT);
		COLORREF oldcolor = pDC->SetTextColor(RGB(255, 255, 255));
		CFont* oldfont = pDC->SelectObject(&font);
		CSize vel = pDC->GetTextExtent(L"The End");
		XFORM old;
		pDC->GetWorldTransform(&old);
		Translate(pDC, rect.Width() / 2, rect.Height()/2, false);
		//Rotate(pDC, -30, false);
		Translate(pDC, -vel.cx / 2 , -vel.cy / 2, false);
		
		TextOut(pDC->m_hDC, 0, 0, L"The End", 7);
		pDC->SetWorldTransform(&old);


		pDC->SetTextColor(oldcolor);
		pDC->SelectObject(oldfont);
		kraj = true;
	}
}

void CKol2015View::DrawGhost(CDC* pDC, CRect rect)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	CBrush crvena(RGB(255, 0, 0));
	CBrush* oldbrush = pDC->SelectObject(&crvena);
	int r = rect.Width() / 2;
	ghostx = rect.left + rect.Width() / 2;
	ghosty = rect.top + rect.Height() / 2;
	ghostr = rect.Width() / 2;
	pDC->BeginPath();
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->MoveTo(rect.right, rect.top + r);
	pDC->ArcTo(rect.left, rect.top, rect.right, rect.bottom, rect.right, rect.top + r, rect.left, rect.top + r);
	pDC->LineTo(rect.left, rect.top + 2*r);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect.left - r / 6, rect.top + 10 * r / 6, rect.left + r / 6, rect.top + 12 * r / 6, rect.left, rect.top + 12 * r / 6, rect.left + r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect.left + r/6, rect.top + 10 * r / 6, rect.left + 3*r / 6, rect.top + 12 * r / 6, rect.left + r/6, rect.top + 11 * r / 6, rect.left + 3*r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect.left + 3*r/6, rect.top + 10 * r / 6, rect.left + 5 * r / 6, rect.top + 12 * r / 6, rect.left + 3 *r / 6, rect.top + 11 * r / 6, rect.left + 5 * r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect.left + 5 * r / 6, rect.top + 10 * r / 6, rect.left + 7 * r / 6, rect.top + 12 * r / 6, rect.left + 5 * r / 6, rect.top + 11 * r / 6, rect.left + 7 * r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect.left + 7 * r / 6, rect.top + 10 * r / 6, rect.left + 9 * r / 6, rect.top + 12 * r / 6, rect.left + 7 * r / 6, rect.top + 11 * r / 6, rect.left + 9 * r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect.left + 9 * r / 6, rect.top + 10 * r / 6, rect.left + 11 * r / 6, rect.top + 12 * r / 6, rect.left + 9 * r / 6, rect.top + 11 * r / 6, rect.left + 11 * r / 6, rect.top + 11 * r / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect.left + 11 * r / 6, rect.top + 10 * r / 6, rect.left + 13 * r / 6, rect.top + 12 * r / 6, rect.left + 11 * r / 6, rect.top + 11 * r / 6, rect.left + 12 * r / 6, rect.top + 12 * r / 6);
	pDC->LineTo(rect.left + 12*r/6, rect.top + 6*r/6);
	pDC->EndPath();

	pDC->StrokeAndFillPath();

	CBrush bela(RGB(255, 255, 255));
	pDC->SelectObject(&bela);

	pDC->Ellipse(ghostx - 15, ghosty - 15, ghostx + 5, ghosty + 5);
	pDC->Ellipse(ghostx + 15, ghosty - 15, ghostx + 35, ghosty + 5);

	CBrush plava(RGB(0, 0, 255));
	pDC->SelectObject(&plava);


	pDC->Ellipse(ghostx - 8, ghosty - 10, ghostx + 2, ghosty);
	pDC->Ellipse(ghostx + 22, ghosty - 10, ghostx + 32, ghosty);


	pDC->SelectObject(oldbrush);
	pDC->SetWorldTransform(&old);
}

void CKol2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	float r = rect.Width();
	
	XFORM old;
	pDC->GetWorldTransform(&old);
	CBrush zuta(RGB(255, 255, 0));
	CBrush crna(RGB(0, 0, 0));
	CBrush* oldbrush = pDC->SelectObject(&zuta);
	float x = r / 2 * cos(TORAD(angleParam * angle));
	float y = r / 2 * sin(TORAD(angleParam * angle));
	pacmanx = r / 2 + rect.left + pomerajx;
	pacmany = r / 2 + rect.top + pomerajy;
	pacmanr = r / 2;
	Scale(pDC, pacmanOrientation, 1, false);
	Translate(pDC, pacmanOrientation*(r / 2 + rect.left+pomerajx), r / 2 + rect.top + pomerajy, false);
	pDC->Pie(-r / 2, -r / 2, r / 2, r / 2, x, -y, x, y);
	pDC->SelectObject(&crna);
	pDC->Ellipse(x / 2 - r / 15, -y - r / 15 - 20, x / 2 + r / 15, -y + r / 15 - 20);
	pDC->SelectObject(oldbrush);
	pDC->SetWorldTransform(&old);
}

void CKol2015View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2015View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CKol2015View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

// CKol2015View printing

BOOL CKol2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2015View diagnostics

#ifdef _DEBUG
void CKol2015View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2015Doc* CKol2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2015Doc)));
	return (CKol2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2015View message handlers

BOOL CKol2015View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}

void CKol2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!kraj)
	{
		if (nChar == VK_LEFT)
		{
			pacmanOrientation = -1;
			pomerajx -= 10;
			angleParam = !angleParam;
		}
		if (nChar == VK_RIGHT)
		{
			pacmanOrientation = 1;
			pomerajx += 10;
			angleParam = !angleParam;
		}
		if (nChar == VK_UP)
		{
			pomerajy -= 10;
			angleParam = !angleParam;
		}
		if (nChar == VK_DOWN)
		{
			pomerajy += 10;
			angleParam = !angleParam;

		}
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
