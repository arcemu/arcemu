/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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
// Fel Orc Scavengers

class FelOrcScavengersQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FelOrcScavengersQAI);
    FelOrcScavengersQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if ( mKiller->IsPlayer() )
		{
			QuestLogEntry *pQuest = TO_PLAYER(mKiller)->GetQuestLogForEntry( 10482 );
			if ( pQuest != NULL && pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
			{
				pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
			}
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// A Burden Of Souls

class BurdenOfSoulsQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BurdenOfSoulsQAI);
    BurdenOfSoulsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if ( !mKiller->IsPlayer() )
			return;

		QuestLogEntry *pQuest = TO_PLAYER(mKiller)->GetQuestLogForEntry( 10864 );
		if ( pQuest != NULL && pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// Testing the Antidote

bool TestingTheAntidote(uint32 i, Spell* pSpell)
{
	if(!pSpell->GetUnitTarget() || pSpell->GetUnitTarget()->GetTypeId() != TYPEID_UNIT)
		return true;

	Creature* target = NULL;
	target = TO_CREATURE(pSpell->GetUnitTarget());
	if(!target || target->GetEntry() != 16880) // Hulking Helboar
		return true;

	Creature* spawned = NULL;
	spawned = target->GetMapMgr()->GetInterface()->SpawnCreature(16992, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), true, false, 0, 0);
	if(!spawned)
		return true;

	target->Despawn(0, 300000);

	spawned->GetAIInterface()->SetNextTarget(pSpell->u_caster);

	return true;
}

class Dreadtusk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Dreadtusk);
	Dreadtusk(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnDied(Unit* mKiller)
	{
		if ( !mKiller->IsPlayer() )
			return;

		QuestLogEntry *pQuest = TO_PLAYER(mKiller)->GetQuestLogForEntry( 10255 );
		if ( pQuest != NULL && pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// Zeth'Gor Must Burn!

class ZethGorMustBurnAlliance : public GameObjectAIScript
{
public:
	ZethGorMustBurnAlliance(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new ZethGorMustBurnAlliance(GO); }

	void OnActivate(Player* pPlayer)
	{
		QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10895 );
		if ( pQuest != NULL )
		{
			// M4ksiu - WTF IS THIS FOR? :|
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();

			GameObject* pBeacon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 184661 );
			if ( pBeacon != NULL && pBeacon->GetUInt32Value( GAMEOBJECT_FLAGS ) > 0 )
			{
				pBeacon->SetUInt32Value( GAMEOBJECT_FLAGS, ( pBeacon->GetUInt32Value( GAMEOBJECT_FLAGS ) - 1 ) );
			}

			// Northern Zeth'Gor Tower
			if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
			{
				GameObject* pNorthern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -820.0f, 2029.0f, 55.0f, 300150 );
				if ( pNorthern != NULL && pPlayer->CalcDistance( pPlayer, pNorthern ) < 40 ) // if reduced the server will crash when out of range
				{
      				pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
					pQuest->SendUpdateAddKill( 0 );
					pQuest->UpdatePlayerFields();

					GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -819.77f, 2029.09f, 55.6082f, 0, 4, 0, 0, 0, 0);
     				if ( pGameobject != NULL )
					{
						sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
					}

					return;
				}
			}

			// Southern Zeth'Gor Tower
			if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
			{
				GameObject* pSouthern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -1150.0f, 2110.0f, 84.0f, 300150 );
                if ( pSouthern != NULL && pPlayer->CalcDistance( pPlayer, pSouthern ) < 40 )
				{
					pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
					pQuest->SendUpdateAddKill( 1 );
					pQuest->UpdatePlayerFields();
	
					GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1150.53f, 2109.92f, 84.4204f, 0, 4, 0, 0, 0, 0 );
	     			if ( pGameobject != NULL )
					{
						sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
					}

					return;
				}
			}

			// Forge Zeth'Gor Tower
			if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
			{
				GameObject* pForge = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -893.0f, 1919.0f, 82.0f, 300150 );
				if ( pForge != NULL && pPlayer->CalcDistance( pPlayer, pForge ) < 40 )
				{
					pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
					pQuest->SendUpdateAddKill( 2 );
					pQuest->UpdatePlayerFields();
	
					GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -893.499f, 1919.27f, 81.6449f, 0, 4, 0, 0, 0, 0);
	     			if ( pGameobject != NULL )
					{
						sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
					}
	
					return;
				}
			}

			// Foothill Zeth'Gor Tower
			if ( pQuest->GetMobCount( 3 ) < pQuest->GetQuest()->required_mobcount[3] )
			{
				GameObject* pFoothill = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -978.0f, 1879.0f, 111.0f, 300150 );
				if ( pFoothill != NULL && pPlayer->CalcDistance( pPlayer, pFoothill ) < 40 )
				{
					pQuest->SetMobCount( 3, pQuest->GetMobCount( 3 ) + 1 );
					pQuest->SendUpdateAddKill( 3 );
					pQuest->UpdatePlayerFields();
	
					GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -977.713f, 1879.500f, 110.892f, 0, 4, 0, 0, 0, 0 );
	     			if ( pGameobject != NULL )
					{
						sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
					}
	
					return;
				}
			}
			else
			{
				pPlayer->BroadcastMessage( "You are to far away!" );
			}

		}
		else
		{
			pPlayer->BroadcastMessage( "Missing required quest : Zeth'Gor Must Burn" );
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// Zeth'Gor Must Burn!

bool ZethGorMustBurnHorde(uint32 i, Spell* pSpell)
{
	Player* pPlayer = pSpell->p_caster;
	if ( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10792 );
	if ( pQuest != NULL )
	{
		// Barracks
		if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
        {
			GameObject* pBarracks = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -1137.0f, 1970.0f, 74.0f, 300151 );
            if ( pBarracks != NULL && pPlayer->CalcDistance( pPlayer, pBarracks ) < 30 )
            {
      			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = NULL;
				pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1129.08f, 1921.77f, 94.0074f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

	     		pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1135.00f, 1944.05f, 84.7084f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

	     		pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1152.01f, 1945.00f, 102.901f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

	     		pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1159.60f, 1958.76f, 83.0412f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

	     		pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1126.17f, 1880.96f, 95.065f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

	     		pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1185.79f, 1968.29f, 90.931f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Eastern Hovel
		if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
        {
			GameObject* pEasternHovel = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -940.0f, 1920.0f, 69.0f, 300151 );
            if ( pEasternHovel != NULL && pPlayer->CalcDistance( pPlayer, pEasternHovel ) < 30 )
            {
      			pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
				pQuest->SendUpdateAddKill( 1 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -938.034f, 1924.153f, 73.590f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Western Hovel
		if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
        {
			GameObject* pWesternHovel = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -1155.0f, 2061.0f, 68.0f, 300151 );
            if ( pWesternHovel != NULL && pPlayer->CalcDistance( pPlayer, pWesternHovel ) < 30 )
            {
      			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
				pQuest->SendUpdateAddKill( 2 );
				pQuest->UpdatePlayerFields();
	
				GameObject*  pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1152.10f, 2066.20f, 72.959f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Stable
		if ( pQuest->GetMobCount( 3 ) < pQuest->GetQuest()->required_mobcount[3] )
        {
			GameObject* pStable = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -1052.0f, 2007.0f, 66.0f, 300151 );
            if ( pStable != NULL && pPlayer->CalcDistance( pPlayer, pStable ) < 30 )
            {
	      		pQuest->SetMobCount( 3, pQuest->GetMobCount( 3 ) + 1 );
				pQuest->SendUpdateAddKill( 3 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -1058.85f, 2010.95f, 68.776f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}
	}
	else
	{
		pPlayer->BroadcastMessage( "Missing required quest : Zeth'Gor Must Burn" );
	}

	return true;
}

/*--------------------------------------------------------------------------------------------------------*/
// Laying Waste to the Unwanted

bool LayingWasteToTheUnwantedAlliance(uint32 i, Spell* pSpell)
{
	Player* pPlayer = pSpell->p_caster;
	if ( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10078 );
	if ( pQuest != NULL )
	{
		// Eastern Thrower
		if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
		{
			GameObject* pEasternTower = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-155.0f, 2517.0f, 43.0f, 300152);
			if ( pEasternTower != NULL && pPlayer->CalcDistance( pPlayer, pEasternTower ) < 30 )
            {
      			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -157.916f, 2517.71f, 58.5508f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Central Eastern Thrower
		if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
		{
			GameObject* pCentralEasternTower = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -152.0f, 2661.0f, 44.0f, 300152 );
			if ( pCentralEasternTower != NULL && pPlayer->CalcDistance( pPlayer, pCentralEasternTower ) < 30 )
			{
      			pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
				pQuest->SendUpdateAddKill( 1 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -152.527f, 2661.99f, 60.8123f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Central Western Thrower
		if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
		{
			GameObject* pCentralWesternTower = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -174.0f, 2772.0f, 32.0f, 300152 );
			if ( pCentralWesternTower != NULL && pPlayer->CalcDistance( pPlayer, pCentralWesternTower ) < 30 )
			{
      			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
				pQuest->SendUpdateAddKill( 2 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -177.916f, 2773.75f, 48.636f, 0, 4, 0, 0, 0, 0 );
				if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}

		// Western Thrower
		if ( pQuest->GetMobCount( 3 ) < pQuest->GetQuest()->required_mobcount[3] )
		{
			GameObject* pWesternTower = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -166.0f, 2818.0f, 29.0f, 300152 );
			if ( pWesternTower != NULL && pPlayer->CalcDistance( pPlayer, pWesternTower ) < 30 )
			{
      			pQuest->SetMobCount( 3, pQuest->GetMobCount( 3 ) + 1 );
				pQuest->SendUpdateAddKill( 3 );
				pQuest->UpdatePlayerFields();
	
				GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -166.0f, 2818.0f, 29.0f, 0, 4, 0, 0, 0, 0 );
	     		if ( pGameobject != NULL )
				{
	     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
				}

				return true;
			}
		}
	}
	else
	{
		pPlayer->BroadcastMessage( "Missing required quest : Laying Waste to the Unwanted" );
	}

	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
// Burn It Up... For the Horde!

bool BurnItUp(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10087 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		GameObject* pEastern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -300.0f, 2407.0f, 50.0f, 183122 );
		if( pEastern == NULL )
			pEastern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -300.0f, 2407.0f, 50.0f, 185122 );

		if ( pEastern != NULL && pPlayer->CalcDistance( pPlayer, pEastern ) < 30 )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			
			GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, -300.0f, 2407.0f, 50.0f, 0, 4, 0, 0, 0, 0 );
			if ( pGameobject != NULL )
			{
     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
			}

			return true;
		}
	}

	if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
	{
		GameObject* pWestern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -350.0f, 2708.0f, 35.0f, 183122 );
		if( pWestern == NULL )
			pWestern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -300.0f, 2407.0f, 50.0f, 185122 );

		if ( pWestern != NULL && pPlayer->CalcDistance( pPlayer, pWestern ) < 30 )
		{
			pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
			pQuest->SendUpdateAddKill( 1 );
			pQuest->UpdatePlayerFields();
			
			GameObject* pGameobject = sEAS.SpawnGameobject(pPlayer, 183816, -350.0f, 2708.0f, 35.0f, 0, 4, 0, 0, 0, 0);
			if ( pGameobject != NULL )
			{
     			sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
			}

			return true;
		}
	}

	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
// The Dreghood Elders

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, pPlayer); \
    Menu->SendTo(pPlayer);

class PrisonerGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
	{
		if ( pPlayer == NULL )			// useless, but who knows
			return;

		if ( !pObject->IsCreature() )	// can't imagine to get this null lol
			return;

		int32 i = -1;
		Creature* pPrisoner = TO_CREATURE( pObject );
		switch ( pPrisoner->GetEntry() )
		{
		case 20677:
			i = 0;
			break;
		case 20678:
			i = 1;
			break;
		case 20679:
			i = 2;
			break;
		}

		if ( i == -1 )
			return;

		QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10368 );
		if ( pQuest != NULL && pQuest->GetMobCount( i ) < pQuest->GetQuest()->required_mobcount[i] )
		{
			if ( pPlayer->GetItemInterface()->GetItemCount( 29501 ) > 0 )
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer( &Menu, pObject->GetGUID(), 10104, pPlayer );
				Menu->AddItem( 0, "Walk free, Elder. Bring the spirits back to your tribe.", 1 );
				if ( AutoSend )
					Menu->SendTo( pPlayer );
			}
		}
   	}
 
	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		if ( pPlayer == NULL )
			return;

		if ( !pObject->IsCreature() )
			return;

		switch ( IntId )
		{
		case 0:
			GossipHello( pObject, pPlayer, true );
			break;
		case 1:
		{
			int32 i = -1;
			Creature* pPrisoner = TO_CREATURE( pObject );
			switch ( pPrisoner->GetEntry() )
			{
			case 20677:
				i = 0;
				break;
			case 20678:
				i = 1;
				break;
			case 20679:
				i = 2;
				break;
			}

			if ( i == -1 )
				return;

			QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10368 );
			if ( pQuest != NULL && pQuest->GetMobCount( i ) < pQuest->GetQuest()->required_mobcount[i] )
			{
				pQuest->SetMobCount( i, pQuest->GetMobCount( i ) + 1 );
				pQuest->SendUpdateAddKill( i );
				pQuest->UpdatePlayerFields();

				pPrisoner->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You've freed me! The winds speak to my people one again and grant us their strength. I thank you, stranger." );
				pPrisoner->Despawn( 5000, 6 * 60 * 1000 );
				pPrisoner->SetStandState( STANDSTATE_STAND );
			}
		}break;
		}
	}

};

