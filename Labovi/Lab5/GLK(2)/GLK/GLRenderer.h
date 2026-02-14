#pragma once
#include <vector>
#include <tuple>
#include "CGLMaterial.h"

using namespace std;

struct Vertex {
	float x, y, z;
};

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

	void SwitchLight1();
	void SwitchLight2();
	void SwitchLight3();
	void SwitchNormals();
	void RotateView(double dXY, double dXZ);

	
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR;
	double m_cameraAngleXY;
	double m_cameraAngleXZ;
	double m_eyex, m_eyey, m_eyez;

	float angle_v, angle_h;
	bool light0, light1, light2, light3, normals;
	tuple<double, double, double> light0Col, light1Col, light2Col, light3Col;

	float radius1, radius2; //radijus vaze
	float height;
	float heightPrev;

	// Materials
	CGLMaterial light_blue;    // Vaza boja 1 (magenta)
	CGLMaterial dark_blue;     // Vaza boja 2 (cyan)
	CGLMaterial matBase;       // Postolje
	CGLMaterial roomBase;      // Soba
	CGLMaterial blueSphere;    // Plavo svetlo
	CGLMaterial redSphere;     // Crveno svetlo
	CGLMaterial yellowSphere;  // Magenta svetlo

	// Lighting functions
	void AmbientLight();
	void ViewPointLight();
	void Light1();			// Magenta svetlo (Light0)
	void Light2();			// Cyan svetlo (Light1)
	void Light3();			// Crveno svetlo (Light2)

	void DrawAxis();
	void DrawRoom();
	void DrawStand();
	void DrawVase();

	// Primitive drawing functions
	void DrawSphere(float radius);
	void DrawRoller(float radius, float height = 0.3, int sides = 18);
	void DrawPrism(float radius, float height = 0.3, int sides = 4);
	void DrawStrip(double r1, double r2, double h, int nSeg);

	// Helper functions
	float GetX(float radius, float angle);
	float GetY(float radius, float angle);
	void IncRing(float step);
	void DecRing(float step);

	void UpdateCameraPosition();
};
