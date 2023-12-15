#include "pch.h"
#include "MyBall.h"

double MyBall::d2r(double digit)
{
	return digit / 180 * PI;
}

Vertex* MyBall::getVertex(int thetax, int thetay)
{
	double z = cz + r * cos(d2r(thetay));
	double tmp = r * sin(d2r(thetay));
	double x = cx + tmp * cos(d2r(thetax));
	double y = cy + tmp * sin(d2r(thetax));
	Vertex* newVertex = new Vertex(x, y, z);
	this->vertexes.push_back(newVertex);
	return newVertex;
}

double MyBall::dot(const std::vector<double>& v1, const std::vector<double>& v2) const
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void MyBall::cross(const Edge& e1, const Edge& e2, std::vector<double>& res) const
{
	std::vector<double> v1 = {e1.v2->x - e1.v1->x, e1.v2->y - e1.v1->y, e1.v2->z - e1.v1->z};
	std::vector<double> v2 = {e2.v2->x - e2.v1->x, e2.v2->y - e2.v1->y, e2.v2->z - e2.v1->z};
	while (res.size() < 3) res.push_back(0);
	res[0] = v1[1] * v2[2] - v1[2] * v2[1];
	res[1] = v1[0] * v2[2] - v1[2] * v2[0];
	res[2] = v1[0] * v2[1] - v1[1] * v2[0];
	std::vector<double> fromCenter = {e2.v1->x - cx, e2.v1->y - cy, e2.v1->z - cz};
	double tmp = sqrt(res[0] * res[0] + res[1] * res[1] + res[2] * res[2]);
	res[0] /= tmp;
	res[1] /= tmp;
	res[2] /= tmp;
	if (dot(fromCenter, res) < 0) {
		res[0] *= -1;
		res[1] *= -1;
		res[2] *= -1;
	}
}

MyBall::MyBall(double cx, double cy, double cz, double r)
{
	this->cx = cx;
	this->cy = cy;
	this->cz = cz;
	this->r = r;

}

MyBall::~MyBall()
{
	for (auto vertex : vertexes) delete vertex;
	for (auto edge : edgeHead) {
		while (edge != NULL) {
			Edge* tmp = edge;
			edge = edge->next;
			delete tmp;
		}
	}
	for (auto edge : midEdges) delete edge;
	for (auto face : faces) delete face;
}

void MyBall::setCX(double cx)
{
	this->cx = cx;
}

void MyBall::setCY(double cy)
{
	this->cy = cy;
}

void MyBall::setCZ(double cz)
{
	this->cz = cz;
}

void MyBall::setR(double r)
{
	this->r = r;
}

