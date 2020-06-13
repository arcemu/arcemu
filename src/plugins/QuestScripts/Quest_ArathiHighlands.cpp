/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
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

class SunkenTreasure : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2768);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Defens Me!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
			sEAS.WaypointCreate(creat, -2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
			sEAS.EnableWaypoints(creat);
		}
};

class Professor_Phizzlethorpe : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Professor_Phizzlethorpe);
		Professor_Phizzlethorpe(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 15)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, I found the fact that, it searched");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(665)->SendQuestComplete();
			}
		}
};

void SetupArathiHighlands(ScriptMgr* mgr)
{
	mgr->register_creature_script(2768, &Professor_Phizzlethorpe::Create);
	/*mgr->register_quest_script(665, new SunkenTreasure());*/
}