// Limbo state?
class PrisonersDreghoodElders : public CreatureAIScript
{
public:
  	ADD_CREATURE_FACTORY_FUNCTION(PrisonersDreghoodElders);
  	PrisonersDreghoodElders(Creature* pCreature) : CreatureAIScript(pCreature) {}

  	void OnLoad()
  	{
    	_unit->SetStandState( STANDSTATE_SIT );
    	_unit->setDeathState( CORPSE );
    	_unit->GetAIInterface()->m_canMove = false;
  	}
};


/*--------------------------------------------------------------------------------------------------------*/
// The Seer's Relic

bool TheSeersRelic(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry * qle = pPlayer->GetQuestLogForEntry( 9545 );
	if ( qle == NULL || qle->GetMobCount( 0 ) >= qle->GetQuest()->required_mobcount[0] )
		return true;

	Creature*  pTarget = pPlayer->GetMapMgr()->GetCreature( GET_LOWGUID_PART( pPlayer->GetSelection() ) );
	if ( pTarget == NULL )
		return true;

	if ( pTarget->GetEntry() != 16852 )
		return true;

	pTarget->SetStandState( 0 );
	pTarget->setDeathState( ALIVE );
	pTarget->Despawn( 30 * 1000, 1 * 60 * 1000 );
  
	qle->SetMobCount( 0, 1 );
	qle->SendUpdateAddKill( 0 );
	qle->UpdatePlayerFields();

	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
// Disrupt Their Reinforcements

bool DisruptTheirReinforcements(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuestA = pPlayer->GetQuestLogForEntry( 10144 );
	QuestLogEntry *pQuestH = pPlayer->GetQuestLogForEntry( 10208 );

	if ( pQuestA != NULL )
	{
		bool SendMsg = false;
		if ( pQuestA->GetMobCount( 0 ) < pQuestA->GetQuest()->required_mobcount[0] )
		{
			GameObject* pGrimh = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -419.0f, 1847.0f, 80.0f, 184414 );
			if ( pGrimh != NULL && pPlayer->CalcDistance( pPlayer, pGrimh ) < 10 )
			{
				pQuestA->SetMobCount( 0, pQuestA->GetMobCount( 0 ) + 1 );
				pQuestA->SendUpdateAddKill( 0 );
				pQuestA->UpdatePlayerFields();
				return true;
			}
			else
			{
				SendMsg = true;
			}
		}
		if ( pQuestA->GetMobCount( 1 ) < pQuestA->GetQuest()->required_mobcount[1] )
		{
			GameObject* pKaalez = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -548.0f, 1782.0f, 58.0f, 184415 );
			if ( pKaalez != NULL && pPlayer->CalcDistance( pPlayer, pKaalez ) < 10 )
			{
				pQuestA->SetMobCount( 1, pQuestA->GetMobCount( 1 ) + 1 );
				pQuestA->SendUpdateAddKill( 1 );
				pQuestA->UpdatePlayerFields();
				return true;
			}
			else
			{
				SendMsg = true;
			}
		}

		if ( SendMsg )
		{
			pPlayer->BroadcastMessage( "Go to the Port of the Dark Legion!" );
		}
	}
	else if ( pQuestH != NULL )
	{
		bool SendMsg = false;
		if ( pQuestH->GetMobCount( 0 ) < pQuestH->GetQuest()->required_mobcount[0] )
		{
			GameObject* pXilus = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( -85.0f, 1880.0f, 74.0f, 184290 );
			if ( pXilus != NULL && pPlayer->CalcDistance( pPlayer, pXilus ) < 10 )
			{
				pQuestH->SetMobCount( 0, pQuestH->GetMobCount( 0 ) + 1 );
				pQuestH->SendUpdateAddKill( 0 );
				pQuestH->UpdatePlayerFields();
				return true;
			}
			else
			{
				SendMsg = true;
			}
		}
		if ( pQuestH->GetMobCount( 1 ) < pQuestH->GetQuest()->required_mobcount[1] )
		{
			GameObject* pKruul = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 147.0f, 1717.0f, 38.0f, 184289 );
			if ( pKruul != NULL && pPlayer->CalcDistance( pPlayer, pKruul ) < 10 )
			{
				pQuestH->SetMobCount( 1, pQuestH->GetMobCount( 1 ) + 1 );
				pQuestH->SendUpdateAddKill( 1 );
				pQuestH->UpdatePlayerFields();
				return true;
			}
			else
			{
				SendMsg = true;
			}
		}

		if ( SendMsg )
		{
			pPlayer->BroadcastMessage( "Go to the Port of the Dark Legion!" );
		}
	}
	else
	{
		pPlayer->BroadcastMessage( "Missing required quest : Disrupt Their Reinforcements" );
	}

	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
