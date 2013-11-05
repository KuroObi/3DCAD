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
	genarateLine(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	genarateLine(1.0f, 0.0f, 0.0f, 0.95f, 0.025f, 0.0f);
	genarateLine(1.0f, 0.0f, 0.0f, 0.95f, -0.025f, 0.0f);
	
	genarateLine(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	genarateLine(0.0f, 1.0f, 0.0f, 0.05f, 0.95f, -0.025f);
	genarateLine(0.0f, 1.0f, 0.0f, -0.05f, 0.95f, 0.025f);
	
	genarateLine(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	genarateLine(0.0f, 0.0f, 1.0f, 0.0f, 0.025f, 0.95f);
	genarateLine(0.0f, 0.0f, 1.0f, 0.0f, -0.025f, 0.95f);
}

void ObjectManager::genaratePoint(float c_x0, float c_y0, float c_z0){	
	Point * generatetPoint = new Point;

	generatetPoint = new Point(c_x0, c_y0, c_z0);
	sector.addPoint(generatetPoint);
}

void ObjectManager::genaratePoint(Vector3f _vertex0){	
	Point * generatetPoint = new Point;

	generatetPoint = new Point(_vertex0);
	sector.addPoint(generatetPoint);
}

void ObjectManager::genarateLine(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1){	
	Line * generatetLine = new Line;

	generatetLine = new Line(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1);
	sector.addLine(generatetLine);
}

void ObjectManager::genarateLine(Vector3f _vertex0, Vector3f _vertex1){	
	Line * generatetLine = new Line;

	generatetLine = new Line(_vertex0, _vertex1);
	sector.addLine(generatetLine);
}

void ObjectManager::genarateTriangle(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1, c_x2, c_y2, c_z2);
	sector.addTriangle(generatedTriangle);
}

void ObjectManager::genarateTriangle(Vector3f _vertex0, Vector3f _vertex1, Vector3f _vertex2){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(_vertex0, _vertex1, _vertex2);
	sector.addTriangle(generatedTriangle);
}

//Square with thee Points
void ObjectManager::genarateSquare(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1, float c_x2, float c_y2, float c_z2){
	Triangle * generatedTriangle1 = new Triangle;
	Triangle * generatedTriangle2 = new Triangle;

	generatedTriangle1 = new Triangle(c_x0, c_y0, c_z0,
									c_x1, c_y1, c_z1,
									c_x2, c_y2, c_z2);
	generatedTriangle2 = new Triangle(c_x0, c_y0, c_z0,
									((c_x0+c_x2) - c_x1), ((c_y0+c_y2) -  c_y1), ((c_z0+c_z2) -  c_z1),
									c_x2, c_y2, c_z2);
	
	sector.addTriangle(generatedTriangle1);
	sector.addTriangle(generatedTriangle2);

	//printf("Tri: %f/%f/%f\n", c_x0, c_y0, c_z0);
}

//Quard with two Points
void ObjectManager::generateQuad(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1){
	genarateSquare(c_x0, c_y0, c_z0, c_x0, c_y1, c_z0, c_x1, c_y1, c_z0);
	genarateSquare(c_x0, c_y0, c_z0, c_x1, c_y0, c_z0, c_x1, c_y0, c_z1);
	genarateSquare(c_x0, c_y0, c_z0, c_x0, c_y0, c_z1, c_x0, c_y1, c_z1);
	
	genarateSquare(c_x1, c_y1, c_z1, c_x1, c_y0, c_z1, c_x1, c_y0, c_z0);
	genarateSquare(c_x1, c_y1, c_z1, c_x1, c_y1, c_z0, c_x0, c_y1, c_z0);
	genarateSquare(c_x1, c_y1, c_z1, c_x0, c_y1, c_z1, c_x0, c_y0, c_z1);
}

/*
//Quard with three Points
void ObjectManager::generateQuad(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2){
	float durch = sqrt((sqrt(c_x0*c_x0 + c_x1*c_x1)) + (sqrt(c_y0*c_y0 + c_y1*c_y1)) + (sqrt(c_z0*c_z0 + c_z1*c_z1)));
	genarateSquare(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1, c_x0 + durch/2, c_y1, c_x1 + durch/2);
	genarateSquare(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1, c_x2, c_y2, c_z2);
	//genarateSquare(c_x0, c_y0, c_z0, c_x0, c_y0, c_z1, c_x0, c_y1, c_z1);
	
	//genarateSquare(c_x2, c_y2, c_z2, c_x1, c_y0, c_z1, c_x1, c_y0, c_z0);
	//genarateSquare(c_x1, c_y1, c_z1, c_x2, c_y2, c_z1, c_x2, c_y2, c_z2);
	//genarateSquare(c_x2, c_y2, c_z2, c_x0, c_y1, c_z1, c_x0, c_y0, c_z1);
}
*/

Vertex* ObjectManager::getWorld(){
	Vertex* r_vertex = NULL;
	return r_vertex;
}
