
// Lab3PripremaView.cpp : implementation of the CLab3PripremaView class
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab3Priprema.h"
#endif

#include "Lab3PripremaDoc.h"
#include "Lab3PripremaView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TORAD(x) x*3.14/180
#define IMG_PATH "C:\\Faks\\Racunarska grafika\\Computer-Graphics\\Lab3Priprema\\Lab3Priprema\\Slike2\\"


// CLab3PripremaView

IMPLEMENT_DYNCREATE(CLab3PripremaView, CView)

BEGIN_MESSAGE_MAP(CLab3PripremaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab3PripremaView construction/destruction

CLab3PripremaView::CLab3PripremaView() noexcept
{
	// TODO: add construction code here

}

CLab3PripremaView::~CLab3PripremaView()
{
}

BOOL CLab3PripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab3PripremaView drawing

void CLab3PripremaView::OnDraw(CDC* pDC)
{
	CLab3PripremaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	CBitmap* slikaMem = new CBitmap();
	slikaMem->CreateCompatibleBitmap(pDC, 500, 500); //proveri sto 510


	CBitmap* oldMemBmp = MemDC->SelectObject(slikaMem);

	MemDC->SetGraphicsMode(GM_ADVANCED);
	DrawGrid(MemDC);

	Translate(MemDC, 10 * 25 - 7, 10 * 25 - 5);
	Mirror(MemDC, true, false);
	Rotate(MemDC, 35);
	DrawPiece(MemDC, CString(IMG_PATH "1.bmp"), true);

	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	Translate(MemDC, 16 * 25 - 7, 16 * 25 + 4);
	Mirror(MemDC, true, false);
	Rotate(MemDC, 100);
	DrawPiece(MemDC, CString(IMG_PATH "2.bmp"));

	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	Translate(MemDC, 4 * 25 + 2, 16 * 25 - 7);
	Mirror(MemDC, true, false);
	Rotate(MemDC, -47);
	DrawPiece(MemDC, CString(IMG_PATH "3.bmp"));

	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	Translate(MemDC, 16 * 25 + 3, 10 * 25 + 7);
	Mirror(MemDC, false, true);
	Rotate(MemDC, 2.8);
	DrawPiece(MemDC, CString(IMG_PATH "4.bmp"));
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);


	Translate(MemDC, 16 * 25 - 7, 4 * 25 + 3);
	Mirror(MemDC, true, false);
	Rotate(MemDC, 102.3);
	DrawPiece(MemDC, CString(IMG_PATH "5.bmp"));
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	Translate(MemDC, 4 * 25 - 5, 10 * 25 + 8);
	Mirror(MemDC, false, true);
	Rotate(MemDC, -47.2);
	DrawPiece(MemDC, CString(IMG_PATH "6.bmp"));
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	Translate(MemDC, 4 * 25 - 7, 4 * 25 - 5);
	Mirror(MemDC, true, false);
	Rotate(MemDC, 33);
	DrawPiece(MemDC, CString(IMG_PATH "7.bmp"));
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);


	Translate(MemDC, 10 * 25 + 6, 16 * 25 - 6);
	Mirror(MemDC, true, false);
	Rotate(MemDC, -69.5);
	DrawPiece(MemDC, CString(IMG_PATH "8.bmp"));
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);



	Translate(MemDC, 10 * 25 + 6, 4 * 25 + 4);
	Mirror(MemDC, true, false);
	Rotate(MemDC, -147);
	DrawPiece(MemDC, CString(IMG_PATH "9.bmp"));



	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	pDC->BitBlt(0, 0, 500, 500, MemDC, 0, 0, SRCCOPY);


	MemDC->SelectObject(oldMemBmp);
	MemDC->DeleteDC();
	delete MemDC;
	delete slikaMem;
}

void CLab3PripremaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm;
	xForm.eM11 = 1.00f;
	xForm.eM12 = 0.00f;
	xForm.eM21 = 0.00f;
	xForm.eM22 = 1.00f;
	xForm.eDx = dX;
	xForm.eDy = dY;

	if (!rightMultiply)
		pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xForm, MWT_RIGHTMULTIPLY);
}
void CLab3PripremaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm;
	xForm.eM11 = sX;
	xForm.eM12 = 0.00f;
	xForm.eM21 = 0.00f;
	xForm.eM22 = sY;
	xForm.eDx = 0.00f;
	xForm.eDy = 0.00f;

	if (!rightMultiply)
		pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xForm, MWT_RIGHTMULTIPLY);

}
void CLab3PripremaView::Rotate(CDC* pDC, float angle, bool rightMultiply) 
{

	XFORM xForm;
	xForm.eM11 = (float)cos(TORAD(angle));
	xForm.eM12 = (float)sin(TORAD(angle));
	xForm.eM21 = (float)-sin(TORAD(angle));
	xForm.eM22 = (float)cos(TORAD(angle));
	xForm.eDx = 0.00f;
	xForm.eDy = 0.00f;

	if (!rightMultiply)
		pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xForm, MWT_RIGHTMULTIPLY);

}
void CLab3PripremaView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xForm;
	xForm.eM11 = int(mx) == 0 ? 1.00f : -1.00f;
	xForm.eM12 = 0.00f;
	xForm.eM21 = 0.00f;
	xForm.eM22 = int(my) == 0 ? 1.00f : -1.00f;
	xForm.eDx = 0.00f;
	xForm.eDy = 0.00f;

	if (!rightMultiply)
		pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xForm, MWT_RIGHTMULTIPLY);

}


