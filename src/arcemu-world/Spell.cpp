/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

extern const char* SpellEffectNames[TOTAL_SPELL_EFFECTS];

enum SpellTargetSpecification
{
    TARGET_SPECT_NONE		= 0,
    TARGET_SPEC_INVISIBLE	= 1,
    TARGET_SPEC_DEAD		= 2,
};

bool CanAttackCreatureType(uint32 TargetTypeMask, uint32 type)
{
	uint32 cmask = 1 << (type - 1);

	if(type != 0 &&
	        TargetTypeMask != 0 &&
	        ((TargetTypeMask & cmask) == 0))
		return false;
	else
		return true;
}

void SpellCastTargets::read(WorldPacket & data, uint64 caster)
{
	m_unitTarget = m_itemTarget = 0;
	m_srcX = m_srcY = m_srcZ = m_destX = m_destY = m_destZ = 0;
	m_strTarget = "";

	data >> m_targetMask;
	data >> m_targetMaskExtended;
	WoWGuid guid;

	if(m_targetMask == TARGET_FLAG_SELF)
	{
		switch(*(uint32*)((data.contents()) + 1)) // Spell ID
		{
			case 14285: // Arcane Shot (Rank 6)
			case 14286: // Arcane Shot (Rank 7)
			case 14287: // Arcane Shot (Rank 8)
			case 27019: // Arcane Shot (Rank 9)
			case 49044: // Arcane Shot (Rank 10)
			case 49045: // Arcane Shot (Rank 11)
			case 15407: // Mind Flay (Rank 1)
			case 17311: // Mind Flay (Rank 2)
			case 17312: // Mind Flay (Rank 3)
			case 17313: // Mind Flay (Rank 4)
			case 17314: // Mind Flay (Rank 5)
			case 18807: // Mind Flay (Rank 6)
			case 25387: // Mind Flay (Rank 7)
			case 48155: // Mind Flay (Rank 8)
			case 48156: // Mind Flay (Rank 9)
				{
					m_targetMask = TARGET_FLAG_UNIT;
					Player* plr = objmgr.GetPlayer((uint32)caster);
					if(plr != NULL)
						m_unitTarget = plr->GetTargetGUID();
				}
				break;
			default:
				m_unitTarget = caster;
				break;
		}
		return;
	}

	if(m_targetMask & (TARGET_FLAG_OBJECT | TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2))
	{
		data >> guid;
		m_unitTarget = guid.GetOldGuid();
	}

	if(m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
	{
		data >> guid;
		m_itemTarget = guid.GetOldGuid();
	}

	if(m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
	{
		WoWGuid guid;
		
		data >> guid;
		unkuint64_1 = guid.GetOldGuid();
		
		data >> m_srcX;
		data >> m_srcY;
		data >> m_srcZ;

		if(!(m_targetMask & TARGET_FLAG_DEST_LOCATION))
		{
			m_destX = m_srcX;
			m_destY = m_srcY;
			m_destZ = m_srcZ;
		}
	}

	if(m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		WoWGuid guid;
		data >> guid;
		unkuint64_2 = guid.GetOldGuid();
		
		data >> m_destX;
		data >> m_destY;
		data >> m_destZ;

		if(!(m_targetMask & TARGET_FLAG_SOURCE_LOCATION))
		{
			m_srcX = m_destX;
			m_srcY = m_destY;
			m_srcZ = m_destZ;
		}
	}

	if(m_targetMask & TARGET_FLAG_STRING)
	{
		std::string ss;
		data >> ss;
		m_strTarget = strdup(ss.c_str());
	}
}

void SpellCastTargets::write(WorldPacket & data)
{
	data << m_targetMask;
	data << m_targetMaskExtended;

	if(/*m_targetMask == TARGET_FLAG_SELF || */m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT | TARGET_FLAG_GLYPH))
		FastGUIDPack(data, m_unitTarget);

	if(m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
		FastGUIDPack(data, m_itemTarget);

	if(m_targetMask & TARGET_FLAG_SOURCE_LOCATION){
		data << WoWGuid( unkuint64_1 );		
		data << m_srcX;
		data << m_srcY;
		data << m_srcZ;
	}

	if(m_targetMask & TARGET_FLAG_DEST_LOCATION){
		data << WoWGuid( unkuint64_2 );		
		data << m_destX;
		data << m_destY;
		data << m_destZ;
	}

	if(m_targetMask & TARGET_FLAG_STRING)
		data << m_strTarget.c_str();
}

Spell::Spell(Object* Caster, SpellEntry* info, bool triggered, Aura* aur)
{
	ARCEMU_ASSERT(Caster != NULL && info != NULL);

	Caster->m_pendingSpells.insert(this);
	m_overrideBasePoints = false;
	m_overridenBasePoints[0] = 0xFFFFFFFF;
	m_overridenBasePoints[1] = 0xFFFFFFFF;
	m_overridenBasePoints[2] = 0xFFFFFFFF;
	chaindamage = 0;
	bDurSet = 0;
	damage = 0;
	m_spellInfo_override = 0;
	bRadSet[0] = 0;
	bRadSet[1] = 0;
	bRadSet[2] = 0;

	if( ( info->SpellDifficultyID != 0 ) && ( Caster->GetTypeId() != TYPEID_PLAYER ) && ( Caster->GetMapMgr() != NULL ) && ( Caster->GetMapMgr()->pInstance != NULL ) )
	{
		SpellEntry* SpellDiffEntry = sSpellFactoryMgr.GetSpellEntryByDifficulty(info->SpellDifficultyID, Caster->GetMapMgr()->iInstanceMode);
		if( SpellDiffEntry != NULL )
			m_spellInfo = SpellDiffEntry;
		else
			m_spellInfo = info;
	}else
		m_spellInfo = info;

	m_spellInfo_override = NULL;
	m_caster = Caster;
	duelSpell = false;
	m_DelayStep = 0;

	switch(Caster->GetTypeId())
	{
		case TYPEID_PLAYER:
			{
				g_caster = NULL;
				i_caster = NULL;
				u_caster = TO< Unit* >(Caster);
				p_caster = TO< Player* >(Caster);
				if(p_caster->GetDuelState() == DUEL_STATE_STARTED)
					duelSpell = true;

#ifdef GM_Z_DEBUG_DIRECTLY
				// cebernic added it
				if(p_caster->GetSession() && p_caster->GetSession()->CanUseCommand('z')  && p_caster->IsInWorld())
					sChatHandler.BlueSystemMessage(p_caster->GetSession(), "[%sSystem%s] |rSpell::Spell: %s ID:%u,Category%u,CD:%u,DisType%u,Field4:%u,etA0=%u,etA1=%u,etA2=%u,etB0=%u,etB1=%u,etB2=%u", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE,
					                               info->Id, info->Category, info->RecoveryTime, info->DispelType, info->castUI, info->EffectImplicitTargetA[0], info->EffectImplicitTargetA[1], info->EffectImplicitTargetA[2], info->EffectImplicitTargetB[0], info->EffectImplicitTargetB[1], info->EffectImplicitTargetB[2]);
#endif

			}
			break;

		case TYPEID_UNIT:
			{
				g_caster = NULL;
				i_caster = NULL;
				p_caster = NULL;
				u_caster = TO< Unit* >(Caster);
				if(u_caster->IsPet() && TO< Pet* >(u_caster)->GetPetOwner() != NULL && TO< Pet* >(u_caster)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED)
					duelSpell = true;
			}
			break;

		case TYPEID_ITEM:
		case TYPEID_CONTAINER:
			{
				g_caster = NULL;
				u_caster = NULL;
				p_caster = NULL;
				i_caster = TO< Item* >(Caster);
				if(i_caster->GetOwner() && i_caster->GetOwner()->GetDuelState() == DUEL_STATE_STARTED)
					duelSpell = true;
			}
			break;

		case TYPEID_GAMEOBJECT:
			{
				u_caster = NULL;
				p_caster = NULL;
				i_caster = NULL;
				g_caster = TO< GameObject* >(Caster);
			}
			break;

		default:
			LOG_DEBUG("[DEBUG][SPELL] Incompatible object type, please report this to the dev's");
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
	add_damage = 0;
	m_Delayed = false;
	pSpellId = 0;
	m_cancelled = false;
	ProcedOnSpell = 0;
	forced_basepoints[0] = forced_basepoints[1] = forced_basepoints[2] = 0;
	extra_cast_number = 0;
	m_reflectedParent = NULL;
	m_isCasting = false;
	m_glyphslot = 0;
	m_charges = info->procCharges;

	UniqueTargets.clear();
	ModeratedTargets.clear();
	for(uint32 i = 0; i < 3; ++i)
	{
		m_targetUnits[i].clear();
	}

	//create rune avail snapshot
	if(p_caster && p_caster->IsDeathKnight())
		m_rune_avail_before = TO_DK(p_caster)->GetRuneFlags();

	m_target_constraint = objmgr.GetSpellTargetConstraintForSpell(info->Id);
	
	m_missilePitch = 0.0f;
	m_missileTravelTime = 0;
}

Spell::~Spell()
{
	// If this spell deals with rune power, send spell_go to update client
	// For instance, when Dk cast Empower Rune Weapon, if we don't send spell_go, the client won't update
	if(GetProto()->RuneCostID && GetProto()->powerType == POWER_TYPE_RUNES)
		SendSpellGo();

	m_caster->m_pendingSpells.erase(this);

	///////////////////////////// This is from the virtual_destructor shit ///////////////
	if(m_caster->GetCurrentSpell() == this)
		m_caster->SetCurrentSpell(NULL);



	if(m_spellInfo_override != NULL)
		delete[] m_spellInfo_override;
	////////////////////////////////////////////////////////////////////////////////////////


	for(uint32 i = 0; i < 3; ++i)
	{
		m_targetUnits[i].clear();
	}

	std::map<uint64, Aura*>::iterator itr;
	for(itr = m_pendingAuras.begin(); itr != m_pendingAuras.end(); ++itr)
	{
		if(itr->second != NULL)
			delete itr->second;
	}
}

//i might forget conditions here. Feel free to add them
bool Spell::IsStealthSpell()
{
	//check if aura name is some stealth aura
	if(GetProto()->EffectApplyAuraName[0] == SPELL_AURA_MOD_STEALTH || GetProto()->EffectApplyAuraName[1] == SPELL_AURA_MOD_STEALTH || GetProto()->EffectApplyAuraName[2] == SPELL_AURA_MOD_STEALTH)
		return true;
	return false;
}

//i might forget conditions here. Feel free to add them
bool Spell::IsInvisibilitySpell()
{
	//check if aura name is some invisibility aura
	if(GetProto()->EffectApplyAuraName[0] == SPELL_AURA_MOD_INVISIBILITY || GetProto()->EffectApplyAuraName[1] == SPELL_AURA_MOD_INVISIBILITY || GetProto()->EffectApplyAuraName[2] == SPELL_AURA_MOD_INVISIBILITY)
		return true;
	return false;
}

void Spell::FillSpecifiedTargetsInArea(float srcx, float srcy, float srcz, uint32 ind, uint32 specification)
{
	FillSpecifiedTargetsInArea(ind, srcx, srcy, srcz, GetRadius(ind), specification);
}

// for the moment we do invisible targets
void Spell::FillSpecifiedTargetsInArea(uint32 i, float srcx, float srcy, float srcz, float range, uint32 specification)
{
	TargetsList* tmpMap = &m_targetUnits[i];
	//IsStealth()
	float r = range * range;
	uint8 did_hit_result;

	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++)
	{
		// don't add objects that are not units and that are dead
		if(!((*itr)->IsUnit()) || ! TO< Unit* >(*itr)->isAlive())
			continue;

		if(GetProto()->TargetCreatureType)
		{
			if(!(*itr)->IsCreature())
				continue;
			CreatureInfo* inf = TO_CREATURE(*itr)->GetCreatureInfo();
			if(!(1 << (inf->Type - 1) & GetProto()->TargetCreatureType))
				continue;
		}

		if(IsInrange(srcx, srcy, srcz, (*itr), r))
		{
			if(u_caster != NULL)
			{
				if(isAttackable(u_caster, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
				{
					did_hit_result = DidHit(i, TO< Unit* >(*itr));
					if(did_hit_result != SPELL_DID_HIT_SUCCESS)
						ModeratedTargets.push_back(SpellTargetMod((*itr)->GetGUID(), did_hit_result));
					else
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}

			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if(isAttackable(g_caster->m_summoner, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if(GetProto()->MaxTargets)
			{
				if(GetProto()->MaxTargets >= tmpMap->size())
				{
					return;
				}
			}
		}
	}
}
void Spell::FillAllTargetsInArea(LocationVector & location, uint32 ind)
{
	FillAllTargetsInArea(ind, location.x, location.y, location.z, GetRadius(ind));
}

void Spell::FillAllTargetsInArea(float srcx, float srcy, float srcz, uint32 ind)
{
	FillAllTargetsInArea(ind, srcx, srcy, srcz, GetRadius(ind));
}

/// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllTargetsInArea(uint32 i, float srcx, float srcy, float srcz, float range)
{
	TargetsList* tmpMap = &m_targetUnits[i];
	float r = range * range;
	uint8 did_hit_result;
	std::set<Object*>::iterator itr, itr2;

	for(itr2 = m_caster->GetInRangeSetBegin(); itr2 != m_caster->GetInRangeSetEnd();)
	{
		itr = itr2;
		//maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
		itr2++;
		if(!((*itr)->IsUnit()) || ! TO< Unit* >(*itr)->isAlive())      //|| ( TO< Creature* >( *itr )->IsTotem() && !TO< Unit* >( *itr )->IsPlayer() ) ) why shouldn't we fill totems?
			continue;

		if(p_caster && (*itr)->IsPlayer() && p_caster->GetGroup() && TO< Player* >(*itr)->GetGroup() && TO< Player* >(*itr)->GetGroup() == p_caster->GetGroup())      //Don't attack party members!!
		{
			//Dueling - AoE's should still hit the target party member if you're dueling with him
			if(!p_caster->DuelingWith || p_caster->DuelingWith != TO< Player* >(*itr))
				continue;
		}
		if(GetProto()->TargetCreatureType)
		{
			if(!(*itr)->IsCreature())
				continue;
			CreatureInfo* inf = TO_CREATURE(*itr)->GetCreatureInfo();
			if(!(1 << (inf->Type - 1) & GetProto()->TargetCreatureType))
				continue;
		}
		if(IsInrange(srcx, srcy, srcz, (*itr), r))
		{
			if(sWorld.Collision)
			{
				if(m_caster->GetMapId() == (*itr)->GetMapId() && !CollideInterface.CheckLOS(m_caster->GetMapId(), m_caster->GetPositionNC(), (*itr)->GetPositionNC()))
					continue;
			}

			if(u_caster != NULL)
			{
				if(isAttackable(u_caster, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
				{
					did_hit_result = DidHit(i, TO< Unit* >(*itr));
					if(did_hit_result == SPELL_DID_HIT_SUCCESS)
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
					else
						ModeratedTargets.push_back(SpellTargetMod((*itr)->GetGUID(), did_hit_result));
				}
			}
			else //cast from GO
			{
				if(g_caster != NULL && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner != NULL)
				{
					//trap, check not to attack owner and friendly
					if(isAttackable(g_caster->m_summoner, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if(GetProto()->MaxTargets)
				if(GetProto()->MaxTargets == tmpMap->size())
				{
					return;
				}
		}
	}
}

// We fill all the targets in the area, including the stealthed ones
void Spell::FillAllFriendlyInArea(uint32 i, float srcx, float srcy, float srcz, float range)
{
	TargetsList* tmpMap = &m_targetUnits[i];
	float r = range * range;
	uint8 did_hit_result;
	std::set<Object*>::iterator itr, itr2;

	for(itr2 = m_caster->GetInRangeSetBegin(); itr2 != m_caster->GetInRangeSetEnd();)
	{
		itr = itr2;
		itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
		if(!((*itr)->IsUnit()) || !TO< Unit* >(*itr)->isAlive())
			continue;

		if(GetProto()->TargetCreatureType)
		{
			if(!(*itr)->IsCreature())
				continue;
			CreatureInfo* inf = TO_CREATURE(*itr)->GetCreatureInfo();
			if(!(1 << (inf->Type - 1) & GetProto()->TargetCreatureType))
				continue;
		}

		if(IsInrange(srcx, srcy, srcz, (*itr), r))
		{
			if(sWorld.Collision)
			{
				if(m_caster->GetMapId() == (*itr)->GetMapId() && !CollideInterface.CheckLOS(m_caster->GetMapId(), m_caster->GetPositionNC(), (*itr)->GetPositionNC()))
					continue;
			}

			if(u_caster != NULL)
			{
				if(isFriendly(u_caster, TO< Unit* >(*itr)))
				{
					did_hit_result = DidHit(i, TO< Unit* >(*itr));
					if(did_hit_result == SPELL_DID_HIT_SUCCESS)
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
					else
						ModeratedTargets.push_back(SpellTargetMod((*itr)->GetGUID(), did_hit_result));
				}
			}
			else //cast from GO
			{
				if(g_caster != NULL && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner != NULL)
				{
					//trap, check not to attack owner and friendly
					if(isFriendly(g_caster->m_summoner, TO< Unit* >(*itr)))
						SafeAddTarget(tmpMap, (*itr)->GetGUID());
				}
				else
					SafeAddTarget(tmpMap, (*itr)->GetGUID());
			}
			if(GetProto()->MaxTargets)
				if(GetProto()->MaxTargets == tmpMap->size())
				{
					return;
				}
		}
	}
}

uint64 Spell::GetSinglePossibleEnemy(uint32 i, float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = GetProto()->base_range_or_radius_sqr;
		if(GetProto()->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius, &r, GetProto()->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius, &r, GetProto()->SpellGroupType);
		}
	}
	float srcx = m_caster->GetPositionX(), srcy = m_caster->GetPositionY(), srcz = m_caster->GetPositionZ();

	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++)
	{
		if(!((*itr)->IsUnit()) || !TO< Unit* >(*itr)->isAlive())
			continue;

		if(GetProto()->TargetCreatureType)
		{
			if(!(*itr)->IsCreature())
				continue;
			CreatureInfo* inf = TO_CREATURE(*itr)->GetCreatureInfo();
			if(!(1 << (inf->Type - 1) & GetProto()->TargetCreatureType))
				continue;
		}
		if(IsInrange(srcx, srcy, srcz, (*itr), r))
		{
			if(u_caster != NULL)
			{
				if(isAttackable(u_caster, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && DidHit(i, TO_UNIT(*itr)) == SPELL_DID_HIT_SUCCESS)
				{
					return (*itr)->GetGUID();
				}
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if(isAttackable(g_caster->m_summoner, *itr, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
					{
						return (*itr)->GetGUID();
					}
				}
			}
		}
	}
	return 0;
}

uint64 Spell::GetSinglePossibleFriend(uint32 i, float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = GetProto()->base_range_or_radius_sqr;
		if(GetProto()->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius, &r, GetProto()->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius, &r, GetProto()->SpellGroupType);
		}
	}
	float srcx = m_caster->GetPositionX(), srcy = m_caster->GetPositionY(), srcz = m_caster->GetPositionZ();

	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++)
	{
		if(!((*itr)->IsUnit()) || !TO< Unit* >(*itr)->isAlive())
			continue;
		if(GetProto()->TargetCreatureType)
		{
			if(!(*itr)->IsCreature())
				continue;
			CreatureInfo* inf = TO_CREATURE(*itr)->GetCreatureInfo();
			if(!(1 << (inf->Type - 1) & GetProto()->TargetCreatureType))
				continue;
		}
		if(IsInrange(srcx, srcy, srcz, (*itr), r))
		{
			if(u_caster != NULL)
			{
				if(isFriendly(u_caster, TO< Unit* >(*itr)) && DidHit(i, TO_UNIT(*itr)) == SPELL_DID_HIT_SUCCESS)
				{
					return (*itr)->GetGUID();
				}
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if(isFriendly(g_caster->m_summoner, TO< Unit* >(*itr)))
					{
						return (*itr)->GetGUID();
					}
				}
			}
		}
	}
	return 0;
}

uint8 Spell::DidHit(uint32 effindex, Unit* target)
{
	//note resistchance is vise versa, is full hit chance
	Unit* u_victim = target;
	if(u_victim == NULL)
		return SPELL_DID_HIT_MISS;

	Player* p_victim = target->IsPlayer() ? TO< Player* >(target) : NULL;

	float baseresist[3] = { 4.0f, 5.0f, 6.0f };
	int32 lvldiff;
	float resistchance ;


	/************************************************************************/
	/* Can't resist non-unit                                                */
	/************************************************************************/
	if(u_caster == NULL)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Can't reduce your own spells                                         */
	/************************************************************************/
	if(u_caster == u_victim)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Check if the unit is evading                                         */
	/************************************************************************/
	if(u_victim->IsCreature() && u_victim->GetAIInterface()->getAIState() == STATE_EVADE)
		return SPELL_DID_HIT_EVADE;

	/************************************************************************/ 
	/* Check if the player target is able to deflect spells					*/ 
	/* Currently (3.3.5a) there is only spell doing that: Deterrence		*/ 
	/************************************************************************/ 
	if( p_victim && p_victim->HasAuraWithName(SPELL_AURA_DEFLECT_SPELLS) )
	{
		return SPELL_DID_HIT_DEFLECT;
	}

	/************************************************************************/
	/* Check if the target is immune to this spell school                   */
	/* Unless the spell would actually dispel invulnerabilities             */
	/************************************************************************/
	int dispelMechanic = GetProto()->Effect[0] == SPELL_EFFECT_DISPEL_MECHANIC && GetProto()->EffectMiscValue[0] == MECHANIC_INVULNERABLE;
	if(u_victim->SchoolImmunityList[ GetProto()->School ] && !dispelMechanic)
		return SPELL_DID_HIT_IMMUNE;

	/* Check if player target has god mode */
	if(p_victim && p_victim->GodModeCheat)
	{
		return SPELL_DID_HIT_IMMUNE;
	}

	/*************************************************************************/
	/* Check if the target is immune to this mechanic                        */
	/*************************************************************************/
	if(m_spellInfo->MechanicsType < MECHANIC_END && u_victim->MechanicsDispels[ m_spellInfo->MechanicsType ])

	{
		// Immune - IF, and ONLY IF, there is no damage component!
		bool no_damage_component = true;
		for(int x = 0 ; x <= 2 ; x ++)
		{
			if(GetProto()->Effect[x] == SPELL_EFFECT_SCHOOL_DAMAGE
			        || GetProto()->Effect[x] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
			        || GetProto()->Effect[x] == SPELL_EFFECT_WEAPON_DAMAGE
			        || GetProto()->Effect[x] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
			        || GetProto()->Effect[x] == SPELL_EFFECT_DUMMY
			        || (GetProto()->Effect[x] == SPELL_EFFECT_APPLY_AURA &&
			            (GetProto()->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_DAMAGE
			            ))
			  )
			{
				no_damage_component = false;
				break;
			}
		}
		if(no_damage_component)
			return SPELL_DID_HIT_IMMUNE; // Moved here from Spell::CanCast
	}

	/************************************************************************/
	/* Check if the target has a % resistance to this mechanic              */
	/************************************************************************/
	if(GetProto()->MechanicsType < MECHANIC_END)
	{
		float res = u_victim->MechanicsResistancesPCT[ m_spellInfo->MechanicsType ];
		if(Rand(res))
			return SPELL_DID_HIT_RESIST;
	}

	/************************************************************************/
	/* Check if the spell is a melee attack and if it was missed/parried    */
	/************************************************************************/
	uint32 melee_test_result;
	if(GetProto()->is_melee_spell || GetProto()->is_ranged_spell)
	{
		uint32 _type;
		if(GetType() == SPELL_DMG_TYPE_RANGED)
			_type = RANGED;
		else
		{
			if(hasAttributeExC(FLAGS4_TYPE_OFFHAND))
				_type = OFFHAND;
			else
				_type = MELEE;
		}

		melee_test_result = u_caster->GetSpellDidHitResult(u_victim, _type, GetProto());
		if(melee_test_result != SPELL_DID_HIT_SUCCESS)
			return (uint8)melee_test_result;
	}

	/************************************************************************/
	/* Check if the spell is resisted.                                      */
	/************************************************************************/
	if(GetProto()->School == SCHOOL_NORMAL  && GetProto()->MechanicsType == MECHANIC_NONE)
		return SPELL_DID_HIT_SUCCESS;

	bool pvp = (p_caster && p_victim);

	if(pvp)
		lvldiff = p_victim->getLevel() - p_caster->getLevel();
	else
		lvldiff = u_victim->getLevel() - u_caster->getLevel();
	if(lvldiff < 0)
	{
		resistchance = baseresist[0] + lvldiff;
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
				resistchance = baseresist[2] + (((float)lvldiff - 2.0f) * 7.0f);
			else
				resistchance = baseresist[2] + (((float)lvldiff - 2.0f) * 11.0f);
		}
	}
	// TODO: SB@L - This mechanic resist chance is handled twice, once several lines above, then as part of resistchance here
	//check mechanical resistance
	//i have no idea what is the best pace for this code
	if(GetProto()->MechanicsType < MECHANIC_END)
	{
		resistchance += u_victim->MechanicsResistancesPCT[ GetProto()->MechanicsType ];
	}
	//rating bonus
	if(p_caster != NULL)
	{
		resistchance -= p_caster->CalcRating(PLAYER_RATING_MODIFIER_SPELL_HIT);
		resistchance -= p_caster->GetHitFromSpell();
	}

	// school hit resistance: check all schools and take the minimal
	if(p_victim != NULL && GetProto()->SchoolMask > 0)
	{
		int32 min = 100;
		for(uint8 i = 0; i < SCHOOL_COUNT; i++)
		{
			if(GetProto()->SchoolMask & (1 << i) && min > p_victim->m_resist_hit_spell[ i ])
				min = p_victim->m_resist_hit_spell[ i ];
		}
		resistchance += min;
	}

	if(GetProto()->Effect[effindex] == SPELL_EFFECT_DISPEL && GetProto()->SpellGroupType)
	{
		SM_FFValue(u_victim->SM_FRezist_dispell, &resistchance, GetProto()->SpellGroupType);
		SM_PFValue(u_victim->SM_PRezist_dispell, &resistchance, GetProto()->SpellGroupType);
	}

	if(GetProto()->SpellGroupType)
	{
		float hitchance = 0;
		SM_FFValue(u_caster->SM_FHitchance, &hitchance, GetProto()->SpellGroupType);
		resistchance -= hitchance;
	}

	if(hasAttribute(ATTRIBUTES_IGNORE_INVULNERABILITY))
		resistchance = 0.0f;

	if(resistchance >= 100.0f)
		return SPELL_DID_HIT_RESIST;
	else
	{
		uint8 res;
		if(resistchance <= 1.0) //resist chance >=1
			res = (Rand(1.0f) ? uint8(SPELL_DID_HIT_RESIST) : uint8(SPELL_DID_HIT_SUCCESS));
		else
			res = (Rand(resistchance) ? uint8(SPELL_DID_HIT_RESIST) : uint8(SPELL_DID_HIT_SUCCESS));

		if(res == SPELL_DID_HIT_SUCCESS)  // proc handling. mb should be moved outside this function
		{
//			u_caster->HandleProc(PROC_ON_SPELL_LAND,target,GetProto());
		}

		return res;
	}
}
uint8 Spell::prepare(SpellCastTargets* targets)
{
	if(!m_caster->IsInWorld())
	{
		LOG_DEBUG("Object " I64FMT " is casting Spell ID %u while not in World", m_caster->GetGUID(), GetProto()->Id);
		DecRef();
		return SPELL_FAILED_DONT_REPORT;
	}

	uint8 ccr;

	// In case spell got cast from a script check fear/wander states
	if(!p_caster && u_caster && u_caster->GetAIInterface())
	{
		AIInterface* ai = u_caster->GetAIInterface();
		if(ai->getAIState() == STATE_FEAR || ai->getAIState() == STATE_WANDER)
		{
			DecRef();
			return SPELL_FAILED_NOT_READY;
		}
	}

	chaindamage = 0;
	m_targets = *targets;

	if(!m_triggeredSpell && p_caster != NULL && p_caster->CastTimeCheat)
		m_castTime = 0;
	else
	{
		m_castTime = GetCastTime(dbcSpellCastTime.LookupEntry(GetProto()->CastingTimeIndex));

		if(m_castTime && GetProto()->SpellGroupType && u_caster != NULL)
		{
			SM_FIValue(u_caster->SM_FCastTime, (int32*)&m_castTime, GetProto()->SpellGroupType);
			SM_PIValue(u_caster->SM_PCastTime, (int32*)&m_castTime, GetProto()->SpellGroupType);
		}

		// handle MOD_CAST_TIME
		if(u_caster != NULL && m_castTime)
		{
			m_castTime = float2int32(m_castTime * u_caster->GetCastSpeedMod());
		}
	}

	if(p_caster != NULL)
	{
		// HookInterface events
		if(!sHookInterface.OnCastSpell(p_caster, GetProto(), this))
		{
			DecRef();
			return SPELL_FAILED_UNKNOWN;
		}

		if(p_caster->cannibalize)
		{
			sEventMgr.RemoveEvents(p_caster, EVENT_CANNIBALIZE);
			p_caster->SetEmoteState(0);
			p_caster->cannibalize = false;
		}
	}

	//let us make sure cast_time is within decent range
	//this is a hax but there is no spell that has more then 10 minutes cast time

	if(m_castTime < 0)
		m_castTime = 0;
	else if(m_castTime > 60 * 10 * 1000)
		m_castTime = 60 * 10 * 1000; //we should limit cast time to 10 minutes right ?

	m_timer = m_castTime;

	m_magnetTarget = 0;

	//if( p_caster != NULL )
	//   m_castTime -= 100;	  // session update time


	m_spellState = SPELL_STATE_PREPARING;

	if(objmgr.IsSpellDisabled(GetProto()->Id))//if it's disabled it will not be casted, even if it's triggered.
		cancastresult = uint8(m_triggeredSpell ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_SPELL_UNAVAILABLE);
	else if(m_triggeredSpell || ProcedOnSpell != NULL)
		cancastresult = SPELL_CANCAST_OK;
	else
		cancastresult = CanCast(false);

	//sLog.outString( "CanCast result: %u. Refer to SpellFailure.h to work out why." , cancastresult );

	ccr = cancastresult;
	if(cancastresult != SPELL_CANCAST_OK)
	{
		SendCastResult(cancastresult);

		if(m_triggeredByAura)
		{
			SendChannelUpdate(0);
			if(u_caster != NULL)
				u_caster->RemoveAura(m_triggeredByAura);
		}
		else
		{
			// HACK, real problem is the way spells are handled
			// when a spell is channeling and a new spell is cast
			// that is a channeling spell, but not triggered by a aura
			// the channel bar/spell is bugged
			if(u_caster && u_caster->GetChannelSpellTargetGUID() != 0 && u_caster->GetCurrentSpell())
			{
				u_caster->GetCurrentSpell()->cancel();
				SendChannelUpdate(0);
				cancel();
				return ccr;
			}
		}
		finish(false);
		return ccr;
	}
	else
	{
		if(p_caster != NULL && p_caster->IsStealth() && m_spellInfo && !hasAttributeEx(ATTRIBUTESEX_NOT_BREAK_STEALTH) && m_spellInfo->Id != 1)   // <-- baaaad, baaad hackfix - for some reason some spells were triggering Spell ID #1 and stuffing up the spell system.
		{
			/* talents procing - don't remove stealth either */
			if(!hasAttribute(ATTRIBUTES_PASSIVE) &&
			        !(pSpellId && dbcSpell.LookupEntry(pSpellId)->Attributes & ATTRIBUTES_PASSIVE))
			{
				p_caster->RemoveAura(p_caster->m_stealth);
				p_caster->m_stealth = 0;
			}
		}

		SendSpellStart();

		// start cooldown handler
		if(p_caster != NULL && !p_caster->CastTimeCheat && !m_triggeredSpell)
		{
			AddStartCooldown();
		}

		if(i_caster == NULL)
		{
			if(p_caster != NULL && m_timer > 0 && !m_triggeredSpell)
				p_caster->delayAttackTimer(m_timer + 1000);
			//p_caster->setAttackTimer(m_timer + 1000, false);
		}

		// aura state removal
		if(GetProto()->CasterAuraState && GetProto()->CasterAuraState != AURASTATE_FLAG_JUDGEMENT)
			u_caster->RemoveFlag(UNIT_FIELD_AURASTATE, GetProto()->CasterAuraState);
	}

	//instant cast(or triggered) and not channeling
	if(u_caster != NULL && (m_castTime > 0 || GetProto()->ChannelInterruptFlags) && !m_triggeredSpell)
	{
		m_castPositionX = m_caster->GetPositionX();
		m_castPositionY = m_caster->GetPositionY();
		m_castPositionZ = m_caster->GetPositionZ();

		u_caster->castSpell(this);
	}
	else
		cast(false);

	return ccr;
}

void Spell::cancel()
{
	if(m_spellState == SPELL_STATE_FINISHED)
		return;

	SendInterrupted(0);
	SendCastResult(SPELL_FAILED_INTERRUPTED);

	if(m_spellState == SPELL_STATE_CASTING)
	{
		if(u_caster != NULL)
			u_caster->RemoveAura(GetProto()->Id);

		if(m_timer > 0 || m_Delayed)
		{
			if(p_caster && p_caster->IsInWorld())
			{
				Unit* pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetChannelSpellTargetGUID());
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());

				if(pTarget)
				{
					pTarget->RemoveAura(GetProto()->Id, m_caster->GetGUID());
				}
				if(m_AreaAura)//remove of blizz and shit like this
				{
					uint64 guid = p_caster->GetChannelSpellTargetGUID();

					DynamicObject* dynObj = m_caster->GetMapMgr()->GetDynamicObject(Arcemu::Util::GUID_LOPART(guid));
					if(dynObj)
						dynObj->Remove();
				}

				if(p_caster->GetSummonedObject())
				{
					if(p_caster->GetSummonedObject()->IsInWorld())
						p_caster->GetSummonedObject()->RemoveFromWorld(true);
					// for now..
					ARCEMU_ASSERT(p_caster->GetSummonedObject()->IsGameObject());
					delete p_caster->GetSummonedObject();
					p_caster->SetSummonedObject(NULL);
				}

				if(m_timer > 0)
				{
					p_caster->delayAttackTimer(-m_timer);
					RemoveItems();
				}
//				p_caster->setAttackTimer(1000, false);
			}
		}
	}

	SendChannelUpdate(0);

	//m_spellState = SPELL_STATE_FINISHED;

	// prevent memory corruption. free it up later.
	// if this is true it means we are currently in the cast() function somewhere else down the stack
	// (recursive spells) and we don't wanna have this class deleted when we return to it.
	// at the end of cast() it will get freed anyway.
	if(!m_isCasting)
		finish(false);
}

void Spell::AddCooldown()
{
	if(p_caster != NULL)
		p_caster->Cooldown_Add(GetProto(), i_caster);
}

void Spell::AddStartCooldown()
{
	if(p_caster != NULL)
		p_caster->Cooldown_AddStart(GetProto());
}

void Spell::cast(bool check)
{
	if(DuelSpellNoMoreValid())
	{
		// Can't cast that!
		SendInterrupted(SPELL_FAILED_TARGET_FRIENDLY);
		finish(false);
		return;
	}

	LOG_DEBUG("Spell::cast %u, Unit: %u", GetProto()->Id, m_caster->GetLowGUID());

	if(objmgr.IsSpellDisabled(GetProto()->Id))//if it's disabled it will not be casted, even if it's triggered.
		cancastresult = uint8(m_triggeredSpell ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_SPELL_UNAVAILABLE);
	else if(check)
		cancastresult = CanCast(true);
	else
		cancastresult = SPELL_CANCAST_OK;

	if(cancastresult == SPELL_CANCAST_OK)
	{
		if(hasAttribute(ATTRIBUTE_ON_NEXT_ATTACK))
		{
			if(!m_triggeredSpell)
			{
				// on next attack - we don't take the mana till it actually attacks.
				if(!HasPower())
				{
					SendInterrupted(SPELL_FAILED_NO_POWER);
					SendCastResult(SPELL_FAILED_NO_POWER);
					finish(false);
					return;
				}
			}
			else
			{
				// this is the actual spell cast
				if(!TakePower())   // shouldn't happen
				{
					SendInterrupted(SPELL_FAILED_NO_POWER);
					SendCastResult(SPELL_FAILED_NO_POWER);
					finish(false);
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
					//LOG_DEBUG("Spell::Not Enough Mana");
					SendInterrupted(SPELL_FAILED_NO_POWER);
					SendCastResult(SPELL_FAILED_NO_POWER);
					finish(false);
					return;
				}
			}
		}

		for(uint32 i = 0; i < 3; i++)
		{
			uint32 TargetType = 0;
			TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i);

			//never get info from B if it is 0 :P
			if(m_spellInfo->EffectImplicitTargetB[i] != 0)
				TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i);

			if(TargetType & SPELL_TARGET_AREA_CURTARGET)
			{
				//this just forces dest as the targets location :P
				Object* target = m_caster->GetMapMgr()->_GetObject(m_targets.m_unitTarget);

				if(target != NULL)
				{
					m_targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
					m_targets.m_destX = target->GetPositionX();
					m_targets.m_destY = target->GetPositionY();
					m_targets.m_destZ = target->GetPositionZ();
				}
			}

			if(GetProto()->Effect[i] && GetProto()->Effect[i] != SPELL_EFFECT_PERSISTENT_AREA_AURA)
				FillTargetMap(i);
		}

		if(m_magnetTarget)
		{
			// Spell was redirected
			// Grounding Totem gets destroyed after redirecting 1 spell
			Unit* MagnetTarget = m_caster->GetMapMgr()->GetUnit(m_magnetTarget);
			m_magnetTarget = 0;
			if(MagnetTarget && MagnetTarget->IsCreature())
			{
				Creature* MagnetCreature = TO< Creature* >(MagnetTarget);
				if(MagnetCreature->IsTotem())
					MagnetCreature->Despawn(1, 0);
			}
		}

		SendCastResult(cancastresult);
		if(cancastresult != SPELL_CANCAST_OK)
		{
			finish(false);
			return;
		}

		m_isCasting = true;

		//sLog.outString( "CanCastResult: %u" , cancastresult );
		if(!m_triggeredSpell)
			AddCooldown();

		if(p_caster)
		{
			if(GetProto()->NameHash == SPELL_HASH_SLAM)
			{
				/* slam - reset attack timer */
				p_caster->setAttackTimer(0, true);
				p_caster->setAttackTimer(0, false);
			}
			else if(m_spellInfo->NameHash == SPELL_HASH_VICTORY_RUSH)
			{
				p_caster->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR);
			}

			if(GetProto()->NameHash == SPELL_HASH_HOLY_LIGHT || GetProto()->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			{
				p_caster->RemoveAura(53672);
				p_caster->RemoveAura(54149);
			}

			if(p_caster->HasAurasWithNameHash(SPELL_HASH_ARCANE_POTENCY) && GetProto()->c_is_flags == SPELL_FLAG_IS_DAMAGING)
			{
				p_caster->RemoveAura(57529);
				p_caster->RemoveAura(57531);
			}

			if(p_caster->IsStealth() && !hasAttributeEx(ATTRIBUTESEX_NOT_BREAK_STEALTH)
			        && GetProto()->Id != 1)  //check spells that get trigger spell 1 after spell loading
			{
				/* talents procing - don't remove stealth either */
				if(!hasAttribute(ATTRIBUTES_PASSIVE) && !(pSpellId && dbcSpell.LookupEntry(pSpellId)->Attributes & ATTRIBUTES_PASSIVE))
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
					case 1784:		// Stealth rank 1
					case 1785:		// Stealth rank 2
					case 1786:		// Stealth rank 3
					case 1787:		// Stealth rank 4
					case 5215:		// Prowl rank 1
					case 6783:		// Prowl rank 2
					case 9913:		// Prowl rank 3
					case 498:		// Divine protection
					case 5573:		// Unknown spell
					case 642:		// Divine shield
					case 1020:		// Unknown spell
					case 1022:		// Hand of Protection rank 1 (ex blessing of protection)
					case 5599:		// Hand of Protection rank 2 (ex blessing of protection)
					case 10278:		// Hand of Protection rank 3 (ex blessing of protection)
					case 1856:		// Vanish rank 1
					case 1857:		// Vanish rank 2
					case 26889:		// Vanish rank 3
					case 45438:		// Ice block
					case 20580:		// Unknown spell
					case 58984:		// Shadowmeld
					case 17624:		// Petrification-> http://www.wowhead.com/?spell=17624
					case 66:		// Invisibility
						if(p_caster->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
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

		if(!(hasAttribute(ATTRIBUTE_ON_NEXT_ATTACK) && !m_triggeredSpell))  //on next attack
		{
			SendSpellGo();

			//******************** SHOOT SPELLS ***********************
			//* Flags are now 1,4,19,22 (4718610) //0x480012

			if(hasAttributeExC(FLAGS4_PLAYER_RANGED_SPELLS) && m_caster->IsPlayer() && m_caster->IsInWorld())
			{
				// Part of this function contains a hack fix
				// hack fix for shoot spells, should be some other resource for it
				// p_caster->SendSpellCoolDown(GetProto()->Id, GetProto()->RecoveryTime ? GetProto()->RecoveryTime : 2300);
				WorldPacket data(SMSG_SPELL_COOLDOWN, 14);
				data << p_caster->GetNewGUID();
				data << uint8(0); //unk, some flags
				data << GetProto()->Id;
				data << uint32(GetProto()->RecoveryTime ? GetProto()->RecoveryTime : 2300);
				p_caster->GetSession()->SendPacket(&data);
			}
			else
			{
				if(GetProto()->ChannelInterruptFlags != 0 && !m_triggeredSpell)
				{
					/*
					Channeled spells are handled a little differently. The five second rule starts when the spell's channeling starts; i.e. when you pay the mana for it.
					The rule continues for at least five seconds, and longer if the spell is channeled for more than five seconds. For example,
					Mind Flay channels for 3 seconds and interrupts your regeneration for 5 seconds, while Tranquility channels for 10 seconds
					and interrupts your regeneration for the full 10 seconds.
					*/

					uint32 channelDuration = GetDuration();
					if(u_caster != NULL)
						channelDuration = static_cast<uint32>(channelDuration * u_caster->GetCastSpeedMod());
					m_spellState = SPELL_STATE_CASTING;
					SendChannelStart(channelDuration);
					if(p_caster != NULL)
					{
						//Use channel interrupt flags here
						if(m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION || m_targets.m_targetMask == TARGET_FLAG_SOURCE_LOCATION)
							u_caster->SetChannelSpellTargetGUID(p_caster->GetSelection());
						else if(p_caster->GetSelection() == m_caster->GetGUID())
						{
							if(p_caster->GetSummon())
								u_caster->SetChannelSpellTargetGUID(p_caster->GetSummon()->GetGUID());
							else if(m_targets.m_unitTarget)
								u_caster->SetChannelSpellTargetGUID(m_targets.m_unitTarget);
							else
								u_caster->SetChannelSpellTargetGUID(p_caster->GetSelection());
						}
						else
						{
							if(p_caster->GetSelection())
								u_caster->SetChannelSpellTargetGUID(p_caster->GetSelection());
							else if(p_caster->GetSummon())
								u_caster->SetChannelSpellTargetGUID(p_caster->GetSummon()->GetGUID());
							else if(m_targets.m_unitTarget)
								u_caster->SetChannelSpellTargetGUID(m_targets.m_unitTarget);
							else
							{
								m_isCasting = false;
								cancel();
								return;
							}
						}
					}
					if(u_caster && u_caster->GetPowerType() == POWER_TYPE_MANA)
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
			// bool canreflect = false, reflected = false;
			for(int j = 0; j < 3; j++)
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
				for(i = UniqueTargets.begin(); i != UniqueTargets.end(); ++i)
				{
					Unit* Target = m_caster->GetMapMgr()->GetUnit(*i);
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
			if(u_caster && !m_triggeredSpell && !m_triggeredByAura && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH))
			{
				u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);
				u_caster->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_CAST);
			}

			// if the spell is not reflected
			if(!IsReflected())
			{
				for(uint32 x = 0; x < 3; x++)
				{
					// check if we actually have a effect
					if(GetProto()->Effect[x])
					{
						isDuelEffect = isDuelEffect ||  GetProto()->Effect[x] == SPELL_EFFECT_DUEL;

						if(m_targetUnits[x].size() > 0)
						{
							for(i = m_targetUnits[x].begin(); i != m_targetUnits[x].end();)
							{
								i2 = i++;
								HandleCastEffects(*i2, x);
							}
						}
						else
							HandleCastEffects(0, x);
					}
				}

				for(SpellTargetsList::iterator itr = ModeratedTargets.begin(); itr != ModeratedTargets.end(); ++itr)
				{
					HandleModeratedTarget(itr->TargetGuid);
				}

				// spells that proc on spell cast, some talents
				if(p_caster && p_caster->IsInWorld())
				{
					for(i = UniqueTargets.begin(); i != UniqueTargets.end(); ++i)
					{
						Unit* Target = p_caster->GetMapMgr()->GetUnit(*i);

						if(!Target)
							continue; //we already made this check, so why make it again ?

						p_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, Target, GetProto(), m_triggeredSpell);
						Target->HandleProc(PROC_ON_SPELL_LAND_VICTIM, u_caster, GetProto(), m_triggeredSpell);
						p_caster->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)

						Target->RemoveFlag(UNIT_FIELD_AURASTATE, GetProto()->TargetAuraState);
					}
				}
			}

			m_isCasting = false;

			if(m_spellState != SPELL_STATE_CASTING)
			{
				finish();
				return;
			}
		}
		else //this shit has nothing to do with instant, this only means it will be on NEXT melee hit
		{
			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.

			if(u_caster && !m_triggeredSpell && !m_triggeredByAura && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH))
			{
				u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);
				u_caster->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_CAST);
			}

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
			if(u_caster != NULL)
				u_caster->SetOnMeleeSpell(GetProto()->Id, extra_cast_number);

			finish();

			return;
		}

		//if( u_caster != NULL )
		//	u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, GetProto()->Id);
	}
	else
	{
		// cancast failed
		SendCastResult(cancastresult);
		SendInterrupted(cancastresult);
		finish(false);
	}
}

void Spell::AddTime(uint32 type)
{
	if(u_caster != NULL)
	{
		if(GetProto()->InterruptFlags & CAST_INTERRUPT_ON_DAMAGE_TAKEN)
		{
			cancel();
			return;
		}
		if(GetProto()->SpellGroupType)
		{
			float ch = 0;
			SM_FFValue(u_caster->SM_PNonInterrupt, &ch, GetProto()->SpellGroupType);
			if(Rand(ch))
				return;
		}
		if(p_caster != NULL)
		{
			if(Rand(p_caster->SpellDelayResist[type]))
				return;
		}
		if(m_DelayStep == 2)
			return; //spells can only be delayed twice as of 3.0.2
		if(m_spellState == SPELL_STATE_PREPARING)
		{
			// no pushback for some spells
			if((GetProto()->InterruptFlags & CAST_INTERRUPT_PUSHBACK) == 0)
				return;
			int32 delay = 500; //0.5 second pushback
			++m_DelayStep;
			m_timer += delay;
			if(m_timer > m_castTime)
			{
				delay -= (m_timer - m_castTime);
				m_timer = m_castTime;
				if(delay < 0)
					delay = 1;
			}

			WorldPacket data(SMSG_SPELL_DELAYED, 13);
			data << u_caster->GetNewGUID();
			data << uint32(delay);
			u_caster->SendMessageToSet(&data, true);

			if(p_caster == NULL)
			{
				//then it's a Creature
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
		else if(GetProto()->ChannelInterruptFlags != 48140)
		{
			int32 delay = GetDuration() / 4; //0.5 second push back
			++m_DelayStep;
			m_timer -= delay;
			if(m_timer < 0)
				m_timer = 0;
			else if(p_caster != NULL)
				p_caster->delayAttackTimer(-delay);

			m_Delayed = true;
			if(m_timer > 0)
				SendChannelUpdate(m_timer);

		}
	}
}

void Spell::update(uint32 difftime)
{
	// skip cast if we're more than 2/3 of the way through
	// TODO: determine which spells can be cast while moving.
	// Client knows this, so it should be easy once we find the flag.
	// XD, it's already there!
	if((GetProto()->InterruptFlags & CAST_INTERRUPT_ON_MOVEMENT) &&
	        ((m_castTime / 1.5f) > m_timer) &&
//		float(m_castTime)/float(m_timer) >= 2.0f		&&
	        (
	            m_castPositionX != m_caster->GetPositionX() ||
	            m_castPositionY != m_caster->GetPositionY() ||
	            m_castPositionZ != m_caster->GetPositionZ()
	        )
	  )
	{
		if(u_caster != NULL)
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


			}
			break;
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
			}
			break;
	}
}

void Spell::finish(bool successful)
{
	if(m_spellState == SPELL_STATE_FINISHED)
		return;

	m_spellState = SPELL_STATE_FINISHED;

	if(u_caster != NULL)
	{
		CALL_SCRIPT_EVENT(u_caster, OnCastSpell)(GetProto()->Id);

		u_caster->m_canMove = true;
		// mana           channeled                                                     power type is mana                             if spell wasn't cast successfully, don't delay mana regeneration
		if(m_usesMana && (GetProto()->ChannelInterruptFlags == 0 && !m_triggeredSpell) && u_caster->GetPowerType() == POWER_TYPE_MANA && successful)
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
	if(p_caster != NULL)
	{
		if(hasAttribute(ATTRIBUTES_STOP_ATTACK) && p_caster->IsAttacking())
		{
			p_caster->EventAttackStop();
			p_caster->smsg_AttackStop(p_caster->GetSelection());
			p_caster->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
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
			Unit* pTarget = NULL;
			if(p_caster->IsInWorld())
			{
				pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetChannelSpellTargetGUID());
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
			}

			if(pTarget)
			{
				pTarget->RemoveAura(GetProto()->Id, m_caster->GetGUID());
			}
		}

		if(GetProto()->NameHash == SPELL_HASH_LIGHTNING_BOLT || GetProto()->NameHash == SPELL_HASH_CHAIN_LIGHTNING)
		{
			//Maelstrom Weapon
			p_caster->RemoveAllAuras(53817, u_caster->GetGUID());
		}
	}

	if(GetProto()->Effect[0] == SPELL_EFFECT_SUMMON_OBJECT ||
	        GetProto()->Effect[1] == SPELL_EFFECT_SUMMON_OBJECT ||
	        GetProto()->Effect[2] == SPELL_EFFECT_SUMMON_OBJECT)
		if(p_caster != NULL)
			p_caster->SetSummonedObject(NULL);
	/*
	Set cooldown on item
	*/
	if(i_caster && i_caster->GetOwner() && cancastresult == SPELL_CANCAST_OK && !GetSpellFailed())
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
		// cooldown starts after leaving combat
		if(i_caster->GetProto()->Class == ITEM_CLASS_CONSUMABLE && i_caster->GetProto()->SubClass == 1)
		{
			i_caster->GetOwner()->SetLastPotion(i_caster->GetProto()->ItemId);
			if(!i_caster->GetOwner()->CombatStatus.IsInCombat())
				i_caster->GetOwner()->UpdatePotionCooldown();
		}
		else
			i_caster->GetOwner()->Cooldown_AddItem(i_caster->GetProto() , x);
	}

	// cebernic added it
	// moved this from ::prepare()
	// With preparing got ClearCooldownForspell, it makes too early for player client.
	// Now .cheat cooldown works perfectly.
	if(!m_triggeredSpell && p_caster != NULL && p_caster->CooldownCheat)
		p_caster->ClearCooldownForSpell(GetProto()->Id);

	/*
	We set current spell only if this spell has cast time or is channeling spell
	otherwise it's instant spell and we delete it right after completion
	*/
	if(u_caster != NULL)
	{
		if(!m_triggeredSpell && (GetProto()->ChannelInterruptFlags || m_castTime > 0))
			u_caster->SetCurrentSpell(NULL);
	}

	// Send Spell cast info to QuestMgr
	if(successful && p_caster != NULL && p_caster->IsInWorld())
	{
		// Taming quest spells are handled in SpellAuras.cpp, in SpellAuraDummy
		// OnPlayerCast shouldn't be called here for taming-quest spells, in case the tame fails (which is handled in SpellAuras)
		bool isTamingQuestSpell = false;
		uint32 tamingQuestSpellIds[] = { 19688, 19694, 19693, 19674, 19697, 19696, 19687, 19548, 19689, 19692, 19699, 19700, 30099, 30105, 30102, 30646, 30653, 30654, 0 };
		uint32* spellidPtr = tamingQuestSpellIds;
		while(*spellidPtr)   // array ends with 0, so this works
		{
			if(*spellidPtr == m_spellInfo->Id)   // it is a spell for taming beast quest
			{
				isTamingQuestSpell = true;
				break;
			}
			++spellidPtr;
		}
		// Don't call QuestMgr::OnPlayerCast for next-attack spells, either.  It will be called during the actual spell cast.
		if(!(hasAttribute(ATTRIBUTE_ON_NEXT_ATTACK) && !m_triggeredSpell) && !isTamingQuestSpell)
		{
			uint32 numTargets = 0;
			TargetsList::iterator itr = UniqueTargets.begin();
			for(; itr != UniqueTargets.end(); ++itr)
			{
				if(GET_TYPE_FROM_GUID(*itr) == HIGHGUID_TYPE_UNIT)
				{
					++numTargets;
					sQuestMgr.OnPlayerCast(p_caster, GetProto()->Id, *itr);
				}
			}
			if(numTargets == 0)
			{
				uint64 guid = p_caster->GetTargetGUID();
				sQuestMgr.OnPlayerCast(p_caster, GetProto()->Id, guid);
			}
		}
	}

	if(p_caster != NULL)
	{
		if(hadEffect || (cancastresult == SPELL_CANCAST_OK && !GetSpellFailed()))
			RemoveItems();
	}

	DecRef();
}

