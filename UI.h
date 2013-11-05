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


#ifndef UI_H
#define	UI_H

#include "math_3d.h"
#include "ObjectManager.h"

enum drawType{
	tPOINT = 1,
	tLINE = 2,
	tTRI = 3,
	tSQUAR = 3,
	tQUAD = 3
};


class UI{
	public:
		UI();

		void changeDrawT(drawType _drawT);

		void draw(Vector3f _vertic, ObjectManager * p_oManager);




	private:
		drawType drawT;
		Vector3f drawingV[3];
		int	vCount;
};



#endif	/* UI_H */