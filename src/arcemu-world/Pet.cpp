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

#define WATER_ELEMENTAL	510
#define PET_IMP			416
#define PET_VOIDWALKER	1860
#define PET_SUCCUBUS	1863
#define PET_FELHUNTER	417
#define PET_FELGUARD	17252
#define SHADOWFIEND		19668
#define SPIRITWOLF		29264
#define DANCINGRUNEWEAPON 27893

uint32 GetAutoCastTypeForSpell( SpellEntry * ent )
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

void Pet::CreateAsSummon( uint32 entry, CreatureInfo *ci, Creature* created_from_creature, Player* owner, SpellEntry* created_by_spell, uint32 type, uint32 expiretime, LocationVector* Vec )
{
	if( !ci || !owner )
	{
		sEventMgr.AddEvent( this, &Pet::PetSafeDelete, EVENT_CREATURE_SAFE_DELETE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
		return;
	}

	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
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
	uint32 level = owner->GetUInt32Value( UNIT_FIELD_LEVEL );
	if( type & 0x2 && created_from_creature != NULL )
		level = created_from_creature->getLevel() < ( level - 5 ) ? level - 5 : created_from_creature->getLevel();

	SetUInt32Value( OBJECT_FIELD_ENTRY, entry );
	SetUInt32Value( UNIT_FIELD_LEVEL, level );
	SetUInt32Value( UNIT_FIELD_DISPLAYID, ci->Male_DisplayID );
	SetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID, ci->Male_DisplayID );
	EventModelChange();
	SetUInt64Value( UNIT_FIELD_SUMMONEDBY, owner->GetGUID() );
	SetUInt64Value( UNIT_FIELD_CREATEDBY, owner->GetGUID() );
	SetUInt32Value( UNIT_FIELD_BYTES_0, 2048 | (0 << 24) );
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 2000 );
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME+1, 2000 );
	SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE ) );
	SetFloatValue( UNIT_MOD_CAST_SPEED, 1.0f );	// better set this one

	if( type & 0x1 || created_from_creature == NULL )
	{
		Summon = true;
		SetNameForEntry( entry );
		if( created_by_spell != NULL )
			SetUInt64Value( UNIT_CREATED_BY_SPELL, created_by_spell->Id );

		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED );
		SetUInt32Value( UNIT_FIELD_BYTES_2, (0x01 | (0x28 << 8) | (0x2 << 24) ) );
		SetFloatValue( UNIT_FIELD_BOUNDINGRADIUS, 0.5f );
		SetFloatValue( UNIT_FIELD_COMBATREACH, 0.75f );
		SetPowerType( POWER_TYPE_MANA );
	}
	else // Hunter pet
	{
		if( myFamily == NULL || myFamily->name == NULL )
			m_name = "Pet";
		else
			m_name.assign( myFamily->name );

		SetFloatValue( UNIT_FIELD_BOUNDINGRADIUS, created_from_creature->GetFloatValue( UNIT_FIELD_BOUNDINGRADIUS ) );
		SetFloatValue( UNIT_FIELD_COMBATREACH, created_from_creature->GetFloatValue( UNIT_FIELD_COMBATREACH ) );

		SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT ); // why combat ??
		SetUInt32Value( UNIT_FIELD_POWER5, PET_HAPPINESS_UPDATE_VALUE >> 1 );	//happiness
		SetUInt32Value( UNIT_FIELD_MAXPOWER5, 1000000 );
		SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, 0 );
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP( level ) );
		SetUInt32Value( UNIT_FIELD_POWER3, 100 );// Focus
		SetUInt32Value( UNIT_FIELD_MAXPOWER3, 100 );
		SetUInt32Value( UNIT_FIELD_BYTES_2, 1 | (0x28 << 8) | (0x3 << 16) );// 0x3 -> Enable pet rename.
		SetPowerType( POWER_TYPE_FOCUS);
	}

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

	if( !bExpires )
	{
		// Create PlayerPet struct (Rest done by UpdatePetInfo)
		PlayerPet *pp = new PlayerPet;
		pp->number = m_PetNumber;
		pp->stablestate = STABLE_STATE_ACTIVE;
		pp->spellid = created_by_spell ? created_by_spell->Id : 0;
		owner->AddPlayerPet( pp, pp->number );
	}

	InitializeMe( true );
}

