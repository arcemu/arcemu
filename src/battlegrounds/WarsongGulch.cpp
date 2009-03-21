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

#include "StdAfx.h"
#include "WarsongGulch.h"

//										   <10 <20 <30 <40 <50 <60 <70  70
static int flagHonorTable[8]			= {  0,  5,  8, 14, 23, 38, 40, 40 };
static int winHonorTable[8]				= {  0,  2,  4,  7, 11, 19, 20, 20 };
static int extraCompleteHonorTable[8]	= {  0,  7, 12, 20, 34, 57, 59, 59 }; // extras only for weekends
static int extraWinHonorTable[8]		= {  0,  5,  8, 14, 23, 38, 40, 40 };

WarsongGulch::WarsongGulch(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	//m_worldStates.clear();
	m_pvpData.clear();
	m_resurrectMap.clear();

	m_flagHolders[0] = m_flagHolders[1] = 0;
	m_lgroup = lgroup;
	
	/* create the buffs */
	for(i = 0; i < 6; ++i)
		SpawnBuff(i);

	/* take note: these are swapped around for performance bonus */
	// warsong flag - horde base
	m_homeFlags[0] = SpawnGameObject(179831, 489, 915.367f, 1433.78f, 346.089f, 3.17301f, 0, 210, 2.5f);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1, 1, 24);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);

	// silverwing flag - alliance base
	m_homeFlags[1] = SpawnGameObject(179830, 489, 1540.29f, 1481.34f, 352.64f, 3.17301f, 0,1314, 2.5f);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1, 1, 24);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);

	// dropped flags
	m_dropFlags[1] = m_mapMgr->CreateGameObject(179786);
	if(!m_dropFlags[1]->CreateFromProto(179785, 489, 0, 0, 0, 0))
		Log.Warning("WarsongGulch", "Could not create dropped flag 1");

	m_dropFlags[0] = m_mapMgr->CreateGameObject(179786);
	if(!m_dropFlags[0]->CreateFromProto(179786, 489, 0, 0, 0, 0))
		Log.Warning("WarsongGulch", "Could not create dropped flag 0");

	for(i = 0; i < 2; ++i)
	{
		m_dropFlags[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
		m_dropFlags[i]->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.5f);
	}

	m_scores[0] = m_scores[1] = 0;

}

WarsongGulch::~WarsongGulch()
{
	// gates are always spawned, so mapmgr will clean them up
	for(uint32 i = 0; i < 6; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if (m_buffs[i] && m_buffs[i]->IsInWorld() == false)
			delete m_buffs[i];
	}

	for(uint32 i = 0; i < 2; ++i)
	{
		if (m_dropFlags[i] && m_dropFlags[i]->IsInWorld() == false)
			delete m_dropFlags[i];

		if (m_homeFlags[i] && m_homeFlags[i]->IsInWorld() == false)
			delete m_homeFlags[i];
	}

	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		if((*itr) != NULL)
		{
			(*itr)->m_battleground = NULL;
			if( !(*itr)->IsInWorld() )
				delete (*itr);
		}
	}

	m_resurrectMap.clear();
	//m_worldStates.clear();
}

void WarsongGulch::EventReturnFlags()
{
	for (uint32 x = 0; x < 2; x++)
	{
		if (m_homeFlags[x] != NULL)
			m_homeFlags[x]->PushToWorld(m_mapMgr);
	}
	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The flags are now placed at their bases.");
}

void WarsongGulch::ReturnFlag(uint32 team)
{
	if (m_dropFlags[team]->IsInWorld())
		m_dropFlags[team]->RemoveFromWorld(false);
	
	if( !m_homeFlags[team]->IsInWorld() )
		m_homeFlags[team]->PushToWorld(m_mapMgr);
	
	if( team )
		SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, 0, "The Alliance flag was returned to its base!" );
	else
		SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, 0, "The Horde flag was returned to its base!" );
}

void WarsongGulch::OnAddPlayer(Player * plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}

void WarsongGulch::OnRemovePlayer(Player * plr)
{
	/* drop the flag if we have it */
	if(plr->m_bgHasFlag)
		sHookInterface.OnMount(plr);

	plr->RemoveAura(BG_PREPARATION);
}

