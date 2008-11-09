/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
void WorldSession::HandleAreaTriggerOpcode(WorldPacket & recv_data)
{
	 if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 id ;
	recv_data >> id;
	_HandleAreaTriggerOpcode(id);
}

enum AreaTriggerFailures
{
	AREA_TRIGGER_FAILURE_OK				= 0,
	AREA_TRIGGER_FAILURE_UNAVAILABLE	= 1,
	AREA_TRIGGER_FAILURE_NO_BC			= 2,
	AREA_TRIGGER_FAILURE_NO_HEROIC		= 3,
	AREA_TRIGGER_FAILURE_NO_RAID		= 4,
	AREA_TRIGGER_FAILURE_NO_ATTUNE_Q	= 5,
	AREA_TRIGGER_FAILURE_NO_ATTUNE_I	= 6,
	AREA_TRIGGER_FAILURE_LEVEL			= 7,
	AREA_TRIGGER_FAILURE_NO_GROUP		= 8,
	AREA_TRIGGER_FAILURE_NO_KEY         = 9,
	AREA_TRIGGER_FAILURE_LEVEL_HEROIC	= 9,
	AREA_TRIGGER_FAILURE_NO_CHECK		= 10,
};

uint32 AreaTriggerFailureMessages[] = {
	34,
	26,
	27,
	28,
	29,
	30, //TODO: Replace attunment with real itemname
	30, //TODO: Replace attunment with real itemname
	31,
	32,
	30, //TODO: Replace attunment with real itemname
	33,
};

uint32 CheckTriggerPrerequsites(AreaTrigger * pAreaTrigger, WorldSession * pSession, Player * pPlayer, MapInfo * pMapInfo)
{
	if(!pMapInfo || !pMapInfo->HasFlag(WMI_INSTANCE_ENABLED))
		return AREA_TRIGGER_FAILURE_UNAVAILABLE;

	if(!pSession->HasFlag(ACCOUNT_FLAG_XPACK_01) && pMapInfo->HasFlag(WMI_INSTANCE_XPACK_01))
		return AREA_TRIGGER_FAILURE_NO_BC;

	// These can be overridden by cheats/GM
	if(pPlayer->TriggerpassCheat)
		return AREA_TRIGGER_FAILURE_OK;

	if(pAreaTrigger->required_level && pPlayer->getLevel() < pAreaTrigger->required_level)
		return AREA_TRIGGER_FAILURE_LEVEL;

	if(pPlayer->iInstanceType >= MODE_HEROIC && pMapInfo->type != INSTANCE_MULTIMODE && pMapInfo->type != INSTANCE_NULL)
		return AREA_TRIGGER_FAILURE_NO_HEROIC;

	if(pMapInfo->type == INSTANCE_RAID && (!pPlayer->GetGroup() || (pPlayer->GetGroup() && pPlayer->GetGroup()->GetGroupType() != GROUP_TYPE_RAID)))
		return AREA_TRIGGER_FAILURE_NO_RAID;

	if((pMapInfo->type == INSTANCE_MULTIMODE && pPlayer->iInstanceType >= MODE_HEROIC) && !pPlayer->GetGroup())
		return AREA_TRIGGER_FAILURE_NO_GROUP;

	if(pMapInfo && pMapInfo->required_quest && !pPlayer->HasFinishedQuest(pMapInfo->required_quest))
		return AREA_TRIGGER_FAILURE_NO_ATTUNE_Q;

	if(pMapInfo && pMapInfo->required_item && !pPlayer->GetItemInterface()->GetItemCount(pMapInfo->required_item, true))
		return AREA_TRIGGER_FAILURE_NO_ATTUNE_I;

	if (pPlayer->iInstanceType >= MODE_HEROIC && 
		pMapInfo->type == INSTANCE_MULTIMODE && 
		!pPlayer->GetItemInterface()->GetItemCount(pMapInfo->heroic_key_1, false) && 
		!pPlayer->GetItemInterface()->GetItemCount(pMapInfo->heroic_key_2, false))
		return AREA_TRIGGER_FAILURE_NO_KEY;

	if(pPlayer->getLevel()<PLAYER_LEVEL_CAP_70 && pPlayer->iInstanceType>=MODE_HEROIC && pMapInfo->type != INSTANCE_NULL)
		return AREA_TRIGGER_FAILURE_LEVEL_HEROIC;

	return AREA_TRIGGER_FAILURE_OK;
}

