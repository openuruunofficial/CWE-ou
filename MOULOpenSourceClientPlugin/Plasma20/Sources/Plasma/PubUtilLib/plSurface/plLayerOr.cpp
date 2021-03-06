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
#include "plLayerOr.h"

plLayerOr::plLayerOr()
{
	fState = TRACKED_NEW hsGMatState;
	fState->Reset();

	fOwnedChannels = kState;
}

plLayerOr::~plLayerOr()
{
}

void	plLayerOr::SetState( const hsGMatState& state )
{
	SetBlendFlags( state.fBlendFlags );
	SetClampFlags( state.fClampFlags );
	SetShadeFlags( state.fShadeFlags );
	SetZFlags( state.fZFlags );
	SetMiscFlags( state.fMiscFlags );
}

plLayerInterface	*plLayerOr::Attach( plLayerInterface* prev )
{
	fDirty = true;

	return plLayerInterface::Attach( prev );
}

UInt32 plLayerOr::Eval(double secs, UInt32 frame, UInt32 ignore)
{
	UInt32 ret = plLayerInterface::Eval(secs, frame, ignore);
	if( fUnderLay )
	{
		if( fDirty || (ret & kState) )
		{
			*fState = fUnderLay->GetState();
			if( fOringState.fBlendFlags & hsGMatState::kBlendMask )
				fState->fBlendFlags &= ~hsGMatState::kBlendMask;
			fState->fBlendFlags |= fOringState.fBlendFlags;

			fState->fClampFlags = fUnderLay->GetClampFlags() | fOringState.fClampFlags;
			fState->fShadeFlags = fUnderLay->GetShadeFlags() | fOringState.fShadeFlags;
			fState->fZFlags = fUnderLay->GetZFlags() | fOringState.fZFlags;
			fState->fMiscFlags = fUnderLay->GetMiscFlags() | fOringState.fMiscFlags;

			fDirty = false;
		}
	}
	return ret;
}
