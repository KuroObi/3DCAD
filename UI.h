

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