void Spell::SendCastResult(uint8 result)
{
	uint32 Extra = 0;
	if(result == SPELL_CANCAST_OK) return;

	SetSpellFailed();

	if(!m_caster->IsInWorld()) return;

	Player* plr = p_caster;

	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr) return;

	// for some reason, the result extra is not working for anything, including SPELL_FAILED_REQUIRES_SPELL_FOCUS
	switch(result)
	{
		case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
			Extra = GetProto()->RequiresSpellFocus;
			break;

		case SPELL_FAILED_REQUIRES_AREA:
			if(GetProto()->RequiresAreaId > 0)
			{
				AreaGroup* ag = dbcAreaGroup.LookupEntry(GetProto()->RequiresAreaId);
				uint16 plrarea = plr->GetMapMgr()->GetAreaID(plr->GetPositionX(), plr->GetPositionY());
				for(uint8 i = 0; i < 7; i++)
					if(ag->AreaId[i] != 0 && ag->AreaId[i] != plrarea)
					{
						Extra = ag->AreaId[i];
						break;
					}
			}
			break;
		case SPELL_FAILED_TOTEMS:
			Extra = GetProto()->Totem[1] ? GetProto()->Totem[1] : GetProto()->Totem[0];
			break;

		case SPELL_FAILED_ONLY_SHAPESHIFT:
			Extra = GetProto()->RequiredShapeShift;
			break;
			//case SPELL_FAILED_TOTEM_CATEGORY: seems to be fully client sided.
	}

	plr->SendCastResult(GetProto()->Id, result, extra_cast_number, Extra);
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
	if(!m_caster->IsInWorld() || hasAttribute(ATTRIBUTES_PASSIVE) || m_triggeredSpell)
		return;

	WorldPacket data(150);

	uint32 cast_flags = 2;

	if(GetType() == SPELL_DMG_TYPE_RANGED)
		cast_flags |= 0x20;

	// hacky yeaaaa
	if(GetProto()->Id == 8326)   // death
		cast_flags = 0x0F;

	data.SetOpcode(SMSG_SPELL_START);
	if(i_caster != NULL)
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	else
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();

	data << extra_cast_number;
	data << GetProto()->Id;
	data << cast_flags;
	data << (uint32)m_castTime;

	m_targets.write(data);

	if(GetType() == SPELL_DMG_TYPE_RANGED)
	{
		ItemPrototype* ip = NULL;
		if(GetProto()->Id == SPELL_RANGED_THROW)   // throw
		{
			if(p_caster != NULL)
			{
				Item* itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(itm != NULL)
				{
					ip = itm->GetProto();
					/* Throwing Weapon Patch by Supalosa
					p_caster->GetItemInterface()->RemoveItemAmt(it->GetEntry(),1);
					(Supalosa: Instead of removing one from the stack, remove one from durability)
					We don't need to check if the durability is 0, because you can't cast the Throw spell if the thrown weapon is broken, because it returns "Requires Throwing Weapon" or something.
					*/

					// burlex - added a check here anyway (wpe suckers :P)
					if(itm->GetDurability() > 0)
					{
						itm->SetDurability(itm->GetDurability() - 1);
						if(itm->GetDurability() == 0)
							p_caster->ApplyItemMods(itm, EQUIPMENT_SLOT_RANGED, false, true);
					}
				}
				else
				{
					ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
				}
			}
		}
		else if(hasAttributeExC(FLAGS4_PLAYER_RANGED_SPELLS))
		{
			if(p_caster != NULL)
				ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value(PLAYER_AMMO_ID));
			else
				ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
		}

		if(ip != NULL)
			data << ip->DisplayInfoID << ip->InventoryType;
	}

	data << (uint32)139; //3.0.2 seems to be some small value around 250 for shadow bolt.
	m_caster->SendMessageToSet(&data, true);
}

