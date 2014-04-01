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

#define grey Vector3f(0.3f, 0.3f, 0.3f)

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
	Vector3f red = Vector3f(1.0f, 0.0f, 0.0f);
	Vector3f green = Vector3f(0.0f, 0.8f, 0.0f);
	Vector3f blue = Vector3f(0.0f, 0.0f, 1.0f);

	genarateLine(center, xA, red, true);
	genarateLine(xA, Vector3f(0.95f, 0.025f, 0.0f), red, true);
	genarateLine(xA, Vector3f(0.95f, -0.025f, 0.0f), red, true);
	
	genarateLine(center, yA, blue, true);
	genarateLine(yA, Vector3f(0.05f, 0.95f, -0.025f), blue, true);
	genarateLine(yA, Vector3f(-0.05f, 0.95f, 0.025f), blue, true);
	
	genarateLine(center, zA, green, true);
	genarateLine(zA, Vector3f(0.0f, 0.025f, 0.95f), green, true);
	genarateLine(zA, Vector3f(0.0f, -0.025f, 0.95f), green, true);
}

void ObjectManager::genaratePoint(Vector3f _vertex0){	
	Point * generatetPoint = new Point;

	generatetPoint = new Point(_vertex0, grey);
	sector.addPoint(generatetPoint);
}
void ObjectManager::deletePoint(Point * rmPoint){
	sector.removePoint(rmPoint);
}

void ObjectManager::genarateLine(Vector3f _vertex0, Vector3f _vertex1){	
	Line * generatetLine = new Line;

	generatetLine = new Line(_vertex0, _vertex1, grey, false);
	sector.addLine(generatetLine);
}
void ObjectManager::genarateLine(Vector3f _vertex0, Vector3f _vertex1, Vector3f _color, bool _protect){	
	Line * generatetLine = new Line;

	generatetLine = new Line(_vertex0, _vertex1, _color, _protect);
	sector.addLine(generatetLine);
}
void ObjectManager::deleteLine(Line * rmLine){
	sector.removeLine(rmLine);
}

void ObjectManager::genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){	
	genarateTriangle(_vertex0, _vertex1, _vertex2, grey);
}
void ObjectManager::genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2, Vector3f _color){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(_vertex0, _vertex1, _vertex2, _color);
	sector.addTriangle(generatedTriangle);
}
void ObjectManager::deleteTriangle(Triangle * rmTriangle){
	sector.removeTriangle(rmTriangle);
}

void ObjectManager::genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){
	genarateSquare(_vertex0, _vertex1, _vertex2, grey);
}
void ObjectManager::genarateSquare(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2, Vector3f _color){
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
