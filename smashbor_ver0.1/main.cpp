#include "stdafx.h"
#include "Camera.h"
#include "Player.h"
#include "AIPlayer.h"
#include"map.h"

//06/17  �� ī�޶� H ��ü ��ü �ʿ� ���� �� ����� ���� �̹��� �Ҵ��س�����.
//����Ʈ �̹����� �������� ����.. ��Ź�帲
//ĳ���� ���� �Ҽ� �ְ� ��Ź
//��ü state�� rank�� �Ǹ� win,lose��  ĳ���� ��������Ʈ �Ҵ��Ҳ��� basic���� �ȵ��ư��� ���� ��Ź
//
int state = 0;
enum state { title, cho_map, cho_cha, play, ranking, ending };//������Ʈ ���� ����
enum DIRECTION_P
{
	DIRECTION_LEFT = 1,
	DIRECTION_RIGHT
};
CImage Title;
CImage Background;
CImage Choice_map[6];
CImage Choice_cha[3];
CImage Ending;
CImage Rankstate;
CImage mapEX[6];
POINT sel;
System*	pSystem;
Sound* stateSound[4];
Sound* choiceSound;
Sound* changeSound;
float stateVolume = 0.2f;
Channel*		pChannel[2];
bool	GameReady = false;
static UCHAR pKeyBuffer[256];

