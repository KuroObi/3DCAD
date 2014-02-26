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


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "ObjectManager.h"
#include "UI.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "gui.h"

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
			initGUI();
			
			m_mousePos = Vector3f(0.0, 0.0, 1.0);
			m_mouseVector = Vector3f(0.0f, 0.0f, 1.0f);
			m_mouse2DPos = Vector2f(0.0f, 0.0f);
			horoptor = 1.0f;

			initMouse();
			
			return true;
		}

	    void Run(){
			GLUTBackendRun(this);
		}

		virtual void RenderSceneCB(){

			if(stereo){
				horoptor = m_mouseVector.Dist(m_pGameCamera->GetPos() - worldPos);
				//horoptor = atanf(horoptor/eyeStep);
			}

			m_pGameCamera->OnRender();
			
			m_pEffect->SetGUI(0);
			RenderPhase();
			
			//Disable 3D-Things
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_CULL_FACE);
			
			m_pEffect->SetGUI(1);
			GuiPhase();
			
			//Reaktivate 3D-Things
			//glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			
			m_pEffect->SetGUI(0);
			MousePhase(); 

			glutSwapBuffers();
		}

		void MousePhase(){
			
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			if(stereo == true){
				glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_mouseVBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
			glDrawArrays(GL_LINES, 0, 6);
			
			if(stereo == true){
				m_pGameCamera->leftEye(horoptor);
				glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);

				glDrawArrays(GL_LINES, 0, 6);
			
				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			}

			glDisableVertexAttribArray(0);		
			glDisableVertexAttribArray(1);
			if(stereo == true){
				m_pGameCamera->rightEye(horoptor);
			}
		}

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
				glEnableVertexAttribArray(1);
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

			glDrawArrays(GL_POINTS,0, m_oManager.sector.numberOfPoints);
			glDrawArrays(GL_LINES, m_oManager.sector.numberOfPoints, m_oManager.sector.numberOfLines*2);
			glDrawArrays(GL_TRIANGLES, m_oManager.sector.numberOfPoints+m_oManager.sector.numberOfLines*2, m_oManager.sector.numberOfTriangles*3);
			
			glDrawBuffer(GL_BACK);

			if(stereo == true){
				m_pGameCamera->leftEye(horoptor);

				p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
				p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
				m_pEffect->SetWVP(p.GetTrans());
				m_pEffect->SetDirectionalLight(m_directionalLight);
				glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
				
				glDrawArrays(GL_POINTS,0, m_oManager.sector.numberOfPoints);
				glDrawArrays(GL_LINES, m_oManager.sector.numberOfPoints, m_oManager.sector.numberOfLines*2);
				glDrawArrays(GL_TRIANGLES, m_oManager.sector.numberOfPoints+m_oManager.sector.numberOfLines*2, m_oManager.sector.numberOfTriangles*3);

				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			}

			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
			if(stereo == true){
				glDisableVertexAttribArray(1);
				m_pGameCamera->rightEye(horoptor);
			}
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
							m_UI.changeDrawT(tQUAD);
							break;
						case 's':
							if(stereo){
								stereo = false;
							}else{
								stereo = true;
							}
							break;
					}
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
			initMouse();
		}

		virtual void MouseCB(int Button, int State, int x, int y){
			
			if (Button == 4 || Button == 3) {
				if(State == GLUT_DOWN){
					State = GLUT_UP;						
					if(Button == 3){				//Mouse Scroll up
						if(m_mouseVector.Dist(m_pGameCamera->GetPos() - worldPos) > 7.0f) {
							return;
						}
						m_mouseVector += m_mouseVector * 0.01f;
					}
					else{						//Mouse Scroll down
						if(m_mouseVector.Dist(m_pGameCamera->GetPos() - worldPos) <= 3.0f) {
							return;
						}
						m_mouseVector -= m_mouseVector * 0.01f;
					}

					calcWorldPos();
					initMouse();
					
					return;
				}
			}

			if (Button == GLUT_LEFT_BUTTON) {
				
				if(State == GLUT_DOWN){
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
							m_UI.changeDrawT(tQUAD);
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
					


					
					CreateVertexBuffer();
					State = GLUT_UP;
					
					return;
				}
			}
		}

		void calcWorldPos(){	
			m_mousePos = m_pGameCamera->GetPos() - worldPos;

			m_mousePos += (m_pGameCamera->GetTarget() * m_mouseVector.Dist(m_pGameCamera->GetPos() - m_mouseVector));
						
			m_mousePos.x += (m_mouse2DPos.x * 2.0f) * m_pGameCamera->GetTarget().z;
			m_mousePos.z += (m_mouse2DPos.x * 2.0f) * -m_pGameCamera->GetTarget().x;

			m_mousePos.y += (m_mouse2DPos.y * 1.0f);
		}


	private:

		void initGUI(){
			m_gui.init();
			int numberOfButtons = m_gui.numOfButtons;
			int sizeVertices = numberOfButtons* 4 * 12 * 2;
			Vertex * guiVert;
			guiVert = new Vertex[numberOfButtons * 4];

			for(int c = 0; c < numberOfButtons; c++){
				for(int v = 0; v < 4 ; v++){
					guiVert[c*4 + v] = Vertex(m_gui.button[c].vertex[v].getVector3f(),Vector3f(1.0f,0.0f,0.0f));
				}
			}
						
			glGenBuffers(1, &m_guiVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_guiVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVertices, guiVert, GL_STATIC_DRAW);

		}

		void initMouse(){
										
			Vertex * vcMouse;
			vcMouse = new Vertex[6];

			vcMouse[0] = Vertex(m_mousePos - Vector3f(0.05f,0.0f,0.0f) , Vector3f(1.0f,1.0f,1.0f));
			vcMouse[1] = Vertex(m_mousePos - Vector3f(-0.05f,0.0f,0.0f), Vector3f(1.0f,1.0f,1.0f));

			vcMouse[2] = Vertex(m_mousePos - Vector3f(0.0f,0.05f,0.0f) , Vector3f(1.0f,1.0f,1.0f));
			vcMouse[3] = Vertex(m_mousePos - Vector3f(0.0f,-0.05f,0.0f), Vector3f(1.0f,1.0f,1.0f));

			vcMouse[4] = Vertex(m_mousePos - Vector3f(0.0f,0.0f,0.05f) , Vector3f(1.0f,1.0f,1.0f));
			vcMouse[5] = Vertex(m_mousePos - Vector3f(0.0f,0.0f,-0.05f), Vector3f(1.0f,1.0f,1.0f));

			glGenBuffers(1, &m_mouseVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_mouseVBO);
			glBufferData(GL_ARRAY_BUFFER, 2 * 6 * 12, vcMouse, GL_DYNAMIC_DRAW);
		}

		void CreateVertexBuffer(){
			int numPoi = m_oManager.sector.numberOfPoints;
			int numLi = m_oManager.sector.numberOfLines;
			int numTri = m_oManager.sector.numberOfTriangles;
			int cVertes = 0;
			int sizeVertices = (numPoi + numLi*2 + numTri*3) * 12 * 2;
			//printf("%i",numLi);
			Vertex * Vertices;
			Vertices = new Vertex[numPoi + numLi*2 + numTri*3];
			
			Point * thisPoint = m_oManager.sector.HeadPoint;
			Line * thisLine = m_oManager.sector.HeadLine;
			Triangle * thisTriangle = m_oManager.sector.HeadTriangle;
			
			
			for(int cPoi = 0; cPoi < numPoi; cPoi++){
				Vertices[cVertes] = Vertex(	thisPoint->vertex[0].xyz,
											thisPoint->vertex[0].rgb);
				cVertes++;
				if(thisPoint->nextPoint != NULL){
					thisPoint = thisPoint->nextPoint;
				}
			}
			for(int cLi = 0; cLi < numLi; cLi++){
				for(int cThisVertice = 0; cThisVertice < 2; cThisVertice++){
					Vertices[cVertes] = Vertex(	thisLine->vertex[cThisVertice].xyz,
												thisLine->vertex[cThisVertice].rgb);
					cVertes++;
				}
				if(thisLine->nextLine == NULL){
					break;
				}else{
					thisLine = thisLine->nextLine;
				}
			}	
			for(int cTri = 0; cTri < numTri; cTri++){
				for(int cThisVertice = 0; cThisVertice < 3; cThisVertice++){
					Vertices[cVertes] = Vertex(	thisTriangle->vertex[cThisVertice].xyz,
												thisTriangle->vertex[cThisVertice].rgb);
					cVertes++;
				}
				if(thisTriangle->nextTriangle == NULL){
					break;
				}else{
					thisTriangle = thisTriangle->nextTriangle;
				}
			}	
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVertices, Vertices, GL_DYNAMIC_DRAW);
		}
		
	//Variables
	    GLuint m_VBO;
		GLuint m_guiVBO;
		GLuint m_mouseVBO;

		GLuint m_IBO;
		LightingTechnique* m_pEffect;
        Camera* m_pGameCamera;
		float m_scale;
		DirectionalLight m_directionalLight;
		
		ObjectManager m_oManager;
		UI m_UI;
		gui m_gui;
		Vector3f m_mousePos;
		Vector3f m_mouseVector;
		Vector2f m_mouse2DPos;

		float eyeStep;
		float horoptor;

		bool stereo;
		bool turnAround;

		Vector3f worldPos;
};

int main(int argc, char** argv){
    GLUTBackendInit(argc, argv);

	bool fullScreen = false;

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 60, fullScreen, "AngTest")){
        return 0x11;
    }

	AngTest* aTest = new AngTest();


	if (!aTest->Init()){
        return 0x12;
    }
	

	aTest->Run();

    delete aTest;
 
    return 0;
}

