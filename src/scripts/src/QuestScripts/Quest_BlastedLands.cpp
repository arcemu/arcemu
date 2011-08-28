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

class HeroesofOld : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Creature* spawncheckcr = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 7750);

			if(!spawncheckcr)
			{
				Creature* general = sEAS.SpawnCreature(mTarget, 7750, -10619, -2997, 28.8f, 4, 0);
				general->Despawn(3 * 60 * 1000, 0);
			}

			GameObject* spawncheckgobj = mTarget->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 141980);

			if(!spawncheckgobj)
			{
				GameObject* generalsbox = sEAS.SpawnGameobject(mTarget, 141980, -10622, -2994, 28.6f, 4, 4, 0, 0, 0, 0);
				sEAS.GameobjectDelete(generalsbox, 3 * 60 * 1000);
			}
		}
};



class HeroesofOld1 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* general = TO_CREATURE(pObject);
			if(general == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(2702) || plr->HasFinishedQuest(2702))
				Menu->AddItem(0, "I need to speak with Corporal.", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* general = TO_CREATURE(pObject);
			if(general == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						Creature* spawncheckcr = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 7750);

						if(!spawncheckcr)
						{
							general = sEAS.SpawnCreature(plr, 7750, -10619, -2997, 28.8f, 4, 0);
							general->Despawn(3 * 60 * 1000, 0);
						}

						GameObject* spawncheckgobj = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 141980);

						if(!spawncheckgobj)
						{
							GameObject* generalsbox = sEAS.SpawnGameobject(plr, 141980, -10622, -2994, 28.6f, 4, 4, 0, 0, 0, 0);
							sEAS.GameobjectDelete(generalsbox, 3 * 60 * 1000);
						}
					}
			}
		}

};

void SetupBlastedLands(ScriptMgr* mgr)
{
	QuestScript* HeroesoO = new HeroesofOld();
	mgr->register_quest_script(2702, HeroesoO);

	GossipScript* gossip1 = new HeroesofOld1();
	mgr->register_gossip_script(7572, gossip1);
}
