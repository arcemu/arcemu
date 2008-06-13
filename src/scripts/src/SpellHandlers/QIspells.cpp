/*
 * WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 *
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

bool NixxsPledgeOfSecrecy(uint32 i, Spell* pSpell)
{
    Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;
    sEAS.AddItem(11270, plr);

  return true;
}

bool CleansingVial(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;
	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9427);
	if(qle == NULL)
		return true;

	Creature *agg = sEAS.SpawnCreature(plr, 17000, 428.15, 3461.73, 63.40, 0, 0);
	agg->Despawn(6*60*1000, 0);
	return true;
}

bool DouseEternalFlame(uint32 i, Spell* pSpell)
{
    Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;
    //Creature *creat = plr->GetMapMgr()->GetSqlIdCreature(415415);
	Creature *creat = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 10917);
    if(creat == NULL)
      return true;;
	creat->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 11);
  return true;
}

bool SummonCyclonian(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster == NULL)
		return true;
	pSpell->u_caster->GetMapMgr()->GetInterface()->SpawnCreature(6239, pSpell->u_caster->GetPositionX(),pSpell->u_caster->GetPositionY(),pSpell->u_caster->GetPositionZ(),pSpell->u_caster->GetOrientation(), true, false, 0, 0)->Despawn(600000, 0);
	return true;
}

bool AwakenPeon(uint32 i, Spell* pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT)
		return true;
    Player *plr = (Player*)pSpell->p_caster;
	Creature* c_target = (Creature*)target;
	if(!c_target->HasAura(17743))
				return true;
	if(c_target->GetEntry() == 10556)
	{
		WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
		data << uint32(6197) << target->GetGUID();
		plr->SendMessageToSet(&data, true);
		c_target->RemoveAllAuras();
		string npc_say;
		npc_say = "Ow!  Ok, I'll get back to work, ";
		npc_say+=plr->GetName();
		npc_say+="!";
		c_target->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, npc_say.c_str());
		c_target->Emote(EMOTE_STATE_WORK_NOSHEATHE_CHOPWOOD);
		sQuestMgr.OnPlayerKill(plr, c_target);
	}
	return true;
}

bool ElementalPowerExtractor(uint32 i, Spell* pSpell)
{
    Player *plr = (Player*)pSpell->u_caster;
    Creature * creat = ((Creature*)pSpell->GetUnitTarget());
	if(creat->GetEntry() == 18881 || creat->GetEntry() == 18865 && creat->isAlive())
		sEAS.AddItem( 28548, plr);
	return true;
}

bool SummonEkkorash(uint32 i, Spell* pSpell)
{
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(19493, 2760.869629f, 3204.398438f, 151.432007f, 5.876738f, true, false, 0, 0);
	return true;
}

bool OpeningTyraliusPrison(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(plr->GetQuestLogForEntry(10442))
	{
		sEAS.SpawnCreature(plr, 20787, plr->GetPositionX(),plr->GetPositionY(),plr->GetPositionZ(),plr->GetOrientation(), 2*60*1000);
		QuestLogEntry *qle = plr->GetQuestLogForEntry(10442);
		qle->SendQuestComplete();
	}
	return true;
}

bool CallRexxar(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(plr->GetQuestLogForEntry(10742))
	{
		sEAS.SpawnCreature(plr, 21984, plr->GetPositionX(),plr->GetPositionY(),plr->GetPositionZ(),plr->GetOrientation(), 2*60*1000);
		sEAS.SpawnCreature(plr, 20555, plr->GetPositionX(),plr->GetPositionY(),plr->GetPositionZ(),plr->GetOrientation(), 2*60*1000);
	}
	return true;
}

bool LayWreath(uint32 i, Spell* pSpell) //Peace at Last quest
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11152);
	if(qle == NULL)
		return true;

	GameObject *wreath = sEAS.SpawnGameobject(plr, 501541, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation(), 1);
	sEAS.GameobjectDelete(wreath, 2*60*1000);
	
	qle->SetMobCount(0, 1);
	qle->SendUpdateAddKill(0);
	qle->UpdatePlayerFields();
	return true;
}

bool ScrapReaver(uint32 i, Spell* pSpell)
{
	if(!pSpell->p_caster)
		return true;
	Player *plr = (Player*)pSpell->p_caster;
	plr->GetMapMgr()->GetInterface()->SpawnCreature(19851, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation(), true, false, 0, 0)->Despawn(600000, 0);
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
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(21767, 3210.960693f, 5348.308594f, 144.537476f, 5.450696f, true, false, 0, 0);
	return true;
}

bool KarangsBanner(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	Player* Plr = pSpell->p_caster;

	// Banner Aura
	Plr->CastSpell(Plr, dbcSpell.LookupEntry(20746), true);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2231.710205f, -1543.603027f, 90.694946f, 4.700579f, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2232.534912f, -1556.983276f, 89.744415f, 1.527570f, true, false, 0, 0);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12757, 2239.357178f, -1546.649536f, 89.671097f, 3.530336f, true, false, 0, 0);

	return true;
}

bool BlackwhelpNet(uint32 i, Spell * pSpell)
{
    Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

    if(!pSpell->u_caster->IsPlayer())
        return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10747);
    if(qle == NULL)
        return true;

    Creature *whelp = static_cast<Creature*>(pSpell->GetUnitTarget());
	if(!whelp)
		return true;

    whelp->Despawn(1000, 6*60*1000);

	sEAS.AddItem(31130, plr);
	return true;
}

bool SpragglesCanteen(uint32 i, Spell* pSpell) // Quest: Lost!
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  
  Creature *target = plr->GetMapMgr()->GetCreature((uint32)plr->GetSelection());
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

bool ADireSituation(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10506);
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

bool AllAlongtheWatchtowers(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(5097);
	if(qle == NULL)
	{
		qle = plr->GetQuestLogForEntry(5098);
		if(qle == NULL)
			return true;
	}

    GameObject *tower1 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1304, -1318, 64, 300030);
	GameObject *tower2 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1311, -1589, 61, 300030);
	GameObject *tower3 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1481, -1414, 67, 300030);
	GameObject *tower4 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1557, -1470, 68, 300030);

	if(tower1 != NULL)
	{
		if(plr->CalcDistance(plr, tower1) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(tower2 != NULL)
	{
		if(plr->CalcDistance(plr, tower2) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(tower3 != NULL)
	{
		if(plr->CalcDistance(plr, tower3) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(tower4 != NULL)
	{
		if(plr->CalcDistance(plr, tower4) < 30)
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
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;



  QuestLogEntry *qle = plr->GetQuestLogForEntry(10369);
  if(qle == NULL)
    return true;

  Creature *arzethpower = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 19354);

  if(arzethpower == NULL)
	  return true;

  Creature *arzethless = sEAS.SpawnCreature(plr, 20680, arzethpower->GetPositionX(), arzethpower->GetPositionY(), arzethpower->GetPositionZ(), arzethpower->GetOrientation(), 0);
  arzethpower->Despawn(1, 6*60*1000);
  arzethless->Despawn(5*60*1000, 0);
  return true;

}

bool ASpiritAlly(uint32 i, Spell* pSpell) 
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9847);
  if(qle == NULL)
    return true;

  Creature *allyspirit = sEAS.SpawnCreature(plr, 18185, -353, 7255, 49.36, 6.28, 0);
  allyspirit->Despawn(6*60*1000, 0);
  return true;
}

bool ASpiritGuide(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9410);
  if(qle == NULL)
    return true;
  
  Creature *spiritwolf = sEAS.SpawnCreature(plr, 17077, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation(), 0);
  spiritwolf->Despawn(10*1000, 0);

  plr->CastSpell(plr, 29938, false);
  return true;
}

bool BalanceMustBePreserved(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9720);
	if(qle == NULL)
		return true;

    GameObject *lake1 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-756, 5926, 19, 300076);
	GameObject *lake2 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-213, 6302, 21, 300076);
	GameObject *lake3 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(498, 8197, 21, 300076);
	GameObject *lake4 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(566, 6358, 23, 300076);

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

bool BlessingofIncineratus(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9805);
	if(qle == NULL)
		return true;

	GameObject *big = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1972, 6276, 56, 300077);
	GameObject *east = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1841, 6387, 52, 400050);
	GameObject *west = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1920, 6361, 56, 400051);
	GameObject *south = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1808, 6293, 59, 400052);

	if(big != NULL)
	{
		if(plr->CalcDistance(plr, big) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(east != NULL)
	{
		if(plr->CalcDistance(plr, east) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(south != NULL)
	{
		if(plr->CalcDistance(plr, south) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(plr->CalcDistance(plr, west) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool BurnItUp(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10087);
	if(qle == NULL)
		return true;

	GameObject *east = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-300, 2407, 50, 185122);
	GameObject *west = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-350, 2708, 35, 183122);


	if(east != NULL)
	{
		if(plr->CalcDistance(plr, east) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}

	if(west != NULL)
	{
		if(plr->CalcDistance(plr, west) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool CatchandRelease(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9629);
  if(qle == NULL)
	return true;

  Creature *murloc = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(!murloc)
	  return true;

  Creature *tagged = sEAS.SpawnCreature(plr, 17654, murloc->GetPositionX(), murloc->GetPositionY(), murloc->GetPositionZ(), 0);
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

bool CookingPot(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
    if(!plr)
	  return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11379);
	if(qle == NULL)
		return true;
				
	plr->GetItemInterface()->RemoveItemAmt(31673, 1);
	plr->GetItemInterface()->RemoveItemAmt(31672, 2);
	sEAS.AddItem(33848, plr);
	return true;
}

bool EvilDrawsNear(uint32 i, Spell* pSpell) 
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10923);
  if(qle == NULL)
    return true;

  Creature *dragon = sEAS.SpawnCreature(plr, 22441, plr->GetPositionX()+15, plr->GetPositionY()+15, plr->GetPositionZ(), plr->GetOrientation(), 0);
  dragon->Despawn(6*60*1000, 0);
  return true;
}

bool UnyieldingBattleHorn(uint32 i, Spell * pSpell)
{
  if(pSpell->p_caster == NULL) 
    return true;

  Creature *creat = pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(19862, -1190.856079f, 2261.246582f, 46.625797f, 1.705882f, true, false, 0, 0);
  creat->Despawn(300000, 0); // 5 mins delay

  return true;
}

bool LayingWastetotheUnwanted(uint32 i, Spell* pSpell)
{
    Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

    if(!pSpell->u_caster->IsPlayer())
        return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10078);
	if(qle == NULL)
		return true;
	
	GameObject *east = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-156, 2517, 43, 183123);
	GameObject *meast = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-154, 2661, 44, 183123);
	GameObject *mwest = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-172, 2773, 35, 183123);
	GameObject *west = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-172, 2818, 27, 183123);

	if(east != NULL)
	{
		if(plr->CalcDistance(plr, east) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(meast != NULL)
	{
		if(plr->CalcDistance(plr, meast) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(mwest != NULL)
	{
		if(plr->CalcDistance(plr, mwest) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(plr->CalcDistance(plr, west) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool MeasuringWarpEnergies(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10313);
	if(qle == NULL)
		return true;

	GameObject *north = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3216, 4045, 145, 300094);
	GameObject *east = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2766, 3865, 145, 300094);
	GameObject *west = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2958, 4318, 145, 300094);
	GameObject *south = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2817, 4337, 145, 300094);

	if(north != NULL)
	{
		if(plr->CalcDistance(plr, north) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(east != NULL)
	{
		if(plr->CalcDistance(plr, east) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(south != NULL)
	{
		if(plr->CalcDistance(plr, south) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(plr->CalcDistance(plr, west) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool YennikuRelease(uint32 i, Spell* pSpell)
{
  	Player *plr = (Player*)pSpell->u_caster;
  	if(!plr)
		return true;

  	if(!pSpell->u_caster->IsPlayer())
    	return true;

  	QuestLogEntry *qle = plr->GetQuestLogForEntry(592);
  	if(qle == NULL)
    	return true;
  
  	Creature *yenniku = static_cast<Creature*>(pSpell->GetUnitTarget());
  	if(!yenniku)
		return true;

  	yenniku->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 29);
  	yenniku->GetAIInterface()->WipeTargetList();
  	yenniku->Despawn(30*1000, 60*1000);

  	return true;
}

bool ScrollOfMyzrael(uint32 i, Spell* pSpell) 
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(656);
  if(qle == NULL)
    return true;

  const float MyzraelPos[] = {-940.7374, -3111.1953, 48.9566, 3.327};

  Creature *myzrael = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], 2755);
  if(myzrael != NULL)
  {
    if(!myzrael->isAlive())
      myzrael->Delete();
    else
      return true;
  }

  myzrael = sEAS.SpawnCreature(plr, 2755, MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], MyzraelPos[3], 0);
  return true;
}

bool Showdown(uint32 i, Spell* pSpell)
{
  Player *p_caster = pSpell->p_caster;
  if(!p_caster)
	  return true;

  QuestLogEntry *qle = p_caster->GetQuestLogForEntry(10742);
  if(qle == NULL)
  {
	  qle = p_caster->GetQuestLogForEntry(10806);
	  if(qle == NULL)
	    return true;
  }

  Creature *goc = NULL;
  goc = sEAS.SpawnCreature(p_caster, 20555, 3739, 5365, -4, 3.5, 0);
  goc->Despawn(6*60*1000, 0);
 
  return true;
}

bool StoppingtheSpread(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9874);
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

bool EternalVigilance(uint32 i, Spell* pSpell)
{
  Player *p_caster = pSpell->p_caster;
  if(!p_caster)
	  return true;

  QuestLogEntry *qle = p_caster->GetQuestLogForEntry(11001);
  if(qle == NULL)
	  return true;

  Creature *boss = sEAS.SpawnCreature(p_caster, 23035, -87.3546, 288.006, 26.4832, 0, 0);
 
  return true;
}

bool TheBaitforLarkorwi(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(4292);
  if(qle == NULL)
    return true;
  
  Creature *larkowi = sEAS.SpawnCreature(plr, 9684, plr->GetPositionX()+2, plr->GetPositionY()+3, plr->GetPositionZ(), plr->GetOrientation(), 0);
  larkowi->Despawn(5*60*1000, 0);

  return true;
}

bool TheBigBoneWorm(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10930);
	if(qle == NULL)
		return true;

	Creature *exarch = sEAS.SpawnCreature(plr, 22038, plr->GetPositionX()+7, plr->GetPositionY()+7, plr->GetPositionZ(), plr->GetOrientation(), 0);
	exarch->Despawn(6*60*1000, 0);

	return true;
}

bool TheSeersRelic(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9545);
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

bool ThirstUnending(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(8346);
  if(!qle)
	  return true;

  Creature *manawyrm = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(!manawyrm)
	  return true;

  if (manawyrm->GetEntry() == 15274)
  {
	if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
	  qle->SetMobCount(0, qle->GetMobCount(0)+1);
	  qle->SendUpdateAddKill(0);
	  qle->UpdatePlayerFields();
	}
  }
  return true;
}

bool Torgos(uint32 i, Spell* pSpell) 
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10035);
  if(qle == NULL)
    return true;

  Creature *torgos = sEAS.SpawnCreature(plr, 18707, plr->GetPositionX(), plr->GetPositionY()-10, plr->GetPositionZ(), plr->GetOrientation(), 0);
  if(!torgos)
	  return true;

  torgos->Despawn(6*60*1000, 0);
  return true;
}

bool WelcomingtheWolfSpirit(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10791);
  if(qle == NULL)
    return true;
  
  Creature *spiritwolf = sEAS.SpawnCreature(plr, 19616, plr->GetPositionX()+2, plr->GetPositionY()+3, plr->GetPositionZ(), plr->GetOrientation(), 0);
  spiritwolf->Despawn(5*60*1000, 0);

  if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
	qle->SetMobCount(0, qle->GetMobCount(0)+1);
	qle->SendUpdateAddKill(0);
	qle->UpdatePlayerFields();
  }
  return true;
}

bool NaturalRemedies(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10351);
  if(qle == NULL)
	return true;

  Creature *colos = sEAS.SpawnCreature(plr, 19305, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 0);
  colos->Despawn(5*60*1000, 0);

  return true;
}

bool FloraoftheEcoDomes(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10426);
  if(qle == NULL)
	return true;

  Creature *normal = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(!normal)
	  return true;

  Creature *mutant = sEAS.SpawnCreature(plr, 20983, normal->GetPositionX(), normal->GetPositionY(), normal->GetPositionZ(), 0);
  normal->Despawn(1, 6*60*1000);
  mutant->Despawn(5*60*1000, 0);

  if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
	  qle->SetMobCount(0, qle->GetMobCount(0)+1);
	  qle->SendUpdateAddKill(0);
	  qle->UpdatePlayerFields();
  }
  return true;
}

bool TheCleansingMustBeStopped(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;
	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9370);
	if(qle == NULL)
		return true;

	Creature *draenei1 = sEAS.SpawnCreature(plr, 16994, plr->GetPositionX()+RandomFloat(5.0f), plr->GetPositionY()+RandomFloat(5.0f), plr->GetPositionZ(), plr->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);

	Creature *draenei2 = sEAS.SpawnCreature(plr, 16994, plr->GetPositionX()-RandomFloat(5.0f), plr->GetPositionY()+RandomFloat(5.0f), plr->GetPositionZ(), plr->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);

	Creature *draenei3 = sEAS.SpawnCreature(plr, 16994, plr->GetPositionX()+RandomFloat(5.0f), plr->GetPositionY()-RandomFloat(5.0f), plr->GetPositionZ(), plr->GetOrientation(), 0);
	draenei1->Despawn(6*60*1000, 0);
	return true;
}

bool AdministreringtheSalve(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9447);
  if(qle == NULL)
	return true;

  Creature *sick = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(!sick)
	  return true;

  Creature *healed = sEAS.SpawnCreature(plr, 16846, sick->GetPositionX(), sick->GetPositionY(), sick->GetPositionZ(), 0);
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

bool ZappedGiants(uint32 i, Spell* pSpell)
{
  Player *plr = (Player*)pSpell->u_caster;
  if(!plr)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(7003);
  if(qle == NULL)
  {
	  QuestLogEntry *qle = plr->GetQuestLogForEntry(7725);
	  if(qle == NULL)
		  return true;
  }

  Creature* creat = static_cast<Creature*>(pSpell->GetUnitTarget());

  float X = creat->GetPositionX();
  float Y = creat->GetPositionY();
  float Z = creat->GetPositionZ();

  uint32 cit = creat->GetEntry();

  switch(cit)
  {
  case 5360:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature *zappedcreat = sEAS.SpawnCreature(plr, 14639, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5361:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature *zappedcreat = sEAS.SpawnCreature(plr, 14638, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5359:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature *zappedcreat = sEAS.SpawnCreature(plr, 14603, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5358:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature *zappedcreat = sEAS.SpawnCreature(plr, 14640, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5357:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature *zappedcreat = sEAS.SpawnCreature(plr, 14604, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  }
  return true;
}

bool BuildingAPerimeter(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10313);
	if(qle == NULL)
		return true;

	GameObject *north = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2375, 2285, 141, 183947);
	GameObject *east = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2257, 2465, 101, 183947);
	GameObject *west = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2283, 2181, 95, 183947);

	if(east != NULL)
	{
		if(plr->CalcDistance(plr, north) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(north != NULL)
	{
		if(plr->CalcDistance(plr, east) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(west != NULL)
	{
		if(plr->CalcDistance(plr, west) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool RodofPurification(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10839);
	if(qle == NULL)
		return true;

	GameObject* Darkstone = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-2512, 5418, 0, 185191);
	if(Darkstone != NULL)
	{
		if(plr->CalcDistance(plr, Darkstone) < 15)
			qle->SendQuestComplete();
	}
	return true;
}

bool AnUnusualPatron(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9457);
	if(qle == NULL)
		return true;

	Creature* Naias = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 17207);
	if(Naias)
		return true;

	sEAS.SpawnCreature(plr, 17207, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation(), 60*10*1000);
	return true;
}

bool Sphere(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10584);
	if(qle == NULL)
		return true;

	Creature *magneto = static_cast<Creature*>(pSpell->GetUnitTarget());
	if(!magneto)
		return true;

	Creature *auramagneto = sEAS.SpawnCreature(plr, 21731, magneto->GetPositionX(), magneto->GetPositionY(), magneto->GetPositionZ(), magneto->GetOrientation(), 0);
	magneto->Despawn(1, 0);
	auramagneto->Despawn(4*60*1000, 0);
  
	return true;
}

bool FindingtheSource(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(974);
	if(qle == NULL)
		return true;

	GameObject *place1 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7163, -1149, -264, 148503);
	GameObject *place2 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7281, -1244, -248, 148503);
	GameObject *place3 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7140, -1465, -242, 148503);
	GameObject *place4 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7328, -1461, -242, 148503);
	GameObject *place5 = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-7092, -1305, -187, 148503);

	if(place1 != NULL)
	{
		if(plr->CalcDistance(plr, place1) < 11)
			plr->CastSpell(plr, 14797, true);		
	}
	if(place2 != NULL)
	{
		if(plr->CalcDistance(plr, place2) < 11)
			plr->CastSpell(plr, 14797, true);
	}
	if(place3 != NULL)
	{
		if(plr->CalcDistance(plr, place3) < 11)
			plr->CastSpell(plr, 14797, true);
	}
	if(place4 != NULL)
	{
		if(plr->CalcDistance(plr, place4) < 11)
			plr->CastSpell(plr, 14797, true);
	}
	if(place5 != NULL)
	{
		if(plr->CalcDistance(plr, place5) < 11)
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

bool TemporalPhaseModulator(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10609);
	if(qle == NULL)
		return true;

	srand ( time(NULL) );
	int random = static_cast<int>(rand()*4.33);
	srand ( random * 23 );

	Creature *whelp = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 20021);
	if(whelp)
	{
		if(rand()%2 == 0)
		{
			Creature *adolescent = sEAS.SpawnCreature(plr, 21817, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			adolescent->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature *proto = sEAS.SpawnCreature(plr, 21821, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			proto->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	whelp = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 21817);
	if(whelp)
	{
		if(rand()%10 < 8 )
		{
			Creature *mature = sEAS.SpawnCreature(plr, 21820, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			mature->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature *funnyd = sEAS.SpawnCreature(plr, 21823, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			funnyd->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	whelp = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 21821);
	if(whelp)
	{
		if(rand()%10 < 8 )
		{
			Creature *mature = sEAS.SpawnCreature(plr, 21820, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			mature->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature *funnyd = sEAS.SpawnCreature(plr, 21823, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			funnyd->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
	}

	
	whelp = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 21823);
	if(whelp)
	{
		if(rand()%2 == 0)
		{
			Creature *adolescent = sEAS.SpawnCreature(plr, 21817, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			adolescent->Despawn(5*60*1000, 0);
		}
		else
		{
			Creature *proto = sEAS.SpawnCreature(plr, 21821, whelp->GetPositionX(), whelp->GetPositionY(), whelp->GetPositionZ(), whelp->GetOrientation(), 0);
			proto->Despawn(5*60*1000, 0);
		}
		whelp->Despawn(1, 0);
		return true;
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
	mgr->register_dummy_spell(15118, &TheBaitforLarkorwi);
	mgr->register_dummy_spell(15591, &SpragglesCanteen);
	mgr->register_dummy_spell(16378, &FindingtheSource);
	mgr->register_dummy_spell(16996, &IncendiaPowder);
	mgr->register_dummy_spell(17016, &AllAlongtheWatchtowers);
	mgr->register_dummy_spell(19470, &GemOfTheSerpent);
	mgr->register_dummy_spell(19938, &AwakenPeon);
	mgr->register_dummy_spell(20737, &KarangsBanner);
	mgr->register_dummy_spell(23359, &ZappedGiants);
	mgr->register_dummy_spell(28734, &ThirstUnending);
	mgr->register_dummy_spell(29279, &TheCleansingMustBeStopped);
	mgr->register_dummy_spell(29297, &CleansingVial);
	mgr->register_dummy_spell(29314, &AdministreringtheSalve);
	mgr->register_dummy_spell(29731, &ASpiritGuide);
	mgr->register_dummy_spell(30015, &AnUnusualPatron);
	mgr->register_dummy_spell(30489, &TheSeersRelic);
	mgr->register_dummy_spell(30877, &CatchandRelease);
	mgr->register_dummy_spell(31497, &DouseEternalFlame);
	mgr->register_dummy_spell(31736, &BalanceMustBePreserved);
	mgr->register_dummy_spell(31927, &BlessingofIncineratus);
	mgr->register_dummy_spell(32037, &ASpiritAlly);
	mgr->register_dummy_spell(32146, &StoppingtheSpread);
	mgr->register_dummy_spell(32979, &LayingWastetotheUnwanted);
	mgr->register_dummy_spell(33067, &BurnItUp);
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
	mgr->register_dummy_spell(37136, &Sphere);
	mgr->register_dummy_spell(37426, &RuuanokClaw);
	mgr->register_dummy_spell(38177, &BlackwhelpNet);
	mgr->register_dummy_spell(38336, &WelcomingtheWolfSpirit);
	mgr->register_dummy_spell(38736, &RodofPurification);
	mgr->register_dummy_spell(39223, &CallRexxar);
	mgr->register_dummy_spell(39224, &Showdown);
	mgr->register_dummy_spell(39239, &EvilDrawsNear);
	mgr->register_dummy_spell(39246, &TheBigBoneWorm);
	mgr->register_dummy_spell(40098, &EternalVigilance);
	mgr->register_dummy_spell(42390, &LayWreath);
	mgr->register_dummy_spell(43723, &CookingPot);
}