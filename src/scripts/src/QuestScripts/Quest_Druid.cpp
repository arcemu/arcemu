/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
 * Copyright (C) 2005-2008 Ascent Team
 * Copyright (C) 2007-2008 Moon++ Team
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
/**********************
Edits by : FenixGman
**********************/
#include "Setup.h"

bool CenarionMoondust(uint32 i, Spell* pSpell) // Body And Heart (Alliance)
{
	if(!pSpell->p_caster)
		return true;

	if(!pSpell->p_caster->IsInWorld())
		return true;

  const float pos[] = {6335.2329f, 144.0811f, 24.0068f, 5.701f}; // x, y, z, o
  Player * p_caster = pSpell->p_caster;

  //Moonkin Stone aura
  GameObject * Msa = sEAS.SpawnGameobject(p_caster, 177644, 6331.01, 88.245, 22.6522, 2.01455, 1.0, 0.0, 0.0, 0.0, 0.0);

  // it dont delete lunaclaw if he is here
  Creature * lunaclaw;
  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);

  sEAS.CreateCustomWaypointMap(lunaclaw);
  uint32 md = lunaclaw->GetDisplayId();

  //Waypoints
  sEAS.WaypointCreate(lunaclaw, 6348.3833, 132.5197, 21.6042, 4.19, 200, 256, md);
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
	sEAS.WaypointCreate(lunaclaw, 5328.2148, 94.5505, 21.4547, 4.2489, 200, 256, md);
  }

  // Make sure that creature will attack player
  if( !lunaclaw->CombatStatus.IsInCombat() )
  {
	lunaclaw->GetAIInterface()->SetNextTarget(p_caster);
  }

  return true;
}

bool CenarionLunardust(uint32 i, Spell * pSpell) // Body And Heart (Horde)
{
	if(!pSpell->p_caster)
		return true;

	if(!pSpell->p_caster->IsInWorld())
		return true;

  const float pos[] = {-2443.9711f, -1642.8002f, 92.5129f, 1.71}; // x, y, z, o
  Player * p_caster = pSpell->p_caster;

  //Moonkin Stone aura
  GameObject * Msa = sEAS.SpawnGameobject(p_caster, 177644, -2499.54, -1633.03, 91.8121, 0.262894, 1.0, 0.0, 0.0, 0.0, 0.0);

  Creature * lunaclaw;

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);

  sEAS.CreateCustomWaypointMap(lunaclaw);
  uint32 md = lunaclaw->GetDisplayId();

  // Waypoints
  sEAS.WaypointCreate(lunaclaw, -2448.2253, -1625.0148, 91.89, 1.913, 200, 256, md); //First
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
	sEAS.WaypointCreate(lunaclaw, -2504.2641, -1630.7354, 91.93, 3.2, 200, 256, md);
  }

  // Make sure that creature will attack player
  if( !lunaclaw->CombatStatus.IsInCombat() )
  {
	lunaclaw->GetAIInterface()->SetNextTarget(p_caster);
  }

  return true;
}

class Lunaclaw : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(Lunaclaw);

  Lunaclaw(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnDied(Unit* mKiller)
  {
	if(!mKiller->IsPlayer())
	  return;

	Player* plr = TO_PLAYER(mKiller);

	if(plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL)
		return;
	sEAS.SpawnCreature(plr, 12144, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1*60*1000);
  }
};

// Lunaclaw ghost gossip
#define GOSSIP_GHOST_MOONKIN    "You have fought well, spirit. I ask you to grand me the strenght of your body and the strenght of your heart."

