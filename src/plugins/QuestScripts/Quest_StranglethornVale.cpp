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


class StrFever : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* doctor = TO_CREATURE(pObject);
			if(doctor == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(348) && plr->GetItemInterface()->GetItemCount(2799, 0) && !plr->GetItemInterface()->GetItemCount(2797, 0))
				Menu->AddItem(0, "I'm ready, Summon Him!", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* doctor = TO_CREATURE(pObject);
			if(doctor == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						plr->GetItemInterface()->RemoveItemAmt(2799, 1);
						doctor->CastSpell(doctor, dbcSpell.LookupEntry(12380), true);
						if(!plr || !plr->GetMapMgr() || !plr->GetMapMgr()->GetInterface())
							return;
						Creature* firstenemy = sEAS.SpawnCreature(plr, 1511, -13770.5f, -6.79f, 42.8f, 5.7f , 0);
						firstenemy->GetAIInterface()->MoveTo(-13727.8f, -26.2f, 46.15f, 4.07f);
						firstenemy->Despawn(10 * 60 * 1000, 0);
					}
					break;
			}
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
				Player* mPlayer = TO_PLAYER(mKiller);
				Creature*  beka1 = sEAS.SpawnCreature(mPlayer, 1516, -13770.5f, -6.79f, 42.8f, 5.7f , 0);
				beka1->GetAIInterface()->MoveTo(-13727.8f, -26.2f, 46.15f, 4.07f);
				beka1->Despawn(10 * 60 * 1000, 0);
			}
			else
			{
				Player* mPlayer = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
				if(mPlayer)
				{
					Creature*  beka1 = sEAS.SpawnCreature(mPlayer, 1516, -13770.5f, -6.79f, 42.8f, 5.7f , 0);
					beka1->GetAIInterface()->MoveTo(-13727.8f, -26.2f, 46.15f, 4.07f);
					beka1->Despawn(10 * 60 * 1000, 0);
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
				Player* mPlayer = TO_PLAYER(mKiller);
				Creature* beka1 = sEAS.SpawnCreature(mPlayer, 1514, -13770.5f, -6.79f, 42.8f, 5.7f, 0);
				beka1->GetAIInterface()->MoveTo(-13727.8f, -26.2f, 46.15f, 4.07f);
				beka1->Despawn(10 * 60 * 1000, 0);
			}
			else
			{
				Player* mPlayer = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
				if(mPlayer)
				{
					Creature* beka1 = sEAS.SpawnCreature(mPlayer, 1514, -13770.5f, -6.79f, 42.8f, 5.7f, 0);
					beka1->GetAIInterface()->MoveTo(-13727.8f, -26.2f, 46.15f, 4.07f);
					beka1->Despawn(10 * 60 * 1000, 0);
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
			float SSX = mKiller->GetPositionX();
			float SSY = mKiller->GetPositionY();
			float SSZ = mKiller->GetPositionZ();

			Creature* doctor = mKiller->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 1449);
			if(doctor)
				doctor->Emote(EMOTE_ONESHOT_CHEER);
		}
};

class BloodscalpClanHeads : public QuestScript
{
	public:

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			GameObject* skull1 = mTarget->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(SSX, SSY, SSZ, 2551);
			if(skull1 == NULL)
				return;

			Creature* Kin_weelay = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2519);
			if(Kin_weelay == NULL)
				return;

			string msg1 = "Ah. Good ";
			msg1 += mTarget->GetName();
			msg1 += ". Now let us see what tale these heads tell...";
			Kin_weelay->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg1.c_str());
			Kin_weelay->CastSpell(Kin_weelay, dbcSpell.LookupEntry(3644), false);
			skull1->Despawn(5000, 0);
			GameObject* skull2 = mTarget->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(SSX, SSY, SSZ, 2551);
			if(skull2)
				skull2->Despawn(5000, 0);

			if(Kin_weelay == NULL)
				return;
			string msg = "There, ";
			msg += mTarget->GetName();
			msg += ". You may now speak to the Bloodscalp chief and his witchdoctor.";
			Kin_weelay->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str(), 500);
		}

};

