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

bool CleansingVial(uint32 i, Spell * pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;
    
	Player * pPlayer = pSpell->p_caster;

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
	if ( pSpell->u_caster == NULL )
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
	if ( pSpell->p_caster == NULL )
		return true;
    
	Player * pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;
	
	Player * plr = pSpell->p_caster;
	if( plr == NULL )
		return true;

	sEAS.SpawnCreature(plr, 19493, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	return true;
}

bool CallRexxar(uint32 i, Spell * pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;
    
	Player * pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;
    
	Player * pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;

	Player * pPlayer = pSpell->p_caster;
	Creature * pCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature( 19851, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), true, false, 0, 0 );
	if ( pCreature != NULL )
	{
		pCreature->Despawn( 600000, 0 );
	}

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

bool ADireSituation(uint32 i, Spell * pSpell)
{
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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

bool FuryoftheDreghoodElders(uint32 i, Spell * pSpell)
{
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
	  return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9847);
  if(qle == NULL)
    return true;

  Creature * allyspirit = sEAS.SpawnCreature(pPlayer, 18185, -353, 7255, 49.36f, 6.28f, 0);
  allyspirit->Despawn(6*60*1000, 0);
  return true;
}

bool BalanceMustBePreserved( uint32 i, Aura * pAura, bool apply )
{
	if( !apply )
		return true;

	if( !pAura->GetCaster()->IsPlayer() )
		return true;

	Player * pPlayer = TO_PLAYER( pAura->GetCaster() );
	if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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

bool TagMurloc( uint32 i, Aura * pAura, bool apply )
{
	Object *Caster = pAura->GetCaster();

	if( !Caster->IsPlayer() )
		return false;

	if( !pAura->GetTarget()->IsCreature() )
		return false;

	if( !apply )
		return true;
	
	Player * pPlayer = TO_PLAYER( Caster );
	
	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9629);
	if(qle == NULL)
		return true;
	
	Creature * murloc = TO_CREATURE( pAura->GetTarget() );
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
	Player * pPlayer = pSpell->p_caster;
    if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
  	Player * pPlayer = pSpell->p_caster;
  	if(!pPlayer)
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
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
	  return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(656);
  if(qle == NULL)
    return true;

  const float MyzraelPos[] = {-940.7374f, -3111.1953f, 48.9566f, 3.327f};

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
  	Player * pPlayer = pSpell->p_caster;
  	if(!pPlayer)
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
  	Player * pPlayer = pSpell->p_caster;
  	if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9370);
	if(qle == NULL)
		return true;

	Creature * draenei1 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);

	Creature * draenei2 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()-RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei2->Despawn(6*60*1000, 0);

	Creature * draenei3 = sEAS.SpawnCreature(pPlayer, 16994, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()-RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
	draenei3->Despawn(6*60*1000, 0);
	return true;
}

bool AdministreringtheSalve( uint32 i, Aura * pAura, bool apply )
{
	Object *m_caster = pAura->GetCaster();
	if( !m_caster->IsPlayer() )
		return false;

	if( !pAura->GetTarget()->IsCreature() )
		return false;

	if( apply ){
		Player * pPlayer = TO_PLAYER( m_caster );
		
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9447);
		
		if(qle == NULL)
			return true;

		Creature * sick = TO_CREATURE( pAura->GetTarget() );
		
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
	}else{
	}

  return true;
}

bool ZappedGiants(uint32 i, Spell * pSpell)
{
  Player * pPlayer = pSpell->p_caster;
  if(!pPlayer)
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
	if ( pSpell->p_caster == NULL )
		return true;
    
	Player * pPlayer = pSpell->p_caster;
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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

bool MagnetoCollector( uint32 i, Aura * pAura, bool apply )
{
	if( !pAura->GetCaster()->IsPlayer() )
		return true;

	Player * pPlayer = TO_PLAYER( pAura->GetCaster() );

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10584);
	if(qle == NULL)
		return true;

	Creature * magneto = TO_CREATURE( pAura->GetTarget() );
	if(!magneto)
		return true;

	Creature * auramagneto = sEAS.SpawnCreature(pPlayer, 21731, magneto->GetPositionX(), magneto->GetPositionY(), magneto->GetPositionZ(), magneto->GetOrientation(), 0);
	magneto->Despawn(1, 0);
	auramagneto->Despawn(4*60*1000, 0);
  
	return true;
}

