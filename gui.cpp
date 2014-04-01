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
	button[0] = Button(	Vertex(Vector3f(-0.95f, 0.78f, 0.0f), Vector3f(0.0f, 0.667f, 1.0f)),
						Vertex(Vector3f(-0.85f, 0.95f, 0.0f), Vector3f(0.333f, 1.0f, 1.0f)),
						NEW);
	button[1] = Button(	Vertex(Vector3f(0.92f, 0.9f, 0.0f), Vector3f(0.334f, 0.667f, 1.0f)),
						Vertex(Vector3f(0.95f, 0.95f, 0.0f), Vector3f(0.666f, 1.0f, 1.0f)),
						QUIT);
	button[2] = Button(	Vertex(Vector3f(0.9f, -0.95f, 0.0f), Vector3f(0.667f, 0.667f, 1.0f)),
						Vertex(Vector3f(0.95f, -0.9f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)),
						STEREO);

	button[3] = Button(	Vertex(Vector3f(-0.35f, -0.95f, 0.0f), Vector3f(0.0f, 0.334f, 1.0f)),
						Vertex(Vector3f(-0.25f, -0.85f, 0.0f), Vector3f(0.333f, 0.666f, 1.0f)),
						POI);
	button[4] = Button( Vertex(Vector3f(-0.20f, -0.95f, 0.0f), Vector3f(0.334f, 0.334f, 1.0f)),
						Vertex(Vector3f(-0.10f, -0.85f, 0.0f), Vector3f(0.666f, 0.666f, 1.0f)),
						LINE);
	button[5] = Button(	Vertex(Vector3f(-0.05f, -0.95f, 0.0f), Vector3f(0.667f, 0.334f, 1.0f)),
						Vertex(Vector3f(0.05f, -0.85f, 0.0f), Vector3f(1.0f, 0.666f, 1.0f)),
						TRI);
	button[6] = Button(	Vertex(Vector3f(0.11f, -0.95f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)),
						Vertex(Vector3f(0.20f, -0.85f, 0.0f), Vector3f(0.333f, 0.333f, 1.0f)),
						QAD);
	button[7] = Button(	Vertex(Vector3f(0.25f, -0.95f, 0.0f), Vector3f(0.334f, 0.0f, 1.0f)),
						Vertex(Vector3f(0.35f, -0.85f, 0.0f), Vector3f(0.666f, 0.333f, 1.0f)),
						REM);
	
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