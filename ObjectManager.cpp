#include "ObjectManager.h"
#include <GL/freeglut.h>

ObjectManager::ObjectManager(){
	sector = World();
	
	//build cordinate System
	generateCordinateSystem();	
}

bool ObjectManager::OnMouse(int button, float x, float y){
	bool Ret = false;
	
	//printf("button: %d (%f / %f)\n",button,x,y);
    
	switch (button) {

    case 0:
        {
			generateQuad(x, y, 0, x+0.2, y+0.2, 0);
			Ret = true;
        }
        break;

	}
	return Ret;
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

void ObjectManager::genarateLine(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1){	
	Line * generatetLine = new Line;

	generatetLine = new Line(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1);
	sector.addLine(generatetLine);
}

void ObjectManager::genarateTriangle(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1,float c_x2, float c_y2, float c_z2){	
	Triangle * generatedTriangle = new Triangle;

	generatedTriangle = new Triangle(c_x0, c_y0, c_z0, c_x1, c_y1, c_z1, c_x2, c_y2, c_z2);
	sector.addTriangle(generatedTriangle);
}

/*Square with two Points
void ObjectManager::genarateSquare(float c_x0, float c_y0, float c_z0, float c_x1, float c_y1, float c_z1){
	Triangle * generatedTriangle1 = new Triangle;
	Triangle * generatedTriangle2 = new Triangle;

	generatedTriangle1 = new Triangle(c_x0, c_y0, c_z0,
										c_x1, c_y0, c_z0,
										c_x1, c_y1, c_z1);
	generatedTriangle2 = new Triangle(c_x0, c_y0, c_z0,
										c_x0, c_y0, c_z1,
										c_x1, c_y1, c_z1);
	
	sector.addTriangle(generatedTriangle1);
	sector.addTriangle(generatedTriangle2);
}*/

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

Vertex* ObjectManager::getWorld(){
	Vertex* r_vertex = NULL;
	return r_vertex;
}