Pet::Pet( uint64 guid ) : Creature( guid )
{
	m_isPet = true;
	m_PetXP = 0;
	Summon = false;
	memset(ActionBar, 0, sizeof(uint32)*10);

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
	Creature::Update( time ); // passthrough

	if( !Summon && !bExpires && isAlive() )
	{
		//ApplyPetLevelAbilities();
		//Happiness
		if( m_HappinessTimer == 0 )
		{
			int32 burn = 1042;		//Based on WoWWiki pet looses 50 happiness over 6 min => 1042 every 7.5 s
			if( CombatStatus.IsInCombat() )
				burn >>= 1;			//in combat reduce burn by half (guessed)
			ModUnsigned32Value( UNIT_FIELD_POWER5, -burn );
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

	uint16 packetsize = ( GetEntry() != WATER_ELEMENTAL && GetEntry() != SPIRITWOLF ) ? ( ( uint16 )mSpells.size() * 4 + 59 ) : 62;
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
	if( GetEntry() != WATER_ELEMENTAL && GetEntry() != SPIRITWOLF )
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
	if( m_Owner == NULL || m_Owner->GetSession() == NULL)
		return;

	WorldPacket data(8);
	data.SetOpcode( SMSG_PET_SPELLS );
	data << uint64(0);
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
			Spell * sp = SpellPool.PooledNew();
			if (!sp)
				return;
			sp->Init(this, info, true, false);
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
	ASSERT( info != NULL );

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

	if( info->Effect[0] == SPELL_EFFECT_APPLY_AURA || info->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA || info->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA2 )
		sp->spellType = STYPE_BUFF;
	else
		sp->spellType = STYPE_DAMAGE;

	sp->spelltargetType = info->ai_target_type;
	sp->autocast_type = GetAutoCastTypeForSpell( info );
	sp->procCount = 0;
	m_AISpellStore[ info->Id ] = sp;
	return sp;
}

void Pet::LoadFromDB( Player* owner, PlayerPet * pi )
{
	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	mPi = pi;
	creature_info = CreatureNameStorage.LookupEntry( mPi->entry );

	Create( pi->name.c_str(), owner->GetMapId(), owner->GetPositionX() + 2 , owner->GetPositionY() + 2, owner->GetPositionZ(), owner->GetOrientation() );

	LoadValues( mPi->fields.c_str() );

	m_PetNumber = mPi->number;
	m_PetXP = mPi->xp;
	m_name = mPi->name;
	Summon = mPi->summon;
	m_HappinessTimer = mPi->happinessupdate;
	reset_time = mPi->reset_time;
	reset_cost = mPi->reset_cost;

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
				mSpells[ dbcSpell.LookupEntry( spellid ) ] = spstate;

			i++;

			q = p+1;
			p = strchr( q, ',' );
		}

		free(ab);
	}

	//Preventing overbuffs
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, 0 );
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER_MODS, 0 );
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 2000 );

	if( m_Owner )
	{
		uint32 level = getLevel();
		if( level < m_Owner->getLevel() - 5 )
			level = m_Owner->getLevel() - 5;
		else if( level > m_Owner->getLevel() )
			level = m_Owner->getLevel();

		if( getLevel() != level )
		{
			if( !Summon )
				SetTPs( GetTPsForLevel( level) - GetSpentTPs() );

			SetUInt32Value( UNIT_FIELD_LEVEL, level );
			SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, 0 );
			SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP( level ) );
			ApplyStatsForLevel();
		}
		else if( Summon )
			ApplySummonLevelAbilities();
		else
			ApplyPetLevelAbilities();
	}

	InitializeMe( false );

	//if pet was dead on logout then it should be dead now too
	if( HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD ) )
	{
		SetUInt32Value(UNIT_FIELD_HEALTH, 0); //this is probably already 0
		setDeathState( JUST_DIED );
		m_Owner->EventDismissPet();	//just in case in near future(or any other) on loading auras get applied then make sure we remove those
	}
}

