#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_angleHorizontal = 30;
	m_angleVertical = 10;
	m_radius = 40;
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

	m_texSpider = LoadTexture("spider.png");
	m_texEnv[0] = LoadTexture("front.jpg");
	m_texEnv[1] = LoadTexture("back.jpg");
	m_texEnv[2] = LoadTexture("left.jpg");
	m_texEnv[3] = LoadTexture("right.jpg");
	m_texEnv[4] = LoadTexture("top.jpg");
	m_texEnv[5] = LoadTexture("bot.jpg");
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// ovde sad ide crtanje
	double radH = m_angleHorizontal * 3.14 / 180.0;
	double radV = m_angleVertical * 3.14 / 180.0;
	double eyeX = 0 + m_radius * cos(radV) * sin(radH);
	double eyeY = 10 + m_radius * sin(radV);
	double eyeZ = 0 + m_radius * cos(radV) * cos(radH);
	gluLookAt(eyeX, eyeY, eyeZ, 0, 10, 0, 0, 1, 0);

	//inicijalna tacka 0, 10, 0 je zadata u zadatku zato su hardkodirane 0 10 0 vrednosti

	
	DrawAxes(pDC);
	DrawEnvCube(100);
	//glTranslatef(0, -50, 0);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);
	//DrawSphere(1, 20, 0.25, 0.25, 0.24);
	//DrawCone(1, 15, 10, 0.75, 0.75, 0.24);
	
	//glRotatef(90, 0, 0, 1);
	//glRotatef(45, 1, 0, 0);
	//DrawAxes(pDC);
	//DrawLeg();
	DrawSpider();
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

	glDeleteTextures(1, &m_texSpider);
	glDeleteTextures(6, m_texEnv);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glLineWidth(2.0);
	glDisable(GL_TEXTURE_2D);

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


void CGLRenderer::DrawEnvCube(double a)
{
	glPushMatrix();
	glTranslatef(0, a / 2, 0);
	glEnable(GL_TEXTURE_2D);

	//FRONT
	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	//posto imamo front stranicu njena pozadina ce biti unutra 
	//pa cemo da redjamo anti clockwise

	glTexCoord2f(0, 0);// ovo se gleda kao da je gornji levi ugao texture
	glVertex3d(-a / 2, a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, a / 2, -a / 2);

	glEnd();

	//BACK
	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	//posto imamo back stranicu - crta se njena zadnja strana koja ce se videti kad se okrenemo za 90
	//pa cemo da redjamo clockwise

	glTexCoord2f(0, 0);// ovo se gleda kao da je gornji levi ugao texture
	glVertex3d(a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glEnd();

	//LEFT
	glBindTexture(GL_TEXTURE_2D, m_texEnv[2]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0, 0);// ovo se gleda kao da je gornji levi ugao texture
	glVertex3d(-a / 2, a / 2, a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);

	glEnd();


	//RIGHT
	glBindTexture(GL_TEXTURE_2D, m_texEnv[3]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0, 0);// ovo se gleda kao da je gornji levi ugao texture
	glVertex3d(a / 2, a / 2, -a / 2);

	glTexCoord2f(0, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, a / 2, a / 2);

	glEnd();


	//TOP
	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3d(a / 2, a / 2, -a / 2);

	glEnd();


	//BOT
	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glTexCoord2f(1, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3d(a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glEnd();
	glPopMatrix();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	double step1 = 2* 3.14 / nSeg; //latituda
	double step2 = 3.14 / (2*nSeg); //longituda

	for (double i = -3.14; i <= 3.14; i += step2)
	{
		glBegin(GL_QUAD_STRIP);

		for (double j = 0; j <= 2*3.14; j += step1)
		{
			double x1 = r * cos(i) * cos(j);
			double y1 = r * sin(i);
			double z1 = r * cos(i) * sin(j);

			double x2 = r * cos(i + step1) * cos(j);
			double y2 = r * sin(i + step1);
			double z2 = r * cos(i + step1) * sin(j);

			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;

			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * z2/ r + texV;

			glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);
			glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);
		}

		glEnd();
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double angleStep = 2*3.14 / nSeg;
	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(texU, texV);
	glVertex3f(0, h, 0);

	for (double i = 0; i <= 2* 3.14 ; i += angleStep)
	{
		double x = r * cos(i);
		double y = 0;
		double z = r * sin(i);

		double tx = texR * x / r + texU;
		double ty = texR * z / r + texV;

		glTexCoord2f(tx, ty);
		glVertex3f(x, y, z);

	}

	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	
	DrawSphere(r, nSeg, 0.25, 0.25, 0.24);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);

}


void CGLRenderer::DrawLeg()
{
	glPushMatrix();

	DrawLegSegment(1, 10, 5);
	glTranslatef(0, 11, 0);
	glRotatef(85, 1, 0, 0);
	DrawLegSegment(1, 15, 5);

	glPopMatrix();
}


void CGLRenderer::DrawSpiderBody()
{
	glPushMatrix();
	glScalef(1, 0.5, 1);
	DrawSphere(3, 10, 0.25, 0.25, 0.24);
	glTranslatef(- 3 - 1, 0, 0);
	DrawSphere(2, 10, 0.75, 0.25, 0.24);
	glPopMatrix();
	glPushMatrix();
	glScalef(1, 0.8, 1);
	glTranslatef(5 + 1.5, 0, 0);
	DrawSphere(5, 10, 0.25, 0.25, 0.24);
	glPopMatrix();
}


void CGLRenderer::DrawSpider()
{
	glPushMatrix();
	glTranslatef(0, 5, 0);
	DrawSpiderBody();
	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();             
		glRotatef(45, 1, 0, 0);
		DrawLeg();
		glPopMatrix();


		glRotatef(-30, 0, 1, 0);
	}
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-135, 0, 1, 0);
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(45, 1, 0, 0);
		DrawLeg();
		glPopMatrix();


		glRotatef(-30, 0, 1, 0);
	}
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}


