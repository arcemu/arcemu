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
		return false;//the caller will delete us.
	
	if( dismiss_old_pet )
		owner->DismissActivePets();

	m_Owner = owner;
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

	m_aiInterface = AIInterface_Mgr::CreateInterface(this, AITYPE_PET, m_Owner);

	// Hunter pet should be max 5 levels below owner
	uint32 level = owner->getLevel();
	if( type == HUNTERPET && created_from_creature != NULL )
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

	if( type == WARLOCKPET)
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
		setPetXP(0);
		setPetNextLvlXP( calcNextLvlXP( level ) );
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

	if( !bExpires )
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
	Summon = false;
	memset(ActionBar, 0, sizeof(uint32)*10);
	ScheduledForDeletion = false;

	m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;
	m_PetNumber = 0;

	m_ExpireTime = 0;
	bExpires = false;
	m_Diet = 0;
	reset_time = 0;
	reset_cost = 0;
	m_talentSpells.clear();
	mPi = NULL;
}

Pet::~Pet()
{
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
	PetAI * ai = TO_AIPET(GetAIInterface() );

	std::map<AI_PetSpell*, uint16>::iterator itr = ai->getSpellMapStart();
	uint16 packetsize = ( GetEntry() != WATER_ELEMENTAL && GetEntry() != SPIRITWOLF ) ? ( ( uint16 )ai->getSpellMapSize() * 4 + 59 ) : 62;
	
	WorldPacket * data = new WorldPacket( SMSG_PET_SPELLS, packetsize );
	*data << GetGUID();
	*data << uint16( myFamily != NULL ? myFamily->ID : 0 );	// pet family to determine talent tree
	*data << m_ExpireTime;
	*data << uint8( ai->Pet_getstate() );	// 0x0 = passive, 0x1 = defensive, 0x2 = aggressive
	*data << uint8( ai->Pet_getaction() );	// 0x0 = stay, 0x1 = follow, 0x2 = attack
	*data << uint16( 0 );				// flags: 0xFF = disabled pet bar (eg. when pet stunned)

	// Send the actionbar
	for( uint8 i = 0; i < 10; ++i )
	{
		if(ActionBar[i] & 0x4000000)		// Commands
			*data << uint32( ActionBar[i] );
		else
		{
			if( ActionBar[i] )
			{
				*data << uint16(ActionBar[i]);
				if( ai->Spell_isautocast( (AI_PetSpell*)ai->Spell_findbyId(ActionBar[i] ) ) )
					*data << uint16(AUTOCAST_SPELL_STATE);
				else
					*data << uint16(DEFAULT_SPELL_STATE);
			}
			else
				*data << uint16(0) << uint8(0) << uint8(i+5);
		}
	}

	// don't send spells for non family creatures.
	if(GetCreatureInfo()->Family > 0)
	{
		// Send the rest of the spells.
		std::map<AI_PetSpell *, uint16>::iterator itr = ai->getSpellMapStart();
		std::map<AI_PetSpell*, uint16>::iterator itend = ai->getSpellMapEnd();
		*data << uint8( ai->getSpellMapSize() );
		for(; itr != itend; ++itr)
		{
			if(! (itr->first->proto->Attributes & ATTRIBUTES_PASSIVE) )
				*data << uint16(itr->first->proto->Id) << itr->second;
		}
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
			{
				if( te->RankID[ j ] > 0 && Talent_has( te->RankID[ j ] ) )
				{
					// if we have the spell, include it in packet
					data << te->TalentID;	// Talent ID
					data << j;				// Rank
					++count;
				}
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

void Pet::LoadFromDB( Player* owner, PlayerPet * pi )
{
	m_Owner = owner;
	m_phase = m_Owner->GetPhase();
	mPi = pi;
	creature_info = CreatureNameStorage.LookupEntry( mPi->entry );
	proto = CreatureProtoStorage.LookupEntry( mPi->entry );
	myFamily = dbcCreatureFamily.LookupEntry( creature_info->Family );

	Create( pi->name.c_str(), owner->GetMapId(), owner->GetPositionX() + 2 , owner->GetPositionY() + 2, owner->GetPositionZ(), owner->GetOrientation() );

	m_PetNumber = mPi->number;
	//todo set xp.
	m_name = mPi->name;
	Summon = false;
	SetEntry( mPi->entry );
	setLevel( mPi->level );


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
			if( !( ActionBar[i] & 0x4000000 ) && spellid )
			{
				AI_PetSpell * aispell = objmgr.Pet_getAISpell( GetCreatureInfo()->Family, spellid);
				if(aispell == NULL)
					sLog.outDebug("Pet::LoadFromDB, attempted to load spell %u but was missing an ai pet spell. \n", spellid);
				else
					TO_AIPET( GetAIInterface() )->Spell_add( aispell, (spstate == AUTOCAST_SPELL_STATE) ? true : false);
			}

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
		SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, calcNextLvlXP( mPi->level ) );
		SetUInt32Value( UNIT_FIELD_BYTES_2, 1 );
		SetPower(POWER_TYPE_FOCUS, 100 );// Focus
		SetMaxPower( POWER_TYPE_FOCUS, 100 );
		SetPowerType(POWER_TYPE_FOCUS);
	}

	memset(&BaseDamage, 0, sizeof(float) * 6);
	/*BaseDamage[0] = 0;
	BaseDamage[1] = 0;
	BaseOffhandDamage[0] = 0;
	BaseOffhandDamage[1] = 0;
	BaseRangedDamage[0] = 0;
	BaseRangedDamage[1] = 0;*/
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
	SetCreatureInfo( CreatureNameStorage.LookupEntry( GetEntry() ) );
	proto = CreatureProtoStorage.LookupEntry( GetEntry() );

	m_Owner->AddSummon( this );
    m_Owner->SetSummonedUnitGUID( GetGUID() );

	SetUInt32Value( UNIT_FIELD_PETNUMBER, GetUIdFromGUID() );
	SetUInt32Value( UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME );

	myFamily = dbcCreatureFamily.LookupEntry( GetCreatureInfo()->Family );

	SetInstanceID( m_Owner->GetInstanceID() );
	SetPetDiet();
	_setFaction();

	//make him visible.
	PushToWorld( m_Owner->GetMapMgr() );

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
			m_aiInterface->disableMelee();
		else if( GetEntry() == PET_FELGUARD )
			SetEquippedItem(MELEE,12784 );

	}
	else // Hunter talents loading.
	{
		//If we have this pets family before, then we probably have saved his talents, we just load them.
		SetTPs( GetTPsForLevel( getLevel() ) );	// set talent points
		// Pull from database... :/
		uint32 family_id = GetCreatureInfo()->Family;
		//count how many talents we have spec'd.
		uint32 talent_cnt = 0;
		QueryResult * query = CharacterDatabase.Query("SELECT * FROM pet_talents WHERE pet_owner = %u AND pet_family = %u",
			m_Owner->GetLowGUID(), family_id);
		if( query )
		{
			Field * f = query->Fetch();
			string talent_array(f[3].GetString() );
			int delimiter = 0, lastdelimiter = 0;
			int length = talent_array.length();
			uint32 spell_id = 0;
			while(delimiter < length && delimiter != string::npos)
			{
				delimiter = talent_array.find(',',lastdelimiter);
				spell_id = (uint32)atoi( talent_array.substr(lastdelimiter,(delimiter != string::npos) ? delimiter : talent_array.length() ).c_str() );
				lastdelimiter = ++delimiter;
				if(spell_id > 0 && !Talent_has(spell_id) )
				{
					Talent_add(spell_id);
					++talent_cnt;
				}
			}
		}
		delete query;
		//subtract from our talent points the talent count.
		SetTPs( GetTPs() - (uint8)talent_cnt);
	}

	//update to retrieve spells we can get.
	UpdateSpellList( false );
	if( first )
		// Set up actionbar containing the 6 command spells(attack,follow,stay, aggressive,defensive,passive) and 4 spells from our spell list.
		setDefaultActionbar();

	//send the pet bar
	SendSpellsToOwner();
	if( !Summon )
		SendTalentsToOwner();

	// set to active
	if( !bExpires )
		UpdatePetInfo( false );
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
	pi->xp = getPetXP();
	pi->level = getLevel();
	pi->happinessupdate = m_HappinessTimer;
	PetAI * ai = TO_AIPET(GetAIInterface() );

	// save actionbar
	ss.rdbuf()->str("");
	for(uint32 i = 0; i < 10; ++i)
	{
		if( ActionBar[i] & 0x4000000 )
			ss << ActionBar[i] << " 0";
		else if(ActionBar[i])
			ss << ActionBar[i] << " " << (ai->Spell_isautocast( (AI_PetSpell*)ai->Spell_findbyId( ActionBar[i]) ) == true) ? uint32(AUTOCAST_SPELL_STATE) : uint32(DEFAULT_SPELL_STATE);
		else
			ss << "0 0";

		ss << ",";
	}

	pi->actionbar = ss.str();
	pi->reset_cost = reset_cost;
	pi->reset_time = reset_time;
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
	xp += getPetXP();
	uint32 nxp = getPetNextLvlXP();
	//if we are rdy to level up.
	if( xp >= nxp )
	{
		SetTPs( GetTPsForLevel( getLevel() + 1 ) - GetSpentTPs() );
		modLevel(1 );
		xp -= nxp;
		nxp = calcNextLvlXP( getLevel() );
		setPetNextLvlXP(nxp);
		ApplyStatsForLevel();
		UpdateSpellList();
		SendTalentsToOwner();
	}
	setPetXP(xp);
}

