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
};

namespace lua_engine
{
	void bindSpellMethods(luabridge::module & m)
	{
#define prop(name) .property_rw(#name, &SpellEntry::name)
		//Make lspellentry and SpellEntry share a common name so that objects derived from the two share the same metatable and thus all the methods associated w/ that metatable.
		m	.class_decl<SpellEntry>("SpellEntry");
		m	.class_<lspellentry>("SpellEntry")
			prop(Id)
			prop(DispelType)
			prop(MechanicsType)
			prop(Attributes)
			prop(AttributesEx)
			prop(AttributesExB)
			prop(AttributesExC)
			prop(AttributesExD)
			prop(AttributesExE)
			prop(AttributesExF)
			prop(RequiredShapeShift)
			prop(Targets)
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
			prop(procCharges)
			prop(maxLevel)
			prop(baseLevel)
			prop(spellLevel)
			prop(DurationIndex)
			prop(powerType)
			prop(manaCost)
			prop(manaCostPerlevel)
			prop(manaPerSecond)
			prop(manaPerSecondPerLevel)
			prop(rangeIndex)
			prop(speed)
			prop(modalNextSpell)
			prop(maxstack)
			prop(School)
			prop(c_is_flags)
			prop(NameHash)
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
			.method("gettype", &Spell::GetType)
			.method("getstate", &Spell::getState)
			.method("cancel", &Spell::cancel)
			.method("cast", &Spell::cast)
			.method("finish", &Spell::finish)
			/*.property_ro("u_target", &Spell::unitTarget)
			.property_ro("i_target", &Spell::itemTarget)
			.property_ro("g_target", &Spell::gameObjTarget)
			.property_ro("p_target", &Spell::playerTarget)*/
			.method("getproto", &Spell::GetProto)
			.method("isaspect", &Spell::IsAspect)
			.method("isseal", &Spell::IsSeal)
			.method("createprotooverride", &Spell::InitProtoOverride)
			.method("getduration", &Spell::GetDuration)
			.method("getradius", &Spell::GetRadius)
			.method("isstealthspell", &Spell::IsStealthSpell)
			.method("isinvisibilityspell", &Spell::IsInvisibilitySpell)
			.property_ro("triggered", &Spell::m_triggeredSpell)
			.property_ro("reflectedfrom", &Spell::m_reflectedParent)
			.method("isreflected", &Spell::IsReflected)
			.property_ro("proto", &Spell::m_spellInfo)
			.property_ro("proto_override", &Spell::m_spellInfo_override);/*
#define prop(name) .property_rw(#name, &AI_Spell::name)
		//AI_Spell, constructed and destroyed in Lua
		m	.class_<AI_Spell>("AI_Spell", true)
			.constructor< void(*)() >()
			prop(proto)
			prop(cooldown)
			prop(interval)
			prop(event_type)
			prop(difficulty)
			prop(spell_agent)
			prop(type)
			prop(spellTargeType)
			prop(procChance)
			prop(maxcastcount)
			prop(currentcastcount)
			prop(minrange)
			prop(maxrange)
			prop(rdata)
			prop(interact)
			prop(isNotProtoSpell);
#undef prop*/
	}
}