bool TemporalPhaseModulator(uint32 i, Spell * pSpell)
{
	Player * pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
	if( pSpell->p_caster == NULL )
		return true;

	Player * pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;
	
	Player * plr = pSpell->p_caster;

	Creature * pCreature = sEAS.SpawnCreature(plr, 24019, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	if( pCreature == NULL )
		return true;
	pCreature->Despawn(5*60*1000, 0);
	return true;
}

bool RaeloraszSpark(uint32 i, Spell * pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;
	
	Player * plr = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;
	
	Player * plr = pSpell->p_caster;
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

bool CleansingVialDND( uint32 i, Spell *s ){
	QuestLogEntry *en= s->p_caster->GetQuestLogForEntry(9427);
	
	if(!en)
		return false;

	en->SendQuestComplete();

	return true;
}

bool HunterTamingQuest( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();
	Player *p_caster = a->GetPlayerCaster();

	if( p_caster == NULL )
		return true;
	
	if (apply){
		m_target->GetAIInterface()->AttackReaction( a->GetUnitCaster(), 10, 0);
	}
	else{
		uint32 TamingSpellid = a->GetSpellProto()->EffectMiscValue[ 1 ];
		
		SpellEntry *triggerspell = dbcSpell.LookupEntryForced( TamingSpellid );
		if(triggerspell == NULL)
		{
			sLog.outError("An Aura with spellid %u is calling HunterTamingQuest() with an invalid TamingSpellid: %u", a->GetSpellId(), TamingSpellid);
			return true;
		}

		Quest* tamequest = QuestStorage.LookupEntry( triggerspell->EffectMiscValue[1] );
		if(tamequest == NULL)
		{
			sLog.outError("An Aura with spellid %u is calling HunterTamingQuest() with an invalid tamequest id: %u", a->GetSpellId(), triggerspell->EffectMiscValue[1]);
			return true;
		}

		if ( !p_caster->GetQuestLogForEntry(tamequest->id ) || m_target->GetEntry() != static_cast<uint32>( tamequest->required_mob[0] ))
		{
			p_caster->SendCastResult( triggerspell->Id, SPELL_FAILED_BAD_TARGETS, 0, 0 );
		}
		else if( !a->GetTimeLeft() )
		{
			// Creates a 15 minute pet, if player has the quest that goes with the spell and if target corresponds to quest
			if( Rand( 75.0f ) )// 75% chance on success
			{
				
				if( m_target->IsCreature() )
				{
					Creature *tamed = TO_CREATURE(m_target);
					tamed->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, p_caster, 0 );
					
					Pet *pPet = objmgr.CreatePet( tamed->GetEntry() );
					if( ! pPet->CreateAsSummon( tamed->GetEntry(), tamed->GetCreatureInfo(), tamed, p_caster, triggerspell, 2, 900000 ) )
					{
						pPet->DeleteMe();//CreateAsSummon() returns false if an error occurred.
						pPet = NULL;
					}

					tamed->Despawn(1,0);//we despawn the tamed creature once we are out of Aura::Remove()
					
					QuestLogEntry *qle = p_caster->GetQuestLogForEntry( tamequest->id );
					if( qle != NULL )
					{
						qle->SetMobCount( 0, 1 );
						qle->SendUpdateAddKill( 1 );
						qle->UpdatePlayerFields();
						qle->SendQuestComplete();
					}
				}
			}
			else
			{
				p_caster->SendCastResult( triggerspell->Id,SPELL_FAILED_TRY_AGAIN,0,0 );
			}
		}
	}

	return true;
}

bool ArcaneDisruption( uint32 i, Aura * pAura, bool apply )
{
	if( !apply )
		return true;

	if( pAura->GetPlayerCaster() == NULL )
		return false;

	Player* plr = pAura->GetPlayerCaster();

	QuestLogEntry* pQuest = plr->GetQuestLogForEntry(13149);
	if( !pQuest )
		return false;
	GameObject* crate = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 190094);
	if( crate )
	{
		GameObject* go = plr->GetMapMgr()->CreateGameObject(190095);
		go->CreateFromProto(190095, crate->GetMapMgr()->GetMapId(), crate->GetPositionX(), crate->GetPositionY(), crate->GetPositionZ(), crate->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f);
		go->PushToWorld(crate->GetMapMgr());
		crate->Despawn(0,0);
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		if( pQuest->GetMobCount( 0 ) == 5 )
		{//weee, Uther
			CreatureProto *cp = CreatureProtoStorage.LookupEntry(26528);
			CreatureInfo *ci = CreatureNameStorage.LookupEntry(26528);
			Creature* c = NULL;
			if (cp && ci)
			{
				c = plr->GetMapMgr()->CreateCreature( 26528 );
				if (c) {
					//position is guessed
					c->Load(cp,1759.4351f, 1265.3317f, 138.052f, 0.1902f);
					c->PushToWorld(plr->GetMapMgr());
				}
			}
		}
	}
	return true;
};

bool ToLegionHold(uint32 i, Aura* pAura, bool apply)
{
	if( pAura == NULL )
		return true;

	Player* pPlayer = pAura->GetPlayerCaster();
	if( pPlayer == NULL )
		return true;

	Creature* pJovaanCheck = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-3310.743896f, 2951.929199f, 171.132538f, 21633);
	if ( pJovaanCheck != NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10563 );
	if ( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry( 10596 );
		if ( pQuest == NULL )
			return true;
	}

	if ( apply )
	{

		pPlayer->Root();
		Creature* pJovaan = sEAS.SpawnCreature( pPlayer, 21633, -3310.743896f, 2951.929199f, 171.132538f, 5.054039f, 0 );	// Spawn Jovaan
		if ( pJovaan != NULL )
		{
			pJovaan->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
			if ( pJovaan->GetAIInterface() != NULL )
			{
				pJovaan->GetAIInterface()->SetAllowedToEnterCombat( false );
			}
		}
		GameObject* pGameObject = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 184834);
		if ( pGameObject != NULL )
		{
			pGameObject->Despawn(60000, 0);
			pPlayer->UpdateNearbyGameObjects();
		}
	}
	else
	{
		if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
		{
			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
			pQuest->SendUpdateAddKill( 2 );
			pQuest->UpdatePlayerFields();
		}
	
		pPlayer->Unroot();
	}

	return true;
}

