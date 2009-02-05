/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

/***********************************************************************
Strand of the Ancients
======================
* Ship Gameobject ID: 20808

* Setup index 34 in worldstring_tables to equal "Strand of the Ancients"

* Battlemaster NPC Text id's (Needs to be setup in database to use these NPC's!)
	npc_gossip_textid
			30586	13832	// Jojindi
			29234	13832	// Strand of the Ancients Battlemaster - Wintergrasp
			30578	13832	// Bethany Aldire - Stormwind City
			30581	13832	// Buhurda - Exodar?
			30590	13832	// Godo Cloudcleaver - Thunder Bluff
			30584	13832	// Mabrian Fardawn - Silvermoon?
			30579	13832	// Marga Bearbrawn - Ironforge
			30580	13832	// Nivara Bladedancer - Darnassus
			30583	13832	// Sarah Forthright - Undercity
			30582	13832	// Ufuda Giant-Slayer - Orgrimmar
			30587	13832	// Vinvo Goldgear - Shattrath

	creature_protos (for above creatures)
			npcflags = 1048577

	npc_text
		 13832 = You are not yet strong enough to do battle in the Strand of the Ancients. Return when you have gained more experience.
		 13834 = We cannot allow the Alliance to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?
		+13833 = We cannot allow the Horde to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?

* Location of "relic" (Courtyard of the Ancients)
	836.97, -108.9, 111.59

* Defenders starting location
	...

************************************************************************/

#include "StdAfx.h"

/*
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
*/

const float sotaAttackers[2][4] = {
	{ 2445.288f, 849.35f, 12.0f, 3.76f },
	{ 2445.288f, 849.35f, 12.0f, 3.76f },
};

const float sotaDefenders[2][4] = {
	{ 2428.288f, 837.05f, 13.0f, 3.76f },
	{ 2428.288f, 837.05f, 13.0f, 3.76f },
};

const float sotaRepop[2][4] = {
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
};

const float sotaBoats[2][4] = {
	{ 2439.4f, 845.38f, 1.0f, 3.76f },
	{ 2439.4f, 845.38f, 1.0f, 3.76f },
};

const float sotaStopBoats[2][4] = {
	{ 1623.34f, 37.0f, 1.0f, 3.9f },
	{ 1623.34f, 37.0f, 1.0f, 3.9f },
};

const float sotaStopBoatsPlayer[2][4] = {
	{ 1624.7f, 42.93f, 12.0f, 2.63f },
	{ 1624.7f, 42.93f, 12.0f, 2.63f },
};

const float sotaFlag[4] = { 836.97f, -108.9f, 111.59f, 0.0f };

StrandOfTheAncient::StrandOfTheAncient(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	m_worldStates.clear();
	m_pvpData.clear();
	m_resurrectMap.clear();

	uint32 mapId = BattlegroundManager.GetMap(BATTLEGROUND_STRAND_OF_THE_ANCIENT);
	m_boats[0] = SpawnGameObject(20808,
		mapId,
		sotaBoats[0][0], sotaBoats[0][1], sotaBoats[0][2], sotaBoats[0][3], 0, 0, 1.0f);

	m_boats[0]->Spawn(mgr);

	m_boats[1] = SpawnGameObject(20808,
		mapId,
		sotaStopBoats[0][0], sotaStopBoats[0][1], sotaStopBoats[0][2], sotaStopBoats[0][3], 0, 0, 1.0f);

	m_boats[1]->Spawn(mgr);

	/* create the buffs */
	for(i = 0; i < 6; ++i)
		SpawnBuff(i);

}

StrandOfTheAncient::~StrandOfTheAncient()
{
	//ObjectMgr::getSingleton().DeleteTransport(m_boats[0]);

	// gates are always spawned, so mapmgr will clean them up
	for (uint32 i = 0; i < 6; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && !m_buffs[i]->IsInWorld())
			delete m_buffs[i];
	}

	if (m_standFlag && !m_standFlag->IsInWorld())
		delete m_standFlag;

}


void StrandOfTheAncient::HookOnAreaTrigger(Player * plr, uint32 id)
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
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnMount(Player * plr)
{
	/* Denied! */
	plr->RemoveAllAuraType(SPELL_AURA_MOUNTED);
}

