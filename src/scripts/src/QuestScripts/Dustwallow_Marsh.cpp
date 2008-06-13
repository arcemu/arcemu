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

#define BALOS_FRIENDLY_TIMER 120
class BalosJackenQAI : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(BalosJackenQAI);
	BalosJackenQAI(Creature* pCreature) : CreatureAIScript(pCreature)  
   {
	  friendlyTimer=BALOS_FRIENDLY_TIMER;
   }

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{ 
	   // If Balos Jacken HP - fAmount < 20%
	   if (_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.2)
	   {
		  //Missing: modify fAmount to prevent Balos Jacken death.
		  //{...}
		  //force player to loose target and stop melee auto-attack:
		  _unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		  //start AIUpdate
		  RegisterAIUpdateEvent(1000);
	   }
	}

	void AIUpdate()
	{   
	  if (friendlyTimer==BALOS_FRIENDLY_TIMER)
	  {
		 // set Balos Jacken friendly and start friendlyTimer cooldown
		 _unit->RemoveNegativeAuras();
		 _unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);   
		 _unit->SetHealthPct(100);		 
		 _unit->GetAIInterface()->WipeTargetList();		 
		 _unit->GetAIInterface()->WipeHateList();
		 _unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
		 _unit->GetAIInterface()->disable_melee = true;
		 _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		 //remove not_selectable flag:
		 _unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		 // decrease timer
		 friendlyTimer--;
	  }
	  else if (friendlyTimer==0)
		 {
			// set Balos Jacken unfriendly and reset FriendlyTimer
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
			_unit->GetAIInterface()->disable_melee = false;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			friendlyTimer=BALOS_FRIENDLY_TIMER;
			RemoveAIUpdateEvent();
		 }
	  else
	  {
		 //friendlyTimer decrease
		 friendlyTimer--;	  
	  }
   }

	void OnDied(Unit *Mkiller)
	{
	  RemoveAIUpdateEvent();
	}
protected:
   short friendlyTimer;
};

class SCRIPT_DECL Cleansing_of_the_Orb_of_Orahil : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 6546);

		if(creat == NULL)
			return;
		
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Fine, if you ready to go!");
		
		TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Cleansing_of_the_Orb_of_Orahil, Creature*>(this, &Cleansing_of_the_Orb_of_Orahil::SumDemon, creat), 0, 12000, 1);
		creat->event_AddEvent(te);
		
		creat->GetAIInterface()->setMoveType(11);
		creat->GetAIInterface()->StopMovement(3000);
		
		sEAS.CreateCustomWaypointMap(creat);
		sEAS.WaypointCreate(creat,-4036.163818f, -3383.940918f, 38.977619f, 2.181819f, 0, 56, 5375);
		sEAS.WaypointCreate(creat,-4037.754639f, -3376.272461f, 37.400284f, 0.002337f, 0, 56, 5375);
		sEAS.WaypointCreate(creat,-4028.081787f, -3376.667969f, 38.152534f, 5.775016f, 0, 56, 5375);
		sEAS.WaypointCreate(creat,-4018.655029f, -3382.780518f, 38.244286f, 5.688624f, 0, 56, 5375);
		sEAS.WaypointCreate(creat,-4018.655029f, -3382.780518f, 38.244286f, 5.688624f, 11000, 256, 5375);
		sEAS.WaypointCreate(creat,-4028.081787f, -3376.667969f, 38.152534f, 5.775016f, 0, 256, 5375);
		sEAS.WaypointCreate(creat,-4037.754639f, -3376.272461f, 37.400284f, 0.002337f, 0, 256, 5375);
		sEAS.WaypointCreate(creat,-4036.163818f, -3383.940918f, 38.977619f, 2.181819f, 0, 256, 5375);
		sEAS.WaypointCreate(creat,-4036.1638f, -3383.9409f, 38.9776f, -1.3662f, 0, 256, 5375);
		sEAS.WaypointCreate(creat,-4032.5759f, -3393.6271f, 38.9960f, 1.8883f, 0, 256, 5375);
	}

	void SumDemon(Creature *creat)
	{
		creat->GetMapMgr()->GetInterface()->SpawnCreature(6549, creat->GetPositionX(), creat->GetPositionY(), creat->GetPositionZ(), creat->GetOrientation(), true, false, 0, 0)->Despawn(120000, 0);
	}
};

class OverlordMokMorokk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(OverlordMokMorokk);
	OverlordMokMorokk(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 23);
		_unit->SetStandState(STANDSTATE_STAND);
	}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		uint32 chance = RandomUInt(100);
		if(chance < 25)
		{
			_unit->CastSpell(mAttacker, dbcSpell.LookupEntry(6749), true);
		}
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.3)
		{
			if(mAttacker->GetTypeId() == TYPEID_PLAYER)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = ((Player*)mAttacker)->GetQuestLogForEntry(1173);
				if(!qle)
					return;
				qle->SendQuestComplete();
			}
		}
	}

	void AIUpdate()
	{
		_unit->RemoveNegativeAuras();
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 29);
		_unit->SetHealthPct(100);      
		_unit->GetAIInterface()->WipeTargetList();         
		_unit->GetAIInterface()->WipeHateList();
        _unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
        _unit->GetAIInterface()->disable_melee = true;
        _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
	}
};

class SCRIPT_DECL ChallengeOverlordMokMorokk : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* Overlord = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 4500);

		if(Overlord == NULL)
			return;

		string say = "Puny ";
		say+=mTarget->GetName();
		say+=" wanna fight Overlord Mok'Morokk? Me beat you! Me boss here!";
		Overlord->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
		Overlord->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 72);
		Overlord->GetAIInterface()->disable_melee = false;
		Overlord->GetAIInterface()->SetAllowedToEnterCombat(true);
	}
};

class PrivateHendel : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PrivateHendel);
	PrivateHendel(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 12);
		_unit->SetStandState(STANDSTATE_STAND);
	}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.37)
		{
			if(mAttacker->GetTypeId() == TYPEID_PLAYER)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = ((Player*)mAttacker)->GetQuestLogForEntry(1324);
				if(!qle)
					return;
				qle->SendQuestComplete();
			}
		}
	}
					
	void AIUpdate()
	{
		_unit->Emote(EMOTE_STATE_KNEEL);
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

class SCRIPT_DECL TheMissingDiplomat2 : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* Dashel = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 4966);

		if(Dashel == NULL)
			return;

		Dashel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 72);
		Dashel->GetAIInterface()->disable_melee = false;
		Dashel->GetAIInterface()->SetAllowedToEnterCombat(true);
	}
};

void SetupDustwallowMarsh(ScriptMgr * mgr)
{
	mgr->register_creature_script(5089, &BalosJackenQAI::Create);
	mgr->register_quest_script(4961, CREATE_QUESTSCRIPT(Cleansing_of_the_Orb_of_Orahil));
	mgr->register_quest_script(1955, CREATE_QUESTSCRIPT(Cleansing_of_the_Orb_of_Orahil));
   	mgr->register_creature_script(4500, &OverlordMokMorokk::Create);
	mgr->register_quest_script(1173, CREATE_QUESTSCRIPT(ChallengeOverlordMokMorokk));
	mgr->register_creature_script(4966, &PrivateHendel::Create);
	mgr->register_quest_script(1324, CREATE_QUESTSCRIPT(TheMissingDiplomat2));
}
