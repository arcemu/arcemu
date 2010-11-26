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

#define WATER_ELEMENTAL		510
#define WATER_ELEMENTAL_NEW 37994
#define PET_IMP				416
#define PET_VOIDWALKER		1860
#define PET_SUCCUBUS		1863
#define PET_FELHUNTER		417
#define PET_FELGUARD		17252
#define SHADOWFIEND			19668
#define SPIRITWOLF			29264
#define DANCINGRUNEWEAPON	27893

uint32 Pet::GetAutoCastTypeForSpell( SpellEntry * ent )
{
	switch( ent->NameHash )
	{
	/************************************************************************/
	/* Warlock Pet Spells													*/
	/************************************************************************/
	case SPELL_HASH_BLOOD_PACT:			// Blood Pact
	case SPELL_HASH_FEL_INTELLIGENCE:
	case SPELL_HASH_AVOIDANCE:
	case SPELL_HASH_PARANOIA:
		return AUTOCAST_EVENT_ON_SPAWN;
		break;

	case SPELL_HASH_FIRE_SHIELD:		// Fire Shield
		return AUTOCAST_EVENT_OWNER_ATTACKED;
		break;

	case SPELL_HASH_PHASE_SHIFT:		// Phase Shift
	case SPELL_HASH_CONSUME_SHADOWS:
	case SPELL_HASH_LESSER_INVISIBILITY:
		return AUTOCAST_EVENT_LEAVE_COMBAT;
		break;

	case SPELL_HASH_WAR_STOMP: // Doomguard spell
	case SPELL_HASH_SACRIFICE: // We don't want auto sacrifice :P
		return AUTOCAST_EVENT_NONE; 
		break;

	/************************************************************************/
	/* Hunter Pet Spells													*/
	/************************************************************************/
	case SPELL_HASH_THUNDERSTOMP:		// Thunderstomp
	case SPELL_HASH_FURIOUS_HOWL:		// Furious Howl
	case SPELL_HASH_DASH:				// Dash
	case SPELL_HASH_DIVE:				// Dive
	case SPELL_HASH_PROWL:				// Prowl
	case SPELL_HASH_SHELL_SHIELD:		// Shell Shield
		return AUTOCAST_EVENT_NONE;
		break;

	/************************************************************************/
	/* Mage Pet Spells														*/
	/************************************************************************/

	case SPELL_HASH_WATERBOLT:			// Waterbolt
		return AUTOCAST_EVENT_ATTACK;
		break;

	/************************************************************************/
	/* Shaman Pet Spells													*/
	/************************************************************************/
	case SPELL_HASH_SPIRIT_HUNT:
		return AUTOCAST_EVENT_ON_SPAWN;
		break;

	case SPELL_HASH_TWIN_HOWL:
	case SPELL_HASH_BASH:
		return AUTOCAST_EVENT_ATTACK;
		break;

	}
	return AUTOCAST_EVENT_ATTACK;
}

void Pet::SetNameForEntry( uint32 entry )
{
	switch( entry )
	{
		case WATER_ELEMENTAL:
		case WATER_ELEMENTAL_NEW:
			m_name = "Water Elemental";
			break;
		case SHADOWFIEND:
			m_name = "Shadowfiend";
			break;
		case SPIRITWOLF:
			m_name = "Spirit Wolf";
			break;
		case DANCINGRUNEWEAPON:
			m_name = "Rune Weapon";
			break;
		case PET_IMP:
		case PET_VOIDWALKER:
		case PET_SUCCUBUS:
		case PET_FELHUNTER:
		case PET_FELGUARD:
			{
				QueryResult* result = CharacterDatabase.Query("SELECT `name` FROM `playersummons` WHERE `ownerguid`=%u AND `entry`=%d", m_Owner->GetLowGUID(), entry);
				if( result )
				{
					m_name = result->Fetch()->GetString();
					delete result;
				}
				else // no name found, generate one and save it
				{
					m_name = sWorld.GenerateName();
					CharacterDatabase.Execute("INSERT INTO playersummons VALUES(%u, %u, '%s')",
						m_Owner->GetLowGUID(), entry, m_name.data() );
				}
			}break;
		default: m_name = sWorld.GenerateName();
	}
}

bool Pet::CreateAsSummon( uint32 entry, CreatureInfo *ci, Creature* created_from_creature, Player* owner, SpellEntry* created_by_spell, uint32 type, uint32 expiretime, LocationVector* Vec, bool dismiss_old_pet )
{
	if( ci == NULL || owner == NULL )
	{
		return false;//the caller will delete us.
	}
	
	if( dismiss_old_pet )
	{
		owner->DismissActivePets();
	}

	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	m_phase = m_Owner->GetPhase();
	m_PetNumber = m_Owner->GeneratePetNumber();
	creature_info = ci;
	myFamily = dbcCreatureFamily.LookupEntry( ci->Family );

	float x, y, z;
	if( Vec )
	{
		x = Vec->x;
		y = Vec->y;
		z = Vec->z;
	}
	else
	{
		x = owner->GetPositionX() + 2;
		y = owner->GetPositionY() + 2;
		z = owner->GetPositionZ();
	}

	// Create ourself
	Create( m_name.c_str(), owner->GetMapId(), x, y, z, owner->GetOrientation() );

	// Hunter pet should be max 5 levels below owner
	uint32 level = owner->getLevel();
	if( type & 0x2 && created_from_creature != NULL )
		level = created_from_creature->getLevel() < ( level - 5 ) ? level - 5 : created_from_creature->getLevel();

	SetEntry(  entry );
	setLevel(level );
	SetDisplayId(ci->Male_DisplayID );
	SetNativeDisplayId(ci->Male_DisplayID );
	EventModelChange();
	SetSummonedByGUID( owner->GetGUID() );
	SetCreatedByGUID( owner->GetGUID() );
	
    SetUInt32Value( UNIT_FIELD_BYTES_0, 2048 | (0 << 24) );


	SetBaseAttackTime(MELEE,2000 );
	SetBaseAttackTime(OFFHAND,2000 );
	SetFaction(owner->GetFaction( ) );
	SetCastSpeedMod(1.0f );	// better set this one

	if( type == 1 )
		Summon = true;

	if( created_from_creature == NULL )
	{
		SetNameForEntry( entry );
		if( created_by_spell != NULL )
			SetUInt64Value( UNIT_CREATED_BY_SPELL, created_by_spell->Id );

		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED );
		SetUInt32Value( UNIT_FIELD_BYTES_2, (0x01 | (0x28 << 8) | (0x2 << 24) ) );
		SetBoundingRadius(0.5f );
		SetCombatReach(0.75f );
		SetPowerType( POWER_TYPE_MANA );
	}
	else // Hunter pet
	{
		if( myFamily == NULL || myFamily->name == NULL )
			m_name = "Pet";
		else
			m_name.assign( myFamily->name );

		SetBoundingRadius(created_from_creature->GetBoundingRadius() );
		SetCombatReach(created_from_creature->GetCombatReach() );

		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT ); // why combat ??
		SetPower( POWER_TYPE_HAPPINESS, PET_HAPPINESS_UPDATE_VALUE >> 1 );	//happiness
		SetMaxPower( POWER_TYPE_HAPPINESS, 1000000 );
		SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, 0 );
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP( level ) );
		SetPower(POWER_TYPE_FOCUS, 100 );// Focus
		SetMaxPower( POWER_TYPE_FOCUS, 100 );
		SetUInt32Value( UNIT_FIELD_BYTES_2, 1  /* | (0x28 << 8) */ | (PET_RENAME_ALLOWED << 16) );// 0x3 -> Enable pet rename.
		SetPowerType(POWER_TYPE_FOCUS);
	}
    SetFaction(owner->GetFaction( ) );

    if( owner->IsPvPFlagged() )
        this->SetPvPFlag();
    else
        this->RemovePvPFlag();

    if( owner->IsFFAPvPFlagged() )
        this->SetFFAPvPFlag();
    else
        this->RemoveFFAPvPFlag();

	if( owner->IsSanctuaryFlagged() )
		this->SetSanctuaryFlag();
	else
		this->RemoveSanctuaryFlag();

    BaseDamage[0] = 0;
	BaseDamage[1] = 0;
	BaseOffhandDamage[0] = 0;
	BaseOffhandDamage[1] = 0;
	BaseRangedDamage[0] = 0;
	BaseRangedDamage[1] = 0;
	m_base_runSpeed = m_runSpeed = owner->m_base_runSpeed;
	m_base_walkSpeed = m_walkSpeed = owner->m_base_walkSpeed;

	ApplyStatsForLevel();

	m_ExpireTime = expiretime;
	bExpires = m_ExpireTime > 0 ? true : false;

	if( !bExpires && owner->IsPlayer() )
	{
		// Create PlayerPet struct (Rest done by UpdatePetInfo)
		PlayerPet *pp = new PlayerPet;
		pp->number = m_PetNumber;
		pp->stablestate = STABLE_STATE_ACTIVE;
		pp->spellid = created_by_spell ? created_by_spell->Id : 0;
		pp->alive = true;
		


		if( owner->getClass() == HUNTER )
			pp->type = HUNTERPET;
		else
			pp->type = WARLOCKPET;

		mPi = pp;
		owner->AddPlayerPet( pp, pp->number );
	}

	InitializeMe( true );
	return true;
}

