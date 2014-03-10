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


#include "UI.h"

UI::UI(){
	drawT = tTRI;
	vCount = 0;
   }

void UI::changeDrawT(drawType _drawT){
	drawT = _drawT;
	
}

Obj checkSourounding(Vector3f _vertic, World sector){
	Point fPoint;

	//fPoint = *sector.HeadPoint;
	//while(Point-> ){
	//
	//}
	return fPoint;
}

void UI::draw(Vector3f _vertic, ObjectManager * p_oManager){
	
	Obj * oj = new Obj;

	if(drawT == tREMOVE){
		float genauigkeit = 0.025f;	//change name and ajust value

		//Check for Point
		if(p_oManager->sector.numberOfPoints > 0){
			
			Point * nP = new Point;
			nP = p_oManager->sector.HeadPoint;

			for(int cPoi = 0; cPoi < p_oManager->sector.numberOfPoints; cPoi++){
				if(_vertic.z < nP->vertex->xyz.z+genauigkeit && _vertic.z > nP->vertex->xyz.z-genauigkeit){
					if(_vertic.y < nP->vertex->xyz.y+genauigkeit && _vertic.y > nP->vertex->xyz.y-genauigkeit){
						if(_vertic.x < nP->vertex->xyz.x+genauigkeit && _vertic.x > nP->vertex->xyz.x-genauigkeit){
							p_oManager->sector.removePoint(nP);
							return;
						}
					}
				}
				nP = nP->nextPoint;
			}
		}
		//Check for Line
		if(p_oManager->sector.numberOfLines > 9){
			
			Line * nL = new Line;
			nL = p_oManager->sector.HeadLine;

			for(int n = 0; n < 9; n++)
				nL = nL->nextLine;

			float dist;

			for(int cLin = 9; cLin < p_oManager->sector.numberOfLines; cLin++){

				dist = calcDistPoiLin(_vertic, nL->vertex[0].getVector3f(), nL->vertex[1].getVector3f());
				printf("\ndist: %f", dist);
				
				/*
				if(_vertic.z < nL->vertex->xyz.z+genauigkeit && _vertic.z > nL->vertex->xyz.z-genauigkeit){
					if(_vertic.y < nL->vertex->xyz.y+genauigkeit && _vertic.y > nL->vertex->xyz.y-genauigkeit){
						if(_vertic.x < nL->vertex->xyz.x+genauigkeit && _vertic.x > nL->vertex->xyz.x-genauigkeit){
							nL->vertex->xyz.Print();
							p_oManager->sector.removeLine(nL);
							return;
						}
					}
				}*/
				nL = nL->nextLine;
			}
		}
		//Check for Triangle
		if(p_oManager->sector.numberOfTriangles > 0){
			
			Triangle * nT = new Triangle;
			nT = p_oManager->sector.HeadTriangle;

			for(int cTri = 0; cTri < p_oManager->sector.numberOfTriangles; cTri++){
				if(_vertic.z < nT->vertex->xyz.z+genauigkeit && _vertic.z > nT->vertex->xyz.z-genauigkeit){
					if(_vertic.y < nT->vertex->xyz.y+genauigkeit && _vertic.y > nT->vertex->xyz.y-genauigkeit){
						if(_vertic.x < nT->vertex->xyz.x+genauigkeit && _vertic.x > nT->vertex->xyz.x-genauigkeit){
							nT->vertex->xyz.Print();
							p_oManager->sector.removeTriangle(nT);
							return;
						}
					}
				}
				nT = nT->nextTriangle;
			}
		}
	}
		//oj = &checkSourounding(_vertic, p_oManager->sector);
	//printf("%i", oj);

	if(vCount <= 1 && _vertic.Dist(drawingV[vCount-1]) <= 0.05f){
		return;
	}

	drawingV[vCount] = _vertic;
	vCount++;

	if(vCount == drawT%10){
		switch(drawT){
			case tPOINT:{
				p_oManager->genaratePoint(drawingV[0]);
			}
			break;
			case tLINE:{
				p_oManager->genarateLine(drawingV[0], drawingV[1]);
			}
			break;
			case tTRI:{
				p_oManager->genarateTriangle(drawingV[0], drawingV[1], drawingV[2]);
			}
			break;
			case tSQUAR:{
				p_oManager->genarateSquare(drawingV[0], drawingV[1], drawingV[2]);
			}
			break;			
		}
		vCount = 0;
	}


}