void WorldSession::_HandleAreaTriggerOpcode(uint32 id)
{		
	sLog.outDebug("AreaTrigger: %u", id);

	// Are we REALLY here?
	if( !_player->IsInWorld() )
		return;

    // Search quest log, find any exploration quests
	sQuestMgr.OnPlayerExploreArea(GetPlayer(),id);

	AreaTriggerEntry* entry = dbcAreaTrigger.LookupEntry(id);
	AreaTrigger* pAreaTrigger = AreaTriggerStorage.LookupEntry(id);

	if( entry == NULL )
	{
		sLog.outDebug("Missing AreaTrigger: %u", id);
		return;
	}

	sHookInterface.OnAreaTrigger(GetPlayer(), id);

#ifdef GM_Z_DEBUG_DIRECTLY
	if( _player->GetSession() && _player->GetSession()->CanUseCommand('z') )
		sChatHandler.BlueSystemMessage( this, "[%sSystem%s] |rEntered areatrigger: %s%u. (%s)", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE, id, pAreaTrigger ? pAreaTrigger->Name : "Unknown name" );
#endif

	// if in BG handle is triggers
	if( _player->m_bg )
	{
		_player->m_bg->HookOnAreaTrigger(_player, id);
		return;
	}

	// Hook for Scripted Areatriggers
	_player->GetMapMgr()->HookOnAreaTrigger(_player, id);

	if (pAreaTrigger == NULL) return;

	switch(pAreaTrigger->Type)
	{
	case ATTYPE_INSTANCE:
		{
			if(GetPlayer()->GetPlayerStatus() == TRANSFER_PENDING)  //only ports if player is out of pendings
				return;
			if( sWorld.instance_CheckTriggerPrerequsites )
			{
				uint32 reason = CheckTriggerPrerequsites(pAreaTrigger, this, _player, WorldMapInfoStorage.LookupEntry(pAreaTrigger->Mapid));
				if(reason != AREA_TRIGGER_FAILURE_OK)
				{
					const char * pReason = GetPlayer()->GetSession()->LocalizedWorldSrv(AreaTriggerFailureMessages[reason]);
					char msg[200];
					WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 50);
					data << uint32(0);
                    
					switch (reason)
					{
					case AREA_TRIGGER_FAILURE_LEVEL:
						snprintf(msg, 200, pReason, pAreaTrigger->required_level);
						data << msg;
						break;
					case AREA_TRIGGER_FAILURE_NO_ATTUNE_I:
						{
							MapInfo * pMi = WorldMapInfoStorage.LookupEntry(pAreaTrigger->Mapid);
							ItemPrototype * pItem = ItemPrototypeStorage.LookupEntry(pMi->required_item);
							if(pItem)
								snprintf(msg, 200, GetPlayer()->GetSession()->LocalizedWorldSrv(35), pItem->Name1);
							else
								snprintf(msg, 200, GetPlayer()->GetSession()->LocalizedWorldSrv(36));

							data << msg;
						}break;
					case AREA_TRIGGER_FAILURE_NO_ATTUNE_Q:
						{
							MapInfo * pMi = WorldMapInfoStorage.LookupEntry(pAreaTrigger->Mapid);
							Quest * pQuest = QuestStorage.LookupEntry(pMi->required_quest);
							if(pQuest)
								snprintf(msg, 200, GetPlayer()->GetSession()->LocalizedWorldSrv(35), pQuest->title);
							else
								snprintf(msg, 200, GetPlayer()->GetSession()->LocalizedWorldSrv(36));

							data << msg;
						}break;
					default:
						data << pReason;
						break;
					}

					data << uint8(0);
					SendPacket(&data);
					return;
				}
			}
			GetPlayer()->SaveEntryPoint(pAreaTrigger->Mapid);
			GetPlayer()->SafeTeleport(pAreaTrigger->Mapid, 0, LocationVector(pAreaTrigger->x, pAreaTrigger->y, pAreaTrigger->z, pAreaTrigger->o));
		}break;
	case ATTYPE_QUESTTRIGGER:
		{

		}break;
	case ATTYPE_INN:
		{
			// Inn
			if (!GetPlayer()->m_isResting) GetPlayer()->ApplyPlayerRestState(true);
		}break;
	case ATTYPE_TELEPORT:
		{
			if(GetPlayer()->GetPlayerStatus() != TRANSFER_PENDING) //only ports if player is out of pendings
			{
				GetPlayer()->SaveEntryPoint(pAreaTrigger->Mapid);
				GetPlayer()->SafeTeleport(pAreaTrigger->Mapid, 0, LocationVector(pAreaTrigger->x, pAreaTrigger->y, pAreaTrigger->z, pAreaTrigger->o));
			}
		}break;
	default:break;
	}
}