Pet::Pet( uint64 guid ) : Creature( guid )
{
	m_PetXP = 0;
	Summon = false;
	memset(ActionBar, 0, sizeof(uint32)*10);
	ScheduledForDeletion = false;

	m_AutoCombatSpell = 0;

	m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;
	m_PetNumber = 0;

	m_State = PET_STATE_DEFENSIVE;
	m_Action = PET_ACTION_FOLLOW;
	m_ExpireTime = 0;
	bExpires = false;
	m_Diet = 0;
	reset_time = 0;
	reset_cost = 0;

	for( uint8 i = 0; i < AUTOCAST_EVENT_COUNT; i++ )
		m_autoCastSpells[i].clear();

	m_AISpellStore.clear();
	mSpells.clear();
	mPi = NULL;
}

Pet::~Pet()
{
	for( std::map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.begin(); itr != m_AISpellStore.end(); ++itr )
		delete itr->second;
	m_AISpellStore.clear();

	for( uint8 i = 0; i < AUTOCAST_EVENT_COUNT; i++ )
		m_autoCastSpells[i].clear();

	mSpells.clear();
}

void Pet::Update( uint32 time )
{
	if(Summon)
		Creature::Update( time ); // passthrough
	else
	{
		Unit::Update( time);//Dead Hunter's Pets should be despawned only if the Owner logs out or goes out of range.
		if(m_corpseEvent)
		{
			sEventMgr.RemoveEvents(this);
			m_corpseEvent = false;
		}
	}

	if( !Summon && !bExpires && isAlive() )
	{
		//ApplyPetLevelAbilities();
		//Happiness
		if( m_HappinessTimer == 0 )
		{
			int32 burn = 1042;		//Based on WoWWiki pet looses 50 happiness over 6 min => 1042 every 7.5 s
			if( CombatStatus.IsInCombat() )
				burn >>= 1;			//in combat reduce burn by half (guessed)
			ModPower(POWER_TYPE_HAPPINESS, -burn );
			m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;// reset timer
		}
		else if( !IsInBg() )
		{
			if( time > m_HappinessTimer )
				m_HappinessTimer = 0;
			else
				m_HappinessTimer -= time;
		}
	}

	if( bExpires )
	{
		if( m_ExpireTime == 0 )
		{
			// remove
			Remove( false, true );
			return;
		}
		else if( time > m_ExpireTime )
			m_ExpireTime = 0;
		else
			m_ExpireTime -= time;
	}
}

void Pet::SendSpellsToOwner()
{
	if( m_Owner == NULL )
		return;

	uint16 packetsize = ( GetEntry() != WATER_ELEMENTAL && GetEntry() != WATER_ELEMENTAL_NEW && GetEntry() != SPIRITWOLF ) ? ( ( uint16 )mSpells.size() * 4 + 59 ) : 62;
	WorldPacket * data = new WorldPacket( SMSG_PET_SPELLS, packetsize );
	*data << GetGUID();
	*data << uint16( myFamily != NULL ? myFamily->ID : 0 );	// pet family to determine talent tree
	*data << m_ExpireTime;
	*data << uint8( GetPetState() );	// 0x0 = passive, 0x1 = defensive, 0x2 = aggressive
	*data << uint8( GetPetAction() );	// 0x0 = stay, 0x1 = follow, 0x2 = attack
	*data << uint16( 0 );				// flags: 0xFF = disabled pet bar (eg. when pet stunned)

	// Send the actionbar
	for( uint8 i = 0; i < 10; ++i )
	{
		if(ActionBar[i] & 0x4000000)		// Commands
			*data << uint32( ActionBar[i] );
		else
		{
			if( ActionBar[i] )
				*data << uint16( ActionBar[i] ) << GetSpellState( ActionBar[i] );
			else
				*data << uint16(0) << uint8(0) << uint8(i+5);
		}
	}

	// we don't send spells for the water elemental so it doesn't show up in the spellbook
	if( GetEntry() != WATER_ELEMENTAL && GetEntry() != WATER_ELEMENTAL_NEW && GetEntry() != SPIRITWOLF )
	{
		// Send the rest of the spells.
		*data << uint8( mSpells.size() );
		for( PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr )
			*data << uint16( itr->first->Id ) << uint16( itr->second );
	}
	*data << uint8(0); 			// loop cycles
	/*in loop:
	*data << uint32(0x6010);	//spellid
	*data << uint64(0);			// unk */
	m_Owner->delayedPackets.add( data );
}

void Pet::SendNullSpellsToOwner()
{
	if( m_Owner == NULL || m_Owner->GetSession() == NULL )
		return;

	WorldPacket data(8);
	data.SetOpcode( SMSG_PET_SPELLS );
	data << uint64(0);
	m_Owner->GetSession()->SendPacket( &data );
}
void Pet::SendTalentsToOwner()
{
	if( m_Owner == NULL )
		return;

	WorldPacket data( SMSG_TALENTS_INFO, 50 );
	data << uint8( 1 );				// Pet talent packet identificator
	data << uint32( GetTPs() );		// Unspent talent points
	
	uint8 count = 0;
	size_t pos = data.wpos();
	data << uint8( 0 );				// Amount of known talents (will be filled later)
	
	CreatureFamilyEntry* cfe = dbcCreatureFamily.LookupEntryForced( GetCreatureInfo()->Family );
	if( !cfe || cfe->talenttree < 0 )
		return;

	// go through talent trees
	for( uint32 tte_id = PET_TALENT_TREE_START; tte_id <= PET_TALENT_TREE_END; tte_id++ )
    {
        TalentTabEntry * tte = dbcTalentTab.LookupEntryForced( tte_id );
        if( tte == NULL )
            continue;

		// check if we match talent tab
		if( !( tte->PetTalentMask & ( 1 << cfe->talenttree ) ) )
			continue;
		
		TalentEntry* te; 
		for( uint32 t_id = 1; t_id < dbcTalent.GetNumRows(); t_id++  )
		{
			// get talent entries for our talent tree
			te = dbcTalent.LookupRowForced( t_id );
			if( te == NULL || te->TalentTree != tte_id )
				continue;
			
			// check our spells
			for( uint8 j = 0; j < 5; j++ )
				if( te->RankID[ j ] > 0 && HasSpell( te->RankID[ j ] ) )
				{
					// if we have the spell, include it in packet
					data << te->TalentID;	// Talent ID
					data << j;				// Rank
					++count;
				}
		}
		// tab loaded, we can exit
		break;
	}
	// fill count of talents
	data.put< uint8 >( pos, count );

	// send the packet to owner
	if( m_Owner->GetSession() != NULL )
		m_Owner->GetSession()->SendPacket( &data );
}

void Pet::SendCastFailed( uint32 spellid, uint8 fail )
{
	if( m_Owner == NULL || m_Owner->GetSession() == NULL)
		return;

	WorldPacket data( SMSG_PET_CAST_FAILED, 6 );
	data << uint8( 0 );
	data << uint32( spellid );
	data << uint8( fail );
	m_Owner->GetSession()->SendPacket( &data );
}

void Pet::SendActionFeedback( PetActionFeedback value )
{
	if( m_Owner == NULL || m_Owner->GetSession() == NULL)
		return;
	m_Owner->GetSession()->OutPacket( SMSG_PET_ACTION_FEEDBACK, 1, &value );
}

void Pet::InitializeSpells()
{
	for( PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr )
	{
		SpellEntry *info = itr->first;

		// Check that the spell isn't passive
		if( info->Attributes & ATTRIBUTES_PASSIVE )
		{
			// Cast on self..
			Spell * sp = new Spell(this, info, true, false);
			SpellCastTargets targets( this->GetGUID() );
			sp->prepare( &targets );

			continue;
		}

		AI_Spell * sp = CreateAISpell( info );
		if( itr->second == AUTOCAST_SPELL_STATE )
			SetAutoCast( sp, true );
		else
			SetAutoCast( sp, false );
	}
}

AI_Spell * Pet::CreateAISpell(SpellEntry * info)
{
	Arcemu::Util::ARCEMU_ASSERT(    info != NULL );

	// Create an AI_Spell
	map<uint32,AI_Spell*>::iterator itr = m_AISpellStore.find( info->Id );
	if( itr != m_AISpellStore.end() )
		return itr->second;

	AI_Spell *sp = new AI_Spell;
	sp->agent = AGENT_SPELL;
	sp->entryId = GetEntry();
	sp->floatMisc1 = 0;
	sp->maxrange = GetMaxRange( dbcSpellRange.LookupEntry( info->rangeIndex ) );
	if( sp->maxrange < sqrt( info->base_range_or_radius_sqr ) )
		sp->maxrange = sqrt( info->base_range_or_radius_sqr );
	sp->minrange = GetMinRange( dbcSpellRange.LookupEntry( info->rangeIndex ) );
	sp->Misc2 = 0;
	sp->procChance = 0;
	sp->spell = info;
	sp->cooldown = objmgr.GetPetSpellCooldown(info->Id);
	if( sp->cooldown == 0 )
		sp->cooldown = info->StartRecoveryTime; //avoid spell spamming
	if( sp->cooldown == 0 )
		sp->cooldown = info->StartRecoveryCategory; //still 0 ?
	if( sp->cooldown == 0 )
		sp->cooldown = PET_SPELL_SPAM_COOLDOWN; //omg, avoid spamming at least
	sp->cooldowntime = 0;

	if( /* info->Effect[0] == SPELL_EFFECT_APPLY_AURA || */ info->Effect[0] == SPELL_EFFECT_APPLY_GROUP_AREA_AURA || info->Effect[0] == SPELL_EFFECT_APPLY_RAID_AREA_AURA )
		sp->spellType = STYPE_BUFF;
	else
		sp->spellType = STYPE_DAMAGE;

	sp->spelltargetType = static_cast<uint8>( info->ai_target_type );
	sp->autocast_type = GetAutoCastTypeForSpell( info );
	sp->procCount = 0;
	m_AISpellStore[ info->Id ] = sp;
	return sp;
}

