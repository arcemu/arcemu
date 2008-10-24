/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
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
// Crisis at the Sunwell

class ScryingOrb : public GameObjectAIScript
{
public:
	ScryingOrb(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new ScryingOrb(GO); }

	void OnActivate(Player * pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(11490);
  		if(qle)
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			
			GameObject* Orb = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 187578);
			if (Orb)
			{
				Orb->SetUInt32Value(GAMEOBJECT_STATE, 0);
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
  Player *caster = pSpell->p_caster;
  if(caster == NULL)
    return true;

  Creature *target = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(target == NULL)
    return true;

  // Erratic Sentry: 24972
  if(target->GetEntry() != 24972)
    return true;

  if(!target->isAlive())
    return true;

  QuestLogEntry *qle = caster->GetQuestLogForEntry(11524);
  if(qle == NULL)
    return true;

  if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
    return true;

  qle->SetMobCount(0, qle->GetMobCount(0)+1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  target->Despawn(500, 2*60*1000);

  return true;
}

void SetupIsleOfQuelDanas(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(187578, &ScryingOrb::Create);
	mgr->register_dummy_spell(44997, &ConvertingSentry);
}
