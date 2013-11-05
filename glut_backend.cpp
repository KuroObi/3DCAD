﻿/*

	Copyright 2011 Etay Meiri

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

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glut_backend.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static ICallbacks* s_pCallbacks = NULL;

static void SpecialKeyboardCB(int Key, int x, int y){
    s_pCallbacks->SpecialKeyboardCB(Key, x, y);
}


static void KeyboardCB(unsigned char Key, int x, int y){
    s_pCallbacks->KeyboardCB(Key, x, y);
}

static void MouseCB(int button, int state, int x, int y){
	s_pCallbacks->MouseCB(button, state, x, y);
}

static void PassiveMouseCB(int x, int y){
    s_pCallbacks->PassiveMouseCB(x, y);
}


static void RenderSceneCB(){
    s_pCallbacks->RenderSceneCB();
}


static void IdleCB(){
    s_pCallbacks->IdleCB();
}


static void InitCallbacks(){
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
	glutMouseFunc(MouseCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}


void GLUTBackendInit(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

}


bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, unsigned int bpp, bool isFullScreen, const char* pTitle){
    if (isFullScreen)    {
        char ModeString[64] = { 0 };
#ifdef WIN32
        _snprintf_s(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
#else
        snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
#endif
        glutGameModeString(ModeString);
        glutEnterGameMode();
    }else{
        glutInitWindowSize(Width, Height);
        glutCreateWindow(pTitle);
    }

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK){
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }
	printf("%s",glutLayerGet(GLUT_OVERLAY_POSSIBLE));
	glutEstablishOverlay();
    return true;
}

void GLUTBackendRun(ICallbacks* pCallbacks){
    if (!pCallbacks){
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
    }

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
       
	//Draw both sides
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE); 

    s_pCallbacks = pCallbacks;
    InitCallbacks();
	
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}