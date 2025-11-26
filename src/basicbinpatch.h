/**
 * vim: set ts=4 :
 * =============================================================================
 * BasicBinPatch
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
#ifndef _BASICBINPATCH_H_
#define _BASICBINPATCH_H_

#include "thirdparty/codepatch/icodepatch.h"
#include "thirdparty/memutils.h"


// Basic binary patch that can be calculated and cached ahead of time
// Only needing to store the "restore" and "replacement" bytes in order for infinite patch/unpatch cycles
class BasicBinPatch : public ICodePatch
{
public:
	// target: ptr to code to be patched
	// replacement: ptr to byte array containing patch (replacement) bytes
	// length: length of patch
	// patchTimeCopy: Should we copy the original code back out every time we Patch, or is during construction enough?
	BasicBinPatch(BYTE* target, const BYTE* replacement, size_t length, bool patchTimeCopy = false)
		: m_length(length), m_pTarget(target), m_pReplacement(NULL), m_pOriginal(NULL),
		m_bFirstTime(true), m_bPatched(false), m_bPatchTimeCopy(patchTimeCopy)
	{
		m_pOriginal = new BYTE[length + length];
		m_pReplacement = m_pOriginal + length;

		memcpy(m_pReplacement, replacement, length);
		if (!patchTimeCopy)
		{
			memcpy(m_pOriginal, target, length);
		}
	}

	~BasicBinPatch()
	{
		delete m_pOriginal;
	}

	void Patch()
	{
		if (m_bPatched) return;

		if (m_bFirstTime)
		{
			g_MemUtils.SetMemPatchable(m_pTarget, m_length);
			m_bFirstTime = false;
		}
		if (m_bPatchTimeCopy)
		{
			memcpy(m_pOriginal, m_pTarget, m_length);
		}
		memcpy(m_pTarget, m_pReplacement, m_length);
		m_bPatched = true;
	}

	void Unpatch()
	{
		if (!m_bPatched) return;
		memcpy(m_pTarget, m_pOriginal, m_length);
	}

private:
	size_t m_length;
	BYTE* m_pTarget;
	BYTE* m_pReplacement;
	BYTE* m_pOriginal;
	bool m_bFirstTime;
	bool m_bPatched;
	bool m_bPatchTimeCopy;
};

// Basic binary patch that can be calculated and cached ahead of time, and size is known at compile time
// Only needing to store the "restore" and "replacement" bytes in order for infinite patch/unpatch cycles
template <size_t LENGTH>
class BasicStaticBinPatch : public ICodePatch
{
public:
	// target: ptr to code to be patched
	// replacement: ptr to byte array containing patch (replacement) bytes
	// patchTimeCopy: Should we copy the original code back out every time we Patch, or is during construction enough?
	BasicStaticBinPatch(BYTE* target, const BYTE* replacement, bool patchTimeCopy = false)
		: m_pTarget(target), m_bFirstTime(true), m_bPatchTimeCopy(patchTimeCopy), m_bPatched(false)
	{
		memcpy(m_replacement, replacement, LENGTH);
		if (!patchTimeCopy)
		{
			memcpy(m_original, target, LENGTH);
		}
	}

	void Patch()
	{
		if (m_bPatched) return;

		if (m_bFirstTime)
		{
			g_MemUtils.SetMemPatchable(m_pTarget, LENGTH);
			m_bFirstTime = false;
		}
		if (m_bPatchTimeCopy)
		{
			memcpy(m_original, m_pTarget, LENGTH);
		}
		memcpy(m_pTarget, m_replacement, LENGTH);
		m_bPatched = true;
	}

	void Unpatch()
	{
		if (!m_bPatched) return;

		memcpy(m_pTarget, m_original, LENGTH);
	}

private:
	BYTE* m_pTarget;
	BYTE m_replacement[LENGTH];
	BYTE m_original[LENGTH];
	bool m_bFirstTime;
	bool m_bPatchTimeCopy;
	bool m_bPatched;
};

#endif // _BASICBINPATCH_H_