int PlayTime = 99;//�÷��� Ÿ��
char Playtime_t[3];
//-----Player�߰� 
int		  nowPlayer;	//���� ���� �����̰��ִ� �÷��̾�
int		  nPlayer;		//���� �÷����ϴ� ��� �÷��̾��� ��
CPlayer** m_Player;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
map m;
CCamera cam;
int map_stage;
CImage UI[3];
CImage demage_UI;
void Timer(void);
void setranking() {
	pChannel[0]->stop();
	pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[ranking-2], false, &pChannel[0]);
	state = ranking;
	for (int i = 0; i < 3; i++) {
		if (m_Player[i]->live == true)m_Player[i]->PlayTime_num = 99 - PlayTime;
		m_Player[i]->total_score_num = m_Player[i]->PlayTime_num*m_Player[i]->damage_num;
	}
	for (int i = 0; i < 3; ++i) {
	
		int rank = 1;
		for (int j = 0; j < 3; j++) {
			if (m_Player[i]->total_score_num < m_Player[j]->total_score_num)rank++;
		}
		m_Player[i]->ranking_num = rank;
		if (m_Player[i]->ranking_num == 1)m_Player[i]->SetStatus(WIN);
		else
			m_Player[i]->SetStatus(LOSE);
		m_Player[i]->text_rank();
	}
	
}
void KeyState(void)
{
	if (state == play) {
		static bool smash = false;
		static bool attack = false;

		DWORD dwDirection = 0;
		if (m_Player[0]->m_bJump == true)
		{
			if (m_Player[0]->JumpHeight > 0)
			{
				m_Player[0]->m_bJump = false;
				m_Player[0]->JumpHeight = 0.f;

				if (m_Player[0]->DIR == DIRECTION_LEFT)
					m_Player[0]->SetStatus(BASIC_LEFT);
				if (m_Player[0]->DIR == DIRECTION_RIGHT)
					m_Player[0]->SetStatus(BASIC_RIGHT);
			
			}

			else
			{
				m_Player[0]->m_fTime += 0.7f;
				m_Player[0]->JumpHeight += 6.f * -1 + (0.098f * m_Player[0]->m_fTime * m_Player[0]->m_fTime) / 2.f;
				
			}
			//��*-sin + (�߷°��ӵ� * t^2(�ð��� ����)) / 2;
		}



		if (GetAsyncKeyState(VK_LEFT))
		{
			m_Player[0]->DIR = DIRECTION_LEFT;
			dwDirection |= DIR_LEFT;

			if (m_Player[0]->GetStatus() == DEFENSE_LEFT || m_Player[0]->GetStatus() == DEFENSE_RIGHT)
			{

			}
			else if (m_Player[0]->GetStatus() == ATTACK1_LEFT || m_Player[0]->GetStatus() == ATTACK2_LEFT ||
				m_Player[0]->GetStatus() == ATTACK1_RIGHT || m_Player[0]->GetStatus() == ATTACK2_RIGHT)
			{

			}
			else
			{

				if (m_Player[0]->m_bJump != true)
					m_Player[0]->SetStatus(MOVE_LEFT);

				else
					m_Player[0]->SetStatus(JUMP_LEFT);
			}

		
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			m_Player[0]->DIR = DIRECTION_RIGHT;
			dwDirection |= DIR_RIGHT;
			if (m_Player[0]->GetStatus() == DEFENSE_LEFT || m_Player[0]->GetStatus() == DEFENSE_RIGHT)
			{
			}

			else if (m_Player[0]->GetStatus() == ATTACK1_LEFT || m_Player[0]->GetStatus() == ATTACK2_LEFT ||
				m_Player[0]->GetStatus() == ATTACK1_RIGHT || m_Player[0]->GetStatus() == ATTACK2_RIGHT)
			{

			}

			else
			{
				if (m_Player[0]->m_bJump != true)
					m_Player[0]->SetStatus(MOVE_RIGHT);

				else
					m_Player[0]->SetStatus(JUMP_RIGHT);
			}

			;
		}


		//	//������ 

		if (GetAsyncKeyState('D'))
		{
			m_Player[0]->JumpTimerOn = true;
			if (m_Player[0]->GetStatus() == DEFENSE_LEFT || m_Player[0]->GetStatus() == DEFENSE_RIGHT)
				return;
			if (m_Player[0]->GetStatus() == ATTACK1_LEFT || m_Player[0]->GetStatus() == ATTACK2_LEFT ||
				m_Player[0]->GetStatus() == ATTACK1_RIGHT || m_Player[0]->GetStatus() == ATTACK2_RIGHT)
			{
				return;

			}
			if (GameReady == false)
			{
			}

			else
			{

				if (m_Player[0]->m_bJump == false)
				{
					if (m_Player[0]->JumpCount != 2) {
						m_Player[0]->m_bJump = true;
						m_Player[0]->m_dwJumpTime = GetTickCount();
						m_Player[0]->m_fTime = 0.f;
						m_Player[0]->JumpHeight = 0.f;
						m_Player[0]->JumpPosY = m_Player[0]->GetPosition().y;

						if (m_Player[0]->GetStatus() == BASIC_RIGHT)
							m_Player[0]->SetStatus(JUMP_RIGHT);
						else if (m_Player[0]->GetStatus() == BASIC_LEFT)
							m_Player[0]->SetStatus(JUMP_LEFT);
						++m_Player[0]->JumpCount;
					}


				
				}
			}

		}
		if (GetAsyncKeyState('S'))
		{
			if (smash == false) {
				if (m_Player[0]->getSmashpoint() > 0)
				{
					if (m_Player[0]->GetStatus() % 2 == 0)
					{
						m_Player[0]->SetStatus(HATTACK_RIGHT);
					}
					if (m_Player[0]->GetStatus() % 2 == 1)
					{
						m_Player[0]->SetStatus(HATTACK_LEFT);
					}
					m_Player[0]->smashsub();
				
				}
				else;
			
			}
			smash = true;
		}
		else smash = false;
		if (GetAsyncKeyState('A'))
		{
			if (attack == false) {
				if (m_Player[0]->GetStatus() == BASIC_RIGHT || m_Player[0]->GetStatus() == MOVE_RIGHT)
				{
					m_Player[0]->SetStatus(ATTACK1_RIGHT);
				}
				else if (m_Player[0]->GetStatus() == BASIC_LEFT || m_Player[0]->GetStatus() == MOVE_LEFT)
				{
					m_Player[0]->SetStatus(ATTACK1_LEFT);
				}

				else if (m_Player[0]->m_State == ATTACK1_RIGHT)
				{
					m_Player[0]->n_AttackCount = 2;
				}
				else if (m_Player[0]->m_State == ATTACK1_LEFT)
				{
					m_Player[0]->n_AttackCount = 2;
				}

				//������ ���ݽ� ������ 
				else if (m_Player[0]->GetStatus() == JUMP_LEFT)
				{
					m_Player[0]->SetStatus(KICK_LEFT);
				}
				else if (m_Player[0]->GetStatus() == JUMP_RIGHT)
				{
					m_Player[0]->SetStatus(KICK_RIGHT);
				}
			}
			attack = true;
		}
		else attack = false;
		//����
		if (GetAsyncKeyState('W'))
		{
			if (m_Player[0]->GetStatus() == BASIC_RIGHT || m_Player[0]->GetStatus() == MOVE_RIGHT||m_Player[0]->GetStatus() ==DEFENSE_RIGHT )
			{
				m_Player[0]->SetStatus(DEFENSE_RIGHT);
			}
			else if (m_Player[0]->GetStatus() == BASIC_LEFT || m_Player[0]->GetStatus() == MOVE_LEFT || m_Player[0]->GetStatus() == DEFENSE_LEFT)
			{
				m_Player[0]->SetStatus(DEFENSE_LEFT);
			}
		}
		if (dwDirection)
		{
			m_Player[0]->Move(dwDirection, 2.0f, true);
			cam.setPos(m_Player[0]->GetPosition().x);
		}
		m_Player[0]->FrameEnd = false;
	}
}
void createSound() {
	System_Create(&pSystem);
	pSystem->init(6, FMOD_INIT_NORMAL, NULL);

	pSystem->createSound("sound\\opening.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[title]);
	pSystem->createSound("sound\\choice.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[cho_map]);
	
	pSystem->createSound("sound\\ranking.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[ranking-2]);
	pSystem->createSound("sound\\ending.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[ending-2]);
	pSystem->createSound("sound\\imfact\\choice.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &choiceSound);
	pSystem->createSound("sound\\imfact\\change.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &changeSound);
}

void reset() {
	pChannel[0]->stop();
	pChannel[1]->stop();
	m.release();
	sel.x = 140;
	sel.y = 650;
	map_stage = 1;
	for (int i = 0; i < 3; i++) {
		m_Player[i]->release();
	}
	m_Player[0]->SetPosition(-200, 300);
	m_Player[1]->SetPosition(200, 300);
	m_Player[2]->SetPosition(0, 300);
	PlayTime = 99;
	pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[cho_map], false, &pChannel[0]);
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	HINSTANCE g_hInst;//
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = TEXT("Smash Mario Brothers!");
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rcWindow = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow(TEXT("Smash Mario Brothers!"), TEXT("Smash Mario Brothers!"), WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);



	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		KeyState();
	}
	return Message.wParam;
}

//-----------------------------------------------------------//

void BuildPlayer(int nMyPlayer)
{
	
	CPlayer *Mario = new CPlayer(26);
	//BASIC 
	Mario->SetTexture(BASIC_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTBASIC.png"), 4);
	Mario->SetTexture(BASIC_LEFT,
		_T("character\\MARIO\\MARIO_LEFTBASIC.png"), 4);
	//RUN
	Mario->SetTexture(MOVE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTMOVE.png"), 8);
	Mario->SetTexture(MOVE_LEFT,
		_T("character\\MARIO\\MARIO_LEFTMOVE.png"), 8);
	//JUMP
	Mario->SetTexture(JUMP_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTJUMP.png"), 5);
	Mario->SetTexture(JUMP_LEFT,
		_T("character\\MARIO\\MARIO_LEFTJUMP.png"), 5);
	//ATTACK - STRONG
	Mario->SetTexture(HATTACK_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTSTRONGATTACK.png"), 6);
	Mario->SetTexture(HATTACK_LEFT,
		_T("character\\MARIO\\MARIO_LEFTSTRONGATTACK.png"), 6);
	//ATTACK 1 
	//1-1
	Mario->SetTexture(ATTACK1_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTATTACK1-1.png"), 4);
	Mario->SetTexture(ATTACK1_LEFT,
		_T("character\\MARIO\\MARIO_LEFTATTACK1-1.png"), 4);
	//1-2
	Mario->SetTexture(ATTACK2_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTATTACK1.png"), 4);
	Mario->SetTexture(ATTACK2_LEFT,
		_T("character\\MARIO\\MARIO_LEFTATTACK1.png"), 4);
	//1-3 ������ ����Ű������ �����°� 
	Mario->SetTexture(KICK_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTKICK.png"), 3);
	Mario->SetTexture(KICK_LEFT,
		_T("character\\MARIO\\MARIO_LEFTKICK.png"), 3);
	//DEFENSE
	Mario->SetTexture(DEFENSE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHT_DEF.png"), 2);
	Mario->SetTexture(DEFENSE_LEFT,
		_T("character\\MARIO\\MARIO_LEFT_DEF.png"), 2);
	//SIT
	Mario->SetTexture(SIT_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTSIT.png"), 3);
	Mario->SetTexture(SIT_LEFT,
		_T("character\\MARIO\\MARIO_LEFTSIT.png"), 3);
	//FLY (���ư��� ���)
	Mario->SetTexture(FLY_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHT_FLY.png"), 4);
	Mario->SetTexture(FLY_LEFT,
		_T("character\\MARIO\\MARIO_LEFT_FLY.png"), 4);
	//UP(�Ͼ�� ���)
	Mario->SetTexture(UP_LEFT,
		_T("character\\MARIO\\MARIO_LEFTUP.png"), 2);
	Mario->SetTexture(UP_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTUP.png"), 2);
	//DYE
	Mario->SetTexture(DYE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTDYE.png"), 3);
	Mario->SetTexture(DYE_LEFT,
		_T("character\\MARIO\\MARIO_LEFTDYE.png"), 3);

	//LOSE
	Mario->SetTexture(LOSE,
		_T("character\\MARIO\\MARIO_LOSE.png"), 2);
	//WIN
	Mario->SetTexture(WIN,
		_T("character\\MARIO\\MARIO_VICTORY.png"), 4);
	Mario->rank_state.Load("character\\MARIO\\marioUI.bmp");
	Mario->UI.Load("character\\MARIO\\mario_UI.bmp");
	

	//2. Wario
	CPlayer *Wario = new CPlayer(26);
	//BASIC
	Wario->SetTexture(BASIC_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTBASIC.png"), 4);
	Wario->SetTexture(BASIC_LEFT,
		_T("character\\WARIO\\WARIO_LEFTBASIC.png"), 4);
	//RUN
	Wario->SetTexture(MOVE_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTMOVE.png"), 8);
	Wario->SetTexture(MOVE_LEFT,
		_T("character\\WARIO\\WARIO_LEFTMOVE.png"), 8);
	//JUMP
	Wario->SetTexture(JUMP_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTJUMP.png"), 5);
	Wario->SetTexture(JUMP_LEFT,
		_T("character\\WARIO\\WARIO_LEFTJUMP.png"), 5);
	//ATTACK - STRONG
	Wario->SetTexture(HATTACK_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTSTRONGATTACK.png"), 6);
	Wario->SetTexture(HATTACK_LEFT,
		_T("character\\WARIO\\WARIO_LEFTSTRONGATTACK.png"), 6);
	//ATTACK 1 
	//1-1
	Wario->SetTexture(ATTACK1_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTATTACK1-1.png"), 4);
	Wario->SetTexture(ATTACK1_LEFT,
		_T("character\\WARIO\\WARIO_LEFTATTACK1-1.png"), 4);
	//1-2
	Wario->SetTexture(ATTACK2_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTATTACK1.png"), 4);
	Wario->SetTexture(ATTACK2_LEFT,
		_T("character\\WARIO\\WARIO_LEFTATTACK1.png"), 4);
	//1-3 ������ ����Ű������ �����°� 
	Wario->SetTexture(KICK_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTKICK.png"), 3);
	Wario->SetTexture(KICK_LEFT,
		_T("character\\WARIO\\WARIO_LEFTKICK.png"), 3);
	//DEFENSE
	Wario->SetTexture(DEFENSE_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHT_DEF.png"), 2);
	Wario->SetTexture(DEFENSE_LEFT,
		_T("character\\WARIO\\WARIO_LEFT_DEF.png"), 2);
	//SIT
	Wario->SetTexture(SIT_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTSIT.png"), 3);
	Wario->SetTexture(SIT_LEFT,
		_T("character\\WARIO\\WARIO_LEFTSIT.png"), 3);
	//FLY (���ư��� ���)
	Wario->SetTexture(FLY_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHT_FLY.png"), 4);
	Wario->SetTexture(FLY_LEFT,
		_T("character\\WARIO\\WARIO_LEFT_FLY.png"), 4);
	//UP(�Ͼ�� ���)
	Wario->SetTexture(UP_LEFT,
		_T("character\\WARIO\\WARIO_LEFTUP.png"), 2);
	Wario->SetTexture(UP_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTUP.png"), 2);
	//DYE
	Wario->SetTexture(DYE_LEFT,
		_T("character\\WARIO\\WARIO_LEFTDYE.png"), 3);
	Wario->SetTexture(DYE_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHTDYE.png"), 3);
	//LOSE
	Wario->SetTexture(LOSE,
		_T("character\\WARIO\\WARIO_LOSE.png"), 2);
	//WIN
	Wario->SetTexture(WIN,
		_T("character\\WARIO\\WARIO_VICTORY.png"), 4);

	Wario->rank_state.Load("character\\WARIO\\warioUI.bmp");
	Wario->UI.Load("character\\WARIO\\wario_UI.bmp");

	//3. LUIZY
	CPlayer *Luizy = new CPlayer(26);
	//BASIC
	Luizy->SetTexture(BASIC_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTBASIC.png"), 4);
	Luizy->SetTexture(BASIC_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTBASIC.png"), 4);
	//RUN
	Luizy->SetTexture(MOVE_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTMOVE.png"), 8);
	Luizy->SetTexture(MOVE_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTMOVE.png"), 8);
	//JUMP
	Luizy->SetTexture(JUMP_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTJUMP.png"), 5);
	Luizy->SetTexture(JUMP_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTJUMP.png"), 5);
	//ATTACK - STRONG
	Luizy->SetTexture(HATTACK_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTSTRONGATTACK.png"), 6);
	Luizy->SetTexture(HATTACK_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTSTRONGATTACK.png"), 6);
	//ATTACK 1 
	//1-1
	Luizy->SetTexture(ATTACK1_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTATTACK1-1.png"), 4);
	Luizy->SetTexture(ATTACK1_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTATTACK1-1.png"), 4);
	//1-2
	Luizy->SetTexture(ATTACK2_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTATTACK1.png"), 4);
	Luizy->SetTexture(ATTACK2_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTATTACK1.png"), 4);
	//1-3 ������ ����Ű������ �����°� 
	Luizy->SetTexture(KICK_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTKICK.png"), 3);
	Luizy->SetTexture(KICK_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTKICK.png"), 3);
	//DEFENSE
	Luizy->SetTexture(DEFENSE_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHT_DEF.png"), 2);
	Luizy->SetTexture(DEFENSE_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFT_DEF.png"), 2);
	//SIT
	Luizy->SetTexture(SIT_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTSIT.png"), 3);
	Luizy->SetTexture(SIT_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTSIT.png"), 3);
	//FLY (���ư��� ���)
	Luizy->SetTexture(FLY_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHT_FLY.png"), 4);
	Luizy->SetTexture(FLY_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFT_FLY.png"), 4);
	//UP(�Ͼ�� ���)
	Luizy->SetTexture(UP_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTUP.png"), 2);
	Luizy->SetTexture(UP_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTUP.png"), 2);
	//DYE
	Luizy->SetTexture(DYE_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFTDYE.png"), 3);
	Luizy->SetTexture(DYE_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHTDYE.png"), 3);
	//LOSE
	Luizy->SetTexture(LOSE,
		_T("character\\LUIZY\\LUIZY_LOSE.png"), 2);
	//WIN
	Luizy->SetTexture(WIN,
		_T("character\\LUIZY\\LUIZY_VICTORY.png"), 4);
	Luizy->rank_state.Load("character\\LUIZY\\luizyUI.bmp");
	Luizy->UI.Load("character\\LUIZY\\luizy_UI.bmp");
	m_Player = new CPlayer*[3];
	//1. Mario
	m_Player[0] = new CPlayer(26);
	m_Player[0]->SetPosition(-200, 300);
	m_Player[0]->SetStatus(BASIC_RIGHT);	//������� ���� 

	m_Player[1] = new CAIPlayer(26);
	m_Player[1]->SetPosition(200, 300);
	m_Player[1]->SetStatus(BASIC_LEFT);	//������� ���� 
	m_Player[1]->GetEnemyPlayer(m_Player[0], m_Player[2]);

	m_Player[2] = new CAIPlayer(26);
	m_Player[2]->SetPosition(0, 300);
	m_Player[2]->SetStatus(BASIC_LEFT);	//������� ���� 

	if (nMyPlayer == 0) {
		m_Player[0]->SetImage(Mario->GetImage());
		m_Player[0]->rank_state = Mario->rank_state;
		m_Player[0]->UI = Mario->UI;
		m_Player[1]->SetImage(Wario->GetImage());
		m_Player[1]->rank_state = Wario->rank_state;
		m_Player[1]->UI = Wario->UI;
		m_Player[2]->SetImage(Luizy->GetImage());
		m_Player[2]->rank_state=Luizy->rank_state;
		m_Player[2]->UI = Luizy->UI;
		m_Player[1]->GetEnemyPlayer(m_Player[0], m_Player[2]);
		m_Player[2]->GetEnemyPlayer(m_Player[0], m_Player[1]);
	}
	if (nMyPlayer == 1) {
		m_Player[1]->SetImage(Mario->GetImage());
		m_Player[1]->rank_state = Mario->rank_state;
		m_Player[1]->UI = Mario->UI;
		m_Player[2]->SetImage(Wario->GetImage());
		m_Player[2]->rank_state = Wario->rank_state;
		m_Player[2]->UI = Wario->UI;
		m_Player[0]->SetImage(Luizy->GetImage());
		m_Player[0]->rank_state = Luizy->rank_state;
		m_Player[0]->UI = Luizy->UI;
		m_Player[1]->GetEnemyPlayer(m_Player[0], m_Player[2]);
		m_Player[2]->GetEnemyPlayer(m_Player[0], m_Player[1]);
	}

	if (nMyPlayer == 2) {
		m_Player[1]->SetImage(Mario->GetImage());
		m_Player[1]->rank_state = Mario->rank_state;
		m_Player[1]->UI = Mario->UI;
		m_Player[0]->SetImage(Wario->GetImage());
		m_Player[0]->rank_state = Wario->rank_state;
		m_Player[0]->UI = Wario->UI;
		m_Player[2]->SetImage(Luizy->GetImage());
		m_Player[2]->rank_state = Luizy->rank_state;
		m_Player[2]->UI = Luizy->UI;

		m_Player[1]->GetEnemyPlayer(m_Player[0], m_Player[2]);
		m_Player[2]->GetEnemyPlayer(m_Player[0], m_Player[1]);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static POINT point;
	HDC hdc;
	PAINTSTRUCT ps;
	static HFONT TimeFont = CreateFont(70, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("HY������M"));
	static HFONT UIFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("HY������M"));//����ü

	static HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	static RECT rectView;
	static int stage_view = 0;
	DWORD dwDirection = 0;
	static int down = 0;
	switch (iMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		Title.Load(TEXT("sub_image\\titleUI.bmp"));
		
		Choice_cha[0].Load(TEXT("sub_image\\choice1.bmp"));
		Choice_cha[1].Load(TEXT("sub_image\\choice2.bmp"));
		Choice_cha[2].Load(TEXT("sub_image\\choice3.bmp"));
		Ending.Load(TEXT("sub_image\\end.bmp"));
		Background.Load(TEXT("sub_image\\background.bmp"));
		mapEX[0].Load(TEXT("map\\map1\\map1.bmp"));
		mapEX[1].Load(TEXT("map\\map2\\map2.bmp"));
		mapEX[2].Load(TEXT("map\\map3\\map3.bmp"));
		mapEX[3].Load(TEXT("map\\map4\\map4.bmp"));
		mapEX[4].Load(TEXT("map\\map5\\map5.bmp"));
		mapEX[5].Load(TEXT("sub_image\\random_map.bmp"));
		UI[0].Load(TEXT("sub_image\\mario_UI.bmp"));
		UI[1].Load(TEXT("sub_image\\luizy_UI.bmp"));
		UI[2].Load(TEXT("sub_image\\wario_UI.bmp"));
		demage_UI.Load(TEXT("sub_image\\demage_UI.bmp"));
		createSound();
		Rankstate.Load(TEXT("sub_image\\ranking.bmp"));//��ŷ ��� ����
		pChannel[0]->setVolume(stateVolume);
		pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[title], false, &pChannel[0]);
	
		pChannel[1]->setVolume(0.5);
	

		wsprintf(Playtime_t, TEXT("%d"), PlayTime);
	
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:
		switch (state)
		{
		case title: {
			state = cho_map;
			pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[state], false, &pChannel[0]);
			sel.x = 140;
			sel.y = 650;
			map_stage = 1;
			pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
			break; }
		case cho_map:
			switch (wParam)
			{
			case VK_LEFT:
				pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
				sel.x -= 200;
				if (sel.x < 0)
					sel.x = 140 + 200 * 5;
				map_stage = ((sel.x - 140) / 200) + 1;
				if (map_stage == 6)
					SetTimer(hWnd, 1, 150, NULL);
				else
					KillTimer(hWnd, 1);
				break;

			case VK_RIGHT:
				pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
				sel.x += 200;
				if (sel.x > 140 + 200 * 5)
					sel.x = 140;
				map_stage = ((sel.x - 140) / 200) + 1;
				if (map_stage == 6)
					SetTimer(hWnd, 1, 150, NULL);
				else
					KillTimer(hWnd, 1);
				break;
			case 'A':
			{
				state = cho_cha;
				map_stage = ((sel.x - 140) / 200) + 1;
				if (map_stage == 6)
					map_stage = rand() % 5 + 1;
				m.load(map_stage, rectView);
				sel.x = 100 + 125 + 50;
				sel.y = 175 + 120 + 50;
				nowPlayer = MARIO;
				pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);

				KillTimer(hWnd, 1);

				break;
			}
			}
			break;
		case cho_cha:
			switch (wParam)
			{
			case VK_LEFT:
				pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
				sel.x -= 360;
				--nowPlayer;
				if (sel.x < 0) {
					sel.x = 275 + 360 * 2;
					nowPlayer = 2;
				}
				break;

			case VK_RIGHT:
				pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
				sel.x += 360;
				++nowPlayer;
				if (sel.x > 275 + 360 * 2) {
					sel.x = 100 + 125 + 50;
					nowPlayer = 0;
				}
				break;
			case 'A':
				pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);
				state = play;
				sel.x = 100 + 125 + 50;
				sel.y = 175 + 120 + 50;
				SetTimer(hWnd, 5, 1000, NULL);
		
				pChannel[0]->stop();
				m.mapSystem->playSound(FMOD_CHANNEL_REUSE, m.mapSound, false, &pChannel[0]);
				//--------PLAYER SET--------//

				nPlayer = 3; // ���� �÷����ϴ� �÷��̾�� 1��. 
				BuildPlayer(nowPlayer);
				cam.setPos(m_Player[0]->GetPosition().x);
				wsprintf(Playtime_t, TEXT("%d"), PlayTime);
				SetTimer(hWnd, 0, 100, NULL);
				SetTimer(hWnd, 6, 1000, NULL);
			}
			break;


		case play:
			*pKeyBuffer = NULL;
			dwDirection = 0;
			GameReady = true;



			if (GetKeyboardState(pKeyBuffer))
			{
				SetTimer(hWnd, 2, 16, NULL);//����Ÿ�̸�

											//------PLAYER CHANGE------//
				if (pKeyBuffer[VK_F1] & 0xf0) {
					nowPlayer = MARIO;
				}

				if (pKeyBuffer[VK_F2] & 0xf0) {
					nowPlayer = WARIO;
					break;
				}

				if (pKeyBuffer[VK_F3] & 0xf0) {
					nowPlayer = RUISY;
				}
				if (GetAsyncKeyState(VK_RIGHT))
				{
					dwDirection |= DIR_RIGHT;


				}
			}
			break;
		case ranking:
			switch (wParam) {
			case 'A':
				state = cho_map;
				pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[state], false, &pChannel[0]);
				reset();
				KillTimer(hWnd, 0);
				KillTimer(hWnd,6);
				break;
			case 'S': {
				state = ending;
				pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[ending - 2], false, &pChannel[0]);
				break;
			}
			}
			break;
		case ending:
			exit(1);
			break;
		
		default:
			break;
		}
		if (wParam == VK_ESCAPE)exit(1);
		//�������� �ڵ� ����
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYUP:
	{
	switch (state)
	{
	case play:
		if (m_Player[nowPlayer]->GetStatus() == MOVE_RIGHT)
		{
			m_Player[nowPlayer]->SetStatus(BASIC_RIGHT);
			break;
		}
		if (m_Player[nowPlayer]->GetStatus() == MOVE_LEFT)
		{
			m_Player[nowPlayer]->SetStatus(BASIC_LEFT);
			break;
		}
		if (m_Player[nowPlayer]->GetStatus() == DEFENSE_RIGHT)
		{
			m_Player[nowPlayer]->SetStatus(BASIC_RIGHT);
			break;
		}
		if (m_Player[nowPlayer]->GetStatus() == DEFENSE_LEFT)
		{
			m_Player[nowPlayer]->SetStatus(BASIC_LEFT);
			break;
		}
	}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	break;

	case WM_TIMER:
	{
		if (state == play) //���� ��ġ�� �����ؾ���
		{
			static int count = 0;
			if (count >=2||m_Player[0]->live==false) {
				setranking();
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
				break;
			}
			count = 0;
			for (int i = 0; i < nPlayer; ++i)
			{
				if (m_Player[i]->GetPosition().y > 800&& m_Player[i]->live==true) {
					m_Player[i]->live = false;
					m_Player[i]->charSystem->playSound(FMOD_CHANNEL_REUSE, m_Player[i]->charSound[3], false, &m_Player[i]->pChannel);
					m_Player[i]->PlayTime_num = 99 - PlayTime;
				}
				if (m_Player[i]->live == false)++count;
				if (m_Player[i]->GetStatus() == JUMP_LEFT || m_Player[i]->GetStatus() == JUMP_RIGHT)
				{
					if (m_Player[i]->UsingJumpFrame == true)
					{
						m.collision(*m_Player[i]);
						m_Player[i]->Playercollision(m_Player, nPlayer);
						m_Player[i]->attack(m_Player, nPlayer);
						m_Player[i]->defance(m_Player, nPlayer);
						if (m_Player[i]->GetStatus() == FLY_RIGHT || m_Player[i]->GetStatus() == FLY_LEFT)
							m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 20, true);
						else if (m_Player[i]->GetStatus() == DYE_RIGHT || m_Player[i]->GetStatus() == DYE_LEFT)
							m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 10, false);
						else
						{
							m_Player[i]->gravity();
						}
					
					}
				}
				else
				{
					m.collision(*m_Player[i]);
					m_Player[i]->Playercollision(m_Player, nPlayer);
					m_Player[i]->attack(m_Player, nPlayer);
					m_Player[i]->defance(m_Player, nPlayer);
					if (m_Player[i]->GetStatus() == FLY_RIGHT || m_Player[i]->GetStatus() == FLY_LEFT)
						m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 20, true);
					else if (m_Player[i]->GetStatus() == DYE_RIGHT || m_Player[i]->GetStatus() == DYE_LEFT)
						m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 10, false);
					else {
						m_Player[i]->gravity();
					}
				}
				m_Player[i]->printdamege();
				m_Player[i]->smashadd();
			}
		}


		switch (wParam)
		{
		case 1:
			stage_view = rand() % 5;
			break;

		case 0:
			Timer();
			if (state == play) {
				m_Player[1]->GetEnemyPlayer(m_Player[0], m_Player[2]);
				m_Player[2]->GetEnemyPlayer(m_Player[1], m_Player[0]);

				for (int i = 0; i < 3; ++i)
				{
					if (m_Player[i]->GetStatus() == JUMP_LEFT || m_Player[i]->GetStatus() == JUMP_RIGHT)
					{
						if (m_Player[i]->JumpTimerOn)
							m_Player[i]->JumpTimer();
					}
				}
				m_Player[1]->CalculateDistanceTimer();
				m_Player[2]->CalculateDistanceTimer();
			}
			break;
		case 5: {
			--PlayTime;
			wsprintf(Playtime_t, TEXT("%d"), PlayTime);
			if (PlayTime == 0) {
				setranking();
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
			}
		}
				break;
		case 6:

			for (int i = 0; i < nPlayer; ++i)
			{
				m_Player[i]->PlayerAttack();
			}

			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		CImage img;
		img.Create(rectView.right, rectView.bottom, 24);
		HDC memDC = img.GetDC();

		switch (state)
		{
		case title:
			Title.Draw(memDC, 0, 0, rectView.right, rectView.bottom);
			break;
		case cho_map: {
			RECT rc;
			RECT rc2;
			RECT selectRC;

			selectRC.left = -80 + sel.x;
			selectRC.top = -45 + sel.y;
			selectRC.right = 80 + sel.x;
			selectRC.bottom = 45 + sel.y;
			Background.Draw(memDC, 0, 0);

			rc2.left = -400;
			rc2.top = -225;
			rc2.right = 400;
			rc2.bottom = 225;
			rc.left = -80;
			rc.top = -45;
			rc.right = 80;
			rc.bottom = 45;

			Rectangle(memDC, rectView.right / 2 + rc2.left, rectView.bottom / 2 + rc2.top - 100, rectView.right / 2 + rc2.right, rectView.bottom / 2 + rc2.bottom - 100);
			if (map_stage == 6) {
				mapEX[stage_view].Draw(memDC, rectView.right / 2 + rc2.left, rectView.bottom / 2 + rc2.top - 100, 800, 450, 0, 0, 800, 450);
			}
			else
				mapEX[map_stage - 1].Draw(memDC, rectView.right / 2 + rc2.left, rectView.bottom / 2 + rc2.top - 100, 800, 450, 0, 0, 800, 450);
			for (int i = 0; i < 6; ++i) {
				Rectangle(memDC, (i)*(40 + 160) + rc.left + 80 + 60, 650 + rc.top, (i) * (40 + 160) + rc.right + 80 + 60, 650 + rc.bottom);
				mapEX[i].Draw(memDC, (i)*(40 + 160) + rc.left + 80 + 60, 650 + rc.top, 160, 90, 0, 0, 800, 450);
				//40 �簢������ �Ÿ�, 160 �簢���� ũ��,80 ù �簢���� ������ ���ߴ� ��, 60,���� ����
			}
			FrameRect(memDC, &selectRC, hBrush);
			break; }
		case cho_cha: {
			RECT selectRC;
			Background.Draw(memDC, 0, 0);
			for (int i = 0; i < 3; ++i) {
				Choice_cha[i].Draw(memDC, 100 + i*(350 + 10), 120, 350, 450);
			}
			selectRC.left = -175 + sel.x;
			selectRC.top = -225 + sel.y;
			selectRC.right = 175 + sel.x;
			selectRC.bottom = 225 + sel.y;
			FrameRect(memDC, &selectRC, hBrush);
			break;
		}
		case play:
			m.draw(memDC, rectView, cam, map_stage);
			//���� �÷����ϰ��ִ� �÷��̾ ��� draw 
			SelectObject(memDC, UIFont);
			for (int i = 0; i < nPlayer; ++i)
			{
				
					m_Player[i]->DrawSprite(memDC,
						m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent, cam);
					m_Player[i]->UI.TransparentBlt(memDC, 280 + i * 350, 660, 50, 50, 0, 0, 30, 30, RGB(0, 0, 0));
					demage_UI.TransparentBlt(memDC, 340 + i * 350, 620, 150, 150, 0, 0, 170, 170, RGB(255, 255, 255));
					SetBkMode(memDC, TRANSPARENT);
					TextOut(memDC, 400 + i * 350 - 10, 690, TEXT(m_Player[i]->getDamege()), strlen(m_Player[i]->getDamege()));
			}

			
			SelectObject(memDC, TimeFont);
			SetBkMode(memDC, TRANSPARENT);
			TextOut(memDC, 650, 20, TEXT(Playtime_t), strlen(Playtime_t));
			break;
		case ranking:
			Rankstate.Draw(memDC, 0, 0, rectView.right, rectView.bottom);
			for (int i = 0; i < 3; i++) {
				m_Player[i]->rank_state.Draw(memDC, 100, 50 + 230 * i, 1080, 200, 0, 0, 550, 115);
				//																												RoundRect(memDC, 100, 50+230*i, rectView.right - 100, 50+230*i+200, 10, 10);
				m_Player[i]->DrawSprite(memDC, m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent, 150, 100 + 230 * i);
				SetTextColor(memDC, RGB(255, 255, 255));
				SetBkMode(memDC, TRANSPARENT);
				//TextOut(memDC, 150,100 + 245 * i, TEXT("Mario"), 6);
				TextOut(memDC, 300, 100 + 245 * i, TEXT(m_Player[i]->damage), strlen(m_Player[i]->damage));
				TextOut(memDC, 600, 100 + 245 * i, TEXT(m_Player[i]->PlayTime), strlen(m_Player[i]->PlayTime));
				TextOut(memDC, 820, 100 + 245 * i, TEXT(m_Player[i]->total_score), strlen(m_Player[i]->total_score));
				TextOut(memDC, 1100, 100 + 245 * i, TEXT(m_Player[i]->ranking), strlen(m_Player[i]->ranking));
				//m_Player[i]->damage_num
			}//��ŷ ȭ�� ���� �̹��� �Ҵ� �� ���� ����
			SetTextColor(memDC, RGB(0, 0, 0));
			break;
		case ending:
			Ending.Draw(memDC, -10, 0, rectView.right, rectView.bottom);
			break;

		default:
			break;
		}
		img.Draw(hdc, 0, 0);
		img.ReleaseDC();

		EndPaint(hWnd, &ps);
	}
	break;


	case WM_DESTROY:
		KillTimer(hWnd, 1);
		pSystem->release();
		//	�ý��� �ݱ�
		pSystem->close();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void Timer(void)
{
	// ��������Ʈ�� 1/60�� ���� �����Ͽ� �� 1.1�� �ֱ�� �ݺ��ȴ�.
	for (int i = 0; i < nPlayer; ++i)
	{
		if (m_Player[i]->m_State == DEFENSE_LEFT || m_Player[i]->m_State == DEFENSE_RIGHT)
		{
			if (m_Player[i]->sma)
				m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 1;
			else
				m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 0;
			//if (m_Player[i]->m_State == DEFENSE_LEFT)m_Player[i]->m_State == DEFENSE_LEFT;
			m_Player[i]->FrameEnd = 1;
		}

		else
		{
			m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent += 1;

			//�����̴°��� ��������Ʈ�� ��� ���ư�����. 
			if (m_Player[i]->m_State == BASIC_RIGHT || m_Player[i]->m_State == BASIC_LEFT
				|| m_Player[i]->m_State == MOVE_RIGHT || m_Player[i]->m_State == MOVE_LEFT
				|| m_Player[i]->m_State == JUMP_RIGHT || m_Player[i]->m_State == JUMP_LEFT
				|| m_Player[i]->m_State == FLY_RIGHT || m_Player[i]->m_State == FLY_LEFT
				||m_Player[i]->m_State==WIN||m_Player[i]->m_State==LOSE)
			{
				(m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent)
					%= m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount;
			}

			//�������� ��������Ʈ 1���� ���ư������ϰ�, ������ ���°� �ٲ�� ���ֱ�. 
			else
			{
				if (m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent >=
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount)
				{
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 0;

					//---------2�� ������ Attack2�� �������� �ϱ�--------//
					if (m_Player[nowPlayer]->n_AttackCount > 1 && m_Player[i]->m_State == ATTACK1_RIGHT)
					{
						m_Player[i]->SetStatus(ATTACK2_RIGHT);
						m_Player[nowPlayer]->n_AttackCount = 1;
					}
					else if (m_Player[nowPlayer]->n_AttackCount > 1 && m_Player[i]->m_State == ATTACK1_LEFT)
					{
						m_Player[i]->SetStatus(ATTACK2_LEFT);
						m_Player[nowPlayer]->n_AttackCount = 1;
					}
					//-------------�ٿ� ���¿��� �Ͼ �� �����ϱ�---------------//
					else if (m_Player[i]->GetStatus() == DYE_RIGHT) {
						m_Player[i]->SetStatus(UP_RIGHT);
					}
					else if (m_Player[i]->GetStatus() == DYE_LEFT) {
						m_Player[i]->SetStatus(UP_LEFT);
					}
					//------------������-------------------//
					else
					{
						m_Player[i]->SetBasic(m_Player[i]->m_State);
					}
				}
			}
		}

	}
}