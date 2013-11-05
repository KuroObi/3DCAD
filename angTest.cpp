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

			worldPos = Vector3f(0.0f, 0.0f, 3.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, 0.05f);

			//genratePyramid(0.0f); //Dummy
			//genratePyramid(1.0f); //Dummy
			//generateRandomPoints(100000,4);

			//generateTestBLA();

			CreateVertexBuffer();
			//CreateIndexBuffer();

			m_pEffect = new LightingTechnique();

			if (!m_pEffect->Init()){
				return false;
			}

			m_pEffect->Enable();
	
			stereo = true;
			turnAround = false;

			initGUI();

			return true;
		}

	    void Run(){
			GLUTBackendRun(this);
		}

		virtual void RenderSceneCB(){
			m_pGameCamera->OnRender();
			
			m_pEffect->setboolGUI(0);
			RenderPhase();
			
			//Disable 3D-Things
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_CULL_FACE);
			
			m_pEffect->setboolGUI(1);
			GuiPhase();

			m_pEffect->setboolGUI(2);
			MousePhase(); //still unsed
			
			//Reaktivate 3D-Things
			//glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			
			glutSwapBuffers();
		}

		void MousePhase(){
		
		}

		void GuiPhase(){

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, m_guiVBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_QUADS, 0,m_gui.numOfButtons * 4);
			
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
		
			if(stereo == true)
				glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);


			glEnableVertexAttribArray(0);
			if(stereo == true)
				glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_POINTS,0, m_oManager.sector.numberOfPoints);
			glDrawArrays(GL_LINES, m_oManager.sector.numberOfPoints, m_oManager.sector.numberOfLines*2);
			glDrawArrays(GL_TRIANGLES, m_oManager.sector.numberOfPoints+m_oManager.sector.numberOfLines*2, m_oManager.sector.numberOfTriangles*3);

			glDrawBuffer(GL_BACK);
			if(stereo == true){
				m_pGameCamera->leftEye();

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
			glDisableVertexAttribArray(0);
			if(stereo == true){
				glDisableVertexAttribArray(1);
				m_pGameCamera->rightEye();
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
			switch (Key){
				case 'q':{
					exit(0);
					break;
				}
			}
		}


		virtual void PassiveMouseCB(int x, int y){
			m_pGameCamera->OnMouse(x, y);		
		}

		virtual void MouseCB(int Button, int State, int x, int y){
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
						case NOTHING:
							m_UI.draw(calcWorldPos(relativ_x, relativ_y), &m_oManager);
					}
					


					
					CreateVertexBuffer();
					State = GLUT_UP;
				}
			}
		}

		Vector3f calcWorldPos(float x, float y){
			
			float pi = (float) M_PI;

			float angleA = m_pGameCamera->m_AngleH*(pi/180.0f) + (x*45.0f)*(pi/180.0f);
			float angleB = m_pGameCamera->m_AngleV*(pi/180.0f) + (y*60.0f)*(pi/180.0f);
				
			Vector3f vp = m_pGameCamera->GetPos() - worldPos;

			Vector3f v(1.5f, 0.0f, 0.0f);

			float _x = cosf(angleA)*v.x + sinf(angleA)*v.z;
			float _y = v.y;
			float _z = -sinf(angleA)*v.x - cosf(angleA)*v.z;
			
			_x = _x + vp.x;
			_y = cosf(angleB)*_y + sinf(angleB)*_z + vp.y;
			_z = -sinf(angleB)*_y + cosf(angleB)*_z + vp.z;
			return Vector3f(_x, _y, _z);
		}


	private:

		void initGUI(){

			m_gui.init();
			int numberOfButtons = m_gui.numOfButtons;
			int sizeVertices = numberOfButtons* 4 * 12;
			Vector3f * guiVert;
			guiVert = new Vector3f[numberOfButtons * 4];

			for(int c = 0; c < numberOfButtons; c++){
				for(int v = 0; v < 4 ; v++){
					guiVert[c*4 + v] = m_gui.button[c].vertex[v].getVector3f();
				}
			}
						
			glGenBuffers(1, &m_guiVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_guiVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVertices, guiVert, GL_STATIC_DRAW);

		}

		void CreateVertexBuffer(){
			int numPoi = m_oManager.sector.numberOfPoints;
			int numLi = m_oManager.sector.numberOfLines;
			int numTri = m_oManager.sector.numberOfTriangles;
			int cVertes = 0;
			int sizeVertices = (numPoi + numLi*2 + numTri*3) * 12;

			Vector3f * Vertices;
			Vertices = new Vector3f[numPoi + numLi*2 + numTri*3];
			
			Point * thisPoint = m_oManager.sector.HeadPoint;
			Triangle * thisTriangle = m_oManager.sector.HeadTriangle;
			Line * thisLine = m_oManager.sector.HeadLine;
			
			for(int cPoi = 0; cPoi < numPoi; cPoi++){
				Vertices[cVertes] = Vector3f(thisPoint->vertex[0].getVector3f());
				cVertes++;
				if(thisPoint->nextPoint != NULL){
					thisPoint = thisPoint->nextPoint;
				}
			}
			for(int cLi = 0; cLi < numLi; cLi++){
				for(int cThisVertice = 0; cThisVertice < 2; cThisVertice++){
					Vertices[cVertes] = Vector3f(thisLine->vertex[cThisVertice].getVector3f());
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
					Vertices[cVertes] = Vector3f(thisTriangle->vertex[cThisVertice].getVector3f());
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

		GLuint m_IBO;
		LightingTechnique* m_pEffect;
        Camera* m_pGameCamera;
		float m_scale;
		DirectionalLight m_directionalLight;
		
		ObjectManager m_oManager;
		UI m_UI;
		gui m_gui;

		bool stereo;
		bool turnAround;

		Vector3f worldPos;
};

int main(int argc, char** argv){
    GLUTBackendInit(argc, argv);

	bool fullScreen = true;

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 60, fullScreen, "AngTest")){
        return 1;
    }

	AngTest* aTest = new AngTest();

	if (!aTest->Init()){
        return 1;
    }
    aTest->Run();

    delete aTest;
 
    return 0;
}

