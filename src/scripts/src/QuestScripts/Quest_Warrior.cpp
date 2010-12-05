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


#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);


class TheSummoning : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature* windwatcher = TO_CREATURE(pObject);
		if (windwatcher == NULL)
			return;

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(1713))
			Menu->AddItem( 0, "I'm ready, Summon Him!", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* windwatcher = TO_CREATURE(pObject);
		if (windwatcher == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
			if(plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL)
				return;
				Creature* whirlwind = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 6239);
				if(whirlwind != NULL)
				{
					if(!whirlwind->isAlive())
					{
						whirlwind->Delete();
					}
					else
						return;
				}

				whirlwind = sEAS.SpawnCreature(plr, 6239, plr->GetPositionX()+7, plr->GetPositionY()+7, plr->GetPositionZ(), plr->GetOrientation(), 0);
				whirlwind->Despawn(5*60*1000, 0);
			}break;
		}
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
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.37f)
		{
			if(mAttacker->IsPlayer())
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = (TO_PLAYER(mAttacker))->GetQuestLogForEntry(1640);
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
        _unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
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

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
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

void SetupWarrior(ScriptMgr * mgr)
{
	GossipScript * gossip1 = new TheSummoning();
	mgr->register_gossip_script(6176, gossip1);
	mgr->register_creature_script(6090, &Bartleby::Create);
	mgr->register_quest_script(1640, new BeatBartleby());
}
