#pragma once

#include <GL/gl.h>
#include <GL/glut.h>
#include "MyBall.h"

struct Light {
	GLfloat ambient[4]; //���������
	GLfloat diffuse[4]; //���������
	GLfloat specular[4]; // ���淴�����
	GLfloat shininess[1]; // �߹�
	GLfloat lightPos[4]; //��Դλ��
};

struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
	GLfloat emission[4];
};

class MyOpenGL
{
private:
	HDC hDC;			//GDI��ͼ��ʹ�õĵ��豸�������
	HGLRC hRC;		//OpenGL��Ⱦʱʹ�õ���Ⱦ�������
	//	GLint winWidth, winHeight;
	double winAspect;
	MyBall ball;

	void drawEdge(Edge *e);
	void drawFace(Face *f);
	double d2r(double digit);
	Vertex getVertex();
	//void normalize(std::vector<double> &v);

	Light light;
	Material material;

	double cx = 1.0, cy = 1.0, cz = -2.0;
	int thetax, thetay;
	double r = 10;
	bool move = 1;
	int theta = 0;
	const double CNEAR = 1.0, CFAR = 11.0;

public:
	MyOpenGL();
	virtual ~MyOpenGL();

	bool SetupPixelFormat(HDC hdc);	//�������ظ�ʽΪ�ʺ�OpenGL�ĸ�ʽ
	void Init(void);									//��ʼ����Ⱦ���������Ե�����
	void Render(void);							//���ƴ���

	void Reshape(int width, int height);		//�ı䴰�ڴ�Сʱ���Ӵ����еĲ���
	void setEye(double dx, double dy);
	void swapMove();
	void zoom(bool flag);
};

