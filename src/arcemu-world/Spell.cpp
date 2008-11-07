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

#define SPELL_CHANNEL_UPDATE_INTERVAL 1000

/// externals for spell system
extern pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS];
extern pSpellTarget SpellTargetHandler[EFF_TARGET_LIST_LENGTH_MARKER];

extern char* SpellEffectNames[TOTAL_SPELL_EFFECTS];

enum SpellTargetSpecification
{
    TARGET_SPECT_NONE       = 0,
    TARGET_SPEC_INVISIBLE   = 1,
    TARGET_SPEC_DEAD        = 2,
};

void SpellCastTargets::read( WorldPacket & data,uint64 caster )
{
	m_unitTarget = m_itemTarget = 0;
	m_srcX = m_srcY = m_srcZ = m_destX = m_destY = m_destZ = 0;
	m_strTarget = "";

	data >> m_targetMask;
	data >> m_targetMaskExtended;
	WoWGuid guid;

	if( m_targetMask == TARGET_FLAG_SELF )
	{
		m_unitTarget = caster;
	}

	if( m_targetMask & (TARGET_FLAG_OBJECT | TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 ) )
	{
		data >> guid;
		m_unitTarget = guid.GetOldGuid();
	}

	if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
	{
		data >> guid;
		m_itemTarget = guid.GetOldGuid();
	}

	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
	{
		data >> m_srcX >> m_srcY >> m_srcZ;

		if( !( m_targetMask & TARGET_FLAG_DEST_LOCATION ) )
		{
			m_destX = m_srcX;
			m_destY = m_srcY;
			m_destZ = m_srcZ;
		}
	}

	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
	{
		data >> m_destX >> m_destY >> m_destZ;
		if( !( m_targetMask & TARGET_FLAG_SOURCE_LOCATION ) )
		{
			m_srcX = m_destX;
			m_srcY = m_destY;
			m_srcZ = m_destZ;
		}
	}

	if( m_targetMask & TARGET_FLAG_STRING )
	{
		std::string ss;
		data >> ss;
		m_strTarget = strdup(ss.c_str());
	}
}

void SpellCastTargets::write( WorldPacket& data )
{
	data << m_targetMask;
	data << m_targetMaskExtended;

	if( /*m_targetMask == TARGET_FLAG_SELF || */m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT ) )
        FastGUIDPack( data, m_unitTarget );

    if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
        FastGUIDPack( data, m_itemTarget );

	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
		data << m_srcX << m_srcY << m_srcZ;

	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
		data << m_destX << m_destY << m_destZ;

	if( m_targetMask & TARGET_FLAG_STRING )
		data << m_strTarget;
}

Spell::Spell()
{
	m_bufferPoolId = OBJECT_WAS_ALLOCATED_STANDARD_WAY;
}

void Spell::Virtual_Constructor()
{
}

