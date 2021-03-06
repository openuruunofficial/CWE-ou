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
/*****************************************************************************
*
*   $/Plasma20/Sources/Plasma/NucleusLib/pnNetBase/Private/pnNbSrvs.cpp
*   
***/

#include "../Pch.h"
#pragma hdrstop

#ifndef BUILD_TYPE
# error "pnProduct not included"
#endif


/*****************************************************************************
*
*   Local data
*
***/

//============================================================================
// Auth
//============================================================================
static const wchar * s_authAddrs[] = {

#if BUILD_TYPE == BUILD_TYPE_DEV
	L"shard"
#elif BUILD_TYPE == BUILD_TYPE_QA
	L"marrim"
#elif BUILD_TYPE == BUILD_TYPE_TEST
	L"test-auth.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_BETA
	L"beta-auth.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_LIVE
	L"184.73.198.22"  //L"auth.urulive.com"
#else
# error "Unknown build type"
#endif

};
static wchar s_authAddrCmdLine[64];
static const wchar * s_authAddrsOverride[] = {
	s_authAddrCmdLine
};


//============================================================================
// File
//============================================================================
static const wchar * s_fileAddrs[] = {

#if BUILD_TYPE == BUILD_TYPE_DEV
	L"shard"
#elif BUILD_TYPE == BUILD_TYPE_QA
	L"marrim"
#elif BUILD_TYPE == BUILD_TYPE_TEST
	L"test-file.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_BETA
	L"beta-file.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_LIVE
	L"67.202.54.141" //unused
#else
# error "Unknown build type"
#endif

};
static wchar s_fileAddrCmdLine[64];
static const wchar * s_fileAddrsOverride[] = {
	s_fileAddrCmdLine
};


//============================================================================
// Csr
//============================================================================
static const wchar * s_csrAddrs[] = {

#if BUILD_TYPE == BUILD_TYPE_DEV
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_QA
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_TEST
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_BETA
	L"beta-csr.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_LIVE
	L"localhost"
#else
# error "Unknown build type"
#endif

};
static wchar s_csrAddrCmdLine[64];
static const wchar * s_csrAddrsOverride[] = {
	s_csrAddrCmdLine
};


//============================================================================
// GateKeeper
//============================================================================
static const wchar * s_gateKeeperAddrs[] = {

#if BUILD_TYPE == BUILD_TYPE_DEV
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_QA
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_TEST
	L"localhost"
#elif BUILD_TYPE == BUILD_TYPE_BETA
	L"beta-csr.urulive.com"
#elif BUILD_TYPE == BUILD_TYPE_LIVE
	L"184.73.198.22"
#else
# error "Unknown build type"
#endif

};
static wchar s_gateKeeperAddrCmdLine[64];
static const wchar * s_gateKeeperAddrsOverride[] = {
	s_gateKeeperAddrCmdLine
};


/*****************************************************************************
*
*   Exports
*
***/

//============================================================================
// Auth
//============================================================================
unsigned GetAuthSrvHostnames (const wchar *** addrs) {

	if (s_authAddrCmdLine[0]) {
		*addrs = s_authAddrsOverride;
		return arrsize(s_authAddrsOverride);
	}
	else {
		*addrs = s_authAddrs; 
		return arrsize(s_authAddrs);
	}
}

//============================================================================
void SetAuthSrvHostname (const wchar addr[]) {

	StrCopy(s_authAddrCmdLine, addr, arrsize(s_authAddrCmdLine));
}

//============================================================================
bool AuthSrvHostnameOverride () {

	return s_authAddrCmdLine[0];
}

//============================================================================
// File
//============================================================================
unsigned GetFileSrvHostnames (const wchar *** addrs) {

	if (s_fileAddrCmdLine[0]) {
		*addrs = s_fileAddrsOverride;
		return arrsize(s_fileAddrsOverride);
	}
	else {
		*addrs = s_fileAddrs; 
		return arrsize(s_fileAddrs);
	}
}

//============================================================================
void SetFileSrvHostname (const wchar addr[]) {

	StrCopy(s_fileAddrCmdLine, addr, arrsize(s_fileAddrCmdLine));
}

//============================================================================
bool FileSrvHostnameOverride () {

	return s_fileAddrCmdLine[0];
}

//============================================================================
// Csr
//============================================================================
unsigned GetCsrSrvHostnames (const wchar *** addrs) {

	if (s_csrAddrCmdLine[0]) {
		*addrs = s_csrAddrsOverride;
		return arrsize(s_csrAddrsOverride);
	}
	else {
		*addrs = s_csrAddrs; 
		return arrsize(s_csrAddrs);
	}
}

//============================================================================
void SetCsrSrvHostname (const wchar addr[]) {

	StrCopy(s_csrAddrCmdLine, addr, arrsize(s_csrAddrCmdLine));
}

//============================================================================
bool CsrSrvHostnameOverride () {

	return s_csrAddrCmdLine[0];
}


//============================================================================
// GateKeeper
//============================================================================
unsigned GetGateKeeperSrvHostnames (const wchar *** addrs) {

	if (s_gateKeeperAddrCmdLine[0]) {
		*addrs = s_gateKeeperAddrsOverride;
		return arrsize(s_gateKeeperAddrsOverride);
	}
	else {
		*addrs = s_gateKeeperAddrs; 
		return arrsize(s_gateKeeperAddrs);
	}
}

//============================================================================
void SetGateKeeperSrvHostname (const wchar addr[]) {
	StrCopy(s_gateKeeperAddrCmdLine, addr, arrsize(s_gateKeeperAddrCmdLine));
}

//============================================================================
bool GateKeeperSrvHostnameOverride () {
	return s_gateKeeperAddrCmdLine[0];
}