bool CenarionMoondust(uint32 i, Spell* pSpell) // Body And Heart (Alliance)
{
	if(!pSpell->p_caster)
		return true;

	if(!pSpell->p_caster->IsInWorld())
		return true;

  const float pos[] = {6335.2329f, 144.0811f, 24.0068f, 5.701f}; // x, y, z, o
  Player * p_caster = pSpell->p_caster;

  //Moonkin Stone aura
  sEAS.SpawnGameobject(p_caster, 177644, 6331.01f, 88.245f, 22.6522f, 2.01455f, 1.0, 0.0, 0.0, 0.0, 0.0);

  // it dont delete lunaclaw if he is here
  Creature * lunaclaw;
  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);

  sEAS.CreateCustomWaypointMap(lunaclaw);
  uint32 md = lunaclaw->GetDisplayId();

  //Waypoints
  sEAS.WaypointCreate(lunaclaw, 6348.3833f, 132.5197f, 21.6042f, 4.19f, 200, 256, md);
  //make sure that player dont cheat speed or something
  if( lunaclaw->GetDistance2dSq(p_caster) < 200 ) // can be more? - he can speed hack or teleport hack
  {
	float x = p_caster->GetPositionX();
	float y = p_caster->GetPositionY();
	float z = p_caster->GetPositionZ();
	float o = p_caster->GetOrientation() + 3;
	sEAS.WaypointCreate(lunaclaw, x, y, z, o, 200, 256, md);
  }
  else
  {
	sEAS.WaypointCreate(lunaclaw, 5328.2148f, 94.5505f, 21.4547f, 4.2489f, 200, 256, md);
  }

  // Make sure that creature will attack player
  if( !lunaclaw->CombatStatus.IsInCombat() )
  {
	lunaclaw->GetAIInterface()->setNextTarget(p_caster);
  }

  return true;
}

bool CenarionLunardust(uint32 i, Spell * pSpell) // Body And Heart (Horde)
{
	if(!pSpell->p_caster)
		return true;

	if(!pSpell->p_caster->IsInWorld())
		return true;

  const float pos[] = {-2443.9711f, -1642.8002f, 92.5129f, 1.71f}; // x, y, z, o
  Player * p_caster = pSpell->p_caster;

  //Moonkin Stone aura
  sEAS.SpawnGameobject(p_caster, 177644, -2499.54f, -1633.03f, 91.8121f, 0.262894f, 1.0, 0.0, 0.0, 0.0, 0.0);

  Creature * lunaclaw;

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);

  sEAS.CreateCustomWaypointMap(lunaclaw);
  uint32 md = lunaclaw->GetDisplayId();

  // Waypoints
  sEAS.WaypointCreate(lunaclaw, -2448.2253f, -1625.0148f, 91.89f, 1.913f, 200, 256, md); //First
  //make sure that player dont cheat speed or something
  if( lunaclaw->GetDistance2dSq(p_caster) < 200 ) // can be more? - he can speed hack or teleport hack
  {
	float x = p_caster->GetPositionX();
	float y = p_caster->GetPositionY();
	float z = p_caster->GetPositionZ();
	float o = p_caster->GetOrientation() + 3;
	sEAS.WaypointCreate(lunaclaw, x, y, z, o, 200, 256, md);
  }
  else
  {
	sEAS.WaypointCreate(lunaclaw, -2504.2641f, -1630.7354f, 91.93f, 3.2f, 200, 256, md);
  }

  // Make sure that creature will attack player
  if( !lunaclaw->CombatStatus.IsInCombat() )
  {
	lunaclaw->GetAIInterface()->setNextTarget(p_caster);
  }

  return true;
}

bool CurativeAnimalSalve(uint32 i, Spell* pSpell) // Curing the Sick
{
	Player* caster = pSpell->p_caster;
	if(caster == NULL)
		return true;

	if(!pSpell->GetUnitTarget()->IsCreature())
		return true;

	Creature* target = TO_CREATURE(pSpell->GetUnitTarget());
	uint32 entry = target->GetEntry();

	if(entry == 12296 || entry == 12298)
	{
		QuestLogEntry *qle = caster->GetQuestLogForEntry(6129);
		if(qle == NULL)
		{
			qle = caster->GetQuestLogForEntry(6124);
			if(qle == NULL)
				return true;
		}

		if(qle->GetQuest()->required_mobcount[0] == qle->GetMobCount(0))
			return true;

		if(entry == 12298) // Sickly Deer
			sEAS.SpawnCreature(caster, 12298, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Deer
		else // Sickly Gazelle
			sEAS.SpawnCreature(caster, 12297, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Gazelle

		target->Despawn(0, 3*60*1000);

		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();

		return true;
	}
	return true;
}

// Trial Of The Lake

bool TrialOfTheLake(uint32 i, Spell* pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 28 );
	if ( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry( 29 );
		if ( pQuest == NULL )
			return true;
	}

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		
		return true;
	}
	
	return true;
}

