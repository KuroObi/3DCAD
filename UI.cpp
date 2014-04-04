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
	genauigkeit = 0.025f;	//change name and ajust value
	nextVertex;
   }

void UI::changeDrawT(drawType _drawT){
	vCount = 0;
	drawT = _drawT;
}

drawType UI::getDrawT(){
	return drawT;
}

float UI::checkSourounding(Vector3f _vertic, ObjectManager * p_oManager, bool del){

	/*	#TODO:
	 *		*Triangle berechnung
	 *		*Optimierung beim Wurzelziehen
	 *			-Auslagern des Wurzelziehens auf dir Rückgabe des Ergebnisses/Ganz verzichten
	 */

	float dist;
	if(p_oManager->sector.numberOfPoints > 0){
			Point * nP = new Point;
			nP = p_oManager->sector.HeadPoint;
			for(int cPoi = 0; cPoi < p_oManager->sector.numberOfPoints; cPoi++, nP = nP->nextPoint){
				dist = _vertic.Dist(nP->vertex->getVector3f());
				if(dist <= genauigkeit){
					if(del){
						p_oManager->sector.removePoint(nP);
						return 1;
					}else{
						return dist/genauigkeit;
					}
				}				
			}
		}
		//Check for Line
		if(p_oManager->sector.numberOfLines >= 9){
			Line * nL = new Line;
			nL = p_oManager->sector.HeadLine;

			for(int cLin = 0; cLin < p_oManager->sector.numberOfLines; cLin++, nL = nL->nextLine){
				if(nL->protect == true)
					continue;

				dist = calcDistPoiLin(_vertic, nL->vertex[0].getVector3f(), nL->vertex[1].getVector3f());
				if(dist <= genauigkeit){
					if(del){
						p_oManager->sector.removeLine(nL);
						return 1;
					}else{
						return dist/genauigkeit;
					}
				}
				
			}
		}

		//Check for Triangle
		if(p_oManager->sector.numberOfTriangles > 0){

			Triangle * nT = new Triangle;
			nT = p_oManager->sector.HeadTriangle;

			float dist;

			for(int cTri = 0; cTri < p_oManager->sector.numberOfTriangles; cTri++, nT = nT->nextTriangle){
				dist = calcDistPoiTri(_vertic, nT->vertex[0].getVector3f(), nT->vertex[1].getVector3f(), nT->vertex[2].getVector3f());
				//printf("%f\n", dist);
				if(dist <= genauigkeit){
					if(del){
						p_oManager->sector.removeTriangle(nT);
						return 1;
					}else{
						return dist/genauigkeit;
					}
				}
			}
		}
		return 0;
}

float UI::checkVertex(Vector3f _vertic, ObjectManager * p_oManager, bool place){
	float dist;

	if(p_oManager->sector.numberOfPoints > 0){
			Point * nP = new Point;
			nP = p_oManager->sector.HeadPoint;
			for(int cPoi = 0; cPoi < p_oManager->sector.numberOfPoints; cPoi++, nP = nP->nextPoint){
				dist = _vertic.RelDist(nP->vertex->getVector3f());
				printf("%f	%f\n", dist, genauigkeit*genauigkeit);
				if(dist <= genauigkeit){
					if(place){
						nextVertex = nP->vertex->getVector3f();
						return 1;
					}else{
						return dist/(genauigkeit*genauigkeit);
					}
				}				
			}
		}
		//Check for Line
		if(p_oManager->sector.numberOfLines >= 9){
			Line * nL = new Line;
			nL = p_oManager->sector.HeadLine;

			for(int cLin = 0; cLin < p_oManager->sector.numberOfLines; cLin++, nL = nL->nextLine){
				if(nL->protect == true)
					continue;
				for(int twoVertex = 0; twoVertex < 2; twoVertex++){
					dist = _vertic.RelDist(nL->vertex[twoVertex].getVector3f());
					if(dist <= genauigkeit*genauigkeit){
						if(place){
							nextVertex = nL->vertex[twoVertex].getVector3f();
							return 1;
						}else{
							return dist/(genauigkeit*genauigkeit);
						}
					}
				}
			}
		}

		//Check for Triangle
		if(p_oManager->sector.numberOfTriangles > 0){

			Triangle * nT = new Triangle;
			nT = p_oManager->sector.HeadTriangle;

			float dist;

			for(int cTri = 0; cTri < p_oManager->sector.numberOfTriangles; cTri++, nT = nT->nextTriangle){
				for(int threeVertex = 0; threeVertex < 3; threeVertex++){
					dist = _vertic.RelDist(nT->vertex[threeVertex].getVector3f());
					if(dist <= genauigkeit*genauigkeit){
						if(place){
							nextVertex = nT->vertex[threeVertex].getVector3f();
							return 1;
						}else{
							return dist/(genauigkeit*genauigkeit);
						}
					}
				}
			}
		}
		return 0;

}

void UI::reset_vCount(){
	vCount = 0;
	return;
}

void UI::draw(Vector3f _vertic, ObjectManager * p_oManager){

	if(drawT == tREMOVE){
		checkSourounding(_vertic, p_oManager, true);
		return;
	}

	if(vCount <= 1 && _vertic.Dist(drawingV[vCount-1]) <= 0.05f){
		return;
	}

	if(checkVertex(_vertic, p_oManager, true)){
		drawingV[vCount] = nextVertex.getVector3f();
	}else{	
		drawingV[vCount] = _vertic;
	}

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
