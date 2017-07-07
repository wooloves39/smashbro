#pragma once
#include "stdafx.h"
#include"Camera.h"

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
public:
	POINT m_Position;
	POINT m_Velocity;//���ӵ�

	POINT m_dirRight;

	int nTexture;
	//���� ���� ������ �� ��ġ
	int smash_point = 3;
	int gage = 0;
public:

	Image	*m_ppTexture;
	UINT attack_SpriteCount;//������ �÷��̾��� ��ü �ε��� �ѹ�
	UINT attack_SpriteCurrent;//������ �÷��̾��� �� ���� �ε��� �ѹ�
	bool impact = false;//���� �������� �ƴ���
	bool impact_de = false;
	CImage rank_state;
	CImage UI;
	DWORD	m_State;			//������� 
	DWORD	m_BeforeState;		//�����ǻ��� 
	int		DIR;
	int		n_AttackCount;



	bool	FrameEnd;
	bool	m_bJump = false;


	System*	charSystem;
	Channel* pChannel;
	Sound* charSound[6];
	int JumpCount = 0;

public:
	CPlayer(int nStatus);
	~CPlayer();
	UINT Get_SPcount() { return m_ppTexture->nSpriteCount; }//���� ���� �ε��� �ѹ�
	UINT Get_SPcurrent() { return m_ppTexture->nSpriteCurrent; }//��ü ���� �ε��� �ʹ�
	void SetStatus(int state);
	DWORD GetStatus(void) { return m_State; };
	void SetPosition(float x, float y) {
		m_Position.x = x; m_Position.y = y;
	};
	void SetTexture(int nIndex, LPCTSTR pCImage, int nSpriteCount);
	POINT GetPosition() { return m_Position; };
	void Move(DWORD dwDirection, float fDistance);
	void MoveX(const POINT& d3dxvShift);
	void MoveY(const POINT& d3dxvShift);
	void MoveY(const float Yshift);
	void StateChangeX(void) { m_Velocity.x = 0.0; }
	void StateChangeY() {
		m_Velocity.y = 0.0;
	}
	void DrawSprite(HDC hDC, int g_nSpriteCurrent, CCamera cam);
	void DrawSprite(HDC hDC, int g_nSpriteCurrent, int x, int y);//ranking state���� �׷����� �͵鿡 ���� �Լ�
	void JumpTimer(void);
	void SetBasic(int State);
	void Render(HDC hDC);
	void Render(HDC hDC, int x, int y, int xsize, int ysize);//ranking state���� �׷����� �͵鿡 ���� �Լ�
//-------------- �����߰� ���⼭���� ----------//
	void SetImage(Image* Image) {
		m_ppTexture = Image;

	};
	Image* GetImage(void) { return m_ppTexture; };
	int damage_num = 100;
public:
	char damage[3];
	int PlayTime_num;
	char PlayTime[3];
	int ranking_num;
	char ranking[2];
	int total_score_num;
	char total_score[10];   //���� �����Ϳ� ��ü ����, ��ŷ->text�� �׷��� �ϱ⶧���� ���� �ε����� ����


	int low_power = 12;
	int High_power = 30;  //���� ������ ���� ������ ���� power
	int attacker_num;//���� �÷��̾��� �ѹ�
	bool mapobject_collsion = false;
	bool sma = false;
	POINT Vmov;
	bool hit = false;
	bool down = false;//�Ʒ� Ű�� ������ �������� ����� �� ���� �̿ϼ�
	bool live = true;//ȭ�鿡�� ���� �Ÿ� ������ ������ �װ� ���� ��� ���� �̱���
//----------- FUNCTION	DEFINE -------------//

public:
	virtual void KeyState(CCamera& cam, int state, int mode = 1, int player = 1);
	int getVelocity() { return m_Velocity.x; }
	int getDamege_num() { return damage_num; }
	void printdamege() { wsprintf(damage, TEXT("%d"), damage_num); }//������ ���� �Լ� �������� �÷��� ���� ���ŵȴ�.
	char* getDamege() { return damage; }
	double Vx(double P) { return sin(45 * RAD)*P; }
	double Vy(double P) { return cos(45 * RAD)*P; }
	void smashing(int damage, int power, bool smash);
	void gravity(void);

	int getAttacker() { return attacker_num; }//�����ڸ� �˰��ϴ� �Լ�
	void text_rank() {//ranking ȭ���� �Ǿ����� Ȱ��ȭ�Ǵ� �Լ�
		wsprintf(PlayTime, TEXT("%d"), PlayTime_num);
		wsprintf(ranking, TEXT("%d"), ranking_num);
		wsprintf(total_score, TEXT("%d"), total_score_num);
	}
	void attack(CPlayer **other, int player_num);
	void defance(CPlayer **other, int player_num);//�Ϻ� ������ �浹üũ 1���� �ϰ� ����
//bool Lowhit_num(); ���ϰ� �´� ��ŸȽ���� ����� ����
	int getSmashpoint() { return smash_point; }
	void smashsub() { if (smash_point > 0)--smash_point; }
	void smashadd() {
		if (gage >= 10) {
			if (smash_point < 3)
				++smash_point;
			gage = 0;
		}
	}
	int getgage() { return gage; }
	virtual void Playercollision(CPlayer **other, int player_num);
	void release() {

		delete[] m_ppTexture;
		rank_state.Destroy();
		UI.Destroy();
	}
	virtual bool AI() {
		return false;
	}
	virtual void distance(CPlayer **other, int player_num) {}
};
class CAIPlayer
	:public CPlayer
{
public:
	float Distance[2];
	POINT movVector;
	bool	Collision;
	CPlayer* target;
	bool targeting = false;
public:

	CAIPlayer(int nStatus) :CPlayer(nStatus) {
		nTexture = nStatus; // ���� ������ ���� 
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
	virtual void KeyState(CCamera& cam, int state, int mode = 1, int player = 1) {
		if (state == 3) {
			if (m_State != FLY_LEFT&& m_State != FLY_RIGHT&& m_State != DYE_LEFT && m_State != DYE_RIGHT && m_State != UP_LEFT && m_State != UP_RIGHT)
			{
				static bool smash = false;
				static bool attack = false;
				static bool jump = false;

				DWORD dwDirection = 0;
				if (targeting == false)return;
				if (target->GetPosition().x + 10 < GetPosition().x)
				{
					DIR = 1;
					dwDirection |= DIR_LEFT;

					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
					{

					}
					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
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
				if (target->GetPosition().x - 10 > GetPosition().x) {
					DIR = 2;
					dwDirection |= DIR_RIGHT;
					if (GetStatus() == DEFENSE_LEFT || GetStatus() == DEFENSE_RIGHT)
					{
					}

					else if (GetStatus() == ATTACK1_LEFT || GetStatus() == ATTACK2_LEFT ||
						GetStatus() == ATTACK1_RIGHT || GetStatus() == ATTACK2_RIGHT)
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
				if (target->GetPosition().y + 30 < GetPosition().y&&jump == false) {
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
				if (dwDirection)
				{
					Move(dwDirection, 20.0f);
				}
				FrameEnd = false;

			}
		}
	}
	//���������� ���ϱ�

	virtual void distance(CPlayer **other, int player_num) {
		int num = 0;
		int player[2];
		for (int i = 0; i < player_num; ++i) {
			if (other[i] == this);
			else {
				Distance[num] = sqrt((pow(m_Position.x - other[i]->GetPosition().x, 2) + pow(m_Position.y - other[i]->GetPosition().y, 2)));
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
		}
		targeting = true;
	}
	virtual bool AI() {
		return true;
	}
};