bool SymbolOfLife(uint32 i, Spell* pSpell) // Alliance ress. quests
{
   Player* plr = pSpell->p_caster; 
   if(!plr)
    return true;

  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  
  if(target == NULL)
    return true;

  const uint32 targets[] = {17542, 6177, 6172}; 
  const uint32 quests[] =  {9600,  1783, 1786};
  bool questOk = false;
  bool targetOk = false;

  for(uint8 j = 0; j<3; j++)
  {
    if(target->GetEntry() == targets[j])
    {
      targetOk = true;
      
      break;
    }
  }
  
  if(!targetOk)
    return true;
  
  QuestLogEntry *qle;
  
  for(uint8 j = 0; j<3; j++)
  {
    if(plr->GetQuestLogForEntry(quests[j]) != NULL)
    {
      qle = plr->GetQuestLogForEntry(quests[j]);
      questOk = true;
      
      break;
    }
  }
  
  if(!questOk)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(10*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();
  
  return true;
}

bool FilledShimmeringVessel(uint32 i, Spell* pSpell) // Blood Elf ress. quest
{
  if(pSpell->p_caster == NULL)
    return true;

  Player* plr = pSpell->p_caster;
  
  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  if(target == NULL)
    return true;

  if(target->GetEntry() != 17768)
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9685);
  if(qle == NULL)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(30*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

bool DouseEternalFlame(uint32 i, Spell* pSpell)
{
	if (pSpell->p_caster == NULL)
		return true;

	Player* plr = pSpell->p_caster;
	QuestLogEntry *qle = plr->GetQuestLogForEntry(9737);
	if(qle == NULL)
		return true;

	GameObject* Flame = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3678, -3640, 139, 182068);
	if(Flame != NULL)
	{
		if(plr->CalcDistance(plr, Flame) < 30)
			if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
				Creature * pCreature = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 10917 );
				if ( pCreature != NULL )
				{
					pCreature->SetFaction(11 );
				}
			}
	}
	return true;
}

bool Triage(uint32 i, Spell* pSpell)
{
	// Unit* target = pSpell->GetUnitTarget();
	// if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;
	if(!pSpell->p_caster || pSpell->GetUnitTarget() == NULL) return true;
		pSpell->p_caster->CastSpell(pSpell->GetUnitTarget(), dbcSpell.LookupEntry(746), true);
		QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(6624);

			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
		uint32 newcount = en->GetMobCount(0) + 1;
		en->SetMobCount(0, newcount);
		en->SendUpdateAddKill(0);
		en->UpdatePlayerFields();
		}
	// Creature* c_target = (Creature*)target;
	// if(!c_target) return true;

	// uint32 creatureID = c_target->GetEntry();

	// if(creatureID == 12937)
		// sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);

	return true;
}

bool NeutralizingTheCauldrons(uint32 i, Spell* pSpell)
{
	if ( pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11647);
	if ( pQuest == NULL )
		return true;

	GameObject* pCauldron = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 187690 );
	if(pCauldron == NULL)
		return true;

	float posX = pCauldron->GetPositionX();

	if(posX == 3747.07f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 0 );

	if(posX == 4023.5f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 1 );

	if(posX == 4126.12f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 2 );
	
	return true;
}

// Stop the Plague
bool HighmessasCleansingSeeds(uint32 i, Spell * pSpell)
{
	if ( pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player*  pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11677 );
	if ( !pQuest )
		return true;

	sEAS.KillMobForQuest( pPlayer, pQuest, 0);
	return true;
}

// There's Something Going On In Those Caves
bool BixiesInhibitingPowder(uint32 i, Spell * pSpell)
{
	if ( pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11694 );
	if ( !pQuest )
		return true;

	sEAS.KillMobForQuest( pPlayer, 11694, 0);
	return true;
}

// Leading the Ancestors Home
bool CompleteAncestorRitual(uint32 i, Spell * pSpell)
{
	if( pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11610 );
	if( !pQuest )
		return true;

	GameObject* pElderObj;
	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191088);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 0);
		return true;
	}

	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191089);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 1);
		return true;
	}

	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191090);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 2);
		return true;
	}

	return true;
}

bool PoweringOurDefenses(uint32 i, Spell* pSpell)
{
	if(pSpell->p_caster == NULL)
    return true;

	Player * plr = pSpell->p_caster;

	QuestLogEntry *qle = plr->GetQuestLogForEntry( 8490 );
	if( qle == NULL )
		return true;

	// Angelis : Need to script the scourge attack

	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	return true;
}

/*--------------------------------------------------------------------------------------------------------*/
// Testing the Antidote

bool TestingTheAntidote(uint32 i, Spell* pSpell)
{
	if(!pSpell->GetUnitTarget() || !pSpell->GetUnitTarget()->IsCreature())
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

	spawned->GetAIInterface()->setNextTarget(pSpell->u_caster);

	return true;
}

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
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;

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
// The Seer's Relic

bool TheSeersRelic(uint32 i, Spell* pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;

	Unit* pUnit = pSpell->GetUnitTarget();
	if ( pUnit == NULL || !pUnit->IsCreature() || pUnit->GetEntry() != 19354 )
		return true;

	//pPlayer->BroadcastMessage("blaah");	// Really blizzlike?
	
	sEAS.SpawnCreature( pPlayer, 20680, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ(), pUnit->GetOrientation(), 5 * 60 * 1000 );
	TO_CREATURE(pUnit)->Despawn( 0, 3 * 60 * 1000 );
	return true;
}

// War is Hell
bool WarIsHell(uint32 i, Spell* pSpell)
{
	if( pSpell->p_caster == NULL )
		return true;

	Player* plr = pSpell->p_caster;

	Creature* target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 24008 );
	if( target == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11270);
  
	if( qle == NULL )
		return true;
 
	GameObject* obj = NULL;

	sEAS.KillMobForQuest( plr, qle, 0 );

	obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
	sEAS.GameobjectDelete(obj, 1*30*1000);

	target->Despawn( 2000, 60*1000 );
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

