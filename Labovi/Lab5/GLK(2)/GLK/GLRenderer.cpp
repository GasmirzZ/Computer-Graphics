#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

const double PI = 3.14159265358979323846;
const double TO_RAD = PI / 180.0;

CGLRenderer::CGLRenderer(void)
{
	light1 = light2 = light3 = true;
	normals = false;

	radius1 = 3.0f;
	radius2 = 4.0f;

	m_cameraR = 28.0;
	m_cameraAngleXY = 70.0;
	m_cameraAngleXZ = 0.0;
	UpdateCameraPosition();

	// VAZA BOJA 1 - Magenta (RGB 1.00 0.00 1.00) - sad je crvena
	light_blue.SetAmbient(0.2, 0.0, 0.0, 0.0);
	light_blue.SetDiffuse(1.0, 0.0, 0.0, 0.0);
	light_blue.SetSpecular(0.5, 0.5, 0.5, 0.0);
	light_blue.SetShininess(25.0);

	// VAZA BOJA 2 - Cyan (RGB 0.00 1.00 1.00) - sad je zuta
	dark_blue.SetAmbient(0.2, 0.2, 0.0, 0.0);
	dark_blue.SetDiffuse(1.0, 1.0, 0.0, 0.0);
	dark_blue.SetSpecular(0.5, 0.5, 0.5, 0.0);
	dark_blue.SetShininess(16.0);

	// Material for stand
	matBase.SetAmbient(0.15, 0.15, 0.15, 0.0);
	matBase.SetDiffuse(0.7, 0.7, 0.7, 0.0);
	matBase.SetSpecular(0.5, 0.9, 0.5, 0.0);
	matBase.SetShininess(64.0);

	// Material for room
	roomBase.SetAmbient(0.1, 0.1, 0.1, 1.0);
	roomBase.SetDiffuse(0.8, 0.8, 0.8, 1.0);
	roomBase.SetSpecular(0.2, 0.2, 0.2, 1.0);
	roomBase.SetEmission(0.1, 0.1, 0.1, 1.0);
	roomBase.SetShininess(0.0);

	// SVETLO 1 - Magenta (RGB 1.00 0.00 1.00)
	yellowSphere.SetAmbient(0.2, 0.0, 0.2, 1.0);
	yellowSphere.SetDiffuse(1.0, 0.0, 1.0, 0.0);
	yellowSphere.SetSpecular(1.0, 0.0, 1.0, 1.0);
	yellowSphere.SetEmission(1.0, 0.0, 1.0, 1.0);
	yellowSphere.SetShininess(0.0);

	// SVETLO 2 - Red (RGB 1.00 0.00 0.00)
	redSphere.SetAmbient(0.2, 0.0, 0.0, 1.0);
	redSphere.SetDiffuse(1.0, 0.0, 0.0, 0.0);
	redSphere.SetSpecular(1.0, 0.0, 0.0, 1.0);
	redSphere.SetEmission(1.0, 0.0, 0.0, 1.0);
	redSphere.SetShininess(0.0);

	// SVETLO 3 - Blue (RGB 0.00 0.00 1.00)
	blueSphere.SetAmbient(0.0, 0.0, 0.2, 1.0);
	blueSphere.SetDiffuse(0.0, 0.0, 1.0, 0.0);
	blueSphere.SetSpecular(0.0, 0.0, 1.0, 1.0);
	blueSphere.SetEmission(0.0, 0.0, 1.0, 1.0);
	blueSphere.SetShininess(0.0);
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
	glClearColor(0.0, 0.0, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	AmbientLight();
	glEnable(GL_LIGHTING);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 10.0, 0.0,   // centar scene
		0.0, 1.0, 0.0
	);

	// Setup lighting
	AmbientLight();
	ViewPointLight();

	// Draw coordinate axes
	glDisable(GL_LIGHTING);
	DrawAxis();
	glEnable(GL_LIGHTING);

	// Setup individual lights
	Light1();
	Light2();
	Light3();

	glPushMatrix();
	{
		// Draw room
		roomBase.SetFace();
		DrawRoom();

		// Draw stand and vase
		glScalef(1.5, 1.5, 1.5);
		matBase.SetFace();
		DrawStand();
		DrawVase();
	}
	glPopMatrix();

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
	gluPerspective(65, (double)w/(double)h, 1, 100);

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

// ============================================================================
// LIGHTING FUNCTIONS
// ============================================================================

void CGLRenderer::AmbientLight()
{
	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void CGLRenderer::ViewPointLight()
{
	GLfloat mat[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_position[] = { 13, 39, 13, 0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, mat);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	glEnable(GL_LIGHT0);
}

void CGLRenderer::Light1()
{
	// SVETLO 1 - Magenta (RGB 1.00 0.00 1.00) - 
	GLfloat light_ambient[] = { 0.25, 0.0, 0.25, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 0.0, 1.0, 1.0 };
	/*GLfloat light_position[] = { 20.0, 15.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { -1.0, 0.0, 0.0 };*/
	GLfloat light_position[] = { 0.0, 15.0, -20.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);

	if (light1)
	{
		glPushMatrix();
		{
			yellowSphere.SetFace();
			glTranslatef(0, 15, -18);
			DrawSphere(1.0);
		}
		glPopMatrix();
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}
}

void CGLRenderer::Light2()
{
	// SVETLO 2 - Red (RGB 1.00 0.00 0.00) 
	GLfloat light_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 0.75, 0.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.5, 1.0 };
	GLfloat light_position[] = { 0.0, 15.0, 20.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 }; 
	/*GLfloat light_position[] = { -20.0, 15.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 1.0, 0.0, 0.0 };*/
	/*GLfloat light_position[] = { 0.0, 40.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };*/

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);

	if (light2)
	{
	/*	glPushMatrix();
		{
			blueSphere.SetFace();
			glTranslatef(0, 38, 0);
			DrawSphere(1.5);
		}*/
		glPushMatrix();
		{
			redSphere.SetFace();
			glTranslatef(0.0, 15.0, 18.0);
			DrawSphere(1.5);
		}
		glPopMatrix();
		glEnable(GL_LIGHT2);
	}
	else
	{
		glDisable(GL_LIGHT2);
	}
}

void CGLRenderer::Light3()
{
	// SVETLO 3 - Blue (RGB 0.00 0.00 1.00) 
	GLfloat light_ambient[] = { 0.0, 0.0, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 40.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 }; 
	/*GLfloat light_position[] = { -20.0, 15.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 1.0, 0.0, 0.0 };*/

	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 20.0);

	if (light3)
	{
		glPushMatrix();
		{
			blueSphere.SetFace();
			glTranslatef(0, 38, 0);
			DrawSphere(1.5);
		}
		glPopMatrix();
		glEnable(GL_LIGHT3);
	}
	else
	{
		glDisable(GL_LIGHT3);
	}
}

