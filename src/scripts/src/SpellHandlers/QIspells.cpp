/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2008 WEmu Team
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
#include "../Common/EasyFunctions.h"

bool NixxsPledgeOfSecrecy(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
    sEAS.AddItem( 11270, pPlayer );

	return true;
}

bool CleansingVial(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9427 );
	if ( pQuest == NULL )
		return true;

	Creature * pAggonar = sEAS.SpawnCreature( pPlayer, 17000, 428.15f, 3461.73f, 63.40f, 0, 0 );
	if ( pAggonar != NULL )
	{
		pAggonar->Despawn( 6 * 60 * 1000, 0 );
	}

	return true;
}

bool SummonCyclonian(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL )
		return true;
    
	Unit * pUnit = pSpell->u_caster;
	Creature * pCreature = pUnit->GetMapMgr()->GetInterface()->SpawnCreature( 6239, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ(), pUnit->GetOrientation(), true, false, 0, 0 );
	if ( pCreature != NULL )
	{
		pCreature->Despawn( 600000, 0 );
	}

	return true;
}

bool ElementalPowerExtractor(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
	Unit * pUnit = pSpell->GetUnitTarget();
	if ( pUnit == NULL || !pUnit->IsCreature() )
		return true;

    Creature * pTarget = TO_CREATURE( pUnit );
	if ( ( pTarget->GetEntry() == 18881 || pTarget->GetEntry() == 18865 ) && pTarget->isAlive() )
	{
		sEAS.AddItem( 28548, pPlayer );
	}

	return true;
}

bool SummonEkkorash(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->p_caster == NULL )
		return true;
	
	Player * plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	sEAS.SpawnCreature(plr, 19493, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	return true;
}

bool OpeningTyraliusPrison(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10442 );
	if ( pQuest != NULL )
	{
		sEAS.SpawnCreature( pPlayer, 20787, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 2 * 60 * 1000 );
		pQuest->SendQuestComplete();	// not sure if it's enough
	}

	return true;
}

bool CallRexxar(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10742 );
	if ( pQuest != NULL )
	{
		sEAS.SpawnCreature( pPlayer, 21984, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 2 * 60 * 1000);
		sEAS.SpawnCreature( pPlayer, 20555, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 2 * 60 * 1000);
	}

	return true;
}

bool LayWreath(uint32 i, Spell * pSpell) //Peace at Last quest
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 11152 );
	if ( pQuest == NULL || pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		return true;

	GameObject * pWreath = sEAS.SpawnGameobject( pPlayer, 501541, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1, 0, 0, 0, 0 );
	if ( pWreath != NULL )
	{
		sEAS.GameobjectDelete( pWreath, 2 * 60 * 1000 );
	}
	
	pQuest->SetMobCount( 0, 1 );
	pQuest->SendUpdateAddKill( 0 );
	pQuest->UpdatePlayerFields();
	return true;
}

bool ScrapReaver(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->p_caster == NULL )
		return true;

	Player * pPlayer = TO_PLAYER( pSpell->p_caster );
	Creature * pCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature( 19851, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), true, false, 0, 0 );
	if ( pCreature != NULL )
	{
		pCreature->Despawn( 600000, 0 );
	}

	return true;
}

bool IncendiaPowder(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(5088);
	if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 175944) && en)
	{
		
		if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			return false;
		}
	}

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(10882, -5005.387695f, -2103.348145f, 83.862106f, 6.265121f, true, false, 0, 0);

	return true;
}

bool GemOfTheSerpent(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12369, 254.924606f, 2967.358154f, 1.382471f, 0.918914f, true, false, 0, 0);

	return true;
}

bool RuuanokClaw(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster)
		return true;

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature( 21767, 3210.960693f, 5348.308594f, 144.537476f, 5.450696f, true, false, 0, 0 );
	return true;
}

