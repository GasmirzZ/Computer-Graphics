#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	angleHorizontal = 30;
	angleVertical = 20;
	radius = 40;
	angle1 = 0;
	angle2 = 0;
	angle3 = 0;
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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	//loaduj texture
	texLamp = LoadTexture("lamp.jpg");
	texCube[0] = LoadTexture("top.jpg");
	texCube[1] = LoadTexture("side.jpg");
	texCube[2] = LoadTexture("bot.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//kamera
	double radH = angleHorizontal * 3.14 / 180.0;
	double radV = angleVertical * 3.14 / 180.0;
	double eyex = 0 + radius * cos(radV) * sin(radH);
	double eyey = 10 + radius * sin(radV);
	double eyez = 0 + radius * cos(radV) * cos(radH);
	gluLookAt(eyex, eyey, eyez, 0, 10, 0, 0, 1, 0);
	//crtanje

	DrawAxes();
	DrawEnvCube(100);
	//DrawLampBase();
	//DrawLampArm();
	//DrawLampHead();
	DrawLamp();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);

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


void CGLRenderer::setCamera(double vert, double hor, double rad)
{
	if(angleVertical + vert <= 1.5*180.0/3.14 && angleVertical + vert >= 0.1 * 180.0 / 3.14)
		angleVertical += vert;
	
	angleHorizontal += hor;
	if(radius + rad >= 5 && radius + rad <= 60)
		radius += rad;
	
}

void CGLRenderer::DrawAxes()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 50);

	glEnd();

	glColor3f(1, 1, 1);
}


UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawEnvCube(double a)
{
	glPushMatrix();
	glTranslatef(0, a / 2, 0);
	glEnable(GL_TEXTURE_2D);

	//top
	glBindTexture(GL_TEXTURE_2D, texCube[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glEnd();

	//sides
	//right
	glBindTexture(GL_TEXTURE_2D, texCube[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(a / 2, a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, a / 2, a / 2);
	glEnd();



	//sides
	//left
	//glBindTexture(GL_TEXTURE_2D, texCube[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(-a / 2, a / 2, a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glEnd();


	//sides
	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, a / 2, -a / 2);
	glEnd();



	//sides
	//left
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(a / 2, a / 2, a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glEnd();


	//bot
	glBindTexture(GL_TEXTURE_2D, texCube[2]);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, -a / 2, a / 2);
	

	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	glPushMatrix();
	//glTranslatef(0, 5, 0);
	glEnable(GL_TEXTURE_2D);
	double step = 360.0 / nSeg;
	double x, z, tx, ty;
	if (!bIsOpen)
	{
		glBindTexture(GL_TEXTURE_2D, texLamp);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5, 0.25);
		glVertex3f(0, 0, 0);
		for (double angle = 0; angle <= 360; angle += step)
		{
			x = r1 * cos(angle * 3.14 / 180.);
			z = r1 * sin(angle * 3.14 / 180.);

			tx = 0.5 * x / r1 + 0.5;
			ty = 0.25 * z / r1 + 0.25;

			glTexCoord2f(tx, ty);
			glVertex3f(x, 0, z);
		}
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5, 0.25);
		glVertex3f(0, h, 0);
		for (double angle = 0; angle <= 360; angle += step)
		{
			x = r2 * cos(angle * 3.14 / 180.);
			z = r2 * sin(angle * 3.14 / 180.);

			tx = 0.5 * x / r2 + 0.5;
			ty = 0.25 * z / r2 + 0.25;

			glTexCoord2f(tx, ty);
			glVertex3f(x, h, z);
		}
		glEnd();
	}
	double texStep;
	double tcoordy1, tcoordy2;
	if (!texMode)
	{
		texStep = 2. / nSeg;
		tx = 2.0;
		tcoordy1 = 0;
		tcoordy2 = 0.5;
	}
	else
	{
		texStep = 2. / nSeg;
		tx = 2.0;
		tcoordy1 = 0.5;
		tcoordy2 = 1;
	}
	glBegin(GL_QUAD_STRIP);
	for (double angle = 0; angle < 360 + step; angle += step)
	{
		x = r2 * cos(angle * 3.14 / 180.);
		z = r2 * sin(angle * 3.14 / 180.);

		glTexCoord2f(tx, tcoordy1);
		glVertex3f(x, h, z);


		x = r1 * cos(angle * 3.14 / 180.);
		z = r1 * sin(angle * 3.14 / 180.);

		glTexCoord2f(tx, tcoordy2);
		glVertex3f(x, 0, z);
		tx -= texStep;
	}
	glEnd();
	/*double texStep = 2.0 / nSeg;
	tx = 2.0;
	glBegin(GL_QUADS);
	for (double angle = 0; angle <= 360; angle += step)
	{
		x = r2 * cos(angle * 3.14 / 180.);
		z = r2 * sin(angle * 3.14 / 180.);

		glTexCoord2f(tx, tcoordy1);
		glVertex3f(x, h, z);

		x = r1 * cos(angle * 3.14 / 180.);
		z = r1 * sin(angle * 3.14 / 180.);

		glTexCoord2f(tx, tcoordy2);
		glVertex3f(x, 0, z);

		x = r1 * cos((angle + step) * 3.14 / 180.);
		z = r1 * sin((angle + step) * 3.14 / 180.);

		glTexCoord2f(tx - texStep, tcoordy2);
		glVertex3f(x, 0, z);

		x = r2 * cos((angle + step) * 3.14 / 180.);
		z = r2 * sin((angle + step) * 3.14 / 180.);

		glTexCoord2f(tx - texStep, tcoordy1);
		glVertex3f(x, h, z);


		tx -= texStep;
	}*/
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CGLRenderer::DrawLampBase()
{
	DrawCylinder(8, 7, 2, 20, 1, 0);
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glRotatef(-90, 0, 1, 0);
	//DrawAxes();
	DrawCylinder(3, 3, 2, 20, 1, 0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	DrawCylinder(1, 1, 15, 20, 1, 0);
	glPopMatrix();
}

void CGLRenderer::DrawLampHead()
{
	//glTranslatef(0, 10, 0);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, -1, 0);
	DrawCylinder(2, 2, 2, 20, 1, 0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3, -7.5, 0);
	DrawCylinder(6, 3, 5, 20, 0, 1);
	glTranslatef(0, 5, 0);
	DrawCylinder(3, 3, 5, 20, 1, 0);
	glTranslatef(0, 5, 0);
	DrawCylinder(3, 2, 1, 20, 1, 0);
	glPopMatrix();
}

void CGLRenderer::DrawLamp()
{
	glPushMatrix();
	DrawLampBase();
	glTranslatef(0, 1.5, 0);
	glRotatef(30 + angle1, 0, 0, 1);
	DrawLampArm();
	glTranslatef(0, 16.5, 0);
	glRotatef(-60 + angle2, 0, 0, 1);
	DrawLampArm();
	glTranslatef(0, 16.5, 0);
	glRotatef(90 + angle3, 0, 0, 1);
	DrawLampHead();
	//DrawAxes();
	glPopMatrix();
}