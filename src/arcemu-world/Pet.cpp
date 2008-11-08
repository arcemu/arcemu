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

#define GROWL_RANK_1 2649
#define GROWL_RANK_2 14916
#define WATER_ELEMENTAL 510
#define PET_IMP 416
#define PET_VOIDWALKER 1860
#define PET_SUCCUBUS 1863
#define PET_FELHUNTER 417
#define PET_FELGUARD 17252


uint32 GetAutoCastTypeForSpell(SpellEntry * ent)
{
	switch(ent->NameHash)
	{

	/************************************************************************/
	/* Warlock Pet Spells													*/
	/************************************************************************/

	case SPELL_HASH_FIREBOLT:			// Firebolt
	case SPELL_HASH_LASH_OF_PAIN:		// Lash of Pain
	case SPELL_HASH_TORMENT:			// Torment
	case SPELL_HASH_SUFFERING:
	case SPELL_HASH_SOOTHING_KISS:
	case SPELL_HASH_SEDUCTION:
	case SPELL_HASH_CLEAVE:
	case SPELL_HASH_INTERCEPT:
	case SPELL_HASH_DEVOUR_MAGIC:
	case SPELL_HASH_SPELL_LOCK:

		return AUTOCAST_EVENT_ATTACK;
		break;

	case SPELL_HASH_BLOOD_PACT:			// Blood Pact
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

	/************************************************************************/
	/* Hunter Pet Spells													*/
	/************************************************************************/

	case SPELL_HASH_BITE:				// Bite
	case SPELL_HASH_CHARGE:				// Charge
	case SPELL_HASH_CLAW:				// Claw
	case SPELL_HASH_COWER:				// Cower
	case SPELL_HASH_GORE:				// Gore
	case SPELL_HASH_GROWL:				// Growl
	case SPELL_HASH_POISON_SPIT:		// Poison Spit 
	case SPELL_HASH_SCORPID_POISON:		// Scorpid Poison
	case SPELL_HASH_SCREECH:			// Screech
	case SPELL_HASH_WARP:				// Warp
	case SPELL_HASH_LIGHTNING_BREATH:	// Lightning Breath
	case SPELL_HASH_FIRE_BREATH:		// Fire Breath
		return AUTOCAST_EVENT_ATTACK;
		break;
	
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
	}

#ifdef _DEBUG
	sLog.outDebug("Pet does not have event assigned for spell %u \n",ent->Id);
#endif

	return AUTOCAST_EVENT_NONE;
}

void Pet::CreateAsSummon(uint32 entry, CreatureInfo *ci, Creature* created_from_creature, Player* owner, SpellEntry* created_by_spell, uint32 type, uint32 expiretime, LocationVector* Vec)
{
	SetIsPet(true);

	//std::string myname = sWorld.GenerateName();

	if(!ci) return;
	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	creature_info = ci;
	if( GetCreatureInfo() )
		myFamily = dbcCreatureFamily.LookupEntry(GetCreatureInfo()->Family);
	else myFamily = NULL;
	//m_name = objmgr.GetCreatureFamilyName(myFamily->ID);
	if( myFamily == NULL || myFamily->name == NULL )
		m_name = "Pet";
	else
		m_name.assign( myFamily->name );

	float x, y, z;
	if( Vec )
	{
		x = Vec->x;
		y = Vec->y;
		z = Vec->z;
	}
	else
	{
		x = owner->GetPositionX();
		y = owner->GetPositionY();
		z = owner->GetPositionZ();
	}
	// Create ourself	
	Create(m_name.c_str(), owner->GetMapId(), x, y, z, owner->GetOrientation());
	SetUInt32Value(OBJECT_FIELD_ENTRY, entry);
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);	// better set this one

	// Fields common to both lock summons and pets
	if(type & 0x2 && created_from_creature != NULL)
		SetUInt32Value(UNIT_FIELD_LEVEL, created_from_creature->getLevel());
	else
		SetUInt32Value(UNIT_FIELD_LEVEL,owner->GetUInt32Value(UNIT_FIELD_LEVEL));

	SetUInt32Value(UNIT_FIELD_DISPLAYID,  ci->Male_DisplayID);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, ci->Male_DisplayID);
	EventModelChange();
	SetUInt64Value(UNIT_FIELD_SUMMONEDBY, owner->GetGUID());
	SetUInt64Value(UNIT_FIELD_CREATEDBY, owner->GetGUID());
	
	if(type & 0x1 && created_by_spell != NULL)
		SetUInt64Value(UNIT_CREATED_BY_SPELL, created_by_spell->Id);

	if(type & 0x1 || created_from_creature == NULL)
	{
		Summon = true;
		SetUInt32Value(UNIT_FIELD_BYTES_0, 2048 | (0 << 24));
		SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME_01, 2000);
		SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.5f);
		SetFloatValue(UNIT_FIELD_COMBATREACH, 0.75f);
		SetUInt32Value(UNIT_FIELD_BYTES_2, (0x01 | (0x28 << 8) | (0x2 << 24)));
		SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());
		SetPowerType(POWER_TYPE_MANA);
		if(entry == WATER_ELEMENTAL)
			m_name = "Water Elemental";
		else if(entry == 19668)
			m_name = "Shadowfiend";
		else if((entry==PET_IMP) || (entry==PET_VOIDWALKER) || (entry==PET_SUCCUBUS) ||
			(entry==PET_FELHUNTER) || (entry==PET_FELGUARD)) // check if it has a name
		{
			QueryResult* result = CharacterDatabase.Query("SELECT `name` FROM `playersummons` WHERE `ownerguid`=%u AND `entry`=%d",
				owner->GetLowGUID(), entry);
			if(result)
			{
				m_name = result->Fetch()->GetString();
				delete result;
			}
			else // no name found, generate one and save it
			{
				m_name = sWorld.GenerateName();
				CharacterDatabase.Execute("INSERT INTO playersummons VALUES(%u, %u, '%s')",
					owner->GetLowGUID(), entry, m_name.data());
			}
		}
		else
			m_name = sWorld.GenerateName();

	} else {
		SetUInt32Value(UNIT_FIELD_BYTES_0, 2048 | (0 << 24));
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME_01, 2000); // Supalosa: 2.00 normalised attack speed
		SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, created_from_creature->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS));
		SetFloatValue(UNIT_FIELD_COMBATREACH, created_from_creature->GetFloatValue(UNIT_FIELD_COMBATREACH));

		// These need to be checked.
		SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT); // why combat ??
		SetUInt32Value(UNIT_FIELD_POWER5, PET_HAPPINESS_UPDATE_VALUE >> 1);//happiness
		SetUInt32Value(UNIT_FIELD_MAXPOWER5, 1000000);
		SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
		SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP(getLevel()));
		SetUInt32Value(UNIT_FIELD_BYTES_1, 0 | (REBELIOUS << 8));//loyalty level
		SetUInt32Value(UNIT_TRAINING_POINTS, 0);				//training points

		// Focus
		SetUInt32Value(UNIT_FIELD_POWER3, 100);
		SetUInt32Value(UNIT_FIELD_MAXPOWER3, 100);

		// 0x3 -> Enable pet rename.
		SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x28 << 8) | (0x3 << 16));

		// Change the power type to FOCUS
		SetPowerType(POWER_TYPE_FOCUS);

		// create our spellz
		SetDefaultSpells();
	}
	
	BaseDamage[0] = 0;
	BaseDamage[1] = 0;
	BaseOffhandDamage[0 ] = 0;
	BaseOffhandDamage[1] = 0;
	BaseRangedDamage[0] = 0;
	BaseRangedDamage[1] = 0;
	
	// Apply stats.
	ApplyStatsForLevel();

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	m_PartySpellsUpdateTimer = 0;

	m_PetNumber = static_cast< Player* >(owner)->GeneratePetNumber();
	SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());

	m_ExpireTime = expiretime;
	bExpires = m_ExpireTime > 0 ? true : false;

	if(!bExpires)
	{
		// Create PlayerPet struct (Rest done by UpdatePetInfo)
		PlayerPet *pp = new PlayerPet;
		pp->number = m_PetNumber;
		pp->stablestate = STABLE_STATE_ACTIVE;
		pp->spellid = created_by_spell ? created_by_spell->Id : 0;
		static_cast< Player* >(owner)->AddPlayerPet(pp, pp->number);
	}	

	//maybe we should use speed from the template we created the creature ?
	m_base_runSpeed = m_runSpeed = owner->m_base_runSpeed; //should we be able to keep up with master ?
	m_base_walkSpeed = m_walkSpeed = owner->m_base_walkSpeed; //should we be able to keep up with master ?

	InitializeMe(true);
}