// A Lesson in Fear
bool PlantForsakenBanner(uint32 i, Spell* pSpell)
{
	if(pSpell->p_caster == NULL)
		return true;

	Player* pPlayer = pSpell->p_caster;

	QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(11282);
	if( pQuest == NULL )
		return true;
	
	Creature* target = TO< Creature* >(pSpell->GetUnitTarget());
	if ( target == NULL || target->isAlive() )
		return true;

	uint32 cit = target->GetEntry();
	switch(cit)
	{
	case 24161:
		sEAS.KillMobForQuest( pPlayer, 11282, 0 );
		target->Despawn(0, 3*60*1000);
		break;
	case 24016:
		sEAS.KillMobForQuest( pPlayer, 11282, 1 );
		target->Despawn(0, 3*60*1000);
		break;
	case 24162:
		sEAS.KillMobForQuest( pPlayer, 11282, 2 );
		target->Despawn(0, 3*60*1000);
		break;
	}
	return true;
}

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
	if(pSpell->p_caster == NULL)
		return true;

	Player* pPlayer = pSpell->p_caster;

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


bool ShipBombing(uint32 i, Spell* pSpell)
{
	Player* pPlayer = pSpell->p_caster;
	if(!pPlayer)
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
	
	GameObject* pSinloren = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13200.232422f, -7049.176270f, 3.838517f, 550000);
	GameObject* pBloodoath = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13319.419922f, -6988.779785f, 4.002993f, 550000);
	GameObject* pDawnchaser = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13274.51625f, -7145.434570f, 4.770292f, 550000);
	
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
				
				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13214.3f, -7059.19f, 17.5717f, 1.58573f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);
				
				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13204.2f, -7059.38f, 17.5717f, 1.57787f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);

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

				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13329.4f, -6994.70f, 14.5219f, 1.38938f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);

				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13315.4f, -6990.72f, 14.7647f, 1.25979f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);

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

				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13284.1f, -7152.65f, 15.9774f, 1.44828f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);

				obj = sEAS.SpawnGameobject(pPlayer, GO_FIRE, 13273.0f, -7151.21f, 15.9774f, 1.39723f, 1, 0, 0, 0, 0);
				sEAS.GameobjectDelete(obj, 2*60*1000);
			
			}
		}
	}
	return true;
}

bool ImpaleEmissary(uint32 i, Spell* pSpell)
{
	if(pSpell->p_caster == NULL)
	return true;

	Player* pPlayer = pSpell->p_caster;

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
	if( pSpell->p_caster == NULL )
		return true;

	Player * pPlayer = pSpell->p_caster;
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
	if(pSpell->p_caster == NULL)
		return true;

	Player * pPlayer = pSpell->p_caster;
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

bool InducingVision(uint32 i, Spell* pSpell)
{
		if(!pSpell->p_caster) return true;
		Player* mTarget = pSpell->p_caster;
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return true;
		  Creature* creat = mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2983, -2238.994873f, -408.009552f, -9.424423f, 5.753043f, true, false, 0, 0);
		  creat->GetAIInterface()->setMoveType(11);

		  sEAS.CreateCustomWaypointMap(creat);

		  sEAS.WaypointCreate(creat,-2240.521729f, -407.114532f, -9.424648f, 5.753043f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2225.764404f, -419.251404f, -9.360950f, 5.575544f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2200.883789f, -440.999634f, -5.606637f, 5.599899f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2143.712646f, -468.068665f, -9.401191f, 0.797177f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2100.811279f, -420.980194f, -5.322747f, 0.918914f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2079.469482f, -392.465576f, -10.262321f, 0.930689f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2043.699707f, -343.802155f, -6.971242f, 0.934694f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2001.858521f, -242.533966f, -10.763200f, 0.952265f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1924.751343f, -119.969292f, -11.770116f, 0.754775f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1794.804565f, -7.919222f, -9.326016f, 0.711578f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1755.206421f, 72.430847f, 1.121445f, 1.103484f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1734.550049f, 116.842003f, -4.337420f, 3.350100f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1720.041870f, 125.933235f, -2.325089f, 0.903206f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1704.406860f, 183.596405f, 12.065074f, 0.691148f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1674.317261f, 201.597565f, 11.235887f, 0.475164f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1624.068726f, 223.555389f, 2.074259f, 0.416259f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1572.863403f, 234.714813f, 2.305119f, 0.781471f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1542.866943f, 277.896759f, 20.543165f, 1.256637f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1541.813232f, 316.415649f, 49.910889f, 1.248783f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1526.979126f, 329.664001f, 61.835552f, 1.182024f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1524.173584f, 335.237610f, 63.325703f, 1.173092f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1513.968506f, 355.759338f, 63.064487f, 1.119193f, 0, 256, 0);
		  sEAS.EnableWaypoints(creat);
		  return true;
}

// Stopping the Spread
bool StoppingTheSpread(uint32 i, Spell* pSpell)
{
	if( pSpell->p_caster == NULL )
		return true;

	Player* plr = pSpell->p_caster;

	Creature* target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 18240 );
	if( target == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9874);
  
	if( qle == NULL )
		return true;
 
	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount( 0, qle->GetMobCount( 0 ) + 1 );
		qle->SendUpdateAddKill( 0 );
		
		GameObject* obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
		sEAS.GameobjectDelete(obj, 1*30*1000);
	};

	target->Despawn( 2000, 60*1000 );
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
		return true;
}

