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

class Veronia : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(10652))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			Menu->AddItem( 0, "I'm ready", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = TO_CREATURE(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(plr, dbcSpell.LookupEntry(34905), true);
			break;
		}
	}

}; 

bool FindingTheKeymaster(uint32 i, Spell* pSpell)
{
	if(!pSpell->u_caster->IsPlayer())
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10256);
	if(!qle)
		return true;

	qle->SetMobCount(0, qle->GetMobCount(0)+1);
	qle->SendUpdateAddKill(0);
	qle->UpdatePlayerFields();
	return true;
}

bool TheFleshLies(uint32 i, Spell* pSpell)
{
	if(!pSpell->u_caster->IsPlayer())
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);
	Creature* target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 20561);

	if(target == NULL)
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10345);
  
	if(qle == NULL)
		return true;

	GameObject* obj = NULL;

	if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		
		obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
		sEAS.GameobjectDelete(obj, 1*30*1000);
	}
	target->Despawn(2000, 60*1000);
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

bool SurveyingtheRuins(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10335 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		GameObject* mark1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4695.2f, 2603.39f, 209.878f, 184612 );
		if( mark1 == NULL )  	
			mark1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4695.28f, 2602.73f, 209.84f, 300095 );

		if ( mark1 != NULL && pPlayer->CalcDistance( pPlayer, mark1 ) < 15 )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			
			//if you know a GO which can/should be spawned after marking the point... uncomment and add GO ID pls :)
			//GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, <INSERT GO ID HERE>, 4695.28f, 2602.73f, 209.84f, 0, 4, 0, 0, 0, 0 );
			//if ( pGameobject != NULL )
			//{
     	//		sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
			//}

			return true;
		}
	}
	
	if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
	{
		GameObject* mark2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4608.08f, 2442.02f, 195.71f, 184612 );
		if( mark2 == NULL )  	
			mark2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4607.71f, 2440.72f, 195.65f, 300095 );

		if ( mark2 != NULL && pPlayer->CalcDistance( pPlayer, mark2 ) < 15 )
		{
			pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
			pQuest->SendUpdateAddKill( 1 );
			pQuest->UpdatePlayerFields();
			
			//if you know a GO which can/should be spawned after marking the point... uncomment and add GO ID pls :)
			//GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, <INSERT GO ID HERE>, 4607.71f, 2440.72f, 195.65f, 0, 4, 0, 0, 0, 0 );
			//if ( pGameobject != NULL )
			//{
     	//		sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
			//}

			return true;
		}
	}
	
	if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
	{
		GameObject* mark3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4716.37f, 2371.59f, 198.168f, 184612 );
		if( mark3 == NULL )  	
			mark3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 4716.77f, 2371.6f, 198.19f, 300095 );

		if ( mark3 != NULL && pPlayer->CalcDistance( pPlayer, mark3 ) < 15 )
		{
			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
			pQuest->SendUpdateAddKill( 2 );
			pQuest->UpdatePlayerFields();
			
			//if you know a GO which can/should be spawned after marking the point... uncomment and add GO ID pls :)
			//GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, <INSERT GO ID HERE>,  4716.77f, 2371.6f, 198.19f, 0, 4 );
			//if ( pGameobject != NULL )
			//{
     	//		sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
			//}

			return true;
		}
	}

	return true;
}

void SetupNetherstorm(ScriptMgr* mgr)
{
	mgr->register_gossip_script(20162, new Veronia());
	mgr->register_dummy_spell(34717, &FindingTheKeymaster);
	mgr->register_dummy_spell(35372, &TheFleshLies);
	mgr->register_dummy_spell(35246, &SurveyingtheRuins);
}