bool KarangsBanner(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	Player * pPlayer = pSpell->p_caster;

	// Banner Aura
	pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(20746), true);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2231.710205f, -1543.603027f, 90.694946f, 4.700579f, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2232.534912f, -1556.983276f, 89.744415f, 1.527570f, true, false, 0, 0);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12757, 2239.357178f, -1546.649536f, 89.671097f, 3.530336f, true, false, 0, 0);

	return true;
}

bool BlackwhelpNet(uint32 i, Spell * pSpell)
{
    Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

    if(!pSpell->u_caster->IsPlayer())
        return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10747);
    if(qle == NULL)
        return true;

    Creature * whelp = TO_CREATURE(pSpell->GetUnitTarget());
	if(!whelp)
		return true;

    whelp->Despawn(1000, 6*60*1000);

	sEAS.AddItem(31130, pPlayer);
	return true;
}

bool ADireSituation(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10506);
  if(qle == NULL)
    return true;

	if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}

  return true;
}

bool AllAlongtheWatchtowers(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(5097);
	if(qle == NULL)
	{
		qle = pPlayer->GetQuestLogForEntry(5098);
		if(qle == NULL)
			return true;
	}

    GameObject * tower1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1304, -1318, 64, 300030);
	GameObject * tower2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1311, -1589, 61, 300030);
	GameObject * tower3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1481, -1414, 67, 300030);
	GameObject * tower4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1557, -1470, 68, 300030);

	if(tower1 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, tower1) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(tower2 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, tower2) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(tower3 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, tower3) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(tower4 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, tower4) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool FuryoftheDreghoodElders(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;



  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10369);
  if(qle == NULL)
    return true;

  Creature * arzethpower = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 19354);

  if(arzethpower == NULL)
	  return true;

  Creature * arzethless = sEAS.SpawnCreature(pPlayer, 20680, arzethpower->GetPositionX(), arzethpower->GetPositionY(), arzethpower->GetPositionZ(), arzethpower->GetOrientation(), 0);
  arzethpower->Despawn(1, 6*60*1000);
  arzethless->Despawn(5*60*1000, 0);
  return true;

}

bool ASpiritAlly(uint32 i, Spell * pSpell) 
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9847);
  if(qle == NULL)
    return true;

  Creature * allyspirit = sEAS.SpawnCreature(pPlayer, 18185, -353, 7255, 49.36, 6.28, 0);
  allyspirit->Despawn(6*60*1000, 0);
  return true;
}

bool BalanceMustBePreserved(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9720);
	if(qle == NULL)
		return true;

    GameObject * lake1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-756, 5926, 19, 300076);
	GameObject * lake2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-213, 6302, 21, 300076);
	GameObject * lake3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(498, 8197, 21, 300076);
	GameObject * lake4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(566, 6358, 23, 300076);

	if(lake1 != NULL)
	{
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(lake2 != NULL)
	{
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	if(lake3 != NULL)
	{
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(lake4 != NULL)
	{
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool BlessingofIncineratus(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9805);
	if(qle == NULL)
		return true;

	GameObject * big = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1972, 6276, 56, 300077);
	GameObject * east = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1841, 6387, 52, 400050);
	GameObject * west = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1920, 6361, 56, 400051);
	GameObject * south = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1808, 6293, 59, 400052);

	if(big != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, big) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(east != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, east) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(south != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, south) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, west) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool CatchandRelease(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9629);
  if(qle == NULL)
	return true;

  Creature * murloc = TO_CREATURE(pSpell->GetUnitTarget());
  if(!murloc)
	  return true;

  Creature * tagged = sEAS.SpawnCreature(pPlayer, 17654, murloc->GetPositionX(), murloc->GetPositionY(), murloc->GetPositionZ(), 0);
  murloc->Despawn(1, 6*60*1000);
  tagged->Despawn(5*60*1000, 0);

  if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
	  qle->SetMobCount(0, qle->GetMobCount(0)+1);
	  qle->SendUpdateAddKill(0);
	  qle->UpdatePlayerFields();
  }
  return true;
}