void Pet::OnPushToWorld()
{
	//before we initialize pet spells so we can apply spell mods on them
	if( m_Owner )
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

	m_Owner->SetSummon( this );
	m_Owner->SetUInt64Value( UNIT_FIELD_SUMMON, GetGUID() );

	SetUInt32Value( UNIT_FIELD_PETNUMBER, GetUIdFromGUID() );
	SetUInt32Value( UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME );

	if( GetCreatureInfo() )
		myFamily = dbcCreatureFamily.LookupEntry( GetCreatureInfo()->Family );
	else myFamily = NULL;

	SetInstanceID( m_Owner->GetInstanceID() );
	SetPetDiet();
	_setFaction();

	// Load our spells
	if( Summon ) // Summons - always
	{
		// Adds parent +frost spell damage
		if( GetEntry() == WATER_ELEMENTAL )
		{
			float parentfrost = (float)m_Owner->GetDamageDoneMod(SCHOOL_FROST);
			parentfrost *= 0.40f;
			ModDamageDone[SCHOOL_FROST] = (uint32)parentfrost;
		}
		else if( GetEntry() == PET_IMP )
			m_aiInterface->disable_melee = true;
		else if( GetEntry() == PET_FELGUARD )
			SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, 12784 );

	}
	else if( first ) // Hunter pets - after taming
	{
		SetTPs( GetTPsForLevel( getLevel() ) );	// set talent points
	}
	else // Hunter pets - load from db
	{
		// Pull from database... :/
		QueryResult * query = CharacterDatabase.Query("SELECT * FROM playerpetspells WHERE ownerguid=%u and petnumber=%u",
			m_Owner->GetLowGUID(), m_PetNumber );
		if( query )
		{
			do
			{
				Field * f = query->Fetch();
				SpellEntry* spell = dbcSpell.LookupEntry( f[2].GetUInt32() );
				uint16 flags = f[3].GetUInt16();
				if( spell != NULL && mSpells.find( spell ) == mSpells.end() )
					mSpells.insert( make_pair( spell, flags ) );

			} while( query->NextRow() );
		}
		delete query;
	}

	InitializeSpells();
	PushToWorld( m_Owner->GetMapMgr() );

	if( first )
	{
		// Set up default actionbar
		SetDefaultActionbar();
	}

	UpdateSpellList( false );
	SendSpellsToOwner();

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
	for( uint32 index = 0; index < UNIT_END; index ++ )
	{
		ss << GetUInt32Value(index) << " ";
	}
	pi->fields = ss.str();
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
	pi->summon = Summon;
	pi->reset_cost = reset_cost;
	pi->reset_time = reset_time;
}

void Pet::Dismiss() //Abandon pet
{
	if( m_Owner && !bExpires )
		m_Owner->RemovePlayerPet( m_PetNumber ); // find playerpet entry and delete it

	Remove( false, true );
}

