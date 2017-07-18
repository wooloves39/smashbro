#include "stdafx.h"
#include "Camera.h"
#include "Player.h"
#include"map.h"

//06/17  맵 카메라 H 전체 교체 필요 점수 판 만들어 놨고 이미지 할당해놔야함.
//임팩트 이미지를 편집하지 못함.. 부탁드림
//캐릭터 선택 할수 있게 부탁
//전체 state가 rank가 되면 win,lose로  캐릭터 스프라이트 할당할껀데 basic으로 안돌아가게 구현 부탁
//
int state = 0;
enum state { title, cho_map, cho_cha, play, ranking, ending };//스테이트 순서 변경
enum DIRECTION_P
{
	DIRECTION_LEFT = 1,
	DIRECTION_RIGHT
};
CImage Title;
CImage Background;
CImage Choice_map[6];
CImage Choice_cha[4];
CImage Ending;
CImage Rankstate;
CImage mapEX[6];
POINT sel;
POINT sel2;
System*	pSystem;
Sound* stateSound[4];
Sound* choiceSound;
Sound* changeSound;
float stateVolume = 0.2f;
Channel*		pChannel[2];
bool	GameReady = false;
static UCHAR pKeyBuffer[256];
int mode;//1피냐 2피냐
int PlayTime = 99;//플레이 타임
char Playtime_t[3];
//-----Player추가 
int		  nowPlayer;	//현재 내가 움직이고있는 플레이어
int		  nPlayer;		//현재 플레이하는 모든 플레이어의 수
CPlayer** m_Player;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
map m;
CCamera cam;
int map_stage;
int map_stage2;
CImage UI[3];
CImage demage_UI;
void Timer(void);
void setranking() {
	pChannel[0]->stop();
	pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[ranking - 2], false, &pChannel[0]);
	state = ranking;
	for (int i = 0; i < 3; i++) {
		if (m_Player[i]->live == true)m_Player[i]->PlayTime_num = 99;
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

void createSound() {
	System_Create(&pSystem);
	pSystem->init(6, FMOD_INIT_NORMAL, NULL);

	/*pSystem->createSound("sound\\opening.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[title]);
	pSystem->createSound("sound\\choice.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[cho_map]);

	pSystem->createSound("sound\\ranking.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[ranking-2]);
	pSystem->createSound("sound\\ending.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, NULL, &stateSound[ending-2]);
	pSystem->createSound("sound\\imfact\\choice.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &choiceSound);
	pSystem->createSound("sound\\imfact\\change.wav", FMOD_HARDWARE | FMOD_LOOP_OFF, NULL, &changeSound);*/
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
		if (m_Player != nullptr) {
			if (mode == 2) {
				m_Player[0]->KeyState(cam, state, mode, 1);
				m_Player[1]->KeyState(cam, state, mode, 2);
				m_Player[2]->KeyState(cam, state, 1);
			}
			else {
				m_Player[0]->KeyState(cam, state, mode);
				m_Player[1]->KeyState(cam, state, 1);
				m_Player[2]->KeyState(cam, state, 1);
			}
		}
	}
	return Message.wParam;
}

//-----------------------------------------------------------//

