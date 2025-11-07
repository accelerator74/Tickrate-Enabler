/**
 * vim: set ts=4 :
 * =============================================================================
 * MaxRate Patches
 * Copyright (C) 2012 Michael "ProdigySim" Busby
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, the author(s) give you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, the author(s) grant
 * this exception to all derivative works.  The author(s) define further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */
#ifndef __MAXRATE_PATCHES_H__
#define __MAXRATE_PATCHES_H__

#include "thirdparty/codepatch/icodepatch.h"
#include "misc_asm.h"

#if defined (_WIN32)
	#define CGAMECLIENT_PATCH
#if defined (_L4D2)
	#define CLAMPCLIENTRATE_PATCH
#endif
#elif defined (_LINUX)
	#define CLAMPCLIENTRATE_PATCH
#if defined (_L4D)
	#define CGAMECLIENT_PATCH
#endif
#endif

class NetChanDataRatePatch : public ICodePatch
{
public:
	NetChanDataRatePatch(BYTE* engine);
	~NetChanDataRatePatch();
	void Patch();
	void Unpatch();

private:
	BYTE* FindCNetChanSetDataRate(BYTE* engine);
	ICodePatch* GeneratePatch(BYTE* pCNetChanSetDataRate);
	ICodePatch* m_patch;
};

#if defined (CGAMECLIENT_PATCH)
class GameClientSetRatePatch : public ICodePatch
{
public:
	GameClientSetRatePatch(BYTE* engine);
	~GameClientSetRatePatch();
	void Patch();
	void Unpatch();

private:
	BYTE* FindCGameClientSetRate(BYTE* engine);
	ICodePatch* GeneratePatch(BYTE* pCGameClientSetRate);
	ICodePatch* m_patch;
};
#endif

#if defined (CLAMPCLIENTRATE_PATCH)
class ClampClientRatePatch : public ICodePatch
{
public:
	ClampClientRatePatch(BYTE* engine);
	~ClampClientRatePatch();
	void Patch();
	void Unpatch();

private:
	BYTE* FindClampClientRate(BYTE* engine);
	ICodePatch* GeneratePatch(BYTE* pClampClientRate);
	ICodePatch* m_patch;
};
#endif

#endif // __MAXRATE_PATCHES_H__
