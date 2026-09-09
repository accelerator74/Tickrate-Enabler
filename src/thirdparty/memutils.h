/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * =============================================================================
 * MemoryUtils
 * Copyright (C) 2004-2011 AlliedModders LLC., 2012 ProdigySim
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
 */

#ifndef _INCLUDE_SOURCEMOD_MEMORYUTILS_H_
#define _INCLUDE_SOURCEMOD_MEMORYUTILS_H_

#include <stdio.h>
#include "sm_platform.h"
#include <stdint.h>
//#include "common_logic.h"
#if defined PLATFORM_LINUX || defined PLATFORM_APPLE
#include "sm_symtable.h"
#include <vector>
#endif

#ifdef PLATFORM_APPLE
#include <CoreServices/CoreServices.h>
#endif

struct DynLibInfo
{
	void *baseAddress;
	size_t memorySize;
};

#if defined PLATFORM_LINUX || defined PLATFORM_APPLE
struct LibSymbolTable
{
	SymbolTable table;
	uintptr_t lib_base;
	uint32_t last_pos;
};
#endif

class MemoryUtils
{
public:
	MemoryUtils();
	~MemoryUtils();
	void *FindLibPattern(const void *libPtr, const char *pattern, size_t len);
	void *FindPattern(const void *start, const void *end, const char *pattern, size_t len);
	void *ResolveSymbol(void *handle, const char *symbol);
	void * SimpleResolve(void * pBaseAddr, const char * symbol);
	// Allows the memory to be written to
	void SetMemPatchable(void *pAddr, size_t nSize);

public:
	bool GetLibraryInfo(const void *libPtr, DynLibInfo &lib);
#if defined PLATFORM_LINUX || defined PLATFORM_APPLE
private:
	std::vector<LibSymbolTable *> m_SymTables;
#ifdef PLATFORM_APPLE
	struct dyld_all_image_infos *m_ImageList;
	SInt32 m_OSXMajor;
	SInt32 m_OSXMinor;
#endif
#endif
};

extern MemoryUtils g_MemUtils;

#endif // _INCLUDE_SOURCEMOD_MEMORYUTILS_H_
