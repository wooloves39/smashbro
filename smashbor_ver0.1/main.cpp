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

//-----Player추가 
int		  nowPlayer;	//현재 내가 움직이고있는 플레이어
int		  nPlayer;		//현재 플레이하는 모든 플레이어의 수
CPlayer** m_Player;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
map m;
CCamera cam;
int map_stage;
CImage UI[3];
CImage demage_UI;
CImage number;
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
	//일단은 마리오만 만들어놓음. 1 
	m_Player = new CPlayer*[1];
	//1. Mario
	CPlayer *Mario = new CPlayer(4);
	Mario->SetTexture(MOVE_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTMOVE.png"));
	Mario->SetTexture(MOVE_LEFT,
		_T("character\\MARIO\\MARIO_LEFTMOVE.png"));
	Mario->SetTexture(JUMP_RIGHT,
		_T("character\\MARIO\\MARIO_RIGHTJUMP.png"));
	Mario->SetTexture(JUMP_LEFT,
		_T("character\\MARIO\\MARIO_LEFTJUMP.png"));

	m_Player[0] = Mario;
	m_Player[0]->SetPosition(500, 300);
	m_Player[0]->SetStatus(MOVE_RIGHT);	//현재상태 셋팅 
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
		number.Load(TEXT("sub_image\\number.bmp"));
		Lankstate;

		//--------PLAYER SET--------//
		nPlayer = 1; // 현재 플레이하는 플레이어는 1명. 
		nowPlayer = MARIO;	//현재 플레이어는 마리오 
		BuildPlayer();
		cam.setPos();
		cout << cam.getPos().x << endl;
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
				SetTimer(hWnd, 2, 16, NULL);//점프타이머
			case VK_LEFT:
				dwDirection |= DIR_LEFT;
				m_Player[nowPlayer]->SetStatus(MOVE_LEFT);

				break;

			case VK_RIGHT:
				dwDirection |= DIR_RIGHT;
				m_Player[nowPlayer]->SetStatus(MOVE_RIGHT);

				break;

			case VK_SPACE:
				dwDirection |= DIR_UP;
				if (m_Player[nowPlayer]->GetStatus() == MOVE_RIGHT)
					m_Player[nowPlayer]->SetStatus(JUMP_RIGHT);
				if (m_Player[nowPlayer]->GetStatus() == MOVE_LEFT)
					m_Player[nowPlayer]->SetStatus(JUMP_LEFT);
				break;

			case 'A':
				state = ending;

				break;

			case 'W':
				m_Player[nowPlayer]->sma = true;
				break;
			}
			if (dwDirection)
			{
				m_Player[nowPlayer]->Move(dwDirection, 2.0f, true);
				if (m_Player[nowPlayer]->GetPosition().x >= 250 && m_Player[nowPlayer]->GetPosition().x <= 750);
				cam.add(m_Player[nowPlayer]->getVelocity().x);
				cout << m_Player[nowPlayer]->GetPosition().x << endl;
				cout << cam.getPos().x << endl;
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

	case WM_TIMER:
	{
		if (m_Player[nowPlayer]->sma == true)
			m_Player[nowPlayer]->smashing();
		switch (wParam)
		{
		case 1:
			stage_view = rand() % 5;
			break;

		case 0:
			// 스프라이트는 1/60초 마다 갱신하여 약 1.1초 주기로 반복된다.
			for (int i = 0; i < nPlayer; ++i)
			{
				m_Player[i]->g_nSpriteCurrent += 1;
				(m_Player[i]->g_nSpriteCurrent) %= m_Player[i]->g_nSpriteCount;
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
				//40 사각형간의 거리, 160 사각형의 크기,80 첫 사각형을 원점에 맞추는 것, 60,양쪽 보정
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
			SetTimer(hWnd, 0, 64, NULL);

			m.draw(memDC, rectView, cam, map_stage);
			//현재 플레이하고있는 플레이어를 모두 draw 
			for (int i = 0; i < nPlayer; ++i)
				m_Player[i]->DrawSprite(memDC, m_Player[i]->g_nSpriteCurrent, cam);
			for (int i = 0; i < 3; ++i) {
				UI[i].TransparentBlt(memDC, 280 + i * 350, 660, 50, 50, 0, 0, 30, 30, RGB(0, 0, 0));
				demage_UI.TransparentBlt(memDC, 340 + i * 350, 620, 150, 150, 0, 0, 170, 170, RGB(255, 255, 255));
				number.Draw(memDC, 390 + i * 350, 690, 15, 15, i * 140, 0, 140, 250);
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

