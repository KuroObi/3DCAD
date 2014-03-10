/*
	Copyright 2013 Christoph Schabert

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OBJECT_MANAGER
#define	OBJECT_MANAGER

#include <stdio.h>
#include "math_3d.h"

struct Vertex{
	Vector3f xyz;
	Vector4f rgba;
	
	Vertex(){
	};

	Vertex(Vector3f _Vertex){
		xyz = _Vertex;
		rgba = Vector4f(0.2f, 0.2f, 0.2f, 1.0f);
	}
	
	Vertex(Vector3f _Vertex, Vector4f _Color){
		xyz = _Vertex;
		rgba = _Color;
	}

	Vector3f getVector3f(){
		return xyz;
	}
};

struct Obj{
};

struct Point : Obj{
	Vertex  vertex[1];
	Point * nextPoint;
	Point * beforePoint;

	Point(){
		vertex[0] = Vertex();
		nextPoint = NULL;
		beforePoint = NULL;
	}

	Point(Vector3f _vertex0){
		vertex[0] = _vertex0;
		nextPoint = NULL;
		beforePoint = NULL;
	}

	Point(Vector3f _vertex0, Vector4f _color){
		vertex[0].xyz = _vertex0;
		vertex[0].rgba = _color;
		nextPoint = NULL;
		beforePoint = NULL;
	}
};

struct Line{
	Vertex  vertex[2];
	Line * nextLine;
	Line * beforeLine;

	Line(){
		vertex[0] = Vertex();
		vertex[1] = Vertex();
		nextLine = NULL;
		beforeLine = NULL;
	}

	Line(Vector3f _vertex0, Vector3f _vertex1){
		vertex[0] = _vertex0;
		vertex[1] = _vertex1;
		nextLine = NULL;
		beforeLine = NULL;
	}

	Line(Vector3f _vertex0, Vector3f _vertex1, Vector4f _color){
		vertex[0].xyz = _vertex0;
		vertex[0].rgba = _color;
		vertex[1].xyz = _vertex1;
		vertex[1].rgba = _color;
		nextLine = NULL;
		beforeLine = NULL;
	}
};

struct Triangle{
	Vertex  vertex[3];
	Triangle * nextTriangle;
	Triangle * beforeTriangle;

	Triangle(){
		vertex[0] = Vertex();
		vertex[1] = Vertex();
		vertex[2] = Vertex();
		nextTriangle = NULL;
		beforeTriangle = NULL;
	}

	Triangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){
		vertex[0] = _vertex0;
		vertex[1] = _vertex1;
		vertex[2] = _vertex2;
		nextTriangle = NULL;
		beforeTriangle = NULL;
	}

	Triangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2, Vector4f _color){
		vertex[0].xyz = _vertex0;
		vertex[0].rgba = _color;
		vertex[1].xyz = _vertex1;
		vertex[1].rgba =_color;
		vertex[2].xyz = _vertex2;
		vertex[2].rgba =_color;
		nextTriangle = NULL;
		beforeTriangle = NULL;	
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
		newPoint->nextPoint = HeadPoint;
		if(numberOfPoints != 0)
			HeadPoint->beforePoint = newPoint;
		else
			newPoint->nextPoint = NULL;
		newPoint->beforePoint = NULL;
		HeadPoint = newPoint;
		
		numberOfPoints++;
		return;
	};
	void removePoint(Point * rmPoint){
		if(numberOfPoints == 1){
			Point * newPoint = new Point;
			HeadPoint = newPoint;
		}else{		
			rmPoint->vertex->xyz.Print();
			if(rmPoint->beforePoint == NULL)
				HeadPoint = rmPoint->nextPoint;
			else
				rmPoint->beforePoint->nextPoint = rmPoint->nextPoint;
			if(rmPoint->nextPoint != NULL)
				rmPoint->nextPoint->beforePoint = rmPoint->beforePoint;
		}
		delete &rmPoint;
		numberOfPoints--;
		return;
	};

	void addLine(Line * newLine){
		newLine->nextLine = HeadLine;
		HeadLine = newLine;

		numberOfLines++;
		return;
	};
	void removeLine(Line * rmLine){
		if(numberOfLines == 1){
			Line * newLine = new Line;
			HeadLine = newLine;
		}else{		
			rmLine->vertex->xyz.Print();
			if(rmLine->beforeLine == NULL)
				HeadLine = rmLine->nextLine;
			else
				rmLine->beforeLine->nextLine = rmLine->nextLine;
			if(rmLine->nextLine != NULL)
				rmLine->nextLine->beforeLine = rmLine->beforeLine;
		}
		delete &rmLine;
		numberOfLines--;
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
	void removeTriangle(Triangle * rmTriangle){
		if(numberOfTriangles == 1){
			Triangle * newTriangle = new Triangle;
			HeadTriangle = newTriangle;
		}else{		
			rmTriangle->vertex->xyz.Print();
			if(rmTriangle->beforeTriangle == NULL)
				HeadTriangle = rmTriangle->nextTriangle;
			else
				rmTriangle->beforeTriangle->nextTriangle = rmTriangle->nextTriangle;
			if(rmTriangle->nextTriangle != NULL)
				rmTriangle->nextTriangle->beforeTriangle = rmTriangle->beforeTriangle;
		}
		delete &rmTriangle;
		numberOfTriangles--;
		return;
	};
};


class ObjectManager{

public:
	ObjectManager();

	bool OnMouse(int button, float x, float y);

	void generateCordinateSystem();

	void genaratePoint(Vector3f _vertex0);
	void genaratePoint(Vector3f _vertex0, Vector4f _color);
	void deletePoint(Point * rmPoint);

	void genarateLine(Vector3f _vertex0, Vector3f _vertex1);
	void genarateLine(Vector3f _vertex0, Vector3f _vertex1, Vector4f _color);
	void deleteLine(Line * rmLine);

	void genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex3);
	void genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex3, Vector4f _color);
	void deleteTriangle(Triangle * rmTriangle);

	void genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex3);
	void genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex3, Vector4f _color);

	Vertex* getWorld();

	World sector;	
};

#endif