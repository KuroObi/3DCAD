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
out vec3 V_Color;																	\n\
																					\n\
void main()                                                                         \n\
{                                                                                   \n\
	V_Color = Color;																\n\
	switch (gui){																	\n\
		case 0:																		\n\
			gl_Position = gWVP * vec4(Position, 1.0);		                        \n\
			break;																	\n\
		case 1:																		\n\
			gl_Position = vec4(Position, 1.0);										\n\
			break;																	\n\
	}																				\n\
}";

static const char* pFS = "															\n\
#version 330																		\n\
																					\n\
in vec3 V_Color;																	\n\
																					\n\
out vec4 FragColor;																	\n\
																					\n\
struct DirectionalLight																\n\
{																					\n\
vec3 Color;																			\n\
float AmbientIntensity;																\n\
};																					\n\
																					\n\
uniform DirectionalLight gDirectionalLight;											\n\
uniform int gui;																	\n\
uniform sampler2D gSampler;                                                         \n\
																					\n\
void main()																			\n\
{																					\n\
	switch(gui){																	\n\
		case 0:																		\n\
			FragColor = vec4(V_Color, 1.0) *										\n\
						vec4(gDirectionalLight.Color, 0.0f) *						\n\
						gDirectionalLight.AmbientIntensity;							\n\
			FragColor = vec4(V_Color, 1.0);					\n\
			break;																	\n\
		case 1:																		\n\
			FragColor = texture(gSampler, V_Color.xy);								\n\
			break;																	\n\
	}																				\n\
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
	if(m_WVPLocation == 0xFFFFFFFF )
		return false;
	m_GUILocation = GetUniformLocation("gui");
	if(m_GUILocation == 0xFFFFFFFF)
		return false;
    m_dirLightColorLocation = GetUniformLocation("gDirectionalLight.Color");
	if(m_dirLightColorLocation == 0xFFFFFFFF)
		return false;
    m_dirLightAmbientIntensityLocation = GetUniformLocation("gDirectionalLight.AmbientIntensity");
	if(m_dirLightAmbientIntensityLocation == 0xFFFFFFFF)
		return false;
    m_samplerLocation = GetUniformLocation("gSampler");
    if(m_samplerLocation == 0xFFFFFFFF)
        return false;
		
	glUniform1i(m_samplerLocation, 0);
	
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

void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
{
}