//Arzeth's Demise

bool FuryOfTheDreghoodElders(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	Unit* pUnit = pSpell->GetUnitTarget();
	if ( pUnit == NULL || !pUnit->IsCreature() || pUnit->GetEntry() != 19354 )
		return true;

	//pPlayer->BroadcastMessage("blaah");	// Really blizzlike?
	
	sEAS.SpawnCreature( pPlayer, 20680, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ(), pUnit->GetOrientation(), 5 * 60 * 1000 );
	TO_CREATURE(pUnit)->Despawn( 0, 3 * 60 * 1000 );
	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
// A Spirit Guide

bool ASpiritGuide(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

  	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9410 );
  	if ( pQuest == NULL )
    	return true;

  	Creature* pSpiritWolf = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(17077, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), true, false, NULL, NULL);
	if ( pSpiritWolf == NULL )
		return true;
	
	pSpiritWolf->Despawn( 10 * 1000, 0 );

  	pPlayer->CastSpell( pPlayer, 29938, false );
  	return true;
}


/*--------------------------------------------------------------------------------------------------------*/

class HellfireDeadNPC : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HellfireDeadNPC);
	HellfireDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetStandState( 7 );
		_unit->setDeathState( CORPSE );
		_unit->GetAIInterface()->m_canMove = false;
	}
};

