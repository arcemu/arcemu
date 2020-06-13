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

class ProtectingtheShipment : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();


			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 5644);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ts...... I must be removed from this lair");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -336.599213f, 1764.957275f, 139.517746f, 4.461932f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -338.992798f, 1750.499023f, 139.414673f, 5.318010f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -327.872040f, 1734.572388f, 139.331665f, 3.382010f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -350.114044f, 1728.984741f, 139.337723f, 2.514144f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -372.413940f, 1745.675293f, 139.382660f, 3.872884f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -386.453796f, 1736.851685f, 137.771500f, 3.790415f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -400.639496f, 1728.250488f, 131.174881f, 3.511599f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -431.880005f, 1713.812378f, 127.584572f, 3.704021f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -453.655090f, 1700.086670f, 120.951744f, 4.261652f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -465.082306f, 1677.391113f, 111.065910f, 4.866406f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -462.129944f, 1665.744507f, 108.106010f, 4.257726f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -477.037445f, 1630.453857f, 99.003723f, 4.787863f, 1, 256, 3791);
			sEAS.WaypointCreate(creat, -474.509247f, 1590.814453f, 94.546768f, 5.197841f, 1, 256, 3791);
			sEAS.EnableWaypoints(creat);
		}
};

class Dalinda_Malem  : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Dalinda_Malem);
		Dalinda_Malem(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 13)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, these orcs wanted me to weld in the boiler");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(1440)->SendQuestComplete();
			}
		}
};

void SetupDesolace(ScriptMgr* mgr)
{
	mgr->register_creature_script(5644, &Dalinda_Malem::Create);
	/*mgr->register_quest_script(1440, new ProtectingtheShipment());*/
}