Pet::Pet(uint64 guid) : Creature(guid)
{
	int i;

	m_PetXP = 0;
	Summon = false;
	memset(ActionBar, 0, sizeof(uint32)*10);

	m_AutoCombatSpell = 0;

	m_PartySpellsUpdateTimer = 0;
	m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;
	m_LoyaltyTimer = PET_LOYALTY_UPDATE_TIMER;
	m_PetNumber = 0;

	m_State = PET_STATE_DEFENSIVE;
	m_Action = PET_ACTION_FOLLOW;
	bHasLoyalty = false;
	m_ExpireTime = 0;
	bExpires = false;
	m_Diet = 0;
	m_Action = PET_ACTION_FOLLOW;
	m_State = PET_STATE_DEFENSIVE;
	TP = 0;
	LoyaltyPts = LoyLvlRange[1];
	LoyaltyXP = 0;
	reset_time = 0;
	reset_cost = 0;

	for(i=0;i<AUTOCAST_EVENT_COUNT;i++)
		m_autoCastSpells[i].clear();
	m_AISpellStore.clear();
	mSpells.clear();
}

Pet::~Pet()
{
	for(std::map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.begin(); itr != m_AISpellStore.end(); ++itr)
		delete itr->second;

	if(IsInWorld())
		this->Remove(false, true, true);
}

void Pet::Update(uint32 time)
{
	Creature::Update(time); // passthrough
	
	if( bHasLoyalty && !bExpires && isAlive() )
	{
		//Happiness
		if( m_HappinessTimer == 0 )
		{	
			int32 val = GetUInt32Value( UNIT_FIELD_POWER5 );
			//amount of burned happiness is loyalty_lvl depended
			int32 burn = HappinessTicks[ GetLoyaltyLevel() - 1 ];
			if( CombatStatus.IsInCombat() )
				burn = burn >> 1; //in combat reduce burn by half (guessed) 
			if( ( val - burn ) < 0 )
				val = 0;
			else
				val -= burn;
			SetUInt32Value( UNIT_FIELD_POWER5, val );// Set the value
			m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;// reset timer
		} 
		else if( !IsInBg() )
		{
			if( time > m_HappinessTimer )
				m_HappinessTimer = 0;
			else
				m_HappinessTimer -= time;
		}
		//Loyalty
		if( m_LoyaltyTimer == 0 && GetHappinessState() != 0 )
		{
			m_LoyaltyTimer = PET_LOYALTY_UPDATE_TIMER;
			if( !UpdateLoyalty(LoyaltyTicks[GetHappinessState()-1]) ) //loyalty tick is happiness state dependent
				return;			
		} 
		else 
		{
			if(time > m_LoyaltyTimer)
				m_LoyaltyTimer = 0;
			else
				m_LoyaltyTimer -= time;
		}
	}

	if(bExpires)
	{
		if(m_ExpireTime == 0)
		{
			// remove
			Dismiss(false);
			return;
		} else {
			if(time > m_ExpireTime)
			{
				m_ExpireTime = 0;
			} else {
				m_ExpireTime -= time;
			}
		}
	}
}

void Pet::SendSpellsToOwner()
{
	if( m_Owner == NULL )
		return;

	int packetsize = ( m_uint32Values[ OBJECT_FIELD_ENTRY ] != WATER_ELEMENTAL) ? ( (int) mSpells.size() * 4 + 20 ) : 64;
	WorldPacket * data = new WorldPacket( SMSG_PET_SPELLS, packetsize );
	*data << GetGUID();
	*data << uint32(0x00000000);//unk1
	
	uint32 state_flags = 0;
	if( GetAIInterface() != NULL && GetAIInterface()->getUnitToFollow() != NULL )
		 state_flags = 0x100;		// 0x0 = stay, 0x100 = follow, 0x200 = attack
	state_flags |= GetPetState();	// 0x0 = passive, 0x1 = defensive, 0x2 = agressive
	*data << state_flags;			
																			
	// Send the actionbar
	for(uint32 i = 0; i < 10; ++i)
	{
		if(ActionBar[i] & 0x4000000)		// Commands
			*data << uint32(ActionBar[i]);
		else
		{
			if(uint16(ActionBar[i]))
				*data << uint16(ActionBar[i]) << GetSpellState(ActionBar[i]);
			else
				*data << uint16(0) << uint8(0) << uint8(i+5);
		}
	}

	// we don't send spells for the water elemental so it doesn't show up in the spellbook
	if(m_uint32Values[OBJECT_FIELD_ENTRY] != WATER_ELEMENTAL)	
	{
		// Send the rest of the spells.
		*data << uint8(mSpells.size());
		for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
			*data << uint16(itr->first->Id) << uint16(itr->second);
	}
	*data << uint64(0);
	m_Owner->delayedPackets.add(data);
}

void Pet::SendNullSpellsToOwner()
{
	if( m_Owner == NULL || m_Owner->GetSession() == NULL)
		return;

	WorldPacket data(8);
	data.SetOpcode(SMSG_PET_SPELLS);
	data << uint64(0);
	m_Owner->GetSession()->SendPacket(&data);
}

