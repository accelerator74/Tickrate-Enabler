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
#include <cstdlib>
#include "thirdparty/memutils.h"
#include "boomervomitpatch.h"
#include "patchexceptions.h"

struct fakeGlobals
{
	float padding[4];
	float frametime;
};

fakeGlobals g_FakeGlobals = { {0.0, 0.0, 0.0, 0.0}, 0.033333333 };
fakeGlobals* gp_FakeGlobals = &g_FakeGlobals;

#ifdef _LINUX
	#if defined (_L4D)
		fakeGlobals** gpp_FakeGlobals = &gp_FakeGlobals; // olol
	#endif
#endif

BoomerVomitFrameTimePatch::BoomerVomitFrameTimePatch(IServerGameDLL* gamedll)
{
	InitializeBinPatches(gamedll);
}

BoomerVomitFrameTimePatch::~BoomerVomitFrameTimePatch()
{
	m_patches.UnregisterAll();
}

void BoomerVomitFrameTimePatch::Patch()
{
	m_patches.PatchAll();
}

void BoomerVomitFrameTimePatch::Unpatch()
{
	m_patches.UnpatchAll();
}

void BoomerVomitFrameTimePatch::InitializeBinPatches(IServerGameDLL* gamedll)
{
	BYTE instr_buf[MAX_MOV_INSTR_LEN];

	BYTE* pCVomitUpdateAbility = FindCVomitUpdateAbility(static_cast<void*>(gamedll));
	DevMsg("CVomitUpdateAbility at 0x%08x\n", pCVomitUpdateAbility);

	if (!pCVomitUpdateAbility)
	{
		throw PatchException("Couldn't find CVomit::UpdateAbility() in server memory.");
	}

	for (size_t i = 0; i < NUM_FRAMETIME_READS; i++)
	{
		DevMsg("Setting up patch for frametime read %d (offs:0x%x).\n", i, g_FrameTimeReadOffsets[i]);

		// Calculate first offset target
		BYTE* pTarget = pCVomitUpdateAbility + g_FrameTimeReadOffsets[i];

		int offs = mov_src_operand_offset(pTarget); // Find offset of disp32 in this particular mov instruction
		if (offs == 0)
		{
			// Throw an exception if we can't identify this offset (unexpected instruction!)
			// TODO: More useful exception here.
			throw PatchException("CVomit::UpdateAbility() Patch Offset incorrect.");
		}

		memcpy(instr_buf, pTarget, MAX_MOV_INSTR_LEN);

		// make this instruction read from an immediate address
		mov_to_disp32(instr_buf);

		// Plug in our super cool immediate address.
#if defined (_WIN32)
		*(fakeGlobals***)(instr_buf + offs) = &gp_FakeGlobals;
#elif defined (_LINUX)
	#if defined (_L4D)
			*(fakeGlobals****)(instr_buf + offs) = &gpp_FakeGlobals;
	#elif defined (_L4D2)
			*(fakeGlobals***)(instr_buf + offs) = &gp_FakeGlobals;
	#endif
#endif

		// Generate BasicBinPatch
		m_patches.Register(new BasicStaticBinPatch<MAX_MOV_INSTR_LEN>(pTarget, instr_buf));
	}
}

BYTE* BoomerVomitFrameTimePatch::FindCVomitUpdateAbility(void* gamedll)
{
#if defined (_LINUX)
	return (BYTE*)g_MemUtils.SimpleResolve(gamedll, LIN_CVomit_UpdateAbility_SYMBOL);
#elif defined (_WIN32)
	return (BYTE*)g_MemUtils.FindLibPattern(gamedll, WIN_CVomit_UpdateAbility_SIG, WIN_CVomit_UpdateAbility_SIGLEN);
#endif
}