void Pet::LoadFromDB( Player* owner, PlayerPet * pi )
{
	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	m_phase = m_Owner->GetPhase();
	mPi = pi;
	creature_info = CreatureNameStorage.LookupEntry( mPi->entry );
	proto = CreatureProtoStorage.LookupEntry( mPi->entry );
	myFamily = dbcCreatureFamily.LookupEntry( creature_info->Family );

	Create( pi->name.c_str(), owner->GetMapId(), owner->GetPositionX() + 2 , owner->GetPositionY() + 2, owner->GetPositionZ(), owner->GetOrientation() );

	m_PetNumber = mPi->number;
	m_PetXP = mPi->xp;
	m_name = mPi->name;
	Summon = false;
	SetEntry( mPi->entry );
	setLevel( mPi->level );


	m_HappinessTimer = mPi->happinessupdate;
	reset_time = mPi->reset_time;
	reset_cost = mPi->reset_cost;
    m_State = mPi->petstate;

	bExpires = false;

	// Setup actionbar
	if( mPi->actionbar.size() > 2 )
	{
		char * ab = strdup( mPi->actionbar.c_str() );
		char * p = strchr( ab, ',' );
		char * q = ab;
		uint32 spellid;
		uint32 spstate;
		uint32 i = 0;

		while( p && i < 10 )
		{
			*p = 0;

			if( sscanf( q, "%u %u", &spellid, &spstate ) != 2 )
				break;

			ActionBar[i] = spellid;
			//SetSpellState(dbcSpell.LookupEntry(spellid), spstate);
			if( !( ActionBar[i] & 0x4000000 ) && spellid )
				mSpells[ dbcSpell.LookupEntry( spellid ) ] = static_cast<unsigned short>( spstate );

			i++;

			q = p+1;
			p = strchr( q, ',' );
		}

		free(ab);
	}

	//Preventing overbuffs
	SetAttackPower(0 );
	SetAttackPowerMods(0 );
	SetBaseAttackTime(MELEE,2000 );
	SetBaseAttackTime(OFFHAND,2000 );
	SetCastSpeedMod(1.0f ); // better set this one


	SetUInt32Value( UNIT_FIELD_BYTES_0, 2048 | (0 << 24) );
	

	if( pi->type == WARLOCKPET ){
		SetNameForEntry( mPi->entry );
		SetUInt64Value( UNIT_CREATED_BY_SPELL, mPi->spellid );
		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED );
		SetUInt32Value( UNIT_FIELD_BYTES_2, (0x01 | (0x28 << 8) | (0x2 << 24) ) );
		SetBoundingRadius(0.5f );
		SetCombatReach(0.75f );
		SetPowerType( POWER_TYPE_MANA );
	}else{
		SetBoundingRadius( proto->BoundingRadius  );
		SetCombatReach( proto->CombatReach );
		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT ); // why combat ??
		SetPower( POWER_TYPE_HAPPINESS, PET_HAPPINESS_UPDATE_VALUE >> 1 );      //happiness
		SetMaxPower( POWER_TYPE_HAPPINESS, 1000000 );
		SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, 0 );
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP( mPi->level ) );
		SetUInt32Value( UNIT_FIELD_BYTES_2, 1 );
		SetPower(POWER_TYPE_FOCUS, 100 );// Focus
		SetMaxPower( POWER_TYPE_FOCUS, 100 );
		SetPowerType(POWER_TYPE_FOCUS);
	}


	BaseDamage[0] = 0;
	BaseDamage[1] = 0;
	BaseOffhandDamage[0] = 0;
	BaseOffhandDamage[1] = 0;
	BaseRangedDamage[0] = 0;
	BaseRangedDamage[1] = 0;
	m_base_runSpeed = m_runSpeed = owner->m_base_runSpeed;
	m_base_walkSpeed = m_walkSpeed = owner->m_base_walkSpeed;

	setLevel( mPi->level );

	
	SetDisplayId(creature_info->Male_DisplayID );
	SetNativeDisplayId(creature_info->Male_DisplayID );
	
	EventModelChange();
	
	SetSummonedByGUID( owner->GetGUID() );
	SetCreatedByGUID( owner->GetGUID() );
	SetCreatedBySpell( mPi->spellid );

	ApplyStatsForLevel();


	SetTPs( static_cast<uint8>( mPi->talentpoints ) );
	SetPower( GetPowerType(), mPi->current_power );
	SetHealth( mPi->current_hp );
	SetPower( POWER_TYPE_HAPPINESS, mPi->current_happiness );

	if( mPi->renamable == 0 )
		SetByte( UNIT_FIELD_BYTES_2, 2, PET_RENAME_NOT_ALLOWED );
	else
		SetByte( UNIT_FIELD_BYTES_2, 2, PET_RENAME_ALLOWED );

	//if pet was dead on logout then it should be dead now too.//we could use mPi->alive but this will break backward compatibility
	if( HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD ))//LoadFromDB() (called by Player::SpawnPet() ) now always revive the Pet if it was dead.
													//This is because now we call SpawnPet() only if it's alive or we wanna revive it.
	{
		SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0 );
		SetHealth( GetMaxHealth() );//this is modified (if required) in Spell::SpellEffectSummonDeadPet() 
		setDeathState( ALIVE );
	}

	InitializeMe( false );
}

void Pet::OnPushToWorld()
{
	//Pets MUST always have an owner
	Arcemu::Util::ARCEMU_ASSERT( m_Owner != NULL );
	//before we initialize pet spells so we can apply spell mods on them
	m_Owner->EventSummonPet( this );

	Creature::OnPushToWorld();
}