LocationVector WarsongGulch::GetStartingCoords(uint32 Team)
{
	if(Team)		// Horde
		return LocationVector(933.989685f, 1430.735840f, 345.537140f, 3.141593f);
	else			// Alliance
		return LocationVector(1519.530273f, 1481.868408f, 352.023743f, 3.141593f);
}

void WarsongGulch::SpawnBuff(uint32 x)
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

void WarsongGulch::OnCreate()
{
	/* add the buffs to the world */
	for(int i = 0; i < 6; ++i)
	{
		if(!m_buffs[i]->IsInWorld())
			m_buffs[i]->PushToWorld(m_mapMgr);
	}

	// Alliance Gates
	GameObject *gate = SpawnGameObject(179921, 489, 1471.554688f, 1458.778076f, 362.633240f, 0, 33, 114, 2.33271f);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179919, 489, 1492.477783f, 1457.912354f, 342.968933f, 0, 33, 114, 2.68149f);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179918, 489, 1503.335327f, 1493.465820f, 352.188843f, 0, 33, 114, 2.26f);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// Horde Gates
	gate = SpawnGameObject(179916, 489, 949.1663208f, 1423.7717285f, 345.6241455f, -0.5756807f, 32, 114, 0.900901f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION,-0.0167336f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_01,-0.004956f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,-0.283972f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,0.9586736f);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179917, 489, 953.0507202f, 1459.8424072f, 340.6525573f, -1.9966197f, 32, 114, 0.854700f);   
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION,-0.1971825f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_01,0.1575096f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,-0.8239487f);
	gate->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,0.5073640f);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	/* set world states */
	SetWorldState(0x8D8, 0);
	SetWorldState(0x8D7, 0);
	SetWorldState(0x8D6, 0);
	SetWorldState(0x8D5, 0);
	SetWorldState(0x8D4, 0);
	SetWorldState(0x8D3, 0);
	SetWorldState(0x60B, 0);
	SetWorldState(0x60A, 0);
	SetWorldState(0x609, 0);
	SetWorldState(WSG_ALLIANCE_FLAG_CAPTURED, 1);
	SetWorldState(WSG_HORDE_FLAG_CAPTURED, 1);
	SetWorldState(WSG_MAX_SCORE, 3);
	SetWorldState(WSG_CURRENT_ALLIANCE_SCORE, 0);
	SetWorldState(WSG_CURRENT_HORDE_SCORE, 0);

	/* spawn spirit guides */
	AddSpiritGuide(SpawnSpiritGuide(1423.218872f, 1554.663574f, 342.833801f, 3.124139f, 0));
	AddSpiritGuide(SpawnSpiritGuide(1032.644775f, 1388.316040f, 340.559937f, 0.043200f, 1));
}

void WarsongGulch::OnStart()
{
	for(uint32 i = 0; i < 2; ++i) {
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) {
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	/* open the gates */
	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
		(*itr)->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		(*itr)->Despawn(5000, 0);
	}

	/* add the flags to the world */
	for(int i = 0; i < 2; ++i)
	{
		if( !m_homeFlags[i]->IsInWorld() )
			m_homeFlags[i]->PushToWorld(m_mapMgr);
	}

	SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The flags are now placed at their bases." );

	/* correct? - burlex */
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	m_started = true;
}

void WarsongGulch::SetIsWeekend(bool isweekend) 
{
	m_isWeekend = isweekend;
}

void WarsongGulch::ReturnTeamFlag(Player *plr, GameObject * obj)
{
	GameObject * droppedFlag = m_dropFlags[plr->GetTeam()];
	GameObject * homeFlag = m_homeFlags[plr->GetTeam()];

	uint32 x = plr->GetTeam() ? 0 : 1;
	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam() ? 0 : 1);

	if (droppedFlag->IsInWorld())
		droppedFlag->RemoveFromWorld(false);

	if (homeFlag->IsInWorld() == false)
		homeFlag->PushToWorld(obj->GetMapMgr());

	plr->m_bgScore.Misc2++;
	UpdatePvPData();

	if( plr->GetTeam() == 1 )
		SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The Horde flag was returned to its base by %s!", plr->GetName() );
	else
		SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "The Alliance flag was returned to its base by %s!", plr->GetName() );

	SetWorldState(plr->GetTeam() ? WSG_ALLIANCE_FLAG_CAPTURED : WSG_HORDE_FLAG_CAPTURED, 1);
	PlaySoundToAll(plr->GetTeam() ? SOUND_HORDE_RETURNED : SOUND_ALLIANCE_RETURNED);
}