void Spell::SendSpellGo()
{
	// Fill UniqueTargets
	TargetsList::iterator i, j;
	for(uint32 x = 0; x < 3; x++)
	{
		if(GetProto()->Effect[x])
		{
			bool add = true;
			for(i = m_targetUnits[x].begin(); i != m_targetUnits[x].end(); ++i)
			{
				add = true;
				for(j = UniqueTargets.begin(); j != UniqueTargets.end(); ++j)
				{
					if((*j) == (*i))
					{
						add = false;
						break;
					}
				}
				if(add && (*i) != 0)
					UniqueTargets.push_back((*i));
				//TargetsList::iterator itr = std::unique(m_targetUnits[x].begin(), m_targetUnits[x].end());
				//UniqueTargets.insert(UniqueTargets.begin(),));
				//UniqueTargets.insert(UniqueTargets.begin(), itr);
			}
		}
	}

	// no need to send this on passive spells
	if(!m_caster->IsInWorld() || hasAttribute(ATTRIBUTES_PASSIVE))
		return;

	// Start Spell
	WorldPacket data(200);
	data.SetOpcode(SMSG_SPELL_GO);
	uint32 flags = 0;

	if( m_missileTravelTime != 0 )
		flags |= 0x20000;

	if(GetType() == SPELL_DMG_TYPE_RANGED)
		flags |= SPELL_GO_FLAGS_RANGED; // 0x20 RANGED

	if(i_caster != NULL)
		flags |= SPELL_GO_FLAGS_ITEM_CASTER; // 0x100 ITEM CASTER

	if(ModeratedTargets.size() > 0)
		flags |= SPELL_GO_FLAGS_EXTRA_MESSAGE; // 0x400 TARGET MISSES AND OTHER MESSAGES LIKE "Resist"

	if(p_caster != NULL && GetProto()->powerType != POWER_TYPE_HEALTH)
		flags |= SPELL_GO_FLAGS_POWER_UPDATE;

	//experiments with rune updates
	uint8 cur_have_runes = 0;
	if(p_caster && p_caster->IsDeathKnight())   //send our rune updates ^^
	{
		if(GetProto()->RuneCostID && GetProto()->powerType == POWER_TYPE_RUNES)
			flags |= SPELL_GO_FLAGS_ITEM_CASTER | SPELL_GO_FLAGS_RUNE_UPDATE | SPELL_GO_FLAGS_UNK40000;
		//see what we will have after cast
		cur_have_runes = TO_DK(p_caster)->GetRuneFlags();
		if(cur_have_runes != m_rune_avail_before)
			flags |= SPELL_GO_FLAGS_RUNE_UPDATE | SPELL_GO_FLAGS_UNK40000;
	}

	// hacky..
	if(GetProto()->Id == 8326)   // death
		flags = SPELL_GO_FLAGS_ITEM_CASTER | 0x0D;

	if(i_caster != NULL && u_caster != NULL)   // this is needed for correct cooldown on items
	{
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	}
	else
	{
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();
	}

	data << extra_cast_number; //3.0.2
	data << GetProto()->Id;
	data << flags;
	data << getMSTime();
	data << (uint8)(UniqueTargets.size()); //number of hits
	writeSpellGoTargets(&data);

	if(flags & SPELL_GO_FLAGS_EXTRA_MESSAGE)
	{
		data << (uint8)(ModeratedTargets.size()); //number if misses
		writeSpellMissedTargets(&data);
	}
	else
		data << uint8(0);   //moderated target size is 0 since we did not set the flag

	m_targets.write(data);   // this write is included the target flag

	if(flags & SPELL_GO_FLAGS_POWER_UPDATE)
		data << (uint32) p_caster->GetPower(GetProto()->powerType);

	// er why handle it being null inside if if you can't get into if if its null
	if(GetType() == SPELL_DMG_TYPE_RANGED)
	{
		ItemPrototype* ip = NULL;
		if(GetProto()->Id == SPELL_RANGED_THROW)
		{
			if(p_caster != NULL)
			{
				Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(it != NULL)
					ip = it->GetProto();
			}
			else
				ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
		}
		else
		{
			if(p_caster != NULL)
				ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value(PLAYER_AMMO_ID));
			else // HACK FIX
				ip = ItemPrototypeStorage.LookupEntry(2512);	/*rough arrow*/
		}
		if(ip != NULL)
			data << ip->DisplayInfoID << ip->InventoryType;
		else
			data << uint32(0) << uint32(0);
	}

	//data order depending on flags : 0x800, 0x200000, 0x20000, 0x20, 0x80000, 0x40 (this is not spellgoflag but seems to be from spellentry or packet..)
//.text:00401110                 mov     eax, [ecx+14h] -> them
//.text:00401115                 cmp     eax, [ecx+10h] -> us
	if(flags & SPELL_GO_FLAGS_RUNE_UPDATE)
	{
		data << uint8(m_rune_avail_before);
		data << uint8(cur_have_runes);
		for(uint8 k = 0; k < MAX_RUNES; k++)
		{
			uint8 x = (1 << k);
			if((x & m_rune_avail_before) != (x & cur_have_runes))
				data << uint8(0);   //values of the rune converted into byte. We just think it is 0 but maybe it is not :P
		}
	}



/*
		float dx = targets.m_destX - targets.m_srcX;
		float dy = targets.m_destY - targets.m_srcY;
		if (missilepitch != M_PI / 4 && missilepitch != -M_PI / 4) //lets not divide by 0 lul
			traveltime = (sqrtf(dx * dx + dy * dy) / (cosf(missilepitch) * missilespeed)) * 1000;
*/
	
	if( flags & 0x20000 ){
		data << float( m_missilePitch );
		data << uint32( m_missileTravelTime );
	}


	if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
		data << uint8(0);   //some spells require this ? not sure if it is last byte or before that.

	m_caster->SendMessageToSet(&data, true);

	// spell log execute is still send 2.08
	// as I see with this combination, need to test it more
	//if (flags != 0x120 && GetProto()->Attributes & 16) // not ranged and flag 5
	//SendLogExecute(0,m_targets.m_unitTarget);
}

void Spell::writeSpellGoTargets(WorldPacket* data)
{
	TargetsList::iterator i;
	for(i = UniqueTargets.begin(); i != UniqueTargets.end(); ++i)
	{
//		SendCastSuccess(*i);
		*data << *i;
	}
}

void Spell::writeSpellMissedTargets(WorldPacket* data)
{
	/*
	 * The flags at the end known to us so far are.
	 * 1 = Miss
	 * 2 = Resist
	 * 3 = Dodge // melee only
	 * 4 = Deflect
	 * 5 = Block // melee only
	 * 6 = Evade
	 * 7 = Immune
	 */
	SpellTargetsList::iterator i;
	if(u_caster && u_caster->isAlive())
	{
		for(i = ModeratedTargets.begin(); i != ModeratedTargets.end(); ++i)
		{
			*data << (*i).TargetGuid;       // uint64
			*data << (*i).TargetModType;    // uint8
			///handle proc on resist spell
			Unit* target = u_caster->GetMapMgr()->GetUnit((*i).TargetGuid);
			if(target && target->isAlive())
			{
				u_caster->HandleProc(PROC_ON_RESIST_VICTIM, target, GetProto()/*,damage*/);		/** Damage is uninitialized at this point - burlex */
				target->CombatStatusHandler_ResetPvPTimeout(); // aaa
				u_caster->CombatStatusHandler_ResetPvPTimeout(); // bbb
			}
		}
	}
	else
		for(i = ModeratedTargets.begin(); i != ModeratedTargets.end(); ++i)
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
	data << GetProto()->SpellVisual;
	data << uint32(1);
	if(m_caster->GetGUID() != targetGuid)
		data << targetGuid;
	if(damage)
		data << damage;
	m_caster->SendMessageToSet(&data, true);
}

void Spell::SendInterrupted(uint8 result)
{
	SetSpellFailed();

	if(m_caster == NULL || !m_caster->IsInWorld())
		return;

	WorldPacket data(SMSG_SPELL_FAILURE, 20);

	// send the failure to pet owner if we're a pet
	Player* plr = p_caster;
	if(plr == NULL && m_caster->IsPet())
	{
		TO< Pet* >(m_caster)->SendCastFailed(m_spellInfo->Id, result);
	}
	else
	{
		if(plr == NULL && u_caster != NULL && u_caster->m_redirectSpellPackets != NULL)
			plr = u_caster->m_redirectSpellPackets;

		if(plr != NULL && plr->IsPlayer())
		{
			data << m_caster->GetNewGUID();
			data << uint8(extra_cast_number);
			data << uint32(m_spellInfo->Id);
			data << uint8(result);

			plr->GetSession()->SendPacket(&data);
		}
	}

	data.Initialize(SMSG_SPELL_FAILED_OTHER);

	data << m_caster->GetNewGUID();
	data << uint8(extra_cast_number);
	data << uint32(GetProto()->Id);
	data << uint8(result);

	m_caster->SendMessageToSet(&data, false);
}

void Spell::SendChannelUpdate(uint32 time)
{
	if(time == 0)
	{
		if(u_caster && u_caster->IsInWorld())
		{
			uint64 guid = u_caster->GetChannelSpellTargetGUID();

			DynamicObject* dynObj = u_caster->GetMapMgr()->GetDynamicObject(Arcemu::Util::GUID_LOPART(guid));
			if(dynObj)
				dynObj->Remove();

			if(dynObj == NULL && m_pendingAuras.find(m_caster->GetGUID()) == m_pendingAuras.end())  //no persistant aura or aura on caster
			{
				u_caster->SetChannelSpellTargetGUID(0);
				u_caster->SetChannelSpellId(0);
			}
		}

		if(p_caster != NULL)
		{
			if(m_spellInfo->HasEffect(SPELL_EFFECT_SUMMON) && (p_caster->GetCharmedUnitGUID() != 0))
			{
				Unit* u = p_caster->GetMapMgr()->GetUnit(p_caster->GetCharmedUnitGUID());
				if((u != NULL) && (u->GetCreatedBySpell() == m_spellInfo->Id))
					p_caster->UnPossess();
			}
		}
	}

	if(!p_caster)
		return;

	WorldPacket data(MSG_CHANNEL_UPDATE, 18);
	data << p_caster->GetNewGUID();
	data << time;

	p_caster->SendMessageToSet(&data, true);
}

