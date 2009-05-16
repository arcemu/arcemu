/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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
#include "EAS/EasyFunctions.h"



/*--------------------------------------------------------------------------------------------------------*/
// Body And Heart (Alliance)

bool CenarionArcScriptdust(uint32 i, Spell* pSpell) 
{
  const float pos[] = {6348.540039f, 128.124176f, 22.024008f, 4.172032f}; // x, y, z, o
  Player *p_caster = pSpell->p_caster;

  Creature *lunaclaw = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pos[0], pos[1], pos[2], 12138);
  if(lunaclaw != NULL)
  {
    if(!lunaclaw->isAlive())
      lunaclaw->Delete();
    else
      return true;
  }

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);
  lunaclaw->GetAIInterface()->SetNextTarget(p_caster);

  sEAS.MoveToPlayer(p_caster, lunaclaw);

  return true;
}



/*--------------------------------------------------------------------------------------------------------*/
// Body And Heart (Horde)

bool CenarionLunardust(uint32 i, Spell* pSpell) 
{
  const float pos[] = {-2449.117920f, -1627.319824f, 91.801430f, 0}; // x, y, z, o
  Player *p_caster = pSpell->p_caster;

  Creature *lunaclaw = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pos[0], pos[1], pos[2], 12138);
  if(lunaclaw != NULL)
  {
    if(!lunaclaw->isAlive())
      lunaclaw->Delete();
    else
      return true;
  }

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);
  lunaclaw->GetAIInterface()->SetNextTarget(p_caster);

  sEAS.MoveToPlayer(p_caster, lunaclaw);

  return true;
}

class Lunaclaw : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(Lunaclaw);

  Lunaclaw(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnDied(Unit *mKiller)
  {
    if(!mKiller->IsPlayer())
      return;

    Player *plr = static_cast<Player*>(mKiller);
    const uint32 quests[] = {6001, 6002};
    QuestLogEntry *qle;
    bool questOk = false;

    for(int i = 0; i<2; i++)
    {
      if(plr->GetQuestLogForEntry(quests[i]) != NULL)
      {
        questOk = true;
        qle = plr->GetQuestLogForEntry(quests[i]);

        break;
      }
    }

    if(!questOk)
      return;

    sEAS.SpawnCreature(plr, 12144, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1*60*1000);

    qle->SendQuestComplete();
    qle->UpdatePlayerFields();
  }
};

class ArcScriptgladeQuest : public QuestScript
{	
public:
  void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
  {
    if(!mTarget->HasSpell(19027))
      mTarget->CastSpell(mTarget, 19027, true);
  }
};



/*--------------------------------------------------------------------------------------------------------*/
// Curing the Sick