//Ruthless Cunning
bool RuthlessCunning(uint32 i, Spell* pSpell)
{
	if(pSpell->p_caster == NULL)
		return true;
	
	Player* plr = pSpell->p_caster;

	Creature* kilsorrow = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ());
	if( kilsorrow == NULL || kilsorrow->isAlive() )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9927);
	if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
	{
		kilsorrow->Despawn(0, 60000);
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	};

	return true;
}

bool FindingTheKeymaster(uint32 i, Spell* pSpell)
{
	if(pSpell->p_caster == NULL)
		return true;

	Player* plr = pSpell->p_caster;

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
	if(pSpell->p_caster == NULL)
		return true;

	Player* plr = pSpell->p_caster;
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
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;

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

bool CrystalOfDeepShadows(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
  if(pSpell->p_caster == NULL)
    return true;

  Player* plr = pSpell->p_caster;
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10833);
  
  if(qle == NULL)
    return true;

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

bool Carcass(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(pSpell->p_caster == NULL)
		return true;

	Player*	pPlayer = pSpell->p_caster;
	QuestLogEntry	*pQuest = pPlayer->GetQuestLogForEntry( 10804 );
	Creature*	NetherDrake = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21648);
	GameObject* FlayerCarcass = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 185155);

	if ( FlayerCarcass == NULL )
	{
		FlayerCarcass = sEAS.SpawnGameobject(pPlayer, 185155, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, 1, 0, 0, 0, 0);
		FlayerCarcass->Despawn(60000, 0);
	}
	if ( NetherDrake == NULL )
		return true;

	if ( NetherDrake->HasAura(38502) )
		return true;

	if( pQuest!=NULL && pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		NetherDrake->CastSpell(NetherDrake, dbcSpell.LookupEntry( 38502 ), true);
		NetherDrake->GetAIInterface()->m_moveFly = true;
		NetherDrake->GetAIInterface()->MoveTo(pPlayer->GetPositionX(), pPlayer->GetPositionY()+2, pPlayer->GetPositionZ(), 0);
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 )+1);
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
	}
	return true;
}

bool ForceofNeltharakuSpell(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(pSpell->p_caster == NULL)
		return true;

	Player*	pPlayer= pSpell->p_caster;
	Creature* pTarget =pPlayer->GetMapMgr()->GetCreature(GET_LOWGUID_PART(pPlayer->GetSelection()));

	if(pTarget == NULL)
		return true;

	QuestLogEntry	 *pQuest	= pPlayer->GetQuestLogForEntry(10854);
	if(pQuest == NULL)
		return true;

	if(pTarget->GetEntry() == 21722 && pPlayer->CalcDistance(pTarget)<30)
	{
		if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pTarget->CastSpell(pPlayer, dbcSpell.LookupEntry(38775), true);
			pQuest->SetMobCount(0, pQuest->GetMobCount(0)+1);
			pQuest->SendUpdateAddKill(0);
			pQuest->UpdatePlayerFields();
			if ( 1 )
			{
				pTarget->Unroot();
				pTarget->GetAIInterface()->setWaypointToMove( 0 );
			}
		}
	}
	return true;
}

bool UnlockKarynakuChains(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(pSpell->p_caster == NULL)
		return true;

	Player*	pPlayer= pSpell->p_caster;
	QuestLogEntry	 *pQuest	= pPlayer->GetQuestLogForEntry(10872);
	if(pQuest == NULL)
		return true;

	if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount(0, pQuest->GetMobCount(0)+1);
		pQuest->SendUpdateAddKill(0);
		pQuest->UpdatePlayerFields();
	}
	return true;
}


bool ShatariTorch(uint32 i, Spell* pSpell)
{
  if(pSpell->p_caster == NULL)
    return true;

  Player* plr = pSpell->p_caster;
  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  
  if(target == NULL)
    return true;
	
  if ( plr->CalcDistance( target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ() ) > 5 )
	return true;
   
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10913);
  if(qle == NULL)
    return true;
  
  GameObject* obj = NULL;

  if(target->GetEntry() == 21859)
  {
    if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
      return true;

    qle->SetMobCount(0, qle->GetMobCount(0)+1);
    qle->SendUpdateAddKill(0);

    obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
    sEAS.GameobjectDelete(obj, 1*60*1000);
  } 
  else if(target->GetEntry() == 21846)
  {
    if(qle->GetMobCount(1) == qle->GetQuest()->required_mobcount[1])
      return true;

    qle->SetMobCount(1, qle->GetMobCount(1)+1);
    qle->SendUpdateAddKill(1);

    obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
    sEAS.GameobjectDelete(obj, 1*60*1000);
  }
  else 
    return true; 

  target->Despawn(0, 1*60*1000);
  qle->UpdatePlayerFields();
  plr->UpdateNearbyGameObjects();

  return true;
}

// Lost!

