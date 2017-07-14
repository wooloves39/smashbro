#pragma once
#include "stdafx.h"

class CCamera
{
private:
	POINT pos;
	POINT realpos;
	int mode;
public:
	void setCam(int game_mode) {
		mode = game_mode;
	}
	void setPos(int x);
	void setPos(int x1, int x2);
	POINT getPos() {
		return pos;
	}
	void realsetPos(int x);
	void realsetPos(int x1, int x2);
	void add();
	CCamera() {};
	~CCamera() {};
};

