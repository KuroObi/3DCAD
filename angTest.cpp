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

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <IL\il.h>

#include "pipeline.h"
#include "camera.h"
#include "ObjectManager.h"
#include "UI.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "gui.h"

GLuint vao, textureID;

#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900

class AngTest : public ICallbacks{
	public:
		AngTest(){
			m_pGameCamera = NULL;
			m_scale = 0.0f;
		    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		    m_directionalLight.AmbientIntensity = 1.0f;	
		}
		
		~AngTest(){
			delete m_pEffect;
			delete m_pGameCamera;
			delete &m_oManager;
			delete &m_directionalLight;
		}

		bool Init(){
			Vector3f Pos(0.0f, 0.0f, -22.0f);
			Vector3f Target(0.0f, -0.2f, 1.0f);
			Vector3f Up(0.0, 0.0f, 0.0f);

			eyeStep = 0.1f;

			worldPos = Vector3f(0.0f, 0.0f, 3.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eyeStep);

			CreateVertexBuffer();
			//CreateIndexBuffer();

			m_pEffect = new LightingTechnique();

			if (!m_pEffect->Init()){
				return false;
			}
			
			m_pEffect->Enable();

			stereo = false;
			turnAround = false;

			m_gui.init();
			initGUI();
			
			m_mousePos = Vector3f(0.0, 0.0, 1.0);
			m_mouse2DPos = Vector2f(0.0f, 0.0f);
			m_horoptor = 1.1f;
			m_horoptorStep = 0.05f;
			
			initMouse();

			return true;
		}

	    void Run(){
			GLUTBackendRun(this);
		}