void Spell::SendChannelStart(uint32 duration)
{
	if(!m_caster->IsGameObject())
	{
		// Send Channel Start
		WorldPacket data(MSG_CHANNEL_START, 22);
		data << m_caster->GetNewGUID();
		data << GetProto()->Id;
		data << duration;
		m_caster->SendMessageToSet(&data, true);
	}

	m_castTime = m_timer = duration;

	if(u_caster != NULL)
		u_caster->SetChannelSpellId(GetProto()->Id);

	/*
	Unit* target = objmgr.GetCreature( TO< Player* >( m_caster )->GetSelection());
	if(!target)
		target = objmgr.GetObject<Player>( TO< Player* >( m_caster )->GetSelection());
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

void Spell::SendTameFailure(uint8 result)
{
	if(p_caster != NULL)
	{
		WorldPacket data(SMSG_PET_TAME_FAILURE, 1);
		data << uint8(result);
		p_caster->GetSession()->SendPacket(&data);
	}
}

bool Spell::HasPower()
{
	int32 powerField;
	if(u_caster != NULL)
		if(u_caster->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER))
			return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

	//Items do not use owner's power
	if(i_caster != NULL)
		return true;

	// Free cast for battle preparation
	if(p_caster && p_caster->HasAura(44521))
		return true;
	if(p_caster && p_caster->HasAura(44535))
		return true;
	if(p_caster && p_caster->HasAura(32727))
		return true;

	switch(GetProto()->powerType)
	{
		case POWER_TYPE_HEALTH:
			{	powerField = UNIT_FIELD_HEALTH;						}
			break;
		case POWER_TYPE_MANA:
			{	powerField = UNIT_FIELD_POWER1;	m_usesMana = true;	}
			break;
		case POWER_TYPE_RAGE:
			{	powerField = UNIT_FIELD_POWER2;						}
			break;
		case POWER_TYPE_FOCUS:
			{	powerField = UNIT_FIELD_POWER3;						}
			break;
		case POWER_TYPE_ENERGY:
			{	powerField = UNIT_FIELD_POWER4;						}
			break;
		case POWER_TYPE_HAPPINESS:
			{	powerField = UNIT_FIELD_POWER5;						}
			break;
		case POWER_TYPE_RUNIC_POWER:
			{	powerField = UNIT_FIELD_POWER7;						}
			break;
		case POWER_TYPE_RUNES:
			{
				if(GetProto()->RuneCostID && p_caster)
				{
					SpellRuneCostEntry* runecost = dbcSpellRuneCost.LookupEntry(GetProto()->RuneCostID);
					DeathKnight* dk = TO_DK(p_caster);
					uint32 credit = dk->HasRunes(RUNE_BLOOD, runecost->bloodRuneCost) +
					                dk->HasRunes(RUNE_FROST, runecost->frostRuneCost) +
					                dk->HasRunes(RUNE_UNHOLY, runecost->unholyRuneCost);
					if(credit > 0 && dk->HasRunes(RUNE_DEATH, credit) > 0)
						return false;
				}
				return true;
			}
		default:
			{
				LOG_DEBUG("unknown power type");
				// we shouldn't be here to return
				return false;
			}
			break;
	}


	//FIX ME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if(u_caster != NULL)
	{
		if(hasAttributeEx(ATTRIBUTESEX_DRAIN_WHOLE_POWER))  // Uses %100 power
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}

	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if(GetProto()->ManaCostPercentage) //Percentage spells cost % of !!!BASE!!! mana
	{
		if(GetProto()->powerType == POWER_TYPE_MANA)
			cost = (u_caster->GetBaseMana() * GetProto()->ManaCostPercentage) / 100;
		else
			cost = (u_caster->GetBaseHealth() * GetProto()->ManaCostPercentage) / 100;
	}
	else
	{
		cost = GetProto()->manaCost;
	}

	if((int32)GetProto()->powerType == POWER_TYPE_HEALTH)
		cost -= GetProto()->baseLevel;//FIX for life tap
	else if(u_caster != NULL)
	{
		if(GetProto()->powerType == POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[GetProto()->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost += float2int32(cost * u_caster->GetPowerCostMultiplier(GetProto()->School));
	}

	//hackfix for shiv's energy cost
	if(p_caster != NULL && m_spellInfo->NameHash == SPELL_HASH_SHIV)
	{
		Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		if(it != NULL)
			cost += (uint32)(10 * (it->GetProto()->Delay / 1000.0f));
	}

	//apply modifiers
	if(GetProto()->SpellGroupType && u_caster)
	{
		SM_FIValue(u_caster->SM_FCost, &cost, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PCost, &cost, GetProto()->SpellGroupType);
	}

	if(cost <= 0)
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
	if(u_caster != NULL)
		if(u_caster->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER))
			return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

	//Items do not use owner's power
	if(i_caster != NULL)
		return true;

	// Free cast for battle preparation
	if(p_caster && p_caster->HasAura(44521))
		return true;
	if(p_caster && p_caster->HasAura(44535))
		return true;
	if(p_caster && p_caster->HasAura(32727))
		return true;

	switch(GetProto()->powerType)
	{
		case POWER_TYPE_HEALTH:
			{	powerField = UNIT_FIELD_HEALTH;						}
			break;
		case POWER_TYPE_MANA:
			{	powerField = UNIT_FIELD_POWER1;	m_usesMana = true;	}
			break;
		case POWER_TYPE_RAGE:
			{	powerField = UNIT_FIELD_POWER2;						}
			break;
		case POWER_TYPE_FOCUS:
			{	powerField = UNIT_FIELD_POWER3;						}
			break;
		case POWER_TYPE_ENERGY:
			{	powerField = UNIT_FIELD_POWER4;						}
			break;
		case POWER_TYPE_HAPPINESS:
			{	powerField = UNIT_FIELD_POWER5;						}
			break;
		case POWER_TYPE_RUNIC_POWER:
			{	powerField = UNIT_FIELD_POWER7;						}
			break;
		case POWER_TYPE_RUNES:
			{
				if(GetProto()->RuneCostID && p_caster)
				{
					SpellRuneCostEntry* runecost = dbcSpellRuneCost.LookupEntry(GetProto()->RuneCostID);
					DeathKnight* dk = TO_DK(p_caster);
					uint32 credit = dk->TakeRunes(RUNE_BLOOD, runecost->bloodRuneCost) +
					                dk->TakeRunes(RUNE_FROST, runecost->frostRuneCost) +
					                dk->TakeRunes(RUNE_UNHOLY, runecost->unholyRuneCost);
					if(credit > 0 && dk->TakeRunes(RUNE_DEATH, credit) > 0)
						return false;
					if(runecost->runePowerGain)
						u_caster->SetPower(POWER_TYPE_RUNIC_POWER, runecost->runePowerGain + u_caster->GetPower(POWER_TYPE_RUNIC_POWER));
				}
				return true;
			}
		default:
			{
				LOG_DEBUG("unknown power type");
				// we shouldn't be here to return
				return false;
			}
			break;
	}

	//FIX ME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if(u_caster != NULL)
	{
		if(hasAttributeEx(ATTRIBUTESEX_DRAIN_WHOLE_POWER))  // Uses %100 power
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}

	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if(GetProto()->ManaCostPercentage) //Percentage spells cost % of !!!BASE!!! mana
	{
		if(GetProto()->powerType == POWER_TYPE_MANA)
			cost = (u_caster->GetBaseMana() * GetProto()->ManaCostPercentage) / 100;
		else
			cost = (u_caster->GetBaseHealth() * GetProto()->ManaCostPercentage) / 100;
	}
	else
	{
		cost = GetProto()->manaCost;
	}

	if((int32)GetProto()->powerType == POWER_TYPE_HEALTH)
		cost -= GetProto()->baseLevel;//FIX for life tap
	else if(u_caster != NULL)
	{
		if(GetProto()->powerType == POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[GetProto()->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost += float2int32(cost * u_caster->GetPowerCostMultiplier(GetProto()->School));
	}

	//hackfix for shiv's energy cost
	if(p_caster != NULL && m_spellInfo->NameHash == SPELL_HASH_SHIV)
	{
		Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		if(it != NULL)
			cost += it->GetProto()->Delay / 100;//10 * it->GetProto()->Delay / 1000;
	}

	//apply modifiers
	if(GetProto()->SpellGroupType && u_caster)
	{
		SM_FIValue(u_caster->SM_FCost, &cost, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PCost, &cost, GetProto()->SpellGroupType);
	}

	if(cost <= 0)
		return true;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		m_caster->DealDamage(u_caster, cost, 0, 0, 0, true);
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
	if(event_GetInstanceID() == WORLD_INSTANCE ||
		DuelSpellNoMoreValid())
	{
		DecRef();
		return;
	}

	if(guid == 0)
	{
		unitTarget = NULL;
		gameObjTarget = NULL;
		playerTarget = NULL;
		itemTarget = NULL;

		if(p_caster != NULL)
		{
			if(m_targets.m_targetMask & TARGET_FLAG_ITEM)
				itemTarget = p_caster->GetItemInterface()->GetItemByGUID(m_targets.m_itemTarget);
			if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
			{
				Player* p_trader = p_caster->GetTradeTarget();
				if(p_trader != NULL)
					itemTarget = p_trader->getTradeItem((uint32)m_targets.m_itemTarget);
			}
		}
	}
	else if(guid == m_caster->GetGUID())
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
			unitTarget = NULL;
			playerTarget = NULL;
			itemTarget = NULL;
			gameObjTarget = NULL;
			corpseTarget = NULL;
		}
		else if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
		{
			if(p_caster != NULL)
			{
				Player* plr = p_caster->GetTradeTarget();
				if(plr)
					itemTarget = plr->getTradeItem((uint32)guid);
			}
		}
		else
		{
			unitTarget = NULL;
			gameObjTarget = NULL;
			playerTarget = NULL;
			itemTarget = NULL;
			switch(GET_TYPE_FROM_GUID(guid))
			{
				case HIGHGUID_TYPE_UNIT:
				case HIGHGUID_TYPE_VEHICLE:
					unitTarget = m_caster->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
					break;
				case HIGHGUID_TYPE_PET:
					unitTarget = m_caster->GetMapMgr()->GetPet(GET_LOWGUID_PART(guid));
					break;
				case HIGHGUID_TYPE_PLAYER:
					{
						unitTarget =  m_caster->GetMapMgr()->GetPlayer(GET_LOWGUID_PART(guid));
						playerTarget = TO< Player* >(unitTarget);
					}
					break;
				case HIGHGUID_TYPE_ITEM:
					if(p_caster != NULL)
						itemTarget = p_caster->GetItemInterface()->GetItemByGUID(guid);

					break;
				case HIGHGUID_TYPE_GAMEOBJECT:
					gameObjTarget = m_caster->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
					break;
				case HIGHGUID_TYPE_CORPSE:
					corpseTarget = objmgr.GetCorpse(GET_LOWGUID_PART(guid));
					break;
				default:
					LOG_ERROR("unitTarget not set");
					DecRef();
					return;
			}
		}
	}

	uint32 id = GetProto()->Effect[i];

	damage = CalculateEffect(i, unitTarget);

#ifdef GM_Z_DEBUG_DIRECTLY
	if(playerTarget && playerTarget->IsPlayer() && playerTarget->IsInWorld())
	{
		if(playerTarget->GetSession() && playerTarget->GetSession()->CanUseCommand('z'))
			sChatHandler.BlueSystemMessage(playerTarget->GetSession(), "[%sSystem%s] |rSpellEffect::Handler: %s Target = %u, Effect id = %u, id = %u, Self: %u.", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE,
			                               playerTarget->GetLowGUID(), m_spellInfo->Effect[i], i, guid);
	}
#endif

	uint32 TargetType = 0;
	TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i);

	//never get info from B if it is 0 :P
	if(m_spellInfo->EffectImplicitTargetB[i] != 0)
		TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i);

	if(u_caster != NULL && unitTarget != NULL && unitTarget->IsCreature() && TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NO_INITIAL_AGGRO))
	{
		unitTarget->GetAIInterface()->AttackReaction(u_caster, 1, 0);
		unitTarget->GetAIInterface()->HandleEvent(EVENT_HOSTILEACTION, u_caster, 0);
	}


	if(id < TOTAL_SPELL_EFFECTS)
	{
		LOG_DEBUG("WORLD: Spell effect id = %u (%s), damage = %d", id, SpellEffectNames[id], damage);
		(*this.*SpellEffectsHandler[id])(i);
	}
	else
		LOG_ERROR("SPELL: unknown effect %u spellid %u", id, GetProto()->Id);

	DoAfterHandleEffect(unitTarget, i);
	DecRef();
}

void Spell::HandleAddAura(uint64 guid)
{
	Unit* Target = NULL;

	std::map<uint64, Aura*>::iterator itr = m_pendingAuras.find(guid);

	if(itr == m_pendingAuras.end() || itr->second == NULL)
	{
		DecRef();
		return;
	}

	//If this aura isn't added correctly it MUST be deleted
	Aura* aur = itr->second;
	itr->second = NULL;

	if(event_GetInstanceID() == WORLD_INSTANCE)
	{
		DecRef();
		return;
	}

	if(u_caster && u_caster->GetGUID() == guid)
		Target = u_caster;
	else if(m_caster->IsInWorld())
		Target = m_caster->GetMapMgr()->GetUnit(guid);

	if(Target == NULL)
	{
		delete aur;
		DecRef();
		return;
	}

	// Applying an aura to a flagged target will cause you to get flagged.
	// self casting doesn't flag himself.
	if(Target->IsPlayer() && p_caster && p_caster != TO< Player* >(Target))
	{
		if(TO< Player* >(Target)->IsPvPFlagged())
		{
			if(p_caster->IsPlayer() && !p_caster->IsPvPFlagged())
				TO< Player* >(p_caster)->PvPToggle();
			else
				p_caster->SetPvPFlag();
		}
	}

	// remove any auras with same type
	if(GetProto()->BGR_one_buff_on_target > 0)
	{
		Target->RemoveAurasByBuffType(GetProto()->BGR_one_buff_on_target, m_caster->GetGUID(), GetProto()->Id);
	}

	uint32 spellid = 0;

	if((GetProto()->MechanicsType == MECHANIC_INVULNARABLE && GetProto()->Id != 25771) || GetProto()->Id == 31884)     // Cast spell Forbearance
	{
		if(GetProto()->Id != 31884)
			spellid = 25771;

		if(Target->IsPlayer())
		{
			sEventMgr.AddEvent(TO< Player* >(Target), &Player::AvengingWrath, EVENT_PLAYER_AVENGING_WRATH, 30000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			TO< Player* >(Target)->mAvengingWrath = false;
		}
	}
	else if(GetProto()->MechanicsType == MECHANIC_HEALING && GetProto()->Id != 11196)  // Cast spell Recently Bandaged
		spellid = 11196;
	else if(GetProto()->MechanicsType == MECHANIC_SHIELDED && GetProto()->Id != 6788)  // Cast spell Weakened Soul
		spellid = 6788;
	else if(GetProto()->Id == 45438)  // Cast spell Hypothermia
		spellid = 41425;
	else if(GetProto()->NameHash == SPELL_HASH_HEROISM)
		spellid = 57723;
	else if(GetProto()->NameHash == SPELL_HASH_BLOODLUST)
		spellid = 57724;
	else if(GetProto()->NameHash == SPELL_HASH_STEALTH)
	{
		if(Target->HasAurasWithNameHash(SPELL_HASH_MASTER_OF_SUBTLETY))
			spellid = 31665;
	}
	else if(GetProto()->Id == 62124 && u_caster)
	{
		if(u_caster->HasAurasWithNameHash(SPELL_HASH_VINDICATION))
			spellid = u_caster->FindAuraByNameHash(SPELL_HASH_VINDICATION)->m_spellProto->RankNumber == 2 ? 26017 : 67;
	}
	else if(GetProto()->Id == 5229 &&
	        p_caster && (
	            p_caster->GetShapeShift() == FORM_BEAR ||
	            p_caster->GetShapeShift() == FORM_DIREBEAR) &&
	        p_caster->HasAurasWithNameHash(SPELL_HASH_KING_OF_THE_JUNGLE))
	{
		SpellEntry* spellInfo = dbcSpell.LookupEntryForced(51185);
		if(!spellInfo)
		{
			delete aur;
			DecRef();
			return;
		}

		Spell* spell = sSpellFactoryMgr.NewSpell(p_caster, spellInfo , true, NULL);
		spell->forced_basepoints[0] = p_caster->FindAuraByNameHash(SPELL_HASH_KING_OF_THE_JUNGLE)->m_spellProto->RankNumber * 5;
		SpellCastTargets targets(p_caster->GetGUID());
		spell->prepare(&targets);
	}
	else if(GetProto()->Id == 19574)
	{
		if(u_caster->HasAurasWithNameHash(SPELL_HASH_THE_BEAST_WITHIN))
			u_caster->CastSpell(u_caster, 34471, true);
	}
	else if(GetProto()->NameHash == SPELL_HASH_RAPID_KILLING)
	{
		if(u_caster->HasAurasWithNameHash(SPELL_HASH_RAPID_RECUPERATION))
			spellid = 56654;
	}

	switch(GetProto()->NameHash)
	{
		case SPELL_HASH_CLEARCASTING:
		case SPELL_HASH_PRESENCE_OF_MIND:
			{
				if(Target->HasAurasWithNameHash(SPELL_HASH_ARCANE_POTENCY))
					spellid = Target->FindAuraByNameHash(SPELL_HASH_ARCANE_POTENCY)->m_spellProto->RankNumber == 1 ? 57529 : 57531;
			}
			break;
	}

	if(spellid && Target)
	{
		SpellEntry* spellInfo = dbcSpell.LookupEntryForced(spellid);
		if(!spellInfo)
		{
			delete aur;
			DecRef();
			return;
		}

		Spell* spell = sSpellFactoryMgr.NewSpell(u_caster, spellInfo , true, NULL);

		if(spellid == 31665 && Target->HasAurasWithNameHash(SPELL_HASH_MASTER_OF_SUBTLETY))
			spell->forced_basepoints[0] = Target->FindAuraByNameHash(SPELL_HASH_MASTER_OF_SUBTLETY)->m_spellProto->EffectBasePoints[0];

		SpellCastTargets targets(Target->GetGUID());
		spell->prepare(&targets);
	}

	// avoid map corruption (this is impossible, btw)
	if(Target->GetInstanceID() != m_caster->GetInstanceID())
	{
		delete aur;
		DecRef();
		return;
	}

	int32 charges = m_charges;
	if(charges > 0)
	{
		if(u_caster != NULL)
		{
			SM_FIValue(u_caster->SM_FCharges, &charges, aur->GetSpellProto()->SpellGroupType);
			SM_PIValue(u_caster->SM_PCharges, &charges, aur->GetSpellProto()->SpellGroupType);
		}
		for(int i = 0; i < (charges - 1); ++i)
		{
			Aura* staur = sSpellFactoryMgr.NewAura(aur->GetSpellProto(), aur->GetDuration(), aur->GetCaster(), aur->GetTarget(), m_triggeredSpell, i_caster);
			Target->AddAura(staur);
		}
		if(!(aur->GetSpellProto()->procFlags & PROC_REMOVEONUSE))
		{
			SpellCharge charge;
			charge.count = charges;
			charge.spellId = aur->GetSpellId();
			charge.ProcFlag = aur->GetSpellProto()->procFlags;
			charge.lastproc = 0;
			Target->m_chargeSpells.insert(make_pair(aur->GetSpellId(), charge));
		}
	}

	Target->AddAura(aur); // the real spell is added last so the modifier is removed last

	DecRef();
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
			LOG_ERROR("WORLD: unknown spell id %i\n", TriggerSpellId);
			return;
		}

		Spell *spell = sSpellFactoryMgr.NewSpell(m_caster, spellInfo,false, NULL);
		WPARCEMU_ASSERT(   spell);

		SpellCastTargets targets;
		if(TriggerSpellTarget)
			targets.m_unitTarget = TriggerSpellTarget;
		else
			targets.m_unitTarget = m_targets.m_unitTarget;

		spell->prepare(&targets);
	}
}*/

void Spell::DetermineSkillUp()
{
	if(p_caster == NULL)
		return;

	skilllinespell* skill = objmgr.GetSpellSkill(GetProto()->Id);
	if(skill == NULL)
		return;

	float chance = 0.0f;

	if(p_caster->_HasSkillLine(skill->skilline))
	{
		uint32 amt = p_caster->_GetSkillLineCurrent(skill->skilline, false);
		uint32 max = p_caster->_GetSkillLineMax(skill->skilline);
		if(amt >= max)
			return;
		if(amt >= skill->grey)   //grey
			chance = 0.0f;
		else if((amt >= (((skill->grey - skill->green) / 2) + skill->green)))          //green
			chance = 33.0f;
		else if(amt >= skill->green)   //yellow
			chance = 66.0f;
		else //brown
			chance = 100.0f;
	}
	if(Rand(chance * sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skill->skilline, float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

bool Spell::IsAspect()
{
	return (
	           (GetProto()->Id ==  2596) || (GetProto()->Id ==  5118) || (GetProto()->Id == 14320) || (GetProto()->Id == 13159) || (GetProto()->Id == 13161) || (GetProto()->Id == 20190) ||
	           (GetProto()->Id == 20043) || (GetProto()->Id == 14322) || (GetProto()->Id == 14321) || (GetProto()->Id == 13163) || (GetProto()->Id == 14319) || (GetProto()->Id == 14318) || (GetProto()->Id == 13165));
}

bool Spell::IsSeal()
{
	return (
	           (GetProto()->Id == 13903) || (GetProto()->Id == 17177) || (GetProto()->Id == 20154) || (GetProto()->Id == 20164) ||
	           (GetProto()->Id == 20165) || (GetProto()->Id == 20166) || (GetProto()->Id == 20375) || (GetProto()->Id == 21084) ||
	           (GetProto()->Id == 31801) || (GetProto()->Id == 31892) || (GetProto()->Id == 53720) || (GetProto()->Id == 53736));
}

uint8 Spell::CanCast(bool tolerate)
{
	uint32 i;

	// Check if spell can be casted while player is moving.
	if( ( p_caster != NULL ) && p_caster->m_isMoving && ( m_spellInfo->InterruptFlags & CAST_INTERRUPT_ON_MOVEMENT ) && ( m_castTime != 0 ) && ( GetDuration() != 0 ) )
		return SPELL_FAILED_MOVING;

	// Check if spell requires caster to be in combat to be casted.
	if(p_caster != NULL && HasCustomFlag(CUSTOM_FLAG_SPELL_REQUIRES_COMBAT) && !p_caster->CombatStatus.IsInCombat())
		return SPELL_FAILED_SPELL_UNAVAILABLE;

	/**
	 *	Object cast checks
	 */
	if(m_caster && m_caster->IsInWorld())
	{
		Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);

		/**
		 *	Check for valid targets
		 */
		if(target)
		{
			// GM Flagged Players should be immune to other players' casts, but not their own.
			if((target != m_caster) && target->IsPlayer() && TO< Player* >(target)->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
				return SPELL_FAILED_BM_OR_INVISGOD;

			//you can't mind control someone already mind controlled
			if(GetProto()->NameHash == SPELL_HASH_MIND_CONTROL && target->HasAurasWithNameHash(SPELL_HASH_MIND_CONTROL))
				return SPELL_FAILED_BAD_TARGETS;

			if(GetProto()->NameHash == SPELL_HASH_DEATH_PACT && target->GetSummonedByGUID() != m_caster->GetGUID())
				return SPELL_FAILED_BAD_TARGETS;

			// Check if we can attack this creature type
			if(target->IsCreature())
			{
				Creature* cp = TO< Creature* >(target);
				uint32 type = cp->GetCreatureInfo()->Type;
				uint32 targettype = GetProto()->TargetCreatureType;

				if(!CanAttackCreatureType(targettype, type))
					return SPELL_FAILED_BAD_TARGETS;
			}
		}

		/**
		 *	Check for valid location
		 */
		if(GetProto()->Id == 32146)
		{
			Creature* corpse = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 18240);
			if(corpse != NULL)
				if(m_caster->CalcDistance(m_caster, corpse) > 5)
					return SPELL_FAILED_NOT_HERE;
		}
		else if(GetProto()->Id == 39246)
		{
			Creature* cleft = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 22105);
			if(cleft == NULL || cleft->isAlive())
				return SPELL_FAILED_NOT_HERE;
		}
		else if(GetProto()->Id == 30988)
		{
			Creature* corpse = m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), 17701);
			if(corpse != NULL)
				if(m_caster->CalcDistance(m_caster, corpse) > 5  || corpse->isAlive())
					return SPELL_FAILED_NOT_HERE;
		}
		else if(GetProto()->Id == 43723)
		{
			Creature* abysal = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), 19973);
			if(abysal != NULL)
			{
				if(!abysal->isAlive())
					if(!(p_caster->GetItemInterface()->GetItemCount(31672) > 1 && p_caster->GetItemInterface()->GetItemCount(31673) > 0 && p_caster->CalcDistance(p_caster, abysal) < 10))
						return SPELL_FAILED_NOT_HERE;
			}
			else
				return SPELL_FAILED_NOT_HERE;
		}
		else if(GetProto()->Id == 32307)
		{
			Creature* kilsorrow = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ());
			if(kilsorrow == NULL || kilsorrow->isAlive() || p_caster->CalcDistance(p_caster, kilsorrow) > 1)
				return SPELL_FAILED_NOT_HERE;
			if(kilsorrow->GetEntry() != 17147 && kilsorrow->GetEntry() != 17148 && kilsorrow->GetEntry() != 18397 && kilsorrow->GetEntry() != 18658 && kilsorrow->GetEntry() != 17146)
				return SPELL_FAILED_NOT_HERE;
		}
	}

	/**
	 *	Unit caster checks
	 */
	if(u_caster)
	{
		if(u_caster->HasAurasWithNameHash(SPELL_HASH_BLADESTORM) && GetProto()->NameHash != SPELL_HASH_WHIRLWIND)
			return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

		if(hasAttribute(ATTRIBUTES_REQ_OOC) && u_caster->CombatStatus.IsInCombat())
		{
			// Warbringer (Warrior 51Prot Talent effect)
			if((GetProto()->Id !=  100 && GetProto()->Id != 6178 && GetProto()->Id != 11578)
			        || (p_caster != NULL && !p_caster->ignoreShapeShiftChecks))
				return SPELL_FAILED_TARGET_IN_COMBAT;
		}
	}

	/**
	 *	Player caster checks
	 */
	if(p_caster)
	{
		/**
		 *	Stealth check
		 */
		if(hasAttribute(ATTRIBUTES_REQ_STEALTH) && !p_caster->IsStealth() && !p_caster->ignoreShapeShiftChecks)
			return SPELL_FAILED_ONLY_STEALTHED;

		/**
		 *	Indoor/Outdoor check
		 */
		if(sWorld.Collision)
		{
			if(GetProto()->MechanicsType == MECHANIC_MOUNTED)
			{
				if(CollideInterface.IsIndoor(p_caster->GetMapId(), p_caster->GetPositionNC()))
					return SPELL_FAILED_NO_MOUNTS_ALLOWED;
			}
			else if(hasAttribute(ATTRIBUTES_ONLY_OUTDOORS))
			{
				if(!CollideInterface.IsOutdoor(p_caster->GetMapId(), p_caster->GetPositionNC()))
					return SPELL_FAILED_ONLY_OUTDOORS;
			}
		}

		/**
		 *	Battlegrounds/Arena check
		 */
		if(p_caster->m_bg)
		{
			if(IS_ARENA(p_caster->m_bg->GetType()) && hasAttributeExD(FLAGS5_NOT_IN_ARENA))
				return SPELL_FAILED_NOT_IN_ARENA;
			if(!p_caster->m_bg->HasStarted() && (m_spellInfo->Id == 1953 || m_spellInfo->Id == 36554))  //Don't allow blink or shadowstep  if in a BG and the BG hasn't started.
				return SPELL_FAILED_SPELL_UNAVAILABLE;
		}
		else if(hasAttributeExC(FLAGS4_BG_ONLY))
			return SPELL_FAILED_ONLY_BATTLEGROUNDS;

		/**
		 *	Cooldowns check
		 */
		if(!tolerate && !p_caster->Cooldown_CanCast(GetProto()))
			return SPELL_FAILED_NOT_READY;

		/**
		 * Mana check
		 */
		if(!HasPower())
			return SPELL_FAILED_NO_POWER;

		/**
		 *	Duel request check
		 */
		if(p_caster->GetDuelState() == DUEL_STATE_REQUESTED)
		{
			for(i = 0; i < 3; ++i)
			{
				if(GetProto()->Effect[i] && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_AURA && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_PET_AREA_AURA
				        && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_GROUP_AREA_AURA && GetProto()->Effect[i] != SPELL_EFFECT_APPLY_RAID_AREA_AURA)
				{
					return SPELL_FAILED_TARGET_DUELING;
				}
			}
		}

		/**
		 *	Duel area check
		 */
		if(GetProto()->Id == 7266)
		{
			AreaTable* at = dbcArea.LookupEntry(p_caster->GetAreaID());
			if(at->AreaFlags & AREA_CITY_AREA)
				return SPELL_FAILED_NO_DUELING;
			// instance & stealth checks
			if(p_caster->GetMapMgr() && p_caster->GetMapMgr()->GetMapInfo() && p_caster->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
				return SPELL_FAILED_NO_DUELING;
			if(p_caster->IsStealth())
				return SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED;
		}

		/**
		 *	On taxi check
		 */
		if(p_caster->m_onTaxi)
		{
			if(!hasAttribute(ATTRIBUTES_MOUNT_CASTABLE))    //Are mount castable spells allowed on a taxi?
			{
				if(m_spellInfo->Id != 33836 && m_spellInfo->Id != 45072 && m_spellInfo->Id != 45115 && m_spellInfo->Id != 31958)   // exception for taxi bombs
					return SPELL_FAILED_NOT_ON_TAXI;
			}
		}
		else
		{
			if(m_spellInfo->Id == 33836 || m_spellInfo->Id == 45072 || m_spellInfo->Id == 45115 || m_spellInfo->Id == 31958)
				return SPELL_FAILED_NOT_HERE;
		}

		/**
		 *	Is mounted check
		 */
		if(!p_caster->IsMounted())
		{
			if(GetProto()->Id == 25860)  // Reindeer Transformation
				return SPELL_FAILED_ONLY_MOUNTED;
		}
		else
		{
			if(!hasAttribute(ATTRIBUTES_MOUNT_CASTABLE))
				return SPELL_FAILED_NOT_MOUNTED;
		}

		/**
		 *	Filter Check
		 */
		if(p_caster->m_castFilterEnabled &&
		        !((m_spellInfo->SpellGroupType[0] & p_caster->m_castFilter[0]) ||
		          (m_spellInfo->SpellGroupType[1] & p_caster->m_castFilter[1]) ||
		          (m_spellInfo->SpellGroupType[2] & p_caster->m_castFilter[2])))
			return SPELL_FAILED_SPELL_IN_PROGRESS;

		/**
		 *	Shapeshifting checks
		 */
		if(!p_caster->ignoreShapeShiftChecks)
		{
			// No need to go through this function if the results are gonna be ignored anyway
			uint8 shapeError = GetErrorAtShapeshiftedCast(GetProto(), p_caster->GetShapeShift());
			if(shapeError != 0)
				return shapeError;
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
						if(!(GetProto()->c_is_flags & SPELL_FLAG_IS_HEALING) && GetProto()->Id != 7355)
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

		/**
		 *	check if spell requires shapeshift
		 */
		// I think Spell prototype's RequiredShapeShift is not entirely accurate ....
		//if( GetProto()->RequiredShapeShift && !(GetProto()->RequiredShapeShift == (uint32)1 << (FORM_SHADOW - 1)) && !((uint32)1 << (p_caster->GetShapeShift()-1) & GetProto()->RequiredShapeShift ) )
		//{
		//	return SPELL_FAILED_ONLY_SHAPESHIFT;
		//}


		/**
		 *	check if spell is allowed while we have a battleground flag
		 */
		if(p_caster->m_bgHasFlag)
		{
			switch(m_spellInfo->Id)
			{
					// stealth spells
				case 1784:
				case 1785:
				case 1786:
				case 1787:
				case 5215:
				case 6783:
				case 9913:
				case 1856:
				case 1857:
				case 26889:
					{
						// thank Cruders for this :P
						if(p_caster->m_bg && p_caster->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
							p_caster->m_bg->HookOnFlagDrop(p_caster);
						else if(p_caster->m_bg && p_caster->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
							p_caster->m_bg->HookOnFlagDrop(p_caster);
						break;
					}
			}


		}

		/**
		 *	Item spell checks
		 */
		if(i_caster)
		{
			if(i_caster->GetProto()->ZoneNameID && i_caster->GetProto()->ZoneNameID != i_caster->GetZoneId())
				return SPELL_FAILED_NOT_HERE;
			if(i_caster->GetProto()->MapID && i_caster->GetProto()->MapID != i_caster->GetMapId())
				return SPELL_FAILED_NOT_HERE;

			if(i_caster->GetProto()->Spells[0].Charges != 0)
			{
				// check if the item has the required charges
				if(i_caster->GetCharges(0) == 0)
					return SPELL_FAILED_NO_CHARGES_REMAIN;
			}
		}

		/**
		 *	Check if we have the required reagents
		 */
		if(!(p_caster->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NO_REAGANT_COST) && hasAttributeExE(FLAGS6_REAGENT_REMOVAL)))
		{
			// Skip this with enchanting scrolls
			if(!i_caster || i_caster->GetProto()->Flags != 268435520)
			{
				for(i = 0; i < 8 ; i++)
				{
					if(GetProto()->Reagent[i] == 0 || GetProto()->ReagentCount[i] == 0)
						continue;

					if(p_caster->GetItemInterface()->GetItemCount(GetProto()->Reagent[i]) < GetProto()->ReagentCount[i])
						return SPELL_FAILED_ITEM_GONE;
				}
			}
		}

		/**
		 *	check if we have the required tools, totems, etc
		 */
		for(i = 0; i < 2; i++)
		{
			if(GetProto()->Totem[i] != 0)
			{
				if(p_caster->GetItemInterface()->GetItemCount(GetProto()->Totem[i]) == 0)
					return SPELL_FAILED_TOTEMS;
			}
		}

		/**
		 *	check if we have the required gameobject focus
		 */
		float focusRange;

		if(GetProto()->RequiresSpellFocus)
		{
			bool found = false;

			for(std::set<Object*>::iterator itr = p_caster->GetInRangeSetBegin(); itr != p_caster->GetInRangeSetEnd(); itr++)
			{
				if(!(*itr)->IsGameObject())
					continue;

				if((TO_GAMEOBJECT(*itr))->GetType() != GAMEOBJECT_TYPE_SPELL_FOCUS)
					continue;

				if(!(p_caster->GetPhase() & (*itr)->GetPhase()))    //We can't see this, can't be the focus, skip further checks
					continue;

				GameObjectInfo* info = TO_GAMEOBJECT(*itr)->GetInfo();
				if(!info)
				{
					LOG_DEBUG("Warning: could not find info about game object %u", (*itr)->GetEntry());
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

			if(!found)
				return SPELL_FAILED_REQUIRES_SPELL_FOCUS;
		}

		/**
		 *	Area requirement
		 */
		if(GetProto()->RequiresAreaId > 0)
		{
			AreaGroup* ag = dbcAreaGroup.LookupEntry(GetProto()->RequiresAreaId);
			uint32 plrarea = p_caster->GetMapMgr()->GetAreaID(p_caster->GetPositionX(), p_caster->GetPositionY());
			if(plrarea != 0xFFFF)//disabling Area checks for maps with no Map_XY.bin file.
			{
				AreaTable* at = dbcArea.LookupEntry(plrarea);
				for(i = 0; i < 7; i++)
					if(ag->AreaId[i] == plrarea || (at->ZoneId != 0 && ag->AreaId[i] == at->ZoneId))    //we check both Area and Zone but only if Zone is a valid one, so != 0.
						break;
				if(i == 7)
					return SPELL_FAILED_REQUIRES_AREA;
			}
		}

		/**
		 *	AuraState check
		 */
		if(!p_caster->ignoreAuraStateCheck)
		{
			if((GetProto()->CasterAuraState && !p_caster->HasFlag(UNIT_FIELD_AURASTATE, GetProto()->CasterAuraState))
			        ||	(GetProto()->CasterAuraStateNot && p_caster->HasFlag(UNIT_FIELD_AURASTATE, GetProto()->CasterAuraStateNot))
			  )
				return SPELL_FAILED_CASTER_AURASTATE;
		}

		/**
		 *	Aura check
		 */
		if(GetProto()->casterAuraSpell && !p_caster->HasAura(GetProto()->casterAuraSpell))
		{
			return SPELL_FAILED_NOT_READY;
		}
		if(GetProto()->casterAuraSpellNot && p_caster->HasAura(GetProto()->casterAuraSpellNot))
		{
			return SPELL_FAILED_NOT_READY;
		}
	}

	/**
	*	Targeted Item Checks
	 */
	if(p_caster && m_targets.m_itemTarget)
	{
		Item* i_target = NULL;

		// check if the targeted item is in the trade box
		if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
		{
			switch(GetProto()->Effect[0])
			{
					// only lockpicking and enchanting can target items in the trade box
				case SPELL_EFFECT_OPEN_LOCK:
				case SPELL_EFFECT_ENCHANT_ITEM:
				case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
					{
						// check for enchants that can only be done on your own items
						if(hasAttributeExB(ATTRIBUTESEXB_ENCHANT_OWN_ONLY))
							return SPELL_FAILED_BAD_TARGETS;

						// get the player we are trading with
						Player* t_player = p_caster->GetTradeTarget();
						// get the targeted trade item
						if(t_player)
							i_target = t_player->getTradeItem((uint32)m_targets.m_itemTarget);
					}
			}
		}
		// targeted item is not in a trade box, so get our own item
		else
		{
			i_target = p_caster->GetItemInterface()->GetItemByGUID(m_targets.m_itemTarget);
		}

		// check to make sure we have a targeted item
		// the second check is a temporary exploit fix, people keep stacking enchants on 0 durability items and then 1hit/1shot the other guys
		if(!i_target || (i_target->GetDurability() == 0 && i_target->GetDurabilityMax() != 0))
			return SPELL_FAILED_BAD_TARGETS;

		ItemPrototype* proto = i_target->GetProto();

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
			case SPELL_EFFECT_ADD_SOCKET:
				{
					if(GetProto()->Effect[ 0 ] == SPELL_EFFECT_ADD_SOCKET && i_target->GetSocketsCount() >= 3)
						return SPELL_FAILED_MAX_SOCKETS;

					// If enchant is permanent and we are casting on Vellums
					if(GetProto()->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM && GetProto()->EffectItemType[0] != 0 &&
					        (proto->ItemId == 38682 || proto->ItemId == 37602 || proto->ItemId == 43145 ||
					         proto->ItemId == 39349 || proto->ItemId == 39350 || proto->ItemId == 43146))
					{
						// Weapons enchants
						if(GetProto()->EquippedItemClass == 2)
						{
							// These are armor vellums
							if(proto->ItemId == 38682 || proto->ItemId == 37602 || proto->ItemId == 43145)
								return SPELL_FAILED_BAD_TARGETS;

							// You tried to cast wotlk enchant on bad item
							if(GetProto()->baseLevel == 60 && proto->ItemId != 43146)
								return SPELL_FAILED_BAD_TARGETS;

							// you tried to cast tbc enchant on bad item
							if(GetProto()->baseLevel == 35 && proto->ItemId == 39349)
								return SPELL_FAILED_BAD_TARGETS;

							// you tried to cast non-lvl enchant on bad item
							if(GetProto()->baseLevel == 0 && proto->ItemId != 39349)
								return SPELL_FAILED_BAD_TARGETS;

							break;
						}

						// Armors enchants
						else if(GetProto()->EquippedItemClass == 4)
						{
							// These are weapon vellums
							if(proto->ItemId == 39349 || proto->ItemId == 39350 || proto->ItemId == 43146)
								return SPELL_FAILED_BAD_TARGETS;

							// You tried to cast wotlk enchant on bad item
							if(GetProto()->baseLevel == 60 && proto->ItemId != 43145)
								return SPELL_FAILED_BAD_TARGETS;

							// you tried to cast tbc enchant on bad item
							if(GetProto()->baseLevel == 35 && proto->ItemId == 38682)
								return SPELL_FAILED_BAD_TARGETS;

							// you tried to cast non-lvl enchant on bad item
							if(GetProto()->baseLevel == 0 && proto->ItemId != 38682)
								return SPELL_FAILED_BAD_TARGETS;
						}

						// If We are here it means that we have right Vellum and right enchant to cast
						break;
					}

					// check if we have the correct class, subclass, and inventory type of target item
					if(GetProto()->EquippedItemClass != (int32)proto->Class)
						return SPELL_FAILED_BAD_TARGETS;

					if(GetProto()->EquippedItemSubClass && !(GetProto()->EquippedItemSubClass & (1 << proto->SubClass)))
						return SPELL_FAILED_BAD_TARGETS;

					if(GetProto()->RequiredItemFlags && !(GetProto()->RequiredItemFlags & (1 << proto->InventoryType)))
						return SPELL_FAILED_BAD_TARGETS;

					if(GetProto()->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM &&
					        GetProto()->baseLevel && (GetProto()->baseLevel > proto->ItemLevel))
						return int8(SPELL_FAILED_BAD_TARGETS); // maybe there is different err code

					if(i_caster && i_caster->GetProto()->Flags == 2097216)
						break;

					// If the spell is castable on our own items only then we can't cast it on someone else's
					if(hasAttributeExB(ATTRIBUTESEXB_ENCHANT_OWN_ONLY) &&
					        i_target != NULL &&
					        u_caster != NULL &&
					        TO_PLAYER(u_caster) != i_target->GetOwner())
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
					Pet* pPet = p_caster->GetSummon();

					// check if we have a pet
					if(!pPet)
						return SPELL_FAILED_NO_PET;

					// check if pet lives
					if(!pPet->isAlive())
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
				// Milling Targeted Item Check
			case SPELL_EFFECT_MILLING:
				{
					// check if the item can be prospected
					if(!(proto->Flags & ITEM_FLAG_MILLABLE))
						return SPELL_FAILED_CANT_BE_PROSPECTED;

					// check if we have at least 5 of the item
					if(p_caster->GetItemInterface()->GetItemCount(proto->ItemId) < 5)
						return SPELL_FAILED_ITEM_GONE;

					// check if we have high enough skill
					if(p_caster->_GetSkillLineCurrent(SKILL_INSCRIPTION) < proto->RequiredSkillRank)
						return SPELL_FAILED_LOW_CASTLEVEL;

					break;
				}

		} // end switch

	} // end targeted item

	/**
	 *	set up our max range
	 *	latency compensation!!
	 *	figure out how much extra distance we need to allow for based on our
	 *	movespeed and latency.
	 */
	float maxRange = 0;

	SpellRange* range = dbcSpellRange.LookupEntry(GetProto()->rangeIndex);

	if(m_caster->IsInWorld())
	{
		Unit* target = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
		if(target != NULL && isFriendly(m_caster, target))
			maxRange = range->maxRangeFriendly;
		else
			maxRange = range->maxRange;
	}
	else
		maxRange = range->maxRange;

	if(u_caster && m_caster->GetMapMgr() && m_targets.m_unitTarget)
	{
		Unit* utarget = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);

		if(utarget && utarget->IsPlayer() && TO< Player* >(utarget)->m_isMoving)
		{
			// this only applies to PvP.
			uint32 lat = TO< Player* >(utarget)->GetSession() ? TO< Player* >(utarget)->GetSession()->GetLatency() : 0;

			// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
			lat = (lat > 500) ? 500 : lat;

			// calculate the added distance
			maxRange += u_caster->m_runSpeed * 0.001f * lat;
		}
	}

	/**
	 *	Some Unit caster range check
	 */
	if(u_caster && GetProto()->SpellGroupType)
	{
		SM_FFValue(u_caster->SM_FRange, &maxRange, GetProto()->SpellGroupType);
		SM_PFValue(u_caster->SM_PRange, &maxRange, GetProto()->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers = 0;
		float spell_pct_modifers = 0;
		SM_FFValue(u_caster->SM_FRange, &spell_flat_modifers, GetProto()->SpellGroupType);
		SM_FFValue(u_caster->SM_PRange, &spell_pct_modifers, GetProto()->SpellGroupType);
		if(spell_flat_modifers != 0 || spell_pct_modifers != 0)
			LOG_DEBUG("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u", spell_flat_modifers, spell_pct_modifers, maxRange, GetProto()->SpellGroupType);
#endif
	}

	// Targeted Location Checks (AoE spells)
	if(m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION)
	{
		if(!IsInrange(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, m_caster, (maxRange * maxRange)))
			return SPELL_FAILED_OUT_OF_RANGE;
	}

	/**
	 *	Targeted Unit Checks
	 */
	if(m_targets.m_unitTarget)
	{
		Unit* target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULL;

		if(target)
		{
			// UNIT_FIELD_BOUNDINGRADIUS + 1.5f; seems to match the client range

			if(tolerate)   // add an extra 33% to range on final check (squared = 1.78x)
			{
				float localrange = maxRange + target->GetBoundingRadius() + 1.5f;
				if(!IsInrange(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, (localrange * localrange * 1.78f)))
					return SPELL_FAILED_OUT_OF_RANGE;
			}
			else
			{
				float localrange = maxRange + target->GetBoundingRadius() + 1.5f;
				if(!IsInrange(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, (localrange * localrange)))
					return SPELL_FAILED_OUT_OF_RANGE;
			}

			/* Target OOC check */
			if(hasAttributeEx(ATTRIBUTESEX_REQ_OOC_TARGET) && target->CombatStatus.IsInCombat())
				return SPELL_FAILED_TARGET_IN_COMBAT;

			if(p_caster != NULL)
			{
				if(p_caster->HasAurasWithNameHash(SPELL_HASH_BLADESTORM) && GetProto()->NameHash != SPELL_HASH_WHIRLWIND)
					return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

				if(GetProto()->Id == SPELL_RANGED_THROW)
				{
					Item* itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(itm == NULL)
						return SPELL_FAILED_NO_AMMO;
				}

				if(sWorld.Collision)
				{
					if(p_caster->GetMapId() == target->GetMapId() && !p_caster->GetMapMgr()->InLineOfSight(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ() + 2, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 2))
						return SPELL_FAILED_LINE_OF_SIGHT;
				}

				// check aurastate
				if(GetProto()->TargetAuraState && !target->HasFlag(UNIT_FIELD_AURASTATE, GetProto()->TargetAuraState) && !p_caster->ignoreAuraStateCheck)
				{
					return SPELL_FAILED_TARGET_AURASTATE;
				}
				if(GetProto()->TargetAuraStateNot && target->HasFlag(UNIT_FIELD_AURASTATE, GetProto()->TargetAuraStateNot) && !p_caster->ignoreAuraStateCheck)
				{
					return SPELL_FAILED_TARGET_AURASTATE;
				}

				// check aura
				if(GetProto()->targetAuraSpell && !target->HasAura(GetProto()->targetAuraSpell))
				{
					return SPELL_FAILED_NOT_READY;
				}
				if(GetProto()->targetAuraSpellNot && target->HasAura(GetProto()->targetAuraSpellNot))
				{
					return SPELL_FAILED_NOT_READY;
				}

				if(target->IsPlayer())
				{
					// disallow spell casting in sanctuary zones
					// allow attacks in duels
					if(p_caster->DuelingWith != target && !isFriendly(p_caster, target))
					{
						AreaTable* atCaster = dbcArea.LookupEntry(p_caster->GetAreaID());
						AreaTable* atTarget = dbcArea.LookupEntry(TO< Player* >(target)->GetAreaID());
						if(atCaster->AreaFlags & 0x800 || atTarget->AreaFlags & 0x800)
							return SPELL_FAILED_NOT_HERE;
					}
				}
				else
				{
					if(target->GetAIInterface()->GetIsSoulLinked() && u_caster && target->GetAIInterface()->getSoullinkedWith() != u_caster)
						return SPELL_FAILED_BAD_TARGETS;
				}

				// pet training
				if(GetProto()->EffectImplicitTargetA[0] == EFF_TARGET_PET &&
				        GetProto()->Effect[0] == SPELL_EFFECT_LEARN_SPELL)
				{
					Pet* pPet = p_caster->GetSummon();
					// check if we have a pet
					if(pPet == NULL)
						return SPELL_FAILED_NO_PET;

					// other checks
					SpellEntry* trig = dbcSpell.LookupEntryForced(GetProto()->EffectTriggerSpell[0]);
					if(trig == NULL)
						return SPELL_FAILED_SPELL_UNAVAILABLE;

					uint32 status = pPet->CanLearnSpell(trig);
					if(status != 0)
						return static_cast<uint8>(status);
				}

				if(GetProto()->EffectApplyAuraName[0] == SPELL_AURA_MOD_POSSESS)  //mind control
				{
					if(GetProto()->EffectBasePoints[0])  //got level req;
					{
						if((int32)target->getLevel() > GetProto()->EffectBasePoints[0] + 1 + int32(p_caster->getLevel() - GetProto()->spellLevel))
							return SPELL_FAILED_HIGHLEVEL;
						else if(target->IsCreature())
						{
							Creature* c = TO_CREATURE(target);
							if(c->GetCreatureInfo()->Rank > ELITE_ELITE)
								return SPELL_FAILED_HIGHLEVEL;
						}
					}
				}
			}

////////////////////////////////////////////////////// Target check spells that are only castable on certain creatures/gameobjects ///////////////

			if(m_target_constraint != NULL)
			{
				// Spell has GO and/or Creature target constraint, yet target is neither -> bad target
				if(!target->IsCreature() && !target->IsGameObject())
					return SPELL_FAILED_BAD_TARGETS;

				// target is the wrong creature
				if(target->IsCreature() && !m_target_constraint->HasCreature(target->GetEntry()))
					return SPELL_FAILED_BAD_TARGETS;

				// target is the wrong GO :/
				if(target->IsGameObject() && !m_target_constraint->HasGameobject(target->GetEntry()))
					return SPELL_FAILED_BAD_TARGETS;
			}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// scripted spell stuff
			switch(GetProto()->Id)
			{
				case 1515: // tame beast
					{
						uint8 result = 0;
						Unit* tgt = unitTarget;
						if(tgt == NULL)
						{
							// we have to pick a target manually as this is a dummy spell which triggers tame effect at end of channeling
							if(p_caster->GetSelection() != 0)
								tgt =  p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
							else
								return SPELL_FAILED_UNKNOWN;
						}

						Creature* tame = tgt->IsCreature() ? TO_CREATURE(tgt) : NULL;

						if(tame == NULL)
							result = PETTAME_INVALIDCREATURE;
						else if(!tame->isAlive())
							result = PETTAME_DEAD;
						else if(tame->IsPet())
							result = PETTAME_CREATUREALREADYOWNED;
						else if(tame->GetCreatureInfo()->Type != UNIT_TYPE_BEAST || !tame->GetCreatureInfo()->Family || !(tame->GetCreatureInfo()->Flags1 & CREATURE_FLAG1_TAMEABLE))
							result = PETTAME_NOTTAMEABLE;
						else if(!p_caster->isAlive() || p_caster->getClass() != HUNTER)
							result = PETTAME_UNITSCANTTAME;
						else if(tame->getLevel() > p_caster->getLevel())
							result = PETTAME_TOOHIGHLEVEL;
						else if(p_caster->GetSummon() || p_caster->GetUnstabledPetNumber())
							result = PETTAME_ANOTHERSUMMONACTIVE;
						else if(p_caster->GetPetCount() >= 5)
							result = PETTAME_TOOMANY;
						else if(!p_caster->HasSpell(53270) && tame->IsExotic())
							result = PETTAME_CANTCONTROLEXOTIC;
						else
						{
							CreatureFamilyEntry* cf = dbcCreatureFamily.LookupEntryForced(tame->GetCreatureInfo()->Family);
							if(cf && !cf->tameable)
								result = PETTAME_NOTTAMEABLE;
						}
						if(result != 0)
						{
							SendTameFailure(result);
							return SPELL_FAILED_DONT_REPORT;
						}
					}
					break;

				case 603: //curse of doom, can't be cast on players
				case 30910:
				case 47867: // Curse of doom rank 4
					{
						if(target->IsPlayer())
							return SPELL_FAILED_TARGET_IS_PLAYER;
					}
					break;
				case 13907: // Smite Demon
					{
						if(target->IsPlayer() || target->getClass() != TARGET_TYPE_DEMON)
							return SPELL_FAILED_SPELL_UNAVAILABLE;
					}
					break;

				default:
					break;
			}

			// if the target is not the unit caster and not the masters pet
			if(target != u_caster && !m_caster->IsPet())
			{

				/***********************************************************
				* Inface checks, these are checked in 2 ways
				* 1e way is check for damage type, as 3 is always ranged
				* 2e way is trough the data in the extraspell db
				*
				**********************************************************/

				uint32 facing_flags = GetProto()->FacingCasterFlags;

				// Holy shock need enemies be in front of caster
				if(GetProto()->NameHash == SPELL_HASH_HOLY_SHOCK && GetProto()->Effect[0] == SPELL_EFFECT_DUMMY && ! isFriendly(u_caster, target))
					facing_flags = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

				/* burlex: units are always facing the target! */
				if(p_caster && facing_flags != SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK)
				{
					if(GetProto()->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED)
					{
						// our spell is a ranged spell
						if(!p_caster->isInFront(target))
							return SPELL_FAILED_UNIT_NOT_INFRONT;
					}
					else
					{
						// our spell is not a ranged spell
						if(facing_flags == SPELL_INFRONT_STATUS_REQUIRE_INFRONT)
						{
							// must be in front
							if(!u_caster->isInFront(target))
								return SPELL_FAILED_UNIT_NOT_INFRONT;
						}
						else if(facing_flags == SPELL_INFRONT_STATUS_REQUIRE_INBACK)
						{
							// behind
							if(target->isInFront(u_caster))
								return SPELL_FAILED_NOT_BEHIND;
						}
					}
				}
			}

			if(GetProto()->Effect[0] == SPELL_EFFECT_SKINNING)  // skinning
			{
				// if target doesn't have skinnable flag, don't let it be skinned
				if(!target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE))
					return SPELL_FAILED_TARGET_UNSKINNABLE;
				// if target is already skinned, don't let it be skinned again
				if(target->IsCreature() && TO_CREATURE(target)->Skinned)
					return SPELL_FAILED_TARGET_UNSKINNABLE;
			}

			// all spells with target 61 need to be in group or raid
			// TODO: need to research this if its not handled by the client!!!
			if(GetProto()->EffectImplicitTargetA[0] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS ||
			        GetProto()->EffectImplicitTargetA[1] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS ||
			        GetProto()->EffectImplicitTargetA[2] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS)
			{
				if(target->IsPlayer() && !TO< Player* >(target)->InGroup())
					return SPELL_FAILED_TARGET_NOT_IN_PARTY;
			}

			// fishing spells
			if(GetProto()->EffectImplicitTargetA[0] == EFF_TARGET_SELF_FISHING)  //||
				//GetProto()->EffectImplicitTargetA[1] == EFF_TARGET_SELF_FISHING ||
				//GetProto()->EffectImplicitTargetA[2] == EFF_TARGET_SELF_FISHING )
			{
				uint32 entry = GetProto()->EffectMiscValue[0];
				if(entry == GO_FISHING_BOBBER)
				{
					//uint32 mapid = p_caster->GetMapId();
					float px = u_caster->GetPositionX();
					float py = u_caster->GetPositionY();
					float pz = u_caster->GetPositionZ();
					float orient = m_caster->GetOrientation();
					float posx = 0, posy = 0, posz = 0;
					float co = cos(orient);
					float si = sin(orient);
					MapMgr* map = m_caster->GetMapMgr();

					float r;
					for(r = 20; r > 10; r--)
					{
						posx = px + r * co;
						posy = py + r * si;
						uint32 liquidtype;
						map->GetLiquidInfo(posx, posy, pz + 2, posz, liquidtype);
						if(!(liquidtype & 1))//water
							continue;
						if(!map->InLineOfSight(px, py, pz + 0.5f, posx, posy, posz))
							continue;
						if(posz > map->GetLandHeight(posx, posy, pz + 2))
							break;
					}
					if(r <= 10)
						return SPELL_FAILED_NOT_FISHABLE;

					// if we are already fishing, don't cast it again
					if(p_caster->GetSummonedObject())
						if(p_caster->GetSummonedObject()->GetEntry() == GO_FISHING_BOBBER)
							return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}

			if(p_caster != NULL)
			{
				if(GetProto()->NameHash == SPELL_HASH_GOUGE)  // Gouge
					if(!target->isInFront(p_caster))
						return SPELL_FAILED_NOT_INFRONT;

				if(GetProto()->Category == 1131) //Hammer of wrath, requires target to have 20- % of hp
				{
					if(target->GetHealth() == 0)
						return SPELL_FAILED_BAD_TARGETS;

					if(target->GetMaxHealth() / target->GetHealth() < 5)
						return SPELL_FAILED_BAD_TARGETS;
				}
				else if(GetProto()->Category == 672) //Conflagrate, requires immolation spell on victim
				{
					if(!target->HasAuraVisual(46))
						return SPELL_FAILED_BAD_TARGETS;
				}

				if(target->dispels[GetProto()->DispelType])
					return SPELL_FAILED_DAMAGE_IMMUNE;			// hackfix - burlex

				// Removed by Supalosa and moved to 'completed cast'
				//if(target->MechanicsDispels[GetProto()->MechanicsType])
				//	return SPELL_FAILED_PREVENTED_BY_MECHANIC-1; // Why not just use 	SPELL_FAILED_DAMAGE_IMMUNE                                   = 144,
			}

			// if we're replacing a higher rank, deny it
			AuraCheckResponse acr = target->AuraCheck(GetProto(), m_caster);
			if(acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT)
				return SPELL_FAILED_AURA_BOUNCED;

			//check if we are trying to stealth or turn invisible but it is not allowed right now
			if(IsStealthSpell() || IsInvisibilitySpell())
			{
				//if we have Faerie Fire, we cannot stealth or turn invisible
				if(u_caster->FindAuraByNameHash(SPELL_HASH_FAERIE_FIRE) || u_caster->FindAuraByNameHash(SPELL_HASH_FAERIE_FIRE__FERAL_))
					return SPELL_FAILED_SPELL_UNAVAILABLE;
			}
		}
	}

	// Special State Checks (for creatures & players)
	if(u_caster != NULL)
	{
		if(u_caster->SchoolCastPrevent[GetProto()->School])
		{
			uint32 now_ = getMSTime();
			if(now_ > u_caster->SchoolCastPrevent[GetProto()->School]) //this limit has expired,remove
				u_caster->SchoolCastPrevent[GetProto()->School] = 0;
			else
			{
				// HACK FIX
				switch(GetProto()->NameHash)
				{
						// This is actually incorrect. school lockouts take precedence over silence.
						// So ice block/divine shield are not usable while their schools are locked out,
						// but can be used while silenced.
						/*case SPELL_HASH_ICE_BLOCK: //Ice Block
						case 0x9840A1A6: //Divine Shield
							break;
						*/
					case SPELL_HASH_WILL_OF_THE_FORSAKEN:
						{
							if(u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP))
								break;
						}
						break;

					case SPELL_HASH_DEATH_WISH:
					case SPELL_HASH_FEAR_WARD:
					case SPELL_HASH_BERSERKER_RAGE:
						{
							if(u_caster->m_special_state & UNIT_STATE_FEAR)
								break;
						}
						break;

						// {Insignia|Medallion} of the {Horde|Alliance}
					case SPELL_HASH_PVP_TRINKET:
					case SPELL_HASH_EVERY_MAN_FOR_HIMSELF:
					case SPELL_HASH_DIVINE_SHIELD:
						{
							if(u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_ROOT | UNIT_STATE_STUN | UNIT_STATE_CONFUSE | UNIT_STATE_SNARE))
								break;
						}
						break;

					case SPELL_HASH_BARKSKIN:
						{
							// This spell is usable while stunned, frozen, incapacitated, feared or asleep.  Lasts 12 sec.
							if(u_caster->m_special_state & (UNIT_STATE_STUN | UNIT_STATE_FEAR | UNIT_STATE_SLEEP))     // Uh, what unit_state is Frozen? (freezing trap...)
								break;
						}
						break;

					case SPELL_HASH_DISPERSION:
						{
							if(u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_STUN | UNIT_STATE_SILENCE))
								break;
						}
						break;

					default:
						return SPELL_FAILED_SILENCED;
				}
			}
		}

		// can only silence non-physical
		if(u_caster->m_silenced && GetProto()->School != SCHOOL_NORMAL)
		{
			switch(GetProto()->NameHash)
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				case SPELL_HASH_DISPERSION:
					break;

				default:
					return SPELL_FAILED_SILENCED;
			}
		}

		Unit* target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULL;
		if(target)  /* -Supalosa- Shouldn't this be handled on Spell Apply? */
		{
			for(i = 0; i < 3; i++)  // if is going to cast a spell that breaks stun remove stun auras, looks a bit hacky but is the best way i can find
			{
				if(GetProto()->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY)
				{
					target->RemoveAllAurasByMechanic(GetProto()->EffectMiscValue[i] , static_cast<uint32>(-1) , true);
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

		// only affects physical damage
		if(u_caster->IsPacified() && GetProto()->School == SCHOOL_NORMAL)
		{
			// HACK FIX
			switch(GetProto()->NameHash)
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				case SPELL_HASH_WILL_OF_THE_FORSAKEN: //Will of the Forsaken
				case SPELL_HASH_EVERY_MAN_FOR_HIMSELF: // Every Man for Himself
					{
						if(u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP))
							break;
					}
					break;

				default:
					return SPELL_FAILED_PACIFIED;
			}
		}

		/**
		 *	Stun check
		 */
		if(u_caster->IsStunned() && (GetProto()->AttributesExE & FLAGS6_USABLE_WHILE_STUNNED) == 0)
			return SPELL_FAILED_STUNNED;

		/**
		 *	Fear check
		 */
		if(u_caster->IsFeared()  && (GetProto()->AttributesExE & FLAGS6_USABLE_WHILE_FEARED) == 0)
			return SPELL_FAILED_FLEEING;

		/**
		 *	Confuse check
		 */
		if(u_caster->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED) && (GetProto()->AttributesExE & FLAGS6_USABLE_WHILE_CONFUSED) == 0)
			return SPELL_FAILED_CONFUSED;


		if(u_caster->GetChannelSpellTargetGUID() != 0)
		{
			SpellEntry* t_spellInfo = (u_caster->GetCurrentSpell() ? u_caster->GetCurrentSpell()->GetProto() : NULL);

			if(!t_spellInfo || !m_triggeredSpell)
				return SPELL_FAILED_SPELL_IN_PROGRESS;
			else if(t_spellInfo)
			{
				if(t_spellInfo->EffectTriggerSpell[0] != GetProto()->Id &&
				        t_spellInfo->EffectTriggerSpell[1] != GetProto()->Id &&
				        t_spellInfo->EffectTriggerSpell[2] != GetProto()->Id)
				{
					return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}
		}
	}

	/**
	 * Dead pet check
	*/
	if(GetProto()->AttributesExB & ATTRIBUTESEXB_REQ_DEAD_PET && p_caster != NULL)
	{
		Pet* pPet = p_caster->GetSummon();
		if(pPet != NULL && !pPet->IsDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;
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
		if(i_caster->GetStackCount() > 1)
		{
			i_caster->ModStackCount(-1);
			i_caster->m_isDirty = true;
			i_caster = NULL;
		}
		else
		{
			for(uint32 x = 0; x < 5; x++)
			{
				int32 charges = static_cast< int32 >(i_caster->GetCharges(x));

				if(charges == 0)
					continue;

				bool Removable = false;

				// Items with negative charges are items that disappear when they reach 0 charge.
				if(charges < 0)
					Removable = true;

				i_caster->m_isDirty = true;

				if(Removable)
				{

					// If we have only 1 charge left, it's pointless to decrease the charge, we will have to remove the item anyways, so who cares ^^
					if(charges == -1)
					{
						i_caster->GetOwner()->GetItemInterface()->SafeFullRemoveItemByGuid(i_caster->GetGUID());
					}
					else
					{
						i_caster->ModCharges(x, 1);
					}

				}
				else
				{
					i_caster->ModCharges(x, -1);
				}

				i_caster = NULL;
				break;
			}
		}
	}
	// Ammo Removal
	if(hasAttributeExB(ATTRIBUTESEXB_REQ_RANGED_WEAPON) || hasAttributeExC(FLAGS4_PLAYER_RANGED_SPELLS))
	{
		if(p_caster && !p_caster->m_requiresNoAmmo)
			p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(p_caster->GetUInt32Value(PLAYER_AMMO_ID), 1, &i_caster);
	}

	// Reagent Removal
	if(!(p_caster->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NO_REAGANT_COST) && hasAttributeExD(FLAGS6_REAGENT_REMOVAL)))
	{
		for(uint32 i = 0; i < 8 ; i++)
		{
			if(GetProto()->Reagent[i])
			{
				p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(GetProto()->Reagent[i], GetProto()->ReagentCount[i], &i_caster);
			}
		}
	}
}

int32 Spell::CalculateEffect(uint32 i, Unit* target)
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

	/* Random suffix value calculation */
	if(i_caster && (int32(i_caster->GetItemRandomPropertyId()) < 0))
	{
		ItemRandomSuffixEntry* si = dbcItemRandomSuffix.LookupEntry(abs(int(i_caster->GetItemRandomPropertyId())));
		EnchantEntry* ent;
		uint32 j, k;

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

	float basePointsPerLevel    = GetProto()->EffectRealPointsPerLevel[i];
	//float randomPointsPerLevel  = GetProto()->EffectDicePerLevel[i];
	int32 basePoints;
	if(m_overrideBasePoints)
		basePoints = m_overridenBasePoints[i];
	else
		basePoints = GetProto()->EffectBasePoints[i] + 1;
	int32 randomPoints = GetProto()->EffectDieSides[i];

	//added by Zack : some talents inherit their basepoints from the previously cast spell: see mage - Master of Elements
	if(forced_basepoints[i])
		basePoints = forced_basepoints[i];

	if(u_caster != NULL)
	{
		int32 diff = -(int32)GetProto()->baseLevel;
		if(GetProto()->maxLevel && u_caster->getLevel() > GetProto()->maxLevel)
			diff += GetProto()->maxLevel;
		else
			diff += u_caster->getLevel();
		//randomPoints += float2int32(diff * randomPointsPerLevel);
		basePoints += float2int32(diff * basePointsPerLevel);
	}

	if(randomPoints <= 1)
		value = basePoints;
	else
		value = basePoints + rand() % randomPoints;

	int32 comboDamage = (int32)GetProto()->EffectPointsPerComboPoint[i];
	if(comboDamage && p_caster != NULL)
	{
		m_requiresCP = true;
		value += (comboDamage * p_caster->m_comboPoints);
		//this is ugly so i will explain the case maybe someone ha a better idea :
		// while casting a spell talent will trigger upon the spell prepare faze
		// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
		p_caster->m_spellcomboPoints = 0;
	}

	value = DoCalculateEffect(i, target, value);

	if(p_caster != NULL)
	{
		SpellOverrideMap::iterator itr = p_caster->mSpellOverrideMap.find(GetProto()->Id);
		if(itr != p_caster->mSpellOverrideMap.end())
		{
			ScriptOverrideList::iterator itrSO;
			for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
			{
				value += RandomUInt((*itrSO)->damage);
			}
		}
	}

	// TODO: INHERIT ITEM MODS FROM REAL ITEM OWNER - BURLEX BUT DO IT PROPERLY

	if(u_caster != NULL)
	{
		int32 spell_flat_modifers = 0;
		int32 spell_pct_modifers = 100;

		SM_FIValue(u_caster->SM_FMiscEffect, &spell_flat_modifers, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PMiscEffect, &spell_pct_modifers, GetProto()->SpellGroupType);

		SM_FIValue(u_caster->SM_FEffectBonus, &spell_flat_modifers, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PEffectBonus, &spell_pct_modifers, GetProto()->SpellGroupType);

		SM_FIValue(u_caster->SM_FDamageBonus, &spell_flat_modifers, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PDamageBonus, &spell_pct_modifers , GetProto()->SpellGroupType);

		switch(i)
		{
			case 0:
				SM_FIValue(u_caster->SM_FEffect1_Bonus, &spell_flat_modifers, GetProto()->SpellGroupType);
				SM_PIValue(u_caster->SM_PEffect1_Bonus, &spell_pct_modifers, GetProto()->SpellGroupType);
				break;
			case 1:
				SM_FIValue(u_caster->SM_FEffect2_Bonus, &spell_flat_modifers, GetProto()->SpellGroupType);
				SM_PIValue(u_caster->SM_PEffect2_Bonus, &spell_pct_modifers, GetProto()->SpellGroupType);
				break;
			case 2:
				SM_FIValue(u_caster->SM_FEffect3_Bonus, &spell_flat_modifers, GetProto()->SpellGroupType);
				SM_PIValue(u_caster->SM_PEffect3_Bonus, &spell_pct_modifers, GetProto()->SpellGroupType);
				break;
		}

		value = float2int32(value * (float)(spell_pct_modifers / 100.0f)) + spell_flat_modifers;
	}
	else if(i_caster != NULL && target != NULL)
	{
		//we should inherit the modifiers from the conjured food caster
		Unit* item_creator = target->GetMapMgr()->GetUnit(i_caster->GetCreatorGUID());

		if(item_creator != NULL)
		{
			int32 spell_flat_modifers = 0;
			int32 spell_pct_modifers = 100;

			SM_FIValue(item_creator->SM_FMiscEffect , &spell_flat_modifers, GetProto()->SpellGroupType);
			SM_PIValue(item_creator->SM_PMiscEffect, &spell_pct_modifers, GetProto()->SpellGroupType);

			SM_FIValue(item_creator->SM_FEffectBonus, &spell_flat_modifers, GetProto()->SpellGroupType);
			SM_PIValue(item_creator->SM_PEffectBonus, &spell_pct_modifers, GetProto()->SpellGroupType);

			value = float2int32(value * (float)(spell_pct_modifers / 100.0f)) + spell_flat_modifers;
		}
	}

	return value;
}

int32 Spell::DoCalculateEffect(uint32 i, Unit* target, int32 value)
{
	//2 switch: the first checking namehash, the second checking spell id. If the spell is still not handled in these 2 blocks of code,
	//3rd block of checks is reached. bool handled is initialized as true and set to false in the default: case of each switch.
	bool handled = true;

	switch(GetProto()->NameHash)
	{
			// Causes Weapon Damage + Ammo + RAP * 0.1 + EffectBasePoints[0] and additional EffectBasePoints[1] if the target is dazed
		case SPELL_HASH_STEADY_SHOT:
			{
				if(u_caster != NULL && i == 0)
				{
					if(p_caster != NULL)
					{
						Item* it;
						it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
						if(it)
						{
							float weapondmg = RandomFloat(1) * (it->GetProto()->Damage[0].Max - it->GetProto()->Damage[0].Min) + it->GetProto()->Damage[0].Min;
							value += float2int32(GetProto()->EffectBasePoints[0] + weapondmg / (it->GetProto()->Delay / 1000.0f) * 2.8f);
						}
					}
					if(target && target->IsDazed())
						value += GetProto()->EffectBasePoints[1];
					value += (uint32)(u_caster->GetRAP() * 0.1);
				}
				break;
			}
		case SPELL_HASH_REND:
			{
				if(p_caster != NULL)
				{
					Item* it;
					it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(it)
					{
						if(it->GetProto()->Class == 2)
						{
							float avgwepdmg = (it->GetProto()->Damage[0].Min + it->GetProto()->Damage[0].Max) * 0.5f;
							float wepspd = (it->GetProto()->Delay * 0.001f);
							int32 dmg = float2int32((avgwepdmg) + p_caster->GetAP() / 14 * wepspd);

							if(target && target->GetHealthPct() > 75)
							{
								dmg = float2int32(dmg + dmg * 0.35f);
							}

							value += dmg / 5;
						}
					}
				}
				break;
			}
		case SPELL_HASH_SLAM:
			{
				if(p_caster != NULL)
				{
					Item* mainHand;
					mainHand = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					float avgWeaponDmg = (mainHand->GetProto()->Damage[0].Max + mainHand->GetProto()->Damage[0].Min) / 2;
					value += float2int32((GetProto()->EffectBasePoints[0] + 1) + avgWeaponDmg);
				}
				break;
			}
		case SPELL_HASH_EVISCERATE:
			{
				if(p_caster != NULL)
					value += (uint32)(p_caster->GetAP() * 0.03f * p_caster->m_comboPoints);
				break;
			}
		case SPELL_HASH_FEROCIOUS_BITE:
			{
				if(p_caster != NULL)
				{
					value += (uint32)((p_caster->GetAP() * 0.1526f) + (p_caster->GetPower(POWER_TYPE_ENERGY) * GetProto()->dmg_multiplier[i]));
					p_caster->SetPower(POWER_TYPE_ENERGY, 0);
				}
				break;
			}
		case SPELL_HASH_VICTORY_RUSH:
			{
				//causing ${$AP*$m1/100} damage
				if(u_caster != NULL && i == 0)
					value = (value * u_caster->GetAP()) / 100;
				break;
			}
		case SPELL_HASH_RAKE:
			{
				//Rake the target for ${$AP/100+$m1} bleed damage and an additional ${$m2*3+$AP*0.06} damage over $d.
				if(u_caster != NULL)
				{
					float ap = float(u_caster->GetAP());
					if(i == 0)
						value += float2int32(ceilf(ap * 0.01f));	// / 100
					else if(i == 1)
						value += float2int32(ap * 0.06f);
				}
				break;
			}
		case SPELL_HASH_GARROTE:
			{
				// WoWWiki says +( 0.18 * attack power / number of ticks )
				// Tooltip gives no specific reading, but says ", increased by your attack power.".
				if(u_caster != NULL && i == 0)
					value += (uint32) ceilf((u_caster->GetAP() * 0.07f) / 6);
				break;
			}
		case SPELL_HASH_RUPTURE:
			{
				/*
				1pt = Attack Power * 0.04 + x
				2pt = Attack Power * 0.10 + y
				3pt = Attack Power * 0.18 + z
				4pt = Attack Power * 0.21 + a
				5pt = Attack Power * 0.24 + b
				*/
				if(p_caster != NULL && i == 0)
				{
					int8 cp = p_caster->m_comboPoints;
					value += (uint32) ceilf((u_caster->GetAP() * 0.04f * cp) / ((6 + (cp << 1)) >> 1));
				}
				break;
			}
		case SPELL_HASH_RIP:
			{
				if(p_caster != NULL)
					value += float2int32(p_caster->GetAP() * 0.01f * p_caster->m_comboPoints);
				break;
			}
		case SPELL_HASH_MONGOOSE_BITE:
			{
				// ${$AP*0.2+$m1} damage.
				if(u_caster != NULL)
					value += u_caster->GetAP() / 5;
				break;
			}
		case SPELL_HASH_SWIPE:
			{
				// ${$AP*0.06+$m1} damage.
				if(u_caster != NULL)
					value += float2int32(u_caster->GetAP() * 0.06f);
				break;
			}
		case SPELL_HASH_HAMMER_OF_THE_RIGHTEOUS:
			{
				if(p_caster != NULL)
					//4x 1h weapon-dps ->  4*(mindmg+maxdmg)/speed/2 = 2*(mindmg+maxdmg)/speed
					value = float2int32((p_caster->GetMinDamage() + p_caster->GetMaxDamage()) / (float(p_caster->GetBaseAttackTime(MELEE)) / 1000.0f)) << 1;
				break;
			}
		case SPELL_HASH_BACKSTAB:  // Egari: spell 31220 is interfering with combopoints
			{
				if(i == 2)
					return GetProto()->EffectBasePoints[i] + 1;
				break;
			}
		case SPELL_HASH_GOUGE:
			{
				if(u_caster != NULL && i == 0)
						value += (uint32)ceilf(u_caster->GetAP() * 0.21f);
				break;
			}
		case SPELL_HASH_FAN_OF_KNIVES:  // rogue - fan of knives
			{
				if(p_caster != NULL)
				{
					Item* mit = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(mit != NULL)
					{
						if(mit->GetProto()->Class == 2 && mit->GetProto()->SubClass == 15)   // daggers
							value = 105;
					}
				}
				break;
			}
		case SPELL_HASH_VAMPIRIC_EMBRACE:
			{
				value = value * (GetProto()->EffectBasePoints[i] + 1) / 100;
				if(p_caster != NULL)
				{
					SM_FIValue(p_caster->SM_FMiscEffect, &value, GetProto()->SpellGroupType);
					SM_PIValue(p_caster->SM_PMiscEffect, &value, GetProto()->SpellGroupType);
				}
				break;
			}
		case SPELL_HASH_SEAL_OF_RIGHTEOUSNESS:
			{
				if(p_caster != NULL)
				{
					Item* mit = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(mit != NULL)
						value = (p_caster->GetAP() * 22 + p_caster->GetPosDamageDoneMod(SCHOOL_HOLY) * 44) * mit->GetProto()->Delay / 1000000;
				}
				break;
			}
		case SPELL_HASH_BLOOD_CORRUPTION:
		case SPELL_HASH_HOLY_VENGEANCE:
			{
				if(p_caster != NULL)
					value = (p_caster->GetAP() * 25 + p_caster->GetPosDamageDoneMod(SCHOOL_HOLY) * 13) / 1000;
				break;
			}
		case SPELL_HASH_JUDGEMENT_OF_LIGHT:
			{
				if(u_caster != NULL)
					value = u_caster->GetMaxHealth() * 2 / 100;
				break;
			}
		case SPELL_HASH_JUDGEMENT_OF_WISDOM:
			{
				if(u_caster != NULL)
					value = u_caster->GetBaseMana() * 2 / 100;
				break;
			}
		case SPELL_HASH_JUDGEMENT:
			{
				if(p_caster != NULL)
					value += (p_caster->GetAP() * 16 + p_caster->GetPosDamageDoneMod(SCHOOL_HOLY) * 25) / 100;
				break;
			}
		case SPELL_HASH_JUDGEMENT_OF_RIGHTEOUSNESS:
			{
				if(p_caster != NULL)
					value += (p_caster->GetAP() * 2 + p_caster->GetPosDamageDoneMod(SCHOOL_HOLY) * 32) / 100;
				break;
			}
		case SPELL_HASH_JUDGEMENT_OF_VENGEANCE:
		case SPELL_HASH_JUDGEMENT_OF_CORRUPTION:
			{
				if(p_caster != NULL)
					value += (p_caster->GetAP() * 14 + p_caster->GetPosDamageDoneMod(SCHOOL_HOLY) * 22) / 100;
				break;
			}
		case SPELL_HASH_ENVENOM:
			{
				if(p_caster != NULL && i == 0)
				{
					value *= p_caster->m_comboPoints;
					value += (uint32)(p_caster->GetAP() * (0.09f * p_caster->m_comboPoints));
					m_requiresCP = true;
				}
				break;
			}
		default:
			{
				//not handled in this switch
				handled = false;
				break;
			}
	}

	if(!handled)
	{
		//it will be set to false in the default case of the switch.
		handled = true;
		switch(GetProto()->Id)
		{
			case 34123:  //Druid - Tree of Life
				{
					if(p_caster != NULL && i == 0)
						//Heal is increased by 6%
						value = float2int32(value * 1.06f);
					break;
				}
			case 57669: //Replenishment
			case 61782:
				{
					if(p_caster != NULL && i == 0 && target != NULL)
						value = int32(0.002 * target->GetMaxPower(POWER_TYPE_MANA));
					break;
				}
			default:
				{
					//not handled in this switch
					handled = false;
					break;
				}
		}

		if(!handled)
		{
			if(GetProto()->c_is_flags & SPELL_FLAG_IS_POISON && u_caster != NULL)   // poison damage modifier
			{
				switch(GetProto()->NameHash)
				{
					case SPELL_HASH_DEADLY_POISON_IX:
					case SPELL_HASH_DEADLY_POISON_VIII:
					case SPELL_HASH_DEADLY_POISON_VII:
					case SPELL_HASH_DEADLY_POISON_VI:
					case SPELL_HASH_DEADLY_POISON_V:
					case SPELL_HASH_DEADLY_POISON_IV:
					case SPELL_HASH_DEADLY_POISON_III:
					case SPELL_HASH_DEADLY_POISON_II:
					case SPELL_HASH_DEADLY_POISON:
						if(GetProto()->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE)
							value += float2int32(u_caster->GetAP() * 0.03f);
						break;
					case SPELL_HASH_INSTANT_POISON_IX:
					case SPELL_HASH_INSTANT_POISON_VIII:
					case SPELL_HASH_INSTANT_POISON_VII:
					case SPELL_HASH_INSTANT_POISON_VI:
					case SPELL_HASH_INSTANT_POISON_V:
					case SPELL_HASH_INSTANT_POISON_IV:
					case SPELL_HASH_INSTANT_POISON_III:
					case SPELL_HASH_INSTANT_POISON_II:
					case SPELL_HASH_INSTANT_POISON:
						if(GetProto()->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE)
							value += float2int32(u_caster->GetAP() * 0.10f);
						break;
					case SPELL_HASH_WOUND_POISON_VII:
					case SPELL_HASH_WOUND_POISON_VI:
					case SPELL_HASH_WOUND_POISON_V:
					case SPELL_HASH_WOUND_POISON_IV:
					case SPELL_HASH_WOUND_POISON_III:
					case SPELL_HASH_WOUND_POISON_II:
					case SPELL_HASH_WOUND_POISON:
						if(GetProto()->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE)
							value += float2int32(u_caster->GetAP() * 0.04f);
						break;
				}
			}
		}
	}

	return value;
}

void Spell::HandleTeleport(float x, float y, float z, uint32 mapid, Unit* Target)
{
	if(Target->IsPlayer())
	{

		Player* pTarget = TO< Player* >(Target);
		pTarget->EventAttackStop();
		pTarget->SetSelection(0);

		// We use a teleport event on this one. Reason being because of UpdateCellActivity,
		// the game object set of the updater thread WILL Get messed up if we teleport from a gameobject
		// caster.

		if(!sEventMgr.HasEvent(pTarget, EVENT_PLAYER_TELEPORT))
		{
			sEventMgr.AddEvent(pTarget, &Player::EventTeleport, mapid, x, y, z, EVENT_PLAYER_TELEPORT, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}

	}
	else
	{
		if(mapid != Target->GetMapId())
		{
			LOG_ERROR("Tried to teleport a Creature to another map.");
			return;
		}

		WorldPacket data(SMSG_MONSTER_MOVE, 50);

		data << Target->GetNewGUID();
		data << uint8(0);
		data << Target->GetPositionX();
		data << Target->GetPositionY();
		data << Target->GetPositionZ();
		data << getMSTime();
		data << uint8(0x00);
		data << uint32(256);
		data << uint32(1);
		data << uint32(1);
		data << float(x);
		data << float(y);
		data << float(z);

		Target->SendMessageToSet(&data, true);
		Target->SetPosition(x, y, z, 0.5f);   // need correct orentation
	}
}

void Spell::CreateItem(uint32 itemId)
{
	/// Creates number of items equal to a "damage" of the effect
	if(itemId == 0 || p_caster == NULL)
		return;

	p_caster->GetItemInterface()->AddItemById(itemId, damage, 0);
}

void Spell::SendHealSpellOnPlayer(Object* caster, Object* target, uint32 healed, bool critical, uint32 overhealed, uint32 spellid, uint32 absorbed)
{
	if(caster == NULL || target == NULL || !target->IsPlayer())
		return;

	WorldPacket data(SMSG_SPELLHEALLOG, 33);
	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << healed;
	data << overhealed;
	data << absorbed;
	data << uint8(critical);

	caster->SendMessageToSet(&data, true);
}

void Spell::SendHealManaSpellOnPlayer(Object* caster, Object* target, uint32 dmg, uint32 powertype, uint32 spellid)
{
	if(caster == NULL || target == NULL || !target->IsPlayer())
		return;

	WorldPacket data(SMSG_SPELLENERGIZELOG, 30);

	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << powertype;
	data << dmg;

	caster->SendMessageToSet(&data, true);
}

void Spell::Heal(int32 amount, bool ForceCrit)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	if(p_caster != NULL)
		p_caster->last_heal_spell = GetProto();

	//self healing shouldn't flag himself
	if(p_caster != NULL && playerTarget != NULL && p_caster != playerTarget)
	{
		// Healing a flagged target will flag you.
		if(playerTarget->IsPvPFlagged())
		{
			if(!p_caster->IsPvPFlagged())
				p_caster->PvPToggle();
			else
				p_caster->SetPvPFlag();
		}
	}

	//Make it critical
	bool critical = false;
	int32 critchance = 0;
	int32 bonus = 0;
	uint32 school = GetProto()->School;

	if(u_caster != NULL && !(GetProto()->AttributesExC & FLAGS4_NO_HEALING_BONUS))
	{
		//Basic bonus
		if(p_caster == NULL ||
		        !(p_caster->getClass() == ROGUE || p_caster->getClass() == WARRIOR || p_caster->getClass() == HUNTER || p_caster->getClass() == DEATHKNIGHT))
			bonus += u_caster->HealDoneMod[school];

		bonus += unitTarget->HealTakenMod[school];

		//Bonus from Intellect & Spirit
		if(p_caster != NULL)
		{
			for(uint8 a = 0; a < 5; a++)
				bonus += float2int32(p_caster->SpellHealDoneByAttribute[a][school] * p_caster->GetStat(a));
		}

		//Spell Coefficient
		if(GetProto()->Dspell_coef_override >= 0)    //In case we have forced coefficients
			bonus = float2int32(bonus * GetProto()->Dspell_coef_override);
		else
		{
			//Bonus to DH part
			if(GetProto()->fixed_dddhcoef >= 0)
				bonus = float2int32(bonus * GetProto()->fixed_dddhcoef);
		}

		critchance = float2int32(u_caster->spellcritperc + u_caster->SpellCritChanceSchool[school]);

		//Sacred Shield
		if(unitTarget->HasAurasWithNameHash(SPELL_HASH_SACRED_SHIELD) && m_spellInfo->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			critchance += 50;

		if(GetProto()->SpellGroupType)
		{
			int penalty_pct = 0;
			int penalty_flt = 0;
			SM_FIValue(u_caster->SM_PPenalty, &penalty_pct, GetProto()->SpellGroupType);
			bonus += amount * penalty_pct / 100;
			SM_FIValue(u_caster->SM_FPenalty, &penalty_flt, GetProto()->SpellGroupType);
			bonus += penalty_flt;
			SM_FIValue(u_caster->SM_CriticalChance, &critchance, GetProto()->SpellGroupType);
		}

		if(p_caster != NULL)
		{
			if(m_spellInfo->NameHash == SPELL_HASH_LESSER_HEALING_WAVE || m_spellInfo->NameHash == SPELL_HASH_HEALING_WAVE)
			{
				//Tidal Waves
				p_caster->RemoveAura(53390, p_caster->GetGUID());
			}

			if(m_spellInfo->NameHash == SPELL_HASH_LESSER_HEALING_WAVE ||
			        m_spellInfo->NameHash == SPELL_HASH_HEALING_WAVE ||
			        m_spellInfo->NameHash == SPELL_HASH_CHAIN_HEAL)
			{
				//Maelstrom Weapon
				p_caster->RemoveAllAuras(53817, p_caster->GetGUID());
			}
		}

		switch(m_spellInfo->Id)
		{
			case 54172: //Paladin - Divine Storm heal effect
				{
					int dmg = (int)CalculateDamage(u_caster, unitTarget, MELEE, 0, dbcSpell.LookupEntry(53385));    //1 hit
					int target = 0;
					uint8 did_hit_result;
					std::set<Object*>::iterator itr, itr2;

					for(itr2 = u_caster->GetInRangeSetBegin(); itr2 != u_caster->GetInRangeSetEnd();)
					{
						itr = itr2;
						itr2++;
						if((*itr)->IsUnit() && TO< Unit* >(*itr)->isAlive() && IsInrange(u_caster, (*itr), 8) && (u_caster->GetPhase() & (*itr)->GetPhase()))
						{
							did_hit_result = DidHit(dbcSpell.LookupEntry(53385)->Effect[0], TO< Unit* >(*itr));
							if(did_hit_result == SPELL_DID_HIT_SUCCESS)
								target++;
						}
					}
					if(target > 4)
						target = 4;

					amount = (dmg * target) >> 2;   // 25%
				}
				break;
		}

		amount += bonus;
		amount += amount * (int32)(u_caster->HealDonePctMod[ school ]);
		amount += float2int32(amount * unitTarget->HealTakenPctMod[ school ]);

		if(GetProto()->SpellGroupType)
			SM_PIValue(u_caster->SM_PDamageBonus, &amount, GetProto()->SpellGroupType);

		if(ForceCrit || ((critical = Rand(critchance)) != 0))
		{
			int32 critical_bonus = 100;
			if(GetProto()->SpellGroupType)
				SM_FIValue(u_caster->SM_PCriticalDamage, &critical_bonus, GetProto()->SpellGroupType);

			if(critical_bonus > 0)
			{
				// the bonuses are halved by 50% (funky blizzard math :S)
				float b = (critical_bonus / 2.0f) / 100.0f;
				amount += float2int32(amount * b);
			}

			unitTarget->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, u_caster, GetProto(), false, amount);
			u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT, unitTarget, GetProto(), false, amount);
		}
	}

	if(amount < 0)
		amount = 0;

	uint32 overheal = 0;
	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 maxHealth = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if((curHealth + amount) >= maxHealth)
	{
		unitTarget->SetHealth(maxHealth);
		overheal = curHealth + amount - maxHealth;
	}
	else
		unitTarget->ModHealth(amount);

	SendHealSpellOnPlayer(m_caster, unitTarget, amount, critical, overheal, pSpellId ? pSpellId : GetProto()->Id);

	if(p_caster != NULL)
	{
		p_caster->m_bgScore.HealingDone += amount;
		if(p_caster->m_bg != NULL)
			p_caster->m_bg->UpdatePvPData();
	}

	if(p_caster != NULL)
	{
		p_caster->m_casted_amount[ school ] = amount;
		p_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, GetProto());
	}

	unitTarget->RemoveAurasByHeal();

	// add threat
	if(u_caster != NULL)
	{
		std::vector<Unit*> target_threat;
		int count = 0;
		Creature* tmp_creature;
		for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
		{
			if(!(*itr)->IsCreature())
				continue;

			tmp_creature = TO_CREATURE(*itr);

			if(!tmp_creature->CombatStatus.IsInCombat() || (tmp_creature->GetAIInterface()->getThreatByPtr(u_caster) == 0 && tmp_creature->GetAIInterface()->getThreatByPtr(unitTarget) == 0))
				continue;

			if(!(u_caster->GetPhase() & (*itr)->GetPhase()))     //Can't see, can't be a threat
				continue;

			target_threat.push_back(tmp_creature);
			count++;
		}
		if(count == 0)
			return;

		amount = amount / count;

		for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
		{
			(*itr)->GetAIInterface()->HealReaction(u_caster, unitTarget, m_spellInfo, amount);
		}

		// remember that we healed (for combat status)
		if(unitTarget->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(unitTarget);
	}
}

