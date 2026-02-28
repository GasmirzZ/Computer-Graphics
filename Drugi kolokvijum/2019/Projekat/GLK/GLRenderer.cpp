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
	angle1 = 0;
	angle2 = 0;
	angle3 = 0;
	angle4 = 0;
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

	//ucitaj teksture ovde
	texExcavator = LoadTexture("excavator.png");
	texTop = LoadTexture("top.jpg");
	texFront = LoadTexture("front.jpg");
	texLeft = LoadTexture("left.jpg"); 
	texRight = LoadTexture("right.jpg");
	texBack = LoadTexture("back.jpg"); 
	texBot = LoadTexture("bot.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	double angleV = angleVertical * 3.14 / 180.;
	double angleH = angleHorizontal * 3.14 / 180.;
	double eyex = 0 + radius * cos(angleV) * sin(angleH);
	double eyey = 5 + radius * sin(angleV);
	double eyez= 0 + radius * cos(angleV) * cos(angleH);
	gluLookAt(eyex, eyey, eyez, 0, 5, 0, 0, 1, 0);

	DrawAxes();
	DrawEnvCube(100);
	DrawExcavator();

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
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
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
	glDeleteTextures(1, &texTop);
	glDeleteTextures(1, &texBot);
	glDeleteTextures(1, &texLeft);
	glDeleteTextures(1, &texRight);
	glDeleteTextures(1, &texExcavator);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawAxes() 
{
	glLineWidth(2);
	glDisable(GL_TEXTURE_2D);

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

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawEnvCube(double a)
{
	glPushMatrix();
	double d = a / 2;
	glTranslatef(0, d, 0);

	glEnable(GL_TEXTURE_2D);
	
	//top
	glBindTexture(GL_TEXTURE_2D, texTop);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-d, d, -d);

	glTexCoord2f(1, 0);
	glVertex3f(d, d, -d);

	glTexCoord2f(1, 1);
	glVertex3f(d, d, d);

	glTexCoord2f(0, 1);
	glVertex3f(-d, d, d);
	glEnd();

	//bot
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

	//right
	glBindTexture(GL_TEXTURE_2D, texRight);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(d, -d, -d);

	glTexCoord2f(0, 1);
	glVertex3f(d, -d, d);

	glTexCoord2f(0, 0);
	glVertex3f(d, d, d);

	glTexCoord2f(1, 0);
	glVertex3f(d, d, -d);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, texLeft);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-d, -d, d);

	glTexCoord2f(0, 1);
	glVertex3f(-d, -d, -d);

	glTexCoord2f(0, 0);
	glVertex3f(-d, d, -d);

	glTexCoord2f(1, 0);
	glVertex3f(-d, d, d);
	glEnd();


	//front
	glBindTexture(GL_TEXTURE_2D, texFront);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-d, -d, -d);

	glTexCoord2f(0, 1);
	glVertex3f(d, -d, -d);

	glTexCoord2f(0, 0);
	glVertex3f(d, d, -d);

	glTexCoord2f(1, 0);
	glVertex3f(-d, d, -d);
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, texBack);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(d, -d, d);

	glTexCoord2f(0, 1);
	glVertex3f(-d, -d, d);

	glTexCoord2f(0, 0);
	glVertex3f(-d, d, d);

	glTexCoord2f(1, 0);
	glVertex3f(d, d, d);
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	POINTF center = {0.0, 0.0};
	POINTF texCenter = { 0.0, 0.0 };

	for (int i = 0; i < n; i++)
	{
		center.x += points[i].x;
		center.y += points[i].y;

		texCenter.x += texCoords[i].x;
		texCenter.y += texCoords[i].y;
	}

	center.x /= n;
	center.y /= n;

	texCenter.x /= n;
	texCenter.y /= n;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExcavator);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(texCenter.x, texCenter.y);
	glVertex3f(center.x, center.y, 0);

	for (int i = 0; i < n; i++)
	{
		glTexCoord2f(texCoords[i].x, texCoords[i].y);
		glVertex3f(points[i].x, points[i].y, 0);
	}

	glTexCoord2f(texCoords[0].x, texCoords[0].y);
	glVertex3f(points[0].x, points[0].y, 0);
	glEnd();

}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glPushMatrix();
	glTranslatef(0 , 0, -zh/2);
	DrawPolygon(points, texCoords, n);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	glColor3f(r, g, b);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(points[i % n].x, points[i % n].y, 0);
		glVertex3f(points[i % n].x, points[i % n].y, zh);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, zh);
	DrawPolygon(points, texCoords, n);

	glPopMatrix();
}


void CGLRenderer::DrawBase()
{
	POINTF points[8];
	POINTF texPoints[8];
	points[0].x = 0.5 * 1;
	points[0].y = 0.5 * 4;
	points[1].x = 0.5 * 15;
	points[1].y = 0.5 * 4;
	points[2].x = 0.5 * 16;
	points[2].y = 0.5 * 3;
	points[3].x = 0.5 * 16;
	points[3].y = 0.5 * 1;
	points[4].x = 0.5 * 15;
	points[4].y = 0.5 * 0;
	points[5].x = 0.5 * 1;
	points[5].y = 0.5 * 0;
	points[6].x = 0.5 * 0;
	points[6].y = 0.5 * 1;
	points[7].x = 0.5 * 0;
	points[7].y = 0.5 * 3;


	texPoints[0].x = 1 * 1.0 / 16;
	texPoints[0].y = (16 - 4) * 1.0 / 16;
	texPoints[1].x = 15 * 1.0 / 16;
	texPoints[1].y = (16 - 4) * 1.0 / 16;
	texPoints[2].x = 16 * 1.0 / 16;
	texPoints[2].y = (16 - 3) * 1.0 / 16;
	texPoints[3].x = 16 * 1.0 / 16;
	texPoints[3].y = (16 - 1) * 1.0 / 16;
	texPoints[4].x = 15 * 1.0 / 16;
	texPoints[4].y = 16 * 1.0 / 16;
	texPoints[5].x = 1 * 1.0 / 16;
	texPoints[5].y = 16 * 1.0 / 16;
	texPoints[6].x = 0 * 1.0 / 16;
	texPoints[6].y = (16 - 1) * 1.0 / 16;
	texPoints[7].x = 0 * 1.0 / 16;
	texPoints[7].y = (16-3) * 1.0 / 16;

	glTranslatef(-8 * 0.5, 0, 0);
	DrawExtrudedPolygon(points, texPoints, 8, 5, 0, 0, 0);
	glTranslatef(8*0.5, 4 * 0.5, 0);
}

