#pragma once
#include <vector>
#include <cmath>
#define PI 3.141592653589793

class Vertex;
class Edge;
class Face;

class Vertex {
public:
	double x, y, z;
	double nx, ny, nz;
	Vertex(double x, double y, double z);
	bool operator==(const Vertex &v);
};

class Edge {
public:
	Vertex* v1, *v2;
	Face* left, *right;
	Edge* next;
	double color[3] = {1.0, 1.0, 1.0};
	Edge();
	Edge(Vertex *v1, Vertex *v2, Face *left, Face *right);
};

class Face {
public:
	Edge* e1, *e2, *e3;
	//std::vector<double> normal;
	Face();
	Face(Edge *e1, Edge *e2, Edge *e3);
};

class MyBall
{
private:
	std::vector<Vertex*> vertexes;
	std::vector<Edge*> edgeHead;
	std::vector<Edge*> midEdges;
	std::vector<Face*> faces;

	double cx, cy, cz, r;

	double d2r(double digit);
	Vertex* getVertex(int thetax, int thetay);
	double dot(const std::vector<double>& v1, const std::vector<double> &v2) const;
	void cross(const Edge &e1, const Edge &e2, std::vector<double> &res) const;
	int step = 1;

public:
	MyBall(double cx=0, double cy=0, double cz=0, double r=1);
	~MyBall();

	void setCX(double cx);
	void setCY(double cy);
	void setCZ(double cz);
	void setR(double r);
	void draw();
	std::vector<Face*> getFaces();
	std::vector<Vertex*> getVertexes();
};

