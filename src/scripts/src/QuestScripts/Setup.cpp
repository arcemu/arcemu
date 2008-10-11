/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 * Based on Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2005-2007 arcemu Team
 * Copyright (C) 2007-2008 Moon++ Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
//	###		Classes Quests	###
	SetupDruid(mgr);
	SetupPaladin(mgr);
	SetupMage(mgr);
	SetupWarrior(mgr);
//	### 	Professions Quests	###
	SetupFirstAid(mgr);
//	###		Zones Quests	###
	SetupArathiHighlands(mgr);
	SetupAzshara(mgr);
	SetupAzuremystIsle(mgr);
	SetupBarrens(mgr);
	SetupBladeEdgeMountains(mgr);
	SetupBlastedLands(mgr);
	SetupBloodmystIsle(mgr);
	SetupDesolace(mgr);
	SetupDustwallowMarsh(mgr);
	SetupEasternPlaguelands(mgr);
	SetupGhostlands(mgr);
	SetupHellfirePeninsula(mgr);
	SetupHillsbradFoothills(mgr);
	SetupLochModan(mgr);
	SetupMulgore(mgr);
	SetupNagrand(mgr);
	SetupNetherstorm(mgr);
	SetupRedrigeMountains(mgr);
	SetupShadowmoon(mgr);
	SetupSilvermoonCity(mgr);
	SetupSilverpineForest(mgr);
	SetupStormwind(mgr);
	SetupStranglethornVale(mgr);
	SetupTanaris(mgr);
	SetupTerrokarForest(mgr);
	SetupThousandNeedles(mgr);
	SetupTirisfalGlades(mgr);
	SetupUndercity(mgr);
	SetupUngoro(mgr);
	SetupWestfall(mgr);
	SetupZangarmarsh(mgr);
//	###		Misc		###
	SetupQuestGossip(mgr);
	SetupQuestHooks(mgr);
	SetupUnsorted(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}

#endif