void Spell::DetermineSkillUp(uint32 skillid, uint32 targetlevel, uint32 multiplicator)
{
	if(p_caster == NULL)
		return;

	if(p_caster->GetSkillUpChance(skillid) < 0.01)
		return;//to prevent getting higher skill than max

	int32 diff = p_caster->_GetSkillLineCurrent(skillid, false) / 5 - targetlevel;

	if(diff < 0)
		diff = - diff;

	float chance;
	if(diff <= 5)
		chance = 95.0f;
	else if(diff <= 10)
		chance = 66.0f;
	else if(diff <= 15)
		chance = 33.0f;
	else
		return;

	if(multiplicator == 0)
		multiplicator = 1;

	if(Rand((chance * sWorld.getRate(RATE_SKILLCHANCE)) * multiplicator))
	{
		p_caster->_AdvanceSkillLine(skillid, float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE)));

		uint32 value = p_caster->_GetSkillLineCurrent(skillid, true);
		uint32 spellid = 0;

		// Lifeblood
		if(skillid == SKILL_HERBALISM)
		{
			switch(value)
			{
				case 75:
					{	spellid = 55428; }
					break;// Rank 1
				case 150:
					{	spellid = 55480; }
					break;// Rank 2
				case 225:
					{	spellid = 55500; }
					break;// Rank 3
				case 300:
					{	spellid = 55501; }
					break;// Rank 4
				case 375:
					{	spellid = 55502; }
					break;// Rank 5
				case 450:
					{	spellid = 55503; }
					break;// Rank 6
			}
		}

		// Toughness
		else if(skillid == SKILL_MINING)
		{
			switch(value)
			{
				case 75:
					{	spellid = 53120; }
					break;// Rank 1
				case 150:
					{	spellid = 53121; }
					break;// Rank 2
				case 225:
					{	spellid = 53122; }
					break;// Rank 3
				case 300:
					{	spellid = 53123; }
					break;// Rank 4
				case 375:
					{	spellid = 53124; }
					break;// Rank 5
				case 450:
					{	spellid = 53040; }
					break;// Rank 6
			}
		}


		// Master of Anatomy
		else if(skillid == SKILL_SKINNING)
		{
			switch(value)
			{
				case 75:
					{	spellid = 53125;}
					break;// Rank 1
				case 150:
					{	spellid = 53662;}
					break;// Rank 2
				case 225:
					{	spellid = 53663;}
					break;// Rank 3
				case 300:
					{	spellid = 53664;}
					break;// Rank 4
				case 375:
					{	spellid = 53665;}
					break;// Rank 5
				case 450:
					{	spellid = 53666;}
					break;// Rank 6
			}
		}

		if(spellid != 0)
			p_caster->addSpell(spellid);
	}
}