bool CookingPot(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
    if(!pPlayer)
	  return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(11379);
	if(qle == NULL)
		return true;
				
	pPlayer->GetItemInterface()->RemoveItemAmt(31673, 1);
	pPlayer->GetItemInterface()->RemoveItemAmt(31672, 2);
	sEAS.AddItem(33848, pPlayer);
	return true;
}

bool EvilDrawsNear(uint32 i, Spell * pSpell) 
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10923);
	if(qle == NULL)
		return true;

	Creature * dragon = sEAS.SpawnCreature(pPlayer, 22441, pPlayer->GetPositionX()+15, pPlayer->GetPositionY()+15, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	dragon->Despawn(6*60*1000, 0);
	return true;
}

bool UnyieldingBattleHorn(uint32 i, Spell * pSpell)
{
  if(pSpell->p_caster == NULL) 
    return true;

  Creature * creat = pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(19862, -1190.856079f, 2261.246582f, 46.625797f, 1.705882f, true, false, 0, 0);
  creat->Despawn(300000, 0); // 5 mins delay

  return true;
}

bool MeasuringWarpEnergies(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10313);
	if(qle == NULL)
		return true;

	GameObject * north = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3216, 4045, 145, 300094);
	GameObject * east = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2766, 3865, 145, 300094);
	GameObject * west = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2958, 4318, 145, 300094);
	GameObject * south = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2817, 4337, 145, 300094);

	if(north != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, north) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(east != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, east) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(south != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, south) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, west) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool YennikuRelease(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
		return true;

  	if(!pSpell->u_caster->IsPlayer())
    	return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(592);
  	if(qle == NULL)
    	return true;
  
  	Creature * yenniku = TO_CREATURE(pSpell->GetUnitTarget());
  	if(!yenniku)
		return true;

  	yenniku->SetFaction(29);
  	yenniku->GetAIInterface()->WipeTargetList();
  	yenniku->Despawn(30*1000, 60*1000);

  	return true;
}

bool ScrollOfMyzrael(uint32 i, Spell * pSpell) 
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(656);
  if(qle == NULL)
    return true;

  const float MyzraelPos[] = {-940.7374, -3111.1953, 48.9566, 3.327};

  Creature * myzrael = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], 2755);
  if(myzrael != NULL)
  {
    if(!myzrael->isAlive())
      myzrael->Delete();
    else
      return true;
  }

  myzrael = sEAS.SpawnCreature(pPlayer, 2755, MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], MyzraelPos[3], 0);
  return true;
}

bool Showdown(uint32 i, Spell * pSpell)
{
  Player * p_caster = pSpell->p_caster;
  if(!p_caster)
	  return true;

  QuestLogEntry *qle = p_caster->GetQuestLogForEntry(10742);
  if(qle == NULL)
  {
	  qle = p_caster->GetQuestLogForEntry(10806);
	  if(qle == NULL)
	    return true;
  }

  Creature * goc = NULL;
  goc = sEAS.SpawnCreature(p_caster, 20555, 3739, 5365, -4, 3.5, 0);
  goc->Despawn(6*60*1000, 0);
 
  return true;
}

bool TheBaitforLarkorwi1(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == NULL)
    		return true;
  
	GameObject * obj = NULL;

	obj = sEAS.SpawnGameobject(pPlayer, 169216, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1, 0, 0, 0, 0);
     	sEAS.GameobjectDelete(obj, 1*60*1000);
     	
  	return true;
}

bool TheBaitforLarkorwi2(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == NULL)
    		return true;
  
	Creature * larkowi = sEAS.SpawnCreature(pPlayer, 9684, pPlayer->GetPositionX()+2, pPlayer->GetPositionY()+3, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  	larkowi->Despawn(5*60*1000, 0);

  	return true;
}

bool Fumping(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;
		
	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10929);
  
	if(qle == NULL)
		return true;

	uint8 chance = RandomUInt(1);
	uint32 entry = 0;

	switch(chance)
	{
	case 0:
		entry = 22482;
		break;
      
    case 1:
		entry = 22483;
		break;
	}

	Creature * creat = sEAS.SpawnCreature(pPlayer, entry, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 5*60*1000);
  
	if(entry == 22483) //Sand Gnomes ;)
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "YIEEEEEEEAA!");

	return true;
}

