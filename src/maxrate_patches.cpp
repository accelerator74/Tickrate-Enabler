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


#include "maxrate_patches.h"
#include "thirdparty/memutils.h"
#include "patchexceptions.h"
#include "basicbinpatch.h"

NetChanDataRatePatch::NetChanDataRatePatch(BYTE* engine) : m_patch(NULL)
{
	m_patch = GeneratePatch(FindCNetChanSetDataRate(engine));
}

NetChanDataRatePatch::~NetChanDataRatePatch()
{
	if (m_patch != NULL) delete m_patch;
}

void NetChanDataRatePatch::Patch()
{
	m_patch->Patch();
}

void NetChanDataRatePatch::Unpatch()
{
	m_patch->Unpatch();
}

BYTE* NetChanDataRatePatch::FindCNetChanSetDataRate(BYTE* engine)
{
#if defined (_LINUX)
	return (BYTE*)g_MemUtils.SimpleResolve(engine, SIG_CNETCHAN_SETDATARATE);
#elif defined (_WIN32)
	return (BYTE*)g_MemUtils.FindLibPattern(engine, SIG_CNETCHAN_SETDATARATE, SIG_CNETCHAN_SETDATARATE_LEN);
#endif
}

ICodePatch* NetChanDataRatePatch::GeneratePatch(BYTE* pCNetChanSetDataRate)
{
	if (!pCNetChanSetDataRate)
	{
		throw PatchException("Unable to find CNetChan::SetDataRate!");
	}

	if (pCNetChanSetDataRate[CNETCHAN_PATCH_OFFSET] != CNETCHAN_PATCH_CHECK_BYTE)
	{
		throw PatchException("CNetChan::SetDataRate patch offset incorrect!");
	}

#if defined _WIN32
	const BYTE replacement[JMP_8_INSTR_LEN] = { JMP_8_OPCODE, CNETCHAN_PATCH_JUMP_OFFSET };
	return new BasicStaticBinPatch<sizeof(replacement)>(pCNetChanSetDataRate + CNETCHAN_PATCH_OFFSET, replacement);
#elif defined _LINUX
	return new BasicStaticBinPatch<sizeof(NOP_2)>(pCNetChanSetDataRate + CNETCHAN_PATCH_OFFSET, NOP_2);
#endif
}

#if defined (SIG_CGAMECLIENT_SETDATARATE)
GameClientSetRatePatch::GameClientSetRatePatch(BYTE* engine) : m_patch(NULL)
{
	m_patch = GeneratePatch(FindCGameClientSetRate(engine));
}

GameClientSetRatePatch::~GameClientSetRatePatch()
{
	if (m_patch != NULL) delete m_patch;
}

void GameClientSetRatePatch::Patch()
{
	m_patch->Patch();
}

void GameClientSetRatePatch::Unpatch()
{
	m_patch->Unpatch();
}

BYTE* GameClientSetRatePatch::FindCGameClientSetRate(BYTE* engine)
{
#if defined (_LINUX)
	return (BYTE*)g_MemUtils.SimpleResolve(engine, SIG_CGAMECLIENT_SETDATARATE);
#elif defined (_WIN32)
	return (BYTE*)g_MemUtils.FindLibPattern(engine, SIG_CGAMECLIENT_SETDATARATE, SIG_CGAMECLIENT_SETDATARATE_LEN);
#endif
}

ICodePatch* GameClientSetRatePatch::GeneratePatch(BYTE* pCGameClientSetRate)
{
	if (!pCGameClientSetRate)
	{
		throw PatchException("Unable to find CGameClient::SetRate!");
	}

	if (pCGameClientSetRate[CGAMECLIENT_PATCH_OFFSET] != CGAMECLIENT_PATCH_CHECK_BYTE)
	{
		throw PatchException("CGameClient::SetRate patch offset incorrect!");
	}

#if defined _WIN32
	const BYTE replacement[JMP_8_INSTR_LEN] = { JMP_8_OPCODE, CGAMECLIENT_PATCH_JUMP_OFFSET };
	return new BasicStaticBinPatch<sizeof(replacement)>(pCGameClientSetRate + CGAMECLIENT_PATCH_OFFSET, replacement);
#elif defined _LINUX
	BYTE replacement[MOV_R32_R32_INSTR_LEN + sizeof(NOP_9) + sizeof(NOP_3)] = { MOV_R32_RM32_OPCODE, MODRM_REG_EAX_EDX, 0,0,0,0,0,0,0,0,0,0,0,0 };
	memcpy(&replacement[2], NOP_9, sizeof(NOP_9));
	memcpy(&replacement[11], NOP_3, sizeof(NOP_3));
	return new BasicStaticBinPatch<sizeof(replacement)>(pCGameClientSetRate + CGAMECLIENT_PATCH_OFFSET, replacement);
#endif
}
#endif

ClampClientRatePatch::ClampClientRatePatch(BYTE* engine) : m_patch(NULL)
{
	m_patch = GeneratePatch(FindClampClientRate(engine));
}

ClampClientRatePatch::~ClampClientRatePatch()
{
	if (m_patch != NULL) delete m_patch;
}

void ClampClientRatePatch::Patch()
{
	m_patch->Patch();
}

void ClampClientRatePatch::Unpatch()
{
	m_patch->Unpatch();
}

BYTE* ClampClientRatePatch::FindClampClientRate(BYTE* engine)
{
#if defined _LINUX
	return (BYTE*)g_MemUtils.SimpleResolve(engine, SIG_CLAMPCLIENTRATE);
#elif defined _WIN32
	return (BYTE*)g_MemUtils.FindLibPattern(engine, SIG_CLAMPCLIENTRATE, SIG_CLAMPCLIENTRATE_LEN);
#endif
}

ICodePatch* ClampClientRatePatch::GeneratePatch(BYTE* pClampClientRate)
{
	if (!pClampClientRate)
	{
		throw PatchException("Unable to find ClampClientRate!");
	}

	if (pClampClientRate[CLAMPCLIENTRATE_PATCH_OFFSET] != CLAMPCLIENTRATE_PATCH_CHECK_BYTE)
	{
		throw PatchException("ClampClientRate patch offset incorrect!");
	}

#if defined _WIN32
	const BYTE replacement[JMP_8_INSTR_LEN] = { JMP_8_OPCODE, CLAMPCLIENTRATE_PATCH_JUMP_OFFSET };
	return new BasicStaticBinPatch<sizeof(replacement)>(pClampClientRate + CLAMPCLIENTRATE_PATCH_OFFSET, replacement);
#elif defined (_LINUX)
	BYTE replacement[MOV_R32_R32_INSTR_LEN + sizeof(NOP_9) + sizeof(NOP_3)] = { MOV_R32_RM32_OPCODE, MODRM_REG_EAX_EDX, 0,0,0,0,0,0,0,0,0,0,0,0 };
	memcpy(&replacement[2], NOP_9, sizeof(NOP_9));
	memcpy(&replacement[11], NOP_3, sizeof(NOP_3));
	return new BasicStaticBinPatch<sizeof(replacement)>(pClampClientRate + CLAMPCLIENTRATE_PATCH_OFFSET, replacement);
#endif
}
