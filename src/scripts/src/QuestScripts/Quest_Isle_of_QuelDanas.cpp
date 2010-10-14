/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2010 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

/*--------------------------------------------------------------------------------------------------------*/
// Crisis at the Sunwell

class ScryingOrb : public GameObjectAIScript
{
public:
	ScryingOrb(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new ScryingOrb(GO); }

	void OnActivate(Player* pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(11490);
  		if(qle)
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			
			GameObject* pOrb = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 187578);
			if (pOrb)
			{
				pOrb->SetState(0);
  				qle->SetMobCount(0, 1);
  				qle->SendUpdateAddKill(0);
  				qle->UpdatePlayerFields();
			}
			return;
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : The Scryer's Scryer");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// Erratic Behavior

bool ConvertingSentry(uint32 i, Spell* pSpell)
{
  Player* pCaster = pSpell->p_caster;
  if( pCaster == NULL )
    return true;

  Creature* pTarget = TO_CREATURE(pSpell->GetUnitTarget());
  if( pTarget == NULL || pTarget->GetEntry() != 24972 || pTarget->isAlive() ) // Erratic Sentry: 24972
    return true;

  QuestLogEntry *qle = NULL;
  qle = pCaster->GetQuestLogForEntry(11525);
  if(qle == NULL)
  {
	qle = pCaster->GetQuestLogForEntry(11524);
	if(qle == NULL)
       return true;
  }

  if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
    return true;

  qle->SetMobCount(0, qle->GetMobCount(0)+1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  pTarget->Despawn(500, 2*60*1000);

  return true;
}

bool OrbOfMurlocControl(uint32 i, Spell* pSpell)
{
	if(pSpell->m_caster->IsPlayer() == false)
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11541);
	if(pQuest == NULL)
		return true;

	Creature* pTarget;
	
	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if( (*itr)->IsUnit() && TO_UNIT(*itr)->IsCreature() )
			pTarget = TO_CREATURE(*itr);
		else
			continue;

		if( pSpell->m_caster->CalcDistance(pTarget) > 5 )
			continue;

		if( pTarget->GetEntry() == 25084)
		{
		  if(pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
		  {
			pQuest->SetMobCount(0, pQuest->GetMobCount(0) + 1);
			pQuest->SendUpdateAddKill(0);		
			Creature* FreedGreengill = sEAS.SpawnCreature(pPlayer, 25085, pTarget->GetPositionX(),
			pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(), 0);
			FreedGreengill->Despawn(6*60*1000, 0);
			pTarget->Despawn(0, 6*60*1000);
			pQuest->UpdatePlayerFields();
			return true;
		  }  
		}
  }
  return true;
}

#define GO_FIRE 183816

static Location BloodoathFire[] =
{
	{13329.4f, -6994.70f, 14.5219f, 1.38938f},
 	{13315.4f, -6990.72f, 14.7647f, 1.25979f}
};
static Location SinlorenFire[] =
{
	{13214.3f, -7059.19f, 17.5717f, 1.58573f},
	{13204.2f, -7059.38f, 17.5717f, 1.57787f}
};
static Location DawnchaserFire[] =
{
	{13284.1f, -7152.65f, 15.9774f, 1.44828f},
	{13273.0f, -7151.21f, 15.9774f, 1.39723f}
};

bool ShipBombing(uint32 i, Spell* pSpell)
{
	Player* pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = NULL;
	qle = pPlayer->GetQuestLogForEntry(11542);
	if(qle == NULL)
	{
		qle = pPlayer->GetQuestLogForEntry(11543);
		if(qle == NULL)
		{
			return true;
		}
	}
	
	GameObject* pSinloren = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13200.232422, -7049.176270, 3.838517, 550000);
	GameObject* pBloodoath = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13319.419922, -6988.779785, 4.002993, 550000);
	GameObject* pDawnchaser = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13274.51625, -7145.434570, 4.770292, 550000);
	
	GameObject* obj = NULL;

	if(pSinloren != NULL)
	{
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			if(pPlayer->CalcDistance(pPlayer, pSinloren) < 15)
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
				for(uint8 j = 0; j < 2; j++)
				{
					obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, SinlorenFire[j].x, SinlorenFire[j].y, SinlorenFire[j].z, SinlorenFire[j].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(pBloodoath != NULL)
	{
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			if(pPlayer->CalcDistance(pPlayer, pBloodoath) < 15)
			{
				qle->SetMobCount(1, qle->GetMobCount(1)+1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
				for(uint8 j = 0; j < 2; j++)
				{
					obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, BloodoathFire[j].x, BloodoathFire[j].y, BloodoathFire[j].z, BloodoathFire[j].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(pDawnchaser != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, pDawnchaser) < 15)
		{
			if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
			{
				qle->SetMobCount(2, qle->GetMobCount(2)+1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
				for(uint8 j = 0; j < 2; j++)
				{
					obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, DawnchaserFire[j].x, DawnchaserFire[j].y, DawnchaserFire[j].z, DawnchaserFire[j].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	return true;
}

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, pPlayer); \
    Menu->SendTo(pPlayer);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Ayren Cloudbreaker Gossip

class SCRIPT_DECL AyrenCloudbreaker_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12252, pPlayer);
		
		if (pPlayer->GetQuestLogForEntry(11532) || pPlayer->GetQuestLogForEntry(11533))
			Menu->AddItem( 0, "Speaking of action, I've been ordered to undertake an air strike.", 1);
			
		if (pPlayer->GetQuestLogForEntry(11543) || pPlayer->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "I need to intercept the Dawnblade reinforcements.", 2);
			
		Menu->SendTo(pPlayer);
	}

	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * pPath = sTaxiMgr.GetTaxiPath( 779 );
				pPlayer->TaxiStart( pPath, 22840, 0 );
				pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		case 2:
			{
				TaxiPath * pPath = sTaxiMgr.GetTaxiPath( 784 );
				pPlayer->TaxiStart( pPath, 22840, 0 );
				pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Unrestrained Dragonhawk Gossip

class SCRIPT_DECL UnrestrainedDragonhawk_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12371, pPlayer);
		if (pPlayer->GetQuestLogForEntry(11543) || pPlayer->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "<Ride the dragonhawk to Sun's Reach>", 1);
			
		Menu->SendTo(pPlayer);
	}

	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * pPath = sTaxiMgr.GetTaxiPath( 788 );
				pPlayer->TaxiStart( pPath, 22840, 0 );
				pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}

};

// The Battle for the Sun's Reach Armory
class TheBattleForTheSunReachArmory : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TheBattleForTheSunReachArmory);
    TheBattleForTheSunReachArmory(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* pKiller)
	{
		if (pKiller->IsPlayer())
		{
			QuestLogEntry *qle = ( TO_PLAYER( pKiller ) )->GetQuestLogForEntry(11537);
			if( qle == NULL )
			{
				qle = ( TO_PLAYER( pKiller ) )->GetQuestLogForEntry(11538);
				if( qle == NULL )
					return;
			}

			if( qle->GetMobCount( 1 ) < qle->GetQuest()->required_mobcount[ 1 ] )
			{
				uint32 newcount = qle->GetMobCount( 1 ) + 1;
				qle->SetMobCount( 1, newcount );
				qle->SendUpdateAddKill( 1 );
				qle->UpdatePlayerFields();
				return;
			}
		}
	}
};

