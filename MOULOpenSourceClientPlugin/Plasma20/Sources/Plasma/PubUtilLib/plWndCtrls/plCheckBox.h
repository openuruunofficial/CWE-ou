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
#ifndef plCheckBox_h_inc
#define plCheckBox_h_inc


class plCheckBox : public plButton
{
	DECLARE_WINDOWSUBCLASS(plCheckBox,plButton)
	plCheckBox()
	{}
	plCheckBox( plWindow * inOwner, int inId=0, plDelegate inClicked=plDelegate(), WNDPROC inSuperProc=nil )
	: plButton( inOwner, inId, inClicked, inSuperProc?inSuperProc:_SuperProc )
	{}
	void OpenWindow( bool visible, int X, int Y, int XL, int YL, const wchar_t * text )
	{
		PerformCreateWindowEx
		(
			0,
            nil,
            WS_CHILD|BS_CHECKBOX,
            X, Y,
			XL, YL,
            *fOwnerWindow,
            (HMENU)fControlID,
            plWndCtrls::Instance()
		);
		SendMessage( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
		SetText( text );
		if( visible )
			ShowWindow( *this, SW_SHOWNOACTIVATE );
	}
	void Check(bool check=true)
	{
		SendMessage(*this,BM_SETCHECK,(check)?BST_CHECKED:BST_UNCHECKED,0);
	}
	bool IsChecked() const
	{
		return (SendMessage(*this,BM_GETCHECK,0,0)==BST_CHECKED);
	}
	bool IsUnchecked() const
	{
		return !IsChecked();
	}
	std::string IGetValue() const
	{
		char tmp[20];
		sprintf(tmp,"%s",IsChecked()?"true":"false");
		return tmp;
	}
	void ISetValue(const char * value)
	{
		if (stricmp(value,"true")==0)
			Check(true);
		else if (stricmp(value,"false")==0)
			Check(false);
		else
			Check(atoi(value)?true:false);
	}
};

#endif // plCheckBox_h_inc