void Pet::SendCastFailed( uint32 spellid, uint8 fail )
{
	if( m_Owner == NULL || m_Owner->GetSession() == NULL)
		return;

	WorldPacket data( SMSG_PET_CAST_FAILED, (4+1) );
	data << uint32( spellid );
	data << uint8( fail );
	m_Owner->GetSession()->SendPacket(&data);
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
			sp->Init( this, info, true, false );
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
	map<uint32,AI_Spell*>::iterator itr = m_AISpellStore.find(info->Id);
	if(itr != m_AISpellStore.end())
		return itr->second;

	AI_Spell *sp = new AI_Spell;
	sp->agent = AGENT_SPELL;
	sp->entryId = GetEntry();
	sp->floatMisc1 = 0;
	sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(info->rangeIndex));
	if( sp->maxrange < sqrt(info->base_range_or_radius_sqr) )
		sp->maxrange = sqrt(info->base_range_or_radius_sqr);
	sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(info->rangeIndex));
	sp->Misc2 = 0;
	sp->procChance = 0;
	sp->spell = info;
	sp->cooldown = objmgr.GetPetSpellCooldown(info->Id);
	if( sp->cooldown == 0 )
		sp->cooldown = info->StartRecoveryTime; //avoid spell spaming
	if( sp->cooldown == 0 )
		sp->cooldown = info->StartRecoveryCategory; //still 0 ?
	if( sp->cooldown == 0 )
		sp->cooldown = PET_SPELL_SPAM_COOLDOWN; //omg, avoid spaming at least
	sp->cooldowntime = 0;
	
	if(info->Effect[0] == SPELL_EFFECT_APPLY_AURA || info->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA)
		sp->spellType = STYPE_BUFF;
	else 
		sp->spellType = STYPE_DAMAGE;
	
	sp->spelltargetType = info->ai_target_type;
	sp->autocast_type = GetAutoCastTypeForSpell(info);
	sp->procCount = 0;
	m_AISpellStore[info->Id] = sp;
	return sp;
}

void Pet::LoadFromDB(Player* owner, PlayerPet * pi)
{
	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	mPi = pi;
	creature_info = CreatureNameStorage.LookupEntry(mPi->entry);

	Create(pi->name.c_str(), owner->GetMapId(), owner->GetPositionX() + 2 , owner->GetPositionY() +2, owner->GetPositionZ(), owner->GetOrientation());

	LoadValues(mPi->fields.c_str());

	m_PetNumber = mPi->number;
	m_PetXP = mPi->xp;
	m_name = mPi->name;
	Summon = mPi->summon;
	SetIsPet(true);

	m_HappinessTimer = mPi->happinessupdate;
	m_LoyaltyTimer = mPi->loyaltyupdate;
	LoyaltyPts = mPi->loyaltypts;
	LoyaltyXP = mPi->loyaltyxp;

	reset_time = mPi->reset_time;
	reset_cost = mPi->reset_cost;

	bExpires = false;

	// Setup actionbar
	if(mPi->actionbar.size() > 2)
	{
		char * ab = strdup(mPi->actionbar.c_str());
		char * p = strchr(ab, ',');
		char * q = ab;
		uint32 spellid;
		uint32 spstate;
		uint32 i = 0;

		while(p && i<10)
		{
			*p = 0;

			if(sscanf(q, "%u %u", &spellid, &spstate) != 2)
				break;

			ActionBar[i] = spellid;
			//SetSpellState(dbcSpell.LookupEntry(spellid), spstate);
			if(!(ActionBar[i] & 0x4000000) && spellid)
				mSpells[dbcSpell.LookupEntry(spellid)] = spstate;

			i++;

			q = p+1;
			p = strchr(q, ',');
		}

		free(ab);
	}
	
	//Preventing overbuffs
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, 0 );
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER_MODS, 0 );
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 2000 );

	if( m_Owner )
	{
		if( getLevel() > m_Owner->getLevel() )
		{
			SetUInt32Value(UNIT_FIELD_LEVEL, m_Owner->getLevel());
			SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
			SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP(m_Owner->getLevel()));
			ApplyStatsForLevel();
		}
		else if(m_uint32Values[UNIT_CREATED_BY_SPELL])
			ApplySummonLevelAbilities();
		else
			ApplyPetLevelAbilities();
	}

	InitializeMe(false);
	
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
	// Nuke auras
	for(uint32 x = UNIT_FIELD_AURA_01; x <= UNIT_FIELD_AURA_55; x++)
		SetUInt32Value(x, 0);

	//before we initialize pet spells so we can apply spell mods on them 
	if( m_Owner )
		m_Owner->EventSummonPet( this );

	Creature::OnPushToWorld();
}