bool ImpaleEmissary(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
	return true;

	Player* pPlayer = TO_PLAYER(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11537);
	if( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry(11538);
		if( pQuest == NULL )
			return true;
	}
	
	Creature* pEmissary = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 25003);
	if( pEmissary == NULL )
		return true;
		
	if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 0 ] )
	{
		uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
		pQuest->SetMobCount( 0, newcount );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		pEmissary->Despawn(0, 3*60*1000);
	}
	return true;
}

bool LeyLine( uint32 i, Spell* pSpell )
{
	if( !pSpell->u_caster->IsPlayer() )
		return true;

	Player * pPlayer = static_cast<Player *>( pSpell->u_caster );
	QuestLogEntry * qle = pPlayer->GetQuestLogForEntry( 11547 );

	if ( qle == NULL )
		return true;

	uint32 portals[] = { 188527, 188526, 188525 };
	Object * portal = NULL;

	for ( uint32 i = 0; i < sizeof( portals ) / sizeof( uint32 ); i++ )
	{
		portal = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), portals[i] );
		if ( portal != NULL && qle->GetMobCount( i ) < qle->GetQuest()->required_mobcount[i] )
		{
			qle->SetMobCount( i,  qle->GetMobCount( i ) + 1 );
			qle->SendUpdateAddKill( i );
			qle->UpdatePlayerFields();

			break;
		}
	}

	return true;
}

bool ManaRemnants( uint32 i, Spell* pSpell )
{
	if(!pSpell->u_caster->IsPlayer())
		return true;

	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	QuestLogEntry * qle;

	Creature * Ward = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 40404);

	uint32 quests[] = { 11496, 11523 };
	for ( uint32 i = 0; i < 2; i++ )
	 {
		qle = pPlayer->GetQuestLogForEntry( quests[i] );
		if ( qle != NULL && qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[0] )
		{			
			pPlayer->CastSpell(Ward, dbcSpell.LookupEntry(44981), false);
			pPlayer->SetChannelSpellTargetGUID(Ward->GetGUID());
			pPlayer->SetChannelSpellId(44981);
			qle->SetMobCount( 0,  qle->GetMobCount( 0 ) + 1 );
			qle->SendUpdateAddKill( 0 );
			qle->UpdatePlayerFields();
		}
	 }
	return true;
}

void SetupIsleOfQuelDanas(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(187578, &ScryingOrb::Create);

	mgr->register_dummy_spell(45109, &OrbOfMurlocControl);
	mgr->register_dummy_spell(44997, &ConvertingSentry);
	mgr->register_dummy_spell(45115, &ShipBombing);
	
	mgr->register_creature_script(24999, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25001, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25002, &TheBattleForTheSunReachArmory::Create);

	mgr->register_dummy_spell(45030, &ImpaleEmissary);
	mgr->register_dummy_spell( 45191, &LeyLine );
	mgr->register_dummy_spell( 44981, &ManaRemnants );

	//GOSSIP
	GossipScript * AyrenCloudbreakerGossip = new AyrenCloudbreaker_Gossip;
	mgr->register_gossip_script(25059, AyrenCloudbreakerGossip);
	GossipScript * UnrestrainedDragonhawkGossip = new UnrestrainedDragonhawk_Gossip;
	mgr->register_gossip_script(25236, UnrestrainedDragonhawkGossip);
}
