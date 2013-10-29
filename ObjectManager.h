#ifndef OBJECT_MANAGER
#define	OBJECT_MANAGER

#include <stdio.h>
#include "math_3d.h"

struct Vertex{
	float x,y,z;
	float u,v;
	
	Vertex(){
	};

	Vertex(float _x, float _y, float _z){
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3f getVector3f(){
		return Vector3f(x,y,z);
	}
};

struct Triangle{
	Vertex  vertex[3];

	Triangle * nextTriangle;

	Triangle(){
		vertex[0] = Vertex(0.0f, 0.0f, 0.0f);
		vertex[1] = Vertex(0.0f, 0.0f, 0.0f);
		vertex[2] = Vertex(0.0f, 0.0f, 0.0f);
		nextTriangle = NULL;
	}
	Triangle(float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2,float c_x3, float c_y3, float c_z3){
		vertex[0] = Vertex(c_x1, c_y1, c_z1);
		vertex[1] = Vertex(c_x2, c_y2, c_z2);
		vertex[2] = Vertex(c_x3, c_y3, c_z3);
		nextTriangle = NULL;	
	}
};

struct Line{
	Vertex  vertex[2];

	Line * nextLine;

	Line(){
		vertex[0] = Vertex(0.0f, 0.0f, 0.0f);
		vertex[1] = Vertex(1.0f, 0.0f, 0.0f);
		nextLine = NULL;
	}
	Line(float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2){
		vertex[0] = Vertex(c_x1, c_y1, c_z1);
		vertex[1] = Vertex(c_x2, c_y2, c_z2);
		nextLine = NULL;	
	}
};

struct Point{
	Vertex  vertex[1];

	Point * nextPoint;

	Point(){
		vertex[0] = Vertex(0.0f, 0.0f, 0.0f);
		nextPoint = NULL;
	}
	Point(float c_x1, float c_y1, float c_z1){
		vertex[0] = Vertex(c_x1, c_y1, c_z1);
		nextPoint = NULL;	
	}
};

struct World{
	int numberOfPoints;
	Point* HeadPoint;
	Point startPoint;

	int numberOfLines;
	Line* HeadLine;
	Line startLine;

	int numberOfTriangles;
	Triangle* HeadTriangle;
	Triangle startTriangle; 

	World(){
		numberOfPoints = 0;
		startPoint = Point();
		HeadPoint = &startPoint;

		numberOfLines = 0;
		startLine = Line();
		HeadLine = &startLine;

		numberOfTriangles = 0;
		startTriangle = Triangle();
		HeadTriangle = &startTriangle;
	}
	void addPoint(Point * newPoint){
		Point * nextPoint = new Point;
		
		nextPoint = newPoint;
		newPoint->nextPoint = HeadPoint;
		HeadPoint = newPoint;

		numberOfPoints++;
		return;
	};
	void addLine(Line * newLine){
		Line * nextLine = new Line;
		
		nextLine = newLine;
		newLine->nextLine = HeadLine;
		HeadLine = newLine;

		numberOfLines++;
		return;
	};
	void addTriangle(Triangle * newTriangle){
		Triangle * nextTriangle = new Triangle;
		
		nextTriangle = newTriangle;
		newTriangle->nextTriangle = HeadTriangle;
		HeadTriangle = newTriangle;

		numberOfTriangles++;
		return;
	};

	//Triangle* giveTriangles{}; //Should return all Triangle Objects
};


class ObjectManager{

public:
	ObjectManager();

	bool OnMouse(int button, float x, float y);

	void generateCordinateSystem();

	void genaratePoint(float c_x1, float c_y1, float c_z1);

	void genarateLine(float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2);
	
	void genarateTriangle(float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2,float c_x3, float c_y3, float c_z3);

	//void genarateSquare(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1);
	void genarateSquare(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1, float c_x2, float c_y2, float c_z2);

	void generateQuad(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1);
	void generateQuad(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2);

	Vertex* getWorld();

	World sector;	
};

#endif