bool SpragglesCanteen(uint32 i, Spell* pSpell)
{
  if(pSpell->p_caster == NULL)
    return true;

  Player* plr = pSpell->p_caster;
  
  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  if(target == NULL)
    return true;

  if(target->GetEntry() != 9999)
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(4492);
  if(qle == NULL)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(30*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

//Finding the Source

bool FindingTheSource(uint32 i, Spell* pSpell)
{
	Player* pPlayer = pSpell->p_caster;
	if(!pPlayer)
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(974);
	if(qle == NULL)
		return true;

	GameObject* place1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7163, -1149, -264, 148503);
	GameObject* place2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7281, -1244, -248, 148503);
	GameObject* place3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7140, -1465, -242, 148503);
	GameObject* place4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7328, -1461, -242, 148503);
	GameObject* place5 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7092, -1305, -187, 148503);

	if(place1 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, place1) < 11)
			pPlayer->CastSpell(pPlayer, 14797, true);		
	}
	if(place2 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, place2) < 11)
			pPlayer->CastSpell(pPlayer, 14797, true);
	}
	if(place3 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, place3) < 11)
			pPlayer->CastSpell(pPlayer, 14797, true);
	}
	if(place4 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, place4) < 11)
			pPlayer->CastSpell(pPlayer, 14797, true);
	}
	if(place5 != NULL)
	{
		if(pPlayer->CalcDistance(pPlayer, place5) < 11)
		{
			if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
			}
		}
	}
	return true;
}

// quest 5163 - Are We There, Yeti?
bool ReleaseUmisYeti( uint32 i, Spell * pSpell )
{
	if ( pSpell->p_caster == NULL || pSpell->GetUnitTarget() == NULL || !pSpell->GetUnitTarget()->IsCreature() )
		return true;

	QuestLogEntry * qLogEntry = pSpell->p_caster->GetQuestLogForEntry( 5163 );
	if ( qLogEntry == NULL )
		return true;

	Creature * target = TO< Creature* >( pSpell->GetUnitTarget() );
	static const uint32 friends[] = { 10978, 7583, 10977 };
	for ( uint32 j = 0; j < sizeof( friends ) / sizeof( uint32 ); j++ )
	{
		if ( target->GetEntry() == friends[j] && qLogEntry->GetMobCount( j ) < qLogEntry->GetQuest()->required_mobcount[j] )
		{
			qLogEntry->SetMobCount( j, 1 );
			qLogEntry->SendUpdateAddKill( j );
			qLogEntry->UpdatePlayerFields();
			return true;
		}
	}

	return true;
}

// Healing The Lake

bool HealingTheLake(uint32 i, Spell* pSpell)
{
	if ( pSpell->p_caster == NULL )
		return true;

	Player* pPlayer = pSpell->p_caster;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9294 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		
		return true;
	}
	
	return true;
}

// Protecting Our Own
bool ProtectingOurOwn(uint32 i, Spell* pSpell)
{
   if(pSpell->p_caster == NULL)
      return true;

   Player* plr = pSpell->p_caster;
   QuestLogEntry *qle = plr->GetQuestLogForEntry(10488);
   
   if(qle == NULL)
      return true;

   if ( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[0] )
   {
      uint32 NewCount = qle->GetMobCount( 0 ) + 1;
      qle->SetMobCount( 0, NewCount );
      qle->SendUpdateAddKill( 0 );
      qle->UpdatePlayerFields();
   }

   return true;
}

