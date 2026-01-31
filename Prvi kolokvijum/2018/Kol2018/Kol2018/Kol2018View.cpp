
// Kol2018View.cpp : implementation of the CKol2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2018.h"
#endif

#include "Kol2018Doc.h"
#include "Kol2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TORAD(X) X*3.14/180.


// CKol2018View

IMPLEMENT_DYNCREATE(CKol2018View, CView)

BEGIN_MESSAGE_MAP(CKol2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2018View construction/destruction

CKol2018View::CKol2018View() noexcept
{
	bager = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	pozadina = new DImage();

	bager->Load(CString("bager.png"));
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	pozadina->Load(CString("pozadina.png"));

	viljuska = GetEnhMetaFile(CString("viljuska.emf"));

}

CKol2018View::~CKol2018View()
{
	if (bager)
		delete bager;
	if (arm1)
		delete arm1;
	if (arm2)
		delete arm2;
	if (pozadina)
		delete pozadina;

	DeleteEnhMetaFile(viljuska);
	
}

BOOL CKol2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2018View drawing

void CKol2018View::OnDraw(CDC* pDC)
{
	CKol2018Doc* pDoc = GetDocument();
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
	XFORM xformold;
	memdc->GetWorldTransform(&xformold);

	//CRTAJ
	DrawBackground(memdc);
	DrawExcavator(memdc);


	/////////////////
	memdc->SetWorldTransform(&xformold);
	memdc->SetGraphicsMode(oldmode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memdc, 0, 0, SRCCOPY);

	memdc->SelectObject(oldmap);
	memdc->DeleteDC();

}

void CKol2018View::DrawExcavator(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	Translate(pDC, rect.right - bager -> Width() + pomeraj, rect.bottom - bager->Height(), false);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}

void CKol2018View::DrawFork(CDC* pDC)
{
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &header);
	int w = header.rclBounds.right - header.rclBounds.left;
	int h = header.rclBounds.bottom - header.rclBounds.top;
	
	Translate(pDC, 272, 40, false);
	Rotate(pDC, angle3, false);
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false);
	PlayEnhMetaFile(pDC->m_hDC, viljuska, CRect(0, 0, w, h));
}

void CKol2018View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 309, 61, false);
	Rotate(pDC, angle2, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);
}

void CKol2018View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, angle1, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
}


void CKol2018View::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 5, 110, false);
	
}

void CKol2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect rect;
	GetClientRect(&rect);
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF boja = RGB(bytes[2], bytes[1], bytes[0]);
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), boja);
}

void CKol2018View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int x = rect.right - pozadina->Width();
	int y = rect.bottom - pozadina->Height();

	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(x / 2, y, rect.right - x / 2, rect.bottom));
}

void CKol2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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


void CKol2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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



// CKol2018View printing

BOOL CKol2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2018View diagnostics

#ifdef _DEBUG
void CKol2018View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2018Doc* CKol2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2018Doc)));
	return (CKol2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2018View message handlers

BOOL CKol2018View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}

void CKol2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == '1')
		angle1 -= 10;
	if (nChar == '2')
		angle1 += 10;
	if (nChar == '3')
		angle2 -= 10;
	if (nChar == '4')
		angle2 += 10;
	if (nChar == '5')
		angle3 -= 10;
	if (nChar == '6')
		angle3 += 10;
	if (nChar == VK_LEFT)
		pomeraj -= 10;
	if (nChar == VK_RIGHT)
		pomeraj += 10;

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