void Spell::Init(Object* Caster, SpellEntry *info, bool triggered, Aura* aur)
{
	//int i;

	ASSERT( Caster != NULL && info != NULL );

	chaindamage = 0;
	bDurSet = 0;
	damage = 0;
	m_spellInfo_override = 0;
	bRadSet[0] = 0;
	bRadSet[1] = 0;
	bRadSet[2] = 0;

	m_spellInfo = info;
	m_spellInfo_override = NULL;
	m_caster = Caster;
	duelSpell = false;

	switch( Caster->GetTypeId() )
	{
		case TYPEID_PLAYER:
		{
			g_caster = NULL;
			i_caster = NULL;
			u_caster = static_cast< Unit* >( Caster );
			p_caster = static_cast< Player* >( Caster );
			if( p_caster->GetDuelState() == DUEL_STATE_STARTED )
				duelSpell = true;
				
#ifdef GM_Z_DEBUG_DIRECTLY
   	    // cebernic added it
   	    if ( p_caster->GetSession() && p_caster->GetSession()->CanUseCommand('z')  && p_caster->IsInWorld() )
    		sChatHandler.BlueSystemMessage( p_caster->GetSession(), "[%sSystem%s] |rSpell::Spell: %s ID:%u,Category%u,CD:%u,DisType%u,Field4:%u,etA0=%u,etA1=%u,etA2=%u,etB0=%u,etB1=%u,etB2=%u", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE, 
    		info->Id,info->Category,info->RecoveryTime,info->DispelType,info->castUI,info->EffectImplicitTargetA[0],info->EffectImplicitTargetA[1],info->EffectImplicitTargetA[2],info->EffectImplicitTargetB[0],info->EffectImplicitTargetB[1],info->EffectImplicitTargetB[2]  );
#endif
				
		} break;

		case TYPEID_UNIT:
		{
			g_caster = NULL;
			i_caster = NULL;
			p_caster = NULL;
			u_caster = static_cast< Unit* >( Caster );
			if( u_caster->IsPet() && static_cast< Pet* >( u_caster)->GetPetOwner() != NULL && static_cast< Pet* >( u_caster )->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
				duelSpell = true;
		} break;

		case TYPEID_ITEM:
		case TYPEID_CONTAINER:
		{
			g_caster = NULL;
			u_caster = NULL;
			p_caster = NULL;
			i_caster = static_cast< Item* >( Caster );
			if( i_caster->GetOwner() && i_caster->GetOwner()->GetDuelState() == DUEL_STATE_STARTED )
				duelSpell = true;
		} break;

		case TYPEID_GAMEOBJECT:
		{
			u_caster = NULL;
			p_caster = NULL;
			i_caster = NULL;
			g_caster = static_cast< GameObject* >( Caster );
		} break;

		default:
			sLog.outDebug("[DEBUG][SPELL] Incompatible object type, please report this to the dev's");
			break;
	}

	m_spellState = SPELL_STATE_NULL;

	m_castPositionX = m_castPositionY = m_castPositionZ = 0;
	//TriggerSpellId = 0;
	//TriggerSpellTarget = 0;
	m_triggeredSpell = triggered;
	m_AreaAura = false;

	m_triggeredByAura = aur;

	damageToHit = 0;
	castedItemId = 0;

	m_usesMana = false;
	m_Spell_Failed = false;
	m_CanRelect = false;
	m_IsReflected = false;
	hadEffect = false;
	bDurSet = false;
	bRadSet[0] = false;
	bRadSet[1] = false;
	bRadSet[2] = false;

	cancastresult = SPELL_CANCAST_OK;

	m_requiresCP = false;
	unitTarget = NULL;
	itemTarget = NULL;
	gameObjTarget = NULL;
	playerTarget = NULL;
	corpseTarget = NULL;
	judgement = false;
	add_damage = 0;
	m_Delayed = false;
	pSpellId = 0;
	m_cancelled = false;
	ProcedOnSpell = 0;
	forced_basepoints[0] = forced_basepoints[1] = forced_basepoints[2] = 0;
	extra_cast_number = 0;
	m_reflectedParent = NULL;
	m_isCasting = false;

	UniqueTargets.clear();
	ModeratedTargets.clear();
	m_targetUnits[0].clear();
	m_targetUnits[1].clear();
	m_targetUnits[2].clear();
}

Spell::~Spell()
{
		for(uint32 x=0;x<3;x++)
			m_targetUnits[x].clear();
}

void Spell::Virtual_Destructor()
{
	if( u_caster != NULL && u_caster->GetCurrentSpell() == this )
		u_caster->SetCurrentSpell(NULL);

	if( p_caster )
		if( hadEffect || ( cancastresult == SPELL_CANCAST_OK && !GetSpellFailed() ) )
			RemoveItems();

	if (m_spellInfo_override !=NULL)
		delete[] m_spellInfo_override;
}

//i might forget conditions here. Feel free to add them
bool Spell::IsStealthSpell()
{
	//check if aura name is some stealth aura
	if( GetProto()->EffectApplyAuraName[0] == 16 || GetProto()->EffectApplyAuraName[1] == 16 || GetProto()->EffectApplyAuraName[2] == 16 )
		return true;
	return false;
}

//i might forget conditions here. Feel free to add them
bool Spell::IsInvisibilitySpell()
{
	//check if aura name is some invisibility aura
	if( GetProto()->EffectApplyAuraName[0] == 18 || GetProto()->EffectApplyAuraName[1] == 18 || GetProto()->EffectApplyAuraName[2] == 18 )
		return true;
	return false;
}

void Spell::FillSpecifiedTargetsInArea( float srcx, float srcy, float srcz, uint32 ind, uint32 specification )
{
	FillSpecifiedTargetsInArea( ind, srcx, srcy, srcz, GetRadius(ind), specification );
}

// for the moment we do invisible targets
void Spell::FillSpecifiedTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range, uint32 specification)
{
	TargetsList *tmpMap=&m_targetUnits[i];
	//IsStealth()
	float r = range * range;
	uint8 did_hit_result;
	m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		// don't add objects that are not units and that are dead
		if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >( *itr )->isAlive())
			continue;

		//static_cast< Unit* >( *itr )->IsStealth()
		if( GetProto()->TargetCreatureType)
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureInfo();
			if(!inf || !(1<<(inf->Type-1) & GetProto()->TargetCreatureType))
				continue;
		}

		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( u_caster != NULL )
			{
				if( isAttackable( u_caster, static_cast< Unit* >( *itr ), !( GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED ) ) )
				{
					did_hit_result = DidHit(i, static_cast< Unit* >( *itr ) );
					if( did_hit_result != SPELL_DID_HIT_SUCCESS )
						ModeratedTargets.push_back(SpellTargetMod((*itr)->GetGUID(), did_hit_result));
					else
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}

			}
			else //cast from GO
			{
				if ( g_caster && g_caster->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && g_caster->m_summoner )
				{
					//trap, check not to attack owner and friendly
					if(isAttackable(g_caster->m_summoner,(Unit*)(*itr),!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if( GetProto()->MaxTargets )
			{
				if( GetProto()->MaxTargets >= tmpMap->size())
				{
					m_caster->ReleaseInrangeLock();
					return;
				}
			}
		}
	}
	m_caster->ReleaseInrangeLock();
}
void Spell::FillAllTargetsInArea(LocationVector & location,uint32 ind)
{
	FillAllTargetsInArea(ind,location.x,location.y,location.z,GetRadius(ind));
}

void Spell::FillAllTargetsInArea(float srcx,float srcy,float srcz,uint32 ind)
{
	FillAllTargetsInArea(ind,srcx,srcy,srcz,GetRadius(ind));
}

/// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range)
{
	TargetsList *tmpMap=&m_targetUnits[i];
	float r = range*range;
	uint8 did_hit_result;
	std::set<Object*>::iterator itr,itr2;
	m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
	for( itr2 = m_caster->GetInRangeSetBegin(); itr2 != m_caster->GetInRangeSetEnd();)
	{
		itr = itr2;
		itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
		if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >( *itr )->isAlive() || ( static_cast< Creature* >( *itr )->IsTotem() && !static_cast< Unit* >( *itr )->IsPlayer() ) )
			continue;

		if( GetProto()->TargetCreatureType )
		{
			if( (*itr)->GetTypeId()!= TYPEID_UNIT )
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureInfo();
			if( !inf || !( 1 << (inf->Type-1) & GetProto()->TargetCreatureType ) )
				continue;
		}
		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( u_caster != NULL )
			{
				if( isAttackable( u_caster, static_cast< Unit* >( *itr ), !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				{
					did_hit_result = DidHit(i, static_cast< Unit* >( *itr ) );
					if( did_hit_result == SPELL_DID_HIT_SUCCESS )
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
					else
						ModeratedTargets.push_back( SpellTargetMod( (*itr)->GetGUID(), did_hit_result ) );
				}
			}
			else //cast from GO
			{
				if( g_caster != NULL && g_caster->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && g_caster->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( g_caster->m_summoner, static_cast< Unit* >( *itr ), !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if( GetProto()->MaxTargets )
				if( GetProto()->MaxTargets == tmpMap->size() )
				{
					m_caster->ReleaseInrangeLock();
					return;
				}
		}
	}
	m_caster->ReleaseInrangeLock();
}

// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllFriendlyInArea( uint32 i, float srcx, float srcy, float srcz, float range )
{
	TargetsList *tmpMap=&m_targetUnits[i];
	float r = range * range;
	uint8 did_hit_result;
	std::set<Object*>::iterator itr,itr2;
	m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
	for( itr2 = m_caster->GetInRangeSetBegin(); itr2 != m_caster->GetInRangeSetEnd();)
	{
		itr = itr2;
		itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
		if( !((*itr)->IsUnit()) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;

		if( GetProto()->TargetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureInfo();
			if(!inf || !(1<<(inf->Type-1) & GetProto()->TargetCreatureType))
				continue;
		}

		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( u_caster != NULL )
			{
				if( isFriendly( u_caster, static_cast< Unit* >( *itr ) ) )
				{
					did_hit_result = DidHit(i, static_cast< Unit* >( *itr ) );
					if( did_hit_result == SPELL_DID_HIT_SUCCESS )
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
					else
						ModeratedTargets.push_back( SpellTargetMod( (*itr)->GetGUID(), did_hit_result ) );
				}
			}
			else //cast from GO
			{
				if( g_caster != NULL && g_caster->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && g_caster->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( g_caster->m_summoner, static_cast< Unit* >( *itr ) ) )
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if( GetProto()->MaxTargets )
				if( GetProto()->MaxTargets == tmpMap->size() )
				{
					m_caster->ReleaseInrangeLock();
					return;
				}
		}
	}
	m_caster->ReleaseInrangeLock();
}

uint64 Spell::GetSinglePossibleEnemy(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = GetProto()->base_range_or_radius_sqr;
		if( GetProto()->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius,&r,GetProto()->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius,&r,GetProto()->SpellGroupType);
		}
	}
	float srcx = m_caster->GetPositionX(), srcy = m_caster->GetPositionY(), srcz = m_caster->GetPositionZ();
	m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;

		if( GetProto()->TargetCreatureType )
		{
			if( (*itr)->GetTypeId() != TYPEID_UNIT )
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureInfo();
			if(!inf || !(1<<(inf->Type-1) & GetProto()->TargetCreatureType))
				continue;
		}
		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( u_caster != NULL )
			{
				if(isAttackable(u_caster, static_cast< Unit* >( *itr ),!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && DidHit(i,((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
				{
					m_caster->ReleaseInrangeLock();
					return (*itr)->GetGUID();
				}
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( g_caster->m_summoner, static_cast< Unit* >( *itr ),!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
					{
						m_caster->ReleaseInrangeLock();
						return (*itr)->GetGUID();
					}
				}
			}
		}
	}
	m_caster->ReleaseInrangeLock();
	return 0;
}

uint64 Spell::GetSinglePossibleFriend(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = GetProto()->base_range_or_radius_sqr;
		if( GetProto()->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius,&r,GetProto()->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius,&r,GetProto()->SpellGroupType);
		}
	}
	float srcx=m_caster->GetPositionX(),srcy=m_caster->GetPositionY(),srcz=m_caster->GetPositionZ();
	m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;
		if( GetProto()->TargetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureInfo();
				if(!inf || !(1<<(inf->Type-1) & GetProto()->TargetCreatureType))
					continue;
		}
		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( u_caster != NULL )
			{
				if( isFriendly( u_caster, static_cast< Unit* >( *itr ) ) && DidHit(i, ((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
				{
					m_caster->ReleaseInrangeLock();
					return (*itr)->GetGUID();
				}
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( g_caster->m_summoner, static_cast< Unit* >( *itr ) ) )
					{
						m_caster->ReleaseInrangeLock();
						return (*itr)->GetGUID();
					}
				}
			}
		}
	}
	m_caster->ReleaseInrangeLock();
	return 0;
}

uint8 Spell::DidHit(uint32 effindex,Unit* target)
{
	//note resistchance is vise versa, is full hit chance
	Unit* u_victim = target;
	Player* p_victim = ( target->GetTypeId() == TYPEID_PLAYER ) ? static_cast< Player* >( target ) : NULL;

	//
	float baseresist[3] = { 4.0f, 5.0f, 6.0f };
	int32 lvldiff;
	float resistchance ;
	if( u_victim == NULL )
		return SPELL_DID_HIT_MISS;

	/************************************************************************/
	/* Can't resist non-unit                                                */
	/************************************************************************/
	if(!u_caster)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Can't reduce your own spells                                         */
	/************************************************************************/
	if(u_caster == u_victim)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Check if the unit is evading                                         */
	/************************************************************************/
	if(u_victim->GetTypeId()==TYPEID_UNIT && u_victim->GetAIInterface()->getAIState()==STATE_EVADE)
		return SPELL_DID_HIT_EVADE;

	/************************************************************************/
	/* Check if the target is immune to this spell school                   */
	/* Unless the spell would actually dispel invulnerabilities             */
	/************************************************************************/
	int dispelMechanic = GetProto()->Effect[0] == SPELL_EFFECT_DISPEL_MECHANIC && GetProto()->EffectMiscValue[0] == MECHANIC_INVULNERABLE;
	if(u_victim->SchoolImmunityList[GetProto()->School] && !dispelMechanic)
		return SPELL_DID_HIT_IMMUNE;

	/*************************************************************************/
	/* Check if the target is immune to this mechanic                        */
	/*************************************************************************/
	if(u_victim->MechanicsDispels[GetProto()->MechanicsType])
	{
		// Immune - IF, and ONLY IF, there is no damage component!
		bool no_damage_component = true;
		for( int x = 0 ; x <= 2 ; x ++ )
		{
			if( GetProto()->Effect[0] == SPELL_EFFECT_SCHOOL_DAMAGE
				|| GetProto()->Effect[0] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
				|| GetProto()->Effect[0] == SPELL_EFFECT_WEAPON_DAMAGE
				|| GetProto()->Effect[0] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
				|| GetProto()->Effect[0] == SPELL_EFFECT_DUMMY
				|| ( GetProto()->Effect[0] == SPELL_EFFECT_APPLY_AURA &&
					( GetProto()->EffectApplyAuraName[0] == SPELL_AURA_PERIODIC_DAMAGE
					) )
				)
			{
				no_damage_component = false;
				break;
			}
		}
		if( no_damage_component )
			return SPELL_DID_HIT_IMMUNE; // Moved here from Spell::CanCast
	}

	/************************************************************************/
	/* Check if the target has a % resistance to this mechanic              */
	/************************************************************************/
		/* Never mind, it's already done below. Lucky I didn't go through with this, or players would get double resistance. */

	/************************************************************************/
	/* Check if the spell is a melee attack and if it was missed/parried    */
	/************************************************************************/
	uint32 melee_test_result;
	if( GetProto()->is_melee_spell || GetProto()->is_ranged_spell )
	{
		uint32 _type;
		if( GetType() == SPELL_DMG_TYPE_RANGED )
			_type = RANGED;
		else
		{
			if (GetProto()->AttributesExC & 0x1000000)
				_type =  OFFHAND;
			else
				_type = MELEE;
		}

		melee_test_result = u_caster->GetSpellDidHitResult( u_victim, _type, GetProto() );
		if(melee_test_result != SPELL_DID_HIT_SUCCESS)
			return (uint8)melee_test_result;
	}

	/************************************************************************/
	/* Check if the spell is resisted.                                      */
	/************************************************************************/
	if( GetProto()->School == 0  && !GetProto()->MechanicsType )
		return SPELL_DID_HIT_SUCCESS;

	bool pvp =(p_caster && p_victim);

	if(pvp)
		lvldiff = p_victim->getLevel() - p_caster->getLevel();
	else
		lvldiff = u_victim->getLevel() - u_caster->getLevel();
	if (lvldiff < 0)
	{
		resistchance = baseresist[0] +lvldiff;
	}
	else
	{
		if(lvldiff < 3)
		{
				resistchance = baseresist[lvldiff];
		}
		else
		{
			if(pvp)
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*7.0f);
			else
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*11.0f);
		}
	}
	//check mechanical resistance
	//i have no idea what is the best pace for this code
	if( GetProto()->MechanicsType < 31 )
	{
		if(p_victim)
			resistchance += p_victim->MechanicsResistancesPCT[GetProto()->MechanicsType];
		else
			resistchance += u_victim->MechanicsResistancesPCT[GetProto()->MechanicsType];
	}
	//rating bonus
	if( p_caster != NULL )
	{
		resistchance -= p_caster->CalcRating( PLAYER_RATING_MODIFIER_SPELL_HIT );
		resistchance -= p_caster->GetHitFromSpell();
	}

	if(p_victim && GetProto()->School != 0)
		resistchance += p_victim->m_resist_hit[ MOD_SPELL ];

	if( this->GetProto()->Effect[effindex] == SPELL_EFFECT_DISPEL && GetProto()->SpellGroupType && u_caster)
	{
		SM_FFValue(u_victim->SM_FRezist_dispell,&resistchance,GetProto()->SpellGroupType);
		SM_PFValue(u_victim->SM_PRezist_dispell,&resistchance,GetProto()->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		int spell_flat_modifers=0;
		int spell_pct_modifers=0;
		SM_FIValue(u_caster->SM_FRezist_dispell,&spell_flat_modifers,GetProto()->SpellGroupType);
		SM_FIValue(u_caster->SM_PRezist_dispell,&spell_pct_modifers,GetProto()->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell dipell resist mod flat %d , spell dipell resist mod pct %d , spell dipell resist %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,resistchance,GetProto()->SpellGroupType);
#endif
	}

	if(GetProto()->SpellGroupType && u_caster)
	{
		float hitchance=0;
		SM_FFValue(u_caster->SM_FHitchance,&hitchance,GetProto()->SpellGroupType);
		resistchance -= hitchance;
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		SM_FFValue(u_caster->SM_FHitchance,&spell_flat_modifers,GetProto()->SpellGroupType);
		if(spell_flat_modifers!=0 )
			printf("!!!!!spell to hit mod flat %f, spell resist chance %f, spell group %u\n",spell_flat_modifers,resistchance,GetProto()->SpellGroupType);
#endif
	}

	if (GetProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY)
		resistchance = 0.0f;

	if(resistchance >= 100.0f)
		return SPELL_DID_HIT_RESIST;
	else
	{
		uint8 res;
		if(resistchance<=1.0)//resist chance >=1
			res =  (Rand(1.0f) ? SPELL_DID_HIT_RESIST : SPELL_DID_HIT_SUCCESS);
		else
			res =  (Rand(resistchance) ? SPELL_DID_HIT_RESIST : SPELL_DID_HIT_SUCCESS);

		if (res == SPELL_DID_HIT_SUCCESS) // proc handling. mb should be moved outside this function
		{
//			u_caster->HandleProc(PROC_ON_SPELL_LAND,target,GetProto());
		}

		return res;
	}

}
//generate possible target list for a spell. Use as last resort since it is not acurate
//this function makes a rough estimation for possible target !
//!!!disabled parts that were not tested !!
void Spell::GenerateTargets(SpellCastTargets *store_buff)
{
	float r = GetProto()->base_range_or_radius_sqr;
	if( GetProto()->SpellGroupType && u_caster)
	{
		SM_FFValue(u_caster->SM_FRadius,&r,GetProto()->SpellGroupType);
		SM_PFValue(u_caster->SM_PRadius,&r,GetProto()->SpellGroupType);
	}
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = GetProto()->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = GetProto()->EffectImplicitTargetB[i];
			switch(cur)
			{
				case EFF_TARGET_NONE:{
					//this is bad for us :(
					}break;
				case EFF_TARGET_SELF:{
						if(m_caster->IsUnit())
							store_buff->m_unitTarget = m_caster->GetGUID();
					}break;
					// need more research
				case 4:{ // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
					}break;
				case EFF_TARGET_PET:
					{// Target: Pet
						if(p_caster && p_caster->GetSummon())
							store_buff->m_unitTarget = p_caster->GetSummon()->GetGUID();
					}break;
				case EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
				case 77:					// grep: i think this fits
				case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
				case EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
				case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
				case EFF_TARGET_IN_FRONT_OF_CASTER:
				case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
				case 31:// related to scripted effects
				case 53:// Target Area by Players CurrentSelection()
				case 54:// Targets in Front of the Caster
					{
						if( p_caster != NULL )
						{
							Unit *selected = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
							if(isAttackable(p_caster,selected,!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
								store_buff->m_unitTarget = p_caster->GetSelection();
						}
						else if( u_caster != NULL )
						{
							if(	u_caster->GetAIInterface()->GetNextTarget() &&
								isAttackable(u_caster,u_caster->GetAIInterface()->GetNextTarget(),!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) &&
								u_caster->GetDistanceSq(u_caster->GetAIInterface()->GetNextTarget()) <= r)
							{
								store_buff->m_unitTarget = u_caster->GetAIInterface()->GetNextTarget()->GetGUID();
							}
							if(u_caster->GetAIInterface()->getAITargetsCount() && u_caster->GetMapMgr())
							{
								//try to get most hated creature
								u_caster->GetAIInterface()->LockAITargets(true);
								TargetMap *m_aiTargets = u_caster->GetAIInterface()->GetAITargets();
								TargetMap::iterator itr;
								for(itr = m_aiTargets->begin(); itr != m_aiTargets->end();itr++)
								{
									Unit *hate_t = u_caster->GetMapMgr()->GetUnit( itr->first );
									if( /*m_caster->GetMapMgr()->GetUnit(itr->first->GetGUID()) &&*/
										hate_t &&
										hate_t->GetMapMgr() == m_caster->GetMapMgr() &&
										hate_t->isAlive() &&
										m_caster->GetDistanceSq(hate_t) <= r &&
										isAttackable(u_caster,hate_t,!(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
										)
									{
										store_buff->m_unitTarget=itr->first;
										break;
									}
								}
								u_caster->GetAIInterface()->LockAITargets(false);
							}
						}
						//try to get a whatever target
						if(!store_buff->m_unitTarget)
						{
							store_buff->m_unitTarget=GetSinglePossibleEnemy(i);
						}
						//if we still couldn't get a target, check maybe we could use
//						if(!store_buff->m_unitTarget)
//						{
//						}
					}break;
					// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
				case 17: // A single target at a xyz location or the target is a possition xyz
				case 18:// Land under caster.Maybe not correct
					{
						store_buff->m_srcX=m_caster->GetPositionX();
						store_buff->m_srcY=m_caster->GetPositionY();
						store_buff->m_srcZ=m_caster->GetPositionZ();
						store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
					}break;
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
					{// All Party Members around the Caster in given range NOT RAID!
						Player* p = p_caster;
						if( p == NULL)
						{
							if( static_cast< Creature* >( u_caster )->IsTotem() )
								p = static_cast< Player* >( static_cast< Creature* >( u_caster )->GetTotemOwner() );
						}
						if( p != NULL )
						{
							if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
							{
								store_buff->m_unitTarget = m_caster->GetGUID();
								break;
							}
							SubGroup * subgroup = p->GetGroup() ?
								p->GetGroup()->GetSubGroup(p->GetSubGroup()) : 0;

							if(subgroup)
							{
								p->GetGroup()->Lock();
								for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); ++itr)
								{
									if(!(*itr)->m_loggedInPlayer || m_caster == (*itr)->m_loggedInPlayer)
										continue;
									if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
									{
										store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
										break;
									}
								}
								p->GetGroup()->Unlock();
							}
						}

						if ( u_caster != NULL && u_caster->IsCreature() )
						{
							//target friendly npcs
							for( std::set<Object*>::iterator itr = u_caster->GetInRangeSameFactsSetBegin(); itr != u_caster->GetInRangeSameFactsSetEnd(); itr++ )
							{
								if ( (*itr) != NULL && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->IsInWorld() && ((Unit*)*itr)->isAlive() && IsInrange(u_caster, (*itr), r) )
								{
									store_buff->m_unitTarget = (*itr)->GetGUID();
									break;
								}
							}
						}
					}break;
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6
				case 57:// Targeted Party Member
					{// Single Target Friend
						if( p_caster != NULL )
						{
							if(isFriendly(p_caster,p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection())))
								store_buff->m_unitTarget = p_caster->GetSelection();
							else store_buff->m_unitTarget = p_caster->GetGUID();
						}
						else if( u_caster != NULL )
						{
							if(u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY))
								store_buff->m_unitTarget = u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY);
							 else
							 {
								//target friendly npcs
								for( std::set<Object*>::iterator itr = u_caster->GetInRangeSameFactsSetBegin(); itr != u_caster->GetInRangeSameFactsSetEnd(); itr++ )
								{
									if ( (*itr) != NULL && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->IsInWorld() && ((Unit*)*itr)->isAlive() && IsInrange(u_caster, (*itr), r) )
									{

										//few additional checks
										if (IsHealingSpell(GetProto()) && ((Unit*)*itr)->GetHealthPct() == 100 && !((Unit*)*itr)->HasAura(GetProto()->Id) /*!((Unit*)*itr)->HasActiveAura(GetProto()->Id, m_caster->GetGUID())*/)
											continue;

										//check if an aura is being applied, and check if it already exists
										bool applies_aura=false;
										for (int i=0; i<3; i++)
										{
											if (GetProto()->Effect[i] == SPELL_EFFECT_APPLY_AURA || GetProto()->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
											{
												applies_aura=true;
												break;
											}
										}

										//majority of healing spells stack, infact I think they all do as of 2.0.1
										if (!IsHealingSpell(GetProto()) && applies_aura && ((Unit*)*itr)->HasAura(GetProto()->Id))
											continue;


										store_buff->m_unitTarget = (*itr)->GetGUID();
										break;
									 }
								}
							}
						}
					}break;
				case EFF_TARGET_GAMEOBJECT:
					{
						if(p_caster && p_caster->GetSelection())
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_DUEL:
					{// Single Target Friend Used in Duel
						if(p_caster && p_caster->DuelingWith && p_caster->DuelingWith->isAlive() && IsInrange(p_caster,p_caster->DuelingWith,r))
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_GAMEOBJECT_ITEM:{// Gameobject/Item Target
						//shit
					}break;
				case 27:{ // target is owner of pet
					// please correct this if not correct does the caster variablen need a Pet caster variable?
						if(u_caster && u_caster->IsPet())
							store_buff->m_unitTarget = static_cast< Pet* >( u_caster )->GetPetOwner()->GetGUID();
					}break;
				case EFF_TARGET_MINION:
				case 73:
					{// Minion Target
						if(m_caster->GetUInt64Value(UNIT_FIELD_SUMMON) == 0)
							store_buff->m_unitTarget = m_caster->GetGUID();
						else store_buff->m_unitTarget = m_caster->GetUInt64Value(UNIT_FIELD_SUMMON);
					}break;
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
					{
						Player *p=NULL;
						if( p_caster != NULL )
								p = p_caster;
						else if( u_caster && u_caster->GetTypeId() == TYPEID_UNIT && static_cast< Creature* >( u_caster )->IsTotem() )
								p = static_cast< Player* >( static_cast< Creature* >( u_caster )->GetTotemOwner() );
						if( p_caster != NULL )
						{
							if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
							{
								store_buff->m_unitTarget = p->GetGUID();
								break;
							}
							SubGroup * pGroup = p_caster->GetGroup() ?
								p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : 0;

							if( pGroup )
							{
								p_caster->GetGroup()->Lock();
								for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
									itr != pGroup->GetGroupMembersEnd(); ++itr)
								{
									if(!(*itr)->m_loggedInPlayer || p == (*itr)->m_loggedInPlayer)
										continue;
									if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
									{
										store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
										break;
									}
								}
								p_caster->GetGroup()->Unlock();
							}
						}

						if ( u_caster != NULL && u_caster->IsCreature() )
						{
							//target friendly npcs
							for( std::set<Object*>::iterator itr = u_caster->GetInRangeSameFactsSetBegin(); itr != u_caster->GetInRangeSameFactsSetEnd(); itr++ )
							{
								if ( (*itr) != NULL && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->IsInWorld() && ((Unit*)*itr)->isAlive() && IsInrange(u_caster, (*itr), r) )								{
									store_buff->m_unitTarget = (*itr)->GetGUID();
									break;
								}
							}
						}
					}break;
				case 38:{//Dummy Target
					//have no idea
					}break;
				case EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
					{
						store_buff->m_unitTarget = m_caster->GetGUID();
					}break;
				case 40://Activate Object target(probably based on focus)
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
					{
						if( p_caster != NULL )
						{
							uint32 slot = GetProto()->Effect[i] - SPELL_EFFECT_SUMMON_TOTEM_SLOT1;
							if(p_caster->m_TotemSlots[slot] != 0)
								store_buff->m_unitTarget = p_caster->m_TotemSlots[slot]->GetGUID();
						}
					}break;
				case 61:{ // targets with the same group/raid and the same class
					//shit again
				}break;
				case EFF_TARGET_ALL_FRIENDLY_IN_AREA:{
					if ( u_caster != NULL && u_caster->IsCreature() )
					{
						for( std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); itr++ )
						{
							if ( (*itr) != NULL && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->IsInWorld() && ((Unit*)*itr)->isAlive() && IsInrange(u_caster, (*itr), r) && isFriendly(u_caster, (*itr)))
							{

								//few additional checks
								if(((Unit*)*itr)->HasAura(m_spellInfo->Id))
									continue;

								//check if an aura is being applied, and check if it already exists
								bool applies_aura=false;
								for (int i=0; i<3; i++)
								{
									if (m_spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AURA || m_spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
									{
										applies_aura=true;
										break;
									}
								}

								if (applies_aura && ((Unit*)*itr)->HasAura(m_spellInfo->Id))
									continue;


								store_buff->m_unitTarget = (*itr)->GetGUID();
							}
						}
					}
				}break;

			}//end switch
		}//end for
	if(store_buff->m_unitTarget)
		store_buff->m_targetMask |= TARGET_FLAG_UNIT;
	if(store_buff->m_srcX)
		store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
	if(store_buff->m_destX)
		store_buff->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}//end function

uint8 Spell::prepare( SpellCastTargets * targets )
{
	uint8 ccr;

	// In case spell got cast from a script check fear/wander states
	if (!p_caster && u_caster && u_caster->GetAIInterface())
	{
		AIInterface *ai = u_caster->GetAIInterface();
		if (ai->getAIState() == STATE_FEAR || ai->getAIState() == STATE_WANDER)
			return SPELL_FAILED_NOT_READY;
	}

	chaindamage = 0;
	m_targets = *targets;

	if( !m_triggeredSpell && p_caster != NULL && p_caster->CastTimeCheat )
		m_castTime = 0;
	else
	{
		m_castTime = GetCastTime( dbcSpellCastTime.LookupEntry( GetProto()->CastingTimeIndex ) );

		if( m_castTime && GetProto()->SpellGroupType && u_caster != NULL )
		{
			SM_FIValue( u_caster->SM_FCastTime, (int32*)&m_castTime, GetProto()->SpellGroupType );
			SM_PIValue( u_caster->SM_PCastTime, (int32*)&m_castTime, GetProto()->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			int spell_flat_modifers=0;
			int spell_pct_modifers=0;
			SM_FIValue(u_caster->SM_FCastTime,&spell_flat_modifers,GetProto()->SpellGroupType);
			SM_FIValue(u_caster->SM_PCastTime,&spell_pct_modifers,GetProto()->SpellGroupType);
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
				printf("!!!!!spell casttime mod flat %d , spell casttime mod pct %d , spell casttime %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,m_castTime,GetProto()->SpellGroupType);
#endif
		}

		// handle MOD_CAST_TIME
		if( u_caster != NULL && m_castTime )
		{
			m_castTime = float2int32( m_castTime * u_caster->GetFloatValue( UNIT_MOD_CAST_SPEED ) );
		}
	}

	if( p_caster != NULL )
	{
		if( p_caster->cannibalize )
		{
			sEventMgr.RemoveEvents( p_caster, EVENT_CANNIBALIZE );
			p_caster->SetUInt32Value( UNIT_NPC_EMOTESTATE, 0 );
			p_caster->cannibalize = false;
		}
	}

	//let us make sure cast_time is within decent range
	//this is a hax but there is no spell that has more then 10 minutes cast time

	if( m_castTime < 0 )
		m_castTime = 0;
	else if( m_castTime > 60 * 10 * 1000)
		m_castTime = 60 * 10 * 1000; //we should limit cast time to 10 minutes right ?

	m_timer = m_castTime;

	m_magnetTarget = 0;

	//if( p_caster != NULL )
	//   m_castTime -= 100;	  // session update time


	m_spellState = SPELL_STATE_PREPARING;

	if( m_triggeredSpell )
		cancastresult = SPELL_CANCAST_OK;
	else
		cancastresult = CanCast(false);

	//sLog.outString( "CanCast result: %u. Refer to SpellFailure.h to work out why." , cancastresult );

	ccr = cancastresult;
	if( cancastresult != SPELL_CANCAST_OK )
	{
		SendCastResult( cancastresult );

		if( m_triggeredByAura )
		{
			SendChannelUpdate( 0 );
			if( u_caster != NULL )
				u_caster->RemoveAura( m_triggeredByAura );
		}
		else
		{
			// HACK, real problem is the way spells are handled
			// when a spell is channeling and a new spell is casted
			// that is a channeling spell, but not triggert by a aura
			// the channel bar/spell is bugged
			if( u_caster && u_caster->GetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT) > 0 && u_caster->GetCurrentSpell() )
			{
				u_caster->GetCurrentSpell()->cancel();
				SendChannelUpdate( 0 );
				cancel();
				return ccr;
			}
		}
		finish();
		return ccr;
	}
	else
	{
		if( p_caster != NULL && p_caster->IsStealth() && m_spellInfo && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH) && m_spellInfo->Id != 1 ) // <-- baaaad, baaad hackfix - for some reason some spells were triggering Spell ID #1 and stuffing up the spell system.
		{
			/* talents procing - don't remove stealth either */
			if (!(m_spellInfo->Attributes & ATTRIBUTES_PASSIVE) && 
				!( pSpellId && dbcSpell.LookupEntry(pSpellId)->Attributes & ATTRIBUTES_PASSIVE ) )
			{
				p_caster->RemoveAura(p_caster->m_stealth);
				p_caster->m_stealth = 0;
			}
		}

		SendSpellStart();

		// start cooldown handler
		if( p_caster != NULL && !p_caster->CastTimeCheat && !m_triggeredSpell )
		{
			AddStartCooldown();
		}

		if( i_caster == NULL )
		{
			if( p_caster != NULL && m_timer > 0 && !m_triggeredSpell )
				p_caster->delayAttackTimer( m_timer + 1000 );
				//p_caster->setAttackTimer(m_timer + 1000, false);
		}

		// aura state removal
		if( GetProto()->CasterAuraState )
			u_caster->RemoveFlag( UNIT_FIELD_AURASTATE, GetProto()->CasterAuraState );
	}

	//instant cast(or triggered) and not channeling
	if( u_caster != NULL && ( m_castTime > 0 || GetProto()->ChannelInterruptFlags ) && !m_triggeredSpell )
	{
		m_castPositionX = m_caster->GetPositionX();
		m_castPositionY = m_caster->GetPositionY();
		m_castPositionZ = m_caster->GetPositionZ();

		u_caster->castSpell( this );
	}
	else
		cast( false );

	return ccr;
}

void Spell::cancel()
{
	if ( GetProto() == NULL ) return; //low chance

	if (m_spellState == SPELL_STATE_FINISHED)
		return;

	SendInterrupted(0);
	SendCastResult(SPELL_FAILED_INTERRUPTED);

	if(m_spellState == SPELL_STATE_CASTING)
	{
		if( u_caster != NULL )
			u_caster->RemoveAura(GetProto()->Id);

		if(m_timer > 0 || m_Delayed)
		{
			if(p_caster && p_caster->IsInWorld())
			{
				Unit *pTarget = p_caster->GetMapMgr()->GetUnit(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());

				if(pTarget)
				{
					pTarget->RemoveAura(GetProto()->Id, m_caster->GetGUID());
				}
				if(m_AreaAura)//remove of blizz and shit like this
				{

					DynamicObject* dynObj=m_caster->GetMapMgr()->GetDynamicObject(m_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
					if(dynObj)
						dynObj->Remove();
				}

				if(p_caster->GetSummonedObject())
				{
					if(p_caster->GetSummonedObject()->IsInWorld())
						p_caster->GetSummonedObject()->RemoveFromWorld(true);
					// for now..
					ASSERT(p_caster->GetSummonedObject()->GetTypeId() == TYPEID_GAMEOBJECT);
					delete ((GameObject*)(p_caster->GetSummonedObject()));
					p_caster->SetSummonedObject(NULL);
				}
				if (m_timer > 0)
					p_caster->delayAttackTimer(-m_timer);
//				p_caster->setAttackTimer(1000, false);
			 }
		}
		SendChannelUpdate(0);
	}

	//m_spellState = SPELL_STATE_FINISHED;

	// prevent memory corruption. free it up later.
	// if this is true it means we are currently in the cast() function somewhere else down the stack
	// (recursive spells) and we don't wanna have this class delete'd when we return to it.
	// at the end of cast() it will get freed anyway.
	if( !m_isCasting )
		finish();
}

void Spell::AddCooldown()
{
	if( p_caster != NULL )
		p_caster->Cooldown_Add( GetProto(), i_caster );
}

void Spell::AddStartCooldown()
{
	if( p_caster != NULL )
		p_caster->Cooldown_AddStart( GetProto() );
}

void Spell::cast(bool check)
{
	if( duelSpell && (
		( p_caster != NULL && p_caster->GetDuelState() != DUEL_STATE_STARTED ) ||
		( u_caster != NULL && u_caster->IsPet() && static_cast< Pet* >( u_caster )->GetPetOwner() && static_cast< Pet* >( u_caster )->GetPetOwner()->GetDuelState() != DUEL_STATE_STARTED ) ) )
	{
		// Can't cast that!
		SendInterrupted( SPELL_FAILED_TARGET_FRIENDLY );
		finish();
		return;
	}

	sLog.outDebug("Spell::cast %u, Unit: %u", GetProto()->Id, m_caster->GetLowGUID());

	if(check)
		cancastresult = CanCast(true);
	else
		cancastresult = SPELL_CANCAST_OK;

	if(cancastresult == SPELL_CANCAST_OK)
	{
		if (p_caster && !m_triggeredSpell && p_caster->IsInWorld() && GET_TYPE_FROM_GUID(m_targets.m_unitTarget)==HIGHGUID_TYPE_UNIT)
		{
			sQuestMgr.OnPlayerCast(p_caster,GetProto()->Id,m_targets.m_unitTarget);
		}
		if( GetProto()->Attributes & ATTRIBUTE_ON_NEXT_ATTACK)
		{
			if(!m_triggeredSpell)
			{
				// on next attack - we don't take the mana till it actually attacks.
				if(!HasPower())
				{
					SendInterrupted(SPELL_FAILED_NO_POWER);
					finish();
					return;
				}
			}
			else
			{
				// this is the actual spell cast
				if(!TakePower())	// shouldn't happen
				{
					SendInterrupted(SPELL_FAILED_NO_POWER);
					finish();
					return;
				}
			}
		}
		else
		{
			if(!m_triggeredSpell)
			{
				if(!TakePower()) //not enough mana
				{
					//sLog.outDebug("Spell::Not Enough Mana");
					SendInterrupted(SPELL_FAILED_NO_POWER);
					finish();
					return;
				}
			}
		}

		for(uint32 i=0;i<3;i++)
        {
			if( GetProto()->Effect[i] && GetProto()->Effect[i] != SPELL_EFFECT_PERSISTENT_AREA_AURA)
				 FillTargetMap(i);
        }

		if(m_magnetTarget){ // Spell was redirected
			// Grounding Totem gets destroyed after redirecting 1 spell
			Unit *MagnetTarget = m_caster->GetMapMgr()->GetUnit(m_magnetTarget);
			m_magnetTarget = 0;
			if ( MagnetTarget && MagnetTarget->IsCreature()){
				Creature *MagnetCreature = static_cast< Creature* >( MagnetTarget );
				if(MagnetCreature->IsTotem()){
					sEventMgr.ModifyEventTimeLeft(MagnetCreature, EVENT_TOTEM_EXPIRE, 0);
				}
			}
		}

		SendCastResult(cancastresult);
		if(cancastresult != SPELL_CANCAST_OK)
		{
			finish();
			return;
		}

		m_isCasting = true;

		//sLog.outString( "CanCastResult: %u" , cancastresult );
		if(!m_triggeredSpell)
			AddCooldown();

		if( p_caster )
		{
			if( GetProto()->NameHash == SPELL_HASH_SLAM)
			{
				/* slam - reset attack timer */
				p_caster->setAttackTimer( 0, true );
				p_caster->setAttackTimer( 0, false );
			}
			else if( m_spellInfo->NameHash == SPELL_HASH_VICTORY_RUSH )
			{
				p_caster->RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_LASTKILLWITHHONOR);
			}

			if( p_caster->IsStealth() && !(GetProto()->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)
				 && GetProto()->Id != 1 ) //check spells that get trigger spell 1 after spell loading
			{
				/* talents procing - don't remove stealth either */
				if (!(GetProto()->Attributes & ATTRIBUTES_PASSIVE) &&
					!( pSpellId && dbcSpell.LookupEntry(pSpellId)->Attributes & ATTRIBUTES_PASSIVE ) )
				{
					p_caster->RemoveAura(p_caster->m_stealth);
					p_caster->m_stealth = 0;
				}
			}

			// special case battleground additional actions
			if(p_caster->m_bg)
			{
				// warsong gulch & eye of the storm flag pickup check
				// also includes check for trying to cast stealth/etc while you have the flag
				switch(GetProto()->Id)
				{
					case 21651:
						// Arathi Basin opening spell, remove stealth, invisibility, etc. 
						p_caster->RemoveStealth();
						p_caster->RemoveInvisibility();
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_PROTECTION);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
						break;
					case 23333:
					case 23335:
					case 34976:
						// if we're picking up the flag remove the buffs
						p_caster->RemoveStealth();
						p_caster->RemoveInvisibility();
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_PROTECTION);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
						break;
					// cases for stealth - etc
					// we can cast the spell, but we drop the flag (if we have it)
					case 1784:	// stealth
					case 1785:
					case 1786:
					case 1787:
					case 5215:	// prowl
					case 6783:
					case 9913:
					case 498:	// divine protection
					case 5573:
					case 642:	// divine shield
					case 1020:
					case 1022:	// blessing of protection
					case 5599:
					case 10278:
					case 1856:	// vanish
					case 1857:
					case 26889:
					case 45438:	// ice block
					case 20580:	// shadowmeld
					case 66:	// invisibility
						if(p_caster->m_bg->GetType() == BATTLEGROUND_WARSUNG_GULCH)
						{
							if(p_caster->GetTeam() == 0)
								p_caster->RemoveAura(23333);	// ally player drop horde flag if they have it
							else
								p_caster->RemoveAura(23335); 	// horde player drop ally flag if they have it
						}
						if(p_caster->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)

							p_caster->RemoveAura(34976);	// drop the flag
						break;	
				}
			}
		}

		/*SpellExtraInfo* sp = objmgr.GetSpellExtraData(GetProto()->Id);
		if(sp)
		{
			Unit *Target = objmgr.GetUnit(m_targets.m_unitTarget);
			if(Target)
				Target->RemoveBySpecialType(sp->specialtype, p_caster->GetGUID());
		}*/

		if(!(GetProto()->Attributes & ATTRIBUTE_ON_NEXT_ATTACK  && !m_triggeredSpell))//on next attack
		{
			SendSpellGo();

			//******************** SHOOT SPELLS ***********************
			//* Flags are now 1,4,19,22 (4718610) //0x480012

			if (GetProto()->AttributesExC & 0x8000 && m_caster->IsPlayer() && m_caster->IsInWorld())
			{
                /// Part of this function contains a hack fix
                /// hack fix for shoot spells, should be some other resource for it
                //p_caster->SendSpellCoolDown(GetProto()->Id, GetProto()->RecoveryTime ? GetProto()->RecoveryTime : 2300);
				WorldPacket data(SMSG_SPELL_COOLDOWN, 14);
				data << GetProto()->Id;
				data << p_caster->GetNewGUID();
				data << uint32(GetProto()->RecoveryTime ? GetProto()->RecoveryTime : 2300);
				p_caster->GetSession()->SendPacket(&data);
			}
			else
			{
				if( GetProto()->ChannelInterruptFlags != 0 && !m_triggeredSpell)
				{
					/*
					Channeled spells are handled a little differently. The five second rule starts when the spell's channeling starts; i.e. when you pay the mana for it.
					The rule continues for at least five seconds, and longer if the spell is channeled for more than five seconds. For example,
					Mind Flay channels for 3 seconds and interrupts your regeneration for 5 seconds, while Tranquility channels for 10 seconds
					and interrupts your regeneration for the full 10 seconds.
					*/

					uint32 channelDuration = GetDuration();
					m_spellState = SPELL_STATE_CASTING;
					SendChannelStart(channelDuration);
					if( p_caster != NULL )
					{
						//Use channel interrupt flags here
						if(m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION || m_targets.m_targetMask == TARGET_FLAG_SOURCE_LOCATION)
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());
						else if(p_caster->GetSelection() == m_caster->GetGUID())
						{
							if(p_caster->GetSummon())
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSummon()->GetGUID());
							else if(m_targets.m_unitTarget)
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_unitTarget);
							else
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());
						}
						else
						{
							if(p_caster->GetSelection())
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());
							else if(p_caster->GetSummon())
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSummon()->GetGUID());
							else if(m_targets.m_unitTarget)
								u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_unitTarget);
							else
							{
								m_isCasting = false;
								cancel();
								return;
							}
						}
					}
					if(u_caster && u_caster->GetPowerType()==POWER_TYPE_MANA)
					{
						if(channelDuration <= 5000)
							u_caster->DelayPowerRegeneration(5000);
						else
							u_caster->DelayPowerRegeneration(channelDuration);
					}
				}
			}

			std::vector<uint64>::iterator i, i2;

			// this is here to avoid double search in the unique list
			//bool canreflect = false, reflected = false;
			for(int j=0;j<3;j++)
			{
				switch(GetProto()->EffectImplicitTargetA[j])
				{
					case 6:
					case 22:
					case 24:
					case 25:
						SetCanReflect();
						break;
				}
				if(GetCanReflect())
					continue;
				else
					break;
			}

			if(!IsReflected() && GetCanReflect() && m_caster->IsInWorld())
			{
				for(i= UniqueTargets.begin();i != UniqueTargets.end();i++)
				{
					Unit *Target = m_caster->GetMapMgr()->GetUnit(*i);
					if(Target)
					{
					   SetReflected(Reflect(Target));
					}

					// if the spell is reflected
					if(IsReflected())
						break;
				}
			}
			else
				SetReflected(false);

			bool isDuelEffect = false;
			//uint32 spellid = GetProto()->Id;

			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.
			if(u_caster && !m_triggeredSpell && !m_triggeredByAura)
				u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);

            // if the spell is not reflected
			if(!IsReflected())
			{
				for(uint32 x=0;x<3;x++)
				{
                    // check if we actualy have a effect
					if( GetProto()->Effect[x])
					{
						isDuelEffect = isDuelEffect ||  GetProto()->Effect[x] == SPELL_EFFECT_DUEL;
						if( GetProto()->Effect[x] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
                        {
							HandleEffects(m_caster->GetGUID(),x);
                        }
						else if (m_targetUnits[x].size()>0)
						{
							for(i= m_targetUnits[x].begin();i != m_targetUnits[x].end();)
                            {
								i2 = i++;
								HandleEffects((*i2),x);
                            }
						}

						// Capt: The way this is done is NOT GOOD. Target code should be redone.
						else if( GetProto()->Effect[x] == SPELL_EFFECT_TELEPORT_UNITS ||
							     GetProto()->Effect[x] == SPELL_EFFECT_SUMMON ||
								 GetProto()->Effect[x] == SPELL_EFFECT_TRIGGER_SPELL)
                        {
							HandleEffects(m_caster->GetGUID(),x);
                        }
					}
				}

				/* don't call HandleAddAura unless we actually have auras... - Burlex*/
				if( GetProto()->EffectApplyAuraName[0] != 0 || GetProto()->EffectApplyAuraName[1] != 0 ||
				   GetProto()->EffectApplyAuraName[2] != 0)
				{
					hadEffect = true; // spell has had an effect (for item removal & possibly other things)

					for(i= UniqueTargets.begin();i != UniqueTargets.end();i++)
					{
						HandleAddAura((*i));
					}
				}
				// spells that proc on spell cast, some talents
				if(p_caster && p_caster->IsInWorld())
				{
					for(i= UniqueTargets.begin();i != UniqueTargets.end();i++)
					{
						Unit * Target = p_caster->GetMapMgr()->GetUnit((*i));

						if(!Target)
							continue; //we already made this check, so why make it again ?

						if(!m_triggeredSpell || GetProto()->NameHash == SPELL_HASH_DEEP_WOUND )//Deep Wounds may trigger Blood Frenzy
						{
							p_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL,Target, GetProto());
							Target->HandleProc(PROC_ON_SPELL_LAND_VICTIM,u_caster,GetProto());
							p_caster->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)
						}

						Target->RemoveFlag(UNIT_FIELD_AURASTATE,GetProto()->TargetAuraState);
					}
				}
			}

			m_isCasting = false;

			if(m_spellState != SPELL_STATE_CASTING)
				finish();
		}
		else //this shit has nothing to do with instant, this only means it will be on NEXT melee hit
		{
			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.

			if(u_caster && !m_triggeredSpell && !m_triggeredByAura)
				u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);

			//not sure if it must be there...
			/*if( p_caster != NULL )
			{
				if(p_caster->m_onAutoShot)
				{
					p_caster->GetSession()->OutPacket(SMSG_CANCEL_AUTO_REPEAT);
					p_caster->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
					p_caster->m_onAutoShot = false;
				}
			}*/

			m_isCasting = false;
			SendCastResult(cancastresult);
			if( u_caster != NULL )
				u_caster->SetOnMeleeSpell(GetProto()->Id);

			finish();
		}

		if( u_caster != NULL )
			u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);
	}
	else
	{
		// cancast failed
		SendCastResult(cancastresult);
		finish();
	}
}