void Pet::InitializeMe( bool first )
{
	GetAIInterface()->Init( this, AITYPE_PET, MOVEMENTTYPE_NONE, m_Owner );
	GetAIInterface()->SetUnitToFollow( m_Owner );
	GetAIInterface()->SetFollowDistance( 3.0f );

	SetCreatureInfo( CreatureNameStorage.LookupEntry( GetEntry() ) );
	proto = CreatureProtoStorage.LookupEntry( GetEntry() );

	m_Owner->AddSummon( this );
    m_Owner->SetSummonedUnitGUID( GetGUID() );

	SetUInt32Value( UNIT_FIELD_PETNUMBER, GetUIdFromGUID() );
	SetUInt32Value( UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME );

	myFamily = dbcCreatureFamily.LookupEntry( GetCreatureInfo()->Family );

	SetPetDiet();
	_setFaction();

	// Load our spells
	if( Summon ) // Summons - always
	{
		// Adds parent +frost spell damage
		if( GetEntry() == WATER_ELEMENTAL || GetEntry() == WATER_ELEMENTAL_NEW )
		{
			float parentfrost = (float)m_Owner->GetDamageDoneMod(SCHOOL_FROST);
			parentfrost *= 0.40f;
			ModDamageDone[SCHOOL_FROST] = (uint32)parentfrost;
		}
		else if( GetEntry() == PET_IMP )
			m_aiInterface->disable_melee = true;
		else if( GetEntry() == PET_FELGUARD )
			SetEquippedItem(MELEE,12784 );

	}
	else if( first ) // Hunter pets - after taming
	{
		SetTPs( GetTPsForLevel( getLevel() ) );	// set talent points
	}
	else // Hunter pets - load from db
	{
		// Pull from database... :/
		QueryResult * query = CharacterDatabase.Query("SELECT * FROM playerpetspells WHERE ownerguid = %u AND petnumber = %u",
			m_Owner->GetLowGUID(), m_PetNumber );
		if( query )
		{
			do
			{
				Field * f = query->Fetch();
				SpellEntry* spell = dbcSpell.LookupEntryForced( f[2].GetUInt32() );
				uint16 flags = f[3].GetUInt16();
				if( spell != NULL && mSpells.find( spell ) == mSpells.end() )
					mSpells.insert( make_pair( spell, flags ) );

			} while( query->NextRow() );
		}
		delete query;
	}

    PushToWorld( m_Owner->GetMapMgr() );
	//we MUST be sure Pet was pushed to world.
	Arcemu::Util::ARCEMU_ASSERT( IsInWorld() );

	InitializeSpells();
	
	if( first )
	{
		// Set up default actionbar
		SetDefaultActionbar();
	}

	UpdateSpellList( false );
	SendSpellsToOwner();
	if( !Summon )
		SendTalentsToOwner();

	// set to active
	if( !bExpires )
		UpdatePetInfo( false );

	sEventMgr.AddEvent( this, &Pet::HandleAutoCastEvent, AUTOCAST_EVENT_ON_SPAWN,		EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	sEventMgr.AddEvent( this, &Pet::HandleAutoCastEvent, AUTOCAST_EVENT_LEAVE_COMBAT,	EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}


void Pet::UpdatePetInfo( bool bSetToOffline )
{
	if( bExpires || m_Owner == NULL ) 	// don't update expiring pets
		return;

	PlayerPet *pi = m_Owner->GetPlayerPet( m_PetNumber );
	pi->active = !bSetToOffline;
	pi->entry = GetEntry();
	std::stringstream ss;

	pi->name = GetName();
	pi->number = m_PetNumber;
	pi->xp = m_PetXP;
	pi->level = getLevel();
	pi->happinessupdate = m_HappinessTimer;

	// save actionbar
	ss.rdbuf()->str("");
	for(uint32 i = 0; i < 10; ++i)
	{
		if( ActionBar[i] & 0x4000000 )
			ss << ActionBar[i] << " 0";
		else if(ActionBar[i])
			ss << ActionBar[i] << " " << uint32(GetSpellState( ActionBar[i] ) );
		else
			ss << "0 0";

		ss << ",";
	}

	pi->actionbar = ss.str();
	pi->reset_cost = reset_cost;
	pi->reset_time = reset_time;
    pi->petstate = m_State;
	pi->alive = isAlive();
	pi->current_power = GetPower( GetPowerType() );
	pi->talentpoints = GetTPs();
	pi->current_hp = GetHealth();
	pi->current_happiness = GetPower( POWER_TYPE_HAPPINESS );
	
	uint32 renamable = GetByte( UNIT_FIELD_BYTES_2, 2 );

	if( renamable == PET_RENAME_ALLOWED )
		pi->renamable = 1;
	else
		pi->renamable = 0;
}

void Pet::Dismiss() //Abandon pet
{
	if( m_Owner && !bExpires )
		m_Owner->RemovePlayerPet( m_PetNumber ); // find playerpet entry and delete it

	Remove( false, true );
}

void Pet::Remove( bool bUpdate, bool bSetOffline )
{
	if( ScheduledForDeletion )
		return;
	ScheduledForDeletion = true;
	PrepareForRemove(bUpdate, bSetOffline);

	if( IsInWorld() )
		Unit::RemoveFromWorld( true );

	SafeDelete();
}

void Pet::RemoveFromWorld(bool free_guid)
{
	if(IsSummon())
		PrepareForRemove(false, true);
	else
		PrepareForRemove(true, false);
	Unit::RemoveFromWorld(free_guid);
}

void Pet::OnRemoveFromWorld()
{
	std::list<Pet*> ownerSummons = m_Owner->GetSummons();
	std::list<Pet*>::iterator itr;
	for(itr = ownerSummons.begin(); itr != ownerSummons.end(); ++itr)
	{
		//m_Owner MUST NOT have a reference to us anymore
		Arcemu::Util::ARCEMU_ASSERT( (*itr)->GetGUID() != GetGUID() );
	}
}

void Pet::Despawn(uint32 delay, uint32 respawntime)
{
	bool delayed = ( delay != 0 );
	DelayedRemove(delayed, true, delay);
}

void Pet::SafeDelete()
{
	sEventMgr.RemoveEvents(this);
	
	m_Owner->AddGarbagePet(this);
}

void Pet::DelayedRemove(bool bTime, bool dismiss, uint32 delay)
{
	if( ScheduledForDeletion )
			return;

	// called when pet has died
	if( bTime )
	{
		if( Summon || dismiss )
			Dismiss();  // remove us..
		else
			Remove( true, false );
	}
	else
		sEventMgr.AddEvent(this, &Pet::DelayedRemove, true, dismiss, uint32(0), EVENT_PET_DELAYED_REMOVE, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Pet::PrepareForRemove(bool bUpdate, bool bSetOffline)
{
	RemoveAllAuras(); // Prevent pet overbuffing
	m_Owner->EventDismissPet();

	if( bUpdate )
	{
		if( !bExpires )
			UpdatePetInfo( bSetOffline );
		if( !IsSummon() )
			m_Owner->_SavePet( NULL );
	}

	bool main_summon = m_Owner->GetSummon() == this ;
	m_Owner->RemoveSummon( this );

	if( m_Owner->GetSummon() == NULL )//we have no more summons, required by spells summoning more than 1.
	{
		m_Owner->SetSummonedUnitGUID( 0 );
		SendNullSpellsToOwner();
	}
	else if( main_summon )//we just removed the summon displayed in the portrait so we need to update it with another one.
	{
		m_Owner->SetSummonedUnitGUID( m_Owner->GetSummon()->GetGUID() );//set the summon still alive
		m_Owner->GetSummon()->SendSpellsToOwner();
	}

	if( IsInWorld() && IsActive() )
		Deactivate( m_mapMgr );
}

void Pet::setDeathState(DeathState s)
{
	Creature::setDeathState(s);
	if ( s == JUST_DIED && IsSummon() )
	{
		//we can't dimiss the summon now now since it's still needed in DealDamage()
		sEventMgr.AddEvent( this ,&Pet::Dismiss , EVENT_UNK, 1 , 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT ); 
	}
	if(mPi != NULL)
		mPi->alive = isAlive();
}

bool Pet::CanGainXP()
{
	// only hunter pets which are below owner level can gain experience
	if( Summon || !m_Owner || getLevel() >= m_Owner->getLevel() )
		return false;
	else
		return true;
}

void Pet::GiveXP( uint32 xp )
{
	xp += m_uint32Values[ UNIT_FIELD_PETEXPERIENCE ];
	uint32 nxp = m_uint32Values[ UNIT_FIELD_PETNEXTLEVELEXP ];

	if( xp >= nxp )
	{
		SetTPs( GetTPsForLevel( getLevel() + 1 ) - GetSpentTPs() );
		modLevel(1 );
		xp -= nxp;
		nxp = GetNextLevelXP( getLevel() );
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, nxp );
		ApplyStatsForLevel();
		UpdateSpellList();
		SendTalentsToOwner();
	}

	SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, xp );
}

uint32 Pet::GetNextLevelXP(uint32 level)
{
	// Pets need only 5% of xp to level up compared to players
	uint32 nextLvlXP = 0;
	if( level > 0 && level <= MAX_PREDEFINED_NEXTLEVELXP )
	{
		nextLvlXP = NextLevelXp[ level - 1 ];
	}
	else
	{
		nextLvlXP = ((int)((((double)(((8 * level) + ((level - 30) * 5)) * ((level * 5) + 45)))/100)+0.5))*100;
	}
	return nextLvlXP / 20;
}

void Pet::UpdateSpellList( bool showLearnSpells )
{
	// SkillLine 1
	uint32 s = 0;
	// SkillLine 2
	uint32 s2 = 0;

	if( GetCreatureInfo()->Family == 0 && Summon )
	{
		// Get spells from the owner
		map<uint32, set<uint32> >::iterator it1;
		set<uint32>::iterator it2;
		it1 = m_Owner->SummonSpells.find(GetEntry());
		if(it1 != m_Owner->SummonSpells.end())
		{
			it2 = it1->second.begin();
			for(; it2 != it1->second.end(); ++it2)
			{
				AddSpell( dbcSpell.LookupEntry( *it2 ), false, false );
			}
		}
		return;
	}
	else
	{
		// Get Creature family from DB (table creature_names, field family), load the skill line from CreatureFamily.dbc for use with SkillLineAbiliby.dbc entry
		CreatureFamilyEntry* f = dbcCreatureFamily.LookupEntryForced( GetCreatureInfo()->Family );
		if( f )
		{
			s = f->skilline;
			s2 = f->tameable;
		}
	}

	if( s || s2 )
	{
		skilllinespell* sls;
		uint32 rowcount = dbcSkillLineSpell.GetNumRows();
		SpellEntry* sp;
		for( uint32 idx = 0; idx < rowcount; ++idx )
		{
			sls = dbcSkillLineSpell.LookupRow( idx );
			// Update existing spell, or add new "automatic-acquired" spell
			if( (sls->skilline == s || sls->skilline == s2) && sls->acquireMethod == 2 )
			{
				sp = dbcSpell.LookupEntryForced( sls->spell );
				if( sp && getLevel() >= sp->baseLevel )
				{
					// Pet is able to learn this spell; now check if it already has it, or a higher rank of it
					bool addThisSpell = true;
					for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
					{
						if( (itr->first->NameHash == sp->NameHash) && (itr->first->RankNumber >= sp->RankNumber) )
						{
							// Pet already has this spell, or a higher rank. Don't add it.
							addThisSpell = false;
						}
					}
					if( addThisSpell )
					{
						AddSpell(sp, true, showLearnSpells);
					}
				}
			}
		}
	}
}

void Pet::AddSpell( SpellEntry * sp, bool learning, bool showLearnSpell )
{
	if( sp == NULL )
		return;

	// Cast on self if we're a passive spell
	if( sp->Attributes & ATTRIBUTES_PASSIVE )
	{
		if( IsInWorld() )
		{
			Spell * spell = new Spell(this, sp, true, NULL);
			SpellCastTargets targets(this->GetGUID());
			spell->prepare(&targets);
			mSpells[sp] = 0x0100;
		}
	}
	else
	{
	   // Active spell add to the actionbar.
		bool ab_replace = false;

		bool done=false;
		if( learning )
		{
			for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
			{
				if( sp->NameHash == itr->first->NameHash )
				{
					// replace the action bar
					for(int i = 0; i < 10; ++i)
					{
						if(ActionBar[i] == itr->first->Id)
						{
							ActionBar[i] = sp->Id;
							ab_replace = true;
							break;
						}
					}

					// Create the AI_Spell
					AI_Spell * asp = CreateAISpell(sp);

					// apply the spell state
					uint16 ss = GetSpellState(itr->first);
					mSpells[sp] = ss;
					if(ss==AUTOCAST_SPELL_STATE)
						SetAutoCast(asp, true);

					if( asp->autocast_type == AUTOCAST_EVENT_ON_SPAWN )
						CastSpell(this, sp, false);

					RemoveSpell(itr->first, showLearnSpell);
					done=true;
					break;
				}
			}
		}

		if( !ab_replace )
		{
			bool has=false;
			for(int i = 0; i < 10; ++i)
			{
				if( ActionBar[i] == sp->Id )
				{
					has=true;
					break;
				}
			}

			if( !has )
			{
				for(int i = 0; i < 10; ++i)
				{
					if( ActionBar[i] == 0 )
					{
						ActionBar[i] = sp->Id;
						break;
					}
				}
			}
		}

		if( done==false )
		{
			if( mSpells.find(sp) != mSpells.end() )
				return;

			if( learning )
			{
				AI_Spell * asp = CreateAISpell(sp);
				uint16 ss = (asp->autocast_type > 0) ? AUTOCAST_SPELL_STATE : DEFAULT_SPELL_STATE;
				mSpells[sp] = ss;
				if( ss == AUTOCAST_SPELL_STATE )
					SetAutoCast(asp,true);

				// Phase shift gets cast on spawn, right?
				if( asp->autocast_type == AUTOCAST_EVENT_ON_SPAWN || asp->spell->NameHash == SPELL_HASH_PHASE_SHIFT )
					CastSpell(this, sp, false);
			}
			else
				mSpells[sp] = DEFAULT_SPELL_STATE;
		}
	}

	if( showLearnSpell && m_Owner && m_Owner->GetSession() && !(sp->Attributes & ATTRIBUTES_NO_CAST) )
		m_Owner->GetSession()->OutPacket(SMSG_PET_LEARNED_SPELL, 2, &sp->Id);

	if( IsInWorld() )
		SendSpellsToOwner();
}

void Pet::SetSpellState(SpellEntry* sp, uint16 State)
{
	PetSpellMap::iterator itr = mSpells.find(sp);
	if(itr == mSpells.end())
		return;

	uint16 oldstate = itr->second;
	itr->second = State;

	if(State == AUTOCAST_SPELL_STATE || oldstate == AUTOCAST_SPELL_STATE)
	{
		AI_Spell * sp2 = GetAISpellForSpellId(sp->Id);
		if(sp2)
		{
			if(State == AUTOCAST_SPELL_STATE)
				SetAutoCast(sp2, true);
			else
				SetAutoCast(sp2,false);
		}
	}
}

uint16 Pet::GetSpellState(SpellEntry* sp)
{
	PetSpellMap::iterator itr = mSpells.find(sp);
	if(itr == mSpells.end())
		return DEFAULT_SPELL_STATE;

	return itr->second;
}

void Pet::SetDefaultActionbar()
{
	// Set up the default actionbar.
	ActionBar[0] = PET_SPELL_ATTACK;
	ActionBar[1] = PET_SPELL_FOLLOW;
	ActionBar[2] = PET_SPELL_STAY;

	// Fill up 4 slots with our spells
	if(mSpells.size() > 0)
	{
		PetSpellMap::iterator itr = mSpells.begin();
		uint32 pos = 0;
		for(; itr != mSpells.end() && pos < 4; ++itr, ++pos)
			ActionBar[3+pos] = itr->first->Id;
	}

	ActionBar[7] = PET_SPELL_AGRESSIVE;
	ActionBar[8] = PET_SPELL_DEFENSIVE;
	ActionBar[9] = PET_SPELL_PASSIVE;
}

void Pet::WipeTalents()
{
	uint32 rows, i, j;
	rows = dbcTalent.GetNumRows();
	for( i = 0; i < rows; i++ )
	{
		TalentEntry *te = dbcTalent.LookupRowForced( i );
		if( te == NULL || te->TalentTree < PET_TALENT_TREE_START || te->TalentTree > PET_TALENT_TREE_END ) // 409-Tenacity, 410-Ferocity, 411-Cunning
			continue;
		for( j = 0; j < 5; j++ )
			if( te->RankID[ j ] != 0 && HasSpell( te->RankID[ j ] ) )
				RemoveSpell( te->RankID[ j ] );
	}
	SendSpellsToOwner();
}

void Pet::RemoveSpell(SpellEntry * sp, bool showUnlearnSpell)
{
	mSpells.erase(sp);
	map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.find(sp->Id);
	if( itr != m_AISpellStore.end() )
	{
		if( itr->second->autocast_type != AUTOCAST_EVENT_NONE )
		{
			list<AI_Spell*>::iterator it3;
			for(list<AI_Spell*>::iterator it2 = m_autoCastSpells[itr->second->autocast_type].begin(); it2 != m_autoCastSpells[itr->second->autocast_type].end(); )
			{
				it3 = it2++;
				if( (*it3) == itr->second )
				{
					m_autoCastSpells[itr->second->autocast_type].erase(it3);
				}
			}
		}
		for(list<AI_Spell*>::iterator it = m_aiInterface->m_spells.begin(); it != m_aiInterface->m_spells.end(); ++it)
		{
			if((*it) == itr->second)
			{
				m_aiInterface->m_spells.erase(it);
				m_aiInterface->CheckNextSpell(itr->second);
				break;
			}
		}

		delete itr->second;
		m_AISpellStore.erase(itr);
	}
	else
	{
		for(list<AI_Spell*>::iterator it = m_aiInterface->m_spells.begin(); it != m_aiInterface->m_spells.end(); ++it)
		{
			if((*it)->spell == sp)
			{
				// woot?
				AI_Spell * spe = *it;
				m_aiInterface->m_spells.erase(it);
				delete spe;
				break;
			}
		}
	}
	//Remove spell from action bar as well
	for( uint32 pos = 0; pos < 10; pos++ )
	{
		if( ActionBar[pos] == sp->Id )
			ActionBar[pos] = 0;
	}

	if( showUnlearnSpell && m_Owner && m_Owner->GetSession() )
		m_Owner->GetSession()->OutPacket( SMSG_PET_UNLEARNED_SPELL, 4, &sp->Id );
}

void Pet::Rename( string NewName )
{
	m_name = NewName;
	// update petinfo
	UpdatePetInfo( false );

	// update timestamp to force a re-query
	SetUInt32Value( UNIT_FIELD_PET_NAME_TIMESTAMP, ( uint32 )UNIXTIME );

	// save new summoned name to db (.pet renamepet)
	if( m_Owner->getClass() == WARLOCK )
	{
		CharacterDatabase.Execute( "UPDATE `playersummons` SET `name`='%s' WHERE `ownerguid`=%u AND `entry`=%u",
			m_name.data(), m_Owner->GetLowGUID(), GetEntry() );
	}
}

void Pet::ApplySummonLevelAbilities()
{
	uint32 level = getLevel();
	double pet_level = (double)level;

	// Determine our stat index.
	int stat_index = -1;
	//float scale = 1;
	bool has_mana = true;

	switch( GetEntry() )
	{
	case PET_IMP:
		stat_index = 0;
		m_aiInterface->disable_melee = true;
		break;
	case PET_VOIDWALKER:
		stat_index = 1;
		break;
	case PET_SUCCUBUS:
		stat_index = 2;
		break;
	case PET_FELHUNTER:
		stat_index = 3;
		break;
	case 11859: // Doomguard
	case 89:	// Infernal
	case PET_FELGUARD:
		stat_index = 4;
		break;
	/*case 11859: // Doomguard
		stat_index = 4;
		break;
	case 89:	// Infernal
		stat_index = 5;
		has_mana = false;
		break;
	case 17252: // Felguard
		stat_index = 6;
		break;*/
	case WATER_ELEMENTAL:
	case WATER_ELEMENTAL_NEW:
		stat_index = 5;
		m_aiInterface->disable_melee = true;
		break;
	case SHADOWFIEND:
		stat_index = 5;
		break;
	case 26125:
		stat_index = 4;
		break;
	case 29264:
		stat_index = 5;
		break;
	case 27893:
		stat_index = 4;
		break;
	}
	if( GetEntry() == 89)
		has_mana = false;

	if(stat_index < 0)
	{
		sLog.outError("PETSTAT: No stat index found for entry %u, `%s`!", GetEntry(), GetCreatureInfo()->Name);
		return;
	}

	static double R_base_str[6] = {18.1884058, -15, -15, -15, -15, -15};
	static double R_mod_str[6] = {1.811594203, 2.4, 2.4, 2.4, 2.4, 2.4};
	static double R_base_agi[6] = {19.72463768, -1.25, -1.25, -1.25, -1.25, -1.25};
	static double R_mod_agi[6] = {0.275362319, 1.575, 1.575, 1.575, 1.575, 1.575};
	static double R_base_sta[6] = {18.82608695, -3.5, -17.75, -17.75, -17.75, 0};
	static double R_mod_sta[6] = {1.173913043, 4.05, 4.525, 4.525, 4.525, 4.044};
	static double R_base_int[6] = {19.44927536, 12.75, 12.75, 12.75, 12.75, 20};
	static double R_mod_int[6] = {4.550724638, 1.875, 1.875, 1.875, 1.875, 2.8276};
	static double R_base_spr[6] = {19.52173913, -2.25, -2.25, -2.25, -2.25, 20.5};
	static double R_mod_spr[6] = {3.47826087, 1.775, 1.775, 1.775, 1.775, 3.5};
	static double R_base_pwr[6] = {7.202898551, -101, -101, -101, -101, -101};
	static double R_mod_pwr[6] = {2.797101449, 6.5, 6.5, 6.5, 6.5, 6.5};
	static double R_base_armor[6] = {-11.69565217, -702, -929.4, -1841.25, -1157.55, 0};
	static double R_mod_armor[6] = {31.69565217, 139.6, 74.62, 89.175, 101.1316667, 20};
	static double R_pet_sta_to_hp[6] = {4.5, 15.0, 7.5, 10.0, 10.6, 7.5};
	static double R_pet_int_to_mana[6] = {15.0, 15.0, 15.0, 15.0, 15.0, 5.0};
	static double R_base_min_dmg[6] = {0.550724638, 4.566666667, 26.82, 29.15, 20.17888889, 20};
	static double R_mod_min_dmg[6] = {1.449275362, 1.433333333, 2.18, 1.85, 1.821111111, 1};
	static double R_base_max_dmg[6] = {1.028985507, 7.133333333, 36.16, 39.6, 27.63111111, 20};
	static double R_mod_max_dmg[6] = {1.971014493, 1.866666667, 2.84, 2.4, 2.368888889, 1.1};

	double base_str = R_base_str[stat_index];
	double mod_str = R_mod_str[stat_index];
	double base_agi = R_base_agi[stat_index];
	double mod_agi = R_mod_agi[stat_index];
	double base_sta = R_base_sta[stat_index];
	double mod_sta = R_mod_sta[stat_index];
	double base_int = R_base_int[stat_index];
	double mod_int = R_mod_int[stat_index];
	double base_spr = R_base_spr[stat_index];
	double mod_spr = R_mod_spr[stat_index];
	double base_pwr = R_base_pwr[stat_index];
	double mod_pwr = R_mod_pwr[stat_index];
	double base_armor = R_base_armor[stat_index];
	double mod_armor = R_mod_armor[stat_index];
	double base_min_dmg = R_base_min_dmg[stat_index];
	double mod_min_dmg = R_mod_min_dmg[stat_index];
	double base_max_dmg = R_base_max_dmg[stat_index];
	double mod_max_dmg = R_mod_max_dmg[stat_index];
	double pet_sta_to_hp = R_pet_sta_to_hp[stat_index];
	double pet_int_to_mana = R_pet_int_to_mana[stat_index];

	// Calculate bonuses
	double pet_str = base_str + pet_level * mod_str;
	double pet_agi = base_agi + pet_level * mod_agi;
	double pet_sta = base_sta + pet_level * mod_sta;
	double pet_int = base_int + pet_level * mod_int;
	double pet_spr = base_spr + pet_level * mod_spr;
	double pet_pwr = base_pwr + pet_level * mod_pwr;
	double pet_arm = base_armor + pet_level * mod_armor;

	// Calculate values
	BaseStats[STAT_STRENGTH] = (uint32)(pet_str);
	BaseStats[STAT_AGILITY] = (uint32)(pet_agi);
	BaseStats[STAT_STAMINA] = (uint32)(pet_sta);
	BaseStats[STAT_INTELLECT] = (uint32)(pet_int);
	BaseStats[STAT_SPIRIT] = (uint32)(pet_spr);

	double pet_min_dmg = base_min_dmg + pet_level * mod_min_dmg;
	double pet_max_dmg = base_max_dmg + pet_level * mod_max_dmg;
	BaseDamage[0] = float(pet_min_dmg);
	BaseDamage[1] = float(pet_max_dmg);

	// Apply attack power.
	SetAttackPower((uint32)(pet_pwr));

	BaseResistance[0] = (uint32)(pet_arm);
	CalcResistance(0);

	// Calculate health / mana
	double health = pet_sta * pet_sta_to_hp;
	double mana = has_mana ? (pet_int * pet_int_to_mana) : 0.0;
	if( health == 0 )
	{
		sLog.outError("Pet with entry %u has 0 health !!", GetEntry() );
		health = 100;
	}
	SetBaseHealth((uint32)(health));
	SetMaxHealth( (uint32)(health));
	SetBaseMana((uint32)(mana));
	SetMaxPower(POWER_TYPE_MANA, (uint32)(mana));

	for(uint32 x = 0; x < 5; ++x)
		CalcStat(x);
}

void Pet::ApplyPetLevelAbilities()
{
	uint32 pet_family = GetCreatureInfo()->Family;
	uint32 level = getLevel();
	if( level > PLAYER_LEVEL_CAP )
		level = PLAYER_LEVEL_CAP;
	else if (level < 1)
		level = 1;
	static uint32 family_aura[47] = { 0   /*0*/,
		17223 /*1*/, 17210 /*2*/, 17129 /*3*/, 17208 /*4*/, 7000  /*5*/, 17212 /*6*/, 17209 /*7*/, 17211 /*8*/, 17214 /*9*/, 0    /*10*/,
		17217/*11*/, 17220/*12*/, 0	   /*13*/, 0    /*14*/, 0    /*15*/, 0    /*16*/, 0    /*17*/, 0    /*18*/, 0    /*19*/, 17218/*20*/,
		17221/*21*/, 0    /*22*/, 0	   /*23*/, 17206/*24*/, 17215/*25*/, 17216/*26*/, 17222/*27*/, 0    /*28*/, 0    /*29*/, 34887/*30*/,
		35257/*31*/, 35254/*32*/, 35258/*33*/, 35253/*34*/, 35386/*35*/, 0    /*36*/, 50297/*37*/, 54642/*38*/, 54676/*39*/, 0    /*40*/,
		55192/*41*/, 55729/*42*/, 56634/*43*/, 56635/*44*/, 58598/*45*/, 61199/*46*/};

	if( pet_family < 47 )
		RemoveAura( family_aura[ pet_family ] );//If the pet gained a level, we need to remove the auras to re-calculate everything.
	LoadPetAuras(-1);//These too

	//Base attributes from http://petopia.brashendeavors.net/html/art...ttributes.shtml
	static uint32 R_pet_base_armor[PLAYER_LEVEL_CAP] = { 15, 16, 41, 76, 120, 174, 239, 316, 406, 512, 538, 573, 608, 642, 677, 713, 748, 782, 817, 852, 888, 922, 957, 992, 1026, 1061, 1097, 1130, 1165, 1200, 1234, 1270, 1304, 1340, 1373, 1480, 1593, 1709, 1834, 1964, 2101, 2246, 2397, 2557, 2725, 2780, 2835, 2888, 2944, 2999, 3052, 3108, 3163, 3216, 3271, 3327, 3380, 3435, 3489, 3791, 4091, 4391, 4691, 4991, 5291, 5591, 5892, 6192, 6492, 6792, 7092, 7392, 7692, 7992, 8292, 8592, 8892, 9192, 9492, 9792 };
	static uint32 R_pet_base_str[PLAYER_LEVEL_CAP] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 40, 42, 44, 45, 47, 49, 50, 52, 53, 55, 56, 58, 60, 61, 63, 64, 66, 67, 69, 70, 72, 74, 76, 78, 81, 86, 91, 97, 102, 104, 106, 108, 110, 113, 115, 117, 119, 122, 124, 127, 129, 131, 134, 136, 139, 141, 144, 146, 149, 151, 154, 156, 159, 162, 165, 168, 171, 174, 177, 181, 184, 187, 190, 193 };
	static uint32 R_pet_base_agi[PLAYER_LEVEL_CAP] = { 15, 16, 16, 16, 17, 18, 18, 19, 20, 20, 20, 21, 23, 23, 24, 25, 26, 27, 28, 30, 30, 30, 32, 33, 34, 35, 36, 37, 38, 40, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 55, 57, 57, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 82, 87, 92, 97, 102, 107, 112, 117, 122, 127, 131, 136, 141, 146, 151, 156, 161, 166, 171, 176 };
	static uint32 R_pet_base_sta[PLAYER_LEVEL_CAP] = { 22, 24, 25, 27, 28, 30, 32, 34, 36, 38, 40, 43, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 79, 83, 88, 93, 98, 103, 109, 114, 119, 124, 129, 134, 140, 146, 152, 158, 164, 170, 177, 183, 190, 196, 203, 210, 217, 224, 232, 240, 247, 255, 263, 271, 279, 288, 296, 305, 314, 323, 332, 342, 351, 361, 370, 380, 391, 401, 412, 423, 434, 445, 456, 467, 478, 489, 501, 512, 523, 534 };
	static uint32 R_pet_base_int[PLAYER_LEVEL_CAP] = { 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43 };
	static uint32 R_pet_base_spi[PLAYER_LEVEL_CAP] = { 20, 20, 21, 21, 21, 21, 22, 22, 22, 23, 23, 24, 25, 26, 27, 28, 29, 29, 31, 32, 32, 34, 34, 36, 37, 37, 39, 39, 41, 42, 42, 44, 44, 46, 47, 48, 49, 49, 51, 52, 53, 54, 55, 56, 58, 58, 60, 60, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75, 78, 80, 84, 86, 88, 91, 93, 95, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123, 126, 129 };

	BaseResistance[0] = R_pet_base_armor[ level - 1 ];
	BaseStats[0] = R_pet_base_str[ level - 1 ];
	BaseStats[1] = R_pet_base_agi[ level - 1 ];
	BaseStats[2] = R_pet_base_sta[ level - 1 ];
	BaseStats[3] = R_pet_base_int[ level - 1 ];
	BaseStats[4] = R_pet_base_spi[ level - 1 ];

	uint32 base_hp = BaseStats[2] * 10;
	SetBaseHealth(base_hp );
	SetUInt32Value( UNIT_FIELD_MAXHEALTH, base_hp );

	//Family Aura
	if( pet_family > 46 )
		sLog.outError( "PETSTAT: Creature family %i [%s] has missing data.", pet_family, myFamily->name );
	else if( family_aura[ pet_family ] != 0 )
		this->CastSpell( this, family_aura[ pet_family ], true );

	for( uint32 x = 0; x < 5; ++x )
		CalcStat( x );

	LoadPetAuras(-2);//Load all BM auras
}

void Pet::ApplyStatsForLevel()
{
	if( Summon )
		ApplySummonLevelAbilities();
	else
		ApplyPetLevelAbilities();

	// Apply common stuff
	// Apply scale for this family.
	if( myFamily != NULL )
	{
		float pet_level = float( getLevel() );
		float level_diff = float( myFamily->maxlevel - myFamily->minlevel );
		float scale_diff = float( myFamily->maxsize - myFamily->minsize );
		float factor = scale_diff / level_diff;
		float scale = factor * pet_level + myFamily->minsize;
		SetScale(  scale );
	}

	// Apply health fields.
	SetHealth(m_uint32Values[ UNIT_FIELD_MAXHEALTH ] );
	SetPower( POWER_TYPE_MANA, GetMaxPower(POWER_TYPE_MANA) ); // mana
	SetPower( POWER_TYPE_FOCUS, GetMaxPower(POWER_TYPE_FOCUS) ); // focus
}

void Pet::LoadPetAuras(int32 id)
{
	/*
	   Talent			   Aura Id
	Unleashed Fury			8875
	Thick Hide				19580
	Endurance Training		19581
	Bestial Swiftness		19582
	Bestial Discipline		19589
	Ferocity				19591
	Animal Handler			34666
	Catlike Reflexes		34667
	Serpent's Swiftness		34675
	*/

	static uint32 mod_auras[9] = { 8875, 19580, 19581, 19582, 19589, 19591, 34666, 34667, 34675 };//Beastmastery Talent's auras.
	InheritSMMods( m_Owner );

	if( id == -1 )//unload all
	{
		for( uint32 x = 0; x < 9; ++x )
			RemoveAura( mod_auras[x] );
	}
	else if( id == -2 )//load all
	{
		for( uint32 x = 0; x < 9; ++x )
			CastSpell( this, mod_auras[x], true );
	}
	else if( mod_auras[id] )//reload one
	{
		RemoveAura( mod_auras[id] );
		CastSpell( this, mod_auras[id], true );
	}
}

void Pet::UpdateAP()
{
	// Only hunter pets
	if( Summon )
		return;

	uint32 str = GetStat(STAT_STRENGTH);
	uint32 AP = ( str * 2 - 20 );
	if( m_Owner )
		AP += m_Owner->GetRAP() * 22 / 100;
	if( AP < 0 ) AP = 0;
	SetAttackPower(AP );
}

uint32 Pet::CanLearnSpell( SpellEntry * sp )
{
	// level requirement
	if( getLevel() < sp->spellLevel )
		return SPELL_FAILED_LEVEL_REQUIREMENT;

	return 0;
}
HappinessState Pet::GetHappinessState()
{
	//gets happiness state from happiness points
	uint32 pts = GetUInt32Value( UNIT_FIELD_POWER5 );
	if( pts < PET_HAPPINESS_UPDATE_VALUE )
		return UNHAPPY;
	else if( pts >= PET_HAPPINESS_UPDATE_VALUE << 1 )
		return HAPPY;
	else
		return CONTENT;
}

AI_Spell * Pet::HandleAutoCastEvent()
{
	list<AI_Spell*>::iterator itr,itr2;
	bool chance = true;
	uint32 size = 0;

	for(  itr2 = m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin(); itr2 != m_autoCastSpells[AUTOCAST_EVENT_ATTACK].end(); )
	{
		itr = itr2;
		itr2++;
		size = (uint32)m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].size();
		if( size > 1 )
			chance = Rand( 100.0f / size );

		if((*itr)->autocast_type == AUTOCAST_EVENT_ATTACK)
		{
			// spells still spammed, I think the cooldowntime is being set incorrectly somewhere else
			if( chance && (*itr)->spell && getMSTime() >= (*itr)->cooldowntime && //cebernic:crashfix
				GetPower((*itr)->spell->powerType) >= (*itr)->spell->manaCost )
			{
				return *itr;
			}
		}
		else
		{
			// bad pointers somehow end up here :S
			sLog.outError("Bad AI_Spell detected in AutoCastEvent!");
			m_autoCastSpells[AUTOCAST_EVENT_ATTACK].erase(itr);
		}
	}

	return NULL;
}

