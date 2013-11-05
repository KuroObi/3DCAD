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


#include "gui.h"


void gui::init(){
	button[0] = Button(	Vector3f(-0.95f, 0.8f, 0.0f),
						Vector3f(-0.8f, 0.95f, 0.0f),
						NEW);
	
	button[1] = Button(	Vector3f(0.92f, 0.9f, 0.0f),
						Vector3f(0.95f, 0.95f, 0.0f),
						QUIT);

	button[2] = Button(	Vector3f(-0.23f, -0.95f, 0.0f),
						Vector3f(-0.13f, -0.85f, 0.0f),
						POI);
	button[3] = Button( Vector3f(-0.11f, -0.95f, 0.0f),
						Vector3f(-0.01f, -0.85f, 0.0f),
						LINE);
	button[4] = Button(	Vector3f(0.01f, -0.95f, 0.0f),
						Vector3f(0.11f, -0.85f, 0.0f),
						TRI);
	button[5] = Button(	Vector3f(0.13f, -0.95f, 0.0f),
						Vector3f(0.23f, -0.85f, 0.0f),
						QAD);
	
}

func gui::buttonCB(float x, float y){
	for(int b = 0; b < numOfButtons; b++){
		if( x > button[b].vertex[0].xyz.x &&
			x < button[b].vertex[2].xyz.x){
			if( y > button[b].vertex[0].xyz.y &&
				y < button[b].vertex[2].xyz.y){
					return button[b].bFunction;
			}
		}
	}
	return NOTHING;
}