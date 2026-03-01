#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	angleVertical = 20;
	angleHorizontal = 30;
	radius = 40;
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

	//teksture i svetla
	glEnable(GL_LIGHTING);
	float belo[] = { 1, 1, 1, 1 };
	float ambient[] = { 0.3, 0.3, 0.3, 1 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, belo);
	glLightfv(GL_LIGHT0, GL_SPECULAR, belo);

	glEnable(GL_TEXTURE_2D);
	texShip = LoadTexture("ShipT1.png");
	texFront = LoadTexture("front.jpg");
	texLeft= LoadTexture("left.jpg");
	texRight = LoadTexture("right.jpg");
	texBack = LoadTexture("back.jpg");
	texTop = LoadTexture("top.jpg");
	texBot = LoadTexture("bot.jpg");
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//podesis svetlo
	float position[] = { 0, 0, 1, 0 };


	//podesavanja kamere
	double radV = angleVertical * 3.14 / 180.;
	double radH = angleHorizontal * 3.14 / 180.;
	eyex = 0 + radius * cos(radV) * sin(radH);
	eyey = 10 + radius * sin(radV);
	eyez = 0 + radius * cos(radV) * cos(radH);

	gluLookAt(eyex, eyey, eyez, 0, 10, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glDisable(GL_LIGHTING);
	DrawAxes();
	DrawSpaceCube(100);
	if (light)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

	}
	DrawTriangle(5.8, 2.15, 3);
	//DrawShip();

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
	glDeleteTextures(1, &texBack);
	glDeleteTextures(1, &texFront);
	glDeleteTextures(1, &texLeft);
	glDeleteTextures(1, &texRight);
	glDeleteTextures(1, &texTop);
	glDeleteTextures(1, &texBot);
	glDeleteTextures(1, &texShip);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glLineWidth(2);
	glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
	glEnable(GL_TEXTURE_2D);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawSpaceCube(double a)
{
	glPushMatrix();
	double d = a / 2;
	//glTranslatef(0, d, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texFront);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-d, d, -d);
	glTexCoord2f(1, 0);
	glVertex3f(d, d, -d);
	glTexCoord2f(1, 1);
	glVertex3f(d, -d, -d);
	glTexCoord2f(0, 1);
	glVertex3f(-d, -d, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texBack);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(d, d, d);
	glTexCoord2f(1, 0);
	glVertex3f(-d, d, d);
	glTexCoord2f(1, 1);
	glVertex3f(-d, -d, d);
	glTexCoord2f(0, 1);
	glVertex3f(d, -d, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texLeft);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-d, d, d);
	glTexCoord2f(1, 0);
	glVertex3f(-d, d, -d);
	glTexCoord2f(1, 1);
	glVertex3f(-d, -d, -d);
	glTexCoord2f(0, 1);
	glVertex3f(-d, -d, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texRight);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(d, d, -d);
	glTexCoord2f(1, 0);
	glVertex3f(d, d, d);
	glTexCoord2f(1, 1);
	glVertex3f(d, -d, d);
	glTexCoord2f(0, 1);
	glVertex3f(d, -d, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texBot);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-d, -d, -d);
	glTexCoord2f(1, 0);
	glVertex3f(d, -d, -d);
	glTexCoord2f(1, 1);
	glVertex3f(d, -d, d);
	glTexCoord2f(0, 1);
	glVertex3f(-d, -d, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texTop);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-d, d, d);
	glTexCoord2f(1, 0);
	glVertex3f(d, d, d);
	glTexCoord2f(1, 1);
	glVertex3f(d, d, -d);
	glTexCoord2f(0, 1);
	glVertex3f(-d, d, -d);
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	glPushMatrix();
	glScalef(1, -1, 1);
	glEnable(GL_TEXTURE_2D);
	double alfa = atan(d2 / d1);
	glRotatef(-alfa * 180. / 3.14, 0, 0, 1);
	double h = sin(alfa) * d1;
	double beta = acos(h / d2);
	double d = sin(beta) * d2;
	double d3 = sqrt(d1 * d1 + d2 * d2);

	double xtex = h / d3;
	double ytex = d / d3;

	glBindTexture(GL_TEXTURE_2D, texShip);

	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.5 * rep, 1.*rep);
	glVertex3f(d1, d2, 0);
	
	glNormal3f(0, 0, 1);
	glTexCoord2f((0.5 + xtex) * rep, (ytex) * rep);
	glVertex3f(d1, 0, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.5 * rep, 0);
	glVertex3f(0, 0, 0);

	glEnd();

	glPopMatrix();
}


void CGLRenderer::DrawShip()
{

}



