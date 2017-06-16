#include "stdafx.h"

void TRACE(_TCHAR *pString)
{
	_TCHAR pszBuffer[256];
	_stprintf_s(pszBuffer, 256, _T("%s\n"), pString);
	OutputDebugString(pszBuffer);
}

void TRACE(_TCHAR *pString, UINT uValue)
{
	_TCHAR pszBuffer[256];
	_stprintf_s(pszBuffer, 256, _T("%s%d\n"), pString, uValue);
	OutputDebugString(pszBuffer);
}

void TRACE(_TCHAR *pString, int nValue)
{
	_TCHAR pszBuffer[256];
	_stprintf_s(pszBuffer, 256, _T("%s%d\n"), pString, nValue);
	OutputDebugString(pszBuffer);
}

void TRACE(_TCHAR *pString, int nValue0, int nValue1)
{
	_TCHAR pszBuffer[256];
	_stprintf_s(pszBuffer, 256, _T("%s %d %d\n"), pString, nValue0, nValue1);
	OutputDebugString(pszBuffer);
}

void TRACE(_TCHAR *pString, float fValue)
{
	_TCHAR pszBuffer[256];
	_stprintf_s(pszBuffer, 256, _T("%s%f\n"), pString, fValue);
	OutputDebugString(pszBuffer);
}