bool TheBigBoneWorm(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10930);
	if(qle == NULL)
		return true;

	Creature * exarch = sEAS.SpawnCreature(pPlayer, 22038, pPlayer->GetPositionX()+7, pPlayer->GetPositionY()+7, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	exarch->Despawn(6*60*1000, 0);

	return true;
}

bool Torgos(uint32 i, Spell * pSpell) 
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10035);
  if(qle == NULL)
    return true;

  Creature * torgos = sEAS.SpawnCreature(pPlayer, 18707, pPlayer->GetPositionX(), pPlayer->GetPositionY()-10, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  if(!torgos)
	  return true;

  torgos->Despawn(6*60*1000, 0);
  return true;
}

bool WelcomingtheWolfSpirit(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10791);
  if(qle == NULL)
    return true;
  
  Creature * spiritwolf = sEAS.SpawnCreature(pPlayer, 19616, pPlayer->GetPositionX()+2, pPlayer->GetPositionY()+3, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  spiritwolf->Despawn(5*60*1000, 0);

  if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
	qle->SetMobCount(0, qle->GetMobCount(0)+1);
	qle->SendUpdateAddKill(0);
	qle->UpdatePlayerFields();
  }
  return true;
}

bool NaturalRemedies(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10351);
  if(qle == NULL)
	return true;

  Creature * colos = sEAS.SpawnCreature(pPlayer, 19305, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0);
  colos->Despawn(5*60*1000, 0);

  return true;
}

bool FloraoftheEcoDomes(uint32 i, Spell * pSpell)
{
	if ( !pSpell->p_caster || !pSpell->GetUnitTarget() || !pSpell->GetUnitTarget()->IsCreature() )
		return true;

	Player * pPlayer = pSpell->p_caster;

	Creature * normal = TO_CREATURE( pSpell->GetUnitTarget() );
	Creature * mutant = sEAS.SpawnCreature( pPlayer, 20983, normal->GetPositionX(), normal->GetPositionY(), normal->GetPositionZ(), 0 );

	normal->Despawn( 1, 6*60*1000 );
	mutant->Despawn( 5*60*1000, 0 );

	mutant->GetAIInterface()->Init( mutant, AITYPE_AGRO, MOVEMENTTYPE_NONE );
	mutant->GetAIInterface()->taunt( pPlayer, true );

	QuestLogEntry * qle = pPlayer->GetQuestLogForEntry( 10426 );
	if ( qle == NULL )
		return true;

	if( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount( 0, qle->GetMobCount( 0 ) + 1 );
		qle->SendUpdateAddKill( 0 );
		qle->UpdatePlayerFields();
	}
	return true;

}

bool TheCleansingMustBeStopped(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;
	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9370);
	if(qle == NULL)
		return true;

	Creature * draenei1 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);

	Creature * draenei2 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()-RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);

	Creature * draenei3 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()-RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);
	return true;
}

bool AdministreringtheSalve(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9447);
  if(qle == NULL)
	return true;

  Creature * sick = TO_CREATURE(pSpell->GetUnitTarget());
  if(!sick)
	  return true;

  Creature * healed = sEAS.SpawnCreature(pPlayer, 16846, sick->GetPositionX(), sick->GetPositionY(), sick->GetPositionZ(), 0);
  sick->Despawn(1, 6*60*1000);
  healed->Despawn(3*60*1000, 0);

  if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
	  qle->SetMobCount(0, qle->GetMobCount(0)+1);
	  qle->SendUpdateAddKill(0);
	  qle->UpdatePlayerFields();
  }
  return true;
}