void Spell::DetermineSkillUp(uint32 skillid)
{
	//This code is wrong for creating items and disenchanting.
	if(p_caster == NULL)
		return;
	float chance = 0.0f;
	skilllinespell* skill = objmgr.GetSpellSkill(GetProto()->Id);
	if(skill != NULL && skillid == skill->skilline && p_caster->_HasSkillLine(skillid))
	{
		uint32 amt = p_caster->_GetSkillLineCurrent(skillid, false);
		uint32 max = p_caster->_GetSkillLineMax(skillid);
		if(amt >= max)
			return;
		if(amt >= skill->grey)   //grey
			chance = 0.0f;
		else if((amt >= (((skill->grey - skill->green) / 2) + skill->green)))          //green
			chance = 33.0f;
		else if(amt >= skill->green)   //yellow
			chance = 66.0f;
		else //brown
			chance = 100.0f;
	}
	if(Rand(chance * sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skillid, float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

void Spell::SafeAddTarget(TargetsList* tgt, uint64 guid)
{
	if(guid == 0)
		return;

	for(TargetsList::iterator i = tgt->begin(); i != tgt->end(); ++i)
	{
		if(*i == guid)
		{
			return;
		}
	}

	tgt->push_back(guid);
}

void Spell::SafeAddMissedTarget(uint64 guid)
{
	for(SpellTargetsList::iterator i = ModeratedTargets.begin(); i != ModeratedTargets.end(); ++i)
		if((*i).TargetGuid == guid)
		{
			//LOG_DEBUG("[SPELL] Something goes wrong in spell target system");
			// this isn't actually wrong, since we only have one missed target map,
			// whereas hit targets have multiple maps per effect.
			return;
		}

	ModeratedTargets.push_back(SpellTargetMod(guid, 2));
}

void Spell::SafeAddModeratedTarget(uint64 guid, uint16 type)
{
	for(SpellTargetsList::iterator i = ModeratedTargets.begin(); i != ModeratedTargets.end(); ++i)
		if((*i).TargetGuid == guid)
		{
			//LOG_DEBUG("[SPELL] Something goes wrong in spell target system");
			// this isn't actually wrong, since we only have one missed target map,
			// whereas hit targets have multiple maps per effect.
			return;
		}

	ModeratedTargets.push_back(SpellTargetMod(guid, (uint8)type));
}

bool Spell::Reflect(Unit* refunit)
{
	SpellEntry* refspell = NULL;
	bool canreflect = false;

	if(m_reflectedParent != NULL || refunit == NULL || m_caster == refunit)
		return false;

	// if the spell to reflect is a reflect spell, do nothing.
	for(uint8 i = 0; i < 3; i++)
	{
		if(GetProto()->Effect[i] == SPELL_EFFECT_APPLY_AURA && (GetProto()->EffectApplyAuraName[i] == SPELL_AURA_REFLECT_SPELLS_SCHOOL ||
		        GetProto()->EffectApplyAuraName[i] == SPELL_AURA_REFLECT_SPELLS))
			return false;
	}

	for(std::list<struct ReflectSpellSchool*>::iterator i = refunit->m_reflectSpellSchool.begin(); i != refunit->m_reflectSpellSchool.end(); ++i)
	{
		if((*i)->school == -1 || (*i)->school == (int32)GetProto()->School)
		{
			if(Rand((float)(*i)->chance))
			{
				//the god blessed special case : mage - Frost Warding = is an augmentation to frost warding
				if((*i)->require_aura_hash && !refunit->HasAurasWithNameHash((*i)->require_aura_hash))
					continue;

				if((*i)->infront == true)
				{
					if(m_caster->isInFront(refunit))
					{
						canreflect = true;
					}
				}
				else
					canreflect = true;

				if((*i)->charges > 0)
				{
					(*i)->charges--;
					if((*i)->charges <= 0)
					{
						if(!refunit->RemoveAura((*i)->spellId))	// should delete + erase RSS too, if unit hasn't such an aura...
						{
							delete *i;								// ...do it manually
							refunit->m_reflectSpellSchool.erase(i);
						}
					}
				}

				refspell = GetProto();
				break;
			}
		}
	}

	if(!refspell || !canreflect)
		return false;

	Spell* spell = sSpellFactoryMgr.NewSpell(refunit, refspell, true, NULL);
	spell->SetReflected();
	SpellCastTargets targets;
	targets.m_unitTarget = m_caster->GetGUID();
	spell->prepare(&targets);

	return true;
}

void ApplyDiminishingReturnTimer(uint32* Duration, Unit* Target, SpellEntry* spell)
{
	uint32 status = spell->DiminishStatus;
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF)
		return;

	// Check if we don't apply to pve
	if(!PvE && !Target->IsPlayer() && !Target->IsPet())
		return;

	// TODO: check for spells that should do this
	uint32 Dur = *Duration;
	uint32 count = Target->m_diminishCount[Grp];

	if(count > 2)   // Target immune to spell
	{
		*Duration = 0;
		return;
	}
	else
	{
		Dur >>= count; //100%, 50%, 25% bitwise
		if((Target->IsPlayer() || Target->IsPet()) && Dur > uint32(10000 >> count))
		{
			Dur = 10000 >> count;
			if(status == DIMINISHING_GROUP_NOT_DIMINISHED)
			{
				*Duration = Dur;
				return;
			}
		}
	}

	*Duration = Dur;

	// Reset the diminishing return counter, and add to the aura count (we don't decrease the timer till we
	// have no auras of this type left.
	//++Target->m_diminishAuraCount[Grp];
	++Target->m_diminishCount[Grp];
}

void UnapplyDiminishingReturnTimer(Unit* Target, SpellEntry* spell)
{
	uint32 status = spell->DiminishStatus;
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;
	uint32 aura_grp;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && !Target->IsPlayer() && !Target->IsPet())
		return;

	//Target->m_diminishAuraCount[Grp]--;

	/*There are cases in which you just refresh an aura duration instead of the whole aura,
	causing corruption on the diminishAura counter and locking the entire diminishing group.
	So it's better to check the active auras one by one*/
	Target->m_diminishAuraCount[Grp] = 0;
	for(uint32 x = MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; x++)
	{
		if(Target->m_auras[x])
		{
			aura_grp = Target->m_auras[x]->GetSpellProto()->DiminishStatus;
			if(aura_grp == status)
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
		case SPELL_HASH_INTERCEPT:
		case SPELL_HASH_CONCUSSION_BLOW:
		case SPELL_HASH_INTIMIDATION:
		case SPELL_HASH_WAR_STOMP:
		case SPELL_HASH_POUNCE:
		case SPELL_HASH_HAMMER_OF_JUSTICE:
			{
				grp = DIMINISHING_GROUP_STUN;
				pve = true;
			}
			break;

		case SPELL_HASH_STARFIRE_STUN:
		case SPELL_HASH_STONECLAW_STUN:
		case SPELL_HASH_STUN:					// Generic ones
			{
				grp = DIMINISHING_GROUP_STUN_PROC;
				pve = true;
			}
			break;

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
			}
			break;

		case SPELL_HASH_GOUGE:
		case SPELL_HASH_REPENTANCE:				// Repentance
		case SPELL_HASH_SAP:
		case SPELL_HASH_POLYMORPH:				// Polymorph
		case SPELL_HASH_POLYMORPH__CHICKEN:		// Chicken
		case SPELL_HASH_POLYMORPH__CRAFTY_WOBBLESPROCKET: // Errr right?
		case SPELL_HASH_POLYMORPH__SHEEP:		// Good ol' sheep
		case SPELL_HASH_POLYMORPH___PENGUIN:	// Penguiiiin!!! :D
		case SPELL_HASH_MAIM:					// Maybe here?
		case SPELL_HASH_HEX:					// Should share diminish group with polymorph, but not interruptflags.
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
		case SPELL_HASH_TURN_EVIL:
			grp = DIMINISHING_GROUP_CHARM;		//Charm???
			break;

		case SPELL_HASH_KIDNEY_SHOT:
			{
				grp = DIMINISHING_GROUP_KIDNEY_SHOT;
				pve = true;
			}
			break;

		case SPELL_HASH_DEATH_COIL:
			grp = DIMINISHING_GROUP_HORROR;
			break;

		case SPELL_HASH_BANISH:					// Banish
			grp = DIMINISHING_GROUP_BANISH;
			break;

			// group where only 10s limit in pvp is applied, not DR
		case SPELL_HASH_FREEZING_TRAP_EFFECT:	// Freezing Trap Effect
		case SPELL_HASH_HAMSTRING:				// Hamstring
		case SPELL_HASH_CURSE_OF_TONGUES:
			{
				grp = DIMINISHING_GROUP_NOT_DIMINISHED;
			}
			break;

		case SPELL_HASH_RIPOSTE:		// Riposte
		case SPELL_HASH_DISARM:			// Disarm
			{
				grp = DIMINISHING_GROUP_DISARM;
			}
			break;

		case SPELL_HASH_SILENCE:
		case SPELL_HASH_GARROTE___SILENCE:
		case SPELL_HASH_SILENCED___IMPROVED_COUNTERSPELL:
		case SPELL_HASH_SILENCED___IMPROVED_KICK:
		case SPELL_HASH_SILENCED___GAG_ORDER:
			{
				grp = DIMINISHING_GROUP_SILENCE;
			}
			break;
	}

	uint32 ret;
	if(pve)
		ret = grp | (1 << 16);
	else
		ret = grp;

	return ret;
}

uint32 GetSpellDuration(SpellEntry* sp, Unit* caster /*= NULL*/)
{
	SpellDuration* dur = dbcSpellDuration.LookupEntryForced(sp->DurationIndex);
	if(dur == NULL)
		return 0;
	if(caster == NULL)
		return dur->Duration1;
	uint32 ret = dur->Duration1 + (dur->Duration2 * caster->getLevel());
	if(ret > dur->Duration3)
		return dur->Duration3;
	return ret;
}

void Spell::SendCastSuccess(Object* target)
{
	Player* plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr || !plr->IsPlayer())
		return;

	/*	WorldPacket data(SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE, 13);
		data << ((target != 0) ? target->GetNewGUID() : uint64(0));
		data << GetProto()->Id;

		plr->GetSession()->SendPacket(&data);*/
}