void Pet::InitializeMe(bool first)
{
	// set up ai and shit
	GetAIInterface()->Init(this,AITYPE_PET,MOVEMENTTYPE_NONE,m_Owner);
	GetAIInterface()->SetUnitToFollow(m_Owner);
	GetAIInterface()->SetFollowDistance(3.0f);

	SetCreatureInfo(CreatureNameStorage.LookupEntry(GetEntry()));
	proto=CreatureProtoStorage.LookupEntry(GetEntry());
	m_Owner->SetSummon(this);
	m_Owner->SetUInt64Value(UNIT_FIELD_SUMMON, this->GetGUID());
	SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());
	SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME);
	if( GetCreatureInfo() )
		myFamily = dbcCreatureFamily.LookupEntry(GetCreatureInfo()->Family);
	else myFamily = NULL;
	bHasLoyalty = m_Owner->getClass() == HUNTER ? true : false;
	SetPetDiet();
	_setFaction();
	m_State = 1;		// dont set agro on spawn

	if(GetEntry() == 416)
		m_aiInterface->disable_melee = true;

	if(GetEntry() == 17252) //felguard
	{
		SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 23904 );
		SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO, 33489154 );
		SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO_01, 273 );
	}

	// Load our spells
	if(Summon)
	{
		SetDefaultSpells();
		// Adds parent +frost spell damage
		if(m_uint32Values[OBJECT_FIELD_ENTRY] == WATER_ELEMENTAL)
		{
			float parentfrost = (float)m_Owner->GetDamageDoneMod(SCHOOL_FROST);
			parentfrost *= 0.40f;
			ModDamageDone[SCHOOL_FROST] = (uint32)parentfrost;
		}
	}
	else
	{
		// Pull from database... :/
		QueryResult * query = CharacterDatabase.Query("SELECT * FROM playerpetspells WHERE ownerguid=%u and petnumber=%u",
			m_Owner->GetLowGUID(), m_PetNumber);
		if(query)
		{
			do 
			{
				Field * f = query->Fetch();
				SpellEntry* spell = dbcSpell.LookupEntry( f[2].GetUInt32() );
				uint16 flags = f[3].GetUInt16();
				if( spell != NULL && mSpells.find( spell ) == mSpells.end() )
					mSpells.insert( make_pair( spell, flags ) );

			} while(query->NextRow());
		}
		delete query;
		UpdateTP();
	}

	InitializeSpells(); 
	PushToWorld(m_Owner->GetMapMgr());
	
	if(first)
	{
		// Set up default actionbar
		SetDefaultActionbar();
	}

	SendSpellsToOwner();

	// set to active
	if( !bExpires )
		UpdatePetInfo( false );

	sEventMgr.AddEvent( this, &Pet::HandleAutoCastEvent, AUTOCAST_EVENT_ON_SPAWN,		EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	sEventMgr.AddEvent( this, &Pet::HandleAutoCastEvent, AUTOCAST_EVENT_LEAVE_COMBAT,	EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void Pet::UpdatePetInfo(bool bSetToOffline)
{
	if(bExpires || m_Owner==NULL)
		return;	// don't update expiring pets

	PlayerPet *pi = m_Owner->GetPlayerPet(m_PetNumber);
	pi->active = !bSetToOffline;
	pi->entry = GetEntry();
	std::stringstream ss;
	for( uint32 index = 0; index < UNIT_END; index ++ )
	{
		if(index >= UNIT_FIELD_AURA_01 && index <= UNIT_FIELD_AURA_55)
			ss << "0 ";
		else
			ss << GetUInt32Value(index) << " ";
	}
	pi->fields = ss.str();
	pi->name = GetName();
	pi->number = m_PetNumber;
	pi->xp = m_PetXP;
	pi->level = GetUInt32Value(UNIT_FIELD_LEVEL);
	pi->loyaltyxp = LoyaltyXP;
	pi->happinessupdate = m_HappinessTimer;
	pi->loyaltypts = LoyaltyPts;
	pi->loyaltyupdate = m_LoyaltyTimer;
	pi->loyaltylvl = GetLoyaltyLevel();

	// save actionbar
	ss.rdbuf()->str("");
	for(uint32 i = 0; i < 10; ++i)
	{
		if(ActionBar[i] & 0x4000000)
			ss << ActionBar[i] << " 0";
		else if(ActionBar[i])
			ss << ActionBar[i] << " " << uint32(GetSpellState(ActionBar[i]));
		else
			ss << "0 0";

		ss << ",";
	}

	pi->actionbar = ss.str();
	pi->summon = Summon;
	pi->reset_cost = reset_cost;
	pi->reset_time = reset_time;
}

void Pet::Dismiss(bool bSafeDelete)//Abandon pet
{
	// Delete any petspells for us.
	if(!Summon && m_Owner)
	{
		CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid=%u AND petnumber=%u",
			m_Owner->GetLowGUID(), m_PetNumber);
	}

	if(m_Owner)
	{
		m_Owner->RemovePlayerPet( m_PetNumber );
	}

	// find out playerpet entry, delete it
	Remove(bSafeDelete, false, true);
}

void Pet::Remove(bool bSafeDelete, bool bUpdate, bool bSetOffline)
{
	RemoveAllAuras(); // Prevent pet overbuffing
	if( m_Owner )
	{
		// remove association with player
		m_Owner->SetUInt64Value(UNIT_FIELD_SUMMON, 0);

		m_Owner->EventDismissPet();

		if(bUpdate) 
		{
			if(!bExpires) 
				UpdatePetInfo(bSetOffline);
			if(!IsSummon())
				m_Owner->_SavePet(NULL);//not perfect but working
		}
		m_Owner->SetSummon(NULL);
		SendNullSpellsToOwner();
	}
	ClearPetOwner();
/*	if(bSafeDelete)
		
	else*/
/*		PetSafeDelete();*/

	// has to be next loop - reason because of RemoveFromWorld, iterator gets broke.
	if(IsInWorld() && Active) Deactivate(m_mapMgr);
	sEventMgr.AddEvent(this, &Pet::PetSafeDelete, EVENT_CREATURE_SAFE_DELETE, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Pet::PetSafeDelete()
{
	if(this->IsInWorld())
	{
		// remove from world, and delete
		RemoveFromWorld(false, false);
	}

	//sEventMgr.AddEvent(World::getSingletonPtr(), &World::DeleteObject, ((Object*)this), EVENT_CREATURE_SAFE_DELETE, 1000, 1);
	Creature::SafeDelete();
}

void Pet::DelayedRemove( bool bTime, bool bDeath )
{
	if( bTime )
	{
		m_Owner = objmgr.GetPlayer( ( uint32 )this->m_OwnerGuid ); // if we need this, then it should be here
		if( GetUInt32Value( UNIT_CREATED_BY_SPELL ) > 0 /*|| bDeath*/ ) // why should we perma delete our dead pet??
			Dismiss( true );  // remove us..
		else
			Remove( true, true, false );
	}
	else
		sEventMgr.AddEvent(this, &Pet::DelayedRemove, true, bDeath, EVENT_PET_DELAYED_REMOVE, PET_DELAYED_REMOVAL_TIME, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Pet::GiveXP( uint32 xp )
{
	if( !m_Owner || Summon )
		return;	

	if( GetLoyaltyLevel() < BEST_FRIEND && LoyaltyXP < m_Owner->GetUInt32Value( PLAYER_NEXT_LEVEL_XP ) / 20 )
	{
		LoyaltyXP += xp;
		UpdateLoyalty( 0 );
	}
	
	if( getLevel() >= m_Owner->getLevel() )		//pet do not get xp if its level >= owners level
		return;

	xp += m_uint32Values[UNIT_FIELD_PETEXPERIENCE];
	uint32 nxp = m_uint32Values[UNIT_FIELD_PETNEXTLEVELEXP];
	bool changed = false;

	while(xp >= nxp)
	{
		ModUnsigned32Value(UNIT_FIELD_LEVEL, 1);
		xp -= nxp;
		nxp = GetNextLevelXP(m_uint32Values[UNIT_FIELD_LEVEL]);
		changed = true;
	}

	SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, xp);
	SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, nxp);
	if(changed) ApplyStatsForLevel();
}

uint32 Pet::GetNextLevelXP(uint32 level)
{
	// Pets need only 1/6 of xp to level up compared to players
	uint32 nextLvlXP = 0;
	if( level > 0 && level <= MAX_PREDEFINED_NEXTLEVELXP )
	{
		nextLvlXP = NextLevelXp[ level - 1 ];
	}
	else
	{
		nextLvlXP = ((int)((((double)(((8 * level) + ((level - 30) * 5)) * ((level * 5) + 45)))/100)+0.5))*100;
	}
	double xp = double(nextLvlXP) / 6;
	return FL2UINT(xp);
}

void Pet::SetDefaultSpells()
{
	if(Summon)
	{
		// this one's easy :p we just pull em from the owner.
		map<uint32, set<uint32> >::iterator it1;
		set<uint32>::iterator it2;
		it1 = m_Owner->SummonSpells.find(GetEntry());
		if(it1 != m_Owner->SummonSpells.end())
		{
			it2 = it1->second.begin();
			for(; it2 != it1->second.end(); ++it2)
			{
				AddSpell( dbcSpell.LookupEntry( *it2 ), false );
			}
		}
	}
	else
	{
		uint32 Line;
		if( GetCreatureInfo() )
			Line = GetCreatureInfo()->SpellDataID;
		else Line = 0;
		if( Line )
		{
			CreatureSpellDataEntry * SpellData = dbcCreatureSpellData.LookupEntry( Line );
			if( SpellData )
				for( uint32 i = 0; i < 3; ++i )
					if( SpellData->Spells[i] != 0 )
						AddSpell( dbcSpell.LookupEntry( SpellData->Spells[i] ), false ); //add spell to pet
		}
	}
}

void Pet::AddSpell( SpellEntry * sp, bool learning )
{
	if( sp == NULL )
		return;

	// Cast on self if we're a passive spell
	if( sp->Attributes & ATTRIBUTES_PASSIVE )
	{
		if(IsInWorld())
		{
			Spell * spell = SpellPool.PooledNew();
			spell->Init(this, sp, true, false);
			SpellCastTargets targets(this->GetGUID());
			spell->prepare(&targets);
			mSpells[sp] = 0x0100;
		}	
	}
	else
	{
	   // Active spell add to the actionbar.
		bool has=false;
		for(int i = 0; i < 10; ++i)
		{
			if(ActionBar[i] == sp->Id)
			{
				has=true;
				break;
			}
		}

		if(!has)
		{
			for(int i = 0; i < 10; ++i)
			{
				if(ActionBar[i] == 0)
				{
					ActionBar[i] = sp->Id;
					break;
				}
			}
		}

		bool done=false;
		if(learning)
		{
			for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
			{
				if(sp->NameHash == itr->first->NameHash)
				{
					// replace the action bar
					for(int i = 0; i < 10; ++i)
					{
						if(ActionBar[i] == itr->first->Id)
						{
							ActionBar[i] = sp->Id;
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

					if(asp->autocast_type==AUTOCAST_EVENT_ON_SPAWN)
						CastSpell(this, sp, false);

					RemoveSpell(itr->first);
					done=true;
					break;
				}
			}
		}

		if(done==false)
		{
			if(mSpells.find(sp) != mSpells.end())
				return;

			if(learning)
			{
				AI_Spell * asp = CreateAISpell(sp);
				uint16 ss = (asp->autocast_type > 0) ? AUTOCAST_SPELL_STATE : DEFAULT_SPELL_STATE;
				mSpells[sp] = ss;
				if(ss==AUTOCAST_SPELL_STATE)
					SetAutoCast(asp,true);

				if(asp->autocast_type==AUTOCAST_EVENT_ON_SPAWN)
					CastSpell(this, sp, false);
			}
			else
				mSpells[sp] = DEFAULT_SPELL_STATE;
		}
	}
	if(IsInWorld())
		SendSpellsToOwner();
	UpdateTP();
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

void Pet::WipeSpells()
{
	while( mSpells.size() > 0 )
	{
		RemoveSpell( mSpells.begin()->first );
	}
	SendSpellsToOwner();
}

void Pet::RemoveSpell(SpellEntry * sp)
{
	mSpells.erase(sp);
	map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.find(sp->Id);
	if(itr != m_AISpellStore.end())
	{
		for(list<AI_Spell*>::iterator it = m_aiInterface->m_spells.begin(); it != m_aiInterface->m_spells.end(); ++it)
		{
			if((*it) == itr->second)
			{
				/*if((*it)->autocast_type > 0)
					m_autoCastSpells[(*it)->autocast_type].remove((*it));*/
				if((*it)->autocast_type > 0)
				{
					for(list<AI_Spell*>::iterator i3 = m_autoCastSpells[(*it)->autocast_type].begin();
						i3 != m_autoCastSpells[(*it)->autocast_type].end(); ++i3)
					{
						if( (*i3) == itr->second )
						{
							m_autoCastSpells[(*it)->autocast_type].erase(i3);
							break;
						}
					}
				}

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
	UpdateTP();
}

void Pet::Rename(string NewName)
{
	m_name = NewName;
	// update petinfo
	UpdatePetInfo(false);

	// update timestamp to force a re-query
	SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME);

	// save new summoned name to db (.pet renamepet)
	if(m_Owner->getClass() == WARLOCK) {
		CharacterDatabase.Execute("UPDATE `playersummons` SET `name`='%s' WHERE `ownerguid`=%u AND `entry`=%u",
			m_name.data(), m_Owner->GetLowGUID(), GetEntry());
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

	switch(m_uint32Values[OBJECT_FIELD_ENTRY])
	{
	case 416: //Imp
		stat_index = 0;
		m_aiInterface->disable_melee = true;
		break;
	case 1860: //VoidWalker
		stat_index = 1;
		break;
	case 1863: //Succubus
		stat_index = 2;
		break;
	case 417: //Felhunter
		stat_index = 3;
		break;
	case 11859: // Doomguard
	case 89:	// Infernal
	case 17252: // Felguard
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
	case 510:	// Mage's water elemental
		stat_index = 5;
		m_aiInterface->disable_melee = true;
		break;
	case 19668:    // Priest's Shadowfiend, until someone knows the stats that real have
		stat_index = 5;
		break;
	}
	if(m_uint32Values[OBJECT_FIELD_ENTRY] == 89)
		has_mana = false;

	if(stat_index < 0)
	{
		sLog.outError("PETSTAT: No stat index found for entry %u, `%s`!", GetEntry(), GetCreatureInfo()->Name);
		return;
	}

	/*
	----------------------------
	------- Warlock Pets -------
	----------------------------
	pet_str = base_str + pet_lvl * mod_str
	pet_agi = base_agi + pet_lvl * mod_agi
	pet_sta = base_sta + pet_lvl * mod_sta + pet_sta_bonus
	pet_int = base_int + pet_lvl * mod_int + pet_int_bonus
	pet_spr = base_spr + pet_lvl * mod_spr
	pet_pwr = base_pwr + pet_lvl * mod_pwr
	pet_arm = base_arm + pet_lvl * mod_arm + pet_arm_bonus

	pet_sta_bonus = 0.3 * plyr_sta + sta_buffs
	pet_int_bonus = 0.3 * plyr_int + int_buffs
	pet_arm_bonus = 0.35 * plyr_arm + arm_buffs

	--[Imp]--
	base_str = 18.0	
	mod_str = 2.0
	base_agi = 20.2	
	mod_agi = 0.2
	base_sta = 19.5	
	mod_sta = 0.5
	base_int = 21.0	
	mod_int = 3.0
	base_spr = 19.0	
	mod_spr = 4.0
	base_pwr = 9.0	
	mod_pwr = 1.0
	base_arm = 47.0	
	mod_arm = 13.0

	--[Voidwalker]--
	base_str = 19.0	
	mod_str = 2.0
	base_agi = 15.0	
	mod_agi = 1.0
	base_sta = -13.0	
	mod_sta = 4.0
	base_int = 17.3	
	mod_int = 1.7
	base_spr = 9.0	
	mod_spr = 2.0
	base_pwr = -16.0
	mod_pwr = 4.0
	base_arm = 177.0	
	mod_arm = 50.0

	--[Succubus]--
	base_str = 2.0	
	mod_str = 2.0
	base_agi = 16.0	
	mod_agi = 1.0
	base_sta = -17.0	
	mod_sta = 4.5
	base_int = 9.0	
	mod_int = 2.0
	base_spr = 12.0	
	mod_spr = 1.3
	base_pwr = -26.0	
	mod_pwr = 4.0
	base_arm = 125.0	
	mod_arm = 23.0

	--[Felhunter]--
	base_str = -3.0	
	mod_str = 2.0
	base_agi = 16.0	
	mod_agi = 1.0
	base_sta = 36.0	
	mod_sta = 3.0
	base_int = 9.0	
	mod_int = 2.0
	base_spr = 21.0	
	mod_spr = 1.0
	base_pwr = -26.0	
	mod_pwr = 4.0
	base_arm = -910.0
	mod_arm = 57.0

	--[Doomguard]--
	base_str = 9.0	
	mod_str = 2.0
	base_agi = -5.0	
	mod_agi = 1.5
	base_sta = -66.0
	mod_sta = 5.0
	base_int = 10.0
	mod_int = 1.0
	base_spr = 30.0
	mod_spr = 2.0
	base_pwr = -62.0
	mod_pwr = 5.0
	base_arm = -187.0
	mod_arm = 65.0

	--[Infernal]--
	base_str = -14.0
	mod_str = 2.5
	base_agi = -20.0
	mod_agi = 2.0
	base_sta = -44.0
	mod_sta = 5.0
	base_int = 20.0	
	mod_int = 0.5
	base_spr = -40.0
	mod_spr = 2.0
	base_pwr = -48.0
	mod_pwr = 5.0
	base_arm = 1605.0
	mod_arm = 50.0

	--[Felguard]--
	base_str = -39.0
	mod_str = 3.0
	base_agi = 7.0	
	mod_agi = 1.5
	base_sta = -14.0	
	mod_sta = 6.0
	base_int = 82.0	
	mod_int = 2.0
	base_spr = -12.0	
	mod_spr = 2.0
	base_pwr = -68.0	
	mod_pwr = 5.5
	base_arm = 1208.0
	mod_arm = 47.0
	*/

	// Imp,Voidwalker,Succubus,Felhunter,Doomguard,Infernal,Felguard
	/*static double R_base_str[7] = { 18.1884058,-15,-15,-15,-15, -14, -39 };
	static double R_mod_str[7] = { 1.811594203,2.4,2.4,2.4,2.4, 2.5, 3 };
	static double R_base_agi[7] = { 20.2, 15, 16, 16, -5, -20, 7 };
	static double R_mod_agi[7] = { 0.2, 1, 1, 1, 1.5, 2, 1.5 };
	static double R_base_sta[7] = { 19.5, -13, -17, 36, -66, -44, -14 };
	static double R_mod_sta[7] = { 0.5, 4, 4.5, 3, 5, 5, 6 };
	static double R_base_int[7] = { 21, 17.3, 9, 9, 10, 20, 82 };
	static double R_mod_int[7] = { 3, 1.7, 2, 2, 1, 0.5, 2 };
	static double R_base_spr[7] = { 19, 9, 12, 21, 30, -40, -12 };
	static double R_mod_spr[7] = { 4, 2, 1.3, 1, 2, 2, 2 };
	static double R_base_pwr[7] = { 9, -16, -26, -26, -62, -48, -68 };
	static double R_mod_pwr[7] = { 1, 4, 4, 4, 5, 5, 5.5 };
	static double R_base_arm[7] = { 47, 177, 125, -910, -187, 1605, 1208 };
	static double R_mod_arm[7] = { 13, 50, 23, 57, 65, 50, 47 };

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
	double base_arm = R_base_arm[stat_index];
	double mod_arm = R_mod_arm[stat_index];

	// Calculate bonuses
	double pet_sta_bonus = 0.3 * (double)m_Owner->BaseStats[STAT_STAMINA];
	double pet_int_bonus = 0.3 * (double)m_Owner->BaseStats[STAT_INTELLECT];
	double pet_arm_bonus = 0.35 * (double)m_Owner->BaseResistance[0];

	double pet_str = base_str + pet_level * mod_str;
	double pet_agi = base_agi + pet_level * mod_agi;
	double pet_sta = base_sta + pet_level * mod_str + pet_sta_bonus;
	double pet_int = base_int + pet_level * mod_int + pet_int_bonus;
	double pet_spr = base_spr + pet_level * mod_spr;
	double pet_pwr = base_pwr + pet_level * mod_pwr;
	double pet_arm = base_arm + pet_level * mod_arm;
	
	// Calculate actual values.
	BaseStats[STAT_STRENGTH] = FL2UINT(pet_str);
	BaseStats[STAT_AGILITY] = FL2UINT(pet_agi);
	BaseStats[STAT_STAMINA] = FL2UINT(pet_sta);
	BaseStats[STAT_INTELLECT] = FL2UINT(pet_int);
	BaseStats[STAT_SPIRIT] = FL2UINT(pet_spr);
	
	for(uint32 x = 0; x < 5; ++x)
		CalcStat(x);

	// Apply armor and attack power.
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, FL2UINT(pet_pwr));
	BaseResistance[0] = FL2UINT(pet_arm);
	CalcResistance(0);
	CalcDamage();

	// Calculate health / mana
	double health = pet_sta * 10;
	double mana = has_mana ? (base_int * 15) : 0.0;
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, FL2UINT(health));
	SetUInt32Value(UNIT_FIELD_BASE_MANA, FL2UINT(mana));*/

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
	/*
	----------[Pets]----------
	Family			  pet_mod_sta		pet_mod_arm			pet_mod_dps			family_aura
	(1)	 Wolf			 1.00				1.05				1.00				17223
	(2)	 Cat			 0.98				1.00				1.10				17210
	(3)	 Spider			 1.00				1.00				1.07				17129
	(4)	 Bear			 1.08				1.05				0.91				17208
	(5)	 Boar			 1.04				1.09				0.90				7000
	(6)	 Crocolisk		 0.95				1.10				1.00				17212
	(7)	 Carrion Bird	 1.00				1.05				1.00				17209
	(8)	 Crab			 0.96				1.13				0.95				17211
	(9)	 Gorilla		 1.04				1.00				1.02				17214
	(10)
	(11) Raptor			 0.95				1.03				1.10				17217
	(12) Tallstrider	 1.05				1.00				1.00				17220
	(13)
	(14)
	(15) Felhunter
	(16) Voidwalker
	(17) Succubus
	(18)
	(19) Doomguard
	(20) Scorpid		 1.00				1.10				0.94				17218
	(21) Turtle			 1.00				1.13				0.90				17221
	(22)
	(23) Imp
	(24) Bat			 1.00				1.00				1.07				17206
	(25) Hyena			 1.00				1.05				1.00				17215
	(26) Owl			 1.00				1.00				1.07				17216
	(27) Wind Serpent	 1.00				1.00				1.07				17222
	(28) Remote Control
	(29) Felguard
	(30) Dragonhawk		 1.00				1.00				1.00				34887
	(31) Ravager		 0.93				1.05				1.10				35257
	(32) Warp Stalker	 1.00				1.05				0.94				35254
	(33) Spore Bat		 1.00				1.00				1.00				35258
	(34) Nether Ray		 1.10				0.90				1.03				35253
	(35) Serpent		 1.00				1.00				1.00				35383
	*/
	
	uint32 pet_family = GetCreatureInfo()->Family;
	uint32 level = getLevel();
	if( level > PLAYER_LEVEL_CAP )
		level = PLAYER_LEVEL_CAP;
	else if (level < 0) level = 1;
	static uint32 family_aura[36] = { 0, 17223, 17210, 17129, 17208, 7000, 17212, 17209, 17211, 17214, 0, 17217, 17220, 0, 0, 0, 0, 0, 0, 0, 17218, 17221, 0, 0, 17206, 17215, 17216, 17222, 0, 0, 34887, 35257, 35254, 35258, 35253, 35383 };
		
	RemoveAura( family_aura[ pet_family ] );//If the pet gained a level, we need to remove the auras to re-calculate everything.
	LoadPetAuras(-1);//These too

	//Base attributes from http://petopia.brashendeavors.net/html/art...ttributes.shtml
	static uint32 R_pet_base_armor[PLAYER_LEVEL_CAP] = { 15, 16, 41, 76, 120, 174, 239, 316, 406, 512, 538, 573, 608, 642, 677, 713, 748, 782, 817, 852, 888, 922, 957, 992, 1026, 1061, 1097, 1130, 1165, 1200, 1234, 1270, 1304, 1340, 1373, 1480, 1593, 1709, 1834, 1964, 2101, 2246, 2397, 2557, 2725, 2780, 2835, 2888, 2944, 2999, 3052, 3108, 3163, 3216, 3271, 3327, 3380, 3435, 3489, 3791, 4091, 4391, 4691, 4991, 5291, 5591, 5892, 6192, 6492, 6792, 
#if PLAYER_LEVEL_CAP==80
		7092,7392,7692,7992,8292,8592,8892,9192,9492,9792,
#endif
	};
	static uint32 R_pet_base_str[PLAYER_LEVEL_CAP] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 40, 42, 44, 45, 47, 49, 50, 52, 53, 55, 56, 58, 60, 61, 63, 64, 66, 67, 69, 70, 72, 74, 76, 78, 81, 86, 91, 97, 102, 104, 106, 108, 110, 113, 115, 117, 119, 122, 124, 127, 129, 131, 134, 136, 139, 141, 144, 146, 149, 151, 154, 156, 159, 162,
#if PLAYER_LEVEL_CAP==80
		165,168,171,174,177,181,184,187,190,193,
#endif
	};
	static uint32 R_pet_base_agi[PLAYER_LEVEL_CAP] = { 15, 16, 16, 16, 17, 18, 18, 19, 20, 20, 20, 21, 23, 23, 24, 25, 26, 27, 28, 30, 30, 30, 32, 33, 34, 35, 36, 37, 38, 40, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 55, 57, 57, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 82, 87, 92, 97, 102, 107, 112, 117, 122, 127, 
#if PLAYER_LEVEL_CAP==80
		131,136,141,146,151,156,161,166,171,176,
#endif
	};
	static uint32 R_pet_base_sta[PLAYER_LEVEL_CAP] = { 22, 24, 25, 27, 28, 30, 32, 34, 36, 38, 40, 43, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 79, 83, 88, 93, 98, 103, 109, 114, 119, 124, 129, 134, 140, 146, 152, 158, 164, 170, 177, 183, 190, 196, 203, 210, 217, 224, 232, 240, 247, 255, 263, 271, 279, 288, 296, 305, 314, 323, 332, 342, 351, 361, 370, 380, 391, 401, 412, 423, 
#if PLAYER_LEVEL_CAP==80
		434,445,456,467,478,489,501,512,523,534,
#endif
	};
	static uint32 R_pet_base_int[PLAYER_LEVEL_CAP] = { 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 
#if PLAYER_LEVEL_CAP==80
		34,35,36,37,38,39,40,41,42,43,
#endif
	};
	static uint32 R_pet_base_spi[PLAYER_LEVEL_CAP] = { 20, 20, 21, 21, 21, 21, 22, 22, 22, 23, 23, 24, 25, 26, 27, 28, 29, 29, 31, 32, 32, 34, 34, 36, 37, 37, 39, 39, 41, 42, 42, 44, 44, 46, 47, 48, 49, 49, 51, 52, 53, 54, 55, 56, 58, 58, 60, 60, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75, 78, 80, 84, 86, 88, 91, 93, 95, 96, 99, 
#if PLAYER_LEVEL_CAP==80
		102,105,108,111,114,117,120,123,126,129,
#endif
	};

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
	if( pet_family > 35 )
		sLog.outError( "PETSTAT: Creature family %i [%s] has missing data.", pet_family, myFamily->name );
	else if( family_aura[ pet_family ] != 0 )
		this->CastSpell( this, family_aura[ pet_family ], true );
		
	for( uint32 x = 0; x < 5; ++x )
		CalcStat( x );
	
	UpdateTP();
	LoadPetAuras(-2);//Load all BM auras
}

void Pet::ApplyStatsForLevel()
{
	if(m_uint32Values[UNIT_CREATED_BY_SPELL])// Summon
		ApplySummonLevelAbilities();
	else
		ApplyPetLevelAbilities();

	// Apply common stuff
	// Apply scale for this family.
	if (myFamily) {
		float pet_level = float(m_uint32Values[UNIT_FIELD_LEVEL]);
		float level_diff = float(myFamily->maxlevel - myFamily->minlevel);
		float scale_diff = float(myFamily->maxsize - myFamily->minsize);
		float factor = scale_diff / level_diff;
		float scale = factor * pet_level + myFamily->minsize;
		SetFloatValue(OBJECT_FIELD_SCALE_X, scale);
	}

	// Apply health fields.
	SetUInt32Value(UNIT_FIELD_HEALTH, m_uint32Values[UNIT_FIELD_MAXHEALTH]);
	SetUInt32Value(UNIT_FIELD_POWER1, m_uint32Values[UNIT_FIELD_MAXPOWER1]);
	SetUInt32Value(UNIT_FIELD_POWER3, m_uint32Values[UNIT_FIELD_MAXPOWER3]);	
}

void Pet::LoadPetAuras(int32 id)
{
	/*
	   Talent			   Aura Id
	Unleashed Fury			8875
	Thick Hde				19580
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
	if( m_uint32Values[ UNIT_CREATED_BY_SPELL ] )
		return;//Only for hunter's. Gtfo warlock's

	uint32 str = GetUInt32Value( UNIT_FIELD_STAT0 );
	uint32 AP = ( str * 2 - 20 );
	if( m_Owner )
		AP += m_Owner->GetRAP() * 22 / 100;
	if( AP < 0 ) AP = 0;
	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, AP );
}

uint16 Pet::SpellTP(uint32 spellId)
{
	//returns required training points for spell
	skilllinespell *sk = objmgr.GetSpellSkill(spellId);
	if(sk)
		return static_cast<uint16>(sk->reqTP);
	return 0;
}
uint16 Pet::GetUsedTP()
{
	//goes through all pet spells and sums training points
	int16 sumTP = 0;
	if(mSpells.size() > 0)
	{
		PetSpellMap::iterator itr = mSpells.begin();
		for(; itr != mSpells.end(); ++itr)
		{
			//higher spell ranks takes TP incrementally from lower rank, so lets sum the highest rank spells
			if(itr->first->Id == GetHighestRankSpell(itr->first->Id))
				sumTP += SpellTP(itr->first->Id);
		}
	}
	return sumTP;
}
uint32 Pet::CanLearnSpell( SpellEntry * sp )
{
	// level requirement
	if( getLevel() < sp->spellLevel )
		return SPELL_FAILED_LEVEL_REQUIREMENT;

	if( Summon )
		return 0;
	
	/** Hunter pet specific checks:
		- number of active spells, max 4 are allowed per pet */
	if( mSpells.size() > 0 && !( sp->Attributes & ATTRIBUTES_PASSIVE ) )
	{
		uint8 cnt = 0;
		PetSpellMap::iterator itr = mSpells.begin();
		for( ; itr != mSpells.end(); ++itr )
		{
			if( itr->first->NameHash == sp->NameHash )
			{
				cnt = 0;			
				break;
			}
			if( !( itr->first->Attributes & ATTRIBUTES_PASSIVE ) )
				cnt++;
		}
		if( cnt > 3 )
			return SPELL_FAILED_TOO_MANY_SKILLS;
	}
	/** - available training points
			- higher spell ranks take TPs incrementally, so we need this calculation */
	uint16 spellTP = SpellTP( sp->Id );
	if( spellTP > 0 && ( spellTP - SpellTP( GetHighestRankSpell( sp->Id ) ) >  TP ) )
		return SPELL_FAILED_TRAINING_POINTS;
	
	return 0;
}
void Pet::UpdateTP()
{
	//update pets TP
	//formula: TP = level*(loyaltyLvl - 1) - usedTP
	//http://petopia.brashendeavors.net/html/articles/skills_main.shtml
	if(!m_Owner || Summon) return;
	int16 pts = static_cast<int16>(getLevel()*(GetLoyaltyLevel()-1)-GetUsedTP());
	TP = pts;
	SetUInt32Value(UNIT_TRAINING_POINTS, pts < 0?(-pts & 0xffff):(pts<<16));//uff, works, but has anybody better idea?
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
void Pet::AddPetSpellToOwner(uint32 spellId)
{
	// Hunter can learn pets "wild" ability and teach it other pets

	if( Summon || !m_Owner )
		return;
	//exit if owner hasn't Beast training ability (id 5149)
	if( !m_Owner->HasSpell( 5149 ) || !Rand( 10 ) )
		return;
	//all wild abilities require training points (just for sure)
	if( !SpellTP( spellId ) )
		return;
	
	//find appropriate teaching spell...
	uint32 TeachingSpellID = sWorld.GetTeachingSpell( spellId );
	if( TeachingSpellID != 0 )
	{
		if( m_Owner->HasSpell( TeachingSpellID ) )
			return;
		else
		{
			//...and add it to pet owner to be able teach other pets
			m_Owner->addSpell( TeachingSpellID );
			return;
		}
	}
	else
		sLog.outDebug( "WORLD: Could not find teaching spell for spell %u", spellId );
}
uint32 Pet::GetHighestRankSpell(uint32 spellId)
{	
	//get the highest rank of spell from known spells
	SpellEntry *sp = dbcSpell.LookupEntry(spellId);
	SpellEntry *tmp = 0;
	if(sp && mSpells.size() > 0)
	{
		PetSpellMap::iterator itr = mSpells.begin();
		for(; itr != mSpells.end(); ++itr)
			if(sp->NameHash == itr->first->NameHash)
				if((!tmp || tmp->RankNumber < itr->first->RankNumber))
					 tmp = itr->first;
	}
	return tmp ? tmp->Id : 0;
}
bool Pet::UpdateLoyalty( char pts )
{	
	//updates loyalty_pts and loyalty lvl if needed
	uint32 lvl;
#ifdef WIN32
	__try {
		if( !m_Owner || Summon || m_Owner->GetMapMgr() != this->GetMapMgr() )
			return true;

		lvl = m_Owner->GetUInt32Value( PLAYER_NEXT_LEVEL_XP );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		m_Owner = NULL;
		return true;
	}
#else
	if( !m_Owner || Summon || m_Owner->GetMapMgr() != this->GetMapMgr() )
		return true;

	lvl = m_Owner->GetUInt32Value( PLAYER_NEXT_LEVEL_XP );
#endif

	uint8 curLvl = GetLoyaltyLevel();
	uint8 newLvl = curLvl;

	LoyaltyPts += pts;

	if( LoyaltyPts > LoyLvlRange[BEST_FRIEND] )	//cap
		LoyaltyPts = LoyLvlRange[BEST_FRIEND];

	if( LoyaltyPts < 0 && pts != 0 )
		newLvl--;
	else if( curLvl < BEST_FRIEND && LoyaltyPts > LoyLvlRange[ curLvl ] && 	// requires some loyalty pts ...
		LoyaltyXP >= lvl / 20 )	// ... and 5% of hunters nextlevel xp
		newLvl++;
	else return true;

	if( newLvl < REBELIOUS )
	{	
		Dismiss(); // pet runs away
		return false;
	}
	
	SetUInt32Value( UNIT_FIELD_BYTES_1, 0 | ( newLvl << 8 ) );		//set new loy level
	LoyaltyPts = newLvl > curLvl ? 0 : LoyLvlRange[ newLvl ];		//reset loy_pts
	LoyaltyXP = 0;													//reset loy_xp
	UpdateTP();
	return true;
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

		if( sp->spell == NULL || sp->autocast_type != Type ) {
			sLog.outError("Found corrupted spell at m_autoCastSpells, skipping");
			continue;
		}

		if( sp->spelltargetType == TTYPE_OWNER )
		{
			if( !m_Owner->HasAura( sp->spell->Id ) )
				CastSpell( m_Owner, sp->spell, false );
		}
		else
		{
			//modified by Zack: Spell targetting will be generated in the castspell function now.You cannot force to target self all the time
			CastSpell( static_cast< Unit* >( NULL ), sp->spell, false);
		}
		
		AddPetSpellToOwner( sp->spell->Id );
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
	uint32 days = (uint32)(sWorld.GetGameTime() - reset_time)/60*60*24;

	if(reset_cost < 1000 || days > 0)
		reset_cost = 1000;
	else if(reset_cost < 5000)
		reset_cost = 5000;
	else if(reset_cost < 10000)
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