// ============================================================================
// SCENE DRAWING FUNCTIONS
// ============================================================================

void CGLRenderer::DrawAxis()
{
	float size = 30.0;
	glLineWidth(1.0);

	glBegin(GL_LINES);
	{
		// Red X axis
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(size, 0.0, 0.0);

		// Green Y axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, size, 0.0);

		// Blue Z axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, size);
	}
	glEnd();
}

void CGLRenderer::DrawRoom()
{
	float a = 60.0;
	float step = 1.0;

	// Bottom (only if camera angle allows)
	if (m_cameraAngleXZ < 35.0)
	{
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		for (float i = -a / 2; i < a / 2; i += step)
		{
			for (float j = -a / 2; j < a / 2; j += step)
			{
				glVertex3f(i, 0, j);
				glVertex3f(i, 0, j + step);
				glVertex3f(i + step, 0, j + step);
				glVertex3f(i + step, 0, j);
			}
		}
		glEnd();
	}

	// Top
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	for (float i = -a / 2; i < a / 2; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(i, a, j);
			glVertex3f(i, a, j + step);
			glVertex3f(i + step, a, j + step);
			glVertex3f(i + step, a, j);
		}
	}
	glEnd();

	// Front wall
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(a / 2, i, j);
			glVertex3f(a / 2, i, j + step);
			glVertex3f(a / 2, i + step, j + step);
			glVertex3f(a / 2, i + step, j);
		}
	}
	glEnd();

	// Back wall
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(-a / 2, i, j);
			glVertex3f(-a / 2, i, j + step);
			glVertex3f(-a / 2, i + step, j + step);
			glVertex3f(-a / 2, i + step, j);
		}
	}
	glEnd();

	// Left wall
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, -a / 2);
			glVertex3f(j + step, i, -a / 2);
			glVertex3f(j + step, i + step, -a / 2);
			glVertex3f(j, i + step, -a / 2);
		}
	}
	glEnd();

	// Right wall
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, a / 2);
			glVertex3f(j + step, i, a / 2);
			glVertex3f(j + step, i + step, a / 2);
			glVertex3f(j, i + step, a / 2);
		}
	}
	glEnd();
}