void Pet::HandleAutoCastEvent( AutoCastEvents Type )
{
	list<AI_Spell*>::iterator itr, it2;
	AI_Spell * sp;
	if( m_Owner == NULL )
		return;

	if( Type == AUTOCAST_EVENT_ATTACK )
	{
		if( m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].size() > 1 )
		{
			for( itr = m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].begin(); itr != m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].end(); itr++ )
			{
				if( itr == m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].end() )
				{
					if( getMSTime() >= (*itr)->cooldowntime )
						m_aiInterface->SetNextSpell(*itr);
					else
						return;
					break;
				}
				else
				{
					if( (*itr)->cooldowntime > getMSTime() )
						continue;

					m_aiInterface->SetNextSpell(*itr);
				}
			}
		}
		else if( m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].size() )
		{
			sp =*m_autoCastSpells[ AUTOCAST_EVENT_ATTACK ].begin();
			if( sp->cooldown && getMSTime() < sp->cooldowntime )
				return;

			m_aiInterface->SetNextSpell( sp );
		}

		return;
	}

	for( itr = m_autoCastSpells[ Type ].begin(); itr != m_autoCastSpells[ Type ].end(); )
	{
		it2 = itr++;
		sp = *it2;

		if( sp->spell == NULL )
		{
			sLog.outError("Found corrupted spell at m_autoCastSpells, skipping");
			continue;
		}
		else if( sp->autocast_type != static_cast<uint32>( Type ) )
		{
			sLog.outError("Found corrupted spell (%lu) at m_autoCastSpells, skipping", sp->entryId);
			continue;
		}

		if( sp->spelltargetType == TTYPE_OWNER )
		{
			if( !m_Owner->HasAura( sp->spell->Id ) )
				CastSpell( m_Owner, sp->spell, false );
		}
		else
		{
			//modified by Zack: Spell targeting will be generated in the castspell function now.You cannot force to target self all the time
			CastSpell( static_cast< Unit* >( NULL ), sp->spell, false);
		}
	}
}