void WarsongGulch::ForceFlagDrop(Player *plr)
{
	ASSERT(plr != NULL);
	if(!plr->m_bgHasFlag || m_dropFlags[plr->GetTeam()]->IsInWorld())
		return;

	m_dropFlags[plr->GetTeam()]->SetPosition(plr->GetPosition());
	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_X, plr->GetPositionX());
	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_Y, plr->GetPositionY());
	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_Z, plr->GetPositionZ());
	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_FACING, plr->GetOrientation());
	m_dropFlags[plr->GetTeam()]->PushToWorld(m_mapMgr);

	m_flagHolders[plr->GetTeam()] = 0;
	plr->m_bgHasFlag = false;
	plr->RemoveAura(23333 + (plr->GetTeam() * 2));

	SetWorldState(plr->GetTeam() ? WSG_ALLIANCE_FLAG_CAPTURED : WSG_HORDE_FLAG_CAPTURED, 1);

	sEventMgr.AddEvent( this, &WarsongGulch::ReturnFlag, plr->GetTeam(), EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam(), 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

	if( plr->GetTeam() == 1 )
		SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "The Alliance flag was dropped by %s!", plr->GetName() );
	else
		SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The Horde flag was dropped by %s!", plr->GetName() );

}

void WarsongGulch::CapturedFlag(Player *plr)
{
	if(m_flagHolders[plr->GetTeam() ? 0 : 1] != 0 || m_dropFlags[plr->GetTeam() ? 0 : 1]->IsInWorld())
	{
		/* can't cap while flag dropped */
		return;
	}
	float distance = (plr->GetTeam() == 0)? plr->CalcDistance(1540.29f, 1481.34f, 352.64f) :plr->CalcDistance(915.367f, 1433.78f, 346.089f);
	if (distance > 50.0f)
	{
		//50 yards from the spawn, gtfo hacker.
		sCheatLog.writefromsession(plr->GetSession(),"Tried to capture the flag in WSG while being more then 50 yards away. (%f yards)",plr->CalcDistance(915.367f, 1433.78f, 346.089f));
		plr->GetSession()->Disconnect();
		return;
	}

	/* remove the bool from the player so the flag doesn't drop */
	m_flagHolders[plr->GetTeam()] = 0;
	plr->m_bgHasFlag = 0;
		
	/* remove flag aura from player */
	plr->RemoveAura(23333+(plr->GetTeam() * 2));

	/* capture flag points */
	plr->m_bgScore.Misc1++;

	PlaySoundToAll( plr->GetTeam() ? SOUND_HORDE_SCORES : SOUND_ALLIANCE_SCORES );

	if( plr->GetTeam() == 1 )
		SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "%s captured the Alliance flag!", plr->GetName() );
	else
		SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "%s captured the Horde flag!", plr->GetName() );

	SetWorldState( plr->GetTeam() ? WSG_ALLIANCE_FLAG_CAPTURED : WSG_HORDE_FLAG_CAPTURED, 1 );

	// Remove the Other Flag
	if (m_homeFlags[plr->GetTeam() ? 0 : 1]->IsInWorld())
		m_homeFlags[plr->GetTeam() ? 0 : 1]->RemoveFromWorld(false);

	// Add the Event to respawn the Flags
	sEventMgr.AddEvent(this, &WarsongGulch::EventReturnFlags, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG, 20000, 1, 0);

	/* give each player on that team a bonus according to flagHonorTable */
	for(set<Player*>::iterator itr = m_players[plr->GetTeam()].begin(); itr != m_players[plr->GetTeam()].end(); ++itr)
	{
		(*itr)->m_bgScore.BonusHonor += flagHonorTable[m_lgroup];
		HonorHandler::AddHonorPointsToPlayer((*itr), flagHonorTable[m_lgroup]);
	}

	m_scores[plr->GetTeam()]++;
	if(m_scores[plr->GetTeam()] == 3)
	{
		/* victory! */
		m_ended = true;
		m_winningteam = plr->GetTeam() ? 1 : 0;
		m_nextPvPUpdateTime = 0;

		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
		sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

		m_mainLock.Acquire();
		/* add the marks of honor to all players */
		SpellEntry * winner_spell = dbcSpell.LookupEntry(24951);
		SpellEntry * loser_spell = dbcSpell.LookupEntry(24950);
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				(*itr)->Root();
				if(i == m_winningteam)
				{
					int honor = winHonorTable[m_lgroup];
					if (m_isWeekend)
					{
						honor+= extraWinHonorTable[m_lgroup];
						honor+= extraCompleteHonorTable[m_lgroup];
					}
					(*itr)->m_bgScore.BonusHonor += honor;
					HonorHandler::AddHonorPointsToPlayer((*itr), honor);
					(*itr)->CastSpell((*itr), winner_spell, true);
				}
				else
				{
					if (m_isWeekend)
					{
						int honor = extraCompleteHonorTable[m_lgroup];
						(*itr)->m_bgScore.BonusHonor += honor;
						HonorHandler::AddHonorPointsToPlayer((*itr), honor);
					}
					(*itr)->CastSpell((*itr), loser_spell, true);
				}
			}
		}
		m_mainLock.Release();
	}

	/* increment the score world state */
	SetWorldState(plr->GetTeam() ? WSG_CURRENT_HORDE_SCORE : WSG_CURRENT_ALLIANCE_SCORE, m_scores[plr->GetTeam()]);
}