class SCRIPT_DECL MoonkinGhost_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4714, plr);

		if(plr->GetQuestLogForEntry(6002) != NULL)
		{
			Menu->AddItem( 0, GOSSIP_GHOST_MOONKIN, 1); //Horde
		}
		else if(plr->GetQuestLogForEntry(6001) != NULL)
		{
			Menu->AddItem( 0, GOSSIP_GHOST_MOONKIN, 2); //Ally
		}
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;

        case 1: //Horde
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4715, plr);
				Menu->SendTo(plr);

				QuestLogEntry *qle = plr->GetQuestLogForEntry(6002);
				if(qle == NULL)
				return;

				if(qle->GetMobCount(0) != 0)
				return;

				qle->SetMobCount(0, 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();

				pCreature->Emote(EMOTE_ONESHOT_WAVE);
				pCreature->Despawn(240000,0);
			}
			break;

		case 2: //Ally
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4715, plr);
				Menu->SendTo(plr);

				QuestLogEntry *qle = plr->GetQuestLogForEntry(6001);
				if(qle == NULL)
				return;

				if(qle->GetMobCount(0) != 0)
				return;

				qle->SetMobCount(0, 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();

				pCreature->Emote(EMOTE_ONESHOT_WAVE);
				pCreature->Despawn(240000,0);
			}
			break;

		}
	}
};

// bear ghost gossip
#define GOSSIP_GHOST_BEAR_A    "What do you represent, spirit?"
#define GOSSIP_GHOST_BEAR_B    "I seek to understand the importance of strength of the body."
#define GOSSIP_GHOST_BEAR_C    "I seek to understand the importance of strength of the heart."
#define GOSSIP_GHOST_BEAR_D    "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw."

class SCRIPT_DECL BearGhost_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4719, plr);

		if(plr->GetQuestLogForEntry(5930) != NULL) // horde
		{
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_A, 1);
		}
		else if(plr->GetQuestLogForEntry(5929) != NULL) // ally
		{
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_A, 5);
		}

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
        case 1:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4721, plr);
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_B, 2);
			Menu->SendTo(plr);
			break;
			}
				case 2:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4733, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_C, 3);
					Menu->SendTo(plr);
					break;
				}
				case 3:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4734, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_D, 4);
					Menu->SendTo(plr);
					break;
				}
			case 4:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, plr);
			
			QuestLogEntry *qle = plr->GetQuestLogForEntry(5930);
			if(qle == NULL)
			return;

			if(qle->GetMobCount(0) != 0)
			return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();

			Menu->SendTo(plr);
			break;
			}
		case 5:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4721, plr);
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_B, 6);
			Menu->SendTo(plr);
			break;
			}
				case 6:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4733, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_C, 7);
					Menu->SendTo(plr);
					break;
				}
				case 7:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4734, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_D, 8);
					Menu->SendTo(plr);
					break;
				}
			case 8:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, plr);
			
			QuestLogEntry *qle = plr->GetQuestLogForEntry(5929);
			if(qle == NULL)
			return;

			if(qle->GetMobCount(0) != 0)
			return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();

			Menu->SendTo(plr);
			break;
			}
		}
	}
};

class MoongladeQuest : public QuestScript
{	
public:
  void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
  {
	if(!mTarget->HasSpell(19027))
	  mTarget->CastSpell(mTarget, dbcSpell.LookupEntry(19027), true);
  }
};

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
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

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

void SetupDruid(ScriptMgr * mgr)
{
	
  GossipScript * MoonkinGhostGossip = new MoonkinGhost_Gossip;
  GossipScript * BearGhostGossip = new BearGhost_Gossip;
  QuestScript *Moonglade = new MoongladeQuest();
  mgr->register_quest_script(5921, Moonglade);
  mgr->register_quest_script(5922, Moonglade);
  mgr->register_creature_script(12138, &Lunaclaw::Create);
  mgr->register_dummy_spell(19138, &CenarionLunardust);
  mgr->register_dummy_spell(18974, &CenarionMoondust);
  mgr->register_dummy_spell(19512, &CurativeAnimalSalve);
  mgr->register_dummy_spell(19719, &TrialOfTheLake);

  //Register gossip scripts
  mgr->register_gossip_script(12144, MoonkinGhostGossip); // Ghost of Lunaclaw
  mgr->register_gossip_script(11956, BearGhostGossip); // Great Bear Spirit
  
}

