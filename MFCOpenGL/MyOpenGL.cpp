#include "pch.h"
#include "MyOpenGL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void MyOpenGL::drawEdge(Edge* e)
{
	//glColor3f(e->color[0], e->color[1], e->color[2]);
	Vertex* v1 = e->v1, * v2 = e->v2;
	int thetax1 = v1->thetax, thetay1 = v1->thetay;
	int thetax2 = v2->thetax, thetay2 = v2->thetay;
	if (thetax1 == 0 && (thetax2 == 360 - step)) {
		thetax1 = 360;
		roundFlag = true;
	}
	else if ((thetax1 == 360 - step) && thetax2 == 0) {
		thetax2 = 360;
		roundFlag = true;
	}
	if (!roundFlag) {
		if (thetax1 == 0) {
			thetax1 = thetax2;
		}
		else if (thetax2 == 0) {
			thetax2 = thetax1;
		}
	}
	else {
		if (thetax1 == 0 && thetax2 == 0) {
			thetax1 = thetax2 = 360;
			roundFlag = false;
		}
	}

	glNormal3f(v1->nx, v1->ny, v1->nz);
	glTexCoord2f(static_cast<float>(thetax1) / 360, static_cast<float>(thetay1) / 180);
	glVertex3f(v1->x, v1->y, v1->z);
	glNormal3f(v2->nx, v2->ny, v2->nz);
	glTexCoord2f(static_cast<float>(thetax2) / 360, static_cast<float>(thetay2) / 180);
	glVertex3f(v2->x, v2->y, v2->z);
}

void MyOpenGL::drawFace(Face* f)
{
	Edge* e1 = f->e1;
	Edge* e2 = f->e2;
	Edge* e3 = f->e3;
	//glBegin(GL_LINES);
	glBegin(GL_POLYGON);
	//glNormal3f(f->normal[0], f->normal[1], f->normal[2]);
	drawEdge(e1);
	drawEdge(e2);
	drawEdge(e3);
	glEnd();
}

double MyOpenGL::d2r(double digit)
{
	return digit / 180 * PI;
}

Vertex MyOpenGL::getVertex()
{
	double z = r * cos(d2r(90 - thetay)) + cz;
	double tmp = r * sin(d2r(90 - thetay));
	double x = tmp * cos(d2r(thetax)) + cx;
	double y = tmp * sin(d2r(thetax)) + cy;
	Vertex newVertex(x, y, z, 0, 0);
	return newVertex;
}

void MyOpenGL::loadTexture(const char* filePath, bool rgb)
{
	int width, height, channels;
	unsigned char* image;
	if (rgb) image = stbi_load(filePath, &width, &height, &channels, 0);
	else image = stbi_load(filePath, &width, &height, &channels, 1);

	if (!image) {
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

//void MyOpenGL::normalize(std::vector<double>& v)
//{
//	double nom = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
//	v[0] /= nom;
//	v[1] /= nom;
//	v[2] /= nom;
//}

MyOpenGL::MyOpenGL()
{
	ball.setCX(0.0);
	ball.setCY(0.0);
	ball.setCZ(0.0);
	ball.setR(5);
	ball.setStep(step);
	ball.draw();

	thetax = 90;
	thetay = 90;
	//r = 10;

	light = {
		{0.2, 0.2, 0.2, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{50.0},
		{ 0.0, 1.0, 2.0, 0.0 }
	};

	material = {
		{0.250000, 0.250000, 0.250000, 1.000000},
		{0.400000, 0.400000, 0.400000, 1.000000},
		{0.774597, 0.774597, 0.774597, 1.000000},
		{10.0},
		{0.0, 0.0, 0.0, 1.0}
	};
}

MyOpenGL::~MyOpenGL()
{
}

bool MyOpenGL::SetupPixelFormat(HDC hdc)
{
	hDC = hdc;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С
		1,                                // �汾��
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ
		PFD_SUPPORT_OPENGL |              // ֧��OpenGL
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ
		24,                               // 24 λ��ɫ���
		0, 0, 0, 0, 0, 0,                 // ������ɫλ
		0,                                // û�з�͸���Ȼ���
		0,                                // ������λλ
		0,                                // ���ۼӻ���
		0, 0, 0, 0,                       // �����ۼ�λ
		32,                               // 32 λ��Ȼ���   
		0,                                // ��ģ�建��
		0,                                // �޸�������
		PFD_MAIN_PLANE,                   // ����
		0,                                // ����
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ
	};

	int nPixelFormat;    // ���ص��ʽ
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"can not find proper mode", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	SetPixelFormat(hDC, nPixelFormat, &pfd);
	hRC = wglCreateContext(hDC);    //����GDI��ͼ��ʹ�õ�HDC������Ӧ��HGLRC
	wglMakeCurrent(hDC, hRC);        //ʹOpenGL��ͼ��ʹ�õ�HGLRCΪ��ǰ��ͼ����

	return TRUE;
}

void MyOpenGL::Init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	loadTexture(".\\texture\\8081_earthmap4k.jpg");
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vertex cam = getVertex();
	gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

void MyOpenGL::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2.5, 2.5, -2.5 / winAspect, 2.5 / winAspect, CNEAR, CFAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vertex cam = getVertex();
	gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//thetax = (thetax - move * 2) % 360;

	// ��������
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);

	// ��Դ����
	glLightfv(GL_LIGHT0, GL_POSITION, light.lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);   //�����������
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);  //���淴���
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light.ambient);  //���������
	glEnable(GL_LIGHT0);      //����0#��ʹ��

	glEnable(GL_LIGHTING);   //����
	glEnable(GL_DEPTH_TEST); //����Ȳ���

	glTranslatef(cx, cy, cz);
	theta = (theta + move * 2) % 360;
	light.lightPos[0] = cx + sin(d2r(theta)) * r;
	light.lightPos[1] = cy + cos(d2r(theta)) * r;

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//std::vector<Vertex*> vertexes = ball.getVertexes();
	std::vector<Face*> faces = ball.getFaces();
	for (auto f: faces) {
		drawFace(f);
	}
	glDisable(GL_TEXTURE_2D);

	//glFlush();
	SwapBuffers(hDC);
}

void MyOpenGL::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	winAspect = (double)width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2.5, 2.5, -2.5 / winAspect, 2.5 / winAspect, CNEAR, CFAR);

}

void MyOpenGL::swapMove()
{
	move ^= 1;
}

void MyOpenGL::zoom(bool flag)
{
	if (flag) r -= 0.1;
	else r += 0.1;
}
