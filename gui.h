

#ifndef GUI
#define	GUI

#include "gui.h"
#include "math_3d.h"
#include "ObjectManager.h"

enum func{
	NOTHING,
	NEW,
	QUIT,
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
		numOfButtons = 6;
		button = new Button[numOfButtons];
	};

	//~gui();

	void init();
	
	func buttonCB(float x, float y);

	int numOfButtons;
	Button* button;

};


#endif	/* GLUT_BACKEND_H */


