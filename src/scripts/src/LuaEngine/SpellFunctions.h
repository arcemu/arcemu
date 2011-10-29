/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LUA_SPELL_H
#define LUA_SPELL_H

#define GET_SPELLVAR_INT(proto,offset,subindex) *(int*)((char*)(proto) + (offset) + (subindex))
#define GET_SPELLVAR_CHAR(proto,offset,subindex) *(char**)((char*)(proto) + (offset) + (subindex))
#define GET_SPELLVAR_BOOL(proto,offset,subindex) *(bool*)((char*)(proto) + (offset) + (subindex))
#define GET_SPELLVAR_FLOAT(proto,offset,subindex) *(float*)((char*)(proto) + (offset) + (subindex))

struct LuaSpellEntry
{
	const char* name;
	uint32 typeId; //0: int, 1: char*, 2: bool, 3: float
	size_t offset;
};

LuaSpellEntry luaSpellVars[] =
{
	{"Id", 0, offsetof(SpellEntry, Id)},
	{"Category", 0, offsetof(SpellEntry, Category)},
	{"DispelType", 0, offsetof(SpellEntry, DispelType)},
	{"MechanicsType", 0, offsetof(SpellEntry, MechanicsType)},
	{"Attributes", 0, offsetof(SpellEntry, Attributes)},
	{"AttributesEx", 0, offsetof(SpellEntry, AttributesEx)},
	{"AttributesExB", 0, offsetof(SpellEntry, AttributesExB)},
	{"AttributesExC", 0, offsetof(SpellEntry, AttributesExC)},
	{"AttributesExD", 0, offsetof(SpellEntry, AttributesExD)},
	{"AttributesExE", 0, offsetof(SpellEntry, AttributesExE)},
	{"AttributesExF", 0, offsetof(SpellEntry, AttributesExF)},
	{"RequiredShapeShift", 0, offsetof(SpellEntry, RequiredShapeShift)},
	{"ShapeshiftExclude", 0, offsetof(SpellEntry, ShapeshiftExclude)},
	{"Targets", 0, offsetof(SpellEntry, Targets)},
	{"TargetCreatureType", 0, offsetof(SpellEntry, TargetCreatureType)},
	{"RequiresSpellFocus", 0, offsetof(SpellEntry, RequiresSpellFocus)},
	{"FacingCasterFlags", 0, offsetof(SpellEntry, FacingCasterFlags)},
	{"CasterAuraState", 0, offsetof(SpellEntry, CasterAuraState)},
	{"TargetAuraState", 0, offsetof(SpellEntry, TargetAuraState)},
	{"CasterAuraStateNot", 0, offsetof(SpellEntry, CasterAuraStateNot)},
	{"TargetAuraStateNot", 0, offsetof(SpellEntry, TargetAuraStateNot)},
	{"casterAuraSpell", 0, offsetof(SpellEntry, casterAuraSpell)},
	{"targetAuraSpell", 0, offsetof(SpellEntry, targetAuraSpell)},
	{"casterAuraSpellNot", 0, offsetof(SpellEntry, casterAuraSpellNot)},
	{"targetAuraSpellNot", 0, offsetof(SpellEntry, targetAuraSpellNot)},
	{"CastingTimeIndex", 0, offsetof(SpellEntry, CastingTimeIndex)},
	{"RecoveryTime", 0, offsetof(SpellEntry, RecoveryTime)},
	{"CategoryRecoveryTime", 0, offsetof(SpellEntry, CategoryRecoveryTime)},
	{"InterruptFlags", 0, offsetof(SpellEntry, InterruptFlags)},
	{"AuraInterruptFlags", 0, offsetof(SpellEntry, AuraInterruptFlags)},
	{"ChannelInterruptFlags", 0, offsetof(SpellEntry, ChannelInterruptFlags)},
	{"procFlags", 0, offsetof(SpellEntry, procFlags)},
	{"procChance", 0, offsetof(SpellEntry, procChance)},
	{"procCharges", 0, offsetof(SpellEntry, procCharges)},
	{"maxLevel", 0, offsetof(SpellEntry, maxLevel)},
	{"baseLevel", 0, offsetof(SpellEntry, baseLevel)},
	{"spellLevel", 0, offsetof(SpellEntry, spellLevel)},
	{"DurationIndex", 0, offsetof(SpellEntry, DurationIndex)},
	{"powerType", 0, offsetof(SpellEntry, powerType)},
	{"manaCost", 0, offsetof(SpellEntry, manaCost)},
	{"manaCostPerlevel", 0, offsetof(SpellEntry, manaCostPerlevel)},
	{"manaPerSecond", 0, offsetof(SpellEntry, manaPerSecond)},
	{"manaPerSecondPerLevel", 0, offsetof(SpellEntry, manaPerSecondPerLevel)},
	{"rangeIndex", 0, offsetof(SpellEntry, rangeIndex)},
	{"speed", 3, offsetof(SpellEntry, speed)},
	{"modalNextSpell", 0, offsetof(SpellEntry, modalNextSpell)},
	{"maxstack", 0, offsetof(SpellEntry, maxstack)},
	{"Totem", 0, offsetof(SpellEntry, Totem[0])},
	{"Reagent", 0, offsetof(SpellEntry, Reagent[0])},
	{"ReagentCount", 0, offsetof(SpellEntry, ReagentCount[0])},
	{"EquippedItemClass", 0, offsetof(SpellEntry, EquippedItemClass)},
	{"EquippedItemSubClass", 0, offsetof(SpellEntry, EquippedItemSubClass)},
	{"RequiredItemFlags", 0, offsetof(SpellEntry, RequiredItemFlags)},
	{"Effect", 0, offsetof(SpellEntry, Effect[0])},
	{"EffectDieSides", 0, offsetof(SpellEntry, EffectDieSides[0])},
	//{"EffectBaseDice", 0, offsetof(SpellEntry, EffectBaseDice[0])},
	//{"EffectDicePerLevel", 3, offsetof(SpellEntry, EffectDicePerLevel[0])},
	{"EffectRealPointsPerLevel", 3, offsetof(SpellEntry, EffectRealPointsPerLevel[0])},
	{"EffectBasePoints", 0, offsetof(SpellEntry, EffectBasePoints[0])},
	{"EffectMechanic", 0, offsetof(SpellEntry, EffectMechanic[0])},
	{"EffectImplicitTargetA", 0, offsetof(SpellEntry, EffectImplicitTargetA[0])},
	{"EffectImplicitTargetB", 0, offsetof(SpellEntry, EffectImplicitTargetB[0])},
	{"EffectRadiusIndex", 0, offsetof(SpellEntry, EffectRadiusIndex[0])},
	{"EffectApplyAuraName", 0, offsetof(SpellEntry, EffectApplyAuraName[0])},
	{"EffectAmplitude", 0, offsetof(SpellEntry, EffectAmplitude[0])},
	{"EffectMultipleValue", 3, offsetof(SpellEntry, EffectMultipleValue[0])},
	{"EffectChainTarget", 0, offsetof(SpellEntry, EffectChainTarget[0])},
	{"EffectItemType", 0, offsetof(SpellEntry, EffectItemType[0])},
	{"EffectMiscValue", 0, offsetof(SpellEntry, EffectMiscValue[0])},
	{"EffectMiscValueB", 0, offsetof(SpellEntry, EffectMiscValueB[0])},
	{"EffectTriggerSpell", 0, offsetof(SpellEntry, EffectTriggerSpell[0])},
	{"EffectPointsPerComboPoint", 3, offsetof(SpellEntry, EffectPointsPerComboPoint[0])},
	{"EffectSpellClassMask", 0, offsetof(SpellEntry, EffectSpellClassMask[0][0])},
	{"SpellVisual", 0, offsetof(SpellEntry, SpellVisual)},
	{"field114", 0, offsetof(SpellEntry, field114)},
	{"spellIconID", 0, offsetof(SpellEntry, spellIconID)},
	{"activeIconID", 0, offsetof(SpellEntry, activeIconID)},
	{"spellPriority", 0, offsetof(SpellEntry, spellPriority)},
	{"Name", 1, offsetof(SpellEntry, Name)},
	{"Rank", 1, offsetof(SpellEntry, Rank)},
	{"Description", 1, offsetof(SpellEntry, Description)},
	{"BuffDescription", 1, offsetof(SpellEntry, BuffDescription)},
	{"ManaCostPercentage", 0, offsetof(SpellEntry, ManaCostPercentage)},
	{"StartRecoveryCategory", 0, offsetof(SpellEntry, StartRecoveryCategory)},
	{"StartRecoveryTime", 0, offsetof(SpellEntry, StartRecoveryTime)},
	{"MaxTargetLevel", 0, offsetof(SpellEntry, MaxTargetLevel)},
	{"SpellFamilyName", 0, offsetof(SpellEntry, SpellFamilyName)},
	{"SpellGroupType", 0, offsetof(SpellEntry, SpellGroupType[0])},
	{"MaxTargets", 0, offsetof(SpellEntry, MaxTargets)},
	{"Spell_Dmg_Type", 0, offsetof(SpellEntry, Spell_Dmg_Type)},
	{"PreventionType", 0, offsetof(SpellEntry, PreventionType)},
	{"StanceBarOrder", 0, offsetof(SpellEntry, StanceBarOrder)},
	{"dmg_multiplier", 3, offsetof(SpellEntry, dmg_multiplier[0])},
	{"MinFactionID", 0, offsetof(SpellEntry, MinFactionID)},
	{"MinReputation", 0, offsetof(SpellEntry, MinReputation)},
	{"RequiredAuraVision", 0, offsetof(SpellEntry, RequiredAuraVision)},
	{"TotemCategory", 0, offsetof(SpellEntry, TotemCategory[0])},
	{"RequiresAreaId", 0, offsetof(SpellEntry, RequiresAreaId)},
	{"School", 0, offsetof(SpellEntry, School)},
	{"RuneCostID", 0, offsetof(SpellEntry, RuneCostID)},
	{"DiminishStatus", 0, offsetof(SpellEntry, DiminishStatus)},
	{"proc_interval", 0, offsetof(SpellEntry, proc_interval)},
	{"BGR_one_buff_on_target", 0, offsetof(SpellEntry, BGR_one_buff_on_target)},
	{"BGR_one_buff_from_caster_on_self", 0, offsetof(SpellEntry, BGR_one_buff_from_caster_on_self)},
	{"c_is_flags", 0, offsetof(SpellEntry, c_is_flags)},
	{"RankNumber", 0, offsetof(SpellEntry, RankNumber)},
	{"NameHash", 0, offsetof(SpellEntry, NameHash)},
	{"talent_tree", 0, offsetof(SpellEntry, talent_tree)},
	{"in_front_status", 0, offsetof(SpellEntry, in_front_status)},
	{"EffectSpellGroupRelation_high", 0, offsetof(SpellEntry, EffectSpellGroupRelation_high[0])},
	{"ThreatForSpell", 0, offsetof(SpellEntry, ThreatForSpell)},
	{"ThreatForSpellCoef", 3, offsetof(SpellEntry, ThreatForSpellCoef)},
	{"ProcOnNameHash", 0, offsetof(SpellEntry, ProcOnNameHash[0])},
	{"spell_coef_flags", 0, offsetof(SpellEntry, spell_coef_flags)},
	{"base_range_or_radius_sqr", 3, offsetof(SpellEntry, base_range_or_radius_sqr)},
	{"cone_width", 3, offsetof(SpellEntry, cone_width)},
	{"casttime_coef", 3, offsetof(SpellEntry, casttime_coef)},
	{"fixed_dddhcoef", 3, offsetof(SpellEntry, fixed_dddhcoef)},
	{"fixed_hotdotcoef", 3, offsetof(SpellEntry, fixed_hotdotcoef)},
	{"Dspell_coef_override", 3, offsetof(SpellEntry, Dspell_coef_override)},
	{"OTspell_coef_override", 3, offsetof(SpellEntry, OTspell_coef_override)},
	{"ai_target_type", 0, offsetof(SpellEntry, ai_target_type)},
	{"self_cast_only", 2, offsetof(SpellEntry, self_cast_only)},
	{"apply_on_shapeshift_change", 2, offsetof(SpellEntry, apply_on_shapeshift_change)},
	{"always_apply", 2, offsetof(SpellEntry, always_apply)},
	{"is_melee_spell", 2, offsetof(SpellEntry, is_melee_spell)},
	{"is_ranged_spell", 2, offsetof(SpellEntry, is_ranged_spell)},
	{"noproc", 2, offsetof(SpellEntry, noproc)},
	{"SchoolMask", 0, offsetof(SpellEntry, SchoolMask)},
	{NULL, 0, 0},
};