void Pet::Remove( bool bUpdate, bool bSetOffline )
{
	RemoveAllAuras(); // Prevent pet overbuffing
	if( m_Owner )
	{
		m_Owner->EventDismissPet();

		if( bUpdate )
		{
			if( !bExpires )
				UpdatePetInfo( bSetOffline );
			if( !IsSummon() )
				m_Owner->_SavePet( NULL );
		}
		m_Owner->SetUInt64Value( UNIT_FIELD_SUMMON, 0 );
		m_Owner->SetSummon( NULL );
		SendNullSpellsToOwner();
		ClearPetOwner();
	}

	// has to be next loop - reason because of RemoveFromWorld, iterator gets broken
	if( IsInWorld() && IsActive() )
		Deactivate( m_mapMgr );

	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent( this, &Pet::PetSafeDelete, EVENT_CREATURE_SAFE_DELETE, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void Pet::PetSafeDelete()
{
	// remove from world, and delete
	if( IsInWorld() )
	{
		RemoveFromWorld( false, false );
	}

	sEventMgr.RemoveEvents( this ); // to avoid crash of double delete
	delete this;					// destructs pet -> creature -> unit...
	//this = NULL;
}

void Pet::DelayedRemove( bool bTime, bool bDeath )
{
	// called when pet has died
	if( bTime )
	{
		if( !m_Owner )
			m_Owner = objmgr.GetPlayer( GET_LOWGUID_PART( m_OwnerGuid ) );

		if( Summon )
			Dismiss();  // remove us..
		else
			Remove( true, false );
	}
	else
		sEventMgr.AddEvent(this, &Pet::DelayedRemove, true, bDeath, EVENT_PET_DELAYED_REMOVE, PET_DELAYED_REMOVAL_TIME, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
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
		ModUnsigned32Value( UNIT_FIELD_LEVEL, 1 );
		xp -= nxp;
		nxp = GetNextLevelXP( m_uint32Values[ UNIT_FIELD_LEVEL ] );
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, nxp );
		ApplyStatsForLevel();
		UpdateSpellList();
	}

	SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, xp );
}

uint32 Pet::GetNextLevelXP(uint32 level)
{
	// Pets need only 10% of xp to level up compared to players
	uint32 nextLvlXP = 0;
	if( level > 0 && level <= MAX_PREDEFINED_NEXTLEVELXP )
	{
		nextLvlXP = NextLevelXp[ level - 1 ];
	}
	else
	{
		nextLvlXP = ((int)((((double)(((8 * level) + ((level - 30) * 5)) * ((level * 5) + 45)))/100)+0.5))*100;
	}
	double xp = double(nextLvlXP) / 10;
	return FL2UINT(xp);
}

