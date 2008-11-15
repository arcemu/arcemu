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

class PortalEffectSunwellIsle : public GameObjectAIScript
  {
  public:
          PortalEffectSunwellIsle(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
          void OnActivate(Player * pPlayer)
          {
                if(pPlayer->getLevel() >= 70 && pPlayer->HasSpell(44934)) // optional you can check for quest 11513
                {
                  pPlayer->SafeTeleport(530, 530, 13001.9f, -6907.46f, 9.56139f, 0.616532f);
                }
                else if(pPlayer->getLevel() <= 69)
                {
                pPlayer->BroadcastMessage("You must be level 70 to use this!");
                }
                else if(!pPlayer->HasSpell(44934))
                {
                pPlayer->BroadcastMessage("You must complete the quest first!");
                }
          }
          static GameObjectAIScript *Create(GameObject * GO) { return new PortalEffectSunwellIsle(GO); }
  };
GameObjectAIScript * create_go(GameObject * GO) { return new PortalEffectSunwellIsle(GO); }
void SetupQDGoHandlers(ScriptMgr * mgr)
{

      mgr->register_gameobject_script(187056, &PortalEffectSunwellIsle::Create);
   
}