void Spell::AddTime(uint32 type)
{
	if(u_caster && u_caster->IsPlayer())
	{
		if( GetProto()->InterruptFlags & CAST_INTERRUPT_ON_DAMAGE_TAKEN)
		{
			cancel();
			return;
		}
		if( GetProto()->SpellGroupType && u_caster)
		{
			float ch=0;
			SM_FFValue(u_caster->SM_PNonInterrupt,&ch,GetProto()->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			float spell_pct_modifers=0;
			SM_FFValue(u_caster->SM_PNonInterrupt,&spell_pct_modifers,GetProto()->SpellGroupType);
			if(spell_pct_modifers!=0)
				printf("!!!!!spell interrupt chance mod pct %f , uninterrupt chance %f, spell group %u\n",spell_pct_modifers,ch,GetProto()->SpellGroupType);
#endif
			if(Rand(ch))
				return;
		}
		if( p_caster != NULL )
		{
			if(Rand(p_caster->SpellDelayResist[type]))
				return;
		}
		if(m_spellState==SPELL_STATE_PREPARING)
		{
			// no pushback for some spells
			if ((GetProto()->InterruptFlags & CAST_INTERRUPT_PUSHBACK) == 0)
				return;

			int32 delay = m_castTime/4;
			m_timer+=delay;
			if(m_timer>m_castTime)
			{
				delay -= (m_timer - m_castTime);
				m_timer=m_castTime;
				if(delay<0)
					delay = 1;
			}

			WorldPacket data(SMSG_SPELL_DELAYED, 13);
			data << u_caster->GetNewGUID();
			data << uint32(delay);
			u_caster->SendMessageToSet(&data, true);

			if(!p_caster)
			{
				if(m_caster->GetTypeId() == TYPEID_UNIT)
					u_caster->GetAIInterface()->AddStopTime(delay);
			}
			//in case cast is delayed, make sure we do not exit combat
			else
			{
//				sEventMgr.ModifyEventTimeLeft(p_caster,EVENT_ATTACK_TIMEOUT,PLAYER_ATTACK_TIMEOUT_INTERVAL,true);
				// also add a new delay to offhand and main hand attacks to avoid cutting the cast short
				p_caster->delayAttackTimer(delay);
			}
		}
		else if( GetProto()->ChannelInterruptFlags != 48140)
		{
			int32 delay = GetDuration()/3;
			m_timer-=delay;
			if(m_timer<0)
				m_timer=0;
			else
				p_caster->delayAttackTimer(-delay);

			m_Delayed = true;
			if(m_timer>0)
				SendChannelUpdate(m_timer);

		}
	}
}

void Spell::update(uint32 difftime)
{
	// skip cast if we're more than 2/3 of the way through
	// TODO: determine which spells can be casted while moving.
	// Client knows this, so it should be easy once we find the flag.
	// XD, it's already there!
	if( ( GetProto()->InterruptFlags & CAST_INTERRUPT_ON_MOVEMENT ) &&
		(((float)m_castTime / 1.5f) > (float)m_timer ) &&
//		float(m_castTime)/float(m_timer) >= 2.0f		&&
		(
		m_castPositionX != m_caster->GetPositionX() ||
		m_castPositionY != m_caster->GetPositionY() ||
		m_castPositionZ != m_caster->GetPositionZ()
		)
		)
	{
		if( u_caster != NULL )
		{
			if(u_caster->HasNoInterrupt() == 0 && GetProto()->EffectMechanic[1] != 14)
			{
				cancel();
				return;
			}
		}
	}

	if(m_cancelled)
	{
		cancel();
		return;
	}

	switch(m_spellState)
	{
	case SPELL_STATE_PREPARING:
		{
			//printf("spell::update m_timer %u, difftime %d, newtime %d\n", m_timer, difftime, m_timer-difftime);
			if((int32)difftime >= m_timer)
				cast(true);
			else
			{
				m_timer -= difftime;
				if((int32)difftime >= m_timer)
				{
					m_timer = 0;
					cast(true);
				}
			}


		} break;
	case SPELL_STATE_CASTING:
		{
			if(m_timer > 0)
			{
				if((int32)difftime >= m_timer)
					m_timer = 0;
				else
					m_timer -= difftime;
			}
			if(m_timer <= 0)
			{
				SendChannelUpdate(0);
				finish();
			}
		} break;
	}
}

void Spell::finish()
{
	if (m_spellState == SPELL_STATE_FINISHED)
		return;

	m_spellState = SPELL_STATE_FINISHED;
	if( u_caster != NULL )
	{
		u_caster->m_canMove = true;
		// mana           channeled                                                     power type is mana
		if(m_usesMana && (GetProto()->ChannelInterruptFlags == 0 && !m_triggeredSpell) && u_caster->GetPowerType()==POWER_TYPE_MANA)
		{
			/*
			Five Second Rule
			After a character expends mana in casting a spell, the effective amount of mana gained per tick from spirit-based regeneration becomes a ratio of the normal
			listed above, for a period of 5 seconds. During this period mana regeneration is said to be interrupted. This is commonly referred to as the five second rule.
			By default, your interrupted mana regeneration ratio is 0%, meaning that spirit-based mana regeneration is suspended for 5 seconds after casting.
			Several effects can increase this ratio, including:
			*/

			u_caster->DelayPowerRegeneration(5000);
		}
	}
	/* Mana Regenerates while in combat but not for 5 seconds after each spell */
	/* Only if the spell uses mana, will it cause a regen delay.
	   is this correct? is there any spell that doesn't use mana that does cause a delay?
	   this is for creatures as effects like chill (when they have frost armor on) prevents regening of mana	*/

	//moved to spellhandler.cpp -> remove item when click on it! not when it finishes

	//enable pvp when attacking another player with spells
	if( p_caster != NULL )
	{
		if (GetProto()->Attributes & ATTRIBUTES_STOP_ATTACK && p_caster->IsAttacking() )
		{
			p_caster->EventAttackStop();
			p_caster->smsg_AttackStop( p_caster->GetSelection() );
			p_caster->GetSession()->OutPacket( SMSG_CANCEL_COMBAT );
		}

		if(m_requiresCP && !GetSpellFailed())
		{
			if(p_caster->m_spellcomboPoints)
			{
				p_caster->m_comboPoints = p_caster->m_spellcomboPoints;
				p_caster->UpdateComboPoints(); //this will make sure we do not use any wrong values here
			}
			else
			{
				p_caster->NullComboPoints();
			}
		}
		if(m_Delayed)
		{
			Unit *pTarget = NULL;
			if( p_caster->IsInWorld() )
			{
				pTarget = p_caster->GetMapMgr()->GetUnit(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
			}

			if(pTarget)
			{
				pTarget->RemoveAura(GetProto()->Id, m_caster->GetGUID());
			}
		}
	}

	if( GetProto()->Effect[0] == SPELL_EFFECT_SUMMON_OBJECT ||
		GetProto()->Effect[1] == SPELL_EFFECT_SUMMON_OBJECT ||
		GetProto()->Effect[2] == SPELL_EFFECT_SUMMON_OBJECT)
		if( p_caster != NULL )
			p_caster->SetSummonedObject(NULL);
	/*
	Set cooldown on item
	*/
	if( i_caster && i_caster->GetOwner() && cancastresult == SPELL_CANCAST_OK && !GetSpellFailed() )
	{
			uint32 x;
		for(x = 0; x < 5; x++)
		{
			if(i_caster->GetProto()->Spells[x].Trigger == USE)
			{
				if(i_caster->GetProto()->Spells[x].Id)
					break;
			}
		}
		i_caster->GetOwner()->Cooldown_AddItem( i_caster->GetProto() , x );
	}


  // cebernic added it
  // moved this from ::prepare() 
  // With preparing got ClearCooldownForspell, it makes too early for player client.
	// Now .cheat cooldown works perfectly.
	if( !m_triggeredSpell && p_caster != NULL && p_caster->CooldownCheat )
		p_caster->ClearCooldownForSpell( GetProto()->Id );


	/*
	We set current spell only if this spell has cast time or is channeling spell
	otherwise it's instant spell and we delete it right after completion
	*/

	if( u_caster != NULL )
	{
		if(!m_triggeredSpell && (GetProto()->ChannelInterruptFlags || m_castTime>0))
			u_caster->SetCurrentSpell(NULL);
	}
	SpellPool.PooledDelete( this );
}

void Spell::SendCastResult(uint8 result)
{
	uint32 Extra = 0;
	if(result == SPELL_CANCAST_OK) return;

	SetSpellFailed();

	if(!m_caster->IsInWorld()) return;

	Player * plr = p_caster;

	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr) return;

	// for some reason, the result extra is not working for anything, including SPELL_FAILED_REQUIRES_SPELL_FOCUS
	switch( result )
	{
	case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
		Extra = GetProto()->RequiresSpellFocus;
		break;

	case SPELL_FAILED_REQUIRES_AREA:
		Extra = GetProto()->RequiresAreaId;
		break;

	case SPELL_FAILED_TOTEMS:
		Extra = GetProto()->Totem[1] ? GetProto()->Totem[1] : GetProto()->Totem[0];
		break;

	//case SPELL_FAILED_TOTEM_CATEGORY: seems to be fully client sided.
	}

	//plr->SendCastResult(GetProto()->Id, result, extra_cast_number, Extra);
	if( Extra )
	{
		packetSMSG_CASTRESULT_EXTRA pe;
		pe.SpellId = GetProto()->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		pe.Extra = Extra;
		plr->GetSession()->OutPacket( SMSG_CAST_RESULT, sizeof( packetSMSG_CASTRESULT_EXTRA ), &pe );
	}
	else
	{
		packetSMSG_CASTRESULT pe;
		pe.SpellId = GetProto()->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		plr->GetSession()->OutPacket( SMSG_CAST_RESULT, sizeof( packetSMSG_CASTRESULT ), &pe );
	}
}

// uint16 0xFFFF
enum SpellStartFlags
{
	//0x01
	SPELL_START_FLAG_DEFAULT = 0x02, // atm set as default flag
	//0x04
	//0x08
	//0x10
	SPELL_START_FLAG_RANGED = 0x20,
	//0x40
	//0x80
	//0x100
	//0x200
	//0x400
	//0x800
	//0x1000
	//0x2000
	//0x4000
	//0x8000
};

void Spell::SendSpellStart()
{
	// no need to send this on passive spells
	if( !m_caster->IsInWorld() || GetProto()->Attributes & 64 || m_triggeredSpell )
		return;

	WorldPacket data( 150 );

	uint16 cast_flags = 2;

	if( GetType() == SPELL_DMG_TYPE_RANGED )
		cast_flags |= 0x20;

    // hacky yeaaaa
	if( GetProto()->Id == 8326 ) // death
		cast_flags = 0x0F;

	data.SetOpcode( SMSG_SPELL_START );
	if( i_caster != NULL )
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	else
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();

	data << GetProto()->Id;
	data << extra_cast_number;
	data << cast_flags;
	data << (uint32)m_castTime;

	m_targets.write( data );

	if( GetType() == SPELL_DMG_TYPE_RANGED )
	{
		ItemPrototype* ip = NULL;
		if( GetProto()->Id == SPELL_RANGED_THROW ) // throw
		{
			if( p_caster != NULL )
			{
				Item *itm = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( itm != NULL )
				{
					ip = itm->GetProto();
					/* Throwing Weapon Patch by Supalosa
					p_caster->GetItemInterface()->RemoveItemAmt(it->GetEntry(),1);
					(Supalosa: Instead of removing one from the stack, remove one from durability)
					We don't need to check if the durability is 0, because you can't cast the Throw spell if the thrown weapon is broken, because it returns "Requires Throwing Weapon" or something.
					*/

					// burlex - added a check here anyway (wpe suckers :P)
					if( itm->GetDurability() > 0 )
					{
						itm->SetDurability( itm->GetDurability() - 1 );
						if( itm->GetDurability() == 0 )
							p_caster->ApplyItemMods( itm, EQUIPMENT_SLOT_RANGED, false, true );
					}
				}
				else
				{
					ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
				}
			}
		}
		else if( GetProto()->AttributesExC & FLAGS4_PLAYER_RANGED_SPELLS )
		{
			if( p_caster != NULL )
				ip = ItemPrototypeStorage.LookupEntry( p_caster->GetUInt32Value( PLAYER_AMMO_ID ) );
			else
				ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
		}

		if( ip != NULL )
			data << ip->DisplayInfoID << ip->InventoryType;
	}

	m_caster->SendMessageToSet( &data, true );
}

/************************************************************************/
/* General Spell Go Flags, for documentation reasons                    */
/************************************************************************/
enum SpellGoFlags
{
	//0x01
	//0x02
	//0x04
	//0x08
	//0x10
	SPELL_GO_FLAGS_RANGED           = 0x20,
	//0x40
	//0x80
	SPELL_GO_FLAGS_ITEM_CASTER      = 0x100,
	//0x200
	SPELL_GO_FLAGS_EXTRA_MESSAGE    = 0x400, //TARGET MISSES AND OTHER MESSAGES LIKE "Resist"
	//0x800
	//0x1000
	//0x2000
	//0x4000
	//0x8000
};

void Spell::SendSpellGo()
{
	// Fill UniqueTargets
	TargetsList::iterator i, j;
	for( uint32 x = 0; x < 3; x++ )
	{
		if( GetProto()->Effect[x] )
		{
			bool add = true;
			for( i = m_targetUnits[x].begin(); i != m_targetUnits[x].end(); i++ )
			{
				add = true;
				for( j = UniqueTargets.begin(); j != UniqueTargets.end(); j++ )
				{
					if( (*j) == (*i) )
					{
						add = false;
						break;
					}
				}
				if( add && (*i) != 0 )
					UniqueTargets.push_back( (*i) );
				//TargetsList::iterator itr = std::unique(m_targetUnits[x].begin(), m_targetUnits[x].end());
				//UniqueTargets.insert(UniqueTargets.begin(),));
				//UniqueTargets.insert(UniqueTargets.begin(), itr);
			}
		}
	}

	// no need to send this on passive spells
	if( !m_caster->IsInWorld() || GetProto()->Attributes & 64 )
		return;

	// Start Spell
	WorldPacket data( 200 );
	data.SetOpcode( SMSG_SPELL_GO );
	uint16 flags = 0;

	if ( GetType() == SPELL_DMG_TYPE_RANGED )
		flags |= 0x20; // 0x20 RANGED

	if( i_caster != NULL )
		flags |= SPELL_GO_FLAGS_ITEM_CASTER; // 0x100 ITEM CASTER

	if( ModeratedTargets.size() > 0 )
		flags |= 0x400; // 0x400 TARGET MISSES AND OTHER MESSAGES LIKE "Resist"

	// hacky..
	if( GetProto()->Id == 8326 ) // death
		flags = SPELL_GO_FLAGS_ITEM_CASTER | 0x0D;


	if( i_caster != NULL && u_caster != NULL ) // this is needed for correct cooldown on items
	{
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	}
	else
	{
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();
	}

	data << GetProto()->Id;
	data << flags;
	data << getMSTime();
	data << (uint8)(UniqueTargets.size()); //number of hits

	writeSpellGoTargets( &data );

	data << (uint8)(ModeratedTargets.size()); //number if misses

	if( flags & 0x400 )
		writeSpellMissedTargets( &data );
	//else
		//data << uint8( 0 );

	m_targets.write( data ); // this write is included the target flag

	// er why handle it being null inside if if you can't get into if if its null
	if( GetType() == SPELL_DMG_TYPE_RANGED )
	{
		ItemPrototype* ip = NULL;
		if( GetProto()->Id == SPELL_RANGED_THROW )
		{
			if( p_caster != NULL )
			{
				Item* it = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( it != NULL )
					ip = it->GetProto();
			}
			else
			{
				ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
			}
		}
		else
		{
			if( p_caster != NULL )
				ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value( PLAYER_AMMO_ID ) );
			else // HACK FIX
				ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
		}
		if( ip != NULL)
			data << ip->DisplayInfoID << ip->InventoryType;
	}

	m_caster->SendMessageToSet( &data, true );

	// spell log execute is still send 2.08
	// as I see with this combination, need to test it more
	//if (flags != 0x120 && GetProto()->Attributes & 16) // not ranged and flag 5
	//SendLogExecute(0,m_targets.m_unitTarget);
}