void CGLRenderer::DrawBody()
{
	POINTF points[5];
	POINTF texPoints[5];
	points[0].x = 0.5 * 0;
	points[0].y = 0.5 * 8;
	points[1].x = 0.5 * 4;
	points[1].y = 0.5 * 8;
	points[2].x = 0.5 * 8;
	points[2].y = 0.5 * 4;
	points[3].x = 0.5 * 8;
	points[3].y = 0.5 * 0;
	points[4].x = 0.5 * 0;
	points[4].y = 0.5 * 0;
	


	texPoints[0].x = 8 * 1.0 / 16;
	texPoints[0].y = 0 * 1.0 / 16;
	texPoints[1].x = 12 * 1.0 / 16;
	texPoints[1].y = 0 * 1.0 / 16;
	texPoints[2].x = 16 * 1.0 / 16;
	texPoints[2].y = 4* 1.0 / 16;
	texPoints[3].x = 16 * 1.0 / 16;
	texPoints[3].y = 8 * 1.0 / 16;
	texPoints[4].x = 8 * 1.0 / 16;
	texPoints[4].y = 8 * 1.0 / 16;
	
	glRotatef(angle1, 0, 1, 0);
	glTranslatef(-4 * 0.5, 0, 0);
	DrawExtrudedPolygon(points, texPoints, 5, 4, 0.96, 0.5, 0.12);
	glTranslatef(7 * 0.5, 1, 0);
	//DrawFork();
}

void CGLRenderer::DrawArm(double zh)
{
	POINTF points[6];
	POINTF texPoints[6];
	points[0].x = 0.5 * 1;
	points[0].y = 0.5 * 4;
	points[1].x = 0.5 * 16;
	points[1].y = 0.5 * 3;
	points[2].x = 0.5 * 16;
	points[2].y = 0.5 * 1;
	points[3].x = 0.5 * 1;
	points[3].y = 0.5 * 0;
	points[4].x = 0.5 * 0;
	points[4].y = 0.5 * 1;
	points[5].x = 0.5 * 0;
	points[5].y = 0.5 * 3;


	texPoints[0].x = 1 * 1.0 / 16;
	texPoints[0].y = (16 - 8) * 1.0 / 16;
	texPoints[1].x = 16 * 1.0 / 16;
	texPoints[1].y = (16 - 7) * 1.0 / 16;
	texPoints[2].x = 16 * 1.0 / 16;
	texPoints[2].y = (16 - 5) * 1.0 / 16;
	texPoints[3].x = 1 * 1.0 / 16;
	texPoints[3].y = (16-4) * 1.0 / 16;
	texPoints[4].x = 0 * 1.0 / 16;
	texPoints[4].y = (16 - 5) * 1.0 / 16;
	texPoints[5].x = 0 * 1.0 / 16;
	texPoints[5].y = (16 - 7) * 1.0 / 16;

	DrawExtrudedPolygon(points, texPoints, 6, zh, 0.96, 0.5, 0.12);
}


void CGLRenderer::DrawFork()
{
	POINTF points[6];
	POINTF texPoints[6];
	points[0].x = 0.5 * 1;
	points[0].y = 0.5 * 6;
	points[1].x = 0.5 * 7;
	points[1].y = 0.5 * 6;
	points[2].x = 0.5 * 8;
	points[2].y = 0.5 * 5;
	points[3].x = 0.5 * 8;
	points[3].y = 0.5 * 0;
	points[4].x = 0.5 * 0;
	points[4].y = 0.5 * 0;
	points[5].x = 0.5 * 0;
	points[5].y = 0.5 * 5;


	texPoints[0].x = 1 * 1.0 / 16;
	texPoints[0].y = 0 * 1.0 / 16;
	texPoints[1].x = 7 * 1.0 / 16;
	texPoints[1].y = 0* 1.0 / 16;
	texPoints[2].x = 8 * 1.0 / 16;
	texPoints[2].y = 1 * 1.0 / 16;
	texPoints[3].x = 8 * 1.0 / 16;
	texPoints[3].y = 6 * 1.0 / 16;
	texPoints[4].x = 0 * 1.0 / 16;
	texPoints[4].y = 6 * 1.0 / 16;
	texPoints[5].x = 0 * 1.0 / 16;
	texPoints[5].y = 1 * 1.0 / 16;

	DrawExtrudedPolygon(points, texPoints, 6, 1, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawExcavator()
{
	DrawBase();
	DrawBody();
	glRotatef(90 - angle2, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawArm(1);
	glTranslatef(6.5 + 1, 1, 0);
	glRotatef(-90 - angle3, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawArm(1.5);
	glTranslatef(6.5 + 1, 1, 0);
	glRotatef(-90 - angle4, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawFork();
	//DrawAxes();
}