bool CurativeAnimalSalve(uint32 i, Spell* pSpell) 
{
	Player *caster = pSpell->p_caster;
	if(caster == NULL)
		return true;

	if(!pSpell->GetUnitTarget()->IsCreature())
		return true;

	Creature* target = static_cast<Creature*>(pSpell->GetUnitTarget());
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

		target->Despawn(0, 3*60*1000);

		if(entry == 12298) // Sickly Deer
			sEAS.SpawnCreature(caster, 12298, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Deer
		else // Sickly Gazelle
			sEAS.SpawnCreature(caster, 12297, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Gazelle

		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();

		return true;
	}
	return true;
}



/*--------------------------------------------------------------------------------------------------------*/
// Vanquish the Raven God

bool SwiftForm(uint32 i, Spell* pSpell)
{	
  	Player *p_caster = pSpell->p_caster;
  	if(!p_caster)
	  	return true;
	 	
	if(p_caster->GetMapMgr()->iInstanceMode != MODE_HEROIC)
	{	
		p_caster->BroadcastMessage("Heroic Mode required!");
		return true;
	}

	GameObject* obj = 0;
	obj = sEAS.SpawnGameobject(p_caster, 300154, -86.6862, 287.625, 26.4832, 0, 1);
	sEAS.GameobjectDelete(obj, 20*60*1000);

  	Creature *boss = sEAS.SpawnCreature(p_caster, 23035, -87.3546, 288.006, 26.4832, 0, 0);

  	return true;
}

// Moonglade 5921
class SCRIPT_DECL MoongladeAlliance : public QuestScript
{
public:
  void OnQuestStart(Player* pPlayer, QuestLogEntry *qLogEntry)
  {
    if(pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetSqlIdCreature(374077) == NULL)
        return;
    Creature *Mathrengyl = pPlayer->GetMapMgr()->GetSqlIdCreature(374077);
    if(pPlayer->HasSpell(18960))
        return;
    Mathrengyl->CastSpell(pPlayer,19027,false);
  }
};
// Moonglade 5922
class SCRIPT_DECL MoongladeHorde : public QuestScript
{
public:
  void OnQuestStart(Player* pPlayer, QuestLogEntry *qLogEntry)
  {
    if(pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetSqlIdCreature(380411) == NULL)
        return;
    Creature *Turak = pPlayer->GetMapMgr()->GetSqlIdCreature(380411);
    if(pPlayer->HasSpell(18960))
        return;
    Turak->CastSpell(pPlayer,19027,false);
  }
};
// Great Bear Spirit Quest 5929Alliance and 5930Horde
class SCRIPT_DECL GreatBearSpirit : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
        {
        if(!pPlayer)
            return;
        GossipMenu *Menu;
        Creature *GreatBearSpirit = (Creature*)(pObject);
        if (GreatBearSpirit == NULL)
            return;
            QuestLogEntry *en1 = pPlayer->GetQuestLogForEntry(5929);
            QuestLogEntry *en2 = pPlayer->GetQuestLogForEntry(5930);
        if(en1 && en1->GetExploredAreas(0)==0 || en2 && en2->GetExploredAreas(0)==0)
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4719, pPlayer);
            Menu->AddItem( 0, "What do you represent, spirit?", 1);
        }
        else
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4718, pPlayer);
        }
        if(AutoSend)
            Menu->SendTo(pPlayer);
        }
        void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
        {
        if(!pPlayer)
            return;
        Creature *GreatBearSpirit = (Creature*)(pObject);
        if (GreatBearSpirit == NULL)
            return;
        GossipMenu *Menu;
        QuestLogEntry *en1 = pPlayer->GetQuestLogForEntry(5929);
        QuestLogEntry *en2 = pPlayer->GetQuestLogForEntry(5930);
        switch (IntId)
        {
            case 0:
                GossipHello(pObject, pPlayer, true);
                break;
            case 1:
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4721, pPlayer);
            Menu->AddItem( 0, "I seek to understand the importance of strength of the body.", 2);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 2:
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4733, pPlayer);
            Menu->AddItem( 0, "I seek to understand the importance of strength of the heart.", 3);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 3:
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4734, pPlayer);
            Menu->AddItem( 0, "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw.", 4);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 4:
            {
            if(en1 && en1->GetExploredAreas(0)==0)
            {
                en1->SetTrigger(0);
                en1->UpdatePlayerFields();
                en1->SendQuestComplete();
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, pPlayer);
                Menu->SendTo(pPlayer);
                return;
            }
            if (en2 && en2->GetExploredAreas(0)==0)
            {
                en2->SetTrigger(0);
                en2->UpdatePlayerFields();
                en2->SendQuestComplete();
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, pPlayer);
                Menu->SendTo(pPlayer);
                return;
            }
            }break;
        }
        }
        void Destroy()
        {
            delete this;
        }
};


void SetupDruid(ScriptMgr * mgr)
{
  QuestScript *ArcScriptglade = (QuestScript*) new ArcScriptgladeQuest();
  mgr->register_quest_script(5921, ArcScriptglade);
  mgr->register_quest_script(5922, ArcScriptglade);
  mgr->register_creature_script(12138, &Lunaclaw::Create);
  mgr->register_dummy_spell(19138, &CenarionLunardust);
  mgr->register_dummy_spell(18974, &CenarionArcScriptdust);
  mgr->register_dummy_spell(19512, &CurativeAnimalSalve);
  mgr->register_dummy_spell(40098, &SwiftForm);
  mgr->register_quest_script(5921, CREATE_QUESTSCRIPT(MoongladeAlliance));
    mgr->register_quest_script(5922, CREATE_QUESTSCRIPT(MoongladeHorde));
    GossipScript * GreatBearSpiritGossip = (GossipScript*) new GreatBearSpirit();
    mgr->register_gossip_script(11956, GreatBearSpiritGossip);
}
