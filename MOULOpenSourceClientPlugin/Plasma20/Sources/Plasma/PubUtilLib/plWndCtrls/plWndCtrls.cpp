/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011 Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/
#include "hsTypes.h"

#include "plWndCtrls.h"
#include "basewnd.h"

#if HS_BUILD_FOR_WIN32

////////////////////////////////////////////////////////////////////

int plWindow::_ModalCount = 0;
std::vector<plWindow*> plWindow::_Windows;
std::vector<plWindow*> plWindow::_DeleteWindows;

////////////////////////////////////////////////////////////////////

WNDPROC plButton::_SuperProc = nil;
WNDPROC plCheckBox::_SuperProc = nil;
WNDPROC plRadioButton::_SuperProc = nil;
WNDPROC plEdit::_SuperProc = nil;
WNDPROC plLabel::_SuperProc = nil;
WNDPROC plProgressBar::_SuperProc = nil;
WNDPROC plTrackBar::_SuperProc = nil;
WNDPROC plComboBox::_SuperProc = nil;
WNDPROC plListBox::_SuperProc = nil;
WNDPROC plStatusBar::_SuperProc = nil;

////////////////////////////////////////////////////////////////////

HINSTANCE	plWndCtrls::hInstance = nil;
DWORD plWndCtrls::fLanguage = 0;

////////////////////////////////////////////////////////////////////

void plWndCtrls::Init(HINSTANCE hInst)
{
	hInstance = hInst;

	CoInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);
 
	REGISTER_WINDOWSUBCLASS(plEdit,L"EDIT");
	REGISTER_WINDOWSUBCLASS(plButton,L"BUTTON");
	REGISTER_WINDOWSUBCLASS(plCheckBox,L"BUTTON");
	REGISTER_WINDOWSUBCLASS(plRadioButton,L"BUTTON");
	REGISTER_WINDOWSUBCLASS(plLabel,L"STATIC");
	REGISTER_WINDOWSUBCLASS(plComboBox,L"COMBOBOX");
	REGISTER_WINDOWSUBCLASS(plListBox,L"LISTBOX");
	REGISTER_WINDOWSUBCLASS(plTrackBar,TRACKBAR_CLASS);
	REGISTER_WINDOWSUBCLASS(plProgressBar,PROGRESS_CLASS);
	REGISTER_WINDOWSUBCLASS(plStatusBar,STATUSCLASSNAME);

	basewnd::Initialize( hInst );
}

void plWndCtrls::Shutdown()
{
	CoUninitialize();
}


////////////////////////////////////////////////////////////////////
#endif	// HS_BUILD_FOR_WIN32