bool ZappedGiants(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(7003);
  if(qle == NULL)
  {
	  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(7725);
	  if(qle == NULL)
		  return true;
  }

  Creature * creat = TO_CREATURE(pSpell->GetUnitTarget());
  if (creat == NULL)
  	return true;

  float X = creat->GetPositionX();
  float Y = creat->GetPositionY();
  float Z = creat->GetPositionZ();

  uint32 cit = creat->GetEntry();

  switch(cit)
  {
  case 5360:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14639, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5361:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14638, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5359:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14603, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5358:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14640, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5357:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14604, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  }
  return true;
}

bool BuildingAPerimeter(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10313 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		GameObject * pEast = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2257.0f, 2465.0f, 101.0f, 183947 );
		if ( pEast != NULL && pPlayer->CalcDistance( pPlayer, pEast ) < 30 )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			return true;
		}
	}
	if ( pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
	{
		GameObject * pNorth = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2375.0f, 2285.0f, 141.0f, 183947 );
		if ( pNorth != NULL && pPlayer->CalcDistance( pPlayer, pNorth ) < 30 )
		{
			pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
			pQuest->SendUpdateAddKill( 1 );
			pQuest->UpdatePlayerFields();
			return true;
		}
	}
	if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
	{
		GameObject * pWest = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2283.0f, 2181.0f, 95.0f, 183947 );
		if( pWest != NULL && pPlayer->CalcDistance( pPlayer, pWest ) < 30 )
		{
			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
			pQuest->SendUpdateAddKill( 2 );
			pQuest->UpdatePlayerFields();
			return true;
		}
	}

	return true;
}

bool RodofPurification(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10839);
	if(qle == NULL)
		return true;

	GameObject *  Darkstone = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-2512, 5418, 0, 185191);
	if(Darkstone != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, Darkstone) < 15)
			qle->SendQuestComplete();
	}
	return true;
}

bool AnUnusualPatron(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9457);
	if(qle == NULL)
		return true;

	Creature * Naias = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17207);
	if(Naias)
		return true;

	sEAS.SpawnCreature(pPlayer, 17207, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 60*10*1000);
	return true;
}

bool MagnetoCollector(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10584);
	if(qle == NULL)
		return true;

	Creature * magneto = TO_CREATURE(pSpell->GetUnitTarget());
	if(!magneto)
		return true;

	Creature * auramagneto = sEAS.SpawnCreature(pPlayer, 21731, magneto->GetPositionX(), magneto->GetPositionY(), magneto->GetPositionZ(), magneto->GetOrientation(), 0);
	magneto->Despawn(1, 0);
	auramagneto->Despawn(4*60*1000, 0);
  
	return true;
}

