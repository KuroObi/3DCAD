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


#ifndef GUI
#define	GUI

#include "gui.h"
#include "math_3d.h"
#include "ObjectManager.h"

enum func{
	NOTHING,
	NEW,
	QUIT,
	STEREO,
	POI,
	LINE,
	TRI,
	QAD,
	REM
};

struct Button{
	Vertex  vertex[4];
	
	func bFunction;

	Button(){
	}

	Button(Vertex _vertex0, Vertex _vertex1, func _bFunction){
		vertex[0] = _vertex0;
		vertex[1] = Vertex(Vector3f(_vertex1.xyz.x,_vertex0.xyz.y,0),Vector3f(_vertex1.rgb.x, _vertex0.rgb.y, 1.0f));
		vertex[2] = _vertex1;
		vertex[3] = Vertex(Vector3f(_vertex0.xyz.x,_vertex1.xyz.y,0),Vector3f(_vertex0.rgb.x, _vertex1.rgb.y, 1.0f));

		bFunction = _bFunction;
	}
};

class gui{
public:
	gui(){
		numOfButtons = 8;
		button = new Button[numOfButtons];

	};

	//~gui();

	void init();
	
	func buttonCB(float x, float y);

	int numOfButtons;
	Button* button;
};


#endif	/* GLUT_BACKEND_H */