		virtual void RenderSceneCB(){
			m_pGameCamera->OnRender();
			
			m_pEffect->SetGUI(0);
			RenderPhase();
			//MousePhase(); 

			glDisable(GL_DEPTH_TEST);			//Disable 3D-Things
			
			m_pEffect->SetGUI(1);
			GuiPhase();
			
			glEnable(GL_DEPTH_TEST);			//Reaktivate 3D-Things
		
			glutSwapBuffers();
		}
/*
		void MousePhase(){
			
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, m_mouseVBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
			glDrawArrays(GL_LINES, 0, 6);
			
			glDisableVertexAttribArray(0);		
			glDisableVertexAttribArray(1);
		}
*/
		void GuiPhase(){

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, m_guiVBO);
			

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
			glDrawArrays(GL_QUADS, 0,m_gui.numOfButtons * 4);
			

			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);

		}

		void RenderPhase(){
		
			glDrawBuffer(GL_BACK);
			glReadBuffer(GL_BACK);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

			static float Scale = 0.0f;

			Scale += 0.0f;

			Pipeline p;
			p.Rotate(0.0f, Scale, 0.0f);
			p.WorldPos(0.0f, -0.0f, 3.0f);

			p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
			p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
	        
			m_pEffect->SetWVP(p.GetTrans());
			m_pEffect->SetDirectionalLight(m_directionalLight);
		
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			if(stereo == true){
				glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

			glDrawArrays(GL_POINTS,0, m_oManager.sector.numberOfPoints);
			glDrawArrays(GL_LINES, m_oManager.sector.numberOfPoints, m_oManager.sector.numberOfLines*2);
			glDrawArrays(GL_TRIANGLES, m_oManager.sector.numberOfPoints+m_oManager.sector.numberOfLines*2, m_oManager.sector.numberOfTriangles*3);
			
			glDrawBuffer(GL_BACK);

			if(stereo == true){
				m_pGameCamera->rightEye(m_horoptor);

				p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
				p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
				m_pEffect->SetWVP(p.GetTrans());
				m_pEffect->SetDirectionalLight(m_directionalLight);
				glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
				
				glDrawArrays(GL_POINTS,0, m_oManager.sector.numberOfPoints);
				glDrawArrays(GL_LINES, m_oManager.sector.numberOfPoints, m_oManager.sector.numberOfLines*2);
				glDrawArrays(GL_TRIANGLES, m_oManager.sector.numberOfPoints+m_oManager.sector.numberOfLines*2, m_oManager.sector.numberOfTriangles*3);

				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
				m_pGameCamera->leftEye(m_horoptor);	
			}
			glDisableVertexAttribArray(0);			
			glDisableVertexAttribArray(1);
		}

		void mouseAction(){

		}

	    virtual void IdleCB(){
		    RenderSceneCB();
		}

		virtual void SpecialKeyboardCB(int Key, int x, int y){
			m_pGameCamera->OnKeyboard(Key);
		}

		virtual void KeyboardCB(unsigned char Key, int x, int y){
			switch(Key){
						case 'n':
							m_oManager = ObjectManager();
							m_UI = UI();
							break;
						case 'q':
							exit(0);
							break;
						case '1':
							m_UI.changeDrawT(tPOINT);
							break;
						case '2':
							m_UI.changeDrawT(tLINE);
							break;
						case '3':
							m_UI.changeDrawT(tTRI);
							break;
						case '4':
							m_UI.changeDrawT(tSQUAR);
							break;
						case 's':
							if(stereo){
								stereo = false;
							}else{
								stereo = true;
							}
							break;
						case 'r':
							m_UI.changeDrawT(tREMOVE);
							break;
					}
			initGUI();
		}

		virtual void PassiveMouseCB(int x, int y){
			if(	x < WINDOW_WIDTH/100 ||
				x > WINDOW_WIDTH - WINDOW_WIDTH/100 ||
				y < WINDOW_HEIGHT/100 ||
				y > WINDOW_HEIGHT - WINDOW_HEIGHT/100){
				m_pGameCamera->OnMouse(x, y);		
			}
			
			float relativ_x = (((float)x)/((float)WINDOW_WIDTH) - 0.5f) * 2.0f;
			float relativ_y = (((float)y)/((float)WINDOW_HEIGHT) - 0.5f) * -2.0f;
			m_mouse2DPos = Vector2f(relativ_x, relativ_y);
			calcWorldPos();
			refreshMouse();
		}

		virtual void MouseCB(int Button, int State, int x, int y){
			
			if (Button == 4 || Button == 3) {
				if(State == GLUT_DOWN){
					State = GLUT_UP;						
					if(Button == 3){				//Mouse Scroll up
						if(m_horoptor > 7.0f){
							return;
						}
						m_horoptor += m_horoptorStep;
					}
					else{						//Mouse Scroll down
						if(m_horoptor <= 1.1f){
							return;
						}
						m_horoptor -= m_horoptorStep;
					}

					calcWorldPos();
					refreshMouse();
					
					return;
				}
			}

			if (Button == GLUT_LEFT_BUTTON) {
				
				if(State == GLUT_DOWN){
					State = GLUT_UP;
					float relativ_x = (((float)x)/((float)WINDOW_WIDTH) - 0.5f) * 2.0f;
					float relativ_y = (((float)y)/((float)WINDOW_HEIGHT) - 0.5f) * -2.0f;
		
					switch(m_gui.buttonCB(relativ_x, relativ_y)){
						case NEW:
							m_oManager = ObjectManager();
							m_UI = UI();
							break;
						case QUIT:
							exit(0);
							break;
						case POI:
							m_UI.changeDrawT(tPOINT);
							break;
						case LINE:
							m_UI.changeDrawT(tLINE);
							break;
						case TRI:
							m_UI.changeDrawT(tTRI);
							break;
						case QAD:
							m_UI.changeDrawT(tSQUAR);
							break;
						case REM:
							m_UI.changeDrawT(tREMOVE);
							break;
						case STEREO:
							if(stereo){
								stereo = false;
							}else{
								stereo = true;
							}
							break;
						case NOTHING:
							calcWorldPos();
							m_UI.draw(m_mousePos, &m_oManager);
							break;
					}
					
					initGUI();
					CreateVertexBuffer();
					
					return;
				}
			}
		}

		void calcWorldPos(){	
			m_mousePos = m_pGameCamera->GetPos() - worldPos;

			m_mousePos += (m_pGameCamera->GetTarget() * m_horoptor);

			m_mousePos.x += (m_mouse2DPos.x * 2.0f) * m_pGameCamera->GetTarget().z;
			m_mousePos.z += (m_mouse2DPos.x * 2.0f) * -m_pGameCamera->GetTarget().x;

			m_mousePos.y += (m_mouse2DPos.y * 1.0f);
		}


	private:

		void initGUI(){
			int numberOfButtons = m_gui.numOfButtons;
			int sizeVertices = numberOfButtons* 4 * 12 * 2;
			Vertex * guiVert;
			guiVert = new Vertex[numberOfButtons * 4];

			float selectedColor = 0.0f;
			
			for(int c = 0; c < numberOfButtons; c++){
				guiVert[c*4 + 0] = m_gui.button[c].vertex[0];
				guiVert[c*4 + 1] = m_gui.button[c].vertex[1];
				guiVert[c*4 + 2] = m_gui.button[c].vertex[2];
				guiVert[c*4 + 3] = m_gui.button[c].vertex[3];
				
				if(m_UI.getDrawT() == m_gui.button[c].bFunction){
					if(m_UI.getDrawT() == tREMOVE){
						selectedColor = 0.5f;
					}else{
						selectedColor = 1.0f;
					}
					guiVert[c*4 + 0].rgb.z = selectedColor;
					guiVert[c*4 + 1].rgb.z = selectedColor;
					guiVert[c*4 + 2].rgb.z = selectedColor;
					guiVert[c*4 + 3].rgb.z = selectedColor;
				}
			}

			glGenBuffers(1, &m_guiVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_guiVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVertices, guiVert, GL_STATIC_DRAW);

		}

		void initMouse(){
			m_oManager.generateMouse(m_mousePos, Vector3f(0.95f, 0.95f, 0.95f));
		}

		void refreshMouse(){
			Vector3f mouseColor = Vector3f(0.95f, 0.95f, 0.95f);
			Vector3f _mouseColor;
			float nextDist = 0;

			if(m_UI.getDrawT() == tREMOVE){
				_mouseColor = Vector3f(0.0f, 1.0f, 1.0f);
				nextDist = m_UI.checkSourounding(m_mousePos, &m_oManager, false);
			}else{
				_mouseColor = Vector3f(1.0f, 1.0f, 0.0f);
				nextDist = m_UI.checkVertex(m_mousePos, &m_oManager, false);
			}

			if(nextDist != 0){
				mouseColor -= (_mouseColor*(1-nextDist));
			}
			

			m_oManager.refMouse(m_mousePos, mouseColor);
			CreateVertexBuffer();
		}

		void CreateVertexBuffer(){
			int numPoi = m_oManager.sector.numberOfPoints;
			int numLi = m_oManager.sector.numberOfLines;
			int numTri = m_oManager.sector.numberOfTriangles;
			int cVertes = 0;
			int sizeVertices = (numPoi + numLi*2 + numTri*3) * 12 * 2;

			Vertex * Vertices;
			Vertices = new Vertex[numPoi + numLi*2 + numTri*3];
			
			Point * thisPoint = m_oManager.sector.HeadPoint;
			Line * thisLine = m_oManager.sector.HeadLine;
			Triangle * thisTriangle = m_oManager.sector.HeadTriangle;
			
			
			for(int cPoi = 0; cPoi < numPoi; cPoi++, cVertes++){
				Vertices[cVertes] = thisPoint->vertex[0];
				if(thisPoint->nextPoint != NULL){
					thisPoint = thisPoint->nextPoint;
				}
			}
			for(int cLi = 0; cLi < numLi; cLi++){
				for(int cThisVertice = 0; cThisVertice < 2; cThisVertice++, cVertes++){
					Vertices[cVertes] = thisLine->vertex[cThisVertice];
				}
				if(thisLine->nextLine == NULL){
					break;
				}else{
					thisLine = thisLine->nextLine;
				}
			}	
			for(int cTri = 0; cTri < numTri; cTri++){
				for(int cThisVertice = 0; cThisVertice < 3; cThisVertice++, cVertes++){
					Vertices[cVertes] = thisTriangle->vertex[cThisVertice];
				}
				if(thisTriangle->nextTriangle == NULL){
					break;
				}else{
					thisTriangle = thisTriangle->nextTriangle;
				}
			}
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVertices, Vertices, GL_STATIC_DRAW);
		}
		
	//Variables
	    GLuint m_VBO;
		GLuint m_guiVBO;
		//GLuint m_mouseVBO;

		LightingTechnique* m_pEffect;
        Camera* m_pGameCamera;
		float m_scale;
		DirectionalLight m_directionalLight;
		
		ObjectManager m_oManager;
		UI m_UI;
		gui m_gui;
		Vector3f m_mousePos;
		Vector2f m_mouse2DPos;

		float eyeStep;
		float m_horoptor;
		float m_horoptorStep;

		Line* vcMouse[3];

		bool stereo;
		bool turnAround;

		Vector3f worldPos;

};



