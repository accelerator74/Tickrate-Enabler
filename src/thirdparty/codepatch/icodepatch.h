/**
 * vim: set ts=4 :
 * =============================================================================
 * ICodePatch
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

#ifndef _INCLUDE_SOURCEMOD_ICODEPATCH_H_
#define _INCLUDE_SOURCEMOD_ICODEPATCH_H_

/*
A simple interface for a patch that can change code memory or restore it to the original state

NOTE: To use this with PatchManager make sure to inherit public ICodePatch
*/
class ICodePatch
{
public:
	/*
		patch the code memory
	*/
	virtual void Patch() = 0;

	/*
		unpatch the code memory, restoring it to its original state
	*/
	virtual void Unpatch() = 0;

	/*
		so that we can delete
	*/
	virtual ~ICodePatch() {}
};

#endif // _INCLUDE_SOURCEMOD_ICODEPATCH_H_
