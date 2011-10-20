// Stolen from: http://www.mvps.org/user32/webhost.cab
// No copyright notices, so I assume it's public domain -Colin

#include "hsConfig.h"
#if HS_BUILD_FOR_WIN32

#pragma once

#include <windows.h>


struct basewnd
{
	static wchar_t szClassName[];
	static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static void Initialize(HINSTANCE hAppInstance,UINT style=0);
	
	HWND hwnd;
	ULONG mcRef;
	basewnd();
	virtual ~basewnd();
	
public:
	virtual ULONG AddRef();
	virtual ULONG Release();
	virtual BOOL HandleMessage(UINT,WPARAM,LPARAM,LRESULT*)=0;
	
public: // inline overrides
	BOOL ShowWindow(int nCmdShow){return ::ShowWindow(hwnd,nCmdShow);}
	BOOL UpdateWindow(void){return ::UpdateWindow(hwnd);}
};

#endif