/*

	Copyright 2013 Etay Meiri/Christoph Schabert

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

#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include "technique.h"
#include "math_3d.h"

struct DirectionalLight{
    Vector3f Color;
    float AmbientIntensity;
};

class LightingTechnique : public Technique{
	public:

		LightingTechnique();

		virtual bool Init();

		void SetWVP(const Matrix4f& WVP);
		void SetDirectionalLight(const DirectionalLight& Light);
		void SetGUI(int _gui);
		void SetTextureUnit(unsigned int TextureUnit);

	private:

		GLuint m_WVPLocation;
		GLuint m_dirLightColorLocation;
		GLuint m_dirLightAmbientIntensityLocation;
		GLuint m_GUILocation;
		GLuint m_samplerLocation;
		//GLuint m_MVectorLocation;
};


#endif	/* LIGHTING_TECHNIQUE_H */