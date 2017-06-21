#pragma once
#include "Player.h"


typedef struct Enemy
{
	CPlayer* mEnemy; // 2���� Enemy�� �������̴�. 
	bool	Collision; // �� �÷��̾�� �浹�� �Ǿ������� Ȯ�ν����ش�.
	
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

