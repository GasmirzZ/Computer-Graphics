
// Kol2016View.cpp : implementation of the CKol2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2016.h"
#endif

#include "Kol2016Doc.h"
#include "Kol2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(X) X*3.14/180.


// CKol2016View

IMPLEMENT_DYNCREATE(CKol2016View, CView)

BEGIN_MESSAGE_MAP(CKol2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2016View construction/destruction

CKol2016View::CKol2016View() noexcept
{
	// TODO: add construction code here
	flet2 = new DImage();
	wood = new DImage();
	flet2->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2016\\felt2.jpg"));
	wood->Load(CString("C:\\Faks\\Racunarska grafika\\Prvi kolokvijum\\2016\\wood.jpg"));
}


CKol2016View::~CKol2016View()
{
}

BOOL CKol2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2016View drawing

void CKol2016View::OnDraw(CDC* pDC)
{
	CKol2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* memdc = new CDC();
	memdc->CreateCompatibleDC(pDC);

	CRect rect;
	GetClientRect(&rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	
	CBitmap* oldbmp = memdc->SelectObject(&bmp);

	int oldmode = memdc->SetGraphicsMode(GM_ADVANCED);
	XFORM xformold;
	memdc->GetWorldTransform(&xformold);

	// TODO: add draw code for native data here

	DrawTable(memdc, rect);
	DrawBorder(memdc, rect, 50);
	DrawHoles(memdc, rect, 35);
	Translate(memdc, 600 , 400, false);
	Rotate(memdc, loptaRot, false);
	Translate(memdc, pomerajLoptica, 0, false);
	DrawBall(memdc, 30);
	Translate(memdc, -pomerajLoptica, 0, false);
	Rotate(memdc, -loptaRot + stapRot, false);
	Translate(memdc, -pomeraj - w, 0, false);
	DrawStick(memdc, w);



	memdc->SetWorldTransform(&xformold);
	memdc->SetGraphicsMode(oldmode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldbmp);

	memdc->DeleteDC();
	delete memdc;

}

void CKol2016View::DrawTable(CDC* pDC, CRect rect) 
{
	int h = flet2->Height();
	int w = flet2->Width();
	for (int i = 0; i <= rect.right; i += w)
		for (int j = 0; j <= rect.bottom; j += h)
			flet2->Draw(pDC, CRect(0, 0, w, h), CRect(i,j,i+w,j+h));
}

void CKol2016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	CRect rect2(&rect);
	
	rect.right = w;
	wood->Draw(pDC, CRect(0, 0, w, wood->Height()), rect); //levo
	rect.right = rect2.right;
	rect.left = rect2.right - w;
	wood->Draw(pDC, CRect(0, 0, w, wood->Height()), rect); //desno
	rect.left = rect2.left;
	rect.bottom = w;
	wood->Draw(pDC, CRect(0, 0, wood->Width(), w), rect); //gore
	rect.bottom = rect2.bottom;
	rect.top = rect2.bottom - w;
	wood->Draw(pDC, CRect(0, 0, wood->Width(), w), rect); //dole
}

void CKol2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	int gore = size;
	int dole = rect.Height() - 2 * size;
	CBrush crna(RGB(0, 0, 0));
	CBrush* oldbrush = pDC->SelectObject(&crna);
	for(int i = size; i <= rect.Width(); i+= rect.Width()/2 - 3*size/2)
	{
		pDC->Ellipse(i, gore, i + size, gore + size);
		pDC->Ellipse(i, dole, i + size, dole + size);
	}

	pDC->SelectObject(oldbrush);
}

void CKol2016View::DrawBall(CDC* pDC, int w)
{
	
	CPen crvena(PS_SOLID, 1, RGB(150, 0, 0));
	CPen bela(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldpen = pDC->SelectObject(&crvena);

	CBrush tamna(RGB(100, 0, 0));
	CBrush* oldbrush = pDC->SelectObject(&tamna);

	pDC->Ellipse(-w / 2, -w / 2, w / 2, w / 2);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(bela);
	pDC->Ellipse(-w / 6 + w/10, -w / 6 - w / 10, w / 6 + w / 10, w / 6 - w / 10);

	pDC->SelectObject(oldpen);

	
}


void CKol2016View::DrawStick(CDC* pDC, int w)
{
	CBrush oker(RGB(210, 170, 60));
	CBrush braon(RGB(50, 35, 10));
	CPen braonpen(PS_SOLID, 1, RGB(50, 35, 10));
	CPen belapen(PS_SOLID, 1, RGB(255, 255, 255));

	CPen* oldpen = pDC->SelectObject(&braonpen);
	CBrush* oldbrush = pDC->SelectObject(&braon);
	CPoint tacke[4];
	CPoint tacke2[4];

	tacke[0] = CPoint(0, 0);
	tacke[1] = CPoint(w/3., 0);
	tacke[2] = CPoint(w / 3., w / 100. + (2 / 3.) * w/100.);
	tacke[3] = CPoint(0, w/50.);
	tacke2[0] = CPoint(w / 3., 0);
	tacke2[1] = CPoint(w / 3., 1.67 * w / 100);
	tacke2[2] = CPoint(w, w / 100);
	tacke2[3] = CPoint(w, 0);

	pDC->Polygon(tacke, 4);
	pDC->SelectObject(&oker);
	pDC->Polygon(tacke2, 4);
	pDC->SelectObject(&belapen);
	pDC->MoveTo(2, 2);
	pDC->LineTo(w, 2);
	pDC->MoveTo(0, 0);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

}

void CKol2016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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


// CKol2016View printing

BOOL CKol2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2016View diagnostics

#ifdef _DEBUG
void CKol2016View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2016Doc* CKol2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2016Doc)));
	return (CKol2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2016View message handlers

BOOL CKol2016View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
	//return CView::OnEraseBkgnd(pDC);
}

void CKol2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_DOWN)
	{
		pomeraj += 10;
		
	}
	if (nChar == VK_UP)
	{
		if (pomeraj - 30 / 2 > 0)
			pomeraj -= 10;
		else
		{
			pomeraj = 30 / 2;
			pomerajLoptica += 10;
		}
		
	}
	if (nChar == VK_LEFT)
	{
		stapRot -= 10;
		if(pomeraj > 30/2)
		{
			loptaRot -= 10;
		}
	}
	if (nChar == VK_RIGHT)
	{
		stapRot += 10;
		if (pomeraj > 30 / 2)
		{
			loptaRot += 10;
		}
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
