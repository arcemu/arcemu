/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

Item::Item()//this is called when constructing as container
{
	m_itemProto = NULL;
	m_owner = NULL;
	loot = NULL;
	locked = false;
	wrapped_item_id = 0;
	m_objectTypeId = TYPEID_ITEM;
	m_valuesCount = ITEM_END;
	m_uint32Values = _fields;
	m_updateMask.SetCount(ITEM_END);
	random_prop = 0;
	random_suffix = 0;
	m_mapMgr = 0;
	m_faction = NULL;
	m_factionDBC = NULL;
	m_instanceId = INSTANCEID_NOT_IN_WORLD;
	m_inQueue = false;
	m_extensions = NULL;
	m_loadedFromDB = false;
    ItemExpiresOn = 0;
	Enchantments.clear();

	for( uint32 i = 0; i < 3; ++i )
		OnUseSpellIDs[ i ] = 0;
}

void Item::Init( uint32 high, uint32 low )
{
    ///////////////////////////// from virtual_constructor ///////////////
    memset( m_uint32Values, 0, (ITEM_END) * sizeof( uint32 ) );
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_ITEM | TYPE_OBJECT );
	SetScale(  1 );//always 1
	SetScale(  1 );//always 1
	m_itemProto = NULL;
	m_owner = NULL;
	loot = NULL;
	locked = false;
	m_isDirty = true;
	random_prop = 0;
	random_suffix = 0;
	wrapped_item_id = 0;

	m_mapId = MAPID_NOT_IN_WORLD;
	m_zoneId = 0;
	m_objectUpdated = false;

	m_mapMgr = 0;
	m_faction = NULL;
	m_factionDBC = NULL;
	m_instanceId = INSTANCEID_NOT_IN_WORLD;
	m_inQueue = false;
	m_extensions = NULL;
	m_loadedFromDB = false;
    //////////////////////////////////////////////////////////
	SetLowGUID( low );
	SetHighGUID( high );
	m_wowGuid.Init( GetGUID() );
}

Item::~Item()
{
    if( loot != NULL )
	{
		delete loot;
		loot = NULL;
	}

	sEventMgr.RemoveEvents( this );

	EnchantmentMap::iterator itr;
	for( itr = Enchantments.begin(); itr != Enchantments.end(); ++itr )
	{
		if( itr->second.Enchantment->type == 0 && itr->second.Slot == 0 && itr->second.ApplyTime == 0 && itr->second.Duration == 0 )
		{
			delete itr->second.Enchantment;
			itr->second.Enchantment = NULL;
		}
	}
	Enchantments.clear();

	if( IsInWorld() )
		RemoveFromWorld();

	m_owner = NULL;
}

void Item::Create( uint32 itemid, Player* owner )
{
	SetEntry(  itemid );

	if( owner != NULL )
	{
        uint64 OwnerGUID = owner->GetGUID();

        SetOwnerGUID( OwnerGUID );
		SetContainerGUID( OwnerGUID );
	}

	SetStackCount(  1 );

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemid );

	Arcemu::Util::ARCEMU_ASSERT(    m_itemProto  != NULL );
	 
	SetCharges( 0, m_itemProto->Spells[0].Charges );
	SetCharges( 1, m_itemProto->Spells[1].Charges );
	SetCharges( 2, m_itemProto->Spells[2].Charges );
	SetCharges( 3, m_itemProto->Spells[3].Charges );
	SetCharges( 4, m_itemProto->Spells[4].Charges );
    SetDurability( m_itemProto->MaxDurability );
	SetDurabilityMax( m_itemProto->MaxDurability );


	m_owner = owner;
	if( m_itemProto->LockId > 1 )
		locked = true;
	else
		locked = false;
}