void Spell::writeSpellGoTargets( WorldPacket * data )
{
	TargetsList::iterator i;
	for ( i = UniqueTargets.begin(); i != UniqueTargets.end(); i++ )
	{
		SendCastSuccess(*i);
		*data << (*i);
	}
}

void Spell::writeSpellMissedTargets( WorldPacket * data )
{
	/*
	 * The flags at the end known to us so far are.
	 * 1 = Miss
	 * 2 = Resist
	 * 3 = Dodge // mellee only
	 * 4 = Deflect
	 * 5 = Block // mellee only
	 * 6 = Evade
	 * 7 = Immune
	 */
	SpellTargetsList::iterator i;
	if(u_caster && u_caster->isAlive())
	{
		for ( i = ModeratedTargets.begin(); i != ModeratedTargets.end(); i++ )
		{
			*data << (*i).TargetGuid;       // uint64
			*data << (*i).TargetModType;    // uint8
			///handle proc on resist spell
			Unit* target = u_caster->GetMapMgr()->GetUnit((*i).TargetGuid);
			if(target && target->isAlive())
			{
				u_caster->HandleProc(PROC_ON_RESIST_VICTIM,target,GetProto()/*,damage*/);		/** Damage is uninitialized at this point - burlex */
				target->CombatStatusHandler_ResetPvPTimeout(); // aaa
				u_caster->CombatStatusHandler_ResetPvPTimeout(); // bbb
			}
		}
	}
	else
		for ( i = ModeratedTargets.begin(); i != ModeratedTargets.end(); i++ )
		{
			*data << (*i).TargetGuid;       // uint64
			*data << (*i).TargetModType;    // uint8
		}
}

void Spell::SendLogExecute(uint32 damage, uint64 & targetGuid)
{
	WorldPacket data(SMSG_SPELLLOGEXECUTE, 37);
	data << m_caster->GetNewGUID();
	data << GetProto()->Id;
	data << uint32(1);
//	data << GetProto()->SpellVisual;
//	data << uint32(1);
	if (m_caster->GetGUID() != targetGuid)
		data << targetGuid;
	if (damage)
		data << damage;
	m_caster->SendMessageToSet(&data,true);
}

void Spell::SendInterrupted( uint8 result )
{
	SetSpellFailed();

	if( m_caster == NULL || !m_caster->IsInWorld() )
		return;

	WorldPacket data( SMSG_SPELL_FAILURE, 20 );

	// send the failure to pet owner if we're a pet
	Player *plr = p_caster;
	if( plr == NULL && m_caster->IsPet() )
 	{
		static_cast<Pet*>(m_caster)->SendCastFailed( m_spellInfo->Id, result );
 	}
	else
	{
		if( plr == NULL && u_caster != NULL && u_caster->m_redirectSpellPackets != NULL )
			plr = u_caster->m_redirectSpellPackets;
	
		if( plr != NULL && plr->IsPlayer() )
		{
			data << m_caster->GetNewGUID();
			data << m_spellInfo->Id;
			data << uint8( result );
			plr->GetSession()->SendPacket( &data );
		}
	}
 
	data.Initialize( SMSG_SPELL_FAILED_OTHER );
	data << m_caster->GetNewGUID();
	data << GetProto()->Id;
	m_caster->SendMessageToSet( &data, false );
}

void Spell::SendChannelUpdate(uint32 time)
{
	if(time == 0)
	{
		if(u_caster && u_caster->IsInWorld())
		{
			DynamicObject* dynObj=u_caster->GetMapMgr()->GetDynamicObject(u_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
			if(dynObj)
				dynObj->Remove();

			u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,0);
			u_caster->SetUInt32Value(UNIT_CHANNEL_SPELL,0);
		}
	}

	if (!p_caster)
		return;

	WorldPacket data(MSG_CHANNEL_UPDATE, 18);
	data << p_caster->GetNewGUID();
	data << time;

	p_caster->SendMessageToSet(&data, true);
}

void Spell::SendChannelStart(uint32 duration)
{
	if (m_caster->GetTypeId() != TYPEID_GAMEOBJECT)
	{
		// Send Channel Start
		WorldPacket data(MSG_CHANNEL_START, 22);
		data << m_caster->GetNewGUID();
		data << GetProto()->Id;
		data << duration;
		m_caster->SendMessageToSet(&data, true);
	}

	m_castTime = m_timer = duration;

	if( u_caster != NULL )
		u_caster->SetUInt32Value(UNIT_CHANNEL_SPELL,GetProto()->Id);

	/*
	Unit* target = objmgr.GetCreature( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		target = objmgr.GetObject<Player>( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		return;

	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT,target->GetGUIDLow());
	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT+1,target->GetGUIDHigh());
	//disabled it can be not only creature but GO as well
	//and GO is not selectable, so this method will not work
	//these fields must be filled @ place of call
	*/
}

void Spell::SendResurrectRequest(Player* target)
{
	WorldPacket data(SMSG_RESURRECT_REQUEST, 13);
	data << m_caster->GetGUID();
	data << uint32(0) << uint8(0);

	target->GetSession()->SendPacket(&data);
	target->m_resurrecter = m_caster->GetGUID();
}

void Spell::SendTameFailure( uint8 result )
{
    if( p_caster != NULL )
    {
        WorldPacket data( SMSG_PET_TAME_FAILURE, 1 );
        data << uint8( result );
        p_caster->GetSession()->SendPacket( &data );
    }
}

bool Spell::HasPower()
{
	int32 powerField;
	if( u_caster != NULL )
		if(u_caster->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER))
			return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

	// Free cast for battle preparation
	if (p_caster && p_caster->HasAura(44521))
		return true;
	if (p_caster && p_caster->HasAura(44535))
		return true;
	if (p_caster && p_caster->HasAura(32727))
		return true;

	switch(GetProto()->powerType)
	{
	case POWER_TYPE_HEALTH:
		{
			powerField = UNIT_FIELD_HEALTH;
		} break;
	case POWER_TYPE_MANA:
		{
			powerField = UNIT_FIELD_POWER1;
			m_usesMana = true;
		} break;
	case POWER_TYPE_RAGE:
		{
			powerField = UNIT_FIELD_POWER2;
		} break;
	case POWER_TYPE_FOCUS:
		{
			powerField = UNIT_FIELD_POWER3;
		}	break;
	case POWER_TYPE_ENERGY:
		{
			powerField = UNIT_FIELD_POWER4;
		} break;
	default:
		{
			sLog.outDebug("unknown power type");
			// we should'nt be here to return
			return false;
		} break;
	}


	//FIXME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if( u_caster != NULL )
	{
		if( GetProto()->AttributesEx & 2 ) // Uses %100 mana
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}

	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if( GetProto()->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( GetProto()->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*GetProto()->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*GetProto()->ManaCostPercentage)/100;
	}
	else
	{
		cost = GetProto()->manaCost;
	}

	if((int32)GetProto()->powerType==POWER_TYPE_HEALTH)
		cost -= GetProto()->baseLevel;//FIX for life tap
	else if( u_caster != NULL )
	{
		if( GetProto()->powerType==POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[GetProto()->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost +=float2int32(cost*u_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+GetProto()->School));
	}

	 //hackfix for shiv's energy cost
	if (p_caster != NULL && m_spellInfo->NameHash == SPELL_HASH_SHIV && p_caster->GetItemInterface())
	{
		Item *it = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if( it != NULL )
			cost += (uint32)(10* (it->GetProto()->Delay / 1000.0f));
	}

	//apply modifiers
	if( GetProto()->SpellGroupType && u_caster)
	{
		SM_FIValue(u_caster->SM_FCost,&cost,GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PCost,&cost,GetProto()->SpellGroupType);
	}

	if (cost <=0)
		return true;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			return true;
		}
		else
			return false;
	}
}

bool Spell::TakePower()
{
	int32 powerField;
	if( u_caster != NULL )
	if(u_caster->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER))
		return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

		// Free cast for battle preparation
	if (p_caster && p_caster->HasAura(44521))
		return true;
	if (p_caster && p_caster->HasAura(44535))
		return true;
	if (p_caster && p_caster->HasAura(32727))
		return true;

	switch(GetProto()->powerType)
	{
		case POWER_TYPE_HEALTH:{
			powerField = UNIT_FIELD_HEALTH;
							   }break;
		case POWER_TYPE_MANA:{
			powerField = UNIT_FIELD_POWER1;
			m_usesMana=true;
							 }break;
		case POWER_TYPE_RAGE:{
			powerField = UNIT_FIELD_POWER2;
							 }break;
		case POWER_TYPE_FOCUS:{
			powerField = UNIT_FIELD_POWER3;
							  }break;
		case POWER_TYPE_ENERGY:{
			powerField = UNIT_FIELD_POWER4;
							  }break;
		default:{
			sLog.outDebug("unknown power type");
			// we should'nt be here to return
			return false;
				}break;
	}

	//FIXME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if( u_caster != NULL )
	{
		if( GetProto()->AttributesEx & ATTRIBUTESEX_DRAIN_WHOLE_MANA ) // Uses %100 mana
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}

	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if( GetProto()->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( GetProto()->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*GetProto()->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*GetProto()->ManaCostPercentage)/100;
	}
	else
	{
		cost = GetProto()->manaCost;
	}

	if((int32)GetProto()->powerType==POWER_TYPE_HEALTH)
			cost -= GetProto()->baseLevel;//FIX for life tap
	else if( u_caster != NULL )
	{
		if( GetProto()->powerType==POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[GetProto()->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost +=float2int32(cost*u_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+GetProto()->School));
	}

	 //hackfix for shiv's energy cost
	if (p_caster != NULL && m_spellInfo->NameHash == SPELL_HASH_SHIV && p_caster->GetItemInterface())
	{
		Item *it = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if( it != NULL )
			cost += (uint32)(10* (it->GetProto()->Delay / 1000.0f));
	}

	//apply modifiers
	if( GetProto()->SpellGroupType && u_caster)
	{
		  SM_FIValue(u_caster->SM_FCost,&cost,GetProto()->SpellGroupType);
		  SM_PIValue(u_caster->SM_PCost,&cost,GetProto()->SpellGroupType);
	}

#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
	int spell_flat_modifers=0;
	int spell_pct_modifers=0;
	SM_FIValue(u_caster->SM_FCost,&spell_flat_modifers,GetProto()->SpellGroupType);
	SM_FIValue(u_caster->SM_PCost,&spell_pct_modifers,GetProto()->SpellGroupType);
	if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
		printf("!!!!!spell cost mod flat %d , spell cost mod pct %d , spell dmg %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,cost,GetProto()->SpellGroupType);
#endif

	if (cost <=0)
		return true;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		m_caster->DealDamage(u_caster, cost, 0, 0, 0,true);
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			m_caster->SetUInt32Value(powerField, currentPower - cost);
			return true;
		}
		else
			return false;
	}
}

