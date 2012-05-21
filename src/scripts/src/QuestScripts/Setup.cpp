/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
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
	SetupMage(mgr);
	SetupPaladin(mgr);
	SetupWarrior(mgr);

//	###		Proffessions Quests	###
	SetupFirstAid(mgr);

//	###		Zones Quests	###
	SetupArathiHighlands(mgr);
	SetupAzshara(mgr);
	SetupAzuremystIsle(mgr);
	SetupBladeEdgeMountains(mgr);
	SetupBlastedLands(mgr);
	SetupBloodmystIsle(mgr);
	SetupBurningSteppes(mgr);
	//SetupDarkshore(mgr);
	SetupDeathKnight(mgr);
	SetupDesolace(mgr);
	SetupDragonblight(mgr);
	SetupDuskwood(mgr);
	SetupDustwallowMarsh(mgr);
	SetupEasternPlaguelands(mgr);
	SetupEversongWoods(mgr);
	SetupGhostlands(mgr);
	SetupHellfirePeninsula(mgr);
	SetupHillsbradFoothills(mgr);
	SetupHowlingFjord(mgr);
	SetupIsleOfQuelDanas(mgr);
	SetupLochModan(mgr);
	SetupMulgore(mgr);
	SetupNagrand(mgr);
	SetupNetherstorm(mgr);
	SetupRedrigeMountains(mgr);
	SetupShadowmoon(mgr);
	SetupSilithus(mgr);
	SetupSilvermoonCity(mgr);
	SetupSilverpineForest(mgr);
	SetupStormwind(mgr);
	SetupStranglethornVale(mgr);
	SetupTanaris(mgr);
	SetupTeldrassil(mgr);
	SetupTerrokarForest(mgr);
	SetupTheStormPeaks(mgr);
	SetupThousandNeedles(mgr);
	SetupTirisfalGlades(mgr);
	SetupUndercity(mgr);
	SetupUnGoro(mgr);
	SetupWestfall(mgr);
	SetupWetlands(mgr);
	SetupWinterspring(mgr);
	SetupZangarmarsh(mgr);
	SetupBarrens(mgr);
	SetupBoreanTundra(mgr);
	SetupSholazarBasin(mgr);
//	###		Misc		###
	SetupQuestGossip(mgr);
	SetupQuestHooks(mgr);
	SetupUnsorted(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

#endif