void Item::LoadFromDB(Field* fields, Player* plr, bool light )
{
	uint32 itemid = fields[2].GetUInt32();
	uint32 random_prop, random_suffix;
	uint32 count;

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemid );

	Arcemu::Util::ARCEMU_ASSERT(    m_itemProto != NULL  );
	
	if(m_itemProto->LockId > 1)
		locked = true;
	else
		locked = false;
	
	SetEntry(  itemid );
	m_owner = plr;

	wrapped_item_id=fields[3].GetUInt32();
	SetGiftCreatorGUID( fields[4].GetUInt32() );
    SetCreatorGUID( fields[5].GetUInt32() );

	count = fields[6].GetUInt32();
	if( count > m_itemProto->MaxCount && (m_owner && !m_owner->ItemStackCheat) )
		count = m_itemProto->MaxCount;
	SetStackCount(  count);

	SetChargesLeft( fields[ 7 ].GetUInt32() );

	SetUInt32Value( ITEM_FIELD_FLAGS, fields[8].GetUInt32() );
	random_prop = fields[9].GetUInt32();
	random_suffix = fields[10].GetUInt32();

	SetItemRandomPropertyId( random_prop );

	int32 rprop = int32( random_prop );
	// If random properties point is negative that means the item uses random suffix as random enchantment
	if( rprop < 0 )
		SetItemRandomSuffixFactor( random_suffix );
	else
		SetItemRandomSuffixFactor( 0 );

	//SetTextId( fields[11].GetUInt32() );

	SetDurabilityMax( m_itemProto->MaxDurability );
	SetDurability( fields[12].GetUInt32() );

	if( light )
		return;

	string enchant_field = fields[15].GetString();
	vector< string > enchants = StrSplit( enchant_field, ";" );
	uint32 enchant_id;
	EnchantEntry* entry;
	uint32 time_left;
	uint32 enchslot;

	for( vector<string>::iterator itr = enchants.begin(); itr != enchants.end(); ++itr )
	{
		if( sscanf( (*itr).c_str(), "%u,%u,%u", (unsigned int*)&enchant_id, (unsigned int*)&time_left, (unsigned int*)&enchslot) == 3 )
		{
			entry = dbcEnchant.LookupEntryForced( enchant_id );
			if( entry && entry->Id == enchant_id && m_itemProto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
			{
				AddEnchantment( entry, time_left, ( time_left == 0 ), false, false, enchslot );
				//(enchslot != 2) ? false : true, false);
			}
			else
			{
				/*
				EnchantEntry *pEnchant = new EnchantEntry;
				memset(pEnchant,0,sizeof(EnchantEntry));

				pEnchant->Id = enchant_id;
				if(enchslot != 2)
					AddEnchantment(pEnchant,0,true, false);
				else
					AddEnchantment(pEnchant,0,false,false);
				*/
			}
		}
	}

    ItemExpiresOn = fields[16].GetUInt32();
    
///////////////////////////////////////////////////// Refund stuff ////////////////////////
    std::pair< time_t, uint32 > refundentry;

    refundentry.first = fields[17].GetUInt32();
    refundentry.second = fields[18].GetUInt32();

    uint32 *played = this->GetOwner()->GetPlayedtime();

    if( refundentry.first != 0 && refundentry.second != 0 ){
        if( played[1] < ( refundentry.first + 60*60*2 ) )
            m_owner->GetItemInterface()->AddRefundable( this, refundentry.second, refundentry.first );
    }

///////////////////////////////////////////////////////////////////////////////////////////
    
	ApplyRandomProperties( false );

	// Charter stuff
	if( GetEntry() == ITEM_ENTRY_GUILD_CHARTER)
	{
        SoulBind();
		SetStackCount(  1 );
		SetItemRandomSuffixFactor( 57813883 );
		if( plr != NULL && plr->m_charters[CHARTER_TYPE_GUILD] )
			SetEnchantmentId( 0, plr->m_charters[CHARTER_TYPE_GUILD]->GetID() );
	}

	if( GetEntry() == ARENA_TEAM_CHARTER_2v2 )
	{
        SoulBind();
		SetStackCount(  1 );
		SetItemRandomSuffixFactor( 57813883 );
		if( plr != NULL && plr->m_charters[CHARTER_TYPE_ARENA_2V2] )
			SetEnchantmentId( 0, plr->m_charters[CHARTER_TYPE_ARENA_2V2]->GetID() );
	}

	if( GetEntry() == ARENA_TEAM_CHARTER_3v3 )
	{
        SoulBind();
		SetStackCount(  1 );
		SetItemRandomSuffixFactor( 57813883 );
		if( plr != NULL && plr->m_charters[CHARTER_TYPE_ARENA_3V3] )
			SetEnchantmentId( 0,  plr->m_charters[CHARTER_TYPE_ARENA_3V3]->GetID() );
	}

	if( GetEntry() == ARENA_TEAM_CHARTER_5v5 )
	{
        SoulBind();
		SetStackCount(  1 );
		SetItemRandomSuffixFactor(  57813883 );
		if( plr != NULL && plr->m_charters[CHARTER_TYPE_ARENA_5V5] )
			SetEnchantmentId( 0,  plr->m_charters[CHARTER_TYPE_ARENA_5V5]->GetID() );
	}
}

void Item::ApplyRandomProperties( bool apply )
{
	// apply random properties
	if( GetItemRandomPropertyId() != 0 )
	{
		if( int32( GetItemRandomPropertyId() ) > 0 )
		{
			RandomProps* rp= dbcRandomProps.LookupEntry( GetItemRandomPropertyId() );
			int32 Slot;
			for( int k = 0; k < 3; k++ )
			{
				if( rp->spells[k] != 0 )
				{	
					EnchantEntry* ee = dbcEnchant.LookupEntry( rp->spells[k] );
					Slot = HasEnchantment( ee->Id );
					if( Slot < 0 ) 
					{
						Slot = FindFreeEnchantSlot( ee, 1 );
						AddEnchantment( ee, 0, false, apply, true, Slot );
					}
					else
						if( apply )
							ApplyEnchantmentBonus( Slot, true );
				}
			}
		}
		else
		{
			ItemRandomSuffixEntry* rs = dbcItemRandomSuffix.LookupEntry( abs( int( GetItemRandomPropertyId() ) ) );
			int32 Slot;
			for( uint32 k = 0; k < 3; ++k )
			{
				if( rs->enchantments[k] != 0 )
				{
					EnchantEntry* ee = dbcEnchant.LookupEntry( rs->enchantments[k] );
					Slot = HasEnchantment( ee->Id );
					if( Slot < 0 ) 
					{
						Slot = FindFreeEnchantSlot( ee, 2 );
						AddEnchantment( ee, 0, false, apply, true, Slot, rs->prefixes[k] );
					}
					else
						if( apply )
							ApplyEnchantmentBonus( Slot, true );
				}
			}
		}
	}
}