void Spell::SendCastSuccess(const uint64 & guid)
{
	Player* plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr || !plr->IsPlayer())
		return;

	// fuck bytebuffers
	unsigned char buffer[13];
	uint32 c = FastGUIDPack(guid, buffer, 0);

	*(uint32*)&buffer[c] = GetProto()->Id;
	c += 4;

	plr->GetSession()->OutPacket(uint16(SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE), static_cast<uint16>(c), buffer);
}

uint8 Spell::GetErrorAtShapeshiftedCast(SpellEntry* spellInfo, uint32 form)
{
	uint32 stanceMask = (form ? DecimalToMask(form) : 0);

	if(spellInfo->ShapeshiftExclude > 0 && spellInfo->ShapeshiftExclude & stanceMask)				// can explicitly not be casted in this stance
		return SPELL_FAILED_NOT_SHAPESHIFT;

	if(spellInfo->RequiredShapeShift == 0 || spellInfo->RequiredShapeShift & stanceMask)			// can explicitly be casted in this stance
		return 0;

	bool actAsShifted = false;
	if(form > FORM_NORMAL)
	{
		SpellShapeshiftForm* ssf = dbcSpellShapeshiftForm.LookupEntryForced(form);
		if(!ssf)
		{
			LOG_ERROR("GetErrorAtShapeshiftedCast: unknown shapeshift %u", form);
			return 0;
		}

		switch(ssf->id)
		{
			case FORM_TREE:
				{
					skilllinespell* sls = objmgr.GetSpellSkill(spellInfo->Id);
					if(sls && sls->skilline == SPELLTREE_DRUID_RESTORATION)		// Restoration spells can be cast in Tree of Life form, for the rest: apply the default rules.
						return 0;
				}
				break;
			case FORM_MOONKIN:
				{
					skilllinespell* sls = objmgr.GetSpellSkill(spellInfo->Id);
					if(sls && sls->skilline == SPELLTREE_DRUID_BALANCE)			// Balance spells can be cast in Moonkin form, for the rest: apply the default rules.
						return 0;
				}
				break;
		}
		actAsShifted = !(ssf->Flags & 1);						// shapeshift acts as normal form for spells
	}

	if(actAsShifted)
	{
		if(hasAttributeExB(ATTRIBUTES_NOT_SHAPESHIFT))	// not while shapeshifted
			return SPELL_FAILED_NOT_SHAPESHIFT;
		else //if( spellInfo->RequiredShapeShift != 0 )			// needs other shapeshift
			return SPELL_FAILED_ONLY_SHAPESHIFT;
	}
	else
	{
		// Check if it even requires a shapeshift....
		if(!hasAttributeExB(ATTRIBUTESEXB_NOT_NEED_SHAPESHIFT) && spellInfo->RequiredShapeShift != 0)
			return SPELL_FAILED_ONLY_SHAPESHIFT;
	}

	return 0;
}