void SetupQuestItems(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(3607, &YennikuRelease);
	mgr->register_dummy_spell(4141, &ScrollOfMyzrael);
	mgr->register_dummy_spell(8606, &SummonCyclonian);

	mgr->register_script_effect(15118, &TheBaitforLarkorwi1);
	mgr->register_script_effect(15119, &TheBaitforLarkorwi2);
	mgr->register_script_effect(20737, &KarangsBanner);
	mgr->register_dummy_spell(23359, &ZappedGiants);	
	mgr->register_script_effect(29279, &TheCleansingMustBeStopped);
	
	mgr->register_dummy_aura( 29314, &AdministreringtheSalve );

	mgr->register_script_effect(30015, &AnUnusualPatron);
	mgr->register_dummy_aura( 30877, &TagMurloc );

	mgr->register_dummy_aura(31736, &BalanceMustBePreserved);
	mgr->register_dummy_spell(31927, &BlessingofIncineratus);
	mgr->register_script_effect(32037, &ASpiritAlly);
	mgr->register_script_effect(34622, &UnyieldingBattleHorn);
	mgr->register_script_effect(34630, &ScrapReaver);
	mgr->register_dummy_spell(34646, &BuildingAPerimeter);
	mgr->register_script_effect(34992, &SummonEkkorash);
	mgr->register_dummy_spell(35113, &MeasuringWarpEnergies);
	mgr->register_script_effect(35413, &NaturalRemedies);
	mgr->register_dummy_spell(35460, &FuryoftheDreghoodElders);
	mgr->register_dummy_spell(37573, &TemporalPhaseModulator);
	mgr->register_dummy_spell(35772, &FloraoftheEcoDomes);
	mgr->register_script_effect(36310, &ADireSituation);
	mgr->register_script_effect(37065, &Torgos);
	mgr->register_dummy_aura(37136, &MagnetoCollector);
	mgr->register_script_effect(37426, &RuuanokClaw);
	mgr->register_script_effect(38336, &WelcomingtheWolfSpirit);
	mgr->register_script_effect(38729, &RodofPurification);
	mgr->register_script_effect(39223, &CallRexxar);
	mgr->register_script_effect(39224, &Showdown);
	mgr->register_script_effect(39239, &EvilDrawsNear);
	mgr->register_dummy_spell(39238, &Fumping);
	mgr->register_dummy_spell(39246, &TheBigBoneWorm);
	mgr->register_script_effect(42390, &LayWreath);
	mgr->register_dummy_spell(43723, &CookingPot);
	mgr->register_script_effect(51770, &EmblazonRuneblade);
	mgr->register_dummy_spell(42817, &WyrmcallersHorn);
	mgr->register_script_effect(60036, &RuneOfDistortion);
	mgr->register_dummy_spell(62272, &RaeloraszSpark);
	mgr->register_dummy_spell( 6509, &GoreBladder);				// http://www.wowhead.com/?item=40551
	mgr->register_dummy_spell(43385, &PlagueSpray);				// http://www.wowhead.com/?item=33621
	mgr->register_script_effect(46203, &GoblinWeatherMachine);	// Goblin Weather machine
	mgr->register_dummy_spell(48549, &PurifiedAshes);			// http://www.wowhead.com/?item=37307
	mgr->register_dummy_spell(43036, &DISMEMBER);				//http://www.wowhead.com/?item=33342
	mgr->register_script_effect(45668, &CraftyBlaster);  			//http://www.wowhead.com/?item=34812
	mgr->register_dummy_spell(51276, &IncineratingOil);			//http://www.wowhead.com/?item=38556
	mgr->register_dummy_spell(46023, &Screwdriver);				//http://www.wowhead.com/?item=35116
	mgr->register_dummy_spell(55804, &TelluricPoultice);			//http://www.wowhead.com/?item=41988
	mgr->register_dummy_spell(55983, &HodirsHorn);				//http://www.wowhead.com/?item=42164
	mgr->register_dummy_spell(12189, &SummonEcheyakee);
	mgr->register_dummy_spell(11548, &SummonShadra);
	mgr->register_dummy_spell(45474, &RagefistTorch);
	mgr->register_script_effect(13978, &SummonAquementas);		//http://www.wowhead.com/?quest=4005
	mgr->register_dummy_spell(39371, &PrayerBeads);				//http://www.wowhead.com/?quest=10935

	mgr->register_script_effect( 29297, &CleansingVial );

	uint32 huntertamingquestspellids[] = {
		19548,
		19674,
		19687,
		19688,
		19689,
		19692,
		19693,
		19694,
		19696,
		19697,
		19699,
		19700,
		30099,
		30102,
		30105,
		30646,
		30653,
		30654,
		0
	};
	mgr->register_dummy_aura( huntertamingquestspellids, &HunterTamingQuest );
	
	mgr->register_dummy_aura(49590, &ArcaneDisruption);

	mgr->register_dummy_aura(37097, &ToLegionHold);
	
	mgr->register_script_effect(19138, &CenarionLunardust);
	mgr->register_script_effect(18974, &CenarionMoondust);
	mgr->register_dummy_spell(19512, &CurativeAnimalSalve);
	mgr->register_script_effect(19719, &TrialOfTheLake);
	
	mgr->register_dummy_spell(8593, &SymbolOfLife);
	mgr->register_dummy_spell(31225, &FilledShimmeringVessel);
	mgr->register_script_effect(31497, &DouseEternalFlame);

	mgr->register_dummy_spell(20804, &Triage);

	mgr->register_dummy_spell(45653, &NeutralizingTheCauldrons);
	// Stop the Plague
	mgr->register_dummy_spell(45834, &HighmessasCleansingSeeds);
	// There's Something Going On In Those Caves
	mgr->register_dummy_spell(45835, &BixiesInhibitingPowder);
	// Leading the Ancestors Home
	mgr->register_dummy_spell(45536, &CompleteAncestorRitual);

	mgr->register_dummy_spell(28247, &PoweringOurDefenses); // need to script event

	mgr->register_dummy_spell(34665, &TestingTheAntidote);
	mgr->register_dummy_spell(35724, &ZethGorMustBurnHorde);
	mgr->register_dummy_spell(32979, &LayingWasteToTheUnwantedAlliance);
	mgr->register_dummy_spell(33067, &BurnItUp);
	mgr->register_script_effect(30489, &TheSeersRelic);
	mgr->register_dummy_spell(34387, &DisruptTheirReinforcements);
	mgr->register_dummy_spell(42793, &WarIsHell);
	mgr->register_dummy_spell(43178, &PlantForsakenBanner);

	mgr->register_dummy_spell(45109, &OrbOfMurlocControl);
	mgr->register_dummy_spell(44997, &ConvertingSentry);
	mgr->register_dummy_spell(45115, &ShipBombing);
	mgr->register_dummy_spell(45030, &ImpaleEmissary);
	mgr->register_dummy_spell( 45191, &LeyLine );
	mgr->register_dummy_spell( 44969, &ManaRemnants );

	/*mgr->register_dummy_spell(4981, &InducingVision);*/

	mgr->register_dummy_spell(32146, &StoppingTheSpread);
	mgr->register_script_effect(32307, &RuthlessCunning);

	mgr->register_script_effect(34717, &FindingTheKeymaster);
	mgr->register_dummy_spell(35372, &TheFleshLies);
	mgr->register_dummy_spell(35246, &SurveyingtheRuins);

	mgr->register_script_effect(39094, &CrystalOfDeepShadows);
	mgr->register_dummy_spell(38439, &Carcass);
	mgr->register_dummy_spell(38762, &ForceofNeltharakuSpell);

	mgr->register_dummy_spell(39189, &ShatariTorch);

  	mgr->register_dummy_spell(15591, &SpragglesCanteen);
	mgr->register_dummy_spell(16378, &FindingTheSource);

	mgr->register_dummy_spell( 17166, &ReleaseUmisYeti );

	mgr->register_script_effect(28700, &HealingTheLake);

	mgr->register_script_effect(32578, &ProtectingOurOwn);


}