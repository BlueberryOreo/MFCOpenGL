#pragma once

#include <GL/gl.h>
#include <GL/glut.h>
#include "MyBall.h"

struct Light {
	GLfloat specular[4]; // 镜面反射参数
	GLfloat diffuse[4]; //漫反射参数
	GLfloat shininess[1]; // 高光
	GLfloat Light_Model_Ambient[4]; //环境光参数
	GLfloat lightPos[4]; //光源位置
};

class MyOpenGL
{
private:
	HDC hDC;			//GDI绘图中使用的的设备环境句柄
	HGLRC hRC;		//OpenGL渲染时使用的渲染环境句柄
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

	bool SetupPixelFormat(HDC hdc);	//设置像素格式为适合OpenGL的格式
	void Init(void);									//初始化渲染过程中属性的设置
	void Render(void);							//绘制代码

	void Reshape(int width, int height);		//改变窗口大小时对视窗进行的操作
	void setEye(double dx, double dy);
	void swapMove();
	void zoom(bool flag);
};