uint32 Pet::calcNextLvlXP(uint32 level)
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
	PetAI * ai = TO_AIPET( GetAIInterface() );
	CreatureInfo * info = GetCreatureInfo();
	// Get Creature family from DB (table creature_names, field family), load the skill line from CreatureFamily.dbc for use with SkillLineAbiliby.dbc entry
	//CreatureFamilyEntry* f = dbcCreatureFamily.LookupEntryForced( info->Family);
	//store the level for fast access.
	uint32 lvl = getLevel();
	//Setup this way, for spells with 0 family id, grab their spells from proto.
	if( objmgr.Pet_getAISpellSize(info->Family) > 0)
	{
		//get iterator to our spells using our family id from objmgr.
		HM_NAMESPACE::hash_multimap<uint32, AI_PetSpell*>::iterator itr = objmgr.Pet_getAISpellStart(info->Family);
		//walk the spells
		for(; itr != objmgr.Pet_getAISpellEnd(info->Family); ++itr)
		{
			bool addthis = false;
			bool found = false;
			//do we meet level requirements?
			if(itr->second->acquire_type != 2 || itr->second->proto->baseLevel > lvl)
				continue;
			if(ai->Spell_gethigherRank(itr->second->proto->Id) == 0)
				addthis = true;
			//Don't show for passive spells.
			if(!found || addthis)
				Spell_add(itr->second, true, ( (itr->second->proto->Attributes & ATTRIBUTES_PASSIVE) != 0 )? showLearnSpells : false);
		}
	}
	else
	{
		//grab spells from proto.
		CreatureProto * proto = GetProto();
		if(proto->spells.size() > 0)
		{
			//ofc, you don't display them learning those spells but show them on the action bar.
			for(std::list<AI_Spell*>::iterator itr = proto->spells.begin(); itr != proto->spells.end(); ++itr)
				Spell_add( (AI_PetSpell*)(*itr), true, false);
		}
	}
}
void Pet::setDefaultActionbar()
{
	// Set up the default actionbar.
	ActionBar[0] = PET_SPELL_ATTACK;
	ActionBar[1] = PET_SPELL_FOLLOW;
	ActionBar[2] = PET_SPELL_STAY;

	PetAI * ai = TO_AIPET( GetAIInterface() );
	// Fill up 4 slots with our spells
	if( ai->getSpellMapSize() )
	{
		std::map<AI_PetSpell*, uint16>::iterator itr = ai->getSpellMapStart();
		std::map<AI_PetSpell*, uint16>::iterator itend = ai->getSpellMapEnd();
		//for each spell we grab, we 1st check if we have a higher rank, once we resolve it, we store it's namehas here so we don't search 4 it again.
		std::set<uint32> resolvedNameHashes;
		uint32 pos = 0;
		uint32 successer = 0;
		for(; itr != itend && pos < 4; ++itr)
		{
			if( !(itr->first->proto->Attributes & ATTRIBUTES_PASSIVE) && resolvedNameHashes.find( itr->first->proto->NameHash) == resolvedNameHashes.end() )
			{
				//find a successer
				successer = ai->Spell_gethigherRank(itr->first->proto->Id);
				//set the successer as the slot otherwise set the current one.
				if(successer != 0)
					ActionBar[3+pos] = successer;
				else
					ActionBar[3+pos] = itr->first->proto->Id;
				//store the namehash so we don't try to find this spell again.
				resolvedNameHashes.insert(itr->first->proto->NameHash);
				++pos;
			}
		}
	}

	ActionBar[7] = PET_SPELL_AGRESSIVE;
	ActionBar[8] = PET_SPELL_DEFENSIVE;
	ActionBar[9] = PET_SPELL_PASSIVE;
}