void Item::SaveToDB( int8 containerslot, int8 slot, bool firstsave, QueryBuffer* buf )
{
	if( !m_isDirty && !firstsave )
		return;

    uint64 GiftCreatorGUID = GetGiftCreatorGUID();
    uint64 CreatorGUID = GetCreatorGUID();

	std::stringstream ss;
    
    ss << "DELETE FROM playeritems WHERE guid = " << GetLowGUID() << ";";

    if( firstsave )
		CharacterDatabase.WaitExecute( ss.str().c_str() );
	else
	{
		if( buf == NULL )
			CharacterDatabase.Execute( ss.str().c_str() );
		else
			buf->AddQueryNA( ss.str().c_str() );
	}


    ss.rdbuf()->str("");

    uint64 ownerGUID = GetOwnerGUID();

	ss << "INSERT INTO playeritems VALUES(";

    ss << ( Arcemu::Util::GUID_LOPART( ownerGUID ) ) << ",";
	ss << GetLowGUID() << ",";
	ss << GetEntry() << ",";
	ss << wrapped_item_id << ",";
	ss << ( Arcemu::Util::GUID_LOPART( GiftCreatorGUID ) ) << ",";
    ss << ( Arcemu::Util::GUID_LOPART( CreatorGUID ) ) << ",";

	ss << GetStackCount() << ",";
	ss << int32( GetChargesLeft() ) << ",";
	ss << uint32( m_uint32Values[ ITEM_FIELD_FLAGS ] ) << ",";
	ss << random_prop << ", " << random_suffix << ", ";
	ss << 0 << ",";
	ss << GetDurability() << ",";
	ss << static_cast<int>(containerslot) << ",";
	ss << static_cast<int>(slot) << ",'";

	// Pack together enchantment fields
	if( Enchantments.size() > 0 )
	{
		EnchantmentMap::iterator itr = Enchantments.begin();
		for(; itr != Enchantments.end(); ++itr)
		{
			if( itr->second.RemoveAtLogout )
				continue;

			uint32 elapsed_duration = uint32( UNIXTIME - itr->second.ApplyTime );
			int32 remaining_duration = itr->second.Duration - elapsed_duration;
			if( remaining_duration < 0 )
				remaining_duration = 0;


			if( itr->second.Enchantment && ( remaining_duration > 5 || itr->second.Duration == 0 ) )
			{
				ss << itr->second.Enchantment->Id << ",";
				ss << remaining_duration << ",";
				ss << itr->second.Slot << ";";
			}
		}
	}
    ss << "','";
    ss << ItemExpiresOn << "','";

////////////////////////////////////////////////// Refund stuff /////////////////////////////////

    // Check if the owner is instantiated. When sending mail he/she obviously will not be :P
    if( this->GetOwner() != NULL ){
        std::pair< time_t, uint32 > refundentry;

        refundentry.first = 0;
        refundentry.second = 0;

        refundentry = this->GetOwner()->GetItemInterface()->LookupRefundable( this->GetGUID() );
        
        ss << uint32( refundentry.first ) << "','";
        ss << uint32( refundentry.second ); 

    }else{
        ss << uint32( 0 ) << "','";
        ss << uint32( 0 );
    }

/////////////////////////////////////////////////////////////////////////////////////////////////
    ss << "')";

	if( firstsave )
		CharacterDatabase.WaitExecute( ss.str().c_str() );
	else
	{
		if( buf == NULL )
			CharacterDatabase.Execute( ss.str().c_str() );
		else
            buf->AddQueryNA( ss.str().c_str() );
	}

	m_isDirty = false;
}

void Item::DeleteFromDB()
{
	if( m_itemProto->ContainerSlots>0 && GetTypeId() == TYPEID_CONTAINER )
	{
		/* deleting a container */
		for( uint32 i = 0; i < m_itemProto->ContainerSlots; ++i )
		{
			if( static_cast< Container* >( this )->GetItem( static_cast<int16>( i ) ) != NULL )
			{
				/* abort the delete */
				return;
			}
		}
	}

	CharacterDatabase.Execute( "DELETE FROM playeritems WHERE guid = %u", m_uint32Values[LOWGUID] );
}

void Item::DeleteMe()
{
	//Don't inline me!

    // check to see if our owner is instantiated
    if( this->m_owner != NULL )
        this->m_owner->GetItemInterface()->RemoveRefundable( this->GetGUID() );

	if( IsContainer() ) {
		delete static_cast<Container*>(this);
	} else {
		delete this;
	}
}

uint32 GetSkillByProto( uint32 Class, uint32 SubClass )
{
	if( Class == 4 && SubClass < 7 )
	{
		return arm_skills[SubClass];
	} 
    else if( Class == 2 )
	{
		if( SubClass < 20 )//no skill for fishing
		{
			return weap_skills[SubClass];
		}
	}
	return 0;
}

//This map is used for profess.
//Prof packet struct: {SMSG_SET_PROFICIENCY,(uint8)item_class,(uint32)1<<item_subclass}
//ie: for fishing (it's class=2--weapon, subclass ==20 -- fishing rod) permissive packet
// will have structure 0x2,524288
//this table is needed to get class/subclass by skill, valid classes are 2 and 4

