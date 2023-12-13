#pragma once

#include <GL/gl.h>
#include <GL/glut.h>
#include "MyBall.h"

struct Light {
	GLfloat specular[4]; // ���淴�����
	GLfloat diffuse[4]; //���������
	GLfloat shininess[1]; // �߹�
	GLfloat Light_Model_Ambient[4]; //���������
	GLfloat lightPos[4]; //��Դλ��
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

	Light light;

	int thetax, thetay;
	double r;
	bool move = 1;

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

