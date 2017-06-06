#include "Player.h"

CPlayer::CPlayer(int nStatus)
{
	nTexture = nStatus; // ���� ������ ���� 
	m_ppTexture = new CImage[nTexture];

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;

	m_dirRight.x = 1.0f;
	m_dirRight.y = 0.0f;

	m_dirUp.x = 0.0f;
	m_dirUp.y = 1.0f;

	g_nSpriteX = 8;
	g_nSpriteY = 1;
	g_nSpriteCount = 8;
	g_nSpriteCurrent = 0;

	jumpUp = 1;

	m_BeforeState = MOVE_RIGHT;
	m_State = MOVE_RIGHT;
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetStatus(int state)
{
	m_BeforeState = m_State;
	m_State = state;
	if (m_BeforeState != m_State) StateChange();
}

void CPlayer::SetTexture(int nIndex, LPCTSTR pCImage)
{
	m_ppTexture[nIndex].Load(pCImage);
}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		POINT d3dxvShift;
		d3dxvShift.x = 0.0f;
		d3dxvShift.y = 0.0f;

		if (dwDirection & DIR_RIGHT)
		{
			d3dxvShift.x += m_dirRight.x * fDistance;
			d3dxvShift.y += m_dirRight.y * fDistance;
		}
		if (dwDirection & DIR_LEFT)
		{
			d3dxvShift.x -= m_dirRight.x * fDistance;
			d3dxvShift.y -= m_dirRight.y * fDistance;
		}
		if (dwDirection & DIR_UP)
		{
			d3dxvShift.x += m_dirUp.x * fDistance;
			d3dxvShift.y += m_dirUp.y * fDistance;
		}
		Move(d3dxvShift, bUpdateVelocity);
	}
}

void  CPlayer::JumpTimer(void)
{
	if (m_Position.y < 250)
	{
		jumpUp = 0;
	}
	if (jumpUp)
		m_Position.y -= 10.0f;
	else
		m_Position.y += 10.0f;

	if (jumpUp == 0 && m_Position.y > 300)
	{
		jumpUp = 1.0f;
		if (m_State == JUMP_LEFT) m_State = MOVE_LEFT;
		if (m_State == JUMP_RIGHT) m_State = MOVE_RIGHT;
	}
}

void CPlayer::Move(const POINT& d3dxvShift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_Velocity.x += d3dxvShift.x;
		m_Velocity.y += d3dxvShift.y;
	}
	else {
		m_Velocity.x += d3dxvShift.x;
		m_Velocity.y -= d3dxvShift.y;
	}
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	//�ʱ�ȭ
}



void CPlayer::DrawSprite(HDC hDC, int g_nSpriteCurrent, CCamera cam)
{
	// 0605. ��������Ʈ �÷��ִ� Ÿ�̸Ӹ� Player Ŭ�����ȿ� ������������? 

	UINT nSpriteWidth = m_ppTexture[0].GetWidth() / g_nSpriteX;
	UINT nSpriteHeight = m_ppTexture[0].GetHeight() / g_nSpriteY;

	UINT xCoord = g_nSpriteCurrent % g_nSpriteX;
	UINT yCoord = g_nSpriteCurrent / g_nSpriteX;

	m_ppTexture[m_State].Draw(hDC
		, m_Position.x - nSpriteWidth / 2, m_Position.y - nSpriteHeight / 2, nSpriteWidth, nSpriteHeight
		, xCoord * nSpriteWidth, yCoord * nSpriteHeight
		, nSpriteWidth, nSpriteHeight
	);
}