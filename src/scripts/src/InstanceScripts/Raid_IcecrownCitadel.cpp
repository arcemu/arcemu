/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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
#include "Raid_IcecrownCitadel.h"

/////////////////////////////////////////////////////////////////////////////////
////// Icecrown Citadel Instance

#define MAP_ICC 631

class IccScript : public MoonInstanceScript
{
	public:
		MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(IccScript, MoonInstanceScript);
		IccScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr) {};

		void OnCreatureDeath(Creature* pVictim, Unit* pKiller)
		{
		switch(pVictim->GetEntry())
			{
			
			case CN_LORD_MARROWGAR:
					AddGameObjectStateByEntry(201563, State_Active);
					AddGameObjectStateByEntry(202245, State_Active); //teleporter needs to be clickcable
					AddGameObjectStateByEntry(201910, State_Active);
					AddGameObjectStateByEntry(201911, State_Active);
					AddGameObjectStateByEntry(201857, State_Active);
				break;
			case CN_LADY_DEATHWISPER:
					AddGameObjectStateByEntry(201563, State_Active);
					AddGameObjectStateByEntry(202243, State_Active); // teleporter on the spire
					AddGameObjectStateByEntry(202244, State_Active); // teleporter on the deathbringer's rise
				break;
			case CN_DEATHBRINGER:
					AddGameObjectStateByEntry(202244, State_Active); // teleporter on the deathbringer's rise
					AddGameObjectStateByEntry(201825, State_Active);
					AddGameObjectStateByEntry(201858, State_Active);
					AddGameObjectStateByEntry(201880, State_Active);
				break;	
			};
		};
};

void SetupICC(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_instance_script(MAP_ICC, &IccScript::Create); 
};

