#pragma once
#include "Player.h"


typedef struct Enemy
{
	CPlayer* mEnemy; // 2명의 Enemy가 있을것이다. 
	bool	Collision; // 내 플레이어와 충돌이 되었는지를 확인시켜준다.
	
};

class CAIPlayer 
	:public CPlayer
{
public:
	Enemy m_Enemy[2];
	float Distance[2];
	POINT movVector;


public:
	CAIPlayer(int nStatus);
	~CAIPlayer();
	virtual void GetEnemyPlayer(CPlayer* Enemy1, CPlayer* Enemy2)
	{
		m_Enemy[0].mEnemy = Enemy1; m_Enemy[1].mEnemy = Enemy2;
	}

	virtual void CalculateDistanceTimer(void);
	virtual void Playercollision(CPlayer **other, int player_num);
	virtual void PlayerAttack(void);
};