// ------------------------------------------------------------
//
//			Loading the image
//
// ------------------------------------------------------------

unsigned int ilLoadImage(std::string filename) {

	ILboolean success;
	unsigned int imageID;
 
	// init DevIL. This needs to be done only once per application
	ilInit();
	// generate an image name
	ilGenImages(1, &imageID); 
	// bind it
	ilBindImage(imageID); 
	// match image origin to OpenGL’s
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	// load  the image
	success = ilLoadImage((ILstring)filename.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	// check to see if everything went OK
	if (!success) {
		ilDeleteImages(1, &imageID); 
		return 0;
	}
	else return imageID;
}


// ------------------------------------------------------------
//
//			Prepare texture
//
// ------------------------------------------------------------

void prepareTexture(int w, int h, unsigned char* data) {

	

	/* Create and load texture to OpenGL */
	glGenTextures(1, &textureID); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D, textureID); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                w, h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE,
                data); 
	glGenerateMipmap(GL_TEXTURE_2D);
}


void showAtt(){
	
	printf("Width: %d, Height %d, Bytes per Pixel %d",
            ilGetInteger(IL_IMAGE_WIDTH),
            ilGetInteger(IL_IMAGE_HEIGHT),
            ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));
 
std::string s;
switch(ilGetInteger(IL_IMAGE_FORMAT)) {
    case IL_COLOR_INDEX      : s =  "IL_COLOR_INDEX"; break;    
    case IL_ALPHA            : s =  "IL_ALPHA"; break; 
    case IL_RGB              : s =  "IL_RGB"; break;    
    case IL_RGBA             : s =  "IL_RGBA"; break;    
    case IL_BGR              : s =  "IL_BGR"; break;    
    case IL_BGRA             : s =  "IL_BGRA"; break;    
    case IL_LUMINANCE        : s =  "IL_LUMINANCE"; break;    
    case  IL_LUMINANCE_ALPHA : s =  "IL_LUMINANCE_ALPHA"; break;
}
printf(" Format %s", s.c_str());
 
