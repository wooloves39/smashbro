#pragma once
#include "stdafx.h"
#include"Camera.h"

typedef struct Image
{
	CImage Texture;
	UINT	nSpriteCount;		// 스프라이트 전체 인덱스
	UINT	nSpriteCurrent;		// 현재 스프라이트 인덱스
	UINT	g_nSpriteX;			// 스프라이트 가로
	UINT	g_nSpriteY;			// 스프라이트 세로
};

class CPlayer
{
public:
	POINT m_Position;
	POINT m_Velocity;
	POINT m_Gravity;
	POINT m_dirRight;
	POINT m_dirUp;

	int nTexture;
	//강한 공격 게이지 및 수치
	int smash_point = 3;
	int gage = 0;
public:
	
	Image	*m_ppTexture;
	UINT attack_SpriteCount;//공격한 플레이어의 전체 인덱스 넘버
	UINT attack_SpriteCurrent;//공격한 플레이어의 그 시점 인덱스 넘버
	bool impact = false;//맞은 상태인지 아닌지
	bool impact_de = false;
	CImage rank_state;
	CImage UI;
	DWORD	m_State;			//현재상태 
	DWORD	m_BeforeState;		//과거의상태 
	int		DIR = 999.f;
	int		n_AttackCount;
	int		JumpPosY;			//점프하기전의 좌표값 저장하는 부분 
	float	JumpHeight;		//점프하는 높이 
	bool	UsingJumpFrame;	//점프 프레임을 사용하는지 알려주는 변수 
	bool	FrameEnd;
	bool	m_bJump = false;
	DWORD	m_dwJumpTime = GetTickCount();
	float	m_fTime = 0.f;
	System*	charSystem;
	Channel* pChannel;
	Sound* charSound[4];
	int JumpCount = 0;
	bool	JumpTimerOn = false;
public:
	CPlayer(int nStatus);
	~CPlayer();
	UINT Get_SPcount() { return m_ppTexture->nSpriteCount; }//현재 상태 인덱스 넘버
	UINT Get_SPcurrent() { return m_ppTexture->nSpriteCurrent; }//전체 상태 인덱스 너버
	void SetStatus(int state);
	DWORD GetStatus(void) { return m_State; };


	void SetPosition(float x, float y) { 
		m_Position.x = x; m_Position.y = y; };
	void SetTexture(int nIndex, LPCTSTR pCImage, int nSpriteCount);

	POINT GetPosition() { return m_Position; };
	void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	void Move(const POINT& d3dxvShift, bool bUpdateVelocity);
	void StateChangeX(void){m_Velocity.x = 0.0;}
	void StateChangeY(){m_Velocity.y = 0.0;}
	void DrawSprite(HDC hDC, int g_nSpriteCurrent, CCamera cam);
	void DrawSprite(HDC hDC, int g_nSpriteCurrent, int x, int y);//ranking state에서 그려지는 것들에 대한 함수
	void JumpTimer(void);
	void SetBasic(int State);
	void Render(HDC hDC);
	void Render(HDC hDC, int x, int y, int xsize, int ysize);//ranking state에서 그려지는 것들에 대한 함수
//-------------- 현우추가 여기서부터 ----------//
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
	char total_score[10];   //받은 데이터와 전체 점수, 랭킹->text로 그려야 하기때문에 각각 인덱스를 가짐


	int low_power = 12;
	int High_power = 30;  //약한 때림과 강한 때림에 따른 power
	int attacker_num;//때린 플레이어의 넘버
	bool mapobject_collsion = false;
	bool sma = false;
	POINT Vmov;
	bool hit = false;
	bool down = false;//아래 키를 누르면 내려가게 만드는 것 아직 미완성

	bool live = true;//화면에서 일정 거리 밖으로 나가면 죽게 만들 요소 아직 미구현
//----------- FUNCTION	DEFINE -------------//

public:
	int getVelocity() { return m_Velocity.x; }
	int getDamege_num() { return damage_num; }
	void printdamege() {  wsprintf(damage, TEXT("%d"), damage_num); }//데미지 갱신 함수 데미지는 플레이 도중 갱신된다.
	char* getDamege() { return damage; }
	double Vx(double P) { return sin(45 * RAD)*P; }
	double Vy(double P) { return cos(45 * RAD)*P; }
	double frameY(double Vy, double gravity) { return Vy - gravity; }
	double frameX(double Vx, double air_resistance) {
		if (Vx < 0)return Vx + air_resistance;
		else return Vx - air_resistance;
	}//방향에 따라 이동하는 범위를 바꿈
	void smashing(int damage, int power, bool smash);
	void gravity(void);

	int getAttacker() { return attacker_num; }//공격자를 알게하는 함수
	void text_rank() {//ranking 화면이 되었을때 활성화되는 함수
		wsprintf(PlayTime, TEXT("%d"), PlayTime_num);
		wsprintf(ranking, TEXT("%d"), ranking_num);
		wsprintf(total_score, TEXT("%d"), total_score_num);
	}
	//void Playercollision(CPlayer **other, int player_num);
	void attack(CPlayer **other, int player_num);
	void defance(CPlayer **other, int player_num);//일부 수정함 충돌체크 1번만 하게 구현
//bool Lowhit_num(); 약하게 맞는 연타횟수를 만들고 싶음

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
	virtual void GetEnemyPlayer(CPlayer* Enemy1, CPlayer* Enemy2)
	{}
	virtual void CalculateDistanceTimer(void) {};
	virtual void PlayerAttack(void) {};
	void release() {
	//	delete[] charSystem;
	//	delete[] pChannel;
	//	delete[] charSound;
		delete[] m_ppTexture;
		rank_state.Destroy();
		UI.Destroy();
	}
};



