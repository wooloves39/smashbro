#pragma once
#include "stdafx.h"

typedef struct Image
{
	CImage Texture;
	UINT	nSpriteCount;		// ��������Ʈ ��ü �ε���
	UINT	nSpriteCurrent;		// ���� ��������Ʈ �ε���
	UINT	g_nSpriteX;			// ��������Ʈ ����
	UINT	g_nSpriteY;			// ��������Ʈ ����
};

class CPlayer
{
protected:
	POINT m_Position;
	POINT m_Velocity;
	POINT m_Gravity;
	POINT m_dirRight;
	POINT m_dirUp;

	int nTexture;
	//CImage *m_ppTexture;
	bool jumpUp;
	char damage[10];
	int damage_num = 0;
	int power;

public: //�� �ΰ��� main�� ī���Ϳ��� �÷��־���ϹǷ� public���� �����Ѵ�. 
	//UINT			g_nSpriteCount;		// ��������Ʈ ��ü �ε���
	//UINT			g_nSpriteCurrent;	// ���� ��������Ʈ �ε���
	Image	*m_ppTexture;
	DWORD	m_State; //������� 
	DWORD	m_BeforeState; // �����ǻ��� 
	int		n_AttackCount;

public:
	CPlayer(int nStatus);
	~CPlayer();
	bool mapobject_collsion = false;
	void SetStatus(int state);
	DWORD GetStatus(void) { return m_State; };

	int getDamege_num() { return damage_num; }
	void printdamege() { wsprintf(damage, TEXT("%d"), damage_num); }
	char* getDamege() { return damage; }


	void SetPosition(float x, float y) { m_Position.x = x; m_Position.y = y; };
	void SetTexture(int nIndex, LPCTSTR pCImage, int nSpriteCount);

	POINT GetPosition() { return m_Position; };

	//CImage* GetImage(int status) { return m_ppTexture; }

	void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	void Move(const POINT& d3dxvShift, bool bUpdateVelocity);
	void StateChange(void)
	{
		m_Velocity.x = 0.0;
		m_Velocity.y = 0.0;
	}

	void DrawSprite(HDC hDC, int g_nSpriteCurrent);
	void JumpTimer(void);

	double Vx(double P) {
		return sin(45 * RAD)*P;
	}
	double Vy(double P) {
		return cos(45 * RAD)*P;
	}
	double frameY(double Vy, double gravity) { return Vy - gravity; }
	double frameX(double Vx, double air_resistance) { return Vx - air_resistance; }
	bool sma = false;
	void smashing() {
		static POINT Vmov;
		static bool start = false;
		if (start == false) {
			Vmov.x = 10;
			Vmov.y = 10;
			start = true;
		}
		else {
			Vmov.x = frameX(Vmov.x, 1);
			if (Vmov.y>-4)
				Vmov.y = frameY(Vmov.y, 2);
		}
		Move(Vmov, false);
	}
	void gravity() {
		static POINT Vmov = { 0,0 };
		if (mapobject_collsion == false)
		{
			if (Vmov.y>-2)
				Vmov.y = frameY(Vmov.y, 1);
		}
		else {
			StateChange();
			Vmov.y = 0;
		}
		Move(Vmov, false);
	}
};

