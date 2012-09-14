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

class TheSummoning : public QuestScript
{
public:
	void OnQuestStart(Player* pPlayer, QuestLogEntry* qLogEntry)
	{
		Creature *windwatcher = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 6176);
		if(!windwatcher) return;

		// questgiver will walk to the place where Cyclonian is spawned
		// only walk when we are at home
		if(windwatcher->CalcDistance(250.839996f, -1470.579956f, 55.4491f) > 1) return;
		{
			windwatcher->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me");
			sEAS.CreateCustomWaypointMap(windwatcher);
			sEAS.WaypointCreate(windwatcher, 269.29f, -1433.32f, 50.31f, 0.19f, 0, 0, 0);
			sEAS.WaypointCreate(windwatcher, 328.52f, -1442.03f, 40.5f, 5.65f, 0, 0, 0);
			sEAS.WaypointCreate(windwatcher, 333.31f, -1453.69f, 42.01f, 4.68f, 0, 0, 0);
			sEAS.EnableWaypoints(windwatcher);
			windwatcher->GetAIInterface()->setMoveType(11);
		}
		windwatcher->Despawn(15*60*1000, 0);

		// spawn cyclonian if not spawned already
		Creature *cyclonian = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(323.947f, -1483.68f, 43.1363f, 6239);
		if( cyclonian == NULL)
		{
			cyclonian = sEAS.SpawnCreature(pPlayer, 6239, 323.947f, -1483.68f, 43.1363f, 0.682991f);

			// if spawning cyclonian failed, we have to return.
			if(cyclonian == NULL)
				return;
		}

		// queue cyclonian for despawn
		cyclonian->Despawn(15*60*1000, 0);
	}
};

class Bartleby : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Bartleby);
		Bartleby(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetFaction(11);
			_unit->setEmoteState(7);
		}

		void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
		{
			if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH) - fAmount <= _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.37f)
			{
				if(mAttacker->IsPlayer())
				{
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					RegisterAIUpdateEvent(1000);
					QuestLogEntry* qle = (TO_PLAYER(mAttacker))->GetQuestLogForEntry(1640);
					if(!qle)
						return;
					qle->SendQuestComplete();
				}
			}
		}

		void AIUpdate()
		{
			_unit->RemoveNegativeAuras();
			_unit->SetFaction(11);
			_unit->SetHealthPct(100);
			_unit->GetAIInterface()->WipeTargetList();
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, _unit, 0);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

};

class BeatBartleby : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* Bartleby = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 6090);

			if(Bartleby == NULL)
				return;

			Bartleby->SetFaction(168);
			Bartleby->GetAIInterface()->disable_melee = false;
			Bartleby->GetAIInterface()->SetAllowedToEnterCombat(true);
		}
};

void SetupWarrior(ScriptMgr* mgr)
{
	mgr->register_quest_script(1713, new TheSummoning());
	mgr->register_creature_script(6090, &Bartleby::Create);
	mgr->register_quest_script(1640, new BeatBartleby());
}
