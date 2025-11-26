/**
 * vim: set ts=4 :
 * =============================================================================
 * Patch Manager
 * Copyright (C) 2012 Michael "ProdigySim" Busby
 * Copyright (C) 2009 Igor "Downtown1" Smirnov.
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

#include "patchmanager.h"

/*
	register a code patch
*/
void PatchManager::Register(ICodePatch* patch)
{
	patchList.push_back(patch);
}

/*
	run Patch() on all registered code patches
*/
void PatchManager::PatchAll()
{
	for (PatchList::iterator iter = patchList.begin(); iter != patchList.end(); ++iter)
	{
		ICodePatch* patch = *iter;
		patch->Patch();
	}
}

/*
	run Unpatch() on all registered code patches
*/
void PatchManager::UnpatchAll()
{
	for (PatchList::iterator iter = patchList.begin(); iter != patchList.end(); ++iter)
	{
		ICodePatch* patch = *iter;
		patch->Unpatch();
	}
}

/*
	unregister all code patches, deleting every one of them
*/
void PatchManager::UnregisterAll()
{
	for (PatchList::iterator iter = patchList.begin(); iter != patchList.end(); ++iter)
	{
		ICodePatch* patch = *iter;
		delete patch;
	}

	patchList.clear();
}

PatchManager::~PatchManager()
{
	UnregisterAll();
}
