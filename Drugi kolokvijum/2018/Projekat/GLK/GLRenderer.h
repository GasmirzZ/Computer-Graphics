#pragma once
#include "DImage.h"
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawAxes();
	UINT LoadTexture(char* fileName);
	void DrawSpaceCube(double a);
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();
	double angleVertical, angleHorizontal, radius, eyex, eyey, eyez;
	bool light = false;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT texShip, texFront, texLeft, texRight, texBack, texTop, texBot;
};
