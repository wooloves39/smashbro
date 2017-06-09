#include "Player.h"

CPlayer::CPlayer(int nStatus)
{
	nTexture = nStatus; // 현재 상태의 개수 
	m_ppTexture = new Image[nTexture];
	
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;

	m_dirRight.x = 1.0f;
	m_dirRight.y = 0.0f;

	m_dirUp.x = 0.0f;
	m_dirUp.y = 1.0f;

	n_AttackCount = 1;
	jumpUp = 1;

	m_BeforeState = BASIC_RIGHT;
	m_State = BASIC_RIGHT;
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

void CPlayer::SetTexture(int nIndex, LPCTSTR pCImage, int nSpriteCount)
{
	m_ppTexture[nIndex].Texture.Load(pCImage);
	m_ppTexture[nIndex].g_nSpriteX = nSpriteCount;
	m_ppTexture[nIndex].nSpriteCount = nSpriteCount;
	m_ppTexture[nIndex].g_nSpriteY = 1;
	m_ppTexture[nIndex].nSpriteCurrent = 0;
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
	if(jumpUp)
		m_Position.y -=  10.0f;
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
		//초기화
}



void CPlayer::DrawSprite(HDC hDC, int g_nSpriteCurrent)
{
	// 0605. 스프라이트 올려주는 타이머를 Player 클래스안에 넣을순없을까? 

	UINT nSpriteWidth = m_ppTexture[m_State].Texture.GetWidth() / m_ppTexture[m_State].g_nSpriteX;
	UINT nSpriteHeight = m_ppTexture[m_State].Texture.GetHeight() / m_ppTexture[m_State].g_nSpriteY;

	UINT xCoord = g_nSpriteCurrent % m_ppTexture[m_State].g_nSpriteX;
	UINT yCoord = g_nSpriteCurrent / m_ppTexture[m_State].g_nSpriteX;

	m_ppTexture[m_State].Texture.Draw(hDC
		, m_Position.x - nSpriteWidth / 2, m_Position.y - nSpriteHeight / 2, nSpriteWidth, nSpriteHeight
		, xCoord * nSpriteWidth, yCoord * nSpriteHeight
		, nSpriteWidth, nSpriteHeight
	);
}