void Pet::Talent_wipe()
{
	//we first remove the aura from ourself
	bool talent_Removed = false, spell_Removed = false;
	SpellEntry * proto = NULL;
	for(std::set<uint32>::iterator itr = m_talentSpells.begin(); itr != m_talentSpells.end(); ++itr)
	{
		//passive spells are cast on us immediately so we simply remove the aura, however, some talents teach the pet a new skill.(dash, callofwild, heartofphoenix)
		proto = dbcSpell.LookupEntryForced(*itr);
		if(proto->Attributes & ATTRIBUTES_PASSIVE)
		{
			talent_Removed = true;
			RemoveAura(*itr);
		}
		else
		{
			Spell_remove(*itr, true);
			spell_Removed = true;
		}
	}
	//wipe our talent list.
	m_talentSpells.clear();
	//visually update to the client, but only if things changed.
	if(talent_Removed)
		SendTalentsToOwner();
	if(spell_Removed)
		SendSpellsToOwner();
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
		m_aiInterface->disableMelee();
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
		m_aiInterface->disableMelee();
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
		sLog.outError("Pet with entry %u has 0 health !! \n", GetEntry() );
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

uint32 Pet::getUntrainCost()
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


void Pet::DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras )
{
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

		if( pVictim->isCritter() )
		{
			m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, pVictim->GetEntry(), 1, 0);
			m_Owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE, GetHighGUID(), GetLowGUID(), 0);
		}

