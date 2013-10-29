/*

	Copyright 2010 Etay Meiri

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

    Tutorial 15 - Camera Control - Part 2
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
#include "lighting_technique.h"
#include "glut_backend.h"

#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900

class AngTest : public ICallbacks{
	public:
		AngTest(){
			m_pGameCamera = NULL;
			m_scale = 0.0f;
		    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		    m_directionalLight.AmbientIntensity = 1.0f;	

			m_leftMouseButton.IsPressed = false;
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

			return true;
		}

	    void Run(){
			GLUTBackendRun(this);
		}

		virtual void RenderSceneCB(){
			m_pGameCamera->OnRender();
			
			MousePhase();
			RenderPhase();
			
			glutSwapBuffers();
		}

		void MousePhase(){
		
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
	        
			
			if (m_leftMouseButton.IsPressed) {
				
				float xx = (((float)m_leftMouseButton.x)/((float)WINDOW_WIDTH) - 0.5) * 2;
				float yy = (((float)m_leftMouseButton.y)/((float)WINDOW_HEIGHT) - 0.5) * -2;
				
				float angleA = m_pGameCamera->m_AngleH*(M_PI/180) + (xx*45)*(M_PI/180);
				float angleB = -m_pGameCamera->m_AngleV*(M_PI/180) + (yy*30)*(M_PI/180);
	
				Vector3f vp = m_pGameCamera->GetPos() -  p.getWorldPos();
	
				Vector3f v(1.5f, 0.0f, 0.0f);

				float _x = cosf(angleA)*v.x + sinf(angleA)*v.z;
				float _y = v.y;
				float _z = -sinf(angleA)*v.x - cosf(angleA)*v.z;
			
				float __x = _x;
				float __y = cosf(angleB)*_y + sinf(angleB)*_z;
				float __z = sinf(angleB)*_y + cosf(angleB)*_z;

				float f_x = __x+vp.x;
				float f_y = __y+vp.y;
				float f_z = __z+vp.z;
				
				m_pGameCamera->GetUp().Print();

				m_oManager.genarateSquare(f_x-0.05,f_y-0.05,f_z, f_x+0.05,f_y-0.05,f_z, f_x+0.05,f_y+0.05,f_z);

				CreateVertexBuffer();

				m_leftMouseButton.IsPressed = false;
			}
        

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
			switch (Key) {
				case 'q': {
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
            m_leftMouseButton.IsPressed = (State == GLUT_DOWN);
            m_leftMouseButton.x = x;
            m_leftMouseButton.y = y;
        }
    }

	private:

		void CreateVertexBuffer(){
			int numPoi = m_oManager.sector.numberOfPoints;
			int numLi = m_oManager.sector.numberOfLines;
			int numTri = m_oManager.sector.numberOfTriangles;
			int cVertes = 0;
			int sizeVertices = (numPoi + numLi*2 + numTri*3)*12;

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

		
		//Dummy
		void genratePyramid(float xVer){
			m_oManager.genarateTriangle(-1.0f+xVer, -1.0f, 0.5773f,
									0.0f+xVer, 1.0f, 0.0f,
									0.0f+xVer, -1.0f, -1.15475f);

			m_oManager.genarateTriangle(0.0f+xVer, -1.0f, -1.15475f,
									0.0f+xVer, 1.0f, 0.0f,
									1.0f+xVer, -1.0f, 0.5773f);

			m_oManager.genarateTriangle(1.0f+xVer, -1.0f, 0.5773f,
									0.0f+xVer, 1.0f, 0.0f,
									-1.0f+xVer, -1.0f, 0.5773f);

			m_oManager.genarateTriangle(-1.0f+xVer, -1.0f, 0.5773f,
									1.0f+xVer, -1.0f, 0.5773f,
									0.0f+xVer, -1.0f, -1.15475f);

			printf("Piramid generated! With xVer: %f\n",xVer);
		}
		void generateRandomPoints(int count, int range){
			int x,y,z;
			int con1 = (range*200)+1, con2 = range*100;
			for(int c = 1; c <= count;c++){
				x = rand()%con1-con2; y = rand()%con1-con2; z = rand()%con1-con2;
				m_oManager.genaratePoint(x*0.01f, y*0.01f, z*0.01f);
			}
		}

		void generateTestBLA(){
			//m_oManager.generateQuad(0.0f, 0.0f, 0.0f, 0.5f, -0.7f, 0.5f, 1.0f, 0.0f, 1.0f);
			m_oManager.generateQuad(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
			//m_oManager.genarateSquare(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			//m_oManager.genarateTriangle(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
			
			//m_oManager.genarateTriangle(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			//m_oManager.genarateTriangle(0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f);
		}


	//Variables
		struct {
			bool IsPressed;
			int x;
			int y;
		}m_leftMouseButton;

	    GLuint m_VBO;
		GLuint m_IBO;
		LightingTechnique* m_pEffect;
        Camera* m_pGameCamera;
		float m_scale;
		DirectionalLight m_directionalLight;
		
		ObjectManager m_oManager;
		
		bool stereo;
		bool turnAround;

};

/* #TODO: Update VertexBuffer --to--> IndexBuffer

static void CreateVertexBuffer()
{
    Vector3f Vertices[4];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.5773f);
    Vertices[1] = Vector3f(0.0f, -1.0f, -1.15475f);
    Vertices[2] = Vector3f(1.0f, -1.0f, 0.5773f);
    Vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 2, 1 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}
*/

int main(int argc, char** argv){
    GLUTBackendInit(argc, argv);

	bool fullScreen = false;

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

