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

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );

	if(!pn)
		return;

	sLog.outDebug( "Received CMSG_NAME_QUERY for: %s", pn->name );

	WorldPacket data(SMSG_NAME_QUERY_RESPONSE, strlen(pn->name) + 35);
	data << pn->guid << uint32(0);	//highguid
	data << pn->name;
	data << uint8(0);	   // this is a string showed besides players name (eg. in combat log), a custom title ?
	data << pn->race << pn->gender << pn->cl;
	data << uint8(0);			// 2.4.0, why do i get the feeling blizz is adding custom classes or custom titles? (same thing in who list)
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
#ifdef USING_BIG_ENDIAN
	swap32(&t);
#endif
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	uint32 entry;
	uint64 guid;
	CreatureInfo *ci;

	recv_data >> entry;
	recv_data >> guid;

	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID";
		for(uint32 i = 0; i < 8;i++)
		{
			data << uint32(0);
		}
		data << uint8(0);  
	}
	else
	{
		ci = CreatureNameStorage.LookupEntry(entry);
		if(ci == NULL)
			return;

		LocalizedCreatureName * lcn = (language>0) ? sLocalizationMgr.GetLocalizedCreatureName(entry, language) : NULL;

		if(lcn == NULL)
		{
			sLog.outDetail("WORLD: CMSG_CREATURE_QUERY '%s'", ci->Name);
			data << (uint32)entry;
			data << ci->Name;
			data << uint8(0) << uint8(0) << uint8(0);
			data << ci->SubName;
		}
		else
		{
			sLog.outDetail("WORLD: CMSG_CREATURE_QUERY '%s' (localized to %s)", ci->Name, lcn->Name);
			data << (uint32)entry;
			data << lcn->Name;
			data << uint8(0) << uint8(0) << uint8(0);
			data << lcn->SubName;
		}
		data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
		data << ci->Flags1;  
		data << ci->Type;
		data << ci->Family;
		data << ci->Rank;
		data << ci->Unknown1;
		data << ci->SpellDataID;
		data << ci->Male_DisplayID;
		data << ci->Female_DisplayID;
		data << ci->Male_DisplayID2;
		data << ci->Female_DisplayID2;
		data << ci->unkfloat1;
		data << ci->unkfloat2;
		data << ci->Leader;
	}

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 300);

	uint32 entryID;
	uint64 guid;
	GameObjectInfo *goinfo;
	

	recv_data >> entryID;
	recv_data >> guid;

	sLog.outDetail("WORLD: CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL)
		return;

	LocalizedGameObjectName * lgn = (language>0) ? sLocalizationMgr.GetLocalizedGameObjectName(entryID, language) : NULL;
    
	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	if(lgn)
		data << lgn->Name;
	else
		data << goinfo->Name;

	data << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0);   // new string in 1.12
	data << goinfo->SpellFocus;
	data << goinfo->sound1;
	data << goinfo->sound2;
	data << goinfo->sound3;
	data << goinfo->sound4;
	data << goinfo->sound5;
	data << goinfo->sound6;
	data << goinfo->sound7;
	data << goinfo->sound8;
	data << goinfo->sound9;
	data << goinfo->Unknown1;
	data << goinfo->Unknown2;
	data << goinfo->Unknown3;
	data << goinfo->Unknown4;
	data << goinfo->Unknown5;
	data << goinfo->Unknown6;
	data << goinfo->Unknown7;
	data << goinfo->Unknown8;
	data << goinfo->Unknown9;
	data << goinfo->Unknown10;
	data << goinfo->Unknown11;
	data << goinfo->Unknown12;
	data << goinfo->Unknown13;
	data << goinfo->Unknown14;

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
	sLog.outDetail("WORLD: Received MSG_CORPSE_QUERY");

	Corpse *pCorpse;
	WorldPacket data(MSG_CORPSE_QUERY, 21);
	MapInfo *pMapinfo;

	pCorpse = objmgr.GetCorpseByOwner(GetPlayer()->GetLowGUID());
	if(pCorpse)
	{
		pMapinfo = WorldMapInfoStorage.LookupEntry(pCorpse->GetMapId());
		if(pMapinfo)
		{
			if(pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP)
			{
				data << uint8(0x01); //show ?
				data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
				data << pCorpse->GetPositionX();
				data << pCorpse->GetPositionY();
				data << pCorpse->GetPositionZ();
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				SendPacket(&data);
			}
			else
			{
				data << uint8(0x01); //show ?
				data << pMapinfo->repopmapid; // mapid (that tombstones shown on)
				data << pMapinfo->repopx;
				data << pMapinfo->repopy;
				data << pMapinfo->repopz;
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				SendPacket(&data);
			}
		}
		else
		{

			data.Initialize(MSG_CORPSE_QUERY);
			data << uint8(0x01); //show ?
			data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
			data << pCorpse->GetPositionX();
			data << pCorpse->GetPositionY();
			data << pCorpse->GetPositionZ();
			data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
			SendPacket(&data);

		}
	}
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	recv_data >> pageid;

	while(pageid)
	{
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(!page)
			return;

		LocalizedItemPage * lpi = (language>0) ? sLocalizationMgr.GetLocalizedItemPage(pageid,language):NULL;
		WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 1000);
		data << pageid;
		if(lpi)
			data << lpi->Text;
		else
			data << page->text;

		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}

}
//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	WorldPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, 100);
	uint32 itemid;
	recv_data >> itemid;
	reply << itemid;
	ItemPrototype *proto=ItemPrototypeStorage.LookupEntry(itemid);
	if(!proto)
		reply << "Unknown Item";
	else
	{
		LocalizedItem * li = (language>0) ? sLocalizationMgr.GetLocalizedItem(itemid, language) : NULL;
		if(li)
			reply << li->Name;
		else
			reply << proto->Name1;
	}
	SendPacket(&reply);	
}

void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	WorldPacket data(SMSG_INRANGE_QUESTGIVER_STATUS_QUERY_RESPONSE, 1000);
	Object::InRangeSet::iterator itr;
	Creature * pCreature;
	uint32 count = 0;
	data << count;

	// 32 count
	// <foreach count>
	//    64 guid
	//    8 status

	for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); ++itr )
	{
		pCreature = static_cast<Creature*>(*itr);
		if( pCreature->GetTypeId() != TYPEID_UNIT )
			continue;

		if( pCreature->isQuestGiver() )
		{
			data << pCreature->GetGUID();
			data << uint8(sQuestMgr.CalcStatus( pCreature, _player ));
			++count;
		}
	}

	*(uint32*)(data.contents()) = count;
	SendPacket(&data);
}