bool StrandOfTheAncient::HookHandleRepop(Player * plr)
{
	LocationVector dest;

	dest.ChangeCoords(sotaRepop[plr->GetTeam()][0], sotaRepop[plr->GetTeam()][1],
		sotaRepop[plr->GetTeam()][2], sotaRepop[plr->GetTeam()][3]);

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void StrandOfTheAncient::SpawnBuff(uint32 x)
{
    switch(x)
	{
	case 0:
		m_buffs[x] = SpawnGameObject(179871, 489, 1449.9296875f, 1470.70971679688f, 342.634552001953f, -1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.73135370016098f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.681998312473297f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 1:
		m_buffs[x] = SpawnGameObject(179899, 489, 1005.17071533203f, 1447.94567871094f, 335.903228759766f, 1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.73135370016098f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,0.681998372077942f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 2:
		m_buffs[x] = SpawnGameObject(179904, 489, 1317.50573730469f, 1550.85070800781f, 313.234375f, -0.26179963350296f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.130526319146156f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.991444826126099f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 3:
		m_buffs[x] = SpawnGameObject(179906, 489, 1110.45129394531f, 1353.65563964844f, 316.518096923828f, -0.68067866563797f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.333806991577148f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.94264143705368f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 4:
		m_buffs[x] = SpawnGameObject(179905, 489, 1320.09375f, 1378.78967285156f, 314.753234863281f, 1.18682384490967f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.559192895889282f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,0.829037606716156f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 5:
		m_buffs[x] = SpawnGameObject(179907, 489, 1139.68774414063f, 1560.28771972656f, 306.843170166016f, -2.4434609413147f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.939692616462708f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.342020124197006f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	}
}

void StrandOfTheAncient::OnCreate()
{
	sLog.outDebug("OnCreate: SOTA Battleground\n");

	/* Flag */
	m_standFlag = m_mapMgr->CreateGameObject(184141);
	m_standFlag->CreateFromProto( 184141, m_mapMgr->GetMapId(), sotaFlag[0], sotaFlag[1], sotaFlag[2], sotaFlag[3] );
	m_standFlag->SetFloatValue( GAMEOBJECT_PARENTROTATION_02, 0.662620f );
	m_standFlag->SetFloatValue( GAMEOBJECT_PARENTROTATION_03, -0.748956f );
	m_standFlag->SetFloatValue( OBJECT_FIELD_SCALE_X, 2.5f );
	m_standFlag->PushToWorld( m_mapMgr );
}

void StrandOfTheAncient::OnStart()
{
	sLog.outDebug("OnStart: SOTA Battleground\n");

	LocationVector dest;

	for(list<Player*>::iterator itr = sota_players.begin(); itr != sota_players.end(); ++itr)
	{
		Player *plr = *itr;
		dest.ChangeCoords(sotaStopBoatsPlayer[plr->GetTeam()][0], sotaStopBoatsPlayer[plr->GetTeam()][1],
			sotaStopBoatsPlayer[plr->GetTeam()][2], sotaStopBoatsPlayer[plr->GetTeam()][3]);

		plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	}

}

void StrandOfTheAncient::HookGenerateLoot(Player *plr, Object * pOCorpse)
{
	sLog.outDebug("*** StrandOfTheAncient::HookGenerateLoot");
}

void StrandOfTheAncient::HookOnUnitKill(Player * plr, Unit * pVictim)
{
	sLog.outDebug("*** StrandOfTheAncient::HookOnUnitKill");
}

void StrandOfTheAncient::SetIsWeekend(bool isweekend) 
{
	sLog.outDebug("*** StrandOfTheAncient::SetIsWeekend");
	m_isWeekend = isweekend;
}

// end game, attackers captured flag
bool StrandOfTheAncient::HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell)
{
	sLog.outDebug("*** StrandOfTheAncient::HookSlowLockOpen");
	PlaySoundToAll( 8212 );
	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
	sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

	/* increment the score world state */
	//SetWorldState(pPlayer->GetTeam() ? WSG_CURRENT_HORDE_SCORE : WSG_CURRENT_ALLIANCE_SCORE, m_scores[pPlayer->GetTeam()]);

	UpdatePvPData();

	return true;
}

// Probably safe not using these
void StrandOfTheAncient::HookFlagStand(Player * plr, GameObject * obj)
{
}

void StrandOfTheAncient::HookOnFlagDrop(Player * plr)
{
}

void StrandOfTheAncient::HookFlagDrop(Player * plr, GameObject * obj)
{
}
void StrandOfTheAncient::HookOnShadowSight() 
{
}
