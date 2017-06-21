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
#define BASIC_RIGHT				0		
#define BASIC_LEFT				1
#define MOVE_RIGHT				2
#define MOVE_LEFT				3
#define JUMP_RIGHT				4		
#define JUMP_LEFT				5
#define HATTACK_RIGHT			6		
#define HATTACK_LEFT			7
#define ATTACK1_RIGHT			8		
#define ATTACK1_LEFT			9
#define ATTACK2_RIGHT			10		
#define ATTACK2_LEFT			11
#define KICK_RIGHT				12		
#define KICK_LEFT				13
#define DEFENSE_RIGHT			14	
#define DEFENSE_LEFT			15
#define SIT_RIGHT				16		
#define SIT_LEFT				17
#define FLY_RIGHT				18	//날아가기	
#define FLY_LEFT				19
#define UP_RIGHT				20
#define UP_LEFT					21
#define DYE_RIGHT				22
#define DYE_LEFT				23
#define LOSE					24		
#define WIN						25

//------------CHAR DEFINE---------------//
#define MARIO					0;
#define WARIO					1;
#define RUISY					2;


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
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;
using namespace std;


extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);