bool AnAmbitiousPlan(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9383 );
	if ( pQuest == NULL )
		return true;

	Creature* pTarget = pPlayer->GetMapMgr()->GetCreature( GET_LOWGUID_PART( pPlayer->GetSelection() ) );
	if ( pTarget == NULL || pTarget->GetEntry() != 16975 )
		return true;

	float SSx = pTarget->GetPositionX();
	float SSy = pTarget->GetPositionY();
	float SSz = pTarget->GetPositionZ();
	pTarget->GetAIInterface()->SetAllowedToEnterCombat( false );
	pTarget->GetAIInterface()->StopMovement( 0 );
	pTarget->GetAIInterface()->setCurrentAgent( AGENT_NULL );
	pTarget->GetAIInterface()->SetAIState( STATE_IDLE );
	pTarget->Despawn( 0, 0 );

	GameObject* pGameobject = sEAS.SpawnGameobject( pPlayer, 183816, SSx, SSy, SSz, 0, 1, 0, 0, 0, 0 );
	if ( pGameobject != NULL )
	{
		sEAS.GameobjectDelete( pGameobject, 1 * 60 * 1000 );
		pPlayer->UpdateNearbyGameObjects();
	}

	return true;
}

class DarkTidingsAlliance : public QuestScript 
{ 
public:
	void OnQuestComplete(Player* pPlayer, QuestLogEntry *qLogEntry)
	{
		if ( pPlayer == NULL )
			return;

		Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17479 );
		if ( pCreature == NULL )
			return;