bool TemporalPhaseModulator(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10609);
	if(qle == NULL)
		return true;

	srand ( (int)time(NULL) );
	int random = static_cast<int>(rand()*4.33);
	srand ( random * 23 );

	Creature * whelp = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 20021);
	if(whelp)
	{
		if(rand()%2 == 0)
		{
			Creature * adolescent = sEAS.SpawnCreature(pPlayer, 21817, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			adolescent->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature * proto = sEAS.SpawnCreature(pPlayer, 21821, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			proto->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	whelp = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21817);
	if(whelp)
	{
		if(rand()%10 < 8 )
		{
			Creature * mature = sEAS.SpawnCreature(pPlayer, 21820, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			mature->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature * funnyd = sEAS.SpawnCreature(pPlayer, 21823, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			funnyd->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	whelp = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21821);
	if(whelp)
	{
		if(rand()%10 < 8 )
		{
			Creature * mature = sEAS.SpawnCreature(pPlayer, 21820, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			mature->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature * funnyd = sEAS.SpawnCreature(pPlayer, 21823, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			funnyd->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	
	whelp = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21823);
	if(whelp)
	{
		if(rand()%2 == 0)
		{
			Creature * adolescent = sEAS.SpawnCreature(pPlayer, 21817, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			adolescent->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature * proto = sEAS.SpawnCreature(pPlayer, 21821, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			proto->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}
  
	return true;
}

bool EmblazonRuneblade(uint32 i, Spell * pSpell)
{
	if( pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsPlayer() )
		return true;

	Player * pPlayer = TO_PLAYER( pSpell->p_caster );
	pPlayer->SendChatMessageToPlayer( CHAT_MSG_SYSTEM, LANG_UNIVERSAL, "Player check", pPlayer );

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12619 );
	if( qle == NULL )
		return true;

	sEAS.AddItem( 38631, pPlayer, 1 );
	pPlayer->GetItemInterface()->RemoveItemAmt( 38607, 1 );
	return true;
}

bool WyrmcallersHorn(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->p_caster == NULL )
		return true;
	
	Player * plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	Creature * pCreature = sEAS.SpawnCreature(plr, 24019, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	if( pCreature == NULL )
		return true;
	pCreature->Despawn(5*60*1000, 0);
	return true;
}

bool RaeloraszSpark(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->p_caster == NULL )
		return true;
	
	Player * plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	Creature * pCreature = sEAS.SpawnCreature(plr, 26237, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	pCreature->Despawn(5*60*1000, 0);
	
	QuestLogEntry *qle = plr->GetQuestLogForEntry(11969);
	if(qle == NULL)
		return true;
	qle->SendQuestComplete();
	return true;
}

bool RuneOfDistortion(uint32 i, Spell * pSpell)
{
	if ( pSpell == NULL || pSpell->p_caster == NULL )
		return true;
	
	Player * plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	Creature * pCreature = sEAS.SpawnCreature(plr, 32162, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	pCreature->Despawn(5*60*1000, 0);
	
	QuestLogEntry *qle = plr->GetQuestLogForEntry(13312);
	if(qle == NULL)
	{
		qle = plr->GetQuestLogForEntry(13337);
		if(qle == NULL)
			return true;
	}
	return true;
}

bool GoreBladder(uint32 i, Spell * pSpell)
{
	if( !pSpell->p_caster )
		return true;

	Unit * target = pSpell->GetUnitTarget();
	if( !target || target->GetEntry() != 29392 || !target->IsDead())
		return true;

	TO_CREATURE(target)->Despawn(500, 360000);

	Player * pPlayer = pSpell->p_caster;
	QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12810 );
	if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
	{
		en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
		en->SendUpdateAddKill( 0 );
		en->UpdatePlayerFields();
	}
	return true;
}

bool PlagueSpray(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 23652 || !target->isAlive())
  return true;
 else if(!target || target->GetEntry() != 23652 || !target->HasAura(40467)) 
  return false;
 
 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 11307 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
 en->UpdatePlayerFields();
   }
 return true;
}

bool GoblinWeatherMachine(uint32 i, Spell* pSpell)
{
 if(!pSpell->p_caster) return true;

 uint32 Weather = 46736 + RandomUInt(4);

 pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(Weather), true);
 return true;
} 

bool PurifiedAshes(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 26633 || !target->IsDead())
  return true;

 Player * pPlayer = pSpell->p_caster;
 int entry;
 
 if ( pPlayer->IsTeamHorde())
 {
	 entry = 12236;
 }
 else
 {
	 entry = 12249;
 }
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( entry );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
   }
 return true;
 }

bool DISMEMBER(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || ( target->GetEntry() != 23657 && target->GetEntry() != 23661 && target->GetEntry() != 23662 && target->GetEntry() != 23663 && target->GetEntry() != 23664 && target->GetEntry() != 23665 && target->GetEntry() != 23666 && target->GetEntry() != 23667 && target->GetEntry() != 23668  && target->GetEntry() != 23669 && target->GetEntry() != 23670)  || !target->IsDead())
  return true;
 
TO_CREATURE(target)->Despawn(500,300000);
  
 Player * pPlayer = pSpell->p_caster;
 int entry;

 if ( pPlayer->IsTeamHorde())
	{
	 entry = 11257;
	}
 else
	{
	 entry = 11246;
	}
	QuestLogEntry * en = pPlayer->GetQuestLogForEntry( entry );
	if( en == NULL)
	return true;
	else
		{
		if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0])
		en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
		en->SendUpdateAddKill( 0 );
		en->UpdatePlayerFields();
		}
			return true;
};
 
bool CraftyBlaster(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if (!target || ( target->GetEntry() != 25432 && target->GetEntry() != 25434 ) || !target->isAlive())
  return true;
  
 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 11653 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
   }
 return true;
}

bool EyesAbove(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 25432 || !target->isAlive())
  return true;
  
 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12453 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
	{
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
	}
 return true;
}

