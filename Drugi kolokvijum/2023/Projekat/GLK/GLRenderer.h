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

	void setCamera(double vert, double hor, double rad);

	void DrawAxes();

	UINT LoadTexture(char* fileName);

	void DrawEnvCube(double a);

	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);

	void DrawLampBase();

	void DrawLampArm();

	void DrawLampHead();

	void DrawLamp();

	double angle1, angle2, angle3;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	double angleVertical, angleHorizontal, radius;
	UINT texCube[3];
	UINT texLamp;
};
