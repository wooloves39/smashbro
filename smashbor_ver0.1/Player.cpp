#include "Player.h"

CPlayer::CPlayer(int nStatus)
{
	nTexture = nStatus; // 현재 상태의 개수 
	m_ppTexture = new Image[nTexture];

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;

	m_dirRight.x = 1.0f;
	m_dirRight.y = 0.0f;



	n_AttackCount = 1;

	m_BeforeState = BASIC_RIGHT;
	m_State = BASIC_RIGHT;


	System_Create(&charSystem);
	charSystem->init(6, FMOD_INIT_NORMAL, NULL);

	charSystem->createSound("sound\\imfact\\hit.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[0]);
	charSystem->createSound("sound\\imfact\\Jump.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[1]);
	charSystem->createSound("sound\\imfact\\fly.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[2]);
	charSystem->createSound("sound\\imfact\\Game over.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[3]);
	charSystem->createSound("sound\\imfact\\bighit.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[4]);
	charSystem->createSound("sound\\imfact\\defense.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[5]);
	pChannel->setVolume(0.3);
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
		switch (m_State)
		{

		case ATTACK1_RIGHT:
		case ATTACK1_LEFT:
		case ATTACK2_RIGHT:
		case ATTACK2_LEFT:
		case KICK_RIGHT:
		case KICK_LEFT:
			charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[0], false, &pChannel);
			break;
		case HATTACK_LEFT:
		case HATTACK_RIGHT:
			charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[4], false, &pChannel);
			break;
		case FLY_LEFT:
		case FLY_RIGHT:
			charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[2], false, &pChannel);
			break;
		case DYE_LEFT:
		case DYE_RIGHT:
			charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[5], false, &pChannel);
			break;
		default:
			break;
		}
		
		StateChangeX();
		//StateChangeY();
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

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		POINT d3dxvShift;
		d3dxvShift.x = 0.0f;
		d3dxvShift.y = 0.0f;

		if (dwDirection & DIR_RIGHT)
		{
			d3dxvShift.x += m_dirRight.x * fDistance;
		}
		if (dwDirection & DIR_LEFT)
		{
			d3dxvShift.x -= m_dirRight.x * fDistance;
		}

		if (d3dxvShift.x != 0.0f)
			MoveX(d3dxvShift);
		if (d3dxvShift.y != 0.0f)
			MoveY(d3dxvShift);
	}
}


void  CPlayer::JumpTimer(void)//벡터로 바꾸기
{
	if (m_bJump == true)
	{
		m_Position.y -= 10;
		mapobject_collsion = false;
		m_Velocity.y = -40;

		m_bJump = false;
	}

}

void CPlayer::MoveX(const POINT& d3dxvShift)
{

	if (m_Velocity.x <= 5 && m_Velocity.x >= -5)
		m_Velocity.x += d3dxvShift.x;
	m_Position.x += m_Velocity.x;

	//초기화

}
void CPlayer::MoveY(const POINT& d3dxvShift)
{
	m_Velocity.y += d3dxvShift.y;
	m_Position.y += m_Velocity.y;
	//초기화

}
void CPlayer::MoveY(const float Yshift) {
	m_Velocity.y += Yshift;
	m_Position.y += m_Velocity.y;

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
	POINT HIT = { 0,0 };
	if (smash == true) {
		if (hit == false) {
			StateChangeX();
			StateChangeY();
			if (m_State % 2 == 1)
				m_Velocity.x = Vx(power*(damage / 50));
			else
				m_Velocity.x = -Vx(power*(damage / 50));
			m_Position.y -= 20;
			m_Velocity.y = -Vy(power*(damage / 50));
			hit = true;
		}
		else {
			if (m_State % 2 == 1)
				HIT.x = 1;
			else
				HIT.x = -1;
			HIT.y = 0;
			if (m_Velocity.y > 2) {
				hit = false;
				sma = false;
				if (m_State % 2 == 1)m_State = JUMP_LEFT;
				else m_State = JUMP_RIGHT;
				StateChangeX();
			}
		}
		MoveX(HIT);
	}
	else {
		if (hit == false) {
			if (m_State % 2 == 1)
				m_Velocity.x = Vx(power*(damage / 200));
			else
				m_Velocity.x = -Vx(power*(damage / 200));
			hit = true;
		}
		else {
			if (m_State % 2 == 1)
				HIT.x = 1;
			else
				HIT.x = -1;
			HIT.y = 0;
			if (m_State == UP_LEFT || m_State == UP_RIGHT)
			{
				hit = false;
				sma = false;
			}
		}
		MoveX(HIT);
	}

}
//중력 추가
void CPlayer::gravity(void) {
	POINT gravity = { 0,3 };
	if (mapobject_collsion == false)
	{
		if (m_Velocity.y < 20)
			MoveY(gravity);
		else
			MoveY(0.0f);
	}
	else {
		JumpCount = 0;
		StateChangeY();
	}
}
//킥추가
void CPlayer::defance(CPlayer **other, int player_num)
{
	POINT other_POS;

	if (impact_de == true) {
		attack_SpriteCurrent = other[attacker_num]->Get_SPcurrent();

		if (attack_SpriteCurrent == attack_SpriteCount || other[attacker_num]->GetStatus() < 6)
		{
			impact_de = false;
			impact = false;
		}
	}
	else {
		for (int i = 0; i < player_num; ++i) {
			other_POS.x = other[i]->GetPosition().x;
			other_POS.y = other[i]->GetPosition().y;
			if (this == other[i])continue;
			else if ((m_Position.y + 20 > other_POS.y) && (m_Position.y - 20 < other_POS.y))//버튼 한번당 한번만 적용되게 만들고 싶다
			{
				StateChangeX();
				switch (other[i]->GetStatus())
				{
				case HATTACK_RIGHT:
					if (m_Position.x - 100 < other_POS.x&&m_Position.x > other_POS.x) {
						gage += 4;
						attacker_num = i;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();

						impact_de = true;
						if (GetStatus() == 15) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(FLY_LEFT);
						}
					}
					break;
				case HATTACK_LEFT:
					if (m_Position.x + 100 > other_POS.x&&m_Position.x < other_POS.x) {
						attacker_num = i;
						gage += 4;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact_de = true;
						if (GetStatus() == 14) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(FLY_RIGHT);
						}
					}
					break;
				case ATTACK1_RIGHT:
					if (m_Position.x - 80 < other_POS.x&&m_Position.x > other_POS.x) {
						attacker_num = i;
						gage += 2;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact_de = true;
						if (GetStatus() == 15) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(DYE_LEFT);
						}
					}
					break;
				case ATTACK1_LEFT:
					if (m_Position.x + 80 > other_POS.x&&m_Position.x < other_POS.x) {
						attacker_num = i;
						gage += 2;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact_de = true;
						if (GetStatus() == 14) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(DYE_RIGHT);
						}
					}
					break;
				case KICK_RIGHT:
					if (m_Position.x - 90 < other_POS.x&&m_Position.x > other_POS.x) {
						attacker_num = i;
						gage += 2;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact_de = true;
						if (GetStatus() == 15) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(DYE_LEFT);
						}
					}
					break;
				case KICK_LEFT:
					if (m_Position.x + 90 > other_POS.x&&m_Position.x < other_POS.x) {
						attacker_num = i;
						gage += 2;
						attack_SpriteCount = other[attacker_num]->Get_SPcount();
						impact_de = true;
						if (GetStatus() == 14) {
							m_ppTexture[GetStatus()].nSpriteCurrent = 1;
						}
						else {
							sma = true;
							SetStatus(DYE_RIGHT);
						}
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
	static int myindex;
	POINT other_POS;
	for (int i = 0; i < player_num; ++i) {
		other_POS = other[i]->GetPosition();
		if (other[i]->impact == false) {
			if (m_Position.x == other_POS.x&&m_Position.y == other_POS.y) { myindex = i; continue; }
			else if ((m_Position.y + 20 > other_POS.y) && (m_Position.y - 20 < other_POS.y))
			{
				switch (m_State)
				{
				case HATTACK_RIGHT:
					if (m_Position.x + 100 > other_POS.x&&m_Position.x < other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;
						if (other[i]->GetStatus() == DEFENSE_LEFT || other[i]->GetStatus() == DEFENSE_LEFT)
							damage_num += 5;
						else
							damage_num += 12;

					}
					break;
				case HATTACK_LEFT:
					if (m_Position.x - 100 < other_POS.x&&m_Position.x > other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;
						if (other[i]->GetStatus() == DEFENSE_RIGHT || other[i]->GetStatus() == DEFENSE_LEFT)
							damage_num += 5;
						else
							damage_num += 12;


					}
					break;
				case ATTACK1_RIGHT:
					if (m_Position.x + 80 > other_POS.x&&m_Position.x < other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;
						if (other[i]->GetStatus() == DEFENSE_LEFT || other[i]->GetStatus() == DEFENSE_RIGHT) {

							damage_num += 3;
						}
						else
							damage_num += 8;
						gage += 6;


					}
					break;
				case ATTACK1_LEFT:
					if (m_Position.x - 80 < other_POS.x&&m_Position.x > other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;

						if (other[i]->GetStatus() == DEFENSE_LEFT || other[i]->GetStatus() == DEFENSE_RIGHT)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;

					}
					break;
				case KICK_RIGHT:
					if (m_Position.x + 90 > other_POS.x&&m_Position.x < other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;
						if (other[i]->GetStatus() == DEFENSE_LEFT || other[i]->GetStatus() == DEFENSE_RIGHT)
							damage_num += 3;
						else
							damage_num += 8;
						gage += 6;

					}
					break;
				case KICK_LEFT:
					if (m_Position.x - 90 < other_POS.x&&m_Position.x > other_POS.x) {
						other[i]->attacker_num = myindex;
						other[i]->impact = true;
						if (other[i]->GetStatus() == DEFENSE_LEFT || other[i]->GetStatus() == DEFENSE_RIGHT)
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
		if (other[i] == this)continue;
		else {
			if ((m_Position.y + 50 > other_POS.y) && (m_Position.y - 50 < other_POS.y))
				if ((m_Position.x + 30 > other_POS.x) && (m_Position.x - 30 < other_POS.x))
					if (m_Position.x - other_POS.x > 0)m_Position.x += 30;
					else
						m_Position.x -= 30;
		}
	}
}
void CPlayer::KeyState(CCamera& cam, int state, int mode, int player) {
	if (state == 3)
	{
		if (m_State == FLY_LEFT || m_State == FLY_RIGHT || m_State == DYE_LEFT || m_State == DYE_RIGHT || m_State == UP_LEFT || m_State == UP_RIGHT);
		else
		{
			if (mode == 1) {
				static bool smash = false;
				static bool attack = false;
				static bool jump = false;

				DWORD dwDirection = 0;
				if (GetAsyncKeyState(VK_LEFT))
				{
					DIR = 1;
					dwDirection |= DIR_LEFT;

					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
					{

					}
					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
						GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
					{

					}
					else
					{

						if (mapobject_collsion == true)
							SetStatus(MOVE_LEFT);
						else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
						{

						}
						else
							SetStatus(JUMP_LEFT);
					}


				}
				if (GetAsyncKeyState(VK_RIGHT))
				{
					DIR = 2;
					dwDirection |= DIR_RIGHT;
					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
					{
					}

					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
						GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
					{

					}

					else
					{
						if (mapobject_collsion == true)
							SetStatus(MOVE_RIGHT);
						else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
						{

						}
						else
							SetStatus(JUMP_RIGHT);
					}
				}


				//	//점프시 

				if (GetAsyncKeyState('D'))
				{
					if (jump == false) {

						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
							return;
						if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
						{
							return;

						}


						else
						{

							if (m_bJump == false)
							{
								if (JumpCount != 2) {
									charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[1], false, &pChannel);
									m_bJump = true;


									JumpTimer();
									if (GetStatus() == BASIC_RIGHT || GetStatus() == JUMP_RIGHT)
										SetStatus(JUMP_RIGHT);
									else if (GetStatus() == BASIC_LEFT || GetStatus() == JUMP_LEFT)
										SetStatus(JUMP_LEFT);
									++JumpCount;
								}



							}
						}
					}
					jump = true;
				}
				else
					jump = false;
				if (GetAsyncKeyState('S'))
				{
					if (smash == false && GetStatus() != HATTACK_LEFT&& GetStatus() != HATTACK_RIGHT) {
						if (getSmashpoint() > 0)
						{
							if (GetStatus() % 2 == 0)
							{
								SetStatus(HATTACK_RIGHT);
							}
							if (GetStatus() % 2 == 1)
							{
								SetStatus(HATTACK_LEFT);
							}
							smashsub();

						}
					}
					smash = true;
				}
				else smash = false;
				if (GetAsyncKeyState('A'))
				{
					if (attack == false) {
						if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT)
						{
							SetStatus(ATTACK1_RIGHT);
						}
						else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT)
						{
							SetStatus(ATTACK1_LEFT);
						}

						else if (m_State == ATTACK1_RIGHT)
						{
							n_AttackCount = 2;
						}
						else if (m_State == ATTACK1_LEFT)
						{
							n_AttackCount = 2;
						}

						//점프중 공격시 발차기 
						else if (GetStatus() == JUMP_LEFT)
						{
							SetStatus(KICK_LEFT);
						}
						else if (GetStatus() == JUMP_RIGHT)
						{
							SetStatus(KICK_RIGHT);
						}
					}
					attack = true;
				}
				else attack = false;
				//막기
				if (GetAsyncKeyState('W'))
				{
					if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT || GetStatus() == DEFENSE_RIGHT)
					{
						SetStatus(DEFENSE_RIGHT);
					}
					else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT || GetStatus() == DEFENSE_LEFT)
					{
						SetStatus(DEFENSE_LEFT);
					}
					m_ppTexture[GetStatus()].nSpriteCurrent = 0;
				}
				if (dwDirection)
				{
					Move(dwDirection, 2.0f);
					cam.realsetPos(GetPosition().x);
				}
				FrameEnd = false;
			}

			else if (mode == 2) {
				if (player == 1) {
					static bool smash = false;
					static bool attack = false;
					static bool jump = false;

					DWORD dwDirection = 0;
					if (GetAsyncKeyState('A'))
					{
						DIR = 1;
						dwDirection |= DIR_LEFT;

						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
						{

						}
						else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
							GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
						{

						}
						else
						{

							if (mapobject_collsion == true)
								SetStatus(MOVE_LEFT);
							else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
							{

							}
							else
								SetStatus(JUMP_LEFT);
						}


					}
					if (GetAsyncKeyState('D'))
					{
						DIR = 2;
						dwDirection |= DIR_RIGHT;
						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
						{
						}

						else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
							GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
						{

						}

						else
						{
							if (mapobject_collsion == true)
								SetStatus(MOVE_RIGHT);
							else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
							{

							}
							else
								SetStatus(JUMP_RIGHT);
						}
					}


					//	//점프시 

					if (GetAsyncKeyState('H'))
					{
						if (jump == false) {

							if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
								return;
							if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
								GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
							{
								return;

							}


							else
							{

								if (m_bJump == false)
								{
									if (JumpCount != 2) {
										charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[1], false, &pChannel);
										m_bJump = true;


										JumpTimer();
										if (GetStatus() == BASIC_RIGHT || GetStatus() == JUMP_RIGHT)
											SetStatus(JUMP_RIGHT);
										else if (GetStatus() == BASIC_LEFT || GetStatus() == JUMP_LEFT)
											SetStatus(JUMP_LEFT);
										++JumpCount;
									}



								}
							}
						}
						jump = true;
					}
					else
						jump = false;
					if (GetAsyncKeyState('G'))
					{
						if (smash == false && GetStatus() != HATTACK_LEFT&& GetStatus() != HATTACK_RIGHT) {
							if (getSmashpoint() > 0)
							{
								if (GetStatus() % 2 == 0)
								{
									SetStatus(HATTACK_RIGHT);
								}
								if (GetStatus() % 2 == 1)
								{
									SetStatus(HATTACK_LEFT);
								}
								smashsub();

							}
						}
						smash = true;
					}
					else smash = false;
					if (GetAsyncKeyState('F'))
					{
						if (attack == false) {
							if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT)
							{
								SetStatus(ATTACK1_RIGHT);
							}
							else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT)
							{
								SetStatus(ATTACK1_LEFT);
							}

							else if (m_State == ATTACK1_RIGHT)
							{
								n_AttackCount = 2;
							}
							else if (m_State == ATTACK1_LEFT)
							{
								n_AttackCount = 2;
							}

							//점프중 공격시 발차기 
							else if (GetStatus() == JUMP_LEFT)
							{
								SetStatus(KICK_LEFT);
							}
							else if (GetStatus() == JUMP_RIGHT)
							{
								SetStatus(KICK_RIGHT);
							}
						}
						attack = true;
					}
					else attack = false;
					//막기
					if (GetAsyncKeyState('T'))
					{
						if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT || GetStatus() == DEFENSE_RIGHT)
						{
							SetStatus(DEFENSE_RIGHT);
						}
						else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT || GetStatus() == DEFENSE_LEFT)
						{
							SetStatus(DEFENSE_LEFT);
						}
						m_ppTexture[GetStatus()].nSpriteCurrent = 0;
					}
					if (dwDirection)
					{
						Move(dwDirection, 2.0f);
						cam.realsetPos(GetPosition().x);
					}
					FrameEnd = false;
				}
				else if (player == 2) {
					static bool smash = false;
					static bool attack = false;
					static bool jump = false;

					DWORD dwDirection = 0;
					if (GetAsyncKeyState(VK_LEFT))
					{
						DIR = 1;
						dwDirection |= DIR_LEFT;

						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
						{

						}
						else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
							GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
						{

						}
						else
						{

							if (mapobject_collsion == true)
								SetStatus(MOVE_LEFT);
							else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
							{

							}
							else
								SetStatus(JUMP_LEFT);
						}


					}
					if (GetAsyncKeyState(VK_RIGHT))
					{
						DIR = 2;
						dwDirection |= DIR_RIGHT;
						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
						{
						}

						else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
							GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
						{

						}

						else
						{
							if (mapobject_collsion == true)
								SetStatus(MOVE_RIGHT);
							else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
							{

							}
							else
								SetStatus(JUMP_RIGHT);
						}
					}


					//	//점프시 

					if (GetAsyncKeyState(VK_NUMPAD6))
					{
						if (jump == false) {

							if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
								return;
							if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
								GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
							{
								return;

							}


							else
							{

								if (m_bJump == false)
								{
									if (JumpCount != 2) {
										charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[1], false, &pChannel);
										m_bJump = true;


										JumpTimer();
										if (GetStatus() == BASIC_RIGHT || GetStatus() == JUMP_RIGHT)
											SetStatus(JUMP_RIGHT);
										else if (GetStatus() == BASIC_LEFT || GetStatus() == JUMP_LEFT)
											SetStatus(JUMP_LEFT);
										++JumpCount;
									}



								}
							}
						}
						jump = true;
					}
					else
						jump = false;
					if (GetAsyncKeyState(VK_NUMPAD5))
					{
						if (smash == false && GetStatus() != HATTACK_LEFT&& GetStatus() != HATTACK_RIGHT) {
							if (getSmashpoint() > 0)
							{
								if (GetStatus() % 2 == 0)
								{
									SetStatus(HATTACK_RIGHT);
								}
								if (GetStatus() % 2 == 1)
								{
									SetStatus(HATTACK_LEFT);
								}
								smashsub();

							}
						}
						smash = true;
					}
					else smash = false;
					if (GetAsyncKeyState(VK_NUMPAD4))
					{
						if (attack == false) {
							if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT)
							{
								SetStatus(ATTACK1_RIGHT);
							}
							else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT)
							{
								SetStatus(ATTACK1_LEFT);
							}

							else if (m_State == ATTACK1_RIGHT)
							{
								n_AttackCount = 2;
							}
							else if (m_State == ATTACK1_LEFT)
							{
								n_AttackCount = 2;
							}

							//점프중 공격시 발차기 
							else if (GetStatus() == JUMP_LEFT)
							{
								SetStatus(KICK_LEFT);
							}
							else if (GetStatus() == JUMP_RIGHT)
							{
								SetStatus(KICK_RIGHT);
							}
						}
						attack = true;
					}
					else attack = false;
					//막기
					if (GetAsyncKeyState(VK_NUMPAD8))
					{
						if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT || GetStatus() == DEFENSE_RIGHT)
						{
							SetStatus(DEFENSE_RIGHT);
						}
						else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT || GetStatus() == DEFENSE_LEFT)
						{
							SetStatus(DEFENSE_LEFT);
						}
						m_ppTexture[GetStatus()].nSpriteCurrent = 0;
					}
					if (dwDirection)
					{
						Move(dwDirection, 2.0f);

					}

					FrameEnd = false;
				}
			}
		}
	}
}

