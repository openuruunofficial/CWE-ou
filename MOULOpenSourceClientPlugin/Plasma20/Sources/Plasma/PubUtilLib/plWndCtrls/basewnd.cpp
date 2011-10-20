#include "hsTypes.h"


// Stolen from: http://www.mvps.org/user32/webhost.cab
// No copyright notices, so I assume it's public domain -Colin
#include "basewnd.h"

#include "hsConfig.h"
#if HS_BUILD_FOR_WIN32


wchar_t basewnd::szClassName[] = L"BASEWND";


void basewnd::Initialize(HINSTANCE hAppInstance,UINT style)
{
	WNDCLASSEX wc = 
	{
		sizeof(wc),
		style,
		WindowProc,
		0,0,
		hAppInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		szClassName,
		LoadIcon(NULL,IDI_APPLICATION)
	};
	RegisterClassEx(&wc);
}


LRESULT CALLBACK basewnd::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	basewnd* _this;
	if(uMsg == WM_CREATE && (_this = (basewnd*)(LPCREATESTRUCT(lParam))->lpCreateParams))
	{
		SetWindowLong(hwnd,GWL_USERDATA,(long)_this);
		_this->hwnd = hwnd;
		_this->AddRef();
	}
	else
		_this = (basewnd*)GetWindowLong(hwnd,GWL_USERDATA);
	
	LRESULT result = 0;
	BOOL fDoDef = !(_this && _this->HandleMessage(uMsg,wParam,lParam,&result));
	
	if(uMsg == WM_DESTROY)
	{
		SetWindowLong(hwnd,GWL_USERDATA,(long)NULL);
		_this->Release();
	}
	
	return fDoDef?DefWindowProc(hwnd,uMsg,wParam,lParam):result;
}


basewnd::basewnd()
: mcRef(1)
{
}


basewnd::~basewnd()
{
}

ULONG basewnd::AddRef()
{
	return mcRef++;
}

ULONG basewnd::Release()
{
	if(--mcRef)
		return mcRef;
	delete this;
	return 0;
}


#endif