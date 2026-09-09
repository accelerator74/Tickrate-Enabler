/**
 * vim: set ts=4 :
 * =============================================================================
 * Tickrate Enabler
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
#ifndef __TICKRATE_ENABLER_H__
#define __TICKRATE_ENABLER_H__

#include "eiface.h"
#include "thirdparty/codepatch/patchmanager.h"

class CvarInfo
{
public:
	CvarInfo(const char* sName)
	{
		strncpy(m_sCvarName, sName, sizeof(m_sCvarName) - 1);
		m_sCvarName[sizeof(m_sCvarName) - 1] = '\0';
	}

	const char* GetName()
	{
		return m_sCvarName;
	}

private:
	char m_sCvarName[128];

public:
	float m_fDefaultMaxValue;
	bool m_bDefaultHasMax;
};

class L4DTickRate : public IServerPluginCallbacks
{
public:
	L4DTickRate();

	KHook::Return<float> GetTickInterval(const IServerGameDLL*);
	KHook::Virtual<IServerGameDLL, float> m_GetTickInterval;

	bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	void			Unload(void);
	void			Pause(void) {}
	void			UnPause(void) {}
	const char*		GetPluginDescription(void);
	void			LevelInit(char const* pMapName) {}
	void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {}
	void			GameFrame(bool simulating) {}
	void			LevelShutdown(void) {}
	void			ClientActive(edict_t* pEntity) {}
	void			ClientFullyConnect(edict_t* pEntity) {}
	void			ClientDisconnect(edict_t* pEntity) {}
	void			ClientPutInServer(edict_t* pEntity, char const* playername) {}
	void			SetCommandClient(int index) {}
	void			ClientSettingsChanged(edict_t* pEdict) {}
	PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return PLUGIN_CONTINUE; }
	PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args) { return PLUGIN_CONTINUE; }
	PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return PLUGIN_CONTINUE; }
	void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) {}
	void			OnEdictAllocated(edict_t* edict) {}
	void			OnEdictFreed(const edict_t* edict) {}

private:
	PatchManager m_patchManager;
};

#endif // __TICKRATE_ENABLER_H__
