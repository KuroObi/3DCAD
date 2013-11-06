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
	QAD
};

struct Button{
	Vertex  vertex[4];
	
	func bFunction;

	Button(){
	}

	Button(Vector3f _vertex0, Vector3f _vertex1, func _bFunction){
		vertex[0] = _vertex0;
		vertex[1] = Vector3f(_vertex0.x,_vertex1.y,0);
		vertex[2] = _vertex1;
		vertex[3] = Vector3f(_vertex1.x,_vertex0.y,0);

		bFunction = _bFunction;
	}
};

class gui{
public:
	gui(){
		numOfButtons = 7;
		button = new Button[numOfButtons];

	};

	//~gui();

	void init();
	
	func buttonCB(float x, float y);

	int numOfButtons;
	Button* button;

	Triangle mouse;

};


#endif	/* GLUT_BACKEND_H */


