#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

// Definicije boja za lakse menjanje
//#define COLOR_BG		0.50, 0.75, 1.00  // Svetlo plava pozadina
#define COLOR_BG		0.47, 0.00, 0.00
//#define COLOR_GRID		0.85, 0.85, 0.85
#define COLOR_GRID		1.00, 1.00, 1.00
#define COLOR_AXIS_X	1.00, 0.00, 0.00
#define COLOR_AXIS_Y	0.00, 1.00, 0.00
#define COLOR_AXIS_Z	0.00, 0.00, 1.00
//#define COLOR_POT		0.87, 0.49, 0.16  // Boja saksije
#define COLOR_POT		0.00, 0.80, 0.00
//#define COLOR_CACTUS	0.00, (255 / 255.), 0.00  // Zelena
#define COLOR_CACTUS	1.00, 0.60, 0.00
//#define COLOR_JOINT	0, (170 / 255.0), 0
#define COLOR_JOINT		0.50, 0.75, 1.00
#define COLOR_YELLOW	1.00, 1.00, 0.00  // Zuti deo

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;

	// Inicijalni polozaj kamere
	m_cameraR = 15.0;
	m_cameraAngleXY = 0.0;
	m_cameraAngleXZ = 20.0; // Malo podignuta kamera

	m_yellowPartAngle = 0.0; // Pocetni ugao rotacije dela figure

	UpdateCameraPosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClearColor(COLOR_BG, 0.0);
	glEnable(GL_DEPTH_TEST);  //z buffer kako bi crtao objekte po redu, a ne obrnuto

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //S. brisanje buffera 
	glLoadIdentity(); //S. podesava transformaciju na jedinicnu transformaciju

	// Postavljanje kamere
	gluLookAt(
		m_eyex, m_eyey, m_eyez,	// Gde je oko
		0.0, 4.0, 0.0,			// Gde gleda (centar figure, malo podignut)
		0.0, 1.0, 0.0			// Up vektor
	);

	// Crtanje pomocnih elemenata
	DrawAxis(10.0);
	DrawGrid(10, 10, 10, 10);

	// Crtanje figure
	DrawFigure(m_yellowPartAngle);

	glFlush();

	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	// Alpha - latitude (od -90 do 90), Beta - longitude (od 0 do 360)
	double alphaStep = 180.0 / nSegAlpha;
	double betaStep = 360.0 / nSegBeta;

	for (double beta = 0; beta < 360; beta += betaStep)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (double alpha = -90; alpha <= 90 + alphaStep; alpha += alphaStep)
		{
			// Prva tacka (trenutni beta)
			double radAlpha = ToRad(alpha);
			double radBeta = ToRad(beta);

			double x = r * cos(radAlpha) * cos(radBeta);
			double y = r * sin(radAlpha);
			double z = r * cos(radAlpha) * sin(radBeta);

			glNormal3d(x / r, y / r, z / r);
			glVertex3d(x, y, z);

			// Druga tacka (sledeci beta)
			radBeta = ToRad(beta + betaStep);

			x = r * cos(radAlpha) * cos(radBeta);
			y = r * sin(radAlpha);
			z = r * cos(radAlpha) * sin(radBeta);

			glNormal3d(x / r, y / r, z / r);
			glVertex3d(x, y, z);
		}
		glEnd();
	}
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	double angleStep = 360.0 / nSeg;

	// Donja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		//glNormal3d(0, -1, 0);
		glVertex3d(0, 0, 0);
		for (double angle = 0; angle <= 360; angle += angleStep)
		{
			double rad = ToRad(angle);
			glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
		}
	}
	glEnd();

	// Gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		//glNormal3d(0, 1, 0);
		glVertex3d(0, h, 0);
		for (double angle = 0; angle <= 360; angle += angleStep)
		{
			double rad = ToRad(angle);
			glVertex3d(r2 * cos(rad), h, r2 * sin(rad));
		}
	}
	glEnd();

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (double angle = 0; angle <= 360; angle += angleStep)
		{
			double rad = ToRad(angle);

			//glNormal3d(x, 0, z); // Priblizna normala
			glVertex3d(r2 * cos(rad), h, r2 * sin(rad));
			glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	double angleStep = 360.0 / nSeg;

	// Osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		//glNormal3d(0, -1, 0);
		glVertex3d(0, 0, 0);
		for (double angle = 0; angle <= 360; angle += angleStep)
		{
			double rad = ToRad(angle);
			glVertex3d(r * cos(rad), 0, r * sin(rad));
		}
	}
	glEnd();

	// Omotac
	glBegin(GL_TRIANGLE_FAN);
	{
		//glNormal3d(0, 1, 0);
		glVertex3d(0, h, 0); // Vrh kupe
		for (double angle = 0; angle <= 360; angle += angleStep)
		{
			double rad = ToRad(angle);
			// Normala bi trebala da bude kosa, ali za jednostavnost:
			glNormal3d(cos(rad), 0, sin(rad));
			glVertex3d(r * cos(rad), 0, r * sin(rad));
		}
	}
	glEnd();
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{	
		// X osa
		glColor3f(COLOR_AXIS_X);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);
		// Y osa
		glColor3f(COLOR_AXIS_Y);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);
		// Z osa
		glColor3f(COLOR_AXIS_Z);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
	glLineWidth(1.0);
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	double stepW = width / nSegW;
	double stepH = height / nSegH;

	double startW = -width / 2.;
	double startH = -height / 2.;

	glColor3f(COLOR_GRID);
	glBegin(GL_LINES);
	{
		// Linije paralelne sa Z osom
		for (int i = 0; i <= nSegW; i++)
		{
			glVertex3d(startW + i * stepW, 0, startH);
			glVertex3d(startW + i * stepW, 0, startH + height);
		}
		// Linije paralelne sa X osom
		for (int i = 0; i <= nSegH; i++)
		{
			glVertex3d(startW, 0, startH + i * stepH);
			glVertex3d(startW + width, 0, startH + i * stepH);
		}
	}
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	// Dimenzije
	double potHeight = 1.3;
	double potR1 = 1, potR2 = 1.25;
	double cactusR = 0.45;
	double jointR = 0.3;
	int nSeg = 8;
	int nSegSphere = 50;

	// --- SAKSIJA ---
	glColor3f(COLOR_POT);
	DrawCylinder(potHeight, potR1, potR2, nSeg);
	// Rub saksije
	glTranslated(0, potHeight, 0);
	DrawCylinder(0.6, potR2 + 0.2, potR2 + 0.2, nSeg);

	// Postavljanje na zemlju saksije za kaktus
	glTranslated(0, 0.6, 0);

	// --- KAKTUS GLAVNO STABLO ---
	glColor3f(COLOR_CACTUS);
	DrawCylinder(potHeight, cactusR, cactusR, nSeg); // Trup

	glTranslated(0, potHeight + jointR, 0);
	glColor3f(COLOR_JOINT);
	DrawSphere(jointR, nSegSphere, nSegSphere); // Zglob

	// --- LEVA DONJA GRANA (Fiksna) ---
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0); // Rotacija u levo
		glTranslated(0, jointR, 0); // Pomeraj iz centra zgloba
		glColor3f(COLOR_CACTUS);
		DrawCylinder(potHeight, cactusR, cactusR, nSeg);

		glTranslated(0, potHeight + jointR, 0);
		glColor3f(COLOR_JOINT);
		DrawSphere(jointR, nSegSphere, nSegSphere);

		glPushMatrix(); //levo pa levo
		{
			glRotated(45, 1, 0, 0);
			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);
		}
		glPopMatrix();

		glPushMatrix(); //levo pa gore vertikalno
		{
			glRotated(-45, 1, 0, 0);
			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);

			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);
		}
		glPopMatrix();
	}
	glPopMatrix();

	// --- DESNA DONJA GRANA (Fiksna) ---
	glPushMatrix();
	{
		glRotated(-45, 1, 0, 0); // Rotacija u desno pravilo desne sake
		glTranslated(0, jointR, 0); // Pomeraj iz centra zgloba
		glColor3f(COLOR_CACTUS);
		DrawCylinder(potHeight, cactusR, cactusR, nSeg);

		glTranslated(0, potHeight + jointR, 0);
		glColor3f(COLOR_JOINT);
		DrawSphere(jointR, nSegSphere, nSegSphere);

		glTranslated(0, jointR, 0);
		glColor3f(COLOR_CACTUS);
		DrawCone(potHeight, cactusR, nSeg);

		glTranslated(0, potHeight + jointR, 0);
		glColor3f(COLOR_JOINT);
		DrawSphere(jointR, nSegSphere, nSegSphere);
	}
	glPopMatrix();

	// --- DESNA GRANA (ZUTI DEO - Rotira se) ---
	glPushMatrix();
	{
		// Rotacija koja se kontrolise tastaturom
		glRotated(angle, 1, 0, 0); // dinamicki ugao
		glTranslated(0, jointR, 0);
		glColor3f(COLOR_YELLOW); // Oznacen zutom bojom kako se trazi
		DrawCone(potHeight, cactusR, nSeg); 

		glTranslated(0, potHeight + jointR, 0);
		glColor3f(COLOR_JOINT);
		DrawSphere(jointR, nSegSphere, nSegSphere);

		glPushMatrix();//iznad rotirajuceg desna grana
		{
			glRotated(-45, 1, 0, 0);
			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);
		}
		glPopMatrix();

		glPushMatrix();//iznad rotirajuceg desna grana
		{
			glRotated(45, 1, 0, 0);
			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);

			glTranslated(0, jointR, 0);
			glColor3f(COLOR_CACTUS);
			DrawCylinder(potHeight, cactusR, cactusR, nSeg);

			glTranslated(0, potHeight + jointR, 0);
			glColor3f(COLOR_JOINT);
			DrawSphere(jointR, nSegSphere, nSegSphere);
		}
		glPopMatrix();
	}
	glPopMatrix();

	//glTranslated(0, jointR, 0);
	//glColor3f(COLOR_CACTUS);
	//DrawCylinder(potHeight, cactusR, cactusR, nSeg); // Trup

	//glTranslated(0, potHeight + jointR, 0);
	//glColor3f(COLOR_JOINT);
	//DrawSphere(jointR, nSegSphere, nSegSphere); // Zglob

	//// --- LEVA GORNJA GRANA (Fiksna) ---
	//glPushMatrix();
	//{
	//	glRotated(45, 1, 0, 0); // Rotacija u levo
	//	glTranslated(0, jointR, 0); // Pomeraj iz centra zgloba
	//	glColor3f(COLOR_CACTUS);
	//	DrawCylinder(potHeight, cactusR, cactusR, nSeg);

	//	glTranslated(0, potHeight + jointR, 0);
	//	glColor3f(COLOR_JOINT);
	//	DrawSphere(jointR, nSegSphere, nSegSphere);

	//	glTranslated(0, jointR, 0);
	//	glColor3f(COLOR_CACTUS);
	//	DrawCylinder(potHeight, cactusR, cactusR, nSeg);

	//	glTranslated(0, potHeight + jointR, 0);
	//	glColor3f(COLOR_JOINT);
	//	DrawSphere(jointR, nSegSphere, nSegSphere);
	//}
	//glPopMatrix();

	//// --- DESNA GRANA (ZUTI DEO - Rotira se) ---
	//glPushMatrix();
	//{
	//	// Rotacija koja se kontrolise tastaturom
	//	glRotated(-45 + angle, 1, 0, 0); // Baznih -45 (desno) + dinamicki ugao
	//	glTranslated(0, jointR, 0);
	//	glColor3f(COLOR_YELLOW); // Oznacen zutom bojom kako se trazi
	//	DrawCylinder(potHeight, cactusR, cactusR, nSeg); // Grana

	//	glTranslated(0, potHeight + jointR, 0);
	//	glColor3f(COLOR_JOINT);
	//	DrawSphere(jointR, nSegSphere, nSegSphere);

	//	glTranslated(0, jointR, 0);
	//	glColor3f(COLOR_CACTUS);
	//	DrawCone(potHeight, cactusR, nSeg);

	//	glTranslated(0, potHeight + jointR, 0);
	//	glColor3f(COLOR_JOINT);
	//	DrawSphere(jointR, nSegSphere, nSegSphere);
	//}
	//glPopMatrix();
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	m_cameraAngleXY += dXY;
	m_cameraAngleXZ += dXZ;

	// Ogranicenje elevacije da se kamera ne prevrne
	if (m_cameraAngleXZ > 90) m_cameraAngleXZ = 90;
	if (m_cameraAngleXZ < -90) m_cameraAngleXZ = -90;

	UpdateCameraPosition();
}

void CGLRenderer::ZoomView(double dR)
{
	m_cameraR += dR;
	if (m_cameraR < 2.0) m_cameraR = 2.0; // Minimalni zoom
	if (m_cameraR > 50.0) m_cameraR = 50.0; // Maksimalni zoom
	UpdateCameraPosition();
}

void CGLRenderer::UpdateCameraPosition()
{
	double radXY = ToRad(m_cameraAngleXY);
	double radXZ = ToRad(m_cameraAngleXZ);

	m_eyex = m_cameraR * cos(radXZ) * cos(radXY);
	m_eyey = m_cameraR * sin(radXZ);
	m_eyez = m_cameraR * cos(radXZ) * sin(radXY);
}

void CGLRenderer::RotateYellowPart(double dAngle)
{
	m_yellowPartAngle += dAngle;
}
