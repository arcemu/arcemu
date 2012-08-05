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

#include "StdAfx.h"
#include "../Common/EasyFunctions.h"
#include "../Common/Instance_Base.h"
#include "../Common/Base.h"

#ifndef	QUEST_SCRIPTS_Setup_H
#define QUEST_SCRIPTS_Setup_H

//	###		Classes Quests	###
void SetupDruid(ScriptMgr* mgr);
void SetupMage(ScriptMgr* mgr);
void SetupPaladin(ScriptMgr* mgr);
void SetupWarrior(ScriptMgr* mgr);

//	###		Proffessions Quests	###
void SetupFirstAid(ScriptMgr* mgr);

//	###		Zones Quests	###
void SetupArathiHighlands(ScriptMgr* mgr);
void SetupAzshara(ScriptMgr* mgr);
void SetupAzuremystIsle(ScriptMgr* mgr);
void SetupBladeEdgeMountains(ScriptMgr* mgr);
void SetupBlastedLands(ScriptMgr* mgr);
void SetupBloodmystIsle(ScriptMgr* mgr);
void SetupBurningSteppes(ScriptMgr* mgr);
//void SetupDarkshore(ScriptMgr * mgr)
void SetupDeathKnight(ScriptMgr* mgr);
void SetupDesolace(ScriptMgr* mgr);
void SetupDragonblight(ScriptMgr* mgr);
void SetupDuskwood(ScriptMgr* mgr);
void SetupDustwallowMarsh(ScriptMgr* mgr);
void SetupEasternPlaguelands(ScriptMgr* mgr);
void SetupEversongWoods(ScriptMgr* mgr);
void SetupGhostlands(ScriptMgr* mgr);
void SetupHellfirePeninsula(ScriptMgr* mgr);
void SetupHillsbradFoothills(ScriptMgr* mgr);
void SetupHowlingFjord(ScriptMgr* mgr);
void SetupIsleOfQuelDanas(ScriptMgr* mgr);
void SetupLochModan(ScriptMgr* mgr);
void SetupMulgore(ScriptMgr* mgr);
void SetupNagrand(ScriptMgr* mgr);
void SetupNetherstorm(ScriptMgr* mgr);
void SetupRedrigeMountains(ScriptMgr* mgr);
void SetupShadowmoon(ScriptMgr* mgr);
void SetupSilithus(ScriptMgr* mgr);
void SetupSilvermoonCity(ScriptMgr* mgr);
void SetupSilverpineForest(ScriptMgr* mgr);
void SetupStormwind(ScriptMgr* mgr);
void SetupStranglethornVale(ScriptMgr* mgr);
void SetupTanaris(ScriptMgr* mgr);
void SetupTeldrassil(ScriptMgr* mgr);
void SetupTerrokarForest(ScriptMgr* mgr);
void SetupTheStormPeaks(ScriptMgr* mgr);
void SetupThousandNeedles(ScriptMgr* mgr);
void SetupTirisfalGlades(ScriptMgr* mgr);
void SetupUndercity(ScriptMgr* mgr);
void SetupUnGoro(ScriptMgr* mgr);
void SetupWestfall(ScriptMgr* mgr);
void SetupWetlands(ScriptMgr *mgr);
void SetupWinterspring(ScriptMgr* mgr);
void SetupZangarmarsh(ScriptMgr* mgr);
void SetupBarrens(ScriptMgr* mgr);
void SetupBoreanTundra(ScriptMgr* mgr);
void SetupSholazarBasin(ScriptMgr* mgr);

//	###		Misc	###
void SetupQuestGossip(ScriptMgr* mgr);
void SetupQuestHooks(ScriptMgr* mgr);
void SetupUnsorted(ScriptMgr* mgr);

#endif