void Pet::UpdateSpellList( bool showLearnSpells )
{
	// SkillLine 1
	uint32 s = 0;
	// SkillLine 2
	uint32 s2 = 0;

	if( GetCreatureInfo() )
	{
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
			CreatureFamilyEntry* f = dbcCreatureFamily.LookupEntry( GetCreatureInfo()->Family );
			if( f )
			{
				s = f->skilline;
				s2 = f->tameable;
			}
		}
	}

	if( s || s2 )
	{
		skilllinespell* sls;
		uint32 rowcount = dbcSkillLineSpell.GetNumRows();
		uint32 current = 0;
		SpellEntry* sp;
		for( uint32 idx = 0; idx < rowcount; ++idx )
		{
			sls = dbcSkillLineSpell.LookupRow( idx );
			// Update existing spell, or add new "automatic-acquired" spell
			if( (sls->skilline == s || sls->skilline == s2) && sls->acquireMethod == 2 )
			{
				sp = dbcSpell.LookupEntry( sls->spell );
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
			Spell * spell = SpellPool.PooledNew();
			if (!spell)
				return;
			spell->Init(this, sp, true, false);
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
		TalentEntry *te = dbcTalent.LookupRow( i );
		if( te == NULL || te->TalentTree < 409 || te->TalentTree > 411 ) // 409-Tenacity, 410-Ferocity, 411-Cunning
			continue;
		for( j = 0; j < 5; j++ )
			if( te->RankID[ j ] != NULL && HasSpell( te->RankID[ j ] ) )
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
		m_Owner->GetSession()->OutPacket( SMSG_PET_UNLEARNED_SPELL, 2, &sp->Id );
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
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
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
	if(m_uint32Values[OBJECT_FIELD_ENTRY] == 89)
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
	BaseStats[STAT_STRENGTH] = FL2UINT(pet_str);
	BaseStats[STAT_AGILITY] = FL2UINT(pet_agi);
	BaseStats[STAT_STAMINA] = FL2UINT(pet_sta);
	BaseStats[STAT_INTELLECT] = FL2UINT(pet_int);
	BaseStats[STAT_SPIRIT] = FL2UINT(pet_spr);

	double pet_min_dmg = base_min_dmg + pet_level * mod_min_dmg;
	double pet_max_dmg = base_max_dmg + pet_level * mod_max_dmg;
	BaseDamage[0] = float(pet_min_dmg);
	BaseDamage[1] = float(pet_max_dmg);

	// Apply attack power.
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, FL2UINT(pet_pwr));

	BaseResistance[0] = FL2UINT(pet_arm);
	CalcResistance(0);

	// Calculate health / mana
	double health = pet_sta * pet_sta_to_hp;
	double mana = has_mana ? (pet_int * pet_int_to_mana) : 0.0;
	if( health == 0 )
	{
		sLog.outError("Pet with entry %u has 0 health !! \n",m_uint32Values[OBJECT_FIELD_ENTRY]);
		health = 100;
	}
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, FL2UINT(health));
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, FL2UINT(health));
	SetUInt32Value(UNIT_FIELD_BASE_MANA, FL2UINT(mana));
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, FL2UINT(mana));

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
	static uint32 family_aura[46] = { 0, 17223, 17210, 17129, 17208, 7000, 17212, 17209, 17211, 17214, 0, 17217, 17220, 0, 0, 0, 0, 0, 0, 0, 17218, 17221, 0, 0, 17206, 17215, 17216, 17222, 0, 0, 34887, 35257, 35254, 35258, 35253, 35386, 50297, 54676, 0, 55192, 55729, 56634, 56635, 58598, 61199 };

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
	SetUInt32Value( UNIT_FIELD_BASE_HEALTH, base_hp );
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
	if( myFamily )
	{
		float pet_level = float( m_uint32Values[ UNIT_FIELD_LEVEL ] );
		float level_diff = float( myFamily->maxlevel - myFamily->minlevel );
		float scale_diff = float( myFamily->maxsize - myFamily->minsize );
		float factor = scale_diff / level_diff;
		float scale = factor * pet_level + myFamily->minsize;
		SetFloatValue( OBJECT_FIELD_SCALE_X, scale );
	}

	// Apply health fields.
	SetUInt32Value( UNIT_FIELD_HEALTH, m_uint32Values[ UNIT_FIELD_MAXHEALTH ] );
	SetUInt32Value( UNIT_FIELD_POWER1, m_uint32Values[ UNIT_FIELD_MAXPOWER1 ] );
	SetUInt32Value( UNIT_FIELD_POWER3, m_uint32Values[ UNIT_FIELD_MAXPOWER3 ] );
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

	uint32 str = GetUInt32Value( UNIT_FIELD_STAT0 );
	uint32 AP = ( str * 2 - 20 );
	if( m_Owner )
		AP += m_Owner->GetRAP() * 22 / 100;
	if( AP < 0 ) AP = 0;
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, AP );
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
			chance = Rand( 100.0f / float( size ) );

		if((*itr)->autocast_type == AUTOCAST_EVENT_ATTACK)
		{
			// spells still spammed, I think the cooldowntime is being set incorrectly somewhere else
			if( chance && (*itr)->spell && getMSTime() >= (*itr)->cooldowntime && //cebernic:crashfix
				GetUInt32Value( UNIT_FIELD_POWER1 + (*itr)->spell->powerType ) >= (*itr)->spell->manaCost )
			{
				return *itr;
			}
		}
		else
		{
			// bad pointers somehow end up here :S
			sLog.outError("Bad AI_Spell detected in AutoCastEvent!\n");
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
		else if( sp->autocast_type != Type )
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
	ASSERT(sp != NULL);
	ASSERT(sp->spell != NULL);

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


