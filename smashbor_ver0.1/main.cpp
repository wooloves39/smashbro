#include "stdafx.h"
#include "Camera.h"
#include "Player.h"
#include"map.h"

int state = 0;
enum state { title, cho_map, cho_cha, play, ending, ranking };

CImage Title;
CImage Background;
CImage Choice_map[6];
CImage Choice_cha[3];
CImage Ending;
CImage Lankstate;
CImage mapEX[6];
POINT sel;

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
	WndClass.lpszClassName = TEXT("Star PangPang!");
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rcWindow = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow(TEXT("Star PangPang!"), TEXT("Star PangPang!"), WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	//HMENU hmymenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	//SetMenu(hWnd, hmymenu);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//-----------------------------------------------------------//
CImage tempImage;

void BuildPlayer(void)
{
	//+WARIO
	m_Player = new CPlayer*[3];
	//1. Mario
	CPlayer *Mario = new CPlayer(26);
	//BASIC 
	Mario->SetTexture(BASIC_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTBASIC.png"), 4);
	Mario->SetTexture(BASIC_LEFT,
		_T("character\\MARIO\\MARIO_LEFTBASIC.png"), 4);
	//RUN
	Mario->SetTexture(MOVE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTMOVE.png"),8);
	Mario->SetTexture(MOVE_LEFT,
		_T("character\\MARIO\\MARIO_LEFTMOVE.png"),8);
	//JUMP
	Mario->SetTexture(JUMP_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTJUMP.png"),5);
	Mario->SetTexture(JUMP_LEFT,
		_T("character\\MARIO\\MARIO_LEFTJUMP.png"),5);
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
		_T("character\\MARIO\\MARIO_RIGHT_DEF.png"), 1);
	Mario->SetTexture(DEFENSE_LEFT,
		_T("character\\MARIO\\MARIO_LEFT_DEF.png"), 1);
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
	Mario->SetTexture(DYE_LEFT,
		_T("character\\MARIO\\MARIO_LEFTDYE.png"), 3);
	Mario->SetTexture(DYE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTDYE.png"), 3);
	//LOSE
	Mario->SetTexture(LOSE,
		_T("character\\MARIO\\MARIO_LOSE.png"), 2);
	//WIN
	Mario->SetTexture(WIN,
		_T("character\\MARIO\\MARIO_VICTORY.png"), 4);

	m_Player[0] = Mario;
	m_Player[0]->SetPosition(250, 300);
	m_Player[0]->SetStatus(BASIC_RIGHT);	//������� ���� 

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
		_T("character\\WARIO\\WARIO_RIGHT_DEF.png"), 1);
	Wario->SetTexture(DEFENSE_LEFT,
		_T("character\\WARIO\\WARIO_LEFT_DEF.png"), 1);
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


	m_Player[1] = Wario;
	m_Player[1]->SetPosition(300, 300);
	m_Player[1]->SetStatus(BASIC_LEFT);	//������� ���� 

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
		_T("character\\LUIZY\\LUIZY_RIGHT_DEF.png"), 1);
	Luizy->SetTexture(DEFENSE_LEFT,
		_T("character\\LUIZY\\LUIZY_LEFT_DEF.png"), 1);
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


	m_Player[2] = Luizy;
	m_Player[2]->SetPosition(350, 300);
	m_Player[2]->SetStatus(BASIC_LEFT);	//������� ���� 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static POINT point;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)), oldBrush;
	static RECT rectView;
	char score[10];
	static int stage_view = 0;
	DWORD dwDirection = 0;

	switch (iMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		Title.Load(TEXT("sub_image\\titleUI.bmp"));
		//Choice_map;
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

		
		Lankstate;

		//--------PLAYER SET--------//
		nPlayer = 3; // ���� �÷����ϴ� �÷��̾�� 1��. 
		nowPlayer = MARIO;	//���� �÷��̾�� ������ 
		BuildPlayer();
		cam.setPos();

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
			sel.x = 140;
			sel.y = 650;
			map_stage = 1;
			break; }
		case cho_map:
			switch (wParam)
			{
			case VK_LEFT:
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
				KillTimer(hWnd, 1);
				break;
			}
			}
			break;
		case cho_cha:
			switch (wParam)
			{
			case VK_LEFT:
				sel.x -= 360;
				if (sel.x < 0)
					sel.x = 275 + 360 * 2;
				break;

			case VK_RIGHT:
				sel.x += 360;
				if (sel.x > 275 + 360 * 2)
					sel.x = 100 + 125 + 50;
				break;
			case 'A':
				state = play;
				sel.x = 100 + 125 + 50;
				sel.y = 175 + 120 + 50;

				break;
			}
			break;


		case play:
			switch (wParam)
			{
				SetTimer(hWnd, 2, 16, NULL);//����Ÿ�̸�
			//------PLAYER CHANGE------//
			case VK_F1:
				nowPlayer = MARIO;
				break;

			case VK_F2:
				nowPlayer = WARIO;
				break;

			case VK_F3:
				nowPlayer = RUISY;
				break;
			//--------------------------//

			case VK_LEFT:
				dwDirection |= DIR_LEFT;
				m_Player[nowPlayer]->SetStatus(MOVE_LEFT);

				//cam.add(-10);
				break;

			case VK_RIGHT:
				dwDirection |= DIR_RIGHT;
				m_Player[nowPlayer]->SetStatus(MOVE_RIGHT);
				//cam.add(10);
				break;

			case VK_SPACE:
				dwDirection |= DIR_UP;
				if(m_Player[nowPlayer]->GetStatus() == MOVE_RIGHT)
					m_Player[nowPlayer]->SetStatus(JUMP_RIGHT);
				if (m_Player[nowPlayer]->GetStatus() == MOVE_LEFT)
					m_Player[nowPlayer]->SetStatus(JUMP_LEFT);
				break;

			case 'G':
				if (m_Player[nowPlayer]->GetStatus() == BASIC_RIGHT)
				{
					m_Player[nowPlayer]->SetStatus(HATTACK_RIGHT);
				}
				if (m_Player[nowPlayer]->GetStatus() == BASIC_LEFT)
				{
					m_Player[nowPlayer]->SetStatus(HATTACK_LEFT);
				}
				break;
			case 'F':
				if (m_Player[nowPlayer]->GetStatus() == BASIC_RIGHT)
				{
					m_Player[nowPlayer]->SetStatus(ATTACK1_RIGHT);
				}
				else if (m_Player[nowPlayer]->GetStatus() == BASIC_LEFT)
				{
					m_Player[nowPlayer]->SetStatus(ATTACK1_LEFT);
				}

				else if (m_Player[nowPlayer]->m_State == ATTACK1_RIGHT)
				{
					m_Player[nowPlayer]->n_AttackCount = 2;
				}
				else if (m_Player[nowPlayer]->m_State == ATTACK1_LEFT)
				{
					m_Player[nowPlayer]->n_AttackCount = 2;
				}
				break;

			case 'A':
				state = ending;

				break;
			}

			if (dwDirection)
			{
				m_Player[nowPlayer]->Move(dwDirection, 2.0f, true);
			}
			break;


		case ending:
			switch (wParam)
			{
			case VK_LEFT:
				sel.x -= 360;
				if (sel.x < 0)
					sel.x = 275 + 360 * 2;
				break;

			case VK_RIGHT:
				sel.x += 360;
				if (sel.x > 275 + 360 * 2)
					sel.x = 100 + 125 + 50;
				break;
			case 'A':
				state = ranking;
				sel.x = 100 + 125 + 50;
				sel.y = 175 + 120 + 50;
				break;
			}
			break;
		case ranking:
			break;
		default:
			break;
		}
		switch (wParam)
		{
		case VK_LEFT:

			break;

		case VK_RIGHT:

			break;
		case VK_DOWN:

			break;
		case VK_SPACE:
			break;
		case VK_ESCAPE:
			exit(1);
		}
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
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	break;

	case WM_TIMER:
	{
		if (state == play) {
			for (int i = 0; i < nPlayer; ++i) {
			m.collision(*m_Player[i]);


				if (m_Player[i]->sma == true)
					m_Player[i]->smashing();
				else {
					m_Player[i]->gravity();
				}
				m_Player[i]->printdamege();
			}
		}
		switch (wParam)
		{
		case 1:
			stage_view = rand() % 5;
			break;

		case 0:
			// ��������Ʈ�� 1/60�� ���� �����Ͽ� �� 1.1�� �ֱ�� �ݺ��ȴ�.
			for (int i = 0; i < nPlayer; ++i)
			{
				m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent += 1;

				//�����ϱ�� ��������Ʈ 1���� ���ư������ϱ�
				if (m_Player[i]->m_State == HATTACK_RIGHT || m_Player[i]->m_State == HATTACK_LEFT
					|| m_Player[i]->m_State == ATTACK1_RIGHT || m_Player[i]->m_State == ATTACK1_LEFT
					|| m_Player[i]->m_State == ATTACK2_RIGHT || m_Player[i]->m_State == ATTACK2_LEFT)
				{
					if (m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent >=
						m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount)
					{
						m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent = 0;
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

						else m_Player[i]->SetStatus(BASIC_RIGHT);
					}
				}

				(m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent) 
					%= m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCount;
			}
				if (m_Player[nowPlayer]->GetStatus() == JUMP_LEFT || m_Player[nowPlayer]->GetStatus() == JUMP_RIGHT)
				{
					m_Player[nowPlayer]->JumpTimer();
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
			SetTimer(hWnd, 0, 100, NULL);
			
			m.draw(memDC, rectView, cam, map_stage);
			//���� �÷����ϰ��ִ� �÷��̾ ��� draw 
			for(int i = 0 ; i < nPlayer ; ++i)
				m_Player[i]->DrawSprite(memDC, 
					m_Player[i]->m_ppTexture[m_Player[i]->m_State].nSpriteCurrent);
			cout << m_Player[1]->m_ppTexture[m_Player[1]->m_State].nSpriteCurrent << endl;
			
			for (int i = 0; i < 3; ++i) {
				UI[i].TransparentBlt(memDC, 280 + i * 350, 660, 50, 50, 0, 0, 30, 30, RGB(0, 0, 0));
				demage_UI.TransparentBlt(memDC, 340 + i * 350, 620, 150, 150, 0, 0, 170, 170, RGB(255, 255, 255));

				TextOut(memDC, 400 + i * 350, 690, TEXT(m_Player[i]->getDamege()), strlen(m_Player[i]->getDamege()));
			}
			break;
		case ending:
			Ending.Draw(memDC, -10, 0, rectView.right, rectView.bottom);
			break;
		case ranking:
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
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

