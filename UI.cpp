#include "UI.h"


UI::UI(){
	drawT = tLINE;
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
		p_oManager->genarateLine(drawingV[0], drawingV[1]);
		vCount = 0;
	}


}