void CLab3PripremaView::DrawGrid(CDC* pDC)
{
	CPen* newPen = new CPen(PS_SOLID, 2, RGB(230, 230, 230));
	CPen* oldPen = pDC->SelectObject(newPen);

	pDC->Rectangle(0, 0, 500, 500);

	for (int i = 0; i <= 500; i += 25)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(oldPen);
	delete newPen;
}


void CLab3PripremaView::DrawPiece(CDC* pDC, CString piece, bool blue)
{
	DImage img;
	img.Load(piece);

	CBitmap* maska = new CBitmap();
	CBitmap* slika = new CBitmap();

	maska->CreateBitmap(img.Width(), img.Height(), 1, 1, NULL); //1 ravan 1 bit za boju znaci monohromatska
	slika->CreateBitmap(img.Width(), img.Height(), 1, 32, img.GetDIBBits()); //rgba


	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);
	
	CBitmap* oldSrcBitmap = srcDC->SelectObject(slika);
	CBitmap* oldDstBitmap = dstDC->SelectObject(maska);

	COLORREF bgColor = srcDC->GetPixel(0, 0);
	COLORREF oldSrcBgColor = srcDC->SetBkColor(bgColor); //sve sto je te boje u masci je 0 a ostalo 1

	dstDC->BitBlt(0, 0, img.Width(), img.Height(), srcDC, 0, 0, SRCCOPY);

	COLORREF oldSrcFrontColor = srcDC->SetTextColor(RGB(255, 255, 255)); //ovo je sad da poredimo sa jedinicama u masci
	srcDC->SetBkColor(RGB(0, 0, 0));
	this->FilterBits(slika, img.BPP() * img.Width() * img.Height(), blue);

	srcDC->BitBlt(0, 0, img.Width(), img.Height(), dstDC, 0, 0, SRCAND);

	srcDC->SetBkColor(oldSrcBgColor);
	srcDC->SetTextColor(oldSrcFrontColor);

	srcDC->SelectObject(oldSrcBitmap);
	dstDC->SelectObject(oldDstBitmap);

	srcDC->DeleteDC();
	dstDC->DeleteDC();

	delete srcDC;
	delete dstDC;

	//vise nam ne trebaju DC-evi napravili smo masku i sliku kao bitmape pa to koristimo dalje


	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	//crtamo masku na pdc
	CBitmap* oldMemBmp = MemDC->SelectObject(maska);
	pDC->BitBlt(-img.Width() / 2, -img.Height() / 2, img.Width(), img.Height(), MemDC, 0, 0, SRCAND);

	oldMemBmp = MemDC->SelectObject(slika);
	pDC->BitBlt(-img.Width() / 2, -img.Height() / 2, img.Width(), img.Height(), MemDC, 0, 0, SRCPAINT);

	MemDC->SelectObject(oldMemBmp);
	MemDC->DeleteDC();
	delete MemDC;
	delete maska;
	delete slika;
}

//filtriranje slike sivo i za plavu je zaplavi
void CLab3PripremaView::FilterBits(CBitmap* map, int count, bool blue)
{
	unsigned char* bits = new unsigned char[count];
	count = map->GetBitmapBits(count, bits);

	int newValue;

	for (int i = 0; i < count; i += 4)
	{
		newValue = 64 + (bits[i] + bits[i + 1] + bits[i + 2]) / 3;

		if (newValue > 255)
			newValue = 255;

		if (blue)
		{
			bits[i] = newValue; //boje ovde idu bgr a ne rgb
			bits[i + 1] = 0;
			bits[i + 2] = 0;
		}
		else
		{
			bits[i] = newValue;
			bits[i + 1] = newValue;
			bits[i + 2] = newValue;
		}
	}


	map->SetBitmapBits(count, bits);

	delete[] bits;
}

// CLab3PripremaView printing

BOOL CLab3PripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab3PripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab3PripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab3PripremaView diagnostics

#ifdef _DEBUG
void CLab3PripremaView::AssertValid() const
{
	CView::AssertValid();
}

void CLab3PripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab3PripremaDoc* CLab3PripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab3PripremaDoc)));
	return (CLab3PripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab3PripremaView message handlers

void CLab3PripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