void Pet::SetAutoCast(AI_Spell * sp, bool on)
{
	Arcemu::Util::ARCEMU_ASSERT(   sp != NULL);
	Arcemu::Util::ARCEMU_ASSERT(   sp->spell != NULL);

	if(sp->autocast_type > 0)
	{
		if(!on)
		{
			for(list<AI_Spell*>::iterator itr = m_autoCastSpells[sp->autocast_type].begin();
				itr != m_autoCastSpells[sp->autocast_type].end(); ++itr)
			{
				if( (*itr) == sp )
				{
					m_autoCastSpells[sp->autocast_type].erase(itr);
					break;
				}
			}
		}
		else
		{
			for(list<AI_Spell*>::iterator itr = m_autoCastSpells[sp->autocast_type].begin();
				itr != m_autoCastSpells[sp->autocast_type].end(); ++itr)
			{
				if((*itr) == sp)
					return;
			}

			m_autoCastSpells[sp->autocast_type].push_back(sp);
		}
	}
}
uint32 Pet::GetUntrainCost()
{
	uint32 days = (uint32)( sWorld.GetGameTime() - reset_time ) / 60 * 60 * 24;

	if( reset_cost < 1000 || days > 0 )
		reset_cost = 1000;
	else if( reset_cost < 5000 )
		reset_cost = 5000;
	else if( reset_cost < 10000 )
		reset_cost = 10000;
	else
		reset_cost = reset_cost + 10000 > 100000 ? 100000 : reset_cost + 10000;

	return reset_cost;
}

