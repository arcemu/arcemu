/*
 * LunarEMU MMORPG Server
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 *		This class (as-is) keeps trying to respawn new instance over and over
 *		it is causing a new instance to be created, every x seconds.
 *
 */

/***********************************************************************
Strand of the Ancients
======================
* Ship Gameobject ID: 20808

* Change: Currently porting to 1519.53, 1482.868, >328

* Attackers starting location (Boat location)
  Boat 1: 2439.4, 845.38, 20, o=3.76
	  Go in a straight line you'll go right to the docks
	  ??Ending location: 1623.34, 37, ~10, o=4.143
		With two boats, might need to switch sides
		of the dock

* Location of "relic" (Courtyard of the Ancients)
	836.97, -108.9, 111.59

* Defenders starting location
	...

************************************************************************/

#include "StdAfx.h"

// Strand of the Ancients Battlemaster
class StrandOfTheAncientsBattlemaster : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 Team = plr->GetTeam();
		if(Team > 1) Team = 1;

		// Check if the player can be entered into the bg or not.
		if(plr->getLevel() < 71) //Levels 71-80 else don't let them into the bg menu.
		{
			// Send "you cannot enter" message.
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13832, plr);
		}
		else
		{
			uint32 FactMessages[2] = { 13833, 13834 }; // NEED TO BE UPDATED TO SOTA

			// Ask to enter battleground
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
			Menu->AddItem( 0, "I would like to enter the battleground.", BATTLEGROUND_STRAND_OF_THE_ANCIENT);
		}

		if (AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		// Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		plr->GetSession()->SendBattlegroundList(((Creature*)pObject), BATTLEGROUND_STRAND_OF_THE_ANCIENT);
	}

	void Destroy()
	{
		delete this;
	}
};

void CBattlegroundManager::SetupStrandOfTheAncientBattleMasters()
{
	ScriptMgr * mgr = &sScriptMgr;

	// Battlemaster Interaction Handler
	GossipScript * sota = (GossipScript*) new StrandOfTheAncientsBattlemaster;

	mgr->register_gossip_script(30586, sota);		// Jojindi
	mgr->register_gossip_script(29234, sota);		// Strand of the Ancients Battlemaster - Wintergrasp
	mgr->register_gossip_script(30578, sota);		// Bethany Aldire - Stormwind City
	mgr->register_gossip_script(30581, sota);		// Buhurda - Exodar?
	mgr->register_gossip_script(30590, sota);		// Godo Cloudcleaver - Thunder Bluff
	mgr->register_gossip_script(30584, sota);		// Mabrian Fardawn - Silvermoon?
	mgr->register_gossip_script(30579, sota);		// Marga Bearbrawn - Ironforge
	mgr->register_gossip_script(30580, sota);		// Nivara Bladedancer - Darnassus
	mgr->register_gossip_script(30583, sota);		// Sarah Forthright - Undercity
	mgr->register_gossip_script(30582, sota);		// Ufuda Giant-Slayer - Orgrimmar
	mgr->register_gossip_script(30587, sota);		// Vinvo Goldgear - Shattrath

}

void StrandOfTheAncient::HookOnAreaTrigger(Player * plr, uint32 id)
{

}

void StrandOfTheAncient::HookFlagStand(Player * plr, GameObject * obj)
{

}

void StrandOfTheAncient::HookOnFlagDrop(Player * plr)
{

}

void StrandOfTheAncient::HookFlagDrop(Player * plr, GameObject * obj)
{

}

void StrandOfTheAncient::HookOnPlayerKill(Player * plr, Player * pVictim)
{
	plr->m_bgScore.KillingBlows++;
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnHK(Player * plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void StrandOfTheAncient::OnAddPlayer(Player * plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
	//plr->m_CurrentTransporter = m_boat[0];
		sota_players.push_back(plr);
}

void StrandOfTheAncient::OnRemovePlayer(Player * plr)
{
	/* drop the flag if we have it */
	if(m_flagHolders[plr->GetTeam()] == plr->GetLowGUID())
		HookOnFlagDrop(plr);

	plr->RemoveAura(BG_PREPARATION);

	sota_players.remove(plr);
}

LocationVector StrandOfTheAncient::GetStartingCoords(uint32 team)
{
	return LocationVector(sotaAttackers[team][0], sotaAttackers[team][1],
		sotaAttackers[team][2], sotaAttackers[team][3]);
}

void StrandOfTheAncient::HookOnPlayerDeath(Player * plr)
{
	plr->m_bgScore.Deaths++;

	/* do we have the flag? */
	if(m_flagHolders[plr->GetTeam()] == plr->GetLowGUID())
		HookOnFlagDrop(plr);

	UpdatePvPData();
}

void StrandOfTheAncient::HookOnMount(Player * plr)
{
	/* do we have the flag? */
	if(m_flagHolders[plr->GetTeam()] == plr->GetLowGUID())
		HookOnFlagDrop(plr);
}

bool StrandOfTheAncient::HookHandleRepop(Player * plr)
{
	LocationVector dest;

	dest.ChangeCoords(sotaRepop[plr->GetTeam()][0], sotaRepop[plr->GetTeam()][1],
		sotaRepop[plr->GetTeam()][2], sotaRepop[plr->GetTeam()][3]);

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void StrandOfTheAncient::OnCreate()
{

}

void StrandOfTheAncient::OnStart()
{
	printf("OnStart: SOTA Battleground\n");

	LocationVector dest;

	for(list<Player*>::iterator itr = sota_players.begin(); itr != sota_players.end(); ++itr)
	{
		Player *plr = itr._Ptr->_Myval;
		dest.ChangeCoords(sotaStopBoatsPlayer[plr->GetTeam()][0], sotaStopBoatsPlayer[plr->GetTeam()][1],
			sotaStopBoatsPlayer[plr->GetTeam()][2], sotaStopBoatsPlayer[plr->GetTeam()][3]);

		plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	}

}

void StrandOfTheAncient::HookOnShadowSight() 
{

}

void StrandOfTheAncient::HookGenerateLoot(Player *plr, Object * pOCorpse)
{

}

void StrandOfTheAncient::HookOnUnitKill(Player * plr, Unit * pVictim)
{

}

void StrandOfTheAncient::SetIsWeekend(bool isweekend) 
{
	m_isWeekend = isweekend;
}