const ItemProf* GetProficiencyBySkill( uint32 skill )
{
	switch( skill )
	{
		case SKILL_CLOTH:
			return &prof[0];
		case SKILL_LEATHER:
			return &prof[1];
		case SKILL_MAIL:
			return &prof[2];
		case SKILL_PLATE_MAIL:
			return &prof[3];
		case SKILL_SHIELD:
			return &prof[4];
		case SKILL_AXES:
			return &prof[5];
		case SKILL_2H_AXES:
			return &prof[6];
		case SKILL_BOWS:
			return &prof[7];
		case SKILL_GUNS:
			return &prof[8];
		case SKILL_MACES:
			return &prof[9];
		case SKILL_2H_MACES:
			return &prof[10];
		case SKILL_POLEARMS:
			return &prof[11];
		case SKILL_SWORDS:
			return &prof[12];
		case SKILL_2H_SWORDS:
			return &prof[13];
		case SKILL_STAVES:
			return &prof[14];
		case SKILL_FIST_WEAPONS:
			return &prof[15];
		case SKILL_DAGGERS:
			return &prof[16];
		case SKILL_THROWN:
			return &prof[17];
		case SKILL_CROSSBOWS:
			return &prof[19];
		case SKILL_WANDS:
			return &prof[20];
		case SKILL_FISHING:
			return &prof[21];
		default:
			return NULL;
	}
}

uint32 GetSellPriceForItem( ItemPrototype *proto, uint32 count )
{
	int32 cost;
	cost = proto->SellPrice * ( ( count < 1 ) ? 1 : count );
	return cost;
}

uint32 GetBuyPriceForItem( ItemPrototype* proto, uint32 count, Player* plr, Creature* vendor )
{
	int32 cost = proto->BuyPrice;

	if( plr != NULL && vendor != NULL )
	{
		Standing plrstanding = plr->GetStandingRank( vendor->m_faction->Faction );
		cost = float2int32( ceilf( proto->BuyPrice * pricemod[plrstanding] ) );
	}

	return cost * count;
}

uint32 GetSellPriceForItem( uint32 itemid, uint32 count )
{
	if( ItemPrototype* proto = ItemPrototypeStorage.LookupEntry( itemid ) )
		return GetSellPriceForItem(proto, count);
	else
		return 1;
}

uint32 GetBuyPriceForItem( uint32 itemid, uint32 count, Player* plr, Creature* vendor )
{
	if( ItemPrototype* proto = ItemPrototypeStorage.LookupEntry( itemid ) )
		return GetBuyPriceForItem( proto, count, plr, vendor );
	else
		return 1;
}

void Item::RemoveFromWorld()
{
	// if we have an owner->send destroy
	if( m_owner != NULL )
		m_owner->SendDestroyObject( GetGUID() );

	if( !IsInWorld() )
		return;

	m_mapMgr->RemoveObject( this, false );
	m_mapMgr = NULL;
  
	// update our event holder
	event_Relocate();
}

void Item::SetOwner( Player* owner )
{ 
	if( owner != NULL )
		SetOwnerGUID( owner->GetGUID() );
	else 
        SetOwnerGUID( 0 );

	m_owner = owner; 
}