uint32 Spell::GetTargetType(uint32 value, uint32 i)
{
	uint32 type = g_spellImplicitTargetFlags[value];

	//CHAIN SPELLS ALWAYS CHAIN!
	uint32 jumps = m_spellInfo->EffectChainTarget[i];
	if(u_caster != NULL)
		SM_FIValue(u_caster->SM_FAdditionalTargets, (int32*)&jumps, m_spellInfo->SpellGroupType);
	if(jumps != 0)
		type |= SPELL_TARGET_AREA_CHAIN;

	return type;
}

void Spell::HandleCastEffects(uint64 guid, uint32 i)
{
	if(m_spellInfo->speed == 0)  //instant
	{
		AddRef();
		HandleEffects(guid, i);
	}
	else
	{
		float destx, desty, destz, dist = 0;

		if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
		{
			destx = m_targets.m_destX;
			desty = m_targets.m_destY;
			destz = m_targets.m_destZ;

			dist = m_caster->CalcDistance(destx, desty, destz);
		}
		else if(guid == 0)
		{
			return;
		}
		else
		{
			if(!m_caster->IsInWorld())
				return;

			if(m_caster->GetGUID() != guid)
			{
				Object* obj = m_caster->GetMapMgr()->_GetObject(guid);
				if(obj == NULL)
					return;

				destx = obj->GetPositionX();
				desty = obj->GetPositionY();
				//todo: this should be destz = obj->GetPositionZ() + (obj->GetModelHighBoundZ() / 2 * obj->GetUInt32Value(OBJECT_FIELD_SCALE_X))
				if(obj->IsUnit())
					destz = obj->GetPositionZ() + TO_UNIT(obj)->GetModelHalfSize();
				else
					destz = obj->GetPositionZ();

				dist = m_caster->CalcDistance(destx, desty, destz);
			}
		}

		if(dist == 0.0f)
		{
			AddRef();
			HandleEffects(guid, i);
		}
		else
		{
			float time;

			if( m_missileTravelTime != 0 )
				time = m_missileTravelTime;
			else
				time = dist * 1000.0f / m_spellInfo->speed;

			//todo: arcemu doesn't support reflected spells
			//if (reflected)
			//	time *= 1.25; //reflected projectiles move back 4x faster

			sEventMgr.AddEvent(this, &Spell::HandleEffects, guid, i, EVENT_SPELL_HIT, float2int32(time), 1, 0);
			AddRef();
		}
	}
}

void Spell::HandleModeratedTarget(uint64 guid)
{
	if(m_spellInfo->speed == 0)  //instant
	{
		AddRef();
		HandleModeratedEffects(guid);
	}
	else
	{
		float destx, desty, destz, dist = 0;

		if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
		{
			destx = m_targets.m_destX;
			desty = m_targets.m_destY;
			destz = m_targets.m_destZ;

			dist = m_caster->CalcDistance(destx, desty, destz);
		}
		else if(guid == 0)
		{
			return;
		}
		else
		{
			if(!m_caster->IsInWorld())
				return;

			if(m_caster->GetGUID() != guid)
			{
				Object* obj = m_caster->GetMapMgr()->_GetObject(guid);
				if(obj == NULL)
					return;

				destx = obj->GetPositionX();
				desty = obj->GetPositionY();
				//todo: this should be destz = obj->GetPositionZ() + (obj->GetModelHighBoundZ() / 2 * obj->GetUInt32Value(OBJECT_FIELD_SCALE_X))
				if(obj->IsUnit())
					destz = obj->GetPositionZ() + TO_UNIT(obj)->GetModelHalfSize();
				else
					destz = obj->GetPositionZ();

				dist = m_caster->CalcDistance(destx, desty, destz);
			}
		}

		if(dist == 0.0f)
		{
			AddRef();
			HandleModeratedEffects(guid);
		}
		else
		{
			float time = dist * 1000.0f / m_spellInfo->speed;
			//todo: arcemu doesn't support reflected spells
			//if (reflected)
			//	time *= 1.25; //reflected projectiles move back 4x faster
			sEventMgr.AddEvent(this, &Spell::HandleModeratedEffects, guid, EVENT_SPELL_HIT, float2int32(time), 1, 0);
			AddRef();
		}
	}
}

void Spell::HandleModeratedEffects(uint64 guid)
{
	//note: because this was a miss etc, we don't need to do attackable target checks
	if(u_caster != NULL && u_caster->GetMapMgr() != NULL)
	{
		Object* obj = u_caster->GetMapMgr()->_GetObject(guid);

		if(obj != NULL && obj->IsCreature() && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NO_INITIAL_AGGRO))
		{
			TO_CREATURE(obj)->GetAIInterface()->AttackReaction(u_caster, 0, 0);
			TO_CREATURE(obj)->GetAIInterface()->HandleEvent(EVENT_HOSTILEACTION, u_caster, 0);
		}
	}

	DecRef();
}

void Spell::SpellEffectJumpBehindTarget(uint32 i)
{
	if(u_caster == NULL)
		return;
	if(m_targets.m_targetMask & TARGET_FLAG_UNIT)
	{
		Object* uobj = m_caster->GetMapMgr()->_GetObject(m_targets.m_unitTarget);

		if(uobj == NULL || !uobj->IsUnit())
			return;
		Unit* un = TO_UNIT(uobj);
		float rad = un->GetBoundingRadius() + u_caster->GetBoundingRadius();
		float angle = un->GetOrientation() + M_PI; //behind
		float x = un->GetPositionX() + cosf(angle) * rad;
		float y = un->GetPositionY() + sinf(angle) * rad;
		float z = un->GetPositionZ();
		float o = un->calcRadAngle(x, y, un->GetPositionX(), un->GetPositionY());

		if(u_caster->GetAIInterface() != NULL)
			u_caster->GetAIInterface()->MoveJump(x, y, z, o);
	}
	else if(m_targets.m_targetMask & (TARGET_FLAG_SOURCE_LOCATION | TARGET_FLAG_DEST_LOCATION))
	{
		float x, y, z;

		//this can also jump to a point
		if(m_targets.m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
		{
			x = m_targets.m_srcX;
			y = m_targets.m_srcY;
			z = m_targets.m_srcZ;
		}
		if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
		{
			x = m_targets.m_destX;
			y = m_targets.m_destY;
			z = m_targets.m_destZ;
		}

		if(u_caster->GetAIInterface() != NULL)
			u_caster->GetAIInterface()->MoveJump(x, y, z);
	}
}

void Spell::HandleTargetNoObject()
{
	float dist = 3;
	float newx = m_caster->GetPositionX() + cosf(m_caster->GetOrientation()) * dist;
	float newy = m_caster->GetPositionY() + sinf(m_caster->GetOrientation()) * dist;
	float newz = m_caster->GetPositionZ();

	//clamp Z
	newz = m_caster->GetMapMgr()->GetLandHeight(newx, newy, newz);

	//if not in line of sight, or too far away we summon inside caster
	if(fabs(newz - m_caster->GetPositionZ()) > 10 || !CollideInterface.CheckLOS(m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ() + 2, newx, newy, newz + 2))
	{
		newx = m_caster->GetPositionX();
		newy = m_caster->GetPositionY();
		newz = m_caster->GetPositionZ();
	}

	m_targets.m_targetMask |= TARGET_FLAG_DEST_LOCATION;
	m_targets.m_destX = newx;
	m_targets.m_destY = newy;
	m_targets.m_destZ = newz;
}

//Logs if the spell doesn't exist, using Debug loglevel.
SpellEntry* CheckAndReturnSpellEntry(uint32 spellid)
{
	//Logging that spellid 0 or -1 don't exist is not needed.
	if(spellid == 0 || spellid == uint32(-1))
		return NULL;

	SpellEntry* sp = dbcSpell.LookupEntryForced(spellid);
	if(sp == NULL)
		LOG_DEBUG("Something tried to access nonexistent spell %u", spellid);

	return sp;
}


bool IsDamagingSpell(SpellEntry* sp)
{

	if(sp->HasEffect(SPELL_EFFECT_SCHOOL_DAMAGE)          ||
	        sp->HasEffect(SPELL_EFFECT_ENVIRONMENTAL_DAMAGE)   ||
	        sp->HasEffect(SPELL_EFFECT_HEALTH_LEECH)           ||
	        sp->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL) ||
	        sp->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS)      ||
	        sp->HasEffect(SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)  ||
	        sp->HasEffect(SPELL_EFFECT_POWER_BURN)             ||
	        sp->HasEffect(SPELL_EFFECT_ATTACK))
		return true;

	if(sp->AppliesAura(SPELL_AURA_PERIODIC_DAMAGE)        ||
	        sp->AppliesAura(SPELL_AURA_PROC_TRIGGER_DAMAGE)    ||
	        sp->AppliesAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT) ||
	        sp->AppliesAura(SPELL_AURA_POWER_BURN))
		return true;

	return false;
}