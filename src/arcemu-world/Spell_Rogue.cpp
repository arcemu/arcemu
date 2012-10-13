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

class CheatDeathAura : public AbsorbAura
{
	public:
		static Aura* Create(SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary = false, Item* i_caster = NULL) { return new CheatDeathAura(proto, duration, caster, target, temporary, i_caster); }

		CheatDeathAura(SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary = false, Item* i_caster = NULL)
			: AbsorbAura(proto, duration, caster, target, temporary, i_caster)
		{
			dSpell = dbcSpell.LookupEntry(31231);
		}

		uint32 AbsorbDamage(uint32 School, uint32* dmg)
		{
			// Checking for 1 min cooldown
			if(dSpell == NULL || ! p_target->Cooldown_CanCast(dSpell))
				return 0;

			// Check for proc chance
			if(RandomFloat(100.0f) > GetSpellProto()->EffectBasePoints[0] + 1)
				return 0;

			// Check if damage will kill player.
			uint32 cur_hlth = p_target->GetHealth();
			if((*dmg) < cur_hlth)
				return 0;

			uint32 max_hlth = p_target->GetMaxHealth();
			uint32 min_hlth = max_hlth / 10;

			/*
				looks like the following lines are not so good, we check and cast on spell id 31231_
				and adding the cooldown to it, but it looks like this spell is useless(all it's doing is_
				casting 45182, so we can do all this stuff on 45182 at first place), BUT_
				as long as proceeding cheat death is not so height (how many rogue at the same time_
				gonna get to this point?) so it's better to use it because we wont lose anything!!
			*/
			p_target->CastSpell(p_target->GetGUID(), dSpell, true);

			// set dummy effect,
			// this spell is used to procced the post effect of cheat death later.
			// Move next line to SPELL::SpellEffectDummy ?!! well it's better in case of dbc changing!!
			p_target->CastSpell(p_target->GetGUID(), 45182, true);

			// Better to add custom cooldown procedure then fucking with entry, or not!!
			dSpell->RecoveryTime = 60000;
			p_target->Cooldown_Add(dSpell, NULL);

			// Calc abs and applying it
			uint32 real_dmg = (cur_hlth > min_hlth ? cur_hlth - min_hlth : 0);
			uint32 absorbed_dmg = *dmg - real_dmg;

			*dmg = real_dmg;
			return absorbed_dmg;
		}

	private:
		SpellEntry* dSpell;
};

void SpellFactoryMgr::SetupRogue()
{
	AddAuraById(31228, &CheatDeathAura::Create);   // Rank 1
	AddAuraById(31229, &CheatDeathAura::Create);   // Rank 2
	AddAuraById(31230, &CheatDeathAura::Create);   // Rank 3
}