int32 Item::AddEnchantment( EnchantEntry* Enchantment, uint32 Duration, bool Perm /* = false */, bool apply /* = true */, bool RemoveAtLogout /* = false */, uint32 Slot_, uint32 RandomSuffix )
{
	int32 Slot = Slot_;
	m_isDirty = true;

	// Create the enchantment struct.
	EnchantmentInstance Instance;
	Instance.ApplyTime = UNIXTIME;
	Instance.BonusApplied = false;
	Instance.Slot = Slot;
	Instance.Enchantment = Enchantment;
	Instance.Duration = Duration;
	Instance.RemoveAtLogout = RemoveAtLogout;
	Instance.RandomSuffix = RandomSuffix;

	// Set the enchantment in the item fields.
	SetEnchantmentId( Slot, Enchantment->Id );
	SetEnchantmentDuration( Slot, (uint32)Instance.ApplyTime );
	SetEnchantmentCharges( Slot, 0 );

	// Add it to our map.
	Enchantments.insert(make_pair((uint32)Slot, Instance));

	if( m_owner == NULL )
		return Slot;

	// Add the removal event.
	if( Duration )
	{
		sEventMgr.AddEvent( this, &Item::RemoveEnchantment, uint32(Slot), EVENT_REMOVE_ENCHANTMENT1 + Slot, Duration * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	}

	// No need to send the log packet, if the owner isn't in world (we're still loading)
	if( !m_owner->IsInWorld() )
		return Slot;

	if( apply )
	{
		WorldPacket EnchantLog( SMSG_ENCHANTMENTLOG, 25 );
		EnchantLog << m_owner->GetGUID();
		EnchantLog << m_owner->GetGUID();
		EnchantLog << GetEntry();
		EnchantLog << Enchantment->Id;
		EnchantLog << uint8(0);
		m_owner->SendPacket( &EnchantLog );

		if( m_owner->GetTradeTarget() )
		{
			m_owner->SendTradeUpdate();
		}
	
		/* Only apply the enchantment bonus if we're equipped */
		uint16 slot = m_owner->GetItemInterface()->GetInventorySlotByGuid( GetGUID() );
		if( slot >= EQUIPMENT_SLOT_START && slot < EQUIPMENT_SLOT_END )
            ApplyEnchantmentBonus( Slot, APPLY );
	}

	return Slot;
}

void Item::RemoveEnchantment( uint32 EnchantmentSlot )
{
	// Make sure we actually exist.
	EnchantmentMap::iterator itr = Enchantments.find( EnchantmentSlot );
	if( itr == Enchantments.end() )
		return;

	m_isDirty = true;
	uint32 Slot = itr->first;
	if( itr->second.BonusApplied )
		ApplyEnchantmentBonus( EnchantmentSlot, REMOVE );

	// Unset the item fields.
	SetEnchantmentId( Slot, 0 );
	SetEnchantmentDuration( Slot, 0 );
	SetEnchantmentCharges( Slot, 0 );

	// Remove the enchantment event for removal.
	event_RemoveEvents( EVENT_REMOVE_ENCHANTMENT1 + Slot );

	// Remove the enchantment instance.
	Enchantments.erase( itr );
}

void Item::ApplyEnchantmentBonus( uint32 Slot, bool Apply )
{
	if( m_owner == NULL )
		return;

	EnchantmentMap::iterator itr = Enchantments.find( Slot );
	if( itr == Enchantments.end() )
		return;

	EnchantEntry* Entry = itr->second.Enchantment;
	uint32 RandomSuffixAmount = itr->second.RandomSuffix;

	if( itr->second.BonusApplied == Apply )
		return;

	itr->second.BonusApplied = Apply;

	if( Apply )
	{
		// Send the enchantment time update packet.
		SendEnchantTimeUpdate( itr->second.Slot, itr->second.Duration );
	}

	// Apply the visual on the player.
	uint32 ItemSlot = m_owner->GetItemInterface()->GetInventorySlotByGuid( GetGUID() ) * PLAYER_VISIBLE_ITEM_LENGTH; //VLack: for 3.1.1 "* 18" is a bad idea, now it's "* 2"; but this could have been calculated based on UpdateFields.h! This is PLAYER_VISIBLE_ITEM_LENGTH
	uint32 VisibleBase = PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + ItemSlot;
	if ( VisibleBase <= PLAYER_VISIBLE_ITEM_19_ENCHANTMENT )
		m_owner->SetUInt32Value( VisibleBase, Apply ? Entry->Id : 0 ); //On 3.1 we can't add a Slot to the base now, as we no longer have multiple fields for storing them. This in some cases will try to write for example 3 visuals into one place, but now every item has only one field for this, and as we can't choose which visual to have, we'll accept the last one.
	else
		sLog.outError( "Item::ApplyEnchantmentBonus visual out of range! Tried to address UInt32 field %i !!!", VisibleBase );

	// Another one of those for loop that where not indented properly god knows what will break
	// but i made it actually affect the code below it
	for( uint32 c = 0; c < 3; c++ )
	{
		if( Entry->type[c] )
		{
			// Depending on the enchantment type, take the appropriate course of action.
			switch( Entry->type[c] )
			{
			case 1:		 // Trigger spell on melee attack.
				{
					if( Apply )
					{
						if( Entry->spell[c] != 0 )
							m_owner->AddProcTriggerSpell(Entry->spell[c], 0, m_owner->GetGUID(), Entry->min[c], PROC_ON_MELEE_ATTACK, 0, NULL, NULL, this);
					}
					else
					{
						m_owner->RemoveProcTriggerSpell(Entry->spell[c], m_owner->GetGUID(), GetGUID());
					}
				}break;

			case 2:		 // Mod damage done.
				{
					int32 val = Entry->min[c];
					if( RandomSuffixAmount )
						val = RANDOM_SUFFIX_MAGIC_CALCULATION( RandomSuffixAmount, GetItemRandomSuffixFactor() );

					if( Apply )
						m_owner->ModPosDamageDoneMod(SCHOOL_NORMAL,val );
					else
						m_owner->ModPosDamageDoneMod(SCHOOL_NORMAL,-val );
					m_owner->CalcDamage();
				}break;

			case 3:		 // Cast spell (usually means apply aura)
				{
					if( Apply )
					{
						SpellCastTargets targets( m_owner->GetGUID() );
						SpellEntry* sp;
						Spell* spell;
						
						if( Entry->spell[c] != 0 )
						{
							sp = dbcSpell.LookupEntryForced( Entry->spell[c] );
							if( sp == NULL )
								continue;

							spell = new Spell( m_owner, sp, true, 0 );
							spell->i_caster = this;
							spell->prepare( &targets );
						}
					}
					else
					{
						if( Entry->spell[c] != 0 )
							m_owner->RemoveAuraByItemGUID( Entry->spell[c] , GetGUID());
					}

				}break;

			case 4:		 // Modify physical resistance
				{
					int32 val = Entry->min[c];
					if( RandomSuffixAmount )
						val = RANDOM_SUFFIX_MAGIC_CALCULATION( RandomSuffixAmount, GetItemRandomSuffixFactor() );

					if( Apply )
					{
						m_owner->FlatResistanceModifierPos[Entry->spell[c]] += val;
					}
					else
					{
						m_owner->FlatResistanceModifierPos[Entry->spell[c]] -= val;
					}
					m_owner->CalcResistance( Entry->spell[c] );
				}break;

			case 5:	 //Modify rating ...order is PLAYER_FIELD_COMBAT_RATING_1 and above
				{
					//spellid is enum ITEM_STAT_TYPE
					//min=max is amount
					int32 val = Entry->min[c];
					if( RandomSuffixAmount )
						val = RANDOM_SUFFIX_MAGIC_CALCULATION( RandomSuffixAmount, GetItemRandomSuffixFactor() );

					m_owner->ModifyBonuses( Entry->spell[c], val, Apply );
					m_owner->UpdateStats();
				}break;

			case 6:	 // Rockbiter weapon (increase damage per second... how the hell do you calc that)
				{
					if( Apply )
					{
						//m_owner->ModUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS, Entry->min[c]);
						//if I'm not wrong then we should apply DMPS formula for this. This will have somewhat a larger value 28->34
						int32 val = Entry->min[c];
						if( RandomSuffixAmount )
							val = RANDOM_SUFFIX_MAGIC_CALCULATION( RandomSuffixAmount, GetItemRandomSuffixFactor() );

						int32 value = GetProto()->Delay * val / 1000;
						m_owner->ModPosDamageDoneMod(SCHOOL_NORMAL,value );
					}
					else
					{
						int32 val = Entry->min[c];
						if( RandomSuffixAmount )
							val = RANDOM_SUFFIX_MAGIC_CALCULATION( RandomSuffixAmount, GetItemRandomSuffixFactor() );

						int32 value =- (int32)(GetProto()->Delay * val / 1000 );
						m_owner->ModPosDamageDoneMod(SCHOOL_NORMAL,value );
					}
					m_owner->CalcDamage();
				}break;

			case 7:{
				if( Apply ){
					for( uint32 i = 0; i < 3; ++i )
						OnUseSpellIDs[ i ] = Entry->spell[ i ];

				}else{
					for( uint32 i = 0; i < 3; ++i )
						OnUseSpellIDs[ i ] = 0;
				}
				   break;}

			case 8:{
				// Adding a prismatic socket to belt, hands, etc is type 8, it has no bonus to apply HERE
				break;
				   }

			default:
				{
					sLog.outError( "Unknown enchantment type: %u (%u)", Entry->type[c], Entry->Id );
				}break;
			}
		}
	}
}

void Item::ApplyEnchantmentBonuses()
{
	EnchantmentMap::iterator itr, itr2;
	for( itr = Enchantments.begin(); itr != Enchantments.end();  )
	{
		itr2 = itr++;
		ApplyEnchantmentBonus( itr2->first, APPLY );
	}
}

void Item::RemoveEnchantmentBonuses()
{
	EnchantmentMap::iterator itr, itr2;
	for( itr = Enchantments.begin(); itr != Enchantments.end(); )
	{
		itr2 = itr++;
		ApplyEnchantmentBonus( itr2->first, REMOVE );
	}
}

void Item::EventRemoveEnchantment( uint32 Slot )
{
	// Remove the enchantment.
	RemoveEnchantment( Slot );
}

int32 Item::FindFreeEnchantSlot( EnchantEntry* Enchantment, uint32 random_type )
{	
	uint32 GemSlotsReserve = GetSocketsCount();
	if( GetProto()->SocketBonus )
		GemSlotsReserve++;

	if( random_type == RANDOMPROPERTY )		// random prop
	{
		for( uint32 Slot = PROP_ENCHANTMENT_SLOT_2; Slot < MAX_ENCHANTMENT_SLOT; ++Slot )
			if( GetEnchantmentId(Slot) == 0 )
				return Slot;
	}
	else if( random_type == RANDOMSUFFIX )	// random suffix
	{
		for( uint32 Slot = PROP_ENCHANTMENT_SLOT_0; Slot < MAX_ENCHANTMENT_SLOT; ++Slot )
			if( GetEnchantmentId(Slot) == 0 )
				return Slot;
	}
	
	for( uint32 Slot = GemSlotsReserve + 2; Slot < 11; Slot++ )
	{
		if( GetEnchantmentId(Slot) == 0 )
			return Slot;	
	}

	return -1;
}

int32 Item::HasEnchantment( uint32 Id )
{
	for( uint32 Slot = 0; Slot < MAX_ENCHANTMENT_SLOT; Slot++ )
	{
		if( GetEnchantmentId(Slot) == Id )
			return Slot;
	}

	return -1;
}

bool Item::HasEnchantmentOnSlot( uint32 slot )
{
	EnchantmentMap::iterator itr = Enchantments.find( slot );
	if( itr == Enchantments.end() )
		return false;

	return true;
}

void Item::ModifyEnchantmentTime( uint32 Slot, uint32 Duration )
{
	EnchantmentMap::iterator itr = Enchantments.find( Slot );
	if( itr == Enchantments.end() )
		return;

	// Reset the apply time.
	itr->second.ApplyTime = UNIXTIME;
	itr->second.Duration = Duration;

	// Change the event timer.
	event_ModifyTimeAndTimeLeft( EVENT_REMOVE_ENCHANTMENT1 + Slot, Duration * 1000 );

	// Send update packet
	SendEnchantTimeUpdate( itr->second.Slot, Duration );
}

void Item::SendEnchantTimeUpdate( uint32 Slot, uint32 Duration )
{
	/*
	{SERVER} Packet: (0x01EB) SMSG_ITEM_ENCHANT_TIME_UPDATE Size = 24
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|69 32 F0 35 00 00 00 40 01 00 00 00 08 07 00 00 |i2.5...@........|
	|51 46 35 00 00 00 00 00						 |QF5.....		|
	-------------------------------------------------------------------

	uint64 item_guid
	uint32 count?
	uint32 time_in_seconds
	uint64 player_guid
	*/

	WorldPacket* data = new WorldPacket(SMSG_ITEM_ENCHANT_TIME_UPDATE, 24 );
	*data << GetGUID();
	*data << Slot;
	*data << Duration;
	*data << m_owner->GetGUID();
	m_owner->delayedPackets.add( data );
}

void Item::RemoveAllEnchantments( bool OnlyTemporary )
{
	EnchantmentMap::iterator itr, it2;
	for( itr = Enchantments.begin(); itr != Enchantments.end(); )
	{
		it2 = itr++;

		if( OnlyTemporary && it2->second.Duration == 0 ) 
			continue;
			
		RemoveEnchantment( it2->first );
	}
}

void Item::RemoveRelatedEnchants( EnchantEntry* newEnchant )
{
	EnchantmentMap::iterator itr,itr2;
	for( itr = Enchantments.begin(); itr != Enchantments.end(); )
	{
		itr2 = itr++;
		
		if( itr2->second.Enchantment->Id == newEnchant->Id || ( itr2->second.Enchantment->EnchantGroups > 1 && newEnchant->EnchantGroups > 1 ) )
		{ 
			RemoveEnchantment( itr2->first );
		}
	}
}

void Item::RemoveProfessionEnchant()
{
	EnchantmentMap::iterator itr;
	for( itr = Enchantments.begin(); itr != Enchantments.end(); itr++ )
	{
		if( itr->second.Duration != 0 )// not perm
			continue;
		if( IsGemRelated( itr->second.Enchantment ) )
			continue;

		RemoveEnchantment( itr->first );
		return;
	}
}

void Item::RemoveSocketBonusEnchant()
{
	EnchantmentMap::iterator itr;
	
	for( itr = Enchantments.begin(); itr != Enchantments.end(); itr++ )
	{
		if( itr->second.Enchantment->Id == GetProto()->SocketBonus )
		{
			RemoveEnchantment( itr->first );
			return;
		}	
	}
}

EnchantmentInstance* Item::GetEnchantment( uint32 slot )
{
	EnchantmentMap::iterator itr = Enchantments.find( slot );
	if( itr != Enchantments.end() )
		return &itr->second;
	else
		return NULL;
}

bool Item::IsGemRelated( EnchantEntry* Enchantment )
{
	if( GetProto()->SocketBonus == Enchantment->Id )
		return true;
	
	return( Enchantment->GemEntry != 0 );
}

uint32 Item::GetSocketsCount()
{
	if(this->GetTypeId() == TYPEID_CONTAINER) // no sockets on containers.
		return 0;

	uint32 c = 0;
	for( uint32 x = 0; x < 3; x++ )
		if( GetProto()->Sockets[x].SocketColor )
			c++;
	//prismatic socket
	if( GetEnchantment(PRISMATIC_ENCHANTMENT_SLOT) != NULL )
		c++;
	return c;
}

uint32 Item::GenerateRandomSuffixFactor( ItemPrototype* m_itemProto )
{
	double value;

	if( m_itemProto->Class == ITEM_CLASS_ARMOR && m_itemProto->Quality > ITEM_QUALITY_UNCOMMON_GREEN )
		value = SuffixMods[m_itemProto->InventoryType] * 1.24;
	else
		value = SuffixMods[m_itemProto->InventoryType];

	value = ( value * double( m_itemProto->ItemLevel ) ) + 0.5;
	return long2int32( value );
}

string Item::GetItemLink(uint32 language = NULL)
{
	return GetItemLinkByProto(GetProto(), language);
}

string GetItemLinkByProto(ItemPrototype * iProto, uint32 language = NULL)
{
	const char * ItemLink;
	char buffer[256];
	std::string colour;

	switch(iProto->Quality)
	{
		case 0: //Poor,gray
			colour = "cff9d9d9d";
		break;
		case 1: //Common,white
			colour = "cffffffff";
		break;
		case 2: //Uncommon,green
			colour = "cff1eff00";
		break;
		case 3: //Rare,blue
			colour = "cff0070dd";
		break;
		case 4: //Epic,purple
			colour = "cffa335ee";
		break;
		case 5: //Legendary,orange
			colour = "cffff8000";
		break;
		case 6: //Artifact,pale gold
			colour = "c00fce080";
		break;
		case 7: //Heirloom,pale gold
			colour = "c00fce080";
		break;
		default:
			colour = "cff9d9d9d";
	}
	
	// try to get localized version
	LocalizedItem *lit	= (language>0) ? sLocalizationMgr.GetLocalizedItem(iProto->ItemId, language) : NULL;

	if(lit)
		snprintf(buffer, 256, "|%s|Hitem:%u:0:0:0:0:0:0:0|h[%s]|h|r", colour.c_str(), iProto->ItemId, lit->Name);
	else
		snprintf(buffer, 256, "|%s|Hitem:%u:0:0:0:0:0:0:0|h[%s]|h|r", colour.c_str(), iProto->ItemId, iProto->Name1);
	

	ItemLink	= static_cast<const char*>(buffer);

	return ItemLink;
}

int32 GetStatScalingStatValueColumn(ItemPrototype *proto, uint32 type)
{
	switch(type){
		case SCALINGSTATSTAT:{ 
			if(proto->ScalingStatsFlag & 1)
				return 0;
			if(proto->ScalingStatsFlag & 2)
			    return 1;
			if(proto->ScalingStatsFlag & 4)
			    return 2;
			if(proto->ScalingStatsFlag & 8)
			    return 3;
			if(proto->ScalingStatsFlag & 16)
			    return 4;
			break; }

		case SCALINGSTATARMOR: { 
			if(proto->ScalingStatsFlag & 32)
				return 5;
			if(proto->ScalingStatsFlag & 64)
				return 6;
			if(proto->ScalingStatsFlag & 128)
				return 7;
			if(proto->ScalingStatsFlag & 256)
				return 8;
			break; }

		case SCALINGSTATDAMAGE: { 
			if(proto->ScalingStatsFlag & 512)
				return 9;
			if(proto->ScalingStatsFlag & 1024)
				return 10;
			if(proto->ScalingStatsFlag & 2048)
				return 11;
			if(proto->ScalingStatsFlag & 4096)
				return 12;
			if(proto->ScalingStatsFlag & 8192)
				return 13;
			if(proto->ScalingStatsFlag & 16384)
				return 14;
			break; }

		case SCALINGSTATSPELLPOWER: { 
			if(proto->ScalingStatsFlag & 32768)
				return 15;
			break; }
	}
	return 1;
}

uint32 Item::CountGemsWithLimitId(uint32 LimitId)
{
	uint32 result = 0;
	for( uint32 count = 0; count < GetSocketsCount(); count++ )
	{
		EnchantmentInstance *ei = GetEnchantment( SOCK_ENCHANTMENT_SLOT1 + count );
		if (ei 
			&& ei->Enchantment->GemEntry //huh ? Gem without entry ?
			)
		{
			ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ei->Enchantment->GemEntry);
			if( ip && ip->ItemLimitCategory == LimitId )
				result++;
		}
	}
    return result;
}

