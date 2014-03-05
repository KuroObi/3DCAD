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

void UI::draw(Vector3f _vertic, ObjectManager * p_oManager){
	
	if(vCount <= 1 && _vertic.Dist(drawingV[vCount-1]) <= 0.05f ){
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
