/*
 * WEmu Scripts for WEmu MMORPG Server
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


#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

class DashelStonefist : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DashelStonefist);
	DashelStonefist(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 12);
		_unit->SetStandState(STANDSTATE_STAND);
	}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		uint32 chance = RandomUInt(100);
		if(chance < 15)
		{
			string say = "Now you're gonna get it good, ";
			say+=((Player*)mAttacker)->GetName();
			say+="!";
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
		}
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.2)
		{
			if(mAttacker->GetTypeId() == TYPEID_PLAYER)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = ((Player*)mAttacker)->GetQuestLogForEntry(1640);
				if(!qle)
					return;
				qle->SendQuestComplete();
			}
		}
	}

	void AIUpdate()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay, okay! Enough fighting. No one else needs to get hurt.");
		_unit->RemoveNegativeAuras();
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 12);
		_unit->SetHealthPct(100);      
		_unit->GetAIInterface()->WipeTargetList();         
		_unit->GetAIInterface()->WipeHateList();
        _unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
        _unit->GetAIInterface()->disable_melee = true;
        _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
	}
};

class SCRIPT_DECL TheMissingDiplomat : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* Dashel = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 4961);

		if(Dashel == NULL)
			return;

		Dashel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 72);
		Dashel->GetAIInterface()->disable_melee = false;
		Dashel->GetAIInterface()->SetAllowedToEnterCombat(true);
	}
};

void SetupStormwind(ScriptMgr * mgr)
{
	mgr->register_creature_script(4961, &DashelStonefist::Create);
	mgr->register_quest_script(1447, CREATE_QUESTSCRIPT(TheMissingDiplomat));
}