void WarsongGulch::GivePlayerFlag(Player * plr, bool fromDroppedFlag)
{
	if (plr->m_bgHasFlag && plr->GetTeam() == 1)
		SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "%s picked-up the Alliance flag!", plr->GetName() );
	else if (plr->m_bgHasFlag)
		SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "%s picked-up the Horde flag!", plr->GetName() );

	m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
	plr->m_bgHasFlag = true;
		
	if (fromDroppedFlag)
	{
	   /* This is *really* strange. Even though the A9 create sent to the client is exactly the same as the first one, if
		* you spawn and despawn it, then spawn it again it will not show. So we'll assign it a new guid, hopefully that
		* will work.
	    * - Burlex
	    */
		m_dropFlags[plr->GetTeam()]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());

	}
	else
	{
		if(m_homeFlags[plr->GetTeam()]->IsInWorld())
			m_homeFlags[plr->GetTeam()]->RemoveFromWorld(false);

		PlaySoundToAll(plr->GetTeam() ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE);
		SetWorldState(plr->GetTeam() ? WSG_ALLIANCE_FLAG_CAPTURED : WSG_HORDE_FLAG_CAPTURED, 2);

	}

	// Cast has-flag aura
	SpellEntry * pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell * sp = new Spell(plr, pSp, true, 0);
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);
	SetWorldState(plr->GetTeam() ? WSG_ALLIANCE_FLAG_CAPTURED : WSG_HORDE_FLAG_CAPTURED, 2);
}


// Warsong Gulch battlemaster
class SCRIPT_DECL WarsongGulchBattlemaster : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 Team = plr->GetTeam();
		if(Team > 1) Team = 1;
		
		// Check if the player can be entered into the bg or not.
		if(plr->getLevel() < 10)
		{
			uint32 FactMessages[2] = { 7599, 7688 };

			// Send "you cannot enter" message.
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
		}
		else
		{
			uint32 FactMessages[2] = { 7689, 7705 }; // need to find the second one

			// Send "you cannot enter" message.
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
			Menu->AddItem( 0, "I would like to enter the battleground.", 1);
		}
		
		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		// Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		plr->GetSession()->SendBattlegroundList(((Creature*)pObject), 2);  // WSG = 2
	}

	void Destroy()
	{
		delete this;
	}
};

bool WarsongGulch::IsInWarsongGulch(Player * plr)
{
	if (plr == NULL || !plr->m_bg)
		return false;
	if (plr->m_bg->GetType() != BATTLEGROUND_WARSONG_GULCH)
		return false;
	return true;
}

