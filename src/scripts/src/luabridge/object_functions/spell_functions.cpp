/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

#include "LUAEngine.h"

#define DEFINEVARMETHODS(name,typ) \
	template<int N> ARCEMU_FORCEINLINE typ varget##name() const { return name[N]; } \
	template<int N> ARCEMU_FORCEINLINE void varset##name(typ val) { name[N] = val; }

class lspellentry : public SpellEntry
{
	public:
		DEFINEVARMETHODS(Effect, uint32)
		DEFINEVARMETHODS(EffectBasePoints, int32)
		DEFINEVARMETHODS(EffectMechanic, int32)
		DEFINEVARMETHODS(EffectAmplitude, uint32)
		DEFINEVARMETHODS(EffectMiscValue, uint32)
		DEFINEVARMETHODS(EffectMiscValueB, uint32)
		DEFINEVARMETHODS(EffectTriggerSpell, uint32)
		DEFINEVARMETHODS(EffectApplyAuraName, uint32)
};

namespace lua_engine
{
	void bindSpellMethods(luabridge::module & m)
	{
		m	.class_decl<SpellEntry>("SpellEntry");
		//Make lspellentry and SpellEntry share a common name so that objects derived from the two share the same metatable and thus all the methods associated w/ that metatable.
		m	.class_<lspellentry>("SpellEntry")
		.property_ro("Id", (uint32(lspellentry::*)) &SpellEntry::Id)
		.property_ro("DispelType", (uint32(lspellentry::*)) &SpellEntry::DispelType)
		.property_ro("MechanicsType", (uint32(lspellentry::*)) &SpellEntry::MechanicsType)
		/*.property_rw("Attributes", (uint32(lspellentry::*) ) &SpellEntry::Attributes)
		.property_rw("AttributesEx", (uint32(lspellentry::*) ) &SpellEntry::AttributesEx)
		.property_rw("AttributesExB", (uint32(lspellentry::*) ) &SpellEntry::AttributesExB)
		prop(AttributesExC)
		prop(AttributesExD)
		prop(AttributesExE)
		prop(AttributesExF)*/
		.property_ro("RequiredShapeShift", (uint32(lspellentry::*)) &SpellEntry::RequiredShapeShift)
		/*prop(Targets)
		prop(TargetCreatureType)
		prop(RequiresSpellFocus)
		prop(FacingCasterFlags)
		prop(CasterAuraState)
		prop(TargetAuraState)
		prop(CasterAuraStateNot)
		prop(TargetAuraStateNot)
		prop(casterAuraSpell)
		prop(targetAuraSpell)
		prop(casterAuraSpellNot)
		prop(targetAuraSpellNot)
		prop(CastingTimeIndex)
		prop(RecoveryTime)
		prop(CategoryRecoveryTime)
		prop(InterruptFlags)
		prop(AuraInterruptFlags)
		prop(ChannelInterruptFlags)
		prop(procFlags)
		prop(procChance)
		prop(procCharges)*/
		.property_ro("maxLevel", (uint32(lspellentry::*)) &SpellEntry::maxLevel)
		.property_ro("baseLevel", (uint32(lspellentry::*)) &SpellEntry::baseLevel)
		.property_ro("spellLevel", (uint32(lspellentry::*)) &SpellEntry::spellLevel)
		.property_ro("powerType", (uint32(lspellentry::*)) &SpellEntry::powerType)
		.property_ro("School", (uint32(lspellentry::*)) &SpellEntry::School)
		.property_ro("c_is_flags", (uint32(lspellentry::*)) &SpellEntry::c_is_flags)
		.property_ro("NameHash", (uint32(lspellentry::*)) &SpellEntry::NameHash)
		.property_rw("Effect1", &lspellentry::vargetEffect<0>, &lspellentry::varsetEffect<0>)
		.property_rw("Effect2", &lspellentry::vargetEffect<1>, &lspellentry::varsetEffect<1>)
		.property_rw("Effect3", &lspellentry::vargetEffect<2>, &lspellentry::varsetEffect<2>)
		.property_rw("EffectBasePoints1", &lspellentry::vargetEffectBasePoints<0>, &lspellentry::varsetEffectBasePoints<0>)
		.property_rw("EffectBasePoints2", &lspellentry::vargetEffectBasePoints<1>, &lspellentry::varsetEffectBasePoints<1>)
		.property_rw("EffectBasePoints3", &lspellentry::vargetEffectBasePoints<2>, &lspellentry::varsetEffectBasePoints<2>)
		.property_rw("EffectMechanic1", &lspellentry::vargetEffectMechanic<0>, &lspellentry::varsetEffectMechanic<0>)
		.property_rw("EffectMechanic2", &lspellentry::vargetEffectMechanic<1>, &lspellentry::varsetEffectMechanic<1>)
		.property_rw("EffectMechanic3", &lspellentry::vargetEffectMechanic<2>, &lspellentry::varsetEffectMechanic<2>)
		.property_rw("EffectAmplitude1", &lspellentry::vargetEffectAmplitude<0>, &lspellentry::varsetEffectAmplitude<0>)
		.property_rw("EffectAmplitude2", &lspellentry::vargetEffectAmplitude<1>, &lspellentry::varsetEffectAmplitude<1>)
		.property_rw("EffectAmplitude3", &lspellentry::vargetEffectAmplitude<2>, &lspellentry::varsetEffectAmplitude<2>)
		.property_rw("EffectMiscValue1", &lspellentry::vargetEffectMiscValue<0>, &lspellentry::varsetEffectMiscValue<0>)
		.property_rw("EffectMiscValue2", &lspellentry::vargetEffectMiscValue<1>, &lspellentry::varsetEffectMiscValue<1>)
		.property_rw("EffectMiscValue3", &lspellentry::vargetEffectMiscValue<2>, &lspellentry::varsetEffectMiscValue<2>)
		.property_rw("EffectMiscValueB1", &lspellentry::vargetEffectMiscValueB<0>, &lspellentry::varsetEffectMiscValueB<0>)
		.property_rw("EffectMiscValueB2", &lspellentry::vargetEffectMiscValueB<1>, &lspellentry::varsetEffectMiscValueB<1>)
		.property_rw("EffectMiscValueB3", &lspellentry::vargetEffectMiscValueB<2>, &lspellentry::varsetEffectMiscValueB<2>)
		.property_rw("EffectTriggerSpell1", &lspellentry::vargetEffectTriggerSpell<0>, &lspellentry::varsetEffectTriggerSpell<0>)
		.property_rw("EffectTriggerSpell2", &lspellentry::vargetEffectTriggerSpell<1>, &lspellentry::varsetEffectTriggerSpell<1>)
		.property_rw("EffectTriggerSpell3", &lspellentry::vargetEffectTriggerSpell<2>, &lspellentry::varsetEffectTriggerSpell<2>);
#undef prop
		m	.class_<Spell>("Spell")
		.property_ro("u_caster", &Spell::u_caster)
		.property_ro("g_caster", &Spell::g_caster)
		.property_ro("m_caster", &Spell::m_caster)
		.property_ro("i_caster", &Spell::i_caster)
		.property_ro("p_caster", &Spell::p_caster)
		.property_ro("duelspell", &Spell::duelSpell)
		.method(&Spell::GetType, "GetType", "getType", "gettype", NULL)
		.method(&Spell::getState, "getstate", "GetState", "getState", NULL)
		.method(&Spell::cancel, "cancel", "Cancel", NULL)
		.method(&Spell::cast, "Cast", "cast", NULL)
		.method(&Spell::finish, "Finish", "finish", NULL)
		/*.property_ro("u_target", &Spell::unitTarget)
		.property_ro("i_target", &Spell::itemTarget)
		.property_ro("g_target", &Spell::gameObjTarget)
		.property_ro("p_target", &Spell::playerTarget)*/
		.method(&Spell::GetProto, "GetProto", "getProto", "getproto", NULL)
		.method(&Spell::IsAspect, "IsAspect" "isAspect", "isaspect", NULL)
		.method(&Spell::IsSeal, "IsSeal", "isSeal", "isseal", NULL)
		.method(&Spell::InitProtoOverride, "createprotooverride", "InitializeProtoOverride", "initializeprotooverride", NULL)
		.method(&Spell::GetDuration, "GetDuration", "getDuration", "getduration", NULL)
		.method(&Spell::GetRadius, "GetRadius", "getRadius", "getradius", NULL)
		.method(&Spell::IsStealthSpell, "IsStealthSpell", "isStealthSpell", "isstealthspell", NULL)
		.method(&Spell::IsInvisibilitySpell, "IsInvisilitySpell", "isInvisibilitySpell", "isinvisibilityspell", NULL)
		.property_ro("triggered", &Spell::m_triggeredSpell)
		.property_ro("reflectedfrom", &Spell::m_reflectedParent)
		.method(&Spell::IsReflected, "IsReflected", "isReflected", "isreflected", NULL)
		.property_ro("proto", &Spell::m_spellInfo)
		.property_ro("proto_override", &Spell::m_spellInfo_override);

	}
}
