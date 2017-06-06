#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else 
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment(lib, "Msimg32.lib")
#define RAD 3.141592/180 
#define CLIENT_WIDTH 1280		// 클라이언트 너비
#define CLIENT_HEIGHT 720		// 클라이언트 높이
//-------------DEFINE CHARDIR-----------//
#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

//-------------DEFINE CHAR STATUS-----------//
#define MOVE_RIGHT				0
#define MOVE_LEFT				1
#define JUMP_RIGHT				2		
#define JUMP_LEFT				3
#define ATTACK_HIGH				4
#define NORMAL					5
#define DEFEND					6
#define FLYING					7
#define FLAT					8 //앉기
#define DYE						9 //쓰러지기
#define VICTORY					10
#define LOSE					11

//------------CHAR DEFINE---------------//
#define MARIO					0;
#define RUISY					1;
#define WARIO					2;


//------------DEFINE HEADER-------------//
#include <Windows.h>
#include<atlimage.h>
#include <iostream>
#include<string>
#include<ctime>
// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

using namespace std;


extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);

