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
*/

#ifndef MATH_3D_H
#define	MATH_3D_H

#include <stdio.h>
#ifdef WIN32
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#define PI 3.14159265359f

#define ToRadian(x) (float)(((x) * PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / PI))

#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z) // dot product (3D) which allows vector operations in arguments
#define norm(v)     sqrt(dot(v,v))     // norm = length of  vector
#define d(u,v)      norm(u-v)          // distance = norm of difference

struct Vector2i{
    int x;
    int y;
};

struct Vector2f{
    float x;
    float y;

    Vector2f(){
    }

    Vector2f(float _x, float _y){
        x = _x;
        y = _y;
    }
};

struct Vector3f{
    float x;
    float y;
    float z;

    Vector3f(){
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();
	
	float Dist(Vector3f& v);

    void Rotate(float Angle, const Vector3f& Axis);

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f)\n", x, y, z);
    }
};

struct Vector4f{
    float x;
    float y;
    float z;
	float a;

    Vector4f(){
    }

    Vector4f(float _x, float _y, float _z, float _a){
        x = _x;
        y = _y;
        z = _z;
		a = _a;
    }
	
    Vector4f& operator+=(const Vector4f& _r){
        x += _r.x;
        y += _r.y;
        z += _r.z;
		a += _r.a;

        return *this;
    }

    Vector4f& operator-=(const Vector4f& _r){
        x -= _r.x;
        y -= _r.y;
        z -= _r.z;
		a -= _r.a;

        return *this;
    }

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f, %.02f)\n", x, y, z, a);
    }
};


inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}

struct Vertex3Drgb{
    float x, y, z;
    float r, g, b;

    Vertex3Drgb(){
    }

    Vertex3Drgb(float _x, float _y, float _z, float _r, float _g, float _b)
    {
        x = _x;
        y = _y;
        z = _z;
		r = _r;
		g = _g;
		b = _b;
    }

	Vertex3Drgb(Vector3f _xyz, Vector3f _rgb){
		x = _xyz.x;
        y = _xyz.y;
        z = _xyz.z;
		r = _rgb.x;
		g = _rgb.y;
		b = _rgb.z;
	}

    void Print() const
    {
        printf("(%.02f/%.02f/%.02f)[%f/%f/%f]\n", x,y,z, r,g,b);
    }
};


class Matrix4f{
	public:
		float m[4][4];

		Matrix4f(){        
		}


		inline void InitIdentity(){
			m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
			m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		}

		inline Matrix4f operator*(const Matrix4f& Right) const{
			Matrix4f Ret;

			for (unsigned int i = 0 ; i < 4 ; i++){
				for (unsigned int j = 0 ; j < 4 ; j++){
					Ret.m[i][j] = m[i][0] * Right.m[0][j] +
								  m[i][1] * Right.m[1][j] +
								  m[i][2] * Right.m[2][j] +
								  m[i][3] * Right.m[3][j];
				}
			}

			return Ret;
		}

		void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
		void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
		void InitTranslationTransform(float x, float y, float z);
		void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
		void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);
};


struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();  
 };

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

float calcDistPoiLin(Vector3f _vertic, Vector3f _L0, Vector3f L1);

#endif	/* MATH_3D_H */

