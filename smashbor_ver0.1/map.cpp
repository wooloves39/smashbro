#include "map.h"



map::map()
{
	System_Create(&mapSystem);
	mapSystem->init(1, FMOD_INIT_NORMAL, NULL);
}


map::~map()
{
}
