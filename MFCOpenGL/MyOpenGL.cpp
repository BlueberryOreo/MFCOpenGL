#include "pch.h"
#include "MyOpenGL.h"

void MyOpenGL::drawEdge(Edge* e)
{
	glColor3f(e->color[0], e->color[1], e->color[2]);
	glNormal3f(e->v1->nx, e->v1->ny, e->v1->nz);
	glVertex3f(e->v1->x, e->v1->y, e->v1->z);
	glNormal3f(e->v2->nx, e->v2->ny, e->v2->nz);
	glVertex3f(e->v2->x, e->v2->y, e->v2->z);
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
	Vertex newVertex(x, y, z);
	return newVertex;
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
	ball.draw();

	thetax = 90;
	thetay = 90;
	//r = 10;

	light = {
		{0.2, 0.2, 0.2, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{100.0},
		{ 0.0, 0.0, 5.0, 0.0 }
	};

	material = {
		{0.250000, 0.250000, 0.250000, 1.000000},
		{0.400000, 0.400000, 0.400000, 1.000000},
		{0.774597, 0.774597, 0.774597, 1.000000},
		{76.800003},
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
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小
		1,                                // 版本号
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图
		PFD_SUPPORT_OPENGL |              // 支持OpenGL
		PFD_DOUBLEBUFFER,                 // 双缓存模式
		PFD_TYPE_RGBA,                    // RGBA 颜色模式
		24,                               // 24 位颜色深度
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位
		0,                                // 没有非透明度缓存
		0,                                // 忽略移位位
		0,                                // 无累加缓存
		0, 0, 0, 0,                       // 忽略累加位
		32,                               // 32 位深度缓存   
		0,                                // 无模板缓存
		0,                                // 无辅助缓存
		PFD_MAIN_PLANE,                   // 主层
		0,                                // 保留
		0, 0, 0                           // 忽略层,可见性和损毁掩模
	};

	int nPixelFormat;    // 像素点格式
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"can not find proper mode", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	SetPixelFormat(hDC, nPixelFormat, &pfd);
	hRC = wglCreateContext(hDC);    //利用GDI绘图所使用的HDC创建对应的HGLRC
	wglMakeCurrent(hDC, hRC);        //使OpenGL绘图所使用的HGLRC为当前绘图工具

	return TRUE;
}

void MyOpenGL::Init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	glFrustum(-2.5, 2.5, -2.5 / winAspect, 2.5 / winAspect, CNEAR, CFAR);					//1：一般透视投影 +3.0~-15.0范围内可见

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vertex cam = getVertex();
	gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//thetax = (thetax + move * 2) % 360;

	// 材质设置
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);

	// 光源设置
	glLightfv(GL_LIGHT0, GL_POSITION, light.lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);   //漫反射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);  //镜面反射光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light.ambient);  //环境光参数
	glEnable(GL_LIGHT0);      //允许0#灯使用

	glEnable(GL_LIGHTING);   //开灯
	glEnable(GL_DEPTH_TEST); //打开深度测试

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glTranslatef(cx, cy, cz);
	theta = (theta + move * 2) % 360;
	light.lightPos[0] = cx + sin(d2r(theta)) * r;
	light.lightPos[1] = cy + cos(d2r(theta)) * r;

	glColor3f(1.0, 1.0, 1.0);
	//std::vector<Vertex*> vertexes = ball.getVertexes();
	std::vector<Face*> faces = ball.getFaces();
	for (auto f: faces) {
		drawFace(f);
	}

	glFlush();
	SwapBuffers(hDC);
}

void MyOpenGL::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	winAspect = (double)width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2.5, 2.5, -2.5 / winAspect, 2.5 / winAspect, CNEAR, CFAR);					//1：一般透视投影 +3.0~-15.0范围内可见
	//	gluPerspective(90.0, winAspect, 2.0, 20.0);			//2：对称透视投影 +3.0~-15.0范围内可见
		//glOrtho(-2.5,2.5, -2.5 / winAspect, 2.5 / winAspect, CNEAR, CFAR);			//3：正投影  +3.0~-15.0范围内可见

}

void MyOpenGL::setEye(double dx, double dy)
{

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