Group *Pet::GetGroup()
{
	if( m_Owner )
		return m_Owner->GetGroup();
	return NULL;
}


void Pet::DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras ){
	if( !pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld() )
		return;
	if( pVictim->IsPlayer() && static_cast< Player* >( pVictim )->GodModeCheat == true )
		return;
	if( pVictim->bInvincible )
		return;
	if( pVictim->IsSpiritHealer() )
		return;

	if( pVictim != this )
		CombatStatus.OnDamageDealt( pVictim );

	pVictim->SetStandState( STANDSTATE_STAND );

	if( pVictim->IsPvPFlagged() ){
		if( !IsPvPFlagged() )
			m_Owner->PvPToggle();

		m_Owner->AggroPvPGuards();
	}

	// Bg dmg counter
	if( pVictim != this ){
		if( m_Owner->m_bg != NULL && GetMapMgr() == pVictim->GetMapMgr() ){
			m_Owner->m_bgScore.DamageDone += damage;
			m_Owner->m_bg->UpdatePvPData();
		}
	}

	// Duel
	if( pVictim->IsPlayer() && m_Owner->DuelingWith != NULL && m_Owner->DuelingWith->GetGUID() == pVictim->GetGUID() ){
		if( pVictim->GetHealth() <= damage ){
			uint32 NewHP = pVictim->GetMaxHealth()/100;

			if(NewHP < 5)
				NewHP = 5;

			pVictim->SetHealth( NewHP );
			m_Owner->EndDuel(DUEL_WINNER_KNOCKOUT);
			pVictim->Emote(EMOTE_ONESHOT_BEG);
			return;
		}
	}

	if( pVictim->GetHealth() <= damage ){

		if( m_Owner->m_bg != NULL ){
			m_Owner->m_bg->HookOnUnitKill( m_Owner, pVictim );

			if( pVictim->IsPlayer() )
				m_Owner->m_bg->HookOnPlayerKill( m_Owner, static_cast< Player* >( pVictim ) );
		}

		if( pVictim->IsPlayer() ){

			sHookInterface.OnKillPlayer( m_Owner, static_cast< Player* >( pVictim ) );
			
			bool setAurastateFlag = false;
			
			if( m_Owner->getLevel() >= ( pVictim->getLevel() - 8) && ( GetGUID() != pVictim->GetGUID() ) ){

#ifdef ENABLE_ACHIEVEMENTS
				m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, m_Owner->GetAreaID(), 1, 0);
				m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL, 1, 0, 0);
#endif
				HonorHandler::OnPlayerKilledUnit( m_Owner, pVictim );
				setAurastateFlag = true;

				}

			if ( setAurastateFlag ){
				SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_LASTKILLWITHHONOR);
				
				if( !sEventMgr.HasEvent( m_Owner,EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE ) )
					sEventMgr.AddEvent( static_cast< Unit* >( m_Owner ), &Unit::EventAurastateExpire, static_cast< uint32 >( AURASTATE_FLAG_LASTKILLWITHHONOR ),EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE,20000,1,0);
				else
					sEventMgr.ModifyEventTimeLeft( m_Owner,EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE,20000);

				}
		}else{
			if( pVictim->IsCreature() ){
				m_Owner->Reputation_OnKilledUnit( pVictim, false );

#ifdef ENABLE_ACHIEVEMENTS
				m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILLING_BLOW, GetMapId(), 0, 0);
#endif

			}
		}

		if( pVictim->IsPvPFlagged() ){
			uint32 team = m_Owner->GetTeam();

			if( team == TEAM_ALLIANCE )
				team = TEAM_HORDE;
			else
				team = TEAM_ALLIANCE;
			
			uint32 AreaID = pVictim->GetMapMgr()->GetAreaID(pVictim->GetPositionX(), pVictim->GetPositionY());
			
			if( AreaID == 0 )
				AreaID = GetZoneId(); // Failsafe for a shitty TerrainMgr
			
			if(AreaID){
				sWorld.SendZoneUnderAttackMsg( AreaID, static_cast< uint8 >( team ) );
			}
		}

		pVictim->Die( this, damage, spellId );