bool RagefistTorch(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || ( target->GetEntry() != 25342 && target->GetEntry() != 25343))
  return true;

 TO_CREATURE(target)->Despawn(500, 360000);

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 11593 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
   }
 return true;
 }

bool SummonShadra(uint32 i, Spell * pSpell)
{
	if( !pSpell->p_caster )
		return true;
	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(2707, SSX, SSY, SSZ, SSO, true, false, 0, 0);
		return true;
}

bool SummonEcheyakee(uint32 i, Spell * pSpell)
{
	if( !pSpell->p_caster || i != 1 )//Effect_1 = SEND_EVENT,Effect_2 = DUMMY
		return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(3475, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	
	return true;
}

bool HodirsHorn(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
  if( !target || ( target->GetEntry() != 29974 && target->GetEntry() != 30144 && target->GetEntry() != 30135)  || !target->IsDead())
  return true;

 TO_CREATURE(target)->Despawn(500, 360000);

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12977 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
   }
 return true;
}

bool TelluricPoultice(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 30035)
  return true;

 TO_CREATURE(target)->Despawn(500, 360000);

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12937 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
	en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
	en->SendUpdateAddKill( 0 );
	en->UpdatePlayerFields();
   }
 return true;
}

bool Screwdriver(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 25753 || !target->IsDead())
  return true;

 TO_CREATURE(target)->Despawn(500, 360000);

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 11730 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
 {
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
   }
 return true;
}

bool IncineratingOil(uint32 i, Spell * pSpell)

{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 28156 )
  return true;

 TO_CREATURE(target)->Despawn(500, 360000);

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12568 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
	{
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
	}
 return true;
}

bool SummonAquementas(uint32 i, Spell * pSpell)
{
	if( !pSpell->p_caster )
		return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(9453, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	
	return true;
}

bool PrayerBeads(uint32 i, Spell * pSpell)
{
 if( !pSpell->p_caster )
  return true;

 Unit * target = pSpell->GetUnitTarget();
 if( !target || target->GetEntry() != 22431 )
  return true;

 Player * pPlayer = pSpell->p_caster;
 QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 10935 );
 if( en && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0] )
	{
  en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );
  en->SendUpdateAddKill( 0 );
  en->UpdatePlayerFields();
	}
 return true;
}

