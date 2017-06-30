#pragma once
#include "stdafx.h"

class CCamera
{
private:
	POINT pos;
	POINT realpos;
public:
	void setPos(int x) { 
		if (x<= -700 || x >= 700);
		else {
			pos.x = x*(1280 / 940) - 640; pos.y = 0;
			realpos.x = x*(1280 / 940) - 640; realpos.y = 0;
		}
	}
	POINT getPos() {
		return pos;
	}
	void realsetPos(int x) {
		if (x <= -700 || x >= 700);
		else {
			realpos.x = x*(1280 / 940) - 640; realpos.y = 0;
		}
	}
	void add() {
	

			if ((realpos.x != pos.x))
			pos.x +=(realpos.x-pos.x)/20;
	/*	else if((realpos.x < pos.x))
			pos.x += realpos.x / (abs(realpos.x));
	*/}

	CCamera() {};
	~CCamera() {};
};

