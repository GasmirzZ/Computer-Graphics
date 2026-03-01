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
	void DrawPatch(double R, int n);
	void DrawEarth(double R, int tes);
	void DrawMoon();
	void DrawSpace();
	double angleVertical, angleHorizontal, radius, eyex, eyey, eyez, angleMoon;
	bool light;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT texTSC[6], texM[6], texS[6];
};