void SetupQuestItems(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(3607, &YennikuRelease);
	mgr->register_dummy_spell(4141, &ScrollOfMyzrael);
	mgr->register_dummy_spell(8606, &SummonCyclonian);
	mgr->register_dummy_spell(10226, &ElementalPowerExtractor);
	mgr->register_dummy_spell(14209, &NixxsPledgeOfSecrecy);
	mgr->register_dummy_spell(15118, &TheBaitforLarkorwi1);
  	mgr->register_dummy_spell(15119, &TheBaitforLarkorwi2);
	mgr->register_dummy_spell(16996, &IncendiaPowder);
	mgr->register_dummy_spell(17016, &AllAlongtheWatchtowers);
	mgr->register_dummy_spell(19470, &GemOfTheSerpent);
	mgr->register_dummy_spell(20737, &KarangsBanner);
	mgr->register_dummy_spell(23359, &ZappedGiants);
	mgr->register_dummy_spell(29279, &TheCleansingMustBeStopped);
	mgr->register_dummy_spell(29297, &CleansingVial);
	mgr->register_dummy_spell(29314, &AdministreringtheSalve);
	mgr->register_dummy_spell(30015, &AnUnusualPatron);
	mgr->register_dummy_spell(30877, &CatchandRelease);
	mgr->register_dummy_spell(31736, &BalanceMustBePreserved);
	mgr->register_dummy_spell(31927, &BlessingofIncineratus);
	mgr->register_dummy_spell(32037, &ASpiritAlly);
	mgr->register_dummy_spell(34622, &UnyieldingBattleHorn);
	mgr->register_dummy_spell(34630, &ScrapReaver);
	mgr->register_dummy_spell(34646, &BuildingAPerimeter);
	mgr->register_dummy_spell(34992, &SummonEkkorash);
	mgr->register_dummy_spell(35113, &MeasuringWarpEnergies);
	mgr->register_dummy_spell(35413, &NaturalRemedies);
	mgr->register_dummy_spell(35460, &FuryoftheDreghoodElders);
	mgr->register_dummy_spell(37573, &TemporalPhaseModulator);
	mgr->register_dummy_spell(35707, &OpeningTyraliusPrison);
	mgr->register_dummy_spell(35772, &FloraoftheEcoDomes);
	mgr->register_dummy_spell(36310, &ADireSituation);
	mgr->register_dummy_spell(37065, &Torgos);
	mgr->register_dummy_spell(37136, &MagnetoCollector);
	mgr->register_dummy_spell(37426, &RuuanokClaw);
	mgr->register_dummy_spell(38177, &BlackwhelpNet);
	mgr->register_dummy_spell(38336, &WelcomingtheWolfSpirit);
	mgr->register_dummy_spell(38736, &RodofPurification);
	mgr->register_dummy_spell(39223, &CallRexxar);
	mgr->register_dummy_spell(39224, &Showdown);
	mgr->register_dummy_spell(39239, &EvilDrawsNear);
	mgr->register_dummy_spell(39238, &Fumping);
	mgr->register_dummy_spell(39246, &TheBigBoneWorm);
	mgr->register_dummy_spell(42390, &LayWreath);
	mgr->register_dummy_spell(43723, &CookingPot);
	mgr->register_dummy_spell(51770, &EmblazonRuneblade);
	mgr->register_dummy_spell(42817, &WyrmcallersHorn);
	mgr->register_dummy_spell(60036, &RuneOfDistortion);
	mgr->register_dummy_spell(62272, &RaeloraszSpark);
	mgr->register_dummy_spell( 6509, &GoreBladder);				// http://www.wowhead.com/?item=40551
	mgr->register_dummy_spell(43381, &PlagueSpray);				// http://www.wowhead.com/?item=33621
	mgr->register_dummy_spell(46203, &GoblinWeatherMachine);	// Goblin Weather machine
	mgr->register_dummy_spell(48549, &PurifiedAshes);			// http://www.wowhead.com/?item=37307
	mgr->register_dummy_spell(43036, &DISMEMBER);				//http://www.wowhead.com/?item=33342
	mgr->register_dummy_spell(51912, &CraftyBlaster);  			//http://www.wowhead.com/?item=34812
	mgr->register_dummy_spell(49546, &EyesAbove);  				//http://www.wowhead.com/?item=37877
	mgr->register_dummy_spell(51276, &IncineratingOil);			//http://www.wowhead.com/?item=38556
	mgr->register_dummy_spell(46023, &Screwdriver);				//http://www.wowhead.com/?item=35116
	mgr->register_dummy_spell(55804, &TelluricPoultice);			//http://www.wowhead.com/?item=41988
	mgr->register_dummy_spell(55983, &HodirsHorn);				//http://www.wowhead.com/?item=42164
	mgr->register_dummy_spell(12189, &SummonEcheyakee);
	mgr->register_dummy_spell(11548, &SummonShadra);
	mgr->register_dummy_spell(45474, &RagefistTorch);
	mgr->register_dummy_spell(13978, &SummonAquementas);		//http://www.wowhead.com/?quest=4005
	mgr->register_dummy_spell(39371, &PrayerBeads);				//http://www.wowhead.com/?quest=10935
}