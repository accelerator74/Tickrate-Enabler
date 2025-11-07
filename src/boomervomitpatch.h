/**
 * vim: set ts=4 :
 * =============================================================================
 * BoomerVomitPatch
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
#ifndef __BOOMERVOMITPATCH_H__
#define __BOOMERVOMITPATCH_H__

#include "eiface.h"
#include "thirdparty/codepatch/icodepatch.h"
#include "thirdparty/codepatch/patchmanager.h"
#include "basicbinpatch.h"
#include "misc_asm.h"
#include "sigs.h"

class BoomerVomitFrameTimePatch : public ICodePatch
{
public:
	BoomerVomitFrameTimePatch(IServerGameDLL* gamedll);
	~BoomerVomitFrameTimePatch();
	void Patch();
	void Unpatch();
private:
	void InitializeBinPatches(IServerGameDLL* gamedll);
	BYTE* FindCVomitUpdateAbility(void* gamedll);
	PatchManager m_patches;
};

#endif // __BOOMERVOMITPATCH_H__
