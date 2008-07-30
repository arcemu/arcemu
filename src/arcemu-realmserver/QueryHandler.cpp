/*
 * ArcEmu MMORPG Server
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

#include "RStdAfx.h"

void Session::HandleCreatureQueryOpcode(WorldPacket & pck)
{
	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	CreatureInfo * ci;
	uint32 entry;
	pck >> entry;

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
		Log.Debug("World", "CMSG_CREATURE_QUERY '%s'", ci->Name);

		data << (uint32)entry;
		data << ci->Name;
		data << uint8(0) << uint8(0) << uint8(0);
		data << ci->SubName;
		data << ci->Flags1;  
		data << ci->Type;
		data << ci->Family;
		data << ci->Rank;
		data << ci->Unknown1;
		data << ci->SpellDataID;
		data << ci->DisplayID;
		data << ci->unk2;
		data << ci->unk3;
		data << ci->Civilian;
		data << ci->Leader;

	}

	SendPacket( &data );
}

void Session::HandleGameObjectQueryOpcode(WorldPacket & pck)
{
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 300);
	uint32 entryID;
	GameObjectInfo *goinfo;

	pck >> entryID;

	Log.Debug("World", "CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == 0)
		return;

	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
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
void Session::HandleItemQuerySingleOpcode(WorldPacket & pck)
{
	int i;
	uint32 itemid;
	pck >> itemid;

	Log.Debug("World", "CMSG_ITEM_QUERY_SINGLE for item id %d",	itemid );

	ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
	if(!itemProto)
	{
		return;
	} 

	WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600 + strlen(itemProto->Name1) + strlen(itemProto->Description) );
	data << itemProto->ItemId;
	data << itemProto->Class;
	data << itemProto->SubClass;
	data << itemProto->unknown_bc;
	data << itemProto->Name1;
	/*data << itemProto->Name2;
	data << itemProto->Name3;
	data << itemProto->Name4;*/
	data << uint8(0) << uint8(0) << uint8(0); // name 2,3,4
	data << itemProto->DisplayInfoID;
	data << itemProto->Quality;
	data << itemProto->Flags;
	data << itemProto->BuyPrice;
	data << itemProto->SellPrice;
	data << itemProto->InventoryType;
	data << itemProto->AllowableClass;
	data << itemProto->AllowableRace;
	data << itemProto->ItemLevel;
	data << itemProto->RequiredLevel;
	data << itemProto->RequiredSkill;
	data << itemProto->RequiredSkillRank;
	data << itemProto->RequiredSkillSubRank;
	data << itemProto->RequiredPlayerRank1;
	data << itemProto->RequiredPlayerRank2;
	data << itemProto->RequiredFaction;
	data << itemProto->RequiredFactionStanding;
	data << itemProto->Unique;
	data << itemProto->MaxCount;
	data << itemProto->ContainerSlots;
	for(i = 0; i < 10; i++)
	{
		data << itemProto->Stats[i].Type;
		data << itemProto->Stats[i].Value;
	}
	for(i = 0; i < 5; i++)
	{
		data << itemProto->Damage[i].Min;
		data << itemProto->Damage[i].Max;
		data << itemProto->Damage[i].Type;
	}
	data << itemProto->Armor;
	data << itemProto->HolyRes;
	data << itemProto->FireRes;
	data << itemProto->NatureRes;
	data << itemProto->FrostRes;
	data << itemProto->ShadowRes;
	data << itemProto->ArcaneRes;
	data << itemProto->Delay;
	data << itemProto->AmmoType;
	data << itemProto->Range;
	for(i = 0; i < 5; i++) {
		data << itemProto->Spells[i].Id;
		data << itemProto->Spells[i].Trigger;
		data << itemProto->Spells[i].Charges;
		data << itemProto->Spells[i].Cooldown;
		data << itemProto->Spells[i].Category;
		data << itemProto->Spells[i].CategoryCooldown;
	}
	data << itemProto->Bonding;
	data << itemProto->Description;
	data << itemProto->PageId;
	data << itemProto->PageLanguage;
	data << itemProto->PageMaterial;
	data << itemProto->QuestId;
	data << itemProto->LockId;
	data << itemProto->LockMaterial;
	data << itemProto->Field108;
	data << itemProto->RandomPropId;
	data << itemProto->RandomSuffixId;
	data << itemProto->Block;
	data << itemProto->ItemSet;
	data << itemProto->MaxDurability;
	data << itemProto->ZoneNameID;
	data << itemProto->MapID;
	data << itemProto->BagFamily;
	data << itemProto->ToolCategory;
	data << itemProto->Sockets[0].SocketColor ;
	data << itemProto->Sockets[0].Unk;
	data << itemProto->Sockets[1].SocketColor ;
	data << itemProto->Sockets[1].Unk ;
	data << itemProto->Sockets[2].SocketColor ;
	data << itemProto->Sockets[2].Unk ;
	/*
	data << itemProto->SocketColor1;
	data << itemProto->Unk201_3;
	data << itemProto->SocketColor2;
	data << itemProto->Unk201_5;
	data << itemProto->SocketColor3;
	data << itemProto->Unk201_7;*/
	data << itemProto->SocketBonus;
	data << itemProto->GemProperties;
	data << itemProto->ItemExtendedCost;
	data << itemProto->DisenchantReqSkill;
	data << itemProto->ArmorDamageModifier;
	//WPAssert(data.size() == 453 + itemProto->Name1.length() + itemProto->Description.length());
	SendPacket( &data );
}