switch(ilGetInteger(IL_IMAGE_TYPE)) {
    case IL_BYTE           : s =  "IL_BYTE"; break;    
    case IL_UNSIGNED_BYTE  : s =  "IL_UNSIGNED_BYTE"; break;   
    case IL_SHORT          : s =  "IL_SHORT"; break;    
    case IL_UNSIGNED_SHORT : s =  "IL_UNSIGNED_SHORT"; break;    
    case IL_INT            : s =  "IL_INT"; break;    
    case IL_UNSIGNED_INT   : s =  "IL_UNSIGNED_INT"; break;    
    case IL_FLOAT          : s =  "IL_FLOAT"; break;    
    case IL_DOUBLE         : s =  "IL_DOUBLE"; break;
    case IL_HALF           : s =  "IL_HALF"; break;
}
printf(" Data type:  %s", s.c_str());


}

int main(int argc, char** argv){

	printf("START\n");

	int w,h,id;
	unsigned char* data;

	printf("STAGE LoadImage\n");
	// load image first so that window opens with image size
	id = ilLoadImage("textures.jpg");
	// image not loaded
	if (id == 0)
		return(2);
	
	printf("STAGE BindImage\n");

	ilBindImage(id);
	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	data = ilGetData();
	
	printf("STAGE GLUT 1/2\n");
    GLUTBackendInit(argc, argv);

	bool fullScreen = true;
	printf("STAGE GLUT 2/2\n");
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 60, fullScreen, "AngTest")){
        return 0x11;
    }
	printf("STAGE AngTest\n");
	AngTest* aTest = new AngTest();

	printf("STAGE Init\n");
	if (!aTest->Init()){
        return 0x12;
    }

	prepareTexture(w,h,data);
	//showAtt(); //just for showing off some pic data
	printf("STAGE Run\n");
	aTest->Run();

	printf("STAGE delete\n");
    delete aTest;
 
	printf("STOP\n");
    return 0;
}