/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Setup.h"

#define SKIP_ALLOCATOR_SHARING 1

#include <ScriptSetup.h>

#include "AlteracValley.h"
#include "ArathiBasin.h"
#include "CircleOfBlood.h"
#include "DalaranSewers.h"
#include "EyeOfTheStorm.h"
#include "IsleOfConquest.h"
#include "RingOfTrials.h"
#include "RingOfValor.h"
#include "RuinsOfLordaeron.h"
#include "StrandOfTheAncient.h"
#include "WarsongGulch.h"

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}


extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	CBattlegroundManager &bgMgr = CBattlegroundManager::getSingleton();

	// Mapping Battleground type to map Id
	bgMgr.RegisterMapForBgType( BATTLEGROUND_ALTERAC_VALLEY,        30 );
	bgMgr.RegisterMapForBgType( BATTLEGROUND_WARSONG_GULCH,         489 );
	bgMgr.RegisterMapForBgType( BATTLEGROUND_ARATHI_BASIN,          529 );
	bgMgr.RegisterMapForBgType( BATTLEGROUND_EYE_OF_THE_STORM,      566 );
	bgMgr.RegisterMapForBgType( BATTLEGROUND_STRAND_OF_THE_ANCIENT, 607 );
	bgMgr.RegisterMapForBgType( BATTLEGROUND_ISLE_OF_CONQUEST,      628 );

	// Registering factory methods
	bgMgr.RegisterBgFactory( 30,  &AlteracValley::Create );
	bgMgr.RegisterBgFactory( 489, &WarsongGulch::Create );
	bgMgr.RegisterBgFactory( 529, &ArathiBasin::Create );
	bgMgr.RegisterBgFactory( 566, &EyeOfTheStorm::Create );
	bgMgr.RegisterBgFactory( 607, &StrandOfTheAncient::Create );
	bgMgr.RegisterBgFactory( 628, &IsleOfConquest::Create );

	bgMgr.RegisterArenaFactory( 559, &RingOfTrials::Create );
	bgMgr.RegisterArenaFactory( 562, &CircleOfBlood::Create );
	bgMgr.RegisterArenaFactory( 572, &RuinsOfLordaeron::Create );
	bgMgr.RegisterArenaFactory( 617, &DalaranSewers::Create );
	bgMgr.RegisterArenaFactory( 618, &RingOfValor::Create );
}

#ifdef WIN32

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

#endif

