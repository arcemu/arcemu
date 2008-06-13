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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
	Menu->SendTo(plr);


class SCRIPT_DECL StrFever : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature *doctor = (Creature*)(pObject);
		if (doctor == NULL)
			return;

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(348) && plr->GetItemInterface()->GetItemCount(2799, 0) && !plr->GetItemInterface()->GetItemCount(2797, 0))
			Menu->AddItem( 0, "I'm ready, Summon Him!", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature *doctor = (Creature*)(pObject);
		if (doctor == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				plr->GetItemInterface()->RemoveItemAmt(2799, 1);
				doctor->CastSpell(doctor, dbcSpell.LookupEntry(12380), true);
				if(!plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL)
					return;
				Creature *firstenemy = sEAS.SpawnCreature(plr, 1511, -13770.5, -6.79, 42.8, 5.7 , 0);
				firstenemy->GetAIInterface()->MoveTo(-13727.8, -26.2, 46.15, 4.07);
				firstenemy->Despawn(10*60*1000, 0);
			}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class Beka : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Beka);
	Beka(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if(mKiller->IsPlayer())
		{
			Player* mPlayer = static_cast< Player* >( mKiller );
			if(mPlayer == NULL || mPlayer->GetMapMgr() == NULL || mPlayer->GetMapMgr()->GetInterface() == NULL)
				return;
			Creature * beka1 = sEAS.SpawnCreature(mPlayer, 1516, -13770.5, -6.79, 42.8, 5.7 , 0);
			beka1->GetAIInterface()->MoveTo(-13727.8, -26.2, 46.15, 4.07);
			beka1->Despawn(10*60*1000, 0);
		}
		else
		{
			Player* mPlayer = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
			if(mPlayer)
			{
				if(mPlayer == NULL || mPlayer->GetMapMgr() == NULL || mPlayer->GetMapMgr()->GetInterface() == NULL)
					return;
				Creature * beka1 = sEAS.SpawnCreature(mPlayer, 1516, -13770.5, -6.79, 42.8, 5.7 , 0);
				beka1->GetAIInterface()->MoveTo(-13727.8, -26.2, 46.15, 4.07);
				beka1->Despawn(10*60*1000, 0);
			}
		}
	}
};

class Beka1 : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Beka1);
	Beka1(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if(mKiller->IsPlayer())
		{
			Player* mPlayer = static_cast< Player* >( mKiller );
			if(mPlayer == NULL || mPlayer->GetMapMgr() == NULL || mPlayer->GetMapMgr()->GetInterface() == NULL)
				return;
			Creature * beka1 = sEAS.SpawnCreature(mPlayer, 1514, -13770.5, -6.79, 42.8, 5.7, 0);
			beka1->GetAIInterface()->MoveTo(-13727.8, -26.2, 46.15, 4.07);
			beka1->Despawn(10*60*1000, 0);
		}
		else
		{
			Player* mPlayer = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
			if(mPlayer)
			{
				if(mPlayer == NULL || mPlayer->GetMapMgr() == NULL || mPlayer->GetMapMgr()->GetInterface() == NULL)
					return;
				Creature * beka1 = sEAS.SpawnCreature(mPlayer, 1514, -13770.5, -6.79, 42.8, 5.7, 0);
				beka1->GetAIInterface()->MoveTo(-13727.8, -26.2, 46.15, 4.07);
				beka1->Despawn(10*60*1000, 0);
			}
		}
	}
};

class Beka2 : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Beka2);
	Beka2(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if( mKiller == NULL || mKiller->GetMapMgr() == NULL || mKiller->GetMapMgr()->GetInterface() == NULL )
			return;

		float SSX = mKiller->GetPositionX();
		float SSY = mKiller->GetPositionY();
		float SSZ = mKiller->GetPositionZ();

		Creature* doctor = mKiller->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 1449);
		if(doctor)
			doctor->Emote(EMOTE_ONESHOT_CHEER);
	}
};

class SCRIPT_DECL BloodscalpClanHeads : public QuestScript 
{ 
public:

	void OnQuestComplete( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		GameObject* skull1 = mTarget->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(SSX, SSY, SSZ, 2551);
		if(skull1)
			return;

		Creature* Kin_weelay = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2519);
		if(Kin_weelay == NULL)
			return;

		string msg1 = "Ah. Good ";
		msg1 += mTarget->GetName();
		msg1 += ". Now let us see what tale these heads tell...";
		Kin_weelay->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg1.c_str());
		Kin_weelay->CastSpell(Kin_weelay, dbcSpell.LookupEntry(3644), false);
		skull1->Despawn(5000);
		GameObject* skull2 = mTarget->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(SSX, SSY, SSZ, 2551);
		if(skull2)
			skull2->Despawn(5000);

		TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<BloodscalpClanHeads, Player*>(this, &BloodscalpClanHeads::SayAfterCast, mTarget), 0, 5000, 1);
		mTarget->event_AddEvent(te);
	}

	void SayAfterCast(Player* plr)
	{
		if( plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL )
			return;

		float SSX = plr->GetPositionX();
		float SSY = plr->GetPositionY();
		float SSZ = plr->GetPositionZ();

		Creature* Kin_weelay = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2519);
		if(Kin_weelay == NULL)
			return;
		string msg = "There, ";
		msg += plr->GetName();
		msg += ". You may now speak to the Bloodscalp chief and his witchdoctor.";
		Kin_weelay->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str());
	}
};

class SCRIPT_DECL BacktoBootyBay : public QuestScript 
{ 
public:

	void OnQuestComplete( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float X = mTarget->GetPositionX();
		float Y = mTarget->GetPositionY();
		float Z = mTarget->GetPositionZ();

		Creature* Crank = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(X, Y, Z, 2498);
		if(Crank)
		{
			string say = "Hm... if you're looking to adle wits. ";
			say += mTarget->GetName();
			say += ", then the secret behind Zanzil's zombies might just fo the trick!";
			Crank->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
		}
	}
};

class SCRIPT_DECL VoodooDues : public QuestScript 
{ 
public:

	void OnQuestComplete( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float X = mTarget->GetPositionX();
		float Y = mTarget->GetPositionY();
		float Z = mTarget->GetPositionZ();

		Creature* MacKinley = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(X, Y, Z, 2501);
		if(MacKinley)
		{
			string say = "Bah! ";
			say += mTarget->GetName();
			say += ", this foot won't budge!";
			MacKinley->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
		}
	}
};

void SetupStranglethornVale(ScriptMgr * mgr)
{
	GossipScript * gossip1 = (GossipScript*) new StrFever();
	mgr->register_gossip_script(1449, gossip1);

	mgr->register_creature_script(1511, &Beka::Create);

	mgr->register_creature_script(1516, &Beka1::Create);

	mgr->register_quest_script(584, CREATE_QUESTSCRIPT(BloodscalpClanHeads));

	mgr->register_quest_script(1118, CREATE_QUESTSCRIPT(BacktoBootyBay));
	mgr->register_quest_script(609, CREATE_QUESTSCRIPT(VoodooDues));
}