class BacktoBootyBay : public QuestScript
{
	public:

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
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

class VoodooDues : public QuestScript
{
	public:

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
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

#define GO_MEAT 181291
#define GO_BOTTLE 2687
#define GO_BREAD 2562


static Location MeatSpawnPoints[] =
{
	{ -14655.1f, 148.229f, 3.01744f, 3.45635f},
	{ -14655.6f, 146.111f, 2.29463f, 1.43766f},
	{ -14655.1f, 147.721f, 2.64111f, 3.99435f},
	{ -14654.3f, 147.015f, 2.21427f, 2.44821f},
	{ -14655.8f, 147.092f, 2.36460f, 0.66155f},
	{ -14654.3f, 147.866f, 2.90964f, 0.05732f},
	{ -14653.5f, 147.004f, 2.36253f, 2.80556f},
	{ -14652.2f, 146.926f, 3.63756f, 6.06693f},
	{ -14653.0f, 145.274f, 2.76439f, 6.06279f}
};
static Location BottleSpawnPoints[] =
{
	{ -14653.5f, 145.711f, 2.01005f, 1.14750f},
	{ -14656.7f, 147.404f, 3.05695f, 1.44181f},
	{ -14657.1f, 147.068f, 2.94368f, 1.40036f},
	{ -14657.5f, 147.567f, 2.83560f, 2.14234f},
	{ -14655.9f, 148.848f, 3.93732f, 2.79728f}
};
static Location BreadSpawnPoints[] =
{
	{ -14654.6f, 146.299f, 2.04134f, 5.47387f},
	{ -14656.5f, 148.372f, 3.50805f, 5.76817f},
	{ -14652.6f, 146.079f, 3.35855f, 2.89231f}
};

class FacingNegolash : public QuestScript
{
		void OnQuestComplete(Player* pPlayer, QuestLogEntry* qLogEntry)
		{
			GameObject* obj = NULL;

			for(uint8 i = 0; i < 9; i++)
			{
				obj = sEAS.SpawnGameobject(pPlayer, GO_MEAT, MeatSpawnPoints[i].x, MeatSpawnPoints[i].y, MeatSpawnPoints[i].z, MeatSpawnPoints[i].o, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2 * 60 * 1000);
			}
			for(uint8 i = 0; i < 5; i++)
			{
				obj = sEAS.SpawnGameobject(pPlayer, GO_BOTTLE, BottleSpawnPoints[i].x, BottleSpawnPoints[i].y, BottleSpawnPoints[i].z, BottleSpawnPoints[i].o, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2 * 60 * 1000);
			}
			for(uint8 i = 0; i < 3; i++)
			{
				obj = sEAS.SpawnGameobject(pPlayer, GO_BREAD, BreadSpawnPoints[i].x, BreadSpawnPoints[i].y, BreadSpawnPoints[i].z, BreadSpawnPoints[i].o, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2 * 60 * 1000);
			}

			Creature* Negolash = sEAS.SpawnCreature(pPlayer, 1494, -14657.400391f, 155.115997f, 4.081050f, 0.353429f);
			Negolash->GetAIInterface()->MoveTo(-14647.526367f, 143.710052f, 1.164550f, 1.909f);
		}
};

class NegolashAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NegolashAI);

		NegolashAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
		}
		void OnDied(Unit* mKiller)
		{
			_unit->Despawn(180000, 0);
			RemoveAIUpdateEvent();
		}
};


void SetupStranglethornVale(ScriptMgr* mgr)
{
	GossipScript* gossip1 = new StrFever();
	mgr->register_gossip_script(1449, gossip1);

	mgr->register_creature_script(1511, &Beka::Create);

	mgr->register_creature_script(1516, &Beka1::Create);

	mgr->register_quest_script(584, new BloodscalpClanHeads());

	mgr->register_quest_script(1118, new BacktoBootyBay());
	mgr->register_quest_script(609, new VoodooDues());
}
