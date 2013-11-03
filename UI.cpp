#include "UI.h"


UI::UI(){
	drawT = tTRI;
	vCount = 0;
   }

void UI::changeDrawT(drawType _drawT){
	drawT = _drawT;
	
}

void UI::draw(Vector3f _vertic, ObjectManager * p_oManager){
	
	if(vCount <= 1 && _vertic.Dist(drawingV[vCount-1]) <= 0.05f ){
		return;
	}

	drawingV[vCount] = _vertic;
	vCount++;

	if(vCount == drawT){
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
			/*
			case tSQUAR:{
			}
			break;
			case tQUAD:{
			}
			break;
			*/
		}
		vCount = 0;
	}


}