void Item::EventRemoveItem(){
    Arcemu::Util::ARCEMU_ASSERT(    this->GetOwner() != NULL );

    m_owner->GetItemInterface()->SafeFullRemoveItemByGuid( this->GetGUID() );
}

void Item::SendDurationUpdate(){
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  As of 3.1.3 the server sends this to set the actual durationtime ( the time the item exists for)
    //  of the item
    //
    //  {SERVER} Packet: (0x01EA) SMSG_ITEM_TIME_UPDATE PacketSize = 12 TimeStamp = 37339296
    //  05 76 83 E7 01 00 00 42 10 0E 00 00 
    //
    //  Structure:
    //
    //  uint64 GUID                      - the identifier of the item (not the itemid)
    //  uint32 remainingtime             - remaining duration
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    WorldPacket durationupdate( SMSG_ITEM_TIME_UPDATE, 12 );
    durationupdate << uint64( GetGUID() );
    durationupdate << uint32( GetItemExpireTime() - UNIXTIME );
    m_owner->SendPacket( &durationupdate );

}



// "Stackable items (such as Frozen Orbs and gems) and 
// charged items that can be purchased with an alternate currency are not eligible. "
bool Item::IsEligibleForRefund(){
    ItemPrototype *proto = this->GetProto();

    if( !(proto->Flags & ITEM_FLAG_REFUNDABLE) )
        return false;

    if( proto->MaxCount > 1 )
        return false;

    for( int i = 0; i < 5; ++i ){
        ItemSpell spell = proto->Spells[i];

        if( spell.Charges != -1  && spell.Charges != 0)
            return false;
    }

    return true;
}