void CGLRenderer::DrawStand()
{
	double plane[] = { 0, 1, 0, 0 };
	glClipPlane(GL_CLIP_PLANE0, plane);
	glEnable(GL_CLIP_PLANE0);

	glColor3f(0.8, 0.8, 0.8);
	// Bottom sphere (half)
	DrawSphere(2.5);

	// Middle cylinder
	glTranslatef(0, 3, 0);
	DrawRoller(1, 2);

	// Top prism
	glTranslatef(0, 1.5, 0);
	DrawPrism(4, 1);

	glDisable(GL_CLIP_PLANE0);
}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	{
		glTranslatef(0.0, 0.25, 0.0);
		glScalef(0.5, 0.5, 0.5);

		float step = 1.0;
		radius1 = 3.0;
		radius2 = 4.0;
		height = 1;
		heightPrev = 0;
		bool color = true;

		//3 - povecava obim prstena i gornji i donji r
		//-3 - smanjuje obim prstena
		//-2 - smanjuje broj stepova, tj pravi krug ili "kupu" skuplja donji deo
		//2 - povecava broj stepova, tj pravi krug ili "kupu", siri donji deo
		//-1 - zamenjuje radiuse, tj obrce smer u kome idu "kupe"
		//1 - povecava radius 1

		// Pattern for vase shape
		int pattern[14] = { 0, -3,-2,-1,1,3,-1,-3,-1, -1,-1, 3 ,-1, -3 };

		for (int i = 0; i < 14; i++)
		{
			// Apply pattern
			switch (pattern[i])
			{
			case 4:
				height += 0.5;
				IncRing(step);
				break;
			case -4:
				height -= 0.5;
				DecRing(step);
				break;
			case 3:
				IncRing(step);
				break;
			case -3:
				DecRing(step);
				break;
			case 2:
				radius2 += step;
				break;
			case -2:
				radius2 -= step;
				break;
			case 1:
				radius1 += step;
				break;
			case -1:
			{
				float temp = radius1;
				radius1 = radius2;
				radius2 = temp;
			}
			break;
			default:
				break;
			}

			// Alternate colors
			if (color)
				dark_blue.SetFace();
			else
				light_blue.SetFace();
			color = !color;

			glTranslatef(0.0, heightPrev, 0.0);
			DrawStrip(radius2, radius1, height, 50);
			heightPrev = height;
		}
	}
	glPopMatrix();
}

// ============================================================================
// PRIMITIVE DRAWING FUNCTIONS
// ============================================================================

void CGLRenderer::DrawSphere(float radius)
{
	int step = 10;

	glBegin(GL_QUAD_STRIP);
	for (int alpha = -90; alpha < 90; alpha += step)
	{
		for (int beta = 0; beta <= 360; beta++)
		{
			float normX = cos(beta * TO_RAD) * cos(alpha * TO_RAD);
			float normY = sin(alpha * TO_RAD);
			float normZ = sin(beta * TO_RAD) * cos(alpha * TO_RAD);

			glNormal3f(normX, normY, normZ);
			glVertex3f(radius * normX, radius * normY, radius * normZ);

			float normX1 = cos(beta * TO_RAD) * cos((alpha + step) * TO_RAD);
			float normY1 = sin((alpha + step) * TO_RAD);
			float normZ1 = sin(beta * TO_RAD) * cos((alpha + step) * TO_RAD);

			glNormal3f(normX1, normY1, normZ1);
			glVertex3f(radius * normX1, radius * normY1, radius * normZ1);
		}
	}
	glEnd();
}

