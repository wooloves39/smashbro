#include "AIPlayer.h"

CAIPlayer::CAIPlayer(int nStatus)
	: CPlayer(nStatus)
{
}


CAIPlayer::~CAIPlayer()
{
}

void CAIPlayer::CalculateDistanceTimer(void)
{
	for (int i = 0; i < 2; ++i) {

		Distance[i] = (m_Position.x - m_Enemy[i].mEnemy->GetPosition().x)*(m_Position.x - m_Enemy[i].mEnemy->GetPosition().x)
			+ (m_Position.y - m_Enemy[i].mEnemy->GetPosition().y)*(m_Position.y - m_Enemy[i].mEnemy->GetPosition().y);
		if (m_Enemy[i].mEnemy->GetPosition().y >= 500)
			Distance[i] = 100000;
	}

	if (Distance[0] > Distance[1])
	{
		movVector.x = m_Enemy[1].mEnemy->GetPosition().x - m_Position.x;
		movVector.y = m_Enemy[1].mEnemy->GetPosition().y - m_Position.y;
	}
	else if (Distance[0] < Distance[1])
	{
		movVector.x = m_Enemy[0].mEnemy->GetPosition().x - m_Position.x;
		movVector.y = m_Enemy[0].mEnemy->GetPosition().y - m_Position.y;
	}
	else {
		movVector.x = m_Enemy[0].mEnemy->GetPosition().x - m_Position.x;
		movVector.y = m_Enemy[0].mEnemy->GetPosition().y - m_Position.y;
	}
	
		if (movVector.x >= 0)
		{
			SetStatus(MOVE_RIGHT);
			m_Position.x += movVector.x / 10;
		}
		if (movVector.x < 0)
		{
			SetStatus(MOVE_LEFT);
			m_Position.x += movVector.x / 10;
		}
	
}






void CAIPlayer::Playercollision(CPlayer **other, int player_num)
{
	POINT other_POS;
	for (int i = 0; i < 2; ++i)
	{
		other_POS.x = m_Enemy[i].mEnemy->m_Position.x;
		other_POS.y = m_Enemy[i].mEnemy->m_Position.y;

		if (m_Position.x == other_POS.x && m_Position.y == other_POS.y)continue;
		else
		{
			if ((m_Position.y + 20 > other_POS.y) && (m_Position.y - 20 < other_POS.y))
				if ((m_Position.x + 30 > other_POS.x) && (m_Position.x - 30 < other_POS.x))
				{
					if (m_Position.x - other_POS.x > 0)
					{
						//오른쪽밀어내기
						m_Position.x += 30;
						m_Enemy[i].Collision = true;
					}
					else
					{
						m_Position.x -= 30;
						m_Enemy[i].Collision = true;
					}
				}
		}
	}
}


void CAIPlayer::PlayerAttack(void)
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_Enemy[i].Collision&&GetStatus()<4)
		{
			if (smash_point != 0) {
				if (GetStatus() % 2 == 0)
					SetStatus(HATTACK_RIGHT);
				else SetStatus(HATTACK_LEFT);
				--smash_point;
			}
			else {
				if (GetStatus() % 2 == 0)
				{
					SetStatus(ATTACK1_RIGHT);
				}
				else if (GetStatus() % 2 == 1)
				{
					SetStatus(ATTACK1_LEFT);
				}
			}
		}
	}
}





