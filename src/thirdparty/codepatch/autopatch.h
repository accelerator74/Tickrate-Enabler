/**
 * vim: set ts=4 :
 * =============================================================================
 * Autopatch
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

#ifndef _INCLUDE_SOURCEMOD_AUTOPATCH_H_
#define _INCLUDE_SOURCEMOD_AUTOPATCH_H_

#include "icodepatch.h"

/*
use this class to automatically construct a codepatch and then have it call patch
*/
template <typename TPatchable>
class AutoPatch : public ICodePatch
{
public:
	AutoPatch() : codePatch()
	{
		//L4D_DEBUG_LOG("AutoPatch constructor");
		Patch(); //note: codePatch.Unpatch() is called automatically by its own destructor.. if it wants to
	}

	~AutoPatch()
	{
		//L4D_DEBUG_LOG("AutoPatch destructor");
	}

	/*
		patch the code memory
	*/
	void Patch()
	{
		codePatch.Patch();
	}

	/*
		unpatch the code memory, restoring it to its original state
	*/
	void Unpatch()
	{
		codePatch.Unpatch();
	}

	/*
		get the underlying ICodePatch if we need to access it directly for some reason
	*/
	TPatchable& GetCodePatch()
	{
		return codePatch;
	}

private:
	TPatchable codePatch;
};

#endif // _INCLUDE_SOURCEMOD_AUTOPATCH_H_