#endif


	}
	else
	{
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

	GetAIInterface()->HandleEvent(EVENT_UNITDIED, pAttacker, 0);
	
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

void Pet::Spell_add( AI_PetSpell* spell, bool learning, bool showLearnSpell )
{
	//Add it to our ai.
	GetAIInterface()->Spell_add( (AI_Spell*)spell );
	//if we are learning it, must either mean we have a lower rank, or a new one.
	if(learning)
	{
		//we always only occupy the 3-6;
		for(uint32 i = 3; i < 7; ++i)
		{
			if(ActionBar[i] == 0 || dbcSpell.LookupEntryForced( ActionBar[i] )->NameHash == spell->proto->NameHash )
			{
				ActionBar[i] = spell->proto->Id;
				break;
			}
		}
	}
	//cast passive spells.
	if(spell->proto->Attributes & ATTRIBUTES_PASSIVE)
		CastSpell(this, spell->proto->Id, false);
	if(showLearnSpell)
		OutPacket(SMSG_PET_LEARNED_SPELL, 1, &spell->proto->Id );
}

bool Pet::Spell_has( uint32 SpellID )
{
	return GetAIInterface()->Spell_has(SpellID);
}

void Pet::Spell_remove(uint32 spell_id, bool showUnlearnSpell)
{
	//remove spell from the ai.
	PetAI * ai = TO_AIPET(GetAIInterface() );
	ai->Spell_remove( ai->Spell_findbyId( spell_id) );
	//Remove spell from action bar as well
	for( uint32 pos = 3; pos < 7; pos++ )
	{
		if( ActionBar[pos] == spell_id)
		{
			ActionBar[pos] = 0;
			//since we changed the action bar, we should update client side.
			SendSpellsToOwner();
			break;
		}
	}
	if( showUnlearnSpell && m_Owner && m_Owner->GetSession() )
		m_Owner->GetSession()->OutPacket( SMSG_PET_UNLEARNED_SPELL, 4, &spell_id);
}

void Pet::Talent_add(uint32 spell, bool show)
{
	SpellEntry * proto = dbcSpell.LookupEntryForced(spell);
	/*	For spells that are simply passive auras, we cast them, but some talents teach the pet a new skill, so we add that to our spell map. */
	if(proto != NULL)
	{
		if(!(proto->Attributes & ATTRIBUTES_PASSIVE) )
		{
			AI_PetSpell * aispell = objmgr.Pet_getAISpell(GetCreatureInfo()->Family, spell);
			if(aispell != NULL)
				Spell_add(aispell, show);
		}
		else
			CastSpell(this, spell, false);

		m_talentSpells.insert(spell);
	}
}
bool Pet::Talent_has(uint32 spell)
{
	return (m_talentSpells.find(spell) != m_talentSpells.end() );
}
void Pet::Talent_remove(uint32 spell, bool show)
{
	SpellEntry * proto = dbcSpell.LookupEntryForced(spell);
	if(proto != NULL)
	{
		if( !(proto->Attributes & ATTRIBUTES_PASSIVE) )
			Spell_remove(spell, show);
		else
			RemoveAura(spell);
		m_talentSpells.erase(spell);
	}
}