///////////////////////////////////////////////////////////// Loot  //////////////////////////////////////////////////////////////////////////////////////////////
		
		if( pVictim->isLootable() ){
			Player *tagger = GetMapMgr()->GetPlayer( Arcemu::Util::GUID_LOPART( pVictim->GetTaggerGUID() ) );

			// Tagger might have left the map so we need to check
			if( tagger != NULL ){
				if( tagger->InGroup() )
					tagger->GetGroup()->SendLootUpdates( pVictim );
				else
					tagger->SendLootUpdate( pVictim);
			}
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////// Experience /////////////////////////////////////////////////////////////////////////////////////////////

		if ( pVictim->GetCreatedByGUID() == 0 && !pVictim->IsPet() && pVictim->IsTagged() ){
				Unit *uTagger = pVictim->GetMapMgr()->GetUnit( pVictim->GetTaggerGUID() ); 

				if (uTagger != NULL && uTagger->IsPlayer() ){
					Player *pTagger = TO_PLAYER(uTagger);
					
					if (pTagger != NULL ){
						
						if (pTagger->InGroup())
							pTagger->GiveGroupXP( pVictim, pTagger);
						else if( IsUnit() ){
							uint32 xp = CalculateXpToGive( pVictim, uTagger );
							
							if( xp > 0 ){
								pTagger->GiveXP( xp, pVictim->GetGUID(), true );

								SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_LASTKILLWITHHONOR);

								if(!sEventMgr.HasEvent(this,EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE))
									sEventMgr.AddEvent(TO_UNIT(this),&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_LASTKILLWITHHONOR,EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE,20000,1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
								else
									sEventMgr.ModifyEventTimeLeft(this,EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE,20000);
								
								// let's give the pet some experience too
								if( pTagger->GetSummon() && pTagger->GetSummon()->CanGainXP() ){
									xp = CalculateXpToGive( pVictim, pTagger->GetSummon() );
									
									if( xp > 0 )
										pTagger->GetSummon()->GiveXP( xp );
								}
							}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if(pVictim->IsCreature())
							{
								sQuestMgr.OnPlayerKill( pTagger, TO_CREATURE( pVictim ), true );

///////////////////////////////////////////////// Kill creature/creature type Achievements /////////////////////////////////////////////////////////////////////
#ifdef ENABLE_ACHIEVEMENTS
								if(pTagger->InGroup()){
									Group *pGroup = pTagger->GetGroup();
									
									pGroup->UpdateAchievementCriteriaForInrange( pVictim, ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, pVictim->GetEntry(), 1, 0 );
									pGroup->UpdateAchievementCriteriaForInrange( pVictim, ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE, GetHighGUID(), GetLowGUID(), 0 );

								}else{
									pTagger->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, pVictim->GetEntry(), 1, 0);
									pTagger->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE, GetHighGUID(), GetLowGUID(), 0);
								}
#endif
							}
						}
					}
				}
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_ACHIEVEMENTS

		if( pVictim->isCritter() ){
			m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, pVictim->GetEntry(), 1, 0);
			m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE, GetHighGUID(), GetLowGUID(), 0);
		}

#endif


	}else{
		pVictim->TakeDamage( this, damage, spellId, no_remove_auras );
	}
}

void Pet::TakeDamage(Unit *pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras )
{
	if( !no_remove_auras )
	{
		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if( spellid )
		{
			RemoveAurasByInterruptFlagButSkip( AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN, spellid );
			if( Rand( 35.0f ) )
				RemoveAurasByInterruptFlagButSkip( AURA_INTERRUPT_ON_UNUSED2, spellid );
		}
		else
		{
			RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN );
			if( Rand( 35.0f ) )
				RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_UNUSED2 );
		}
	}

	GetAIInterface()->AttackReaction( pAttacker, damage, spellid );

	ModHealth( -1 * static_cast< int32 >( damage ) );
}

void Pet::Die( Unit *pAttacker, uint32 damage, uint32 spellid ){
	//general hook for die
	if( !sHookInterface.OnPreUnitDie( pAttacker, this) )
		return;

	// on die and an target die proc
	{
		SpellEntry *killerspell;
		if( spellid )
			killerspell = dbcSpell.LookupEntry( spellid );
		else killerspell = NULL;
		
		HandleProc( PROC_ON_DIE, this, killerspell );
		m_procCounter = 0;
		pAttacker->HandleProc( PROC_ON_TARGET_DIE, this, killerspell );
		pAttacker->m_procCounter = 0;
	}

	setDeathState( JUST_DIED );
	GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, this, 0);

	if( GetChannelSpellTargetGUID() != 0 ){
		
		Spell *spl = GetCurrentSpell();
		
		if( spl != NULL ){
			
			for(int i = 0; i < 3; i++){
				if(spl->GetProto()->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA){
					uint64 guid = GetChannelSpellTargetGUID();
					DynamicObject *dObj = GetMapMgr()->GetDynamicObject( Arcemu::Util::GUID_LOPART( guid ) );
					if(!dObj)
						return;
					
					dObj->Remove();
				}
			}

			if( spl->GetProto()->ChannelInterruptFlags == 48140) spl->cancel();
		}
	}

	/* Stop players from casting */
	for( std::set< Object* >::iterator itr = GetInRangePlayerSetBegin() ; itr != GetInRangePlayerSetEnd() ; itr ++ ){
		Unit *attacker = static_cast< Unit* >( *itr );
		
		if( attacker->GetCurrentSpell() != NULL){
			if ( attacker->GetCurrentSpell()->m_targets.m_unitTarget == GetGUID())
				attacker->GetCurrentSpell()->cancel();
		}
	}

	smsg_AttackStop( this );
	SetHealth( 0 );

	// Wipe our attacker set on death
	CombatStatus.Vanished();
	
	CALL_SCRIPT_EVENT( pAttacker, OnTargetDied )( this );
	pAttacker->smsg_AttackStop( this );
	
	/* Tell Unit that it's target has Died */
	pAttacker->addStateFlag( UF_TARGET_DIED );

	GetAIInterface()->OnDeath(pAttacker);
	
	{/* ----------------------------- PET DEATH HANDLING -------------- */
		Pet* pPet = this;
		
		// dying pet looses 1 happiness level (not in BG)
		if( !pPet->IsSummon() && !pPet->IsInBg() ){
			uint32 hap = pPet->GetPower( POWER_TYPE_HAPPINESS );
			hap = hap - PET_HAPPINESS_UPDATE_VALUE > 0 ? hap - PET_HAPPINESS_UPDATE_VALUE : 0;
			pPet->SetPower( POWER_TYPE_HAPPINESS, hap );
		}
		pPet->DelayedRemove( false );
	}/* ----------------------------- PET DEATH HANDLING END -------------- */
}