LuaSpellEntry GetLuaSpellEntryByName(const char* name)
{
	for(uint32 itr = 0; luaSpellVars[itr].name != NULL; itr++)
	{
		LuaSpellEntry l = luaSpellVars[itr];
		if(strcmp(l.name, name) == 0)  //they entered a correct var name
			return l;
	}
	int lElem = sizeof(luaSpellVars) / sizeof(luaSpellVars[0]) - 1;
	return luaSpellVars[lElem];
}

namespace LuaSpell
{
	int GetCaster(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		if(sp->u_caster)  //unit caster
		{
			PUSH_UNIT(L, sp->u_caster);
			return 1;
		}
		else if(sp->g_caster)  //gameobject
		{
			PUSH_GO(L, sp->g_caster);
			return 1;
		}
		else if(sp->i_caster)  //item
		{
			PUSH_ITEM(L, sp->i_caster);
			return 1;
		}
		else
		{
			lua_pushnil(L);
			return 1;
		}
	}

	int GetEntry(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushinteger(L, sp->GetProto()->Id);
		return 1;
	}

	int IsDuelSpell(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->duelSpell ? 1 : 0);
		return 1;
	}

	int GetSpellType(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushinteger(L, sp->GetType());
		return 1;
	}

	int GetSpellState(lua_State* L, Spell* sp)
	{
		/*
		SPELL_STATE_NULL      = 0,
		SPELL_STATE_PREPARING = 1,
		SPELL_STATE_CASTING   = 2,
		SPELL_STATE_FINISHED  = 3,
		SPELL_STATE_IDLE      = 4
		*/
		if(!sp) return 0;
		lua_pushinteger(L, sp->getState());
		return 1;
	}

	int Cancel(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		sp->cancel();
		return 0;
	}

	int Cast(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		bool check = CHECK_BOOL(L, 1);
		sp->cast(check);
		return 0;
	}

	int CanCast(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushinteger(L, sp->CanCast(false));
		return 1;
	}

	int Finish(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		sp->finish();
		return 0;
	}

	int GetTarget(lua_State* L, Spell* sp)
	{
		if(!sp || !sp->m_caster->IsInWorld())
			RET_NIL()

			if(sp->m_targets.m_unitTarget)
			{
				PUSH_UNIT(L, sp->m_caster->GetMapMgr()->GetUnit(sp->m_targets.m_unitTarget));
				return 1;
			}
			else if(sp->m_targets.m_itemTarget)
			{
				if(!sp->p_caster)
					RET_NIL()
					PUSH_ITEM(L, sp->p_caster->GetItemInterface()->GetItemByGUID(sp->m_targets.m_itemTarget));
				return 1;
			}
			else
				RET_NIL()
			}

	int IsStealthSpell(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->IsStealthSpell() ? 1 : 0);
		return 1;
	}

	int IsInvisibilitySpell(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->IsInvisibilitySpell() ? 1 : 0);
		return 1;
	}

	int GetPossibleEnemy(lua_State* L, Spell* sp)
	{
		float range = (float)luaL_optnumber(L, 1, 0.0f);
		if(!sp || range < 0) return 0;
		PUSH_GUID(L, sp->GetSinglePossibleEnemy(0, range));
		return 1;
	}

	int GetPossibleFriend(lua_State* L, Spell* sp)
	{
		float range = (float)luaL_optnumber(L, 1, 0.0f);
		if(!sp || range < 0) return 0;
		PUSH_GUID(L, sp->GetSinglePossibleFriend(0, range));
		return 1;
	}

	int HasPower(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->HasPower() ? 1 : 0);
		return 1;
	}

	int IsAspect(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->IsAspect() ? 1 : 0);
		return 1;
	}

	int IsSeal(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		lua_pushboolean(L, sp->IsSeal() ? 1 : 0);
		return 1;
	}

	int GetObjectType(lua_State* L, Spell* sp)
	{
		if(!sp)
		{
			lua_pushnil(L);
			return 1;
		}
		lua_pushstring(L, "Spell");
		return 1;
	}

	int SetVar(lua_State* L, Spell* sp)
	{
		const char* var = luaL_checkstring(L, 1);
		int subindex = 0;
		int valindex = 2;
		if(lua_gettop(L) == 3)
		{
			subindex = luaL_optint(L, 2, 0);
			valindex++;
		}
		if(!sp || !var || subindex < 0)
		{
			lua_pushboolean(L, 0);
			return 1;
		}
		sp->InitProtoOverride();
		SpellEntry* proto = sp->GetProto();
		LuaSpellEntry l = GetLuaSpellEntryByName(var);
		if(!l.name)
			RET_BOOL(false);
		switch(l.typeId)  //0: int, 1: char*, 2: bool, 3: float
		{
			case 0:
				GET_SPELLVAR_INT(proto, l.offset, subindex) = luaL_checkinteger(L, valindex);
				lua_pushboolean(L, 1);
				break;
			case 1:
				strcpy(GET_SPELLVAR_CHAR(proto, l.offset, subindex), luaL_checkstring(L, valindex));
				lua_pushboolean(L, 1);
				break;
			case 2:
				GET_SPELLVAR_BOOL(proto, l.offset, subindex) = CHECK_BOOL(L, valindex);
				lua_pushboolean(L, 1);
				break;
			case 3:
				GET_SPELLVAR_FLOAT(proto, l.offset, subindex) = (float)luaL_checknumber(L, valindex);
				lua_pushboolean(L, 1);
				break;
		}
		return 1;
	}

	int GetVar(lua_State* L, Spell* sp)
	{
		const char* var = luaL_checkstring(L, 1);
		int subindex = luaL_optint(L, 2, 0);
		if(!sp || !var || subindex < 0)
		{
			lua_pushnil(L);
			return 1;
		}
		SpellEntry* proto = sp->GetProto();
		LuaSpellEntry l = GetLuaSpellEntryByName(var);
		if(!l.name)
			RET_NIL();
		switch(l.typeId)  //0: int, 1: char*, 2: bool, 3: float
		{
			case 0:
				lua_pushinteger(L, GET_SPELLVAR_INT(proto, l.offset, subindex));
				break;
			case 1:
				lua_pushstring(L, GET_SPELLVAR_CHAR(proto, l.offset, subindex));
				break;
			case 2:
				lua_pushboolean(L, (GET_SPELLVAR_BOOL(proto, l.offset, subindex)) ? 1 : 0);
				break;
			case 3:
				lua_pushnumber(L, GET_SPELLVAR_FLOAT(proto, l.offset, subindex));
				break;
		}
		return 1;
	}

	int ResetVar(lua_State* L, Spell* sp)
	{
		const char* var = luaL_checkstring(L, 1);
		int subindex = luaL_optint(L, 2, 0);
		if(!sp || !var || subindex < 0)
		{
			lua_pushboolean(L, 0);
			return 1;
		}
		LuaSpellEntry l = GetLuaSpellEntryByName(var);
		if(!l.name)
			RET_BOOL(false);
		switch(l.typeId)  //0: int, 1: char*, 2: bool, 3: float
		{
			case 0:
				GET_SPELLVAR_INT(sp->GetProto(), l.offset, subindex) = GET_SPELLVAR_INT(sp->m_spellInfo, l.offset, subindex);
				lua_pushboolean(L, 1);
				break;
			case 1:
				GET_SPELLVAR_CHAR(sp->GetProto(), l.offset, subindex) = GET_SPELLVAR_CHAR(sp->m_spellInfo, l.offset, subindex);
				lua_pushboolean(L, 1);
				break;
			case 2:
				GET_SPELLVAR_BOOL(sp->GetProto(), l.offset, subindex) = GET_SPELLVAR_BOOL(sp->m_spellInfo, l.offset, subindex);
				lua_pushboolean(L, 1);
				break;
			case 3:
				GET_SPELLVAR_FLOAT(sp->GetProto(), l.offset, subindex) = GET_SPELLVAR_FLOAT(sp->m_spellInfo, l.offset, subindex);
				lua_pushboolean(L, 1);
				break;
		}
		return 1;
	}

	int ResetAllVars(lua_State* L, Spell* sp)
	{
		if(!sp) return 0;
		sp->m_spellInfo_override = NULL;
		return 0;
	}

	int GetCastedItemId(lua_State* L, Spell* sp)
	{
		if(!sp)
		{
			lua_pushnil(L);
			return 1;
		}
		lua_pushnumber(L, sp->castedItemId);
		return 1;
	}
}
#endif