void Spell::HandleEffects(uint64 guid, uint32 i)
{
	uint32 id;

	if(guid == m_caster->GetGUID() || guid == 0)
	{
		unitTarget = u_caster;
		gameObjTarget = g_caster;
		playerTarget = p_caster;
		itemTarget = i_caster;
	}
	else
	{
		if(!m_caster->IsInWorld())
		{
			unitTarget = 0;
			playerTarget = 0;
			itemTarget = 0;
			gameObjTarget = 0;
			corpseTarget = 0;
		}
		else if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
		{
			if( p_caster != NULL )
			{
				Player * plr = p_caster->GetTradeTarget();
				if(plr)
					itemTarget = plr->getTradeItem((uint32)guid);
			}
		}
		else
		{
			unitTarget = NULL;
			switch(GET_TYPE_FROM_GUID(guid))
			{
			case HIGHGUID_TYPE_UNIT:
				unitTarget = m_caster->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PET:
				unitTarget = m_caster->GetMapMgr()->GetPet(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PLAYER:
				{
					unitTarget =  m_caster->GetMapMgr()->GetPlayer((uint32)guid);
					playerTarget = static_cast< Player* >(unitTarget);
				}break;
			case HIGHGUID_TYPE_ITEM:
				if( p_caster != NULL )
					itemTarget = p_caster->GetItemInterface()->GetItemByGUID(guid);

				break;
			case HIGHGUID_TYPE_GAMEOBJECT:
				gameObjTarget = m_caster->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_CORPSE:
				corpseTarget = objmgr.GetCorpse((uint32)guid);
				break;
			}
		}
	}

	damage = CalculateEffect(i,unitTarget);

#ifdef GM_Z_DEBUG_DIRECTLY	
	if ( playerTarget && playerTarget->IsPlayer() && playerTarget->IsInWorld() ){
    if ( playerTarget->GetSession() && playerTarget->GetSession()->CanUseCommand('z') )
  		sChatHandler.BlueSystemMessage( playerTarget->GetSession(), "[%sSystem%s] |rSpellEffect::Handler: %s Target = %u, Effect id = %u, id = %u, Self: %u.", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE, 
	  	playerTarget->GetLowGUID(),m_spellInfo->Effect[i],i, guid );
	}
#endif
	

	id = GetProto()->Effect[i];
	if( id<TOTAL_SPELL_EFFECTS)
	{
		sLog.outDebug( "WORLD: Spell effect id = %u (%s), damage = %d", id, SpellEffectNames[id], damage);

		/*if(unitTarget && p_caster && isAttackable(p_caster,unitTarget))
			sEventMgr.ModifyEventTimeLeft(p_caster,EVENT_ATTACK_TIMEOUT,PLAYER_ATTACK_TIMEOUT_INTERVAL);*/

		(*this.*SpellEffectsHandler[id])(i);
	}
	else
		sLog.outError("SPELL: unknown effect %u spellid %u", id, GetProto()->Id);
}

void Spell::HandleAddAura(uint64 guid)
{
	Unit * Target = 0;
	if(guid == 0)
		return;

	if(u_caster && u_caster->GetGUID() == guid)
		Target = u_caster;
	else if(m_caster->IsInWorld())
		Target = m_caster->GetMapMgr()->GetUnit(guid);

	if(!Target)
		return;

	// Applying an aura to a flagged target will cause you to get flagged.
    // self casting doesnt flag himself.
	if(Target->IsPlayer() && p_caster && p_caster != static_cast< Player* >(Target))
	{
		if(static_cast< Player* >(Target)->IsPvPFlagged())
			p_caster->SetPvPFlag();
	}

	// remove any auras with same type
	if( GetProto()->BGR_one_buff_on_target > 0)
		Target->RemoveAurasByBuffType(GetProto()->BGR_one_buff_on_target, m_caster->GetGUID(),GetProto()->Id);

	uint32 spellid = 0;

	if( GetProto()->MechanicsType == MECHANIC_INVULNARABLE && GetProto()->Id != 25771) // Cast spell Forbearance
		spellid = 25771;
	else if( GetProto()->NameHash == SPELL_HASH_AVENGING_WRATH )
		spellid = 25771;
	else if( GetProto()->MechanicsType == MECHANIC_HEALING && GetProto()->Id != 11196) // Cast spell Recently Bandaged
		spellid = 11196;
	else if( GetProto()->MechanicsType == MECHANIC_SHIELDED && GetProto()->Id != 6788) // Cast spell Weakened Soul
		spellid = 6788;
	else if( GetProto()->Id == 45438) // Cast spell Hypothermia
		spellid = 41425;
	else if( GetProto()->Id == 30451) // Cast spell Arcane Blast
		spellid = 36032;
	else if( GetProto()->Id == 20572 || GetProto()->Id == 33702 || GetProto()->Id == 33697) // Cast spell Blood Fury
		spellid = 23230;
	else if( GetProto()->Id == 9036) // cebernic: Cast Wisp Spirit
		spellid = 20584;


	if(spellid && p_caster)
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( spellid );
		if(!spellInfo) return;
		Spell *spell = SpellPool.PooledNew();
		spell->Init(p_caster, spellInfo ,true, NULL);
		SpellCastTargets targets(Target->GetGUID());
		spell->prepare(&targets);
	}

	// avoid map corruption
	if(Target->GetInstanceID()!=m_caster->GetInstanceID())
		return;

	std::map<uint32,Aura*>::iterator itr=Target->tmpAura.find(GetProto()->Id);
	if(itr!=Target->tmpAura.end())
	{
		if(itr->second)
		{
			if(itr->second->GetSpellProto()->procCharges>0)
			{
				Aura *aur=NULL;
				int charges = itr->second->GetSpellProto()->procCharges;
				if( itr->second->GetSpellProto()->SpellGroupType && u_caster != NULL )
				{
					SM_FIValue( u_caster->SM_FCharges, &charges, itr->second->GetSpellProto()->SpellGroupType );
					SM_PIValue( u_caster->SM_PCharges, &charges, itr->second->GetSpellProto()->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
					float spell_flat_modifers=0;
					float spell_pct_modifers=0;
					SM_FIValue(u_caster->SM_FCharges,&spell_flat_modifers,itr->second->GetSpellProto()->SpellGroupType);
					SM_FIValue(u_caster->SM_PCharges,&spell_pct_modifers,itr->second->GetSpellProto()->SpellGroupType);
					if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
						printf("!!!!!spell charge bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxRange,GetProto()->SpellGroupType);
#endif
				}
				for(int i=0;i<charges-1;i++)
				{
					aur = AuraPool.PooledNew();
					aur->Init(itr->second->GetSpellProto(),itr->second->GetDuration(),itr->second->GetCaster(),itr->second->GetTarget(), i_caster);
					Target->AddAura(aur);
					aur=NULL;
				}
				if(!(itr->second->GetSpellProto()->procFlags & PROC_REMOVEONUSE))
				{
					SpellCharge charge;
					charge.count=charges;
					charge.spellId=itr->second->GetSpellId();
					charge.ProcFlag=itr->second->GetSpellProto()->procFlags;
					charge.lastproc = 0;
					Target->m_chargeSpells.insert(make_pair(itr->second->GetSpellId(),charge));
				}
			}
			Target->AddAura(itr->second); // the real spell is added last so the modifier is removed last
			Target->tmpAura.erase(itr);
		}
	}
}

/*
void Spell::TriggerSpell()
{
	if(TriggerSpellId != 0)
	{
		// check for spell id
		SpellEntry *spellInfo = sSpellStore.LookupEntry(TriggerSpellId );

		if(!spellInfo)
		{
			sLog.outError("WORLD: unknown spell id %i\n", TriggerSpellId);
			return;
		}

		Spell *spell = new Spell(m_caster, spellInfo,false, NULL);
		WPAssert(spell);

		SpellCastTargets targets;
		if(TriggerSpellTarget)
			targets.m_unitTarget = TriggerSpellTarget;
		else
			targets.m_unitTarget = m_targets.m_unitTarget;

		spell->prepare(&targets);
	}
}*/
/*
void Spell::DetermineSkillUp()
{
	skilllinespell* skill = objmgr.GetSpellSkill(GetProto()->Id);
	if (m_caster->GetTypeId() == TYPEID_PLAYER)
	{
		if ((skill) && static_cast< Player* >( m_caster )->HasSkillLine( skill->skilline ) )
		{
			uint32 amt = static_cast< Player* >( m_caster )->GetBaseSkillAmt(skill->skilline);
			uint32 max = static_cast< Player* >( m_caster )->GetSkillMax(skill->skilline);
			if (amt >= skill->grey) //grey
			{
			}
			else if ((amt >= (((skill->grey - skill->green) / 2) + skill->green)) && (amt < max))
			{
				if (rand()%100 < 33) //green
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);

			}
			else if ((amt >= skill->green) && (amt < max))
			{
				if (rand()%100 < 66) //yellow
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);

			}
			else
			{
				if (amt < max) //brown
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);
			}
		}
	}
}
*/

bool Spell::IsAspect()
{
	return (
		(GetProto()->Id ==  2596) || (GetProto()->Id ==  5118) || (GetProto()->Id == 14320) || (GetProto()->Id == 13159) || (GetProto()->Id == 13161) || (GetProto()->Id == 20190) ||
		(GetProto()->Id == 20043) || (GetProto()->Id == 14322) || (GetProto()->Id == 14321) || (GetProto()->Id == 13163) || (GetProto()->Id == 14319) || (GetProto()->Id == 14318) || (GetProto()->Id == 13165));
}

bool Spell::IsSeal()
{
	return (
		(GetProto()->Id == 13903) || (GetProto()->Id == 17177) || (GetProto()->Id == 20154) || (GetProto()->Id == 20162) || (GetProto()->Id == 20164) || (GetProto()->Id == 20165) ||
		(GetProto()->Id == 20166) || (GetProto()->Id == 20287) || (GetProto()->Id == 20288) || (GetProto()->Id == 20289) || (GetProto()->Id == 20290) || (GetProto()->Id == 20291) ||
		(GetProto()->Id == 20292) || (GetProto()->Id == 20293) || (GetProto()->Id == 20305) || (GetProto()->Id == 20306) || (GetProto()->Id == 20307) || (GetProto()->Id == 20308) ||
		(GetProto()->Id == 20347) || (GetProto()->Id == 20348) || (GetProto()->Id == 20349) || (GetProto()->Id == 20356) || (GetProto()->Id == 20357) || (GetProto()->Id == 20375) ||
		(GetProto()->Id == 20915) || (GetProto()->Id == 20918) || (GetProto()->Id == 20919) || (GetProto()->Id == 20920) || (GetProto()->Id == 21082) || (GetProto()->Id == 21084));
}

uint8 Spell::CanCast(bool tolerate)
{
  // cebernic,can get this.
  if ( !GetProto() ) return SPELL_FAILED_SPELL_UNAVAILABLE;
  // how this happended ? cebernic
  if ( GetProto()->School < NORMAL_DAMAGE || GetProto()->School > ARCANE_DAMAGE ) return SPELL_FAILED_SPELL_UNAVAILABLE;

	uint32 i;
	if(objmgr.IsSpellDisabled(GetProto()->Id))
		return SPELL_FAILED_SPELL_UNAVAILABLE;

	if(m_caster->IsInWorld())
	{
		Unit *target = m_caster->GetMapMgr()->GetUnit( m_targets.m_unitTarget );

		if( target )
		{
			//gm flags can't be targetted by ANY spell, not even their own self casts
			if (target->IsPlayer() && (Player*)target->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
				return SPELL_FAILED_BM_OR_INVISGOD;

			//you can't mind control someone already mind controlled
			if (GetProto()->NameHash == SPELL_HASH_MIND_CONTROL && target->HasAurasWithNameHash(SPELL_HASH_MIND_CONTROL))
				return SPELL_FAILED_BAD_TARGETS;


			/* Shady: wtf is that? commented.
			else  if(GetProto()->NameHash == SPELL_HASH_VINDICATION)
			{
				if( !target->IsPlayer() )
					return SPELL_FAILED_BAD_TARGETS;
			}*/
		}
		if(GetProto()->Id == 32146)
		{
			Creature *corpse = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 18240);
			if(corpse != NULL)
				if (m_caster->CalcDistance(m_caster, corpse) > 5)
					return SPELL_FAILED_NOT_HERE;
		}

		else if(GetProto()->Id == 39246)
		{
			Creature *cleft = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 22105);
			if(cleft == NULL || cleft->isAlive())
				return SPELL_FAILED_NOT_HERE;
		}

		else if(GetProto()->Id == 30988)
		{
			Creature *corpse = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 17701);
			if(corpse != NULL)
				if (m_caster->CalcDistance(m_caster, corpse) > 5  || corpse->isAlive())
					return SPELL_FAILED_NOT_HERE;
		}

		else if(GetProto()->Id == 43723)
		{
			Creature *abysal = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), 19973);
			if(abysal != NULL)
			{
				if(!abysal->isAlive())
					if(!(p_caster->GetItemInterface()->GetItemCount(31672, FALSE) > 1 && p_caster->GetItemInterface()->GetItemCount(31673, FALSE) > 0 && p_caster->CalcDistance(p_caster, abysal) < 10))
						return SPELL_FAILED_NOT_HERE;
			}
			else
				return SPELL_FAILED_NOT_HERE;
		}

		else if(GetProto()->Id == 32307)
		{
			Creature *kilsorrow = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ());
			if(kilsorrow == NULL || kilsorrow->isAlive() || p_caster->CalcDistance(p_caster, kilsorrow) > 1)
				return SPELL_FAILED_NOT_HERE;
			if(kilsorrow->GetEntry() != 17147 && kilsorrow->GetEntry() != 17148 && kilsorrow->GetEntry() != 18397 && kilsorrow->GetEntry() != 18658 && kilsorrow->GetEntry() != 17146)
				return SPELL_FAILED_NOT_HERE;
		}
	}

	if (u_caster != NULL)
	{
		if (GetProto()->Attributes & ATTRIBUTES_REQ_STEALTH && !u_caster->IsStealth())
			return SPELL_FAILED_ONLY_STEALTHED;

		if (GetProto()->Attributes & ATTRIBUTES_REQ_OOC && u_caster->CombatStatus.IsInCombat())
			return SPELL_FAILED_TARGET_IN_COMBAT;
	}
	//Shady: EquippedItemClass, EquippedItemSubClass (doesn't it handled with client?)


	if( p_caster != NULL )
	{
		if (sWorld.Collision) {
			if (GetProto()->MechanicsType == MECHANIC_MOUNTED)
			{
				if (CollideInterface.IsIndoor( p_caster->GetMapId(), p_caster->GetPositionNC() ))
					return SPELL_FAILED_NO_MOUNTS_ALLOWED;
			}
			else if( GetProto()->Attributes & ATTRIBUTES_ONLY_OUTDOORS )
			{
				if( !CollideInterface.IsOutdoor( p_caster->GetMapId(), p_caster->GetPositionNC() ) )
					return SPELL_FAILED_ONLY_OUTDOORS;
			}
		}

		//are we in an arena and the spell cooldown is longer then 15mins?
		if ( p_caster->m_bg && ( p_caster->m_bg->GetType() >= BATTLEGROUND_ARENA_2V2 && p_caster->m_bg->GetType() <= BATTLEGROUND_ARENA_5V5 ) &&
			( GetProto()->RecoveryTime > 900000 || GetProto()->CategoryRecoveryTime > 900000 ) )
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		//if ( !p_caster->HasSpell(GetProto()->Id) && i_caster == NULL && !tolerate )
		//	return SPELL_FAILED_SPELL_UNAVAILABLE;

		// backstab/ambush
		/*Shady: This shit shouldn't be here
		We have special attribute ATTRIBUTES_REQ_STEALTH for stealth requiring.
		Also we have EquippedItemClass, EquippedItemSubClass fields in DBC for items handling. 
		SO FUCKING USE IT!
		if( GetProto()->NameHash == SPELL_HASH_BACKSTAB || GetProto()->NameHash == SPELL_HASH_AMBUSH )
		{
			if( GetProto()->NameHash == SPELL_HASH_AMBUSH && !p_caster->IsStealth() )
				return SPELL_FAILED_ONLY_STEALTHED;

			Item * pMainHand = p_caster->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}*/

		// check for cooldowns
		if(!tolerate && !p_caster->Cooldown_CanCast(GetProto()))
				return SPELL_FAILED_NOT_READY;

		if(p_caster->GetDuelState() == DUEL_STATE_REQUESTED)
		{
			for(i = 0; i < 3; ++i)
			{
				if( GetProto()->Effect[i] && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_AURA && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_PET_AURA
					&& GetProto()->Effect[i] != SPELL_EFFECT_APPLY_AREA_AURA)
				{
					return SPELL_FAILED_TARGET_DUELING;
				}
			}
		}

		// check for duel areas
		if(GetProto()->Id == 7266)
		{
			AreaTable* at = dbcArea.LookupEntry( p_caster->GetAreaID() );
			if(at->AreaFlags & AREA_CITY_AREA)
				return SPELL_FAILED_NO_DUELING;
			// instance & stealth checks
			if ( p_caster->GetMapMgr() && p_caster->GetMapMgr()->GetMapInfo() && p_caster->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL )
					return SPELL_FAILED_NO_DUELING;
			if ( p_caster->IsStealth() )
				return SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED;
		}

		// check if spell is allowed while player is on a taxi
		//Shady: handled via ATTRIBUTES_MOUNT_CASTABLE, nah?
		//if(p_caster->m_onTaxi)
		//{
		//	if( GetProto()->Id != 33836 || GetProto()->Id != 33655) // exception for Area 52 Special
		//		return SPELL_FAILED_NOT_ON_TAXI;
		//}

		// check if spell is allowed while player is on a transporter
		if(p_caster->m_CurrentTransporter)
		{
			// no mounts while on transporters
			if( GetProto()->EffectApplyAuraName[0] == SPELL_AURA_MOUNTED || GetProto()->EffectApplyAuraName[1] == SPELL_AURA_MOUNTED || GetProto()->EffectApplyAuraName[2] == SPELL_AURA_MOUNTED)
				return SPELL_FAILED_NOT_ON_TRANSPORT;
		}

		// check if spell is allowed while not mounted
		if(!p_caster->IsMounted())
		{
			if( GetProto()->Id == 25860) // Reindeer Transformation
				return SPELL_FAILED_ONLY_MOUNTED;
		}
		else
		{
			if (!(GetProto()->Attributes & ATTRIBUTES_MOUNT_CASTABLE))
				return SPELL_FAILED_NOT_MOUNTED;
		}

		// check if spell is allowed while shapeshifted
		if(p_caster->GetShapeShift())
		{
			switch(p_caster->GetShapeShift())
			{
				case FORM_TREE:
				case FORM_BATTLESTANCE:
				case FORM_DEFENSIVESTANCE:
				case FORM_BERSERKERSTANCE:
				case FORM_SHADOW:
				case FORM_STEALTH:
				case FORM_MOONKIN:
				{
					break;
				}

				case FORM_SWIFT:
				case FORM_FLIGHT:
				{
					// check if item is allowed (only special items allowed in flight forms)
					if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
						return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;

					break;
				}

				//case FORM_CAT:
				//case FORM_TRAVEL:
				//case FORM_AQUA:
				//case FORM_BEAR:
				//case FORM_AMBIENT:
				//case FORM_GHOUL:
				//case FORM_DIREBEAR:
				//case FORM_CREATUREBEAR:
				//case FORM_GHOSTWOLF:

				case FORM_SPIRITOFREDEMPTION:
				{
					//Spirit of Redemption (20711) fix
					if (!(GetProto()->c_is_flags & SPELL_FLAG_IS_HEALING) && GetProto()->Id != 7355)
						return SPELL_FAILED_CASTER_DEAD;
					break;
				}
				

				default:
				{
					// check if item is allowed (only special & equipped items allowed in other forms)
					if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
						if(i_caster->GetProto()->InventoryType == INVTYPE_NON_EQUIP)
							return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;
				}
			}
		}

		//Check if spell allowed while out of stealth
		if(m_spellInfo->Attributes & ATTRIBUTES_REQ_STEALTH && !p_caster->IsStealth()) //Stealth check
		{
			return SPELL_FAILED_ONLY_STEALTHED;
		}

		// item spell checks
		if(i_caster && i_caster->GetProto()) //Let's just make sure there's something here, so we don't crash ;)
		{
			if( i_caster->GetProto()->ZoneNameID && i_caster->GetProto()->ZoneNameID != i_caster->GetZoneId() )
				return SPELL_FAILED_NOT_HERE;
			if( i_caster->GetProto()->MapID && i_caster->GetProto()->MapID != i_caster->GetMapId() )
				return SPELL_FAILED_NOT_HERE;

			if(i_caster->GetProto()->Spells[0].Charges != 0)
			{
				// check if the item has the required charges
				if(i_caster->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES) == 0)
					return SPELL_FAILED_NO_CHARGES_REMAIN;

				// for items that combine to create a new item, check if we have the required quantity of the item
				if(i_caster->GetProto()->ItemId == GetProto()->Reagent[0])
					if(p_caster->GetItemInterface()->GetItemCount(GetProto()->Reagent[0]) < 1 + GetProto()->ReagentCount[0])
						return SPELL_FAILED_ITEM_GONE;
			}
		}

		// check if we have the required reagents
		if (!(p_caster->removeReagentCost && GetProto()->AttributesExD & 2) )
		{
			for(i=0; i<8 ;i++)
			{
				if( GetProto()->Reagent[i] == 0 || GetProto()->ReagentCount[i] == 0)
					continue;

				if(p_caster->GetItemInterface()->GetItemCount(GetProto()->Reagent[i]) < GetProto()->ReagentCount[i])
					return SPELL_FAILED_ITEM_GONE;
			}
		}

		// check if we have the required tools, totems, etc
		for(i=0; i<2 ;i++)
		{
			if( GetProto()->Totem[i] != 0)
			{
				if(!p_caster->GetItemInterface()->GetItemCount(GetProto()->Totem[i]))
					return SPELL_FAILED_TOTEMS;
			}
		}

		// stealth check
		//Shady: :\ ATTRIBUTES_REQ_OOC owns!!!
		/*if( GetProto()->NameHash == SPELL_HASH_STEALTH )
		{
			if( p_caster->CombatStatus.IsInCombat() )
				return SPELL_FAILED_TARGET_IN_COMBAT;
		}*/

		// check if we have the required gameobject focus
		float focusRange;

		if( GetProto()->RequiresSpellFocus)
		{
			bool found = false;

			m_caster->AquireInrangeLock(); //make sure to release lock before exit function !
			for(std::set<Object*>::iterator itr = p_caster->GetInRangeSetBegin(); itr != p_caster->GetInRangeSetEnd(); itr++ )
			{
				if((*itr)->GetTypeId() != TYPEID_GAMEOBJECT)
					continue;

				if((*itr)->GetUInt32Value(GAMEOBJECT_TYPE_ID) != GAMEOBJECT_TYPE_SPELL_FOCUS)
					continue;

				GameObjectInfo *info = ((GameObject*)(*itr))->GetInfo();
				if(!info)
				{
					sLog.outDebug("Warning: could not find info about game object %u",(*itr)->GetEntry());
					continue;
				}

				// professions use rangeIndex 1, which is 0yds, so we will use 5yds, which is standard interaction range.
				if(info->sound1)
					focusRange = float(info->sound1);
				else
					focusRange = GetMaxRange(dbcSpellRange.LookupEntry(GetProto()->rangeIndex));

				// check if focus object is close enough
				if(!IsInrange(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), (*itr), (focusRange * focusRange)))
					continue;

				if(info->SpellFocus == GetProto()->RequiresSpellFocus)
				{
					found = true;
					break;
				}
			}

			m_caster->ReleaseInrangeLock();
			if(!found)
				return SPELL_FAILED_REQUIRES_SPELL_FOCUS;
		}

		if (GetProto()->RequiresAreaId && GetProto()->RequiresAreaId != p_caster->GetMapMgr()->GetAreaID(p_caster->GetPositionX(),p_caster->GetPositionY()))
			return SPELL_FAILED_REQUIRES_AREA;

		// aurastate check
		if( GetProto()->CasterAuraState )
		{
			if( !p_caster->HasFlag( UNIT_FIELD_AURASTATE, GetProto()->CasterAuraState ) )
				return SPELL_FAILED_CASTER_AURASTATE;
		}

		// Let's not allow players to blink trew gates.
		// Until we fix the real problem this will work.
		if( p_caster->m_bg && !p_caster->m_bg->HasStarted() )
		{
			if( GetProto()->NameHash == SPELL_HASH_BLINK )
				return SPELL_FAILED_SPELL_UNAVAILABLE;
		}
	}

	// Targetted Item Checks
	if(m_targets.m_itemTarget && p_caster)
	{
		Item *i_target = NULL;

		// check if the targeted item is in the trade box
		if( m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM )
		{
			switch( GetProto()->Effect[0] )
			{
				// only lockpicking and enchanting can target items in the trade box
				case SPELL_EFFECT_OPEN_LOCK:
				case SPELL_EFFECT_ENCHANT_ITEM:
				case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
				{
					// check for enchants that can only be done on your own items
					if( GetProto()->AttributesExB & FLAGS3_ENCHANT_OWN_ONLY )
						return SPELL_FAILED_BAD_TARGETS;

					// get the player we are trading with
					Player* t_player = p_caster->GetTradeTarget();
					// get the targeted trade item
					if( t_player != NULL )
						i_target = t_player->getTradeItem((uint32)m_targets.m_itemTarget);
				}
			}
		}
		// targeted item is not in a trade box, so get our own item
		else
		{
			i_target = p_caster->GetItemInterface()->GetItemByGUID( m_targets.m_itemTarget );
		}

		// check to make sure we have a targeted item
		if( i_target == NULL )
			return SPELL_FAILED_BAD_TARGETS;

		ItemPrototype* proto = i_target->GetProto();

		// check to make sure we have it's prototype info
		if(!proto) return SPELL_FAILED_BAD_TARGETS;

		// check to make sure the targeted item is acceptable
		switch(GetProto()->Effect[0])
		{
			// Lock Picking Targeted Item Check
			case SPELL_EFFECT_OPEN_LOCK:
			{
				// this is currently being handled in SpellEffects
				break;
			}

			// Enchanting Targeted Item Check
			case SPELL_EFFECT_ENCHANT_ITEM:
			case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
			{
				// check if we have the correct class, subclass, and inventory type of target item
				if( GetProto()->EquippedItemClass != (int32)proto->Class)
					return SPELL_FAILED_BAD_TARGETS;

				if( GetProto()->EquippedItemSubClass && !(GetProto()->EquippedItemSubClass & (1 << proto->SubClass)))
					return SPELL_FAILED_BAD_TARGETS;

				if( GetProto()->RequiredItemFlags && !(GetProto()->RequiredItemFlags & (1 << proto->InventoryType)))
					return SPELL_FAILED_BAD_TARGETS;

				if (GetProto()->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM &&
					GetProto()->baseLevel && (GetProto()->baseLevel > proto->ItemLevel))
					return int8(SPELL_FAILED_BAD_TARGETS); // maybe there is different err code

				if( i_caster && i_caster->GetProto()->Flags == 2097216)
					break;

				if( GetProto()->AttributesExB & FLAGS3_ENCHANT_OWN_ONLY && !(i_target->IsSoulbound()))
					return SPELL_FAILED_BAD_TARGETS;

				break;
			}

			// Disenchanting Targeted Item Check
			case SPELL_EFFECT_DISENCHANT:
			{
				// check if item can be disenchanted
				if(proto->DisenchantReqSkill < 1)
					return SPELL_FAILED_CANT_BE_DISENCHANTED;

				// check if we have high enough skill
				if((int32)p_caster->_GetSkillLineCurrent(SKILL_ENCHANTING) < proto->DisenchantReqSkill)
					return SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL;

				break;
			}

			// Feed Pet Targeted Item Check
			case SPELL_EFFECT_FEED_PET:
			{
				Pet *pPet = p_caster->GetSummon();

				// check if we have a pet
				if(!pPet)
					return SPELL_FAILED_NO_PET;
				
				// check if pet lives
				if( !pPet->isAlive() )
					return SPELL_FAILED_TARGETS_DEAD;

				// check if item is food
				if(!proto->FoodType)
					return SPELL_FAILED_BAD_TARGETS;

				// check if food type matches pets diet
				if(!(pPet->GetPetDiet() & (1 << (proto->FoodType - 1))))
					return SPELL_FAILED_WRONG_PET_FOOD;

				// check food level: food should be max 30 lvls below pets level
				if(pPet->getLevel() > proto->ItemLevel + 30)
					return SPELL_FAILED_FOOD_LOWLEVEL;

				break;
			}

			// Prospecting Targeted Item Check
			case SPELL_EFFECT_PROSPECTING:
			{
				// check if the item can be prospected
				if(!(proto->Flags & ITEM_FLAG_PROSPECTABLE))
					return SPELL_FAILED_CANT_BE_PROSPECTED;

				// check if we have at least 5 of the item
				if(p_caster->GetItemInterface()->GetItemCount(proto->ItemId) < 5)
					return SPELL_FAILED_ITEM_GONE;

				// check if we have high enough skill
				if(p_caster->_GetSkillLineCurrent(SKILL_JEWELCRAFTING) < proto->RequiredSkillRank)
					return SPELL_FAILED_LOW_CASTLEVEL;

				break;
			}
		}
	}

	// set up our max Range
	float maxRange = GetMaxRange( dbcSpellRange.LookupEntry( GetProto()->rangeIndex ) );
		// latency compensation!!
		// figure out how much extra distance we need to allow for based on our movespeed and latency.
		if( u_caster && m_caster->GetMapMgr() && m_targets.m_unitTarget )
		{
			Unit * utarget;

			utarget = m_caster->GetMapMgr()->GetUnit( m_targets.m_unitTarget );
			if( utarget && utarget->IsPlayer() && static_cast< Player* >( utarget )->m_isMoving )
				{
					// this only applies to PvP.
					uint32 lat = static_cast< Player* >( utarget )->GetSession() ? static_cast< Player* >( utarget )->GetSession()->GetLatency() : 0;

					// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
					lat = ( lat > 500 ) ? 500 : lat;

					// calculate the added distance
					maxRange += ( u_caster->m_runSpeed * 0.001f ) * float( lat );
				}
		}
	if( GetProto()->SpellGroupType && u_caster != NULL )
	{
		SM_FFValue( u_caster->SM_FRange, &maxRange, GetProto()->SpellGroupType );
		SM_PFValue( u_caster->SM_PRange, &maxRange, GetProto()->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		float spell_pct_modifers=0;
		SM_FFValue(u_caster->SM_FRange,&spell_flat_modifers,GetProto()->SpellGroupType);
		SM_FFValue(u_caster->SM_PRange,&spell_pct_modifers,GetProto()->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxRange,GetProto()->SpellGroupType);
#endif
	}

	// Targeted Location Checks (AoE spells)
	if( m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION )
	{
		if( !IsInrange( m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, m_caster, ( maxRange * maxRange ) ) )
			return SPELL_FAILED_OUT_OF_RANGE;
	}

	Unit *target = NULL;

	// Targeted Unit Checks
	if(m_targets.m_unitTarget)
	{
		target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULL;

		if(target)
		{
			// UNIT_FIELD_BOUNDINGRADIUS + 1.5f; seems to match the client range

			if( tolerate ) // add an extra 33% to range on final check (squared = 1.78x)
			{
				float localrange=maxRange + target->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS) + 1.5f;
				if( !IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, ( localrange * localrange * 1.78f ) ) )
					return SPELL_FAILED_OUT_OF_RANGE;
			}
			else
			{
				float localrange=maxRange + target->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS) + 1.5f;
				if( !IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, ( localrange * localrange ) ) )
					return SPELL_FAILED_OUT_OF_RANGE;
			}

			if( p_caster != NULL )
			{
				if( GetProto()->Id == SPELL_RANGED_THROW)
				{
					Item * itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(!itm || itm->GetDurability() == 0)
						return SPELL_FAILED_NO_AMMO;
				}

				if (sWorld.Collision) {
					if (p_caster->GetMapId() == target->GetMapId() && !CollideInterface.CheckLOS(p_caster->GetMapId(),p_caster->GetPositionNC(),target->GetPositionNC()))
						return SPELL_FAILED_LINE_OF_SIGHT;
				}

				// check aurastate
				if( GetProto()->TargetAuraState )
				{
					if( !target->HasFlag( UNIT_FIELD_AURASTATE, GetProto()->TargetAuraState ) )
					{
						return SPELL_FAILED_TARGET_AURASTATE;
					}
				}

				if(target->IsPlayer())
				{
					// disallow spell casting in sanctuary zones
					// allow attacks in duels
					if( p_caster->DuelingWith != target && !isFriendly( p_caster, target ) )
					{
						AreaTable* atCaster = dbcArea.LookupEntry( p_caster->GetAreaID() );
						AreaTable* atTarget = dbcArea.LookupEntry( static_cast< Player* >( target )->GetAreaID() );
						if( atCaster->AreaFlags & 0x800 || atTarget->AreaFlags & 0x800 )
							return SPELL_FAILED_NOT_HERE;
					}
				}
				else
				{
					if (target->GetAIInterface()->GetIsSoulLinked() && u_caster && target->GetAIInterface()->getSoullinkedWith() != u_caster)
						return SPELL_FAILED_BAD_TARGETS;
				}

				// pet training
				if( GetProto()->EffectImplicitTargetA[0] == EFF_TARGET_PET &&
					GetProto()->Effect[0] == SPELL_EFFECT_LEARN_SPELL )
				{
					Pet *pPet = p_caster->GetSummon();
					// check if we have a pet
					if( pPet == NULL )
						return SPELL_FAILED_NO_PET;

					// other checks
					SpellEntry* trig = dbcSpell.LookupEntry( GetProto()->EffectTriggerSpell[0] );
					if( trig == NULL )
						return SPELL_FAILED_SPELL_UNAVAILABLE;

					uint32 status = pPet->CanLearnSpell( trig );
					if( status != 0 )
						return static_cast<uint8>(status);
				}

				if( GetProto()->EffectApplyAuraName[0]==2)//mind control
				{
					if( GetProto()->EffectBasePoints[0])//got level req;
					{
						if((int32)target->getLevel() > GetProto()->EffectBasePoints[0]+1 + int32(p_caster->getLevel() - GetProto()->spellLevel))
							return SPELL_FAILED_HIGHLEVEL;
						else if(target->GetTypeId() == TYPEID_UNIT)
						{
							Creature * c = (Creature*)(target);
							if (c&&c->GetCreatureInfo()&&c->GetCreatureInfo()->Rank >ELITE_ELITE)
								return SPELL_FAILED_HIGHLEVEL;
						}
					}
				}
			}

			// scripted spell stuff
			switch(GetProto()->Id)
			{
				case 1515: // tame beast
				{
                    
					uint8 result = 0;
					Unit* tgt = unitTarget;
					if( tgt == NULL )
					{
						// we have to pick a target manually as this is a dummy spell which triggers tame effect at end of channeling
						if( p_caster->GetSelection() != 0 )
							tgt =  p_caster->GetMapMgr()->GetUnit( p_caster->GetSelection() );
						else
							return SPELL_FAILED_UNKNOWN;
					}

					Creature *tame = tgt->GetTypeId() == TYPEID_UNIT ? ( Creature* ) tgt : NULL;
                    
					if ( tame == NULL )
						result = PETTAME_INVALIDCREATURE;
					else if( !tame->isAlive() )
						result = PETTAME_DEAD;
					else if( tame->IsPet() )
						result = PETTAME_CREATUREALREADYOWNED;
					else if( !tame->GetCreatureInfo() || tame->GetCreatureInfo()->Type != BEAST || !tame->GetCreatureInfo()->Family )
						result = PETTAME_NOTTAMEABLE;
					else if( !p_caster->isAlive() || p_caster->getClass() != HUNTER )
						result = PETTAME_UNITSCANTTAME;
					else if( tame->getLevel() > p_caster->getLevel() )
						result = PETTAME_TOOHIGHLEVEL;
					else if( p_caster->GetSummon() || p_caster->GetUnstabledPetNumber() )
						result = PETTAME_ANOTHERSUMMONACTIVE;
					else if( p_caster->GetPetCount() >= 3 )
						result = PETTAME_TOOMANY;
					else
					{
						CreatureFamilyEntry* cf = dbcCreatureFamily.LookupEntry( tame->GetCreatureInfo()->Family );
						if( cf && !cf->tameable )
							result = PETTAME_NOTTAMEABLE;
					}
					if( result != 0 )
					{
						SendTameFailure( result );
						return SPELL_FAILED_DONT_REPORT;
					}
				}break;
				case 2699:
				{
					if(target->GetEntry() != 5307 || target->isAlive())
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 30877:
				{
					if(target->GetEntry() != 17326 && target != m_caster)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 34665:
				{
					if(target->GetEntry() != 16880)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 3607:
				{
					if(target->GetEntry() != 2530)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 36310:
				{
					if(target->GetEntry() != 20058)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 41291:
				{
					if(target->GetEntry() != 22357)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 28369: // Gas
				{
					if( !target->IsCreature() || target->GetEntry() != 18879 ) // Phase Hunter
						return SPELL_FAILED_BAD_TARGETS;
				} break;
				case 29528: // Inoculation
				{
					if( !target->IsCreature() || target->GetEntry() != 16518 ) // Nestlewood Owlkin
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 41621: // Wolpertinger Net
				{
					if(!target->IsCreature() || target->GetEntry()!=23487 ) // Wild Wolpertinger
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 32578: // Gor'drek's Ointment
				{
					if(!target->IsCreature() || target->GetEntry()!=20748) // Thunderlord Dire Wolf NPC
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 44997: // Converting Sentry
				{
					if( !target->IsCreature() || target->GetEntry()!=24972 ) // Erratic Sentry
						return SPELL_FAILED_BAD_TARGETS;

					if( !target->IsCreature() || !target->isDead() )
						return SPELL_FAILED_TARGET_NOT_DEAD;
				}break;
				case 30077: // Carinda's Retribution
				{
					if(!target->IsCreature() || target->GetEntry()!=17226 ) // Viera Sunwhisper
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 32825: // Soul Cannon
				{
					if( !target->IsCreature() || target->GetEntry() != 22357 ) // Reth'hedron the Subduer
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 27907: // Disciplinary Rod
				{
					if(!target->IsCreature() || target->GetEntry() != 15945 && target->GetEntry() != 15941 ) // 'Apprentice Meledor' and 'Apprentice Ralen'
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 19938: // Awaken Peon (Foreman's Blackjack)
				{
					if( !target->IsCreature() || target->GetEntry() != 10556 ) // Lazy Peon
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 603: //curse of doom, can't be casted on players
				case 30910:
				{
					if(target->IsPlayer())
						return SPELL_FAILED_TARGET_IS_PLAYER;
				}break;
				case 13907: // Smite Demon
				{
					if ( target->IsPlayer() || target->getClass()!=TARGET_TYPE_DEMON )
						return SPELL_FAILED_SPELL_UNAVAILABLE;
				}break;
				// disable spell
				case 38554: //Absorb Eye of Grillok
				{
					// do not allow spell to be cast
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}break;
				case 36314: //The Seer's Presence
				{
					// this spell can be cast only on socrethar. Otherwife cool exploit
					if(target->IsPlayer() || !target->IsUnit())
						return SPELL_FAILED_BAD_TARGETS;
					// this spell should be used only for a specific quest on specific monster = "Socrethar"
					if(target->GetEntry()!=20132) //nasty fixed numbers :(
						return SPELL_FAILED_BAD_TARGETS;
				}break;
				case 982: //Revive Pet
				{
					Pet *pPet = p_caster->GetSummon();
					if(pPet && !pPet->isDead())
						return SPELL_FAILED_TARGET_NOT_DEAD;
				}break;
				case 38177: //Blackwhelp Net
				{
					if( !target->IsCreature() || target->GetEntry()!= 21387 ) // castable only on Wyrmcult Blackwhelps
						return SPELL_FAILED_BAD_TARGETS;
				} break;
				case 35772: // Energy Field Modulator
				{
					if ( !target->IsCreature() || target->GetEntry() != 20774 ) // castable only on Farahlon Lasher
						return SPELL_FAILED_BAD_TARGETS;
				} break;
			}

			// if the target is not the unit caster and not the masters pet
			if(target != u_caster && !m_caster->IsPet())
			{
				// Dummy spells check
				switch ( GetProto()->Id )
				{
					case 4130: // Banish Burning Exile
					{
						if(target->GetEntry()!= 2760) // target needs to be a Burning Exile
							return SPELL_FAILED_BAD_TARGETS;
					} break;
					case 4131:// Banish Cresting Exile
					{
						if(target->GetEntry()!= 2761) // target needs to be a Cresting Exile
							return SPELL_FAILED_BAD_TARGETS;
					} break;
					case 4132:// Banish Thundering Exile
					{
						if(target->GetEntry()!= 2762) // target needs to be a Thundering Exile
							return SPELL_FAILED_BAD_TARGETS;
					} break;
				}
				/***********************************************************
				* Inface checks, these are checked in 2 ways
				* 1e way is check for damage type, as 3 is always ranged
				* 2e way is trough the data in the extraspell db
				*
				**********************************************************/

				/* burlex: units are always facing the target! */
				if(p_caster && 	GetProto()->FacingCasterFlags != SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK )
				{
					if( GetProto()->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED )
					{ // our spell is a ranged spell
						if(!p_caster->isInFront(target))
							return SPELL_FAILED_UNIT_NOT_INFRONT;
					}
					else
					{ // our spell is not a ranged spell
						if( GetProto()->FacingCasterFlags == SPELL_INFRONT_STATUS_REQUIRE_INFRONT )
						{
							// must be in front
							if(!u_caster->isInFront(target))
								return SPELL_FAILED_UNIT_NOT_INFRONT;
						}
						else if( GetProto()->FacingCasterFlags == SPELL_INFRONT_STATUS_REQUIRE_INBACK)
						{
							// behind
							if(target->isInFront(u_caster))
								return SPELL_FAILED_NOT_BEHIND;
						}
					}
				}
			}

			// if target is already skinned, don't let it be skinned again
			if( GetProto()->Effect[0] == SPELL_EFFECT_SKINNING) // skinning
				if(target->IsUnit() && (((Creature*)target)->Skinned) )
					return SPELL_FAILED_TARGET_UNSKINNABLE;

			// all spells with target 61 need to be in group or raid
			// TODO: need to research this if its not handled by the client!!!
			if(
				GetProto()->EffectImplicitTargetA[0] == 61 ||
				GetProto()->EffectImplicitTargetA[1] == 61 ||
				GetProto()->EffectImplicitTargetA[2] == 61)
			{
				if( target->IsPlayer() && !static_cast< Player* >( target )->InGroup() )
					return SPELL_FAILED_NOT_READY;//return SPELL_FAILED_TARGET_NOT_IN_PARTY or SPELL_FAILED_TARGET_NOT_IN_PARTY;
			}

			// pet's owner stuff
			/*if (GetProto()->EffectImplicitTargetA[0] == 27 ||
				GetProto()->EffectImplicitTargetA[1] == 27 ||
				GetProto()->EffectImplicitTargetA[2] == 27)
			{
				if (!target->IsPlayer())
					return SPELL_FAILED_TARGET_NOT_PLAYER; //if you are there something is very wrong
			}*/

			// target 39 is fishing, all fishing spells are handled
			if( GetProto()->EffectImplicitTargetA[0] == 39 )//||
			 //GetProto()->EffectImplicitTargetA[1] == 39 ||
			 //GetProto()->EffectImplicitTargetA[2] == 39)
			{
				uint32 entry = GetProto()->EffectMiscValue[0];
				if(entry == GO_FISHING_BOBBER)
				{
					//uint32 mapid = p_caster->GetMapId();
					float px=u_caster->GetPositionX();
					float py=u_caster->GetPositionY();
					//float pz=u_caster->GetPositionZ();
					float orient = m_caster->GetOrientation();
					float posx = 0,posy = 0,posz = 0;
					float co = cos(orient);
					float si = sin(orient);
					MapMgr * map = m_caster->GetMapMgr();

					float r;
					for(r=20; r>10; r--)
					{
						posx = px + r * co;
						posy = py + r * si;
						/*if(!(map->GetWaterType(posx,posy) & 1))//water
							continue;*/
						posz = map->GetWaterHeight(posx,posy);
						if(posz > map->GetLandHeight(posx,posy))//water
							break;
					}
					if(r<=10)
						return SPELL_FAILED_NOT_FISHABLE;

					// if we are already fishing, dont cast it again
					if(p_caster->GetSummonedObject())
						if(p_caster->GetSummonedObject()->GetEntry() == GO_FISHING_BOBBER)
							return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}

			if( p_caster != NULL )
			{
				if( GetProto()->NameHash == SPELL_HASH_GOUGE )// Gouge
					if(!target->isInFront(p_caster))
						return SPELL_FAILED_NOT_INFRONT;

				if( GetProto()->Category==1131)//Hammer of wrath, requires target to have 20- % of hp
				{
					if(target->GetUInt32Value(UNIT_FIELD_HEALTH) == 0)
						return SPELL_FAILED_BAD_TARGETS;

					if(target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/target->GetUInt32Value(UNIT_FIELD_HEALTH)<5)
						 return SPELL_FAILED_BAD_TARGETS;
				}
				else if( GetProto()->Category==672)//Conflagrate, requires immolation spell on victim
				{
					if(!target->HasAuraVisual(46))
						return SPELL_FAILED_BAD_TARGETS;
				}

				if(target->dispels[GetProto()->DispelType])
					return SPELL_FAILED_PREVENTED_BY_MECHANIC-1;			// hackfix - burlex

				// Removed by Supalosa and moved to 'completed cast'
				//if(target->MechanicsDispels[GetProto()->MechanicsType])
				//	return SPELL_FAILED_PREVENTED_BY_MECHANIC-1; // Why not just use 	SPELL_FAILED_DAMAGE_IMMUNE                                   = 144,
			}

			// if we're replacing a higher rank, deny it
			AuraCheckResponse acr = target->AuraCheck(GetProto()->NameHash, GetProto()->RankNumber,m_caster);
			if( acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT )
				return SPELL_FAILED_AURA_BOUNCED;

			//check if we are trying to stealth or turn invisible but it is not allowed right now
			if( IsStealthSpell() || IsInvisibilitySpell() )
			{
				//if we have Faerie Fire, we cannot stealth or turn invisible
				if( u_caster->FindAuraByNameHash( SPELL_HASH_FAERIE_FIRE ) || u_caster->FindAuraByNameHash( SPELL_HASH_FAERIE_FIRE__FERAL_ ) )
					return SPELL_FAILED_SPELL_UNAVAILABLE;
			}

			/*SpellReplacement*rp=objmgr.GetReplacingSpells(GetProto()->Id);
			if(rp)
			{
				if(isAttackable(u_caster,target))//negative, replace only our own spell
				{
					for(uint32 x=0;x<rp->count;x++)
					{
						if(target->HasActiveAura(rp->spells[x],m_caster->GetGUID()))
						{
							return SPELL_FAILED_AURA_BOUNCED;
						}
					}
				}
				else
				{
					for(uint32 x=0;x<rp->count;x++)
					{
						if(target->HasActiveAura(rp->spells[x]))
						{
							return SPELL_FAILED_AURA_BOUNCED;
						}
					}
				}
			}	*/
		}
	}

	//Checking for Debuffs that dont allow power word:shield, those Pala spells, ice block or use first aid, hacky, is there any way to check if he has "immune mechanic"?
	if (GetProto()->MechanicsType == MECHANIC_SHIELDED && ((target) ? target->HasAura(6788) : u_caster->HasAura(6766))) //Weakened Soul
		return SPELL_FAILED_DAMAGE_IMMUNE;
	if (GetProto()->MechanicsType == MECHANIC_INVULNARABLE && ((target) ? target->HasAura(25771) : u_caster->HasAura(25771))) //Forbearance
		return SPELL_FAILED_DAMAGE_IMMUNE;
	if( GetProto()->NameHash == SPELL_HASH_AVENGING_WRATH && ((target) ? target->HasAura(25771) : u_caster->HasAura(25771))) // forbearance, avenging wrath
		return SPELL_FAILED_DAMAGE_IMMUNE;
	if (GetProto()->NameHash == SPELL_HASH_ICE_BLOCK && u_caster->HasAura(41425))
		return SPELL_FAILED_DAMAGE_IMMUNE;
	if (GetProto()->MechanicsType == MECHANIC_HEALING && ((target) ? target->HasAura(11196) : u_caster->HasAura(11196)))
		return SPELL_FAILED_DAMAGE_IMMUNE;

	// Special State Checks (for creatures & players)
	if( u_caster )
	{
		if( u_caster->SchoolCastPrevent[GetProto()->School] )
		{
			uint32 now_ = getMSTime();
			if( now_ > u_caster->SchoolCastPrevent[GetProto()->School] )//this limit has expired,remove
				u_caster->SchoolCastPrevent[GetProto()->School] = 0;
			else
			{
				// HACK FIX
				switch( GetProto()->NameHash )
				{
					// This is actually incorrect. school lockouts take precedence over silence.
					// So ice block/divine shield are not usable while their schools are locked out,
					// but can be used while silenced.
					/*case SPELL_HASH_ICE_BLOCK: //Ice Block
					case 0x9840A1A6: //Divine Shield
						break;
					*/
					case 0x3DFA70E5: //Will of the Forsaken
						{
							if( u_caster->m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP ) )
								break;
						}break;

					case 0xF60291F4: //Death Wish
					case 0xD77038F4: //Fear Ward
					case 0x19700707: //Berserker Rage
						{
							if( u_caster->m_special_state & UNIT_STATE_FEAR )
								break;
						}break;

					// {Insignia|Medallion} of the {Horde|Alliance}
					case 0xC7C45478: //Immune Movement Impairment and Loss of Control
					case 0x048c32f9:	// insignia of the alliance/horde
					case 0xDD06F1BF: // Stop fucking renaming the spell, Blizzard! (This time it's PvP Trinket)
						break;


						{
							if( u_caster->m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_ROOT | UNIT_STATE_STUN | UNIT_STATE_CONFUSE | UNIT_STATE_SNARE ) )
								break;
						}
							break;

					case 0xCD4CDF55: // Barksin
					{ // This spell is usable while stunned, frozen, incapacitated, feared or asleep.  Lasts 12 sec.
						if( u_caster->m_special_state & ( UNIT_STATE_STUN | UNIT_STATE_FEAR | UNIT_STATE_SLEEP ) ) // Uh, what unit_state is Frozen? (freezing trap...)
							break;
					}

					default:
						return SPELL_FAILED_SILENCED;
				}
			}
		}

		if( u_caster && u_caster->m_silenced && GetProto()->School != NORMAL_DAMAGE )// can only silence non-physical
		{
			switch( GetProto()->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				break;

				default:
				return SPELL_FAILED_SILENCED;
			}
		}

		if(target) /* -Supalosa- Shouldn't this be handled on Spell Apply? */
		{
			for( int i = 0; i < 3; i++ ) // if is going to cast a spell that breaks stun remove stun auras, looks a bit hacky but is the best way i can find
			{
				if( GetProto()->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY )
				{
					target->RemoveAllAurasByMechanic( GetProto()->EffectMiscValue[i] , static_cast<uint32>(-1) , true );
					// Remove all debuffs of that mechanic type.
					// This is also done in SpellAuras.cpp - wtf?
				}
				/*
				if( GetProto()->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY && (GetProto()->EffectMiscValue[i] == 12 || GetProto()->EffectMiscValue[i] == 17))
				{
					for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
						if(target->m_auras[x])
							if(target->m_auras[x]->GetSpellProto()->MechanicsType == GetProto()->EffectMiscValue[i])
								target->m_auras[x]->Remove();
				}
				*/
			}
		}

		if( u_caster->IsPacified() && GetProto()->School == NORMAL_DAMAGE ) // only affects physical damage
		{
			// HACK FIX
			switch( GetProto()->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case 0x9840A1A6: //Divine Shield
				case 0x3DFA70E5: //Will of the Forsaken
				{
					if( u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP))
						break;
				}break;

				default:
					return SPELL_FAILED_PACIFIED;
			}
		}

		if( u_caster->IsStunned() || u_caster->IsFeared())
		{
			// HACK FIX
			switch( GetProto()->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				case SPELL_HASH_DIVINE_PROTECTION: //Divine Protection
				case 0xCD4CDF55: //Barkskin
					break;
				/* -Supalosa- For some reason, being charmed or sleep'd is counted as 'Stunned'.
				Check it: http://www.wowhead.com/?spell=700 */

				case 0xC7C45478: /* Immune Movement Impairment and Loss of Control (PvP Trinkets) */
					break;

				case 0x3DFA70E5: /* Will of the Forsaken (Undead Racial) */
					break;

				case 0x048c32f9:	// insignia of the alliance/horde
				case 0xDD06F1BF: // Stop fucking renaming the spell, Blizzard! (This time it's PvP Trinket)
					break;

				case SPELL_HASH_BERSERKER_RAGE://Berserker Rage frees the caster from fear effects.
					{
						if( u_caster->IsStunned() )
							return SPELL_FAILED_STUNNED;
					}break;

				default:
					return SPELL_FAILED_STUNNED;
			}
		}

		if(u_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			SpellEntry * t_spellInfo = (u_caster->GetCurrentSpell() ? u_caster->GetCurrentSpell()->GetProto() : NULL);

			if(!t_spellInfo || !m_triggeredSpell)
				return SPELL_FAILED_SPELL_IN_PROGRESS;
			else if (t_spellInfo)
			{
				if(
					t_spellInfo->EffectTriggerSpell[0] != GetProto()->Id &&
					t_spellInfo->EffectTriggerSpell[1] != GetProto()->Id &&
					t_spellInfo->EffectTriggerSpell[2] != GetProto()->Id)
				{
					return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}
		}
	}

	
	// no problems found, so we must be ok
	return SPELL_CANCAST_OK;
}

void Spell::RemoveItems()
{
	// Item Charges & Used Item Removal
	if(i_caster)
	{
		// Stackable Item -> remove 1 from stack
		if ( i_caster->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > 1 )
		{
			i_caster->ModUnsigned32Value( ITEM_FIELD_STACK_COUNT, -1 );
			i_caster->m_isDirty = true;
		}
		else
		{
			for ( uint32 x = 0; x < 5; x++ )
			{
				int32 charges = (int32)i_caster->GetUInt32Value( ITEM_FIELD_SPELL_CHARGES + x );
				if ( charges == -1 ) // if expendable item && item has no charges remaining -> delete item
				{
					//i bet this crashed happend due to some script. Items wihtout owners ?
					if( i_caster->GetOwner() && i_caster->GetOwner()->GetItemInterface() )
						i_caster->GetOwner()->GetItemInterface()->SafeFullRemoveItemByGuid( i_caster->GetGUID() );
					i_caster = NULL;
					break;
				}
				else if ( charges > 0 || charges < -1 ) // remove 1 charge
				{
					i_caster->ModSignedInt32Value( ITEM_FIELD_SPELL_CHARGES + x, ( charges < 0 ) ? +1 : -1 ); // if charges < 0 then -1 else +1
					i_caster->m_isDirty = true;
					break;
				}
			}
		}
	}
	// Ammo Removal
	if( GetProto()->AttributesExB == FLAGS3_REQ_RANGED_WEAPON || GetProto()->AttributesExC & FLAGS4_PLAYER_RANGED_SPELLS)
	{
		p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(p_caster->GetUInt32Value(PLAYER_AMMO_ID), 1, &i_caster);
	}

	// Reagent Removal
	if (!(p_caster->removeReagentCost && GetProto()->AttributesExD & 2) )
	{
		for(uint32 i=0; i<8 ;i++)
		{
			if( GetProto()->Reagent[i])
			{
				p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(GetProto()->Reagent[i], GetProto()->ReagentCount[i], &i_caster);
			}
		}
	}
}

int32 Spell::CalculateEffect(uint32 i,Unit *target)
{
	// TODO: Add ARMOR CHECKS; Add npc that have ranged weapons use them;

	// Range checks
 /*   if (GetProto()->Id == SPELL_RANGED_GUN) // this includes bow and gun
	{
		if(!u_caster || !unitTarget)
			return 0;

		return ::CalculateDamage( u_caster, unitTarget, RANGED, GetProto()->SpellGroupType );
	}
*/
	int32 value = 0;

	float basePointsPerLevel    = GetProto()->EffectRealPointsPerLevel[i];
	float randomPointsPerLevel  = GetProto()->EffectDicePerLevel[i];
	int32 basePoints = GetProto()->EffectBasePoints[i] + 1;
	int32 randomPoints = GetProto()->EffectDieSides[i];

	//added by Zack : some talents inherit their basepoints from the previously casted spell: see mage - Master of Elements
	if(forced_basepoints[i])
		basePoints = forced_basepoints[i];

	/* Random suffix value calculation */
	if(i_caster && (int32(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID)) < 0))
	{
        ItemRandomSuffixEntry * si = dbcItemRandomSuffix.LookupEntry(abs(int(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID))));
		EnchantEntry * ent;
		uint32 j,k;

		for(j = 0; j < 3; ++j)
		{
			if(si->enchantments[j] != 0)
			{
				ent = dbcEnchant.LookupEntry(si->enchantments[j]);
				for(k = 0; k < 3; ++k)
				{
					if(ent->spell[k] == GetProto()->Id)
					{
						if(si->prefixes[k] == 0)
							goto exit;

						value = RANDOM_SUFFIX_MAGIC_CALCULATION(si->prefixes[j], i_caster->GetItemRandomSuffixFactor());

						if(value == 0)
							goto exit;

						return value;
					}
				}
			}
		}
	}
exit:

	if( u_caster != NULL )
	{
		int32 diff = -(int32)GetProto()->baseLevel;
		if (GetProto()->maxLevel && u_caster->getLevel()>GetProto()->maxLevel)
			diff +=GetProto()->maxLevel;
		else
			diff +=u_caster->getLevel();
		randomPoints += float2int32(diff * randomPointsPerLevel);
		basePoints += float2int32(diff * basePointsPerLevel );
	}

	if(randomPoints <= 1)
		value = basePoints;
	else
		value = basePoints + rand() % randomPoints;

	int32 comboDamage = (int32)GetProto()->EffectPointsPerComboPoint[i];
	if(comboDamage && p_caster != NULL )
	{
		m_requiresCP = true;
		value += ( comboDamage * p_caster->m_comboPoints );
			//this is ugly so i will explain the case maybe someone ha a better idea :
			// while casting a spell talent will trigger uppon the spell prepare faze
			// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
		p_caster->m_spellcomboPoints = 0;
	}

	//scripted shit
	if( GetProto()->Id == 34120)
	{	//A steady shot that causes ${$RAP*0.3+$m1} damage.
		//	Actual Equation (http://www.wowwiki.com/Steady_Shot)
		//		* The tooltip is proven to be wrong and the following is the best player worked out formula so far with data taken from [1]
		//		* Formula: DamagePercentageBonus*RangedWeaponSpecialization*(150 + WeaponDamage/WeaponSpeed*2.8 + 0.2*RAP + [Dazed: 175])
		if(i==0 && u_caster)
		{
			if( p_caster != NULL )
			{
				Item *it;
				if(p_caster->GetItemInterface())
				{
					it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(it)
					{
						float weapondmg = RandomFloat(1)*(it->GetProto()->Damage[0].Max - it->GetProto()->Damage[0].Min) + it->GetProto()->Damage[0].Min;
						value += float2int32(150 + weapondmg/float(it->GetProto()->Delay/1000.0f)*2.8f);
					}
				}
			}
			if(target && target->IsDazed() )
				value += 175;
			value += (uint32)(u_caster->GetRAP()*0.2);
		}
	}
	else if( GetProto()->NameHash == SPELL_HASH_EVISCERATE ) //Eviscerate
	{
		if (p_caster != NULL) {
			value += (uint32)( p_caster->GetAP() *  0.03f * p_caster->m_comboPoints  );
		}
	}
	else if( GetProto()->NameHash == SPELL_HASH_FEROCIOUS_BITE )
	{
		if (p_caster != NULL) {
			value += (uint32)( ( p_caster->GetAP() * 0.1526f ) + ( p_caster->GetUInt32Value( UNIT_FIELD_POWER4 ) * GetProto()->dmg_multiplier[i] ) );
			p_caster->SetUInt32Value( UNIT_FIELD_POWER4, 0 );
		}
	}
	else if( GetProto()->Id == 34123) //Druid - Tree of Life
	{
		if( i==0 && p_caster != NULL )
		{
			//Increases healing received by 25% of the Tree of Life's total spirit.
			value = p_caster->GetUInt32Value( UNIT_FIELD_STAT4 ) >> 2;
		}
	}
	// HACK FIX
	else if( GetProto()->NameHash == SPELL_HASH_VICTORY_RUSH )
	{//causing ${$AP*$m1/100} damage
		if(i==0 && u_caster)
			value = (value*u_caster->GetAP())/100;
	}
	else if( GetProto()->NameHash == SPELL_HASH_RAKE )//rake
	{
		//Rake the target for ${$AP/100+$m1} bleed damage and an additional ${$m2*3+$AP*0.06} damage over $d.
		if( u_caster != NULL )
		{
			float ap = (float)u_caster->GetAP();
			if(i==0)
				value+=(uint32)ceilf((ap*0.01f));	// / 100
			else if(i==1)
				value=(int32)ceilf((float(value * 3) + ceilf((ap*0.06f))) / 3.0f);
		}
	}
	else if( GetProto()->NameHash == SPELL_HASH_GARROTE )
	{
		// WoWWiki says +( 0.18 * attack power / number of ticks )
		// Tooltip gives no specific reading, but says ", increased by your attack power.".
		if( u_caster != NULL )
		{
			if( i == 0 )
			{
				value += (uint32) ceilf( ( u_caster->GetAP() * 0.18f ) / 6 );
			}
		}

	}
	else if( GetProto()->NameHash == SPELL_HASH_RUPTURE )
	{
		/*
		1pt = Attack Power * 0.04 + x
		2pt = Attack Power * 0.10 + y
		3pt = Attack Power * 0.18 + z
		4pt = Attack Power * 0.21 + a
		5pt = Attack Power * 0.24 + b
		*/
		if( p_caster != NULL && i == 0 )
		{
			int8 cp = p_caster->m_comboPoints;
			value += (uint32) ceilf( ( u_caster->GetAP() * 0.04f * cp ) / ( ( 6 + ( cp * 2 ) ) / 2 ) );
		}
	}
	else if( GetProto()->NameHash == SPELL_HASH_RIP ) //rip
	{
		if( u_caster != NULL )
			value += (uint32)ceilf(u_caster->GetAP() * 0.036f);
	}
	else if( GetProto()->NameHash == SPELL_HASH_MONGOOSE_BITE ) //Mongoose Bite
	{
		// ${$AP*0.2+$m1} damage.
		if( u_caster != NULL )
			value+=u_caster->GetAP()/5;
	}
	else if( GetProto()->NameHash == SPELL_HASH_SWIPE ) // Swipe
	{
		// ${$AP*0.06+$m1} damage.
		if( u_caster != NULL )
			value+=float2int32(u_caster->GetAP()*0.06f);
	}
	else if ( GetProto()->Id == 34501 && ( i == 0 || i == 1 ) ) //Hunter - Expose Weakness
	{
		if (u_caster != NULL) {
//			value = ( u_caster->GetUInt32Value( UNIT_FIELD_STAT1 ) * 25 / 100 );
			value = u_caster->GetUInt32Value( UNIT_FIELD_STAT1 ) >> 2;
		}
	}
/*	else if ( GetProto()->NameHash == SPELL_HASH_HUNTER_S_MARK && target && target->HasAurasWithNameHash( SPELL_HASH_HUNTER_S_MARK ) ) //Hunter - Hunter's Mark
	{
		value = value / 10; //aditional stacks only increase value by X
	}*/

	if( p_caster != NULL )
	{
		if( GetProto()->NameHash == SPELL_HASH_ENVENOM && i == 0 )//Envenom
		{
			value *= p_caster->m_comboPoints;
			value += (uint32)(p_caster->GetAP()*(0.03f*p_caster->m_comboPoints));
			m_requiresCP=true;
		}

		SpellOverrideMap::iterator itr = p_caster->mSpellOverrideMap.find(GetProto()->Id);
		if(itr != p_caster->mSpellOverrideMap.end())
		{
			ScriptOverrideList::iterator itrSO;
			for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
			{
        // Capt: WHAT THE FUCK DOES THIS MEAN....
				// Supa: WHAT THE FUCK DOES THIS MEAN?
				value += RandomUInt((*itrSO)->damage);
			}
		}
	 }

	// TODO: INHERIT ITEM MODS FROM REAL ITEM OWNER - BURLEX BUT DO IT PROPERLY

	if( u_caster != NULL )
	{
		SM_FIValue(u_caster->SM_FMiscEffect,&value,GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PMiscEffect,&value,GetProto()->SpellGroupType);

		SM_FIValue(u_caster->SM_FEffectBonus,&value,GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PEffectBonus,&value,GetProto()->SpellGroupType);
	}
	else if( i_caster != NULL && target)
	{
		//we should inherit the modifiers from the conjured food caster
		Unit *item_creator = target->GetMapMgr()->GetUnit( i_caster->GetUInt64Value( ITEM_FIELD_CREATOR ) );
		if( item_creator != NULL )
		{
			SM_FIValue(item_creator->SM_FMiscEffect,&value,GetProto()->SpellGroupType);
			SM_PIValue(item_creator->SM_PMiscEffect,&value,GetProto()->SpellGroupType);

			SM_FIValue(item_creator->SM_FEffectBonus,&value,GetProto()->SpellGroupType);
			SM_PIValue(item_creator->SM_PEffectBonus,&value,GetProto()->SpellGroupType);
		}
	}

	return value;
}

void Spell::HandleTeleport(uint32 id, Unit* Target)
{
	if(Target->GetTypeId()!=TYPEID_PLAYER)
		return;

	Player* pTarget = static_cast< Player* >( Target );

	float x,y,z;
	uint32 mapid;

	// predefined behavior
	if (GetProto()->Id == 8690 || GetProto()->Id == 556 || GetProto()->Id == 39937)// 556 - Astral Recall ; 39937 - Ruby Slippers
	{
		x = pTarget->GetBindPositionX();
		y = pTarget->GetBindPositionY();
		z = pTarget->GetBindPositionZ();
		mapid = pTarget->GetBindMapId();
	}
	else // normal behavior
	{
		TeleportCoords* TC = TeleportCoordStorage.LookupEntry(id);
		if(!TC)
			return;

		x=TC->x;
		y=TC->y;
		z=TC->z;
		mapid=TC->mapId;
	}

	pTarget->EventAttackStop();
	pTarget->SetSelection(0);

	// We use a teleport event on this one. Reason being because of UpdateCellActivity,
	// the game object set of the updater thread WILL Get messed up if we teleport from a gameobject
	// caster.
	if(!sEventMgr.HasEvent(pTarget, EVENT_PLAYER_TELEPORT))
		sEventMgr.AddEvent(pTarget, &Player::EventTeleport, mapid, x, y, z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::CreateItem(uint32 itemId)
{
    if( !itemId )
        return;

	Player*			pUnit = static_cast< Player* >( m_caster );
	Item*			newItem;
	Item*			add;
	SlotResult		slotresult;
	ItemPrototype*	m_itemProto;

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemId );
	if( m_itemProto == NULL )
	    return;

	if (pUnit->GetItemInterface()->CanReceiveItem(m_itemProto, 1))
	{
		SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
		return;
	}

	add = pUnit->GetItemInterface()->FindItemLessMax(itemId, 1, false);
	if (!add)
	{
		slotresult = pUnit->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
		if(!slotresult.Result)
		{
			 SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			 return;
		}

		newItem = objmgr.CreateItem(itemId, pUnit);
		AddItemResult result = pUnit->GetItemInterface()->SafeAddItem(newItem, slotresult.ContainerSlot, slotresult.Slot);
		if(!result)
		{
			newItem->DeleteMe();
			return;
		}

		newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
		newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, damage);

		/*WorldPacket data(45);
		p_caster->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		p_caster->SendMessageToSet(&data, true);*/
		p_caster->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,1);
		newItem->m_isDirty = true;

	}
	else
	{
		add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + damage);
		/*WorldPacket data(45);
		p_caster->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		p_caster->SendMessageToSet(&data, true);*/
		p_caster->GetSession()->SendItemPushResult(add,true,false,true,false,p_caster->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,1);
		add->m_isDirty = true;
	}
}

/*void Spell::_DamageRangeUpdate()
{
	if(unitTarget)
	{
		if(unitTarget->isAlive())
		{
			m_caster->SpellNonMeleeDamageLog(unitTarget,GetProto()->Id, damageToHit);
		}
		else
		{	if( u_caster != NULL )
			if(u_caster->GetCurrentSpell() != this)
			{
					if(u_caster->GetCurrentSpell() != NULL)
					{
						u_caster->GetCurrentSpell()->SendChannelUpdate(0);
						u_caster->GetCurrentSpell()->cancel();
					}
			}
			SendChannelUpdate(0);
			cancel();
		}
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
	else if(gameObjTarget)
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
		//Go Support
	}
	else
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
}*/

void Spell::SendHealSpellOnPlayer(Object* caster, Object* target, uint32 dmg,bool critical)
{
	if(!caster || !target || !target->IsPlayer())
		return;
	WorldPacket data(SMSG_HEALSPELL_ON_PLAYER, 27);
	// Bur: I know it says obsolete, but I just logged this tonight and got this packet.

	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << (pSpellId ? pSpellId : GetProto()->Id);
	data << uint32(dmg);	// amt healed
	data << uint8(critical);	 //this is critical message

	caster->SendMessageToSet(&data, true);
}

void Spell::Heal(int32 amount, bool ForceCrit)
{
	int32 base_amount = amount; //store base_amount for later use

	if(!unitTarget || !unitTarget->isAlive())
		return;

	if( p_caster != NULL )
		p_caster->last_heal_spell=GetProto();

    //self healing shouldn't flag himself
	if(p_caster && playerTarget && p_caster != playerTarget)
	{
		// Healing a flagged target will flag you.
		if(playerTarget->IsPvPFlagged())
			p_caster->SetPvPFlag();
	}

	//Make it critical
	bool critical = false;
	int32 critchance = 0;
	int32 bonus = 0;
	float healdoneaffectperc = 1.0f;
	if( u_caster != NULL )
	{
		//Downranking
		if(p_caster && p_caster->IsPlayer() && GetProto()->baseLevel > 0 && GetProto()->maxLevel > 0)
		{
			float downrank1 = 1.0f;
			if (GetProto()->baseLevel < 20)
				downrank1 = 1.0f - (20.0f - float (GetProto()->baseLevel) ) * 0.0375f;
			float downrank2 = ( float(GetProto()->maxLevel + 5.0f) / float(p_caster->getLevel()) );
			if (downrank2 >= 1 || downrank2 < 0)
				downrank2 = 1.0f;
			healdoneaffectperc *= downrank1 * downrank2;
		}

		//Spells Not affected by Bonus Healing
		if(GetProto()->NameHash == SPELL_HASH_SEAL_OF_LIGHT) //Seal of Light
			healdoneaffectperc = 0.0f;

		if(GetProto()->NameHash == SPELL_HASH_LESSER_HEROISM) //Lesser Heroism
			healdoneaffectperc = 0.0f;

		if(GetProto()->NameHash == SPELL_HASH_HEROISM) //Heroism, a.k.a. Darkmoon Card: Heroism
			healdoneaffectperc = 0.0f;

		if(GetProto()->NameHash == SPELL_HASH_CITRINE_PENDANT_OF_GOLDEN_HEALING) //Citrine Pendant of Golden Healing
			healdoneaffectperc = 0.0f;

		if(GetProto()->NameHash == SPELL_HASH_LIVING_RUBY_PENDANT) //Living Ruby Pendant
			healdoneaffectperc = 0.0f;

		//Spells affected by Bonus Healing
		if(GetProto()->NameHash == SPELL_HASH_EARTH_SHIELD) //Earth Shield
			healdoneaffectperc = 0.3f;

		//Basic bonus
		bonus += u_caster->HealDoneMod[GetProto()->School];
		bonus += unitTarget->HealTakenMod[GetProto()->School];

		//Bonus from Intellect & Spirit
		if( p_caster != NULL )
		{
			for(uint32 a = 0; a < 6; a++)
				bonus += float2int32(p_caster->SpellHealDoneByAttribute[a][GetProto()->School] * p_caster->GetUInt32Value(UNIT_FIELD_STAT0 + a));
		}

		//Spell Coefficient
		if(  GetProto()->Dspell_coef_override >= 0 ) //In case we have forced coefficients
			bonus = float2int32( float( bonus ) * GetProto()->Dspell_coef_override );
		else
		{
			//Bonus to DH part
			if( GetProto()->fixed_dddhcoef >= 0 )
				bonus = float2int32( float( bonus ) * GetProto()->fixed_dddhcoef );
		}

		critchance = float2int32(u_caster->spellcritperc + u_caster->SpellCritChanceSchool[GetProto()->School]);

		if(GetProto()->SpellGroupType)
		{
			int penalty_pct = 0;
			int penalty_flt = 0;
			SM_FIValue( u_caster->SM_PPenalty, &penalty_pct, GetProto()->SpellGroupType );
			bonus += amount*penalty_pct/100;
			SM_FIValue( u_caster->SM_FPenalty, &penalty_flt, GetProto()->SpellGroupType );
			bonus += penalty_flt;
			SM_FIValue( u_caster->SM_CriticalChance,&critchance,GetProto()->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			int spell_flat_modifers=0;
			int spell_pct_modifers=0;
			SM_FIValue(u_caster->SM_FPenalty,&spell_flat_modifers,GetProto()->SpellGroupType);
			SM_FIValue(u_caster->SM_PPenalty,&spell_pct_modifers,GetProto()->SpellGroupType);
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
				printf("!!!!!HEAL : spell dmg bonus(p=24) mod flat %d , spell dmg bonus(p=24) pct %d , spell dmg bonus %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,bonus,GetProto()->SpellGroupType);
#endif
		}

		if(u_caster->IsPlayer())
		{
			if(m_spellInfo->NameHash == SPELL_HASH_FLASH_HEAL || m_spellInfo->NameHash == SPELL_HASH_BINDING_HEAL || m_spellInfo->NameHash == SPELL_HASH_GREATER_HEAL)
			{
				if(u_caster->HasAura(34754))
					u_caster->RemoveAura(34754, u_caster->GetGUID());
			}
		}

		// Paladin: Healing Light talent
		if(u_caster->IsPlayer())
		{
			if(m_spellInfo->NameHash == SPELL_HASH_FLASH_OF_LIGHT || m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT)
			{
				uint8 TalentRank = 0;
				if(static_cast<Player*>(u_caster)->HasSpell(20237))
					TalentRank = 1;
				if(static_cast<Player*>(u_caster)->HasSpell(20238))
					TalentRank = 2;
				if(static_cast<Player*>(u_caster)->HasSpell(20239))
					TalentRank = 3;

				if(TalentRank)
				{
					float TalentPct = 0.0f;
					switch(TalentRank)
					{
						case 1:
							TalentPct = 0.04f;
							break;
						case 2:
							TalentPct = 0.08f;
							break;
						case 3:
							TalentPct = 0.12f;
							break;
						default:
							TalentPct = 0.0f;
							break;
					}
					TalentPct += 1.0f;
					amount = (int)( amount * TalentPct );
				}
			}
		}

		amount += float2int32( float( bonus ) * healdoneaffectperc ); //apply downranking on final value ?
		amount += amount*u_caster->HealDonePctMod[GetProto()->School]/100;
		amount += float2int32( float( amount ) * unitTarget->HealTakenPctMod[GetProto()->School] );

		if (GetProto()->SpellGroupType)
			SM_PIValue(u_caster->SM_PDamageBonus,&amount,GetProto()->SpellGroupType);

		if( ((critical = Rand(critchance))  != 0) || ForceCrit )
		{
			/*int32 critbonus = amount >> 1;
			if( GetProto()->SpellGroupType)
					SM_PIValue(static_cast<Unit*>(u_caster)->SM_PCriticalDamage, &critbonus, GetProto()->SpellGroupType);
			amount += critbonus;*/

			int32 critical_bonus = 100;
			if( GetProto()->SpellGroupType )
				SM_FIValue( u_caster->SM_PCriticalDamage, &critical_bonus, GetProto()->SpellGroupType );

			if( critical_bonus > 0 )
			{
				// the bonuses are halved by 50% (funky blizzard math :S)
				float b = ( ( float(critical_bonus) / 2.0f ) / 100.0f );
				amount += float2int32( float(amount) * b );
			}

			//Shady: does it correct> caster casts heal and proc ..._VICTIM ?
			// Or mb i'm completely wrong? So if true  - just replace with old string.
			//u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, unitTarget, GetProto(), amount);
			//Replaced with following one:


			unitTarget->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, u_caster, GetProto(), amount);
			u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT, unitTarget, GetProto(), amount);
		}

	}

	if(amount < 0)
		amount = 0;

	if( p_caster != NULL )
	{
		if( unitTarget->IsPlayer() )
		{
			SendHealSpellOnPlayer( p_caster, static_cast< Player* >( unitTarget ), amount, critical );
		}
		p_caster->m_bgScore.HealingDone += amount;
		if( p_caster->m_bg != NULL )
			p_caster->m_bg->UpdatePvPData();
	}
	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 maxHealth = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if((curHealth + amount) >= maxHealth)
	{
		amount = maxHealth - curHealth;
		unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, maxHealth);
	}
	else
		unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH, amount);

	if (p_caster)
	{
		p_caster->m_casted_amount[GetProto()->School]=amount;
		p_caster->HandleProc( PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, GetProto() );
	}

	unitTarget->RemoveAurasByHeal();

	// add threat
	if( u_caster != NULL )
	{
		std::vector<Unit*> target_threat;
		int count = 0;
		for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT || !static_cast<Unit *>(*itr)->CombatStatus.IsInCombat() || (static_cast<Unit *>(*itr)->GetAIInterface()->getThreatByPtr(u_caster) == 0 && static_cast<Unit *>(*itr)->GetAIInterface()->getThreatByPtr(unitTarget) == 0))
				continue;

			target_threat.push_back(static_cast<Unit *>(*itr));
			count++;
		}
		if (count == 0)
			return;

		amount = amount / count;

		for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
		{
			static_cast<Unit *>(*itr)->GetAIInterface()->HealReaction(u_caster, unitTarget, m_spellInfo, amount);
		}

		// remember that we healed (for combat status)
		if(unitTarget->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(unitTarget);
	}
}

void Spell::DetermineSkillUp(uint32 skillid,uint32 targetlevel)
{
	if(!p_caster)return;
	if(p_caster->GetSkillUpChance(skillid)<0.01)return;//to preven getting higher skill than max
	int32 diff=p_caster->_GetSkillLineCurrent(skillid,false)/5-targetlevel;
	if(diff<0)diff=-diff;
	float chance;
	if(diff<=5)chance=95.0;
	else if(diff<=10)chance=66;
	else if(diff <=15)chance=33;
	else return;
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

void Spell::DetermineSkillUp(uint32 skillid)
{
	//This code is wrong for creating items and disenchanting.
	if(!p_caster)return;
	float chance = 0.0f;
	skilllinespell* skill = objmgr.GetSpellSkill(GetProto()->Id);
	if( skill != NULL && static_cast< Player* >( m_caster )->_HasSkillLine( skill->skilline ) )
	{
		uint32 amt = static_cast< Player* >( m_caster )->_GetSkillLineCurrent( skill->skilline, false );
		uint32 max = static_cast< Player* >( m_caster )->_GetSkillLineMax( skill->skilline );
		if( amt >= max )
			return;
		if( amt >= skill->grey ) //grey
			chance = 0.0f;
		else if( ( amt >= ( ( ( skill->grey - skill->green) / 2 ) + skill->green ) ) ) //green
			chance = 33.0f;
		else if( amt >= skill->green ) //yellow
			chance = 66.0f;
		else //brown
			chance=100.0f;
	}
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

void Spell::SafeAddTarget(TargetsList* tgt,uint64 guid)
{
	if(guid == 0)
		return;

	for(TargetsList::iterator i=tgt->begin();i!=tgt->end();i++)
		if((*i)==guid)
			return;

	tgt->push_back(guid);
}

void Spell::SafeAddMissedTarget(uint64 guid)
{
    for(SpellTargetsList::iterator i=ModeratedTargets.begin();i!=ModeratedTargets.end();i++)
        if((*i).TargetGuid==guid)
        {
            //sLog.outDebug("[SPELL] Something goes wrong in spell target system");
			// this isnt actually wrong, since we only have one missed target map,
			// whereas hit targets have multiple maps per effect.
            return;
        }

    ModeratedTargets.push_back(SpellTargetMod(guid,2));
}

void Spell::SafeAddModeratedTarget(uint64 guid, uint16 type)
{
	for(SpellTargetsList::iterator i=ModeratedTargets.begin();i!=ModeratedTargets.end();i++)
		if((*i).TargetGuid==guid)
        {
            //sLog.outDebug("[SPELL] Something goes wrong in spell target system");
			// this isnt actually wrong, since we only have one missed target map,
			// whereas hit targets have multiple maps per effect.
			return;
        }

	ModeratedTargets.push_back(SpellTargetMod(guid, (uint8)type));
}

bool Spell::Reflect(Unit *refunit)
{
	SpellEntry * refspell = NULL;

	if( m_reflectedParent != NULL || refunit == NULL || m_caster == refunit )
		return false;

	// if the spell to reflect is a reflect spell, do nothing.
	for(int i=0; i<3; i++)
	{
		if( GetProto()->Effect[i] == 6 && (GetProto()->EffectApplyAuraName[i] == 74 || GetProto()->EffectApplyAuraName[i] == 28))
			return false;
	}
	for(std::list<struct ReflectSpellSchool*>::iterator i = refunit->m_reflectSpellSchool.begin();i != refunit->m_reflectSpellSchool.end();i++)
	{
		ReflectSpellSchool *rss = *i;
		if(rss->school == -1 || rss->school == (int32)GetProto()->School)
		{
			if(Rand((float)rss->chance))
			{
				//the god blessed special case : mage - Frost Warding = is an augmentation to frost warding
				if(rss->require_aura_hash && !refunit->HasAurasWithNameHash(rss->require_aura_hash))
                {
					continue;
                }
				if (rss->charges > 0)
				{
					rss->charges--;
					if (rss->charges <= 0)
					{
						refunit->m_reflectSpellSchool.erase(i);
						refunit->RemoveAura(rss->spellId);
					}
				}
				refspell = GetProto();
				break;
			}
		}
	}

	if(!refspell) return false;

	Spell *spell = SpellPool.PooledNew();
	spell->Init(refunit, refspell, true, NULL);
	spell->SetReflected();
	SpellCastTargets targets;
	targets.m_unitTarget = m_caster->GetGUID();
	spell->prepare(&targets);

	return true;
}

void ApplyDiminishingReturnTimer(uint32 * Duration, Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	// TODO: check for spells that should do this
	float Dur = float(*Duration);

	switch(Target->m_diminishCount[Grp])
	{
	case 0: // Full effect
		if (( Target->IsPlayer() || Target->IsPet() ) && Dur > 10000)
		{
			Dur = 10000;

			// for this group only lower duration to 10s
			if (status == DIMINISHING_GROUP_NOT_DIMINISHED) {
				*Duration = FL2UINT(Dur);
				return;
			}
		}
		break;

	case 1: // Reduced by 50%
		Dur *= 0.5f;
		if (( Target->IsPlayer() || Target->IsPet() ) && Dur > 5000)
		{
			Dur = 5000;
		}
		break;

	case 2: // Reduced by 75%
		Dur *= 0.25f;
		if (( Target->IsPlayer() || Target->IsPet() ) && Dur > 2500)
		{
			Dur = 2500;
		}
		break;

	default:// Target immune to spell
		{
			*Duration = 0;
			return;
		}break;
	}

	// Convert back
	*Duration = FL2UINT(Dur);

	// Reset the diminishing return counter, and add to the aura count (we don't decrease the timer till we
	// have no auras of this type left.
	//++Target->m_diminishAuraCount[Grp];
	++Target->m_diminishCount[Grp];
}

void UnapplyDiminishingReturnTimer(Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;
	uint32 aura_grp;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	//Target->m_diminishAuraCount[Grp]--;
	
	/*There are cases in which you just refresh an aura duration instead of the whole aura,
	causing corruption on the diminishAura counter and locking the entire diminishing group.
	So it's better to check the active auras one by one*/
	Target->m_diminishAuraCount[Grp] = 0;
	for( uint32 x = MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; x++ )
	{
		if( Target->m_auras[x] )
		{	
			aura_grp = GetDiminishingGroup( Target->m_auras[x]->GetSpellProto()->NameHash );
			if( aura_grp == status )
				Target->m_diminishAuraCount[Grp]++;
		}
	}

	// start timer decrease
	if(!Target->m_diminishAuraCount[Grp])
	{
		Target->m_diminishActive = true;
		Target->m_diminishTimer[Grp] = 15000;
	}
}

/// Calculate the Diminishing Group. This is based on a name hash.
/// this off course is very hacky, but as its made done in a proper way
/// I leave it here.
uint32 GetDiminishingGroup(uint32 NameHash)
{
	int32 grp = -1;
	bool pve = false;

	switch(NameHash)
	{
	case SPELL_HASH_BASH:
	case SPELL_HASH_IMPACT:
	case SPELL_HASH_CHEAP_SHOT:
	case SPELL_HASH_SHADOWFURY:
	case SPELL_HASH_CHARGE_STUN:
	case SPELL_HASH_INTERCEPT_STUN:
	case SPELL_HASH_CONCUSSION_BLOW:
	case SPELL_HASH_INTIMIDATION:
	case SPELL_HASH_WAR_STOMP:
	case SPELL_HASH_POUNCE:
		{
			grp = DIMINISHING_GROUP_STUN;
			pve = true;
		}break;

	case SPELL_HASH_STARFIRE_STUN:
 	case SPELL_HASH_STONECLAW_STUN:
	case SPELL_HASH_STUN:					// Generic ones
	case SPELL_HASH_BLACKOUT:
	case SPELL_HASH_MACE_STUN_EFFECT:		// Mace Specialization
		{
			grp = DIMINISHING_GROUP_STUN_PROC;
			pve = true;
		}break;

	case SPELL_HASH_ENTANGLING_ROOTS:
	case SPELL_HASH_FROST_NOVA:
			grp = DIMINISHING_GROUP_ROOT;
		break;

	case SPELL_HASH_IMPROVED_WING_CLIP:	
	case SPELL_HASH_FROSTBITE:
	case SPELL_HASH_IMPROVED_HAMSTRING:
	case SPELL_HASH_ENTRAPMENT:
			grp = DIMINISHING_GROUP_ROOT_PROC;
		break;

	case SPELL_HASH_HIBERNATE:
 	case SPELL_HASH_WYVERN_STING:
 	case SPELL_HASH_SLEEP:
 	case SPELL_HASH_RECKLESS_CHARGE:		//Gobling Rocket Helmet
			grp = DIMINISHING_GROUP_SLEEP;
		break;

	case SPELL_HASH_CYCLONE:
	case SPELL_HASH_BLIND:
		{
			grp = DIMINISHING_GROUP_BLIND_CYCLONE;
			pve = true;
		}break;

	case SPELL_HASH_GOUGE:
	case SPELL_HASH_REPENTANCE:			// Repentance
	case SPELL_HASH_SAP:
	case SPELL_HASH_POLYMORPH:				// Polymorph
	case SPELL_HASH_POLYMORPH__CHICKEN:		// Chicken
	case SPELL_HASH_POLYMORPH__PIG:			// Pig
	case SPELL_HASH_POLYMORPH__TURTLE:		// Turtle
	case SPELL_HASH_POLYMORPH__SHEEP:		// Good ol' sheep
	case SPELL_HASH_MAIM:					// Maybe here?
			grp = DIMINISHING_GROUP_GOUGE_POLY_SAP;
		break;

	case SPELL_HASH_FEAR:
	case SPELL_HASH_PSYCHIC_SCREAM:
	case SPELL_HASH_SEDUCTION:
	case SPELL_HASH_HOWL_OF_TERROR:
	case SPELL_HASH_SCARE_BEAST:
			grp = DIMINISHING_GROUP_FEAR;
		break;

	case SPELL_HASH_ENSLAVE_DEMON:			// Enslave Demon
	case SPELL_HASH_MIND_CONTROL:
			grp = DIMINISHING_GROUP_CHARM;		//Charm???
		break;

	case SPELL_HASH_KIDNEY_SHOT:
		{
			grp = DIMINISHING_GROUP_KIDNEY_SHOT;
			pve = true;
		}break;

	case SPELL_HASH_DEATH_COIL:
			grp = DIMINISHING_GROUP_HORROR;
		break;
	

	case SPELL_HASH_BANISH:					// Banish
			grp = DIMINISHING_GROUP_BANISH;
		break;

	// group where only 10s limit is pvp is applied, not DR
	case SPELL_HASH_FREEZING_TRAP_EFFECT:	// Freezing Trap Effect
	case SPELL_HASH_HAMSTRING:	// Hamstring
	case SPELL_HASH_CURSE_OF_TONGUES:
		{
			grp = DIMINISHING_GROUP_NOT_DIMINISHED;
		}break;

	case SPELL_HASH_RIPOSTE:			// Riposte
	case SPELL_HASH_DISARM:			// Disarm
		{
			grp = DIMINISHING_GROUP_DISARM;
		}break;
	}

	uint32 ret;
	if( pve )
		ret = grp | (1 << 16);
	else
		ret = grp;

	return ret;
}

void Spell::SendCastSuccess(Object * target)
{
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr||!plr->IsPlayer())
		return;

	WorldPacket data(SMSG_TARGET_CAST_RESULT, 13);
	data << ((target != 0) ? target->GetNewGUID() : uint64(0));
	data << GetProto()->Id;

	plr->GetSession()->SendPacket(&data);
}

void Spell::SendCastSuccess(const uint64& guid)
{
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr || !plr->IsPlayer())
		return;

	// fuck bytebuffers
	unsigned char buffer[13];
	uint32 c = FastGUIDPack(guid, buffer, 0);
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[c] = swap32(GetProto()->Id);         c += 4;
#else
	*(uint32*)&buffer[c] = GetProto()->Id;                 c += 4;
#endif

	plr->GetSession()->OutPacket(SMSG_TARGET_CAST_RESULT, c, buffer);
}
/*
bool IsBeneficSpell(SpellEntry *sp)
{
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = sp->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = sp->EffectImplicitTargetB[i];
			switch(cur)
			{
				case EFF_TARGET_SELF:
				case EFF_TARGET_PET:
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6
				case 57:// Targeted Party Member
				case 27: // target is owner of pet
				case EFF_TARGET_MINION:// Minion Target
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
				case EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
				case 40://Activate Object target(probably based on focus)
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
				case 61: // targets with the same group/raid and the same class
				case 32:
				case 73:
					return true;
			}//end switch
		}//end for
	return false;
}

AI_SpellTargetType RecommandAISpellTargetType(SpellEntry *sp)
{
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = sp->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = sp->EffectImplicitTargetB[i];
			switch(cur)
			{
				case EFF_TARGET_NONE:
				case EFF_TARGET_GAMEOBJECT:
				case EFF_TARGET_GAMEOBJECT_ITEM:// Gameobject/Item Target
				case EFF_TARGET_SELF_FISHING://Fishing
				case 47:// Portal
				case 52:	// Lightwells, etc
				case 40://Activate Object target(probably based on focus)
					return TTYPE_NULL;

				case EFF_TARGET_SELF:
				case 38://Dummy Target
				case 32:
				case 73:
					return TTYPE_CASTER;

				case EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
					return TTYPE_DESTINATION;

				case 4: // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
				case EFF_TARGET_PET:
				case EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
				case 77:					// grep: i think this fits
				case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
				case EFF_TARGET_IN_FRONT_OF_CASTER:
				case 31:// related to scripted effects
				case 53:// Target Area by Players CurrentSelection()
				case 54:// Targets in Front of the Caster
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6
				case 57:// Targeted Party Member
				case EFF_TARGET_DUEL:
				case 27: // target is owner of pet
				case EFF_TARGET_MINION:// Minion Target
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
				case 61: // targets with the same group/raid and the same class
					return TTYPE_SINGLETARGET;

					// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
				case 17: // A single target at a xyz location or the target is a possition xyz
				case 18:// Land under caster.Maybe not correct
				case 46://Unknown Summon Atal'ai Skeleton
				case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
					return TTYPE_SOURCE;

			}//end switch
		}//end for
	return TTYPE_NULL;// this means a new spell :P
}
*/
