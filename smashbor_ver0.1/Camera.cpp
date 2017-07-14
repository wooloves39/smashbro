#include "Camera.h"

void CCamera::setPos(int x) {
	if (x <= -700 || x >= 700);
	else {
		pos.x = x*(1280 / 940) - 640; pos.y = 0;
		realpos.x = x*(1280 / 940) - 640; realpos.y = 0;
	}
}
void CCamera::setPos(int x1, int x2) {
	pos.x = ((x1 + x2) / 2)*(1280 / 940) - 640; pos.y = 0;
	realpos.x = ((x1 + x2) / 2)*(1280 / 940) - 640; realpos.y = 0;
}
void CCamera::realsetPos(int x) {
	if (x <= -700 || x >= 700);
	else {
		realpos.x = x*(1280 / 940) - 640; realpos.y = 0;
	}
}
void CCamera::realsetPos(int x1, int x2) {
	realpos.x = ((x1 + x2) / 2)*(1280 / 940) - 640; realpos.y = 0;
}
void CCamera::add() {
	if ((realpos.x != pos.x))
		pos.x += (realpos.x - pos.x) / 20;
}