void BuildPlayer(int nMyPlayer, int mode)
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
	//1-3 점프중 공격키누르면 나오는것 
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
	//FLY (날아가는 모션)
	Mario->SetTexture(FLY_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHT_FLY.png"), 4);
	Mario->SetTexture(FLY_LEFT,
		_T("character\\MARIO\\MARIO_LEFT_FLY.png"), 4);
	//UP(일어나는 모션)
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
	//1-3 점프중 공격키누르면 나오는것 
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
	//FLY (날아가는 모션)
	Wario->SetTexture(FLY_RIGHT,
		_T("character\\WARIO\\WARIO_RIGHT_FLY.png"), 4);
	Wario->SetTexture(FLY_LEFT,
		_T("character\\WARIO\\WARIO_LEFT_FLY.png"), 4);
	//UP(일어나는 모션)
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
	//1-3 점프중 공격키누르면 나오는것 
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
	//FLY (날아가는 모션)
	Luizy->SetTexture(FLY_RIGHT,
		_T("character\\LUIZY\\LUIZY_RIGHT_FLY.png"), 4);
	Luizy->SetTexture(FLY_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFT_FLY.png"), 4);
	//UP(일어나는 모션)
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
	if (mode == 1) {
		m_Player[0] = new CPlayer(26);
		m_Player[0]->SetPosition(-200, 300);
		m_Player[0]->SetStatus(BASIC_RIGHT);	//현재상태 셋팅 

		m_Player[1] = new CAIPlayer(26);
		m_Player[1]->SetPosition(200, 300);
		m_Player[1]->SetStatus(BASIC_LEFT);	//현재상태 셋팅 


		m_Player[2] = new CAIPlayer(26);
		m_Player[2]->SetPosition(0, 300);
		m_Player[2]->SetStatus(BASIC_LEFT);	//현재상태 셋팅 
	}
	if (mode == 2) {
		m_Player[0] = new CPlayer(26);
		m_Player[0]->SetPosition(-200, 300);
		m_Player[0]->SetStatus(BASIC_RIGHT);	//현재상태 셋팅 

		m_Player[1] = new CPlayer(26);
		m_Player[1]->SetPosition(200, 300);
		m_Player[1]->SetStatus(BASIC_LEFT);	//현재상태 셋팅 


		m_Player[2] = new CAIPlayer(26);
		m_Player[2]->SetPosition(0, 300);
		m_Player[2]->SetStatus(BASIC_LEFT);	//현재상태 셋팅 
	}
	if (nMyPlayer == 0) {
		m_Player[0]->SetImage(Mario->GetImage());
		m_Player[0]->rank_state = Mario->rank_state;
		m_Player[0]->UI = Mario->UI;
		m_Player[1]->SetImage(Wario->GetImage());
		m_Player[1]->rank_state = Wario->rank_state;
		m_Player[1]->UI = Wario->UI;
		m_Player[2]->SetImage(Luizy->GetImage());
		m_Player[2]->rank_state = Luizy->rank_state;
		m_Player[2]->UI = Luizy->UI;

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

	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static POINT point;
	HDC hdc;
	PAINTSTRUCT ps;
	static HFONT TimeFont = CreateFont(70, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("HY헤드라인M"));
	static HFONT UIFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("HY헤드라인M"));//문자체
	static HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	static HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)), hBrush2 = CreateSolidBrush(RGB(0, 255, 0));
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
		Choice_cha[3].Load(TEXT("sub_image\\choice4.bmp"));
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
		Rankstate.Load(TEXT("sub_image\\ranking.bmp"));//랭킹 배경 구현
		pChannel[0]->setVolume(stateVolume);
		pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[title], false, &pChannel[0]);

		pChannel[1]->setVolume(0.5);
		sel.x = 330;
		sel.y = 530;

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
		case title:
		{
			switch (wParam)
			{
			case VK_DOWN:
				sel.y += 80;
				if (sel.y > 620)
					sel.y = 530;
				break;
			case VK_UP:
				sel.y -= 80;
				if (sel.y < 530)
					sel.y = 610;
				break;
			case 'A':
			case 'S':
				if (sel.y < 550)mode = 1;
				else mode = 2;

				state = cho_map;
				pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[state], false, &pChannel[0]);
				sel.x = 140;
				sel.y = 650;
				if (mode == 2)
				{
					map_stage2 = 10;
					sel2.x = 140 + 200 * 5;
					sel2.y = 650;
				}
				map_stage = 1;
				pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
				break;
			default:
				break;
			}
		}
		break;
		case cho_map:
			if (mode == 1) {
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
					sel.x = 50 + 125;
					sel.y = 175 + 120 + 25;
					nowPlayer = MARIO;
					pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);

					KillTimer(hWnd, 1);

					break;
				}
				}
			}
			else {
				switch (wParam)
				{
				case 'A':
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
				case 'D':
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
				case 'F':
					state = cho_cha;
					map_stage = ((sel.x - 140) / 200) + 1;
					if (map_stage == 6)
						map_stage = rand() % 5 + 1;
					m.load(map_stage, rectView);
					sel.x = 50 + 125;
					sel.y = 175 + 120 + 25;
					nowPlayer = MARIO;
					pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);
					sel2.y = 175 + 120 + 25;
					sel2.x = 300 * 3 + 50 + 125;
					KillTimer(hWnd, 1);
					break;
				}
			}
			break;
		case cho_cha:
			if (mode == 1) {
				switch (wParam)
				{
				case VK_LEFT:
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel.x -= 250 + 50;
					--nowPlayer;
					if (sel.x < 0) {
						sel.x = 300 * 3 + 50 + 125;
						nowPlayer = 3;
					}
					break;

				case VK_RIGHT:
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel.x += 300;
					++nowPlayer;
					if (sel.x > 1200) {
						sel.x = 175;
						nowPlayer = 0;
					}
					break;
				case 'A':
					pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);
					state = play;
					sel.x = 100 + 125 + 50;
					sel.y = 175 + 120 + 50;
					SetTimer(hWnd, 5, 1000, NULL);
					SetTimer(hWnd, 6, 200, NULL);
					pChannel[0]->stop();
					m.mapSystem->playSound(FMOD_CHANNEL_REUSE, m.mapSound, false, &pChannel[0]);
					//--------PLAYER SET--------//

					nPlayer = 3; // 현재 플레이하는 플레이어는 1명. 
					BuildPlayer(nowPlayer, mode);
					if (mode == 1)
						cam.setPos(m_Player[0]->GetPosition().x);
					else if (mode == 2)
						cam.setPos(m_Player[0]->GetPosition().x, m_Player[1]->GetPosition().x);
					wsprintf(Playtime_t, TEXT("%d"), PlayTime);
					SetTimer(hWnd, 0, 100, NULL);

				}
				break;

			}
			else {
				switch (wParam)
				{
				case 'A':
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel.x -= 250 + 50;
					--nowPlayer;
					if (sel.x < 0) {
						sel.x = 300 * 3 + 50 + 125;
						nowPlayer = 3;
					}
					break;

				case 'D':
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel.x += 300;
					++nowPlayer;
					if (sel.x > 1200) {
						sel.x = 175;
						nowPlayer = 0;
					}
					break;
				case 'F':
					pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);
					state = play;
					sel.x = 100 + 125 + 50;
					sel.y = 175 + 120 + 50;
					SetTimer(hWnd, 5, 1000, NULL);
					SetTimer(hWnd, 6, 200, NULL);
					pChannel[0]->stop();
					m.mapSystem->playSound(FMOD_CHANNEL_REUSE, m.mapSound, false, &pChannel[0]);
					//--------PLAYER SET--------//

					nPlayer = 3; // 현재 플레이하는 플레이어는 1명. 
					BuildPlayer(nowPlayer, mode);

					if (mode == 1)
						cam.setPos(m_Player[0]->GetPosition().x);
					else if (mode == 2)
						cam.setPos(m_Player[0]->GetPosition().x, m_Player[1]->GetPosition().x);
					wsprintf(Playtime_t, TEXT("%d"), PlayTime);
					SetTimer(hWnd, 0, 100, NULL);

				case VK_LEFT:
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel2.x -= 250 + 50;
					//	--nowPlayer;
					if (sel2.x < 0) {
						sel2.x = 300 * 3 + 50 + 125;
						//	nowPlayer = 3;
					}
					break;
				case VK_RIGHT:
					pSystem->playSound(FMOD_CHANNEL_REUSE, changeSound, false, &pChannel[1]);
					sel2.x += 300;
					//++nowPlayer;
					if (sel2.x > 1200) {
						sel2.x = 175;
						//	nowPlayer = 0;
					}
					break;
				case VK_NUMPAD4:
					pSystem->playSound(FMOD_CHANNEL_REUSE, choiceSound, false, &pChannel[1]);
					state = play;
					sel2.x = 100 + 125 + 50;
					sel2.y = 175 + 120 + 50;
					SetTimer(hWnd, 5, 1000, NULL);
					SetTimer(hWnd, 6, 200, NULL);
					pChannel[0]->stop();
					m.mapSystem->playSound(FMOD_CHANNEL_REUSE, m.mapSound, false, &pChannel[0]);
					//--------PLAYER SET--------//

					nPlayer = 3; // 현재 플레이하는 플레이어는 1명. 
					BuildPlayer(nowPlayer, mode);

					if (mode == 1)
						cam.setPos(m_Player[0]->GetPosition().x);
					else if (mode == 2)
						cam.setPos(m_Player[0]->GetPosition().x, m_Player[1]->GetPosition().x);
					wsprintf(Playtime_t, TEXT("%d"), PlayTime);
					SetTimer(hWnd, 0, 100, NULL);

					break;

				}
				break;
			}
		case play:
			*pKeyBuffer = NULL;
			dwDirection = 0;
			GameReady = true;
			if (GetKeyboardState(pKeyBuffer))
			{
				SetTimer(hWnd, 2, 16, NULL);//점프타이머

											//------PLAYER CHANGE------//

			}
			break;
		case ranking:
			if (mode == 1) {
				switch (wParam) {
				case 'A':
					state = cho_map;
					pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[state], false, &pChannel[0]);
					reset();
					KillTimer(hWnd, 0);

					break;
				case 'S': {
					state = ending;
					pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[ending - 2], false, &pChannel[0]);
					break;
				}
				}
			}
			else {
				switch (wParam) {
				case 'F':
				case VK_NUMPAD4:
					state = cho_map;
					pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[state], false, &pChannel[0]);
					reset();
					KillTimer(hWnd, 0);
					break;
				case 'G':
				case VK_NUMPAD5:
				{
					state = ending;
					pSystem->playSound(FMOD_CHANNEL_REUSE, stateSound[ending - 2], false, &pChannel[0]);
					break;
				}
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
		//쓸때없는 코드 삭제
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYUP:
	{
		switch (state)
		{
		case play:
			if (mode == 1) {
				if (m_Player[0]->GetStatus() == MOVE_RIGHT)
				{
					m_Player[0]->SetStatus(BASIC_RIGHT);
					break;
				}
				if (m_Player[0]->GetStatus() == MOVE_LEFT)
				{
					m_Player[0]->SetStatus(BASIC_LEFT);
					break;
				}
				if (m_Player[0]->GetStatus() == DEFENSE_RIGHT)
				{
					m_Player[0]->SetStatus(BASIC_RIGHT);
					break;
				}
				if (m_Player[0]->GetStatus() == DEFENSE_LEFT)
				{
					m_Player[0]->SetStatus(BASIC_LEFT);
					break;
				}
			}
			else {
				switch (wParam)
				{
				case 'A':
				case 'D':
				case 'F':
				case 'G':
				case 'H':
				case 'T':
					if (m_Player[0]->GetStatus() == MOVE_RIGHT)
					{
						m_Player[0]->SetStatus(BASIC_RIGHT);
						break;
					}
					if (m_Player[0]->GetStatus() == MOVE_LEFT)
					{
						m_Player[0]->SetStatus(BASIC_LEFT);
						break;
					}
					if (m_Player[0]->GetStatus() == DEFENSE_RIGHT)
					{
						m_Player[0]->SetStatus(BASIC_RIGHT);
						break;
					}
					if (m_Player[0]->GetStatus() == DEFENSE_LEFT)
					{
						m_Player[0]->SetStatus(BASIC_LEFT);
						break;
					}
					break;
				case VK_LEFT:
				case VK_RIGHT:
				case VK_NUMPAD4:
				case VK_NUMPAD5:
				case VK_NUMPAD6:
				case VK_NUMPAD8:
					if (m_Player[1]->GetStatus() == MOVE_RIGHT)
					{
						m_Player[1]->SetStatus(BASIC_RIGHT);
						break;
					}
					if (m_Player[1]->GetStatus() == MOVE_LEFT)
					{
						m_Player[1]->SetStatus(BASIC_LEFT);
						break;
					}
					if (m_Player[1]->GetStatus() == DEFENSE_RIGHT)
					{
						m_Player[1]->SetStatus(BASIC_RIGHT);
						break;
					}
					if (m_Player[1]->GetStatus() == DEFENSE_LEFT)
					{
						m_Player[1]->SetStatus(BASIC_LEFT);
						break;
					}
					break;
				default:
					break;
				}
			}

		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	break;
	case WM_LBUTTONDOWN:
	{cout << LOWORD(lParam) << endl;
	cout << HIWORD(lParam) << endl;
	}
	case WM_TIMER:
	{
		if (state == play) //점프 고치면 수정해야함
		{
			if (mode == 1)
				cam.realsetPos(m_Player[0]->GetPosition().x);
			else if (mode == 2)
				cam.realsetPos(m_Player[0]->GetPosition().x, m_Player[1]->GetPosition().x);
			//cam.realsetPos(m_Player[0]->GetPosition().x);
			cam.add();
			static int count = 0;
			if (count >= 2) {
				setranking();
				count = 0;
				KillTimer(hWnd, 5);
				break;
			}
			count = 0;
			for (int i = 0; i < nPlayer; ++i)
			{
				if (m_Player[i]->AI() == true) {
					m_Player[i]->distance(m_Player, 3);
				}
				m.collision(*m_Player[i]);
				m_Player[i]->Playercollision(m_Player, nPlayer);
				m_Player[i]->gravity();
				m_Player[i]->attack(m_Player, nPlayer);
				m_Player[i]->defance(m_Player, nPlayer);
				if (m_Player[i]->GetStatus() == FLY_RIGHT || m_Player[i]->GetStatus() == FLY_LEFT)
					m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 20, true);
				else if (m_Player[i]->GetStatus() == DYE_RIGHT || m_Player[i]->GetStatus() == DYE_LEFT)
					m_Player[i]->smashing(m_Player[m_Player[i]->getAttacker()]->getDamege_num(), 10, false);

				if (m_Player[i]->GetPosition().y > 900 && m_Player[i]->live == true) {
					m_Player[i]->live = false;
					m_Player[i]->charSystem->playSound(FMOD_CHANNEL_REUSE, m_Player[i]->charSound[3], false, &m_Player[i]->pChannel);
					m_Player[i]->PlayTime_num = 99 - PlayTime;
				}
				if (m_Player[i]->live == false)++count;
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

			break;
		case 5: {
			--PlayTime;
			wsprintf(Playtime_t, TEXT("%d"), PlayTime);
			if (PlayTime == 0) {
				setranking();

				KillTimer(hWnd, 5);

			}
		}
				break;
		case 6: {
		
			if (m_Player != nullptr) {
				if (mode == 2) {
					m_Player[2]->KeyState(cam, state, 2);
				}
				else
				{
					m_Player[1]->KeyState(cam, state, 2);
					m_Player[2]->KeyState(cam, state, 2);
				}
			}
		}
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
			RECT selectRC;

			selectRC.left = -210 + sel.x;
			selectRC.top = -40 + sel.y;
			selectRC.right = 210 + sel.x;
			selectRC.bottom = 40 + sel.y;
			FrameRect(memDC, &selectRC, hBrush);
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
				//40 사각형간의 거리, 160 사각형의 크기,80 첫 사각형을 원점에 맞추는 것, 60,양쪽 보정
			}
			FrameRect(memDC, &selectRC, hBrush);
			break; }
		case cho_cha: {
			RECT selectRC;
			Background.Draw(memDC, 0, 0);
			for (int i = 0; i < 4; ++i) {
				Choice_cha[i].Draw(memDC, 50 + i*(250 + 50), 120, 250, 400);
			}
			selectRC.left = -125 + sel.x;
			selectRC.top = -200 + sel.y;
			selectRC.right = 125 + sel.x;
			selectRC.bottom = 200 + sel.y;
			if (mode == 2) {
				RECT selectRC2;
				selectRC2.left = -125 + sel2.x;
				selectRC2.top = -200 + sel2.y;
				selectRC2.right = 125 + sel2.x;
				selectRC2.bottom = 200 + sel2.y;
				FrameRect(memDC, &selectRC2, hBrush2);
			}
			FrameRect(memDC, &selectRC, hBrush);
			break;
		}
		case play:
			m.draw(memDC, rectView, cam, map_stage);
			//현재 플레이하고있는 플레이어를 모두 draw 
			SelectObject(memDC, UIFont);
			for (int i = 0; i < nPlayer; ++i)
			{

				m_Player[i]->DrawSprite(memDC,
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent, cam);
				m_Player[i]->UI.TransparentBlt(memDC, 280 + i * 350, 660, 50, 50, 0, 0, 30, 30, RGB(255, 255, 255));
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
				m_Player[i]->DrawSprite(memDC, m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent, 150, 100 + 230 * i);
				SetTextColor(memDC, RGB(255, 255, 255));
				SetBkMode(memDC, TRANSPARENT);
				TextOut(memDC, 300, 100 + 245 * i, TEXT(m_Player[i]->damage), strlen(m_Player[i]->damage));
				TextOut(memDC, 600, 100 + 245 * i, TEXT(m_Player[i]->PlayTime), strlen(m_Player[i]->PlayTime));
				TextOut(memDC, 820, 100 + 245 * i, TEXT(m_Player[i]->total_score), strlen(m_Player[i]->total_score));
				TextOut(memDC, 1100, 100 + 245 * i, TEXT(m_Player[i]->ranking), strlen(m_Player[i]->ranking));
				//m_Player[i]->damage_num
			}//랭킹 화면 구현 이미지 할다 후 수정 예정
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
		//	시스템 닫기
		pSystem->close();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void Timer(void)//플레이어 클래스 안에 종속 시키기
{
	// 스프라이트는 1/60초 마다 갱신하여 약 1.1초 주기로 반복된다.
	for (int i = 0; i < nPlayer; ++i)
	{
		if (m_Player[i]->m_State == DEFENSE_LEFT || m_Player[i]->m_State == DEFENSE_RIGHT)
		{
			if (m_Player[i]->sma)
				m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 1;
			else
				m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 0;
			m_Player[i]->FrameEnd = 1;
		}

		else
		{
			m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent += 1;

			//움직이는것은 스프라이트가 계속 돌아가도록. 
			if (m_Player[i]->m_State == BASIC_RIGHT || m_Player[i]->m_State == BASIC_LEFT
				|| m_Player[i]->m_State == MOVE_RIGHT || m_Player[i]->m_State == MOVE_LEFT
				|| m_Player[i]->m_State == JUMP_RIGHT || m_Player[i]->m_State == JUMP_LEFT
				|| m_Player[i]->m_State == FLY_RIGHT || m_Player[i]->m_State == FLY_LEFT
				|| m_Player[i]->m_State == WIN || m_Player[i]->m_State == LOSE)
			{
				(m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent)
					%= m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount;
			}

			//나머지는 스프라이트 1번만 돌아가도록하고, 끝나면 상태가 바뀌도록 해주기. 
			else
			{
				if (m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent >=
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount)
				{
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 0;

					//---------2번 누르면 Attack2가 나오도록 하기--------//
					if (m_Player[i]->n_AttackCount > 1 && m_Player[i]->m_State == ATTACK1_RIGHT)
					{
						m_Player[i]->SetStatus(ATTACK2_RIGHT);
						m_Player[i]->n_AttackCount = 1;
					}
					else if (m_Player[i]->n_AttackCount > 1 && m_Player[i]->m_State == ATTACK1_LEFT)
					{
						m_Player[i]->SetStatus(ATTACK2_LEFT);
						m_Player[i]->n_AttackCount = 1;
					}
					//-------------다운 상태에서 일어난 후 복귀하기---------------//
					else if (m_Player[i]->GetStatus() == DYE_RIGHT) {
						m_Player[i]->SetStatus(UP_RIGHT);
						m_Player[i]->hit = false;
					}
					else if (m_Player[i]->GetStatus() == DYE_LEFT) {
						m_Player[i]->SetStatus(UP_LEFT);
						m_Player[i]->hit = false;
					}
					//------------여까지-------------------//
					else
					{
						m_Player[i]->SetBasic(m_Player[i]->m_State);
					}
				}
			}
		}

	}
}