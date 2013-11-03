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