void Item::RemoveFromRefundableMap(){
    Player *owner = NULL;
    uint64 GUID = 0;

    owner = this->GetOwner();
    GUID = this->GetGUID();

    if( owner != NULL && GUID != 0 )
        owner->GetItemInterface()->RemoveRefundable( GUID );
}

uint32 Item::RepairItemCost()
{
	DurabilityCostsEntry * dcosts = dbcDurabilityCosts.LookupEntryForced( m_itemProto->ItemLevel );
	if( dcosts == NULL )
	{
		sLog.outError("Repair: Unknown item level (%u)", dcosts);
		return 0;
	}

	DurabilityQualityEntry * dquality = dbcDurabilityQuality.LookupEntryForced( ( m_itemProto->Quality + 1 ) * 2);
	if( dquality == NULL )
	{
		sLog.outError("Repair: Unknown item quality (%u)", dquality);
		return 0;
	}

	uint32 dmodifier = dcosts->modifier[ m_itemProto->Class == ITEM_CLASS_WEAPON ? m_itemProto->SubClass : m_itemProto->SubClass + 21 ];
	uint32 cost = long2int32( ( GetDurabilityMax() - GetDurability() ) * dmodifier * double( dquality->quality_modifier ) );
	return cost;
}

bool Item::RepairItem(Player * pPlayer, bool guildmoney, int32 * pCost) //pCost is needed for the guild log
{
	//int32 cost = (int32)pItem->GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) - (int32)pItem->GetUInt32Value( ITEM_FIELD_DURABILITY );
	int32 cost = RepairItemCost();
	if( cost <= 0 )
		return false;
	if( guildmoney && pPlayer->IsInGuild() ) 
	{
		if( !pPlayer->GetGuildMember()->RepairItem((uint32)cost) )
			return false;//we should tell the client that he can't repair with the guild gold.
		if(  pCost != NULL )
			*pCost += cost;
	}
	else//we pay with our gold
	{
		if( !pPlayer->HasGold(cost) )
			return false;

		pPlayer->ModGold( -cost );
	}
	SetDurabilityToMax();
	m_isDirty = true;
	return true;
}