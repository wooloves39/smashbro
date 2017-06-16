#pragma once
#include "stdafx.h"

class CCamera
{
private:
	POINT pos;

public:
	void setPos(int x) { 
		if (x<= -700 || x >= 700);
		else {
			pos.x = x*(1280 / 940) - 640; pos.y = 0;
		}
	}
	POINT getPos() {
		return pos;
	}
	void add(int x) {
		if (pos.x + x <= -230 || pos.x + x >= 230);
		else
			pos.x += x;
	}

	CCamera() {};
	~CCamera() {};
};