		char msg[100];
		sprintf( msg, "Psst, %s, get over here.", pPlayer->GetName() );
		pCreature->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, msg );	// Changed Player to Creature. I wonder if it was blizzlike
	}
};

class DarkTidingsHorde : public QuestScript 
{ 
public:
	void OnQuestComplete(Player* pPlayer, QuestLogEntry *qLogEntry)
	{
		if ( pPlayer == NULL )
			return;

		Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17558 );
		if ( pCreature == NULL )
			return;

		char msg[100];
		sprintf( msg, "Psst, %s, get over here.", pPlayer->GetName() );
		pCreature->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, msg );
	}
};


void SetupHellfirePeninsula(ScriptMgr * mgr)
{
	/*-------------------------------------------------------------------*/
	// Finished
	mgr->register_creature_script(16772, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(19701, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16876, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16925, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(18952, &FelOrcScavengersQAI::Create);

	mgr->register_creature_script(19411, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19410, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16867, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16870, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19413, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19414, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16878, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19415, &BurdenOfSoulsQAI::Create);
	mgr->register_dummy_spell(34665, &TestingTheAntidote);
	mgr->register_creature_script(16992, &Dreadtusk::Create);
	
	mgr->register_gameobject_script(184661, &ZethGorMustBurnAlliance::Create);
	mgr->register_dummy_spell(35724, &ZethGorMustBurnHorde);
	mgr->register_dummy_spell(32979, &LayingWasteToTheUnwantedAlliance);
	mgr->register_dummy_spell(33067, &BurnItUp);
	mgr->register_dummy_spell(30489, &TheSeersRelic);
	mgr->register_dummy_spell(34387, &DisruptTheirReinforcements);
	mgr->register_dummy_spell(29364, &AnAmbitiousPlan);
	
	GossipScript *pPrisonerGossip = new PrisonerGossip();
	mgr->register_gossip_script(20677, pPrisonerGossip);
	mgr->register_gossip_script(20678, pPrisonerGossip);
	mgr->register_gossip_script(20679, pPrisonerGossip);
	
	/*-------------------------------------------------------------------*/
	// TODO
	//mgr->register_dummy_spell(35460, &FuryOfTheDreghoodElders);
	mgr->register_dummy_spell(29731, &ASpiritGuide);
	
	
	/*-------------------------------------------------------------------*/
	// NPC States
	mgr->register_creature_script(20677, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20678, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20679, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(17405, &HellfireDeadNPC::Create);
	mgr->register_creature_script(16852, &HellfireDeadNPC::Create);
	mgr->register_creature_script(20158, &HellfireDeadNPC::Create);

	QuestScript *DarkTidingsHordeQuest = new DarkTidingsHorde();
	QuestScript *DarkTidingsAllianceQuest = new DarkTidingsAlliance();
	mgr->register_quest_script(9587, DarkTidingsAllianceQuest);
	mgr->register_quest_script(9588, DarkTidingsHordeQuest);
}