void MyBall::draw()
{
	int thetax, thetay;
	thetax = thetay = 0;

	Vertex* top = new Vertex(cx, cy, cz + r);
	Vertex* bottom = new Vertex(cx, cy, cz - r);

	this->vertexes.push_back(top);
	this->vertexes.push_back(bottom);

	Edge* now = new Edge(top, NULL, NULL, NULL);
	edgeHead.push_back(now);
	for (thetay = step; thetay < 180; thetay += step) {
		now->color[0] = 1.0;
		now->color[1] = 0.0;
		now->color[2] = 0.0;
		now->v2 = getVertex(thetax, thetay);
		Edge* next = new Edge(now->v2, NULL, NULL, NULL);
		now->next = next;
		now = now->next;
	}
	now->v2 = bottom;
	bool flag = 1;

	for (thetax = step; thetax < 360; thetax += step) {
		Edge* leftHead = edgeHead.back();
		Edge* rightHead = new Edge(top, NULL, NULL, NULL);
		edgeHead.push_back(rightHead);
		for (thetay = step; thetay < 180; thetay += step) {
			rightHead->v2 = getVertex(thetax, thetay);
			Edge* next = new Edge(rightHead->v2, NULL, NULL, NULL);
			rightHead->next = next;

			if (leftHead->v1 == rightHead->v1) {
				// top
				Edge* bottomEdge = new Edge(leftHead->v2, rightHead->v2, NULL, NULL);
				midEdges.push_back(bottomEdge);
				Face* f = new Face(leftHead, bottomEdge, rightHead);
				//cross(*f->e1, *f->e2, f->normal);
				leftHead->right = bottomEdge->left = rightHead->left = f;
				faces.push_back(f);
			}
			else {
				Edge* topEdge = faces.back()->e2;
				Edge* bottomEdge = new Edge(leftHead->v2, rightHead->v2, NULL, NULL);
				Edge* split;
				Face* upFace, * downFace;
				if (flag) {
					split = new Edge(leftHead->v1, rightHead->v2, NULL, NULL);
					upFace = new Face(topEdge, split, rightHead);
					downFace = new Face(leftHead, bottomEdge, split);
					leftHead->right = bottomEdge->left = split->left = downFace;
					rightHead->left = topEdge->right = split->right = upFace;
				}
				else {
					split = new Edge(leftHead->v2, rightHead->v1, NULL, NULL);
					upFace = new Face(topEdge, leftHead, split);
					downFace = new Face(split, bottomEdge, rightHead);
					leftHead->right = split->left = topEdge->right = upFace;
					split->right = bottomEdge->left = rightHead->left = downFace;
				}
				//cross(*upFace->e1, *upFace->e2, upFace->normal);
				//cross(*downFace->e1, *downFace->e2, downFace->normal);
				faces.push_back(upFace);
				faces.push_back(downFace);
				midEdges.push_back(bottomEdge);
				midEdges.push_back(split);
			}

			rightHead = rightHead->next;
			leftHead = leftHead->next;
			flag ^= 1;
		}
		rightHead->v2 = bottom;
		Edge* topEdge = faces.back()->e2;
		Face* lastFace = new Face(leftHead, rightHead, topEdge);
		//cross(*lastFace->e1, *lastFace->e2, lastFace->normal);
		leftHead->right = rightHead->left = topEdge->right = lastFace;
		faces.push_back(lastFace);
	}

	Edge* leftHead = edgeHead.back();
	Edge* rightHead = edgeHead[0];
	while (leftHead != NULL) {
		if (leftHead->v1 == rightHead->v1) {
			Edge* bottomEdge = new Edge(leftHead->v2, rightHead->v2, NULL, NULL);
			midEdges.push_back(bottomEdge);
			Face* f = new Face(leftHead, bottomEdge, rightHead);
			//cross(*f->e1, *f->e2, f->normal);
			leftHead->right = bottomEdge->left = rightHead->left = f;
			faces.push_back(f);
		}
		else if (leftHead->v2 == rightHead->v2) {
			Edge* topEdge = faces.back()->e2;
			Face* f = new Face(leftHead, rightHead, topEdge);
			//cross(*f->e1, *f->e2, f->normal);
			leftHead->right = rightHead->left = topEdge->right = f;
			faces.push_back(f);
		}
		else {
			Edge* topEdge = faces.back()->e2;
			Edge* bottomEdge = new Edge(leftHead->v2, rightHead->v2, NULL, NULL);
			Edge* split;
			Face* upFace, * downFace;
			if (flag) {
				split = new Edge(leftHead->v1, rightHead->v2, NULL, NULL);
				upFace = new Face(topEdge, split, rightHead);
				downFace = new Face(leftHead, bottomEdge, split);
				leftHead->right = bottomEdge->left = split->left = downFace;
				rightHead->left = topEdge->right = split->right = upFace;
			}
			else {
				split = new Edge(leftHead->v2, rightHead->v1, NULL, NULL);
				upFace = new Face(topEdge, leftHead, split);
				downFace = new Face(split, bottomEdge, rightHead);
				leftHead->right = split->left = topEdge->right = upFace;
				split->right = bottomEdge->left = rightHead->left = downFace;
			}
			//cross(*upFace->e1, *upFace->e2, upFace->normal);
			//cross(*downFace->e1, *downFace->e2, downFace->normal);
			faces.push_back(upFace);
			faces.push_back(downFace);
			midEdges.push_back(bottomEdge);
			midEdges.push_back(split);
		}
		leftHead = leftHead->next;
		rightHead = rightHead->next;
		flag ^= 1;
	}
}

std::vector<Face*> MyBall::getFaces()
{
	return faces;
}

std::vector<Vertex*> MyBall::getVertexes()
{
	return vertexes;
}

Vertex::Vertex(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	double norm = sqrt(x * x + y * y + z * z);
	this->nx = x / norm;
	this->ny = y / norm;
	this->nz = z / norm;
}

bool Vertex::operator==(const Vertex& v)
{
	return x == v.x && y == v.y && z == v.z;
}

Edge::Edge()
{
	this->v1 = this->v2 = NULL;
	this->left = this->right = NULL;
	this->next = NULL;
}

Edge::Edge(Vertex* v1, Vertex* v2, Face* left, Face* right)
{
	this->v1 = v1;
	this->v2 = v2;
	this->left = left;
	this->right = right;
	this->next = NULL;
}

Face::Face()
{
	this->e1 = this->e2 = this->e3 = NULL;
	//this->normal = {0, 0, 0};
}

Face::Face(Edge* e1, Edge* e2, Edge* e3)
{
	this->e1 = e1;
	this->e2 = e2;
	this->e3 = e3;
}
