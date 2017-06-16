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

	JumpPosY = 0.0f;

	n_AttackCount = 1;

	m_BeforeState = BASIC_RIGHT;
	m_State = BASIC_RIGHT;

	JumpHeight = 50.0f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetStatus(int state)
{

	if (m_State != state)
	{
		m_BeforeState = m_State;
		m_State = state;
		StateChange();
	}
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
	if (m_Position.y > JumpPosY - JumpHeight)
		m_Position.y -= 10.0f;
	else
	{
		if (m_State == JUMP_LEFT) m_State = BASIC_LEFT;
		if (m_State == JUMP_RIGHT) m_State = BASIC_RIGHT;
	}
}

void CPlayer::Move(const POINT& d3dxvShift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		if (m_Velocity.x <= 10)
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

void CPlayer::SetBasic(int State)
{
	if (State % 2)//2로 나눈 나머지가 1이면 LEFT
		SetStatus(BASIC_LEFT);
	else //아니면 RIGHT
		SetStatus(BASIC_RIGHT);
}

void CPlayer::Render(HDC hDC)
{
	// 0605. 스프라이트 올려주는 타이머를 Player 클래스안에 넣을순없을까? 

	UINT nSprim_teWidth = m_ppTexture[m_State].Texture.GetWidth();
	UINT nSpriteHeight = m_ppTexture[m_State].Texture.GetHeight();

	m_ppTexture[m_State].Texture.Draw(hDC
		, m_Position.x, m_Position.y
	);
}
void CPlayer::Render(HDC hDC, int x, int y, int xsize, int ysize) {
}

void CPlayer::DrawSprite(HDC hDC, int g_nSpriteCurrent, CCamera cam)
{
	// 0605. 스프라이트 올려주는 타이머를 Player 클래스안에 넣을순없을까? 

	UINT nSpriteWidth = m_ppTexture[m_State].Texture.GetWidth() / m_ppTexture[m_State].g_nSpriteX;
	UINT nSpriteHeight = m_ppTexture[m_State].Texture.GetHeight() / m_ppTexture[m_State].g_nSpriteY;

	UINT xCoord = g_nSpriteCurrent % m_ppTexture[m_State].g_nSpriteX;
	UINT yCoord = g_nSpriteCurrent / m_ppTexture[m_State].g_nSpriteX;
	m_ppTexture[m_State].Texture.Draw(hDC
		, m_Position.x - nSpriteWidth / 2 - cam.getPos().x, m_Position.y - nSpriteHeight / 2 - cam.getPos().y*(1280 / 940) * 3, nSpriteWidth, nSpriteHeight
		, xCoord * nSpriteWidth, yCoord * nSpriteHeight
		, nSpriteWidth, nSpriteHeight
	);
}
void CPlayer::DrawSprite(HDC hDC, int g_nSpriteCurrent, int x, int y) {
	UINT nSpriteWidth = m_ppTexture[m_State].Texture.GetWidth() / m_ppTexture[m_State].g_nSpriteX;
	UINT nSpriteHeight = m_ppTexture[m_State].Texture.GetHeight() / m_ppTexture[m_State].g_nSpriteY;

	UINT xCoord = g_nSpriteCurrent % m_ppTexture[m_State].g_nSpriteX;
	UINT yCoord = g_nSpriteCurrent / m_ppTexture[m_State].g_nSpriteX;
	m_ppTexture[m_State].Texture.Draw(hDC
		, x, y, nSpriteWidth, nSpriteHeight
		, xCoord * nSpriteWidth, yCoord * nSpriteHeight
		, nSpriteWidth, nSpriteHeight
	);
}

//--------------현우--------------//
//보완
void CPlayer::smashing(int damage, int power, bool smash)
{
	if (smash == true) {
		if (hit == false) {
			if (m_State % 2 == 1)
				Vmov.x = Vx(power*(damage / 100));
			else
				Vmov.x = -Vx(power*(damage / 100));
			Vmov.y = Vy(power*(damage / 100));
			hit = true;
		}
		else {
			Vmov.x = frameX(Vmov.x, 1);
			cout << Vmov.y << endl;
			cout << Vy(power*(damage / 100))*0.4 << endl;
			if (Vmov.y > Vy(power*(damage / 100))*0.4)
				Vmov.y = frameY(Vmov.y, 2);
			else {
				Vmov.x = 0;
				Vmov.y = 0;
				//smash = false;
				hit = false;
				StateChange();
				sma = false;
				if (m_State % 2 == 1)m_State = JUMP_LEFT;
				else m_State = JUMP_RIGHT;
			}
		}

		Move(Vmov, false);
	}
	else {
		if (hit == false) {
			if (m_State % 2 == 1)
				Vmov.x = Vx(power*(damage / 100));
			else
				Vmov.x = -Vx(power*(damage / 100));
			Vmov.y = 0;
			hit = true;
		}
		else {
			Vmov.x = frameX(Vmov.x, 1);
			if (Vmov.y > Vy(power*(damage / 100)*0.4));
			else {
				Vmov.x = 0;
				Vmov.y = 0;
				//smash = false;
				hit = false;
				StateChange();
				sma = false;
			}
		}
		Move(Vmov, false);
	}

}
//중력 추가
void CPlayer::gravity(void) {
	static POINT Vmov = { 0,0 };
	if (mapobject_collsion == false)
	{
		if (Vmov.y > -6)
			Vmov.y = frameY(Vmov.y, 3);
	}
	else {
		StateChange();
		Vmov.y = 0;
	}
	Move(Vmov, false);
}
//킥추가
void CPlayer::defance(CPlayer **other, int player_num)
{
	POINT other_POS;
	if (impact == true) {
		attack_SpriteCurrent = other[attacker_num]->Get_SPcurrent();
	
		if (attack_SpriteCurrent == attack_SpriteCount||other[attacker_num]->GetStatus()<6)
			impact = false;
	}
	else {
		for (int i = 0; i < player_num; ++i) {
			other_POS.x = other[i]->GetPosition().x;
			other_POS.y = other[i]->GetPosition().y;
			if (m_Position.x == other_POS.x&&m_Position.y == other_POS.y)continue;
			else if ((m_Position.y + 20 > other_POS.y) && (m_Position.y - 20 < other_POS.y))//버튼 한번당 한번만 적용되게 만들고 싶다
			{
				switch (other[i]->GetStatus())
				{
				case HATTACK_RIGHT:
					if (m_Position.x - 100 < other_POS.x&&m_Position.x > other_POS.x) {

						sma = true;
						attacker_num = i;
						SetStatus(FLY_LEFT);
						gage += 4;
						cout << "강하게 맞음" << sma << attacker_num << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact = true;
					}
					return;
				case HATTACK_LEFT:
					if (m_Position.x + 100 > other_POS.x&&m_Position.x < other_POS.x) {
						sma = true;
						attacker_num = i;
						SetStatus(FLY_RIGHT);
						gage += 4;
						cout << "강하게 맞음" << sma << attacker_num << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact =true;
					}
					return;
				case ATTACK1_RIGHT:
					if (m_Position.x - 80 < other_POS.x&&m_Position.x > other_POS.x) {
						sma = true;
						attacker_num = i;
						SetStatus(DYE_LEFT);
						gage += 2;
						cout << "적당히 맞음" << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact = true;
					}
					break;
				case ATTACK1_LEFT:
					if (m_Position.x + 80 > other_POS.x&&m_Position.x < other_POS.x) {
						sma = true;
						attacker_num = i;
						SetStatus(DYE_RIGHT);
						gage += 2;
						cout << "적당히 맞음" << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact = true;
					}
					break;
				case KICK_RIGHT:
					if (m_Position.x - 90 < other_POS.x&&m_Position.x > other_POS.x) {
						sma = true;
						attacker_num = i;
						SetStatus(DYE_LEFT);
						gage += 2;
						cout << "적당히 맞음" << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact = true;
					}
					break;
				case KICK_LEFT:
					if (m_Position.x + 90 > other_POS.x&&m_Position.x < other_POS.x) {
						sma = true;
						attacker_num = i;
						SetStatus(DYE_RIGHT);
						gage += 2;
						cout << "적당히 맞음" << endl;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact = true;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}
//킥추가
void CPlayer::attack(CPlayer **other, int player_num) {
	POINT other_POS;
	for (int i = 0; i < player_num; ++i) {
		other_POS.x = other[i]->GetPosition().x;
		other_POS.y = other[i]->GetPosition().y;
		if (other[i]->impact == false) {
			if (m_Position.x == other_POS.x&&m_Position.y == other_POS.y)continue;
			else if ((m_Position.y + 20 > other_POS.y) && (m_Position.y - 20 < other_POS.y))
			{
				switch (m_State)
				{
				case HATTACK_RIGHT:
					if (m_Position.x + 100 > other_POS.x&&m_Position.x < other_POS.x) {
						cout << "강하게 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 5;
						else
							damage_num += 12;
					}
					break;
				case HATTACK_LEFT:
					if (m_Position.x - 100 < other_POS.x&&m_Position.x > other_POS.x) {
						cout << "강하게 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 5;
						else
							damage_num += 12;
					}
					break;
				case ATTACK1_RIGHT:
					if (m_Position.x + 80 > other_POS.x&&m_Position.x < other_POS.x) {
						cout << "적당히 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;
					}
					break;
				case ATTACK1_LEFT:
					if (m_Position.x - 80 < other_POS.x&&m_Position.x > other_POS.x) {
						cout << "적당히 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;
					}
					break;
				case KICK_RIGHT:
					if (m_Position.x + 90 > other_POS.x&&m_Position.x < other_POS.x) {
						cout << "적당히 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;
					}
					break;
				case KICK_LEFT:
					if (m_Position.x - 90 < other_POS.x&&m_Position.x > other_POS.x) {
						cout << "적당히 때림" << endl;
						if (other[i]->GetStatus() == 14 || other[i]->GetStatus() == 15)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void CPlayer::Playercollision(CPlayer **other, int player_num) {
	POINT other_POS;
	for (int i = 0; i < player_num; ++i)
	{
		other_POS.x = other[i]->GetPosition().x;
		other_POS.y = other[i]->GetPosition().y;
		//cout << other_POS.x << ' ' << other_POS.y << endl;
		if (m_Position.x == other_POS.x&&m_Position.y == other_POS.y)continue;
		else {
			if ((m_Position.y + 50 > other_POS.y) && (m_Position.y - 50 < other_POS.y))
				if ((m_Position.x + 30 > other_POS.x) && (m_Position.x - 30 < other_POS.x))
					if (m_Position.x - other_POS.x > 0)m_Position.x += 30;
					else
						m_Position.x -= 30;
		}
	}
}