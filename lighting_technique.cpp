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


#include "lighting_technique.h"

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec3 Color;												\n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
uniform int gui;																	\n\
																					\n\
out vec4 frag_color;																\n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
	switch (gui){																	\n\
		case 0:																		\n\
			gl_Position = gWVP * vec4(Position, 1.0);		                        \n\
			frag_color = vec4(Color, 1.0);											\n\
			break;																	\n\
		case 1:																		\n\
			gl_Position = vec4(Position, 1.0);										\n\
			frag_color = vec4(Color, 1.0);											\n\
			break;																	\n\
	}																				\n\
}";

/*		next Step -> Textures
static const char* pVS0 = "                                                         \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoord0 = TexCoord;                                                           \n\
}";
*/

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 frag_color;																	\n\
                                                                                    \n\
out vec4 out_color;																	\n\
                                                                                    \n\
struct DirectionalLight                                                             \n\
{                                                                                   \n\
    vec3 Color;                                                                     \n\
    float AmbientIntensity;                                                         \n\
};                                                                                  \n\
                                                                                    \n\
uniform DirectionalLight gDirectionalLight;											\n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
	out_color = frag_color *														\n\
                vec4(gDirectionalLight.Color, 0.0f) *                               \n\
                gDirectionalLight.AmbientIntensity;                                 \n\
}";


LightingTechnique::LightingTechnique(){ 
}

bool LightingTechnique::Init(){
    if (!Technique::Init()){
		printf("Technique Error");
        return false;
    }

	
	if (!AddShader(GL_VERTEX_SHADER, pVS)){
        return false;
    }
	
    if (!AddShader(GL_FRAGMENT_SHADER, pFS)){
        return false;
    }
	
    if (!Finalize()){
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");
	m_GUILocation = GetUniformLocation("gui");
    m_dirLightColorLocation = GetUniformLocation("gDirectionalLight.Color");
    m_dirLightAmbientIntensityLocation = GetUniformLocation("gDirectionalLight.AmbientIntensity");

    if (m_dirLightAmbientIntensityLocation == 0xFFFFFFFF ||
        m_WVPLocation == 0xFFFFFFFF ||
        m_dirLightColorLocation == 0xFFFFFFFF||
		//m_MVectorLocation == 0xFFFFFFFF||
        m_GUILocation == 0xFFFFFFFF){
        return false;
    }

    return true;
}

void LightingTechnique::SetWVP(const Matrix4f& WVP){
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light){
    glUniform3f(m_dirLightColorLocation, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightAmbientIntensityLocation, Light.AmbientIntensity);
}

void LightingTechnique::SetGUI(int _gui){
	glUniform1i(m_GUILocation, _gui);
}