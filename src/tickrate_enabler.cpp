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

#include "thirdparty/memutils.h"
#include "thirdparty/sm_convar.h"
#include "tier0/icommandline.h"

#include "sourcehook.h"
#include "sourcehook_impl.h"
#include "sourcehook_impl_chookidman.h"

#include "thirdparty/codepatch/patchmanager.h"

#include "tickrate_enabler.h"
#include "patchexceptions.h"

#include "boomervomitpatch.h"
#include "maxrate_patches.h"

 // memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define TE_VERSION "1.6.2"

L4DTickRate g_L4DTickRatePlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(L4DTickRate, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_L4DTickRatePlugin);

SourceHook::Impl::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook* g_SHPtr = &g_SourceHook;
int g_PLID = 0;

SH_DECL_HOOK0(IServerGameDLL, GetTickInterval, const, 0, float);

IServerGameDLL* g_pGameDll = NULL;
IVEngineServer* g_pEngine = NULL;
ICvar* g_pCvar = NULL;

CvarInfo g_ResetCvars[] = { "sv_maxrate", "sv_minrate", "net_splitpacket_maxrate" };
size_t g_iResetCvarsCount = sizeof(g_ResetCvars) / sizeof(g_ResetCvars[0]);

static float Handler_GetTickInterval()
{
	float fDefTickInterval = SH_CALL(g_pGameDll, &IServerGameDLL::GetTickInterval)();
	float fTickInterval = fDefTickInterval;

	if (CommandLine()->CheckParm("-tickrate")) {
		float fTickRate = CommandLine()->ParmValue("-tickrate", 0);

		if (fTickRate > 10) {
			fTickInterval = 1.0f / fTickRate;

			Msg("[Tickrate_Enabler] Applied tickrate %.2f (interval %.6f); Default tickrate: %.2f (interval: %.6f).\n", fTickRate, fTickInterval, 1.0f / fDefTickInterval, fDefTickInterval);
		}
	}

	RETURN_META_VALUE(MRES_SUPERCEDE, fTickInterval);
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool L4DTickRate::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	g_pGameDll = reinterpret_cast<IServerGameDLL*>(gameServerFactory(INTERFACEVERSION_SERVERGAMEDLL, NULL));
	if (g_pGameDll == NULL) {
		Error("[Tickrate_Enabler] Failed to get a pointer on 'IServerGameDLL' interface.\n");
		return false;
	}

	g_pEngine = reinterpret_cast<IVEngineServer*>(interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL));
	if (g_pEngine == NULL) {
		Error("[Tickrate_Enabler] Failed to get a pointer on 'IVEngineServer' interface.\n");
		return false;
	}

	g_pCvar = reinterpret_cast<ICvar*>(interfaceFactory(CVAR_INTERFACE_VERSION, NULL));
	if (g_pCvar == NULL) {
		Error("[Tickrate_Enabler] Failed to get a pointer on 'ICvar' interface.\n");
		return false;
	}

	SH_ADD_HOOK(IServerGameDLL, GetTickInterval, g_pGameDll, SH_STATIC(Handler_GetTickInterval), false);

	try {
		m_patchManager.Register(new BoomerVomitFrameTimePatch(g_pGameDll));
		m_patchManager.Register(new NetChanDataRatePatch((BYTE*)g_pEngine));
#if defined (SIG_CGAMECLIENT_SETDATARATE)
		m_patchManager.Register(new GameClientSetRatePatch((BYTE*)g_pEngine));
#endif

		m_patchManager.Register(new ClampClientRatePatch((BYTE*)g_pEngine));

		m_patchManager.PatchAll();
	} catch (PatchException& e) {
		Error("!!!!!\nPatch Failure: %s\n!!!!!\n", e.GetDescription());
		Error("Failed to process all tickrate_enabler patches, bailing out.\n");
		return false;
	}

	for (size_t i = 0; i < g_iResetCvarsCount; i++) {
		ConVar* pCvar = g_pCvar->FindVar(g_ResetCvars[i].GetName());

		if (pCvar == NULL) {
			Msg("[Tickrate_Enabler] Can't find convar '%s'.""\n", g_ResetCvars[i].GetName());
			continue;
		}

		g_ResetCvars[i].m_bDefaultHasMax = pCvar->m_bHasMax;
		g_ResetCvars[i].m_fDefaultMaxValue = pCvar->m_fMaxVal;

		pCvar->m_bHasMax = false;
		pCvar->m_fMaxVal = 0.0f;
	}

	return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void L4DTickRate::Unload(void)
{
	for (size_t i = 0; i < g_iResetCvarsCount; i++) {
		ConVar* pCvar = g_pCvar->FindVar(g_ResetCvars[i].GetName());

		if (pCvar == NULL) {
			Msg("[Tickrate_Enabler] Can't find convar '%s'.""\n", g_ResetCvars[i].GetName());
			continue;
		}

		pCvar->m_bHasMax = g_ResetCvars[i].m_bDefaultHasMax;
		pCvar->m_fMaxVal = g_ResetCvars[i].m_fDefaultMaxValue;
	}

	m_patchManager.UnpatchAll();
	m_patchManager.UnregisterAll();

	SH_REMOVE_HOOK(IServerGameDLL, GetTickInterval, g_pGameDll, SH_STATIC(Handler_GetTickInterval), false);
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char* L4DTickRate::GetPluginDescription(void)
{
#ifdef _DEBUG
	return "Tickrate_Enabler " TE_VERSION " (dev), ProdigySim.";
#else 
	return "Tickrate_Enabler " TE_VERSION ", ProdigySim.";
#endif
}
