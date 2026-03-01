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
	radius = 30;
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

	texTSC[0] = LoadTexture("TSC0.jpg");
	texTSC[1] = LoadTexture("TSC1.jpg");
	texTSC[2] = LoadTexture("TSC2.jpg");
	texTSC[3] = LoadTexture("TSC3.jpg");
	texTSC[4] = LoadTexture("TSC4.jpg");
	texTSC[5] = LoadTexture("TSC5.jpg");

	texM[0] = LoadTexture("M0.jpg");
	texM[1] = LoadTexture("M1.jpg");
	texM[2] = LoadTexture("M2.jpg");
	texM[3] = LoadTexture("M3.jpg");
	texM[4] = LoadTexture("M4.jpg");
	texM[5] = LoadTexture("M5.jpg");

	texS[0] = LoadTexture("S0.jpg");
	texS[1] = LoadTexture("S1.jpg");
	texS[2] = LoadTexture("S2.jpg");
	texS[3] = LoadTexture("S3.jpg");
	texS[4] = LoadTexture("S4.jpg");
	texS[5] = LoadTexture("S5.jpg");

	float belo[] = { 1,1,1,1 };
	float ambient[] = { 0,0,0,1 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, belo);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, belo);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float position[] = { 0,0,1,0 };
	double radV = angleVertical * 3.14 / 180.;
	double radH = angleHorizontal * 3.14 / 180.;
	eyex = 0 + radius * cos(radV) * sin(radH);
	eyey = 5 + radius * sin(radV);
	eyez = 0 + radius * cos(radV) * cos(radH);

	gluLookAt(eyex, eyey, eyez, 0, 5, 0, 0, 1, 0);
	glDisable(GL_LIGHTING);
	DrawAxes();

	glDisable(GL_DEPTH_TEST);
	//svemir ovde
	glPushMatrix();
	glTranslated(eyex, eyey, eyez); // <-- Centriraj svemir na samu kameru!
	DrawSpace();                    // (u ovom slucaju moci ce i DrawPatch(1, 20))
	glPopMatrix();
	if (light)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}
	//ovde planete jer utice svetlo
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0, 5, 0);
	DrawEarth(5, 20);
	glTranslatef(0, 0, -50);
	DrawMoon();

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
	gluPerspective(40, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(6, texTSC);
	glDeleteTextures(6, texM);
	glDeleteTextures(6, texS);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 1, 0);
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

void CGLRenderer::DrawPatch(double R, int n)
{
	
	double step = 2.0 / n;
	for (double i = -1.0; i <= 1.0 - step; i+=step)
	{
		glBegin(GL_QUAD_STRIP);
		double alfa = atan(i);
		double alfaNext = atan(i + step);
		for (double j = 1.0; j >= -1.0; j -= step)
		{
			double beta = atan(j * cos(alfa));
			double betaNext = atan((j)*cos(alfaNext));
			double x1 = R * cos(beta) * sin(alfa);
			double y1 = R * sin(beta);
			double z1 = R * cos(beta) * cos(alfa);

			double x2 = R * cos(betaNext) * sin(alfaNext);
			double y2 = R * sin(betaNext);
			double z2 = R * cos(betaNext) * cos(alfaNext);

			glNormal3d(x1 / R, y1 / R, z1 / R);
			glTexCoord2f((i + 1.0) / 2.0, 1-(j + 1.0) / 2.0);
			glVertex3f(x1, y1, z1);

			glNormal3d(x2 / R, y2 / R, z2 / R);
			glTexCoord2f((i + step + 1.0) / 2.0, 1-(j + 1.0) / 2.0);
			glVertex3f(x2, y2, z2);
		}
		glEnd();

	}
}

void CGLRenderer::DrawEarth(double R, int tes)
{
	glEnable(GL_TEXTURE_2D); 
	glPushMatrix();
	//glRotatef(180, 0, 0, 1);
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texTSC[i]);
		DrawPatch(R, tes);
		glRotatef(90, 0, 1, 0);

	}

	glBindTexture(GL_TEXTURE_2D, texTSC[4]);
	glRotatef(-90, 1, 0, 0);
	DrawPatch(R, tes);

	glBindTexture(GL_TEXTURE_2D, texTSC[5]);
	glRotatef(180, 1, 0, 0);
	DrawPatch(R, tes);


	glPopMatrix();
}

void CGLRenderer::DrawMoon()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//glRotatef(180, 0, 0, 1);
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texM[i]);
		DrawPatch(1, 20);
		glRotatef(90, 0, 1, 0);

	}

	glBindTexture(GL_TEXTURE_2D, texM[4]);
	glRotatef(-90, 1, 0, 0);
	DrawPatch(1, 20);

	glBindTexture(GL_TEXTURE_2D, texM[5]);
	glRotatef(180, 1, 0, 0);
	DrawPatch(1, 20);


	glPopMatrix();
}

void CGLRenderer::DrawSpace()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//glRotatef(180, 0, 0, 1);
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texS[i]);
		DrawPatch(1, 20);
		glRotatef(90, 0, 1, 0);

	}

	glBindTexture(GL_TEXTURE_2D, texS[4]);
	glRotatef(-90, 1, 0, 0);
	DrawPatch(1, 20);

	glBindTexture(GL_TEXTURE_2D, texS[5]);
	glRotatef(180, 1, 0, 0);
	DrawPatch(1, 20);


	glPopMatrix();
}