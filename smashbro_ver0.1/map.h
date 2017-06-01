#pragma once
#include<Windows.h>
#include<atlimage.h>
using namespace std;
class Camera {
private:
	POINT pos;
public:
	void setPos() { pos.x = 0; pos.y = 0; }
	POINT getPos() {
		return pos;
	}
	void add(int x) {
		if (pos.x + x <= -230 || pos.x + x >= 230);
		else
			pos.x += x;
	}
};

class Tile {
private:
	CImage tileimage;
	int object_num;
	POINT *object_pos;//그려지는 좌표
	POINT *object_realpos;//충돌할 영역
	POINT size;
public:
	Tile() {}
	void load(LPCTSTR pstname) { tileimage.Load(pstname); }
	void setPos(int x, int y, int num) {
		object_pos[num].x = x; object_pos[num].y = y;
	}
	POINT getPos(int num) { return object_pos[num]; }
	void setobject(int num) {
		object_num = num;
		object_pos = new POINT[object_num];
		object_realpos = new POINT[object_num];
	}
	void draw(HDC hdc, Camera cam) {
		int pitch= tileimage.GetWidth();
		int height= tileimage.GetHeight();
		for (int i = 0; i < object_num; ++i) {
			tileimage.TransparentBlt(hdc, object_pos[i].x-size.x - cam.getPos().x*(1280/ 940)*3, object_pos[i].y - size.y + cam.getPos().y, size.x*2 ,  size.y*2,
				0,0,pitch,height ,RGB(64,72,96));
		}
	}
	void setsize(int xsize, int ysize) { size.x = xsize; size.y = ysize; }
	~Tile() {}
};
class map
{
private:
	CImage background;
	Tile* tiles;
	int obnum;
public:
	map();
	void draw(HDC hdc, RECT rectView, Camera cam, int stage)
	{
		background.Draw(hdc, 0, 0, rectView.right, rectView.bottom, 470 - 240 + cam.getPos().x, 0, 480, 270);
		for (int i = 0; i < obnum; ++i)
			tiles[i].draw(hdc, cam);
	}
	void load(int stage, RECT rectView) {

		switch (stage)
		{
		case 1:
			obnum = 2;
			tiles = new Tile[2];
			background.Load(TEXT("map\\map1\\background.bmp"));
			tiles[0].load(TEXT("map\\map1\\image1.bmp"));
			tiles[0].setobject(1);
			tiles[0].setPos(rectView.right / 2, rectView.bottom - 150, 0);
			tiles[0].setsize(400, 140);
			tiles[1].load(TEXT("map\\map1\\image2.bmp"));
			tiles[1].setobject(2);
			tiles[1].setPos(rectView.right / 3, rectView.bottom - 500, 0);
			tiles[1].setPos(rectView.right / 3 * 2, rectView.bottom - 500, 1);
			tiles[1].setsize(160,40);
			break;
		case 2:
			obnum = 4;
			tiles = new Tile[4];
			background.Load(TEXT("map\\map2\\background.bmp"));
			tiles[0].load(TEXT("map\\map2\\image1.bmp"));
			tiles[0].setobject(1);
			tiles[0].setPos(rectView.right / 2, rectView.bottom - 150, 0);
			tiles[0].setsize(500, 140);
			tiles[1].load(TEXT("map\\map2\\image2.bmp"));
			tiles[1].setobject(1);
			tiles[1].setPos(rectView.right / 3, rectView.bottom - 500, 0);
			tiles[1].setsize(150, 100);
			tiles[2].load(TEXT("map\\map2\\image3.bmp"));
			tiles[2].setobject(1);
			tiles[2].setPos(rectView.right / 3*2, rectView.bottom - 500, 0);
			tiles[2].setsize(150,100);
			tiles[3].load(TEXT("map\\map2\\image4.bmp"));
			tiles[3].setobject(2);
			tiles[3].setPos(rectView.right /2-50, rectView.bottom - 400, 0);
			tiles[3].setPos(rectView.right /2+50, rectView.bottom - 400, 1);
			tiles[3].setsize(100,20);
			break;
		case 3:
			obnum = 2;
			tiles = new Tile[2];
			background.Load(TEXT("map\\map3\\background.bmp"));
			tiles[0].load(TEXT("map\\map3\\image1.bmp"));
			tiles[0].setobject(1);
			tiles[0].setPos(rectView.right / 2, rectView.bottom - 300, 0);
			tiles[0].setsize(400, 400);
			tiles[1].load(TEXT("map\\map3\\image2.bmp"));
			tiles[1].setobject(1);
			tiles[1].setPos(rectView.right / 2, rectView.bottom - 500, 0);
			tiles[1].setsize(470, 67);
			break;
		case 4:
			obnum = 3;
			tiles = new Tile[3];
			background.Load(TEXT("map\\map4\\background.bmp"));
			tiles[0].load(TEXT("map\\map4\\image1.bmp"));
			tiles[0].setobject(1);
			tiles[0].setPos(rectView.right / 2, rectView.bottom - 250, 0);
			tiles[0].setsize(600,300);
			tiles[1].load(TEXT("map\\map4\\image2.bmp"));
			tiles[1].setobject(1);
			tiles[1].setPos(rectView.right / 2, rectView.bottom - 200, 0);
			tiles[1].setsize(300, 40);
			tiles[2].load(TEXT("map\\map4\\image3.bmp"));
			tiles[2].setobject(1);
			tiles[2].setPos(rectView.right / 2, rectView.bottom - 250, 0);
			tiles[2].setsize(150,40);
			break;
		case 5:
			obnum = 2;
			tiles = new Tile[2];
			background.Load(TEXT("map\\map5\\background.bmp"));
			tiles[0].load(TEXT("map\\map5\\image1.bmp"));
			tiles[0].setobject(1);
			tiles[0].setPos(rectView.right / 2, rectView.bottom - 50, 0);
			tiles[0].setsize(450,250);
			tiles[1].load(TEXT("map\\map5\\image2.bmp"));
			tiles[1].setobject(3);
			tiles[1].setPos(rectView.right / 4, rectView.bottom - 400, 0);
			tiles[1].setPos(rectView.right / 4*3, rectView.bottom - 400, 1);
			tiles[1].setPos(rectView.right / 2, rectView.bottom - 500, 2);
			tiles[1].setsize(100,50);
			break;
		default:
			break;
		}
	}
	~map();
};