void CGLRenderer::DrawRoller(float radius, float height, int sides)
{
	float step = (2.0 * PI) / sides;
	float halfHeight = height / 2.0;

	// Top base
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	for (int i = 0; i < sides; i++)
	{
		glVertex3f(GetX(radius, i * step), halfHeight, GetY(radius, i * step));
	}
	glEnd();

	// Bottom base
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, -1.0, 0.0);
	for (int i = 0; i < sides; i++)
	{
		glVertex3f(GetX(radius, i * step), -halfHeight, GetY(radius, i * step));
	}
	glEnd();

	// Wrap
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++)
	{
		float angle = i * step;
		glNormal3f(GetX(1, angle), 0, GetY(1, angle));
		glVertex3f(GetX(radius, angle), halfHeight, GetY(radius, angle));
		glNormal3f(GetX(1, angle), 0, GetY(1, angle));
		glVertex3f(GetX(radius, angle), -halfHeight, GetY(radius, angle));
	}
	glEnd();
}

void CGLRenderer::DrawPrism(float radius, float height, int sides)
{
	float halfHeight = height / 2.0;
	float step = (2.0 * PI) / sides;

	glPushMatrix();
	glRotatef(45, 0, 1, 0);

	// Top base
	glBegin(GL_TRIANGLE_FAN);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, 1, 0);
		glVertex3f(GetX(radius, i * step), halfHeight, GetY(radius, i * step));
	}
	glEnd();

	// Bottom base
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++)
	{
		glNormal3f(0, -1, 0);
		glVertex3f(GetX(radius, i * step), -halfHeight, GetY(radius, i * step));
	}
	glEnd();

	// Wrap
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++)
	{
		float angle = i * step;
		glNormal3f(GetX(1, angle), 0, GetY(1, angle));
		glVertex3f(GetX(radius, angle), halfHeight, GetY(radius, angle));
		glNormal3f(GetX(1, angle), 0, GetY(1, angle));
		glVertex3f(GetX(radius, angle), -halfHeight, GetY(radius, angle));
	}
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawStrip(double r1, double r2, double h, int nSeg)
{
	double step = 2.0 * PI / nSeg;

	// Calculate normal components
	int v = r2 > r1 ? -1 : 1;
	double r = fabs(r1 - r2);
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	// Draw strip
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++)
	{
		double angle = step * i;
		glNormal3f(nr * cos(angle), ny * v, nr * sin(angle));
		glVertex3f(cos(angle) * r1, 0.0, sin(angle) * r1);
		glVertex3f(cos(angle) * r2, h, sin(angle) * r2);
	}
	glEnd();

	// Draw normals if enabled
	if (normals)
	{
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(0.0, 1.0, 0.0);

		for (int i = 0; i <= nSeg; i++)
		{
			double angle = step * i;

			// Bottom point normal
			glVertex3f(cos(angle) * r1, 0.0, sin(angle) * r1);
			glVertex3f(cos(angle) * r1 + cos(angle) * nr, ny * v,
				sin(angle) * r1 + sin(angle) * nr);

			// Top point normal
			glVertex3f(cos(angle) * r2, h, sin(angle) * r2);
			glVertex3f(cos(angle) * r2 + cos(angle) * nr, h + ny * v,
				sin(angle) * r2 + sin(angle) * nr);
		}

		glEnd();
		glEnable(GL_LIGHTING);
	}
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

float CGLRenderer::GetX(float radius, float angle)
{
	return radius * cosf(angle);
}

float CGLRenderer::GetY(float radius, float angle)
{
	return radius * sinf(angle);
}

void CGLRenderer::IncRing(float step)
{
	radius1 += step;
	radius2 += step;
}

void CGLRenderer::DecRing(float step)
{
	radius1 -= step;
	radius2 -= step;
}

void CGLRenderer::SwitchLight1() { light1 = !light1; }
void CGLRenderer::SwitchLight2() { light2 = !light2; }
void CGLRenderer::SwitchLight3() { light3 = !light3; }
void CGLRenderer::SwitchNormals() { normals = !normals; }

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	m_cameraAngleXY += dXY;
	m_cameraAngleXZ += dXZ;

	if (m_cameraAngleXZ > 89) m_cameraAngleXZ = 89;
	if (m_cameraAngleXZ < -89) m_cameraAngleXZ = -89;

	UpdateCameraPosition();
}

void CGLRenderer::UpdateCameraPosition()
{
	double radXY = TO_RAD * m_cameraAngleXY;
	double radXZ = TO_RAD * m_cameraAngleXZ;

	m_eyex = m_cameraR * cos(radXZ) * cos(radXY);
	m_eyey = m_cameraR * sin(radXZ);
	m_eyez = m_cameraR * cos(radXZ) * sin(radXY);
}