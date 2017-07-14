#include "map.h"


void Tile::setobject(int num) {
	object_num = num;
	object_pos = new POINT[object_num];
	object_realpos = new RECT[object_num];
}
void Tile:: draw(HDC hdc, CCamera cam) {
	int pitch = tileimage.GetWidth();
	int height = tileimage.GetHeight();
	for (int i = 0; i < object_num; ++i) {
		tileimage.TransparentBlt(hdc, object_pos[i].x - size.x - cam.getPos().x, object_pos[i].y - size.y + cam.getPos().y, size.x * 2, size.y * 2,
			0, 0, pitch, height, RGB(64, 72, 96));
		object_realpos[i].left = object_pos[i].x - realsize.x;
		object_realpos[i].top = object_pos[i].y - realsize.y + downbalance;
		object_realpos[i].right = object_pos[i].x + realsize.x;
		object_realpos[i].bottom = object_pos[i].y + realsize.y + downbalance;
		}
}
map::map()
{
	System_Create(&mapSystem);
	mapSystem->init(1, FMOD_INIT_NORMAL, NULL);
}


map::~map()
{
}
void map::draw(HDC hdc, RECT rectView, CCamera cam, int stage)
{
	background.Draw(hdc, 0, 0, rectView.right, rectView.bottom, 470 - 240 + (cam.getPos().x + 640) / 3, 0, 480, 270);
	for (int i = 0; i < obnum; ++i)
		tiles[i].draw(hdc, cam);
}
void map::load(int stage, RECT rectView) {

	switch (stage)
	{
	case 1:
		obnum = 2;
		tiles = new Tile[2];
		background.Load(TEXT("map\\map1\\background.bmp"));
		tiles[0].load(TEXT("map\\map1\\image1.bmp"));
		tiles[0].setobject(1);
		tiles[0].setPos(0, rectView.bottom - 150, 0);
		tiles[0].setsize(400, 140);
		tiles[0].setRealsize(390, 20, -30);
		tiles[1].load(TEXT("map\\map1\\image2.bmp"));
		tiles[1].setobject(5);
		tiles[1].setPos(-200, rectView.bottom - 500, 0);
		tiles[1].setPos(200, rectView.bottom - 500, 1);
		tiles[1].setPos(0, rectView.bottom - 300, 2);
		tiles[1].setPos(450, rectView.bottom - 300, 3);
		tiles[1].setPos(-450, rectView.bottom - 300, 4);
		tiles[1].setsize(160, 40);
		tiles[1].setRealsize(150, 20, -10);
		mapSystem->createSound("sound\\map1.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &mapSound);
		break;
	case 2:
		obnum = 4;
		tiles = new Tile[4];
		background.Load(TEXT("map\\map2\\background.bmp"));
		tiles[0].load(TEXT("map\\map2\\image1.bmp"));
		tiles[0].setobject(1);
		tiles[0].setPos(0, rectView.bottom - 150, 0);
		tiles[0].setsize(500, 140);
		tiles[0].setRealsize(490, 20, 30);
		tiles[1].load(TEXT("map\\map2\\image2.bmp"));
		tiles[1].setobject(1);
		tiles[1].setPos(-200, rectView.bottom - 500, 0);
		tiles[1].setsize(150, 100);
		tiles[1].setRealsize(140, 20, 30);
		tiles[2].load(TEXT("map\\map2\\image3.bmp"));
		tiles[2].setobject(1);
		tiles[2].setPos(200, rectView.bottom - 500, 0);
		tiles[2].setsize(150, 100);
		tiles[2].setRealsize(140, 20, 30);
		tiles[3].load(TEXT("map\\map2\\image4.bmp"));
		tiles[3].setobject(6);
		tiles[3].setPos(-50, rectView.bottom - 400, 0);
		tiles[3].setPos(+50, rectView.bottom - 400, 1);
		tiles[3].setPos(+300, rectView.bottom - 300, 2);
		tiles[3].setPos(-300, rectView.bottom - 300, 3);
		tiles[3].setPos(+200, rectView.bottom - 200, 4);
		tiles[3].setPos(-200, rectView.bottom - 200, 5);
		tiles[3].setsize(100, 20);
		tiles[3].setRealsize(95, 20, -20);
		mapSystem->createSound("sound\\map2.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &mapSound);
		break;
	case 3:
		obnum = 3;
		tiles = new Tile[3];
		background.Load(TEXT("map\\map3\\background.bmp"));
		tiles[0].load(TEXT("map\\map3\\image1.bmp"));
		tiles[0].setobject(1);
		tiles[0].setPos(0, rectView.bottom - 300, 0);
		tiles[0].setsize(400, 400);
		tiles[0].setRealsize(390, 20, 220);
		tiles[1].load(TEXT("map\\map3\\image2.bmp"));
		tiles[1].setobject(1);
		tiles[1].setPos(0, rectView.bottom - 500, 0);
		tiles[1].setsize(470, 67);
		tiles[1].setRealsize(460, 20, 10);
		tiles[2].load(TEXT("map\\map3\\image3.bmp"));
		tiles[2].setobject(2);
		tiles[2].setPos(400, rectView.bottom - 200, 0);
		tiles[2].setPos(-400, rectView.bottom - 200, 1);
		tiles[2].setsize(100, 50);
		tiles[2].setRealsize(90, 20, -40);
		mapSystem->createSound("sound\\map3.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &mapSound);
		break;
	case 4:
		obnum = 3;
		tiles = new Tile[3];
		background.Load(TEXT("map\\map4\\background.bmp"));
		tiles[0].load(TEXT("map\\map4\\image1.bmp"));
		tiles[0].setobject(1);
		tiles[0].setPos(0, rectView.bottom - 250, 0);
		tiles[0].setsize(600, 300);
		tiles[0].setRealsize(590, 20, 170);
		tiles[1].load(TEXT("map\\map4\\image2.bmp"));
		tiles[1].setobject(1);
		tiles[1].setPos(0, rectView.bottom - 200, 0);
		tiles[1].setsize(300, 40);
		tiles[1].setRealsize(290, 20, -10);
		tiles[2].load(TEXT("map\\map4\\image3.bmp"));
		tiles[2].setobject(1);
		tiles[2].setPos(0, rectView.bottom - 250, 0);
		tiles[2].setsize(150, 40);
		tiles[2].setRealsize(145, 20, -10);
		mapSystem->createSound("sound\\map4.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &mapSound);
		break;
	case 5:
		obnum = 2;
		tiles = new Tile[2];
		background.Load(TEXT("map\\map5\\background.bmp"));
		tiles[0].load(TEXT("map\\map5\\image1.bmp"));
		tiles[0].setobject(1);
		tiles[0].setPos(0, rectView.bottom - 50, 0);
		tiles[0].setsize(450, 250);
		tiles[0].setRealsize(430, 20, -180);
		tiles[1].load(TEXT("map\\map5\\image2.bmp"));
		tiles[1].setobject(6);
		tiles[1].setPos(-250, rectView.bottom - 300, 0);
		tiles[1].setPos(250, rectView.bottom - 300, 1);
		tiles[1].setPos(-100, rectView.bottom - 500, 2);
		tiles[1].setPos(100, rectView.bottom - 500, 3);
		tiles[1].setPos(-400, rectView.bottom - 400, 4);
		tiles[1].setPos(400, rectView.bottom - 400, 5);
		tiles[1].setsize(100, 50);
		tiles[1].setRealsize(90, 20, -50);
		mapSystem->createSound("sound\\map5.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &mapSound);
		break;
	default:
		break;
	}
}
void map::collision(CPlayer& player) {
	player.mapobject_collsion = false;
	if (player.down == false && player.GetStatus() != FLY_LEFT&&player.GetStatus() != FLY_RIGHT) {
		for (int i = 0; i < obnum; ++i) {
			for (int j = 0; j < tiles[i].Get_ob_num(); j++) {
				if (player.GetPosition().x <= tiles[i].collisionPos(j).left)continue;
				if (player.GetPosition().x >= tiles[i].collisionPos(j).right)continue;
				if (player.GetPosition().y <= tiles[i].collisionPos(j).top - 30)continue;
				if (player.GetPosition().y >= tiles[i].collisionPos(j).bottom)continue;
				player.SetPosition(player.GetPosition().x, tiles[i].collisionPos(j).top - 20);
				if (player.GetStatus() == JUMP_LEFT || player.GetStatus() == JUMP_RIGHT ||
					player.GetStatus() == FLY_LEFT || player.GetStatus() == FLY_RIGHT) {
					player.SetStatus(player.GetStatus() % 2 + BASIC_RIGHT);
				}
				player.mapobject_collsion = true;//맵에 안붙어있을때
				player.m_bJump = false;
				return;
			}
		}
	}
}