bool WarsongGulch::OnRepopRequest(Player * plr)
{
	if (!IsInWarsongGulch(plr))
		return false;

    LocationVector dest;
	if(plr->GetTeam())
		dest.ChangeCoords(1032.644775f, 1388.316040f, 340.559937f, 0.043200f);
	else
		dest.ChangeCoords(1423.218872f, 1554.663574f, 342.833801f, 3.124139f);
	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void WarsongGulch::OnPlayerKill(Player * plr, Player * pVictim)
{
	plr->m_bgScore.KillingBlows++;
	static_cast<WarsongGulch *>(plr->m_bg)->UpdatePvPData();
}

void WarsongGulch::OnHonorKill(Player * plr)
{
	if (!IsInWarsongGulch(plr))
		return;

	plr->m_bgScore.HonorableKills++;
	static_cast<WarsongGulch *>(plr->m_bg)->UpdatePvPData();
}

void WarsongGulch::OnPlayerDeath(Player * plr)
{
	if (!IsInWarsongGulch(plr))
		return;

	plr->m_bgScore.Deaths++;

	/* do we have the flag? */
	if(plr->m_bgHasFlag)
	    plr->RemoveAura( 23333 + (plr->GetTeam() * 2) );

	static_cast<WarsongGulch *>(plr->m_bg)->UpdatePvPData();
}

/* Pickup a dropped flag */
void WarsongGulch::OnFlagDropPickup(Player * plr, GameObject * obj)
{
	if (!IsInWarsongGulch(plr))
		return;

	WarsongGulch *bg = static_cast<WarsongGulch *>(plr->m_bg);
	GameObject * droppedFlag = bg->m_dropFlags[plr->GetTeam()];
	GameObject * homeFlag = bg->m_homeFlags[plr->GetTeam()];

	/* picking up a dropped flag */
	if (droppedFlag != obj)
	{
		/* are we returning it? */
		if( (obj->GetEntry() == 179785 && plr->GetTeam() == 0) ||
			(obj->GetEntry() == 179786 && plr->GetTeam() == 1) )
		{
			bg->ReturnTeamFlag(plr, obj);
		}
		return;
	}

	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	if (plr->GetTeam() == 0)
		sEventMgr.RemoveEvents(bg, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG);
	else
		sEventMgr.RemoveEvents(bg, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + 1);

	if (droppedFlag->IsInWorld())
	{
		droppedFlag->RemoveFromWorld(false);
	}

	bg->GivePlayerFlag(plr, true); // true == dropped flag

}

bool WarsongGulch::OnSpellCast(Player * p_caster, uint32 spellId)
{
	// if we're picking up the flag remove the buffs
	if ((spellId == 23333) || (spellId == 23335) || (spellId == 34976))
	{
		p_caster->RemoveStealth();
		p_caster->RemoveInvisibility();
		p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
		p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_PROTECTION);
		p_caster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
	}

	// also includes check for trying to cast stealth/etc while you have the flag
	if (BattlegroundManager.IsSpellRestricted(spellId))
	{
		if (p_caster->GetTeam() == 0)
			p_caster->RemoveAura(23333);	// ally player drop horde flag if they have it
		else
			p_caster->RemoveAura(23335); 	// horde player drop ally flag if they have it
	}

	return false;
}

/* Picking up the flag off the stand */
void WarsongGulch::OnFlagStandPickup(Player * plr, GameObject * obj)
{
	if (!IsInWarsongGulch(plr))
		return;

	WarsongGulch *bg = static_cast<WarsongGulch *>(plr->m_bg);

#ifdef ANTI_CHEAT
	if (!bg->m_started)
	{
		Anticheat_Log->writefromsession(plr->GetSession(), "%s tryed to hook the flag in warsong gluch before battleground (ID %u) started.", plr->GetName(), bg->GetId());
		bg->SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "%s will be removed from the game for cheating.", plr->GetName());
		// Remove player from battleground.
		bg->RemovePlayer(plr, false);
		// Kick player from server.
		plr->Kick(6000);
		return;
	}
#endif

	// Is this check really needed?
	//if(m_flagHolders[plr->GetTeam()] || m_homeFlags[plr->GetTeam()] != obj)
	//{
		// cheater!
	//	return;
	//}

	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	SpellEntry * pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell * sp = new Spell(plr, pSp, true, 0);
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);

	/* Give player flag from stand */
	bg->GivePlayerFlag(plr, false);
}

void WarsongGulch::OnFlagDrop(Player * plr)
{
	if (!IsInWarsongGulch(plr))
		return;

	/* drop the flag! */
	WarsongGulch *bg = static_cast<WarsongGulch *>(plr->m_bg);
	bg->ForceFlagDrop(plr);

}

