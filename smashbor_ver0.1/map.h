#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Player.h"

//맵 전체 보완 6.15
class Tile {
private:
	CImage tileimage;
	int object_num;
	POINT *object_pos;//그려지는 좌표
					  //	POINT *object_realpos;//충돌할 영역
	RECT *object_realpos;//충돌할 영역
	POINT size;
	POINT realsize;
	int downbalance;

public:
	Tile() {}
	void load(LPCTSTR pstname) { tileimage.Load(pstname); }
	void setPos(int x, int y, int num) {
		object_pos[num].x = x; object_pos[num].y = y;
	}
	POINT getPos(int num) { return object_pos[num]; }
	void setobject(int num);
	void draw(HDC hdc, CCamera cam);
	void setsize(int xsize, int ysize) { size.x = xsize; size.y = ysize; }
	void setRealsize(int xsize, int ysize, int balance) {
		realsize.x = xsize, realsize.y = ysize;
		downbalance = balance;
	}
	RECT collisionPos(int num) { return object_realpos[num]; }
	int Get_ob_num() { return object_num; }
	~Tile() {}
};
class map
{
private:
	CImage background;
	Tile* tiles;
	int obnum;

public:
	System*	mapSystem;
	Sound* mapSound;
	map();
	void draw(HDC hdc, RECT rectView, CCamera cam, int stage);
	void load(int stage, RECT rectView);
	void collision(CPlayer& player);
	void release() {
		delete[] tiles;
		background.Destroy();

	}
	~map();
};