CAIPlayer::CAIPlayer(int nStatus) :CPlayer(nStatus) {
	nTexture = nStatus; // 현재 상태의 개수 
	m_ppTexture = new Image[nTexture];

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;

	m_dirRight.x = 1.0f;
	m_dirRight.y = 0.0f;



	n_AttackCount = 1;

	m_BeforeState = BASIC_RIGHT;
	m_State = BASIC_RIGHT;


	System_Create(&charSystem);
	charSystem->init(6, FMOD_INIT_NORMAL, NULL);

	charSystem->createSound("sound\\imfact\\hit.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[0]);
	charSystem->createSound("sound\\imfact\\Jump.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[1]);
	charSystem->createSound("sound\\imfact\\fly.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[2]);
	charSystem->createSound("sound\\imfact\\Game over.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[3]);
	charSystem->createSound("sound\\imfact\\bighit.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[4]);
	charSystem->createSound("sound\\imfact\\defense.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &charSound[5]);
	pChannel->setVolume(0.3);
}
void CAIPlayer::KeyState(CCamera& cam, int state, int mode, int player) {
	if (state == 3) {
		if (m_State == FLY_LEFT || m_State == FLY_RIGHT || m_State == DYE_LEFT || m_State == DYE_RIGHT || m_State == UP_LEFT || m_State == UP_RIGHT);
		else
		{
			static bool smash = false;
			static bool attack = false;
			static bool jump = false;
			if (mapobject_collsion == true)jump = false;
			DWORD dwDirection = 0;
			if (targeting == false)return;
			if (mode == 1) {

				if (target->GetPosition().x + 40 < GetPosition().x)
				{
					DIR = 1;
					dwDirection |= DIR_LEFT;

					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
					{

					}
					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
						GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT)
					{

					}
					else
					{

						if (mapobject_collsion == true)
							SetStatus(MOVE_LEFT);
						else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
						{

						}
						else
							SetStatus(JUMP_LEFT);
					}

				}
				if (target->GetPosition().x - 40 > GetPosition().x) {
					DIR = 2;
					dwDirection |= DIR_RIGHT;
					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT);
					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT ||
						GetStatus() == HATTACK_RIGHT || GetStatus() == HATTACK_LEFT);
					else
					{
						if (mapobject_collsion == true)
							SetStatus(MOVE_RIGHT);
						else if (GetStatus() == KICK_LEFT || GetStatus() == KICK_RIGHT)
						{

						}
						else
							SetStatus(JUMP_RIGHT);
					}
				}
			}
			else if (mode == 2) {
				if (target->GetPosition().y + 20 < GetPosition().y&&jump == false && target->mapobject_collsion == true) {
					cout << jump << " " << target->mapobject_collsion << endl;
					if (jump == false) {

						if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
							return;
						if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
							GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
						{
							return;
						}
						else
						{
							if (m_bJump == false)
							{
								if (JumpCount != 2) {
									charSystem->playSound(FMOD_CHANNEL_REUSE, charSound[1], false, &pChannel);
									m_bJump = true;
									JumpTimer();
									if (GetStatus() == BASIC_RIGHT || GetStatus() == JUMP_RIGHT)
										SetStatus(JUMP_RIGHT);
									else if (GetStatus() == BASIC_LEFT || GetStatus() == JUMP_LEFT)
										SetStatus(JUMP_LEFT);
									++JumpCount;
								}
							}
						}
					}
					jump = true;
				}
				else
					jump = false;
				if (GetPosition().x + 100 > target->GetPosition().x&&GetPosition().x - 100 < target->GetPosition().x) {
					if (GetPosition().y + 20 > target->GetPosition().y&&GetPosition().y - 20 < target->GetPosition().y) {
						if (getSmashpoint() > 0) {
							if (smash == false && GetStatus() != HATTACK_LEFT&& GetStatus() != HATTACK_RIGHT) {
								if (getSmashpoint() > 0)
								{
									if (GetStatus() % 2 == 0)
									{
										SetStatus(HATTACK_RIGHT);
									}
									if (GetStatus() % 2 == 1)
									{
										SetStatus(HATTACK_LEFT);
									}
									smashsub();
								}
								smash = true;
							}
							else smash = false;
						}
						else {
							if (attack == false) {
								if (GetStatus() == BASIC_RIGHT || GetStatus() == MOVE_RIGHT)
								{
									SetStatus(ATTACK1_RIGHT);
								}
								else if (GetStatus() == BASIC_LEFT || GetStatus() == MOVE_LEFT)
								{
									SetStatus(ATTACK1_LEFT);
								}

								else if (m_State == ATTACK1_RIGHT)
								{
									n_AttackCount = 2;
								}
								else if (m_State == ATTACK1_LEFT)
								{
									n_AttackCount = 2;
								}
								//점프중 공격시 발차기 
								else if (GetStatus() == JUMP_LEFT)
								{
									SetStatus(KICK_LEFT);
								}
								else if (GetStatus() == JUMP_RIGHT)
								{
									SetStatus(KICK_RIGHT);
								}
								attack = true;
							}
							else attack = false;
						}
					}
				}
			}




			if (dwDirection)
			{
				Move(dwDirection, 2.0f);
			}
			FrameEnd = false;

		}
	}
}
void CAIPlayer::distance(CPlayer **other, int player_num) {
	int num = 0;
	int player[2];
	int score[2] = { 0, 0 };

	for (int i = 0; i < player_num; ++i) {
		if (other[i] == this);
		else {
			Distance[num] = sqrt((pow(m_Position.x - other[i]->GetPosition().x, 2) + pow(m_Position.y - other[i]->GetPosition().y, 2)));
			score[num] += other[i]->getDamege_num() - Distance[num];
			player[num] = i;
			++num;
		}
	}
	if (Distance[0] < Distance[1] || other[player[1]]->live == false)
		target = other[player[0]];
	else
		target = other[player[1]];
	if (mapobject_collsion == false) {
		if (other[player[0]]->mapobject_collsion == true)target = other[player[0]];
		else if (other[player[1]]->mapobject_collsion == true)target = other[player[1]];
		else {
			targeting = false;
			return;
		}
	}
	targeting = true;
}