void WarsongGulch::OnMount(Player * plr)
{
	if (!IsInWarsongGulch(plr))
		return;

	WarsongGulch *bg = static_cast<WarsongGulch *>(plr->m_bg);
	if (bg->IsFlagHolder(plr))
		sHookInterface.OnFlagDrop(plr);
}


void WarsongGulch::OnAreaTrigger(Player * plr, uint32 id)
{
	if (!IsInWarsongGulch(plr))
		return;

	int32 buffslot = -1;
	switch(id)
	{
	case 3686:	  // Speed
		buffslot = 0;
		break;
	case 3687:	  // Speed (Horde)
		buffslot = 1;
		break;
	case 3706:	  // Restoration
		buffslot = 2;
		break;
	case 3708:	  // Restoration (Horde)
		buffslot = 3;
		break;
	case 3707:	  // Berserking
		buffslot = 4;
		break;
	case 3709:	  // Berserking (Horde)
		buffslot = 5;
		break;
	}

	WarsongGulch * bg = static_cast<WarsongGulch *>(plr->m_bg);
	if(buffslot >= 0)
	{
		GameObject * buff = bg->m_buffs[buffslot];
		if(buff != 0 && buff->IsInWorld())
		{
			/* apply the buff */
			SpellEntry * sp = dbcSpell.LookupEntry(buff->GetInfo()->sound3);
			Spell * s = new Spell(plr, sp, true, 0);
			//s->Init(plr, sp, true, 0);
			SpellCastTargets targets(plr->GetGUID());
			s->prepare(&targets);

			/* despawn the gameobject (not delete!) */
			buff->Despawn(0, BUFF_RESPAWN_TIME);
		}
		return;
	}

	if (((id == 3646 && plr->GetTeam() == 0) || (id == 3647 && plr->GetTeam() == 1)) && bg->IsFlagHolder(plr))
	{
		bg->CapturedFlag(plr);
		bg->UpdatePvPData();
	}
}



void SetupWarsongGulch(ScriptMgr * mgr)
{
	uint32 nameId = 39;
	// Register the battleground
	BattlegroundManager.RegisterBattleground(BATTLEGROUND_WARSONG_GULCH, MAP_WARSONG_GULCH,
		nameId, 10, 10, 10, &WarsongGulch::Create);

	// Battlemaster Interaction Handler
	GossipScript * wsg = (GossipScript*) new WarsongGulchBattlemaster;

	mgr->register_gossip_script(19910, wsg);		// Gargok
	mgr->register_gossip_script(15105, wsg);		// Warsong Emissary
	mgr->register_gossip_script(20118, wsg);		// Jihi
	mgr->register_gossip_script(16696, wsg);		// Krukk
	mgr->register_gossip_script(2804, wsg);			// Kurden Bloodclaw
	mgr->register_gossip_script(20272, wsg);		// Lylandor
	mgr->register_gossip_script(20269, wsg);		// Montok Redhands
	mgr->register_gossip_script(19908, wsg);		// Su'ura Swiftarrow
	mgr->register_gossip_script(15102, wsg);		// Silverwing Emissary
	mgr->register_gossip_script(14981, wsg);		// Elfarran
	mgr->register_gossip_script(14982, wsg);		// Lylandris
	mgr->register_gossip_script(2302, wsg);			// Aethalas
	mgr->register_gossip_script(10360, wsg);		// Kergul Bloodaxe
	mgr->register_gossip_script(3890, wsg);			// Brakgul Deathbringer

	// Register the event hooks
	mgr->register_hook(SERVER_HOOK_EVENT_ON_PLAYER_DEATH, &WarsongGulch::OnPlayerDeath);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_REPOP, &WarsongGulch::OnRepopRequest);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_AREA_TRIGGER, &WarsongGulch::OnAreaTrigger);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_FLAG_DROP_PICKUP, &WarsongGulch::OnFlagDropPickup);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_FLAG_STAND_PICKUP, &WarsongGulch::OnFlagStandPickup);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_FLAG_DROP, &WarsongGulch::OnFlagDrop);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_MOUNT, &WarsongGulch::OnMount);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_HONOR_KILL, &WarsongGulch::OnHonorKill);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_PLAYER_KILL, &WarsongGulch::OnPlayerKill);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_SPELL_CAST, &WarsongGulch::OnSpellCast);

}