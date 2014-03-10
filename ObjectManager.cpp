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

#include "ObjectManager.h"
#include <GL/freeglut.h>

ObjectManager::ObjectManager(){
	sector = World();
	
	//build cordinate System
	generateCordinateSystem();	
}


void ObjectManager::generateCordinateSystem(){
	Vector3f center = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f xA = Vector3f(1.0f, 0.0f, 0.0f);
	Vector3f yA = Vector3f(0.0f, 1.0f, 0.0f);
	Vector3f zA = Vector3f(0.0f, 0.0f, 1.0f);
	Vector4f red = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
	Vector4f green = Vector4f(0.0f, 0.8f, 0.0f, 1.0f);
	Vector4f blue = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

	genarateLine(center, xA, red);
	genarateLine(xA, Vector3f(0.95f, 0.025f, 0.0f), red);
	genarateLine(xA, Vector3f(0.95f, -0.025f, 0.0f), red);
	
	genarateLine(center, yA, blue);
	genarateLine(yA, Vector3f(0.05f, 0.95f, -0.025f), blue);
	genarateLine(yA, Vector3f(-0.05f, 0.95f, 0.025f), blue);
	
	genarateLine(center, zA, green);
	genarateLine(zA, Vector3f(0.0f, 0.025f, 0.95f), green);
	genarateLine(zA, Vector3f(0.0f, -0.025f, 0.95f), green);
}

void ObjectManager::genaratePoint(Vector3f _vertex0){	
	Point * generatetPoint = new Point;

	generatetPoint = new Point(_vertex0);
	sector.addPoint(generatetPoint);
}
void ObjectManager::deletePoint(Point * rmPoint){
	sector.removePoint(rmPoint);
}

void ObjectManager::genarateLine(Vector3f _vertex0, Vector3f _vertex1){	
	Line * generatetLine = new Line;

	generatetLine = new Line(_vertex0, _vertex1);
	sector.addLine(generatetLine);
}

void ObjectManager::genarateLine(Vector3f _vertex0, Vector3f _vertex1, Vector4f _color){	
	Line * generatetLine = new Line;

	generatetLine = new Line(_vertex0, _vertex1, _color);
	sector.addLine(generatetLine);
}
void ObjectManager::deleteLine(Line * rmLine){
	sector.removeLine(rmLine);
}

void ObjectManager::genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(_vertex0, _vertex1, _vertex2);
	sector.addTriangle(generatedTriangle);
}
void ObjectManager::genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2, Vector4f _color){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(_vertex0, _vertex1, _vertex2, _color);
	sector.addTriangle(generatedTriangle);
}
void ObjectManager::deleteTriangle(Triangle * rmTriangle){
	sector.removeTriangle(rmTriangle);
}

void ObjectManager::genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){
	Triangle * generatedTriangle1 = new Triangle;
	Triangle * generatedTriangle2 = new Triangle;

	generatedTriangle1 = new Triangle(_vertex0, _vertex1, _vertex2);
	
	generatedTriangle2 = new Triangle(_vertex0, _vertex0 - (_vertex1 - _vertex2), _vertex2);
	
	sector.addTriangle(generatedTriangle1);
	sector.addTriangle(generatedTriangle2);
}
void ObjectManager::genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2, Vector4f _color){
	Triangle * generatedTriangle1 = new Triangle;
	Triangle * generatedTriangle2 = new Triangle;

	generatedTriangle1 = new Triangle(_vertex0, _vertex1, _vertex2, _color);
	
	generatedTriangle2 = new Triangle(_vertex0, _vertex0 - (_vertex1 - _vertex2), _vertex2, _color);
	
	sector.addTriangle(generatedTriangle1);
	sector.addTriangle(generatedTriangle2);
}


Vertex* ObjectManager::getWorld(){
	Vertex* r_vertex = NULL;
	return r_vertex;
}
