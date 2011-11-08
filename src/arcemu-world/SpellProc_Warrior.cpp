/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

class DamageShieldSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(DamageShieldSpellProc);

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			// Allow only proc for player unit
			if(! mTarget->IsPlayer())
				return false;
			return true;
		}

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			Player* plr = TO_PLAYER(mTarget);

			dmg_overwrite[0] = plr->GetBlockDamageReduction() * (mOrigSpell->EffectBasePoints[0] + 1) / 100;

			// plr->GetBlockDamageReduction() returns ZERO if player has no shield equipped
			if(dmg_overwrite[0] == 0)
				return true;

			return false;
		}
};


class JuggernautSpellProc : public SpellProc{
	SPELL_PROC_FACTORY_FUNCTION( JuggernautSpellProc );

	bool CanProc(Unit* victim, SpellEntry* CastingSpell){
		if( CastingSpell == NULL )
			return false;

		if( CastingSpell->NameHash == SPELL_HASH_CHARGE )
			return true;
		else
			return false;
	}
};

void SpellProcMgr::SetupWarrior()
{
	AddByNameHash(SPELL_HASH_DAMAGE_SHIELD, &DamageShieldSpellProc::Create);
	AddById( 65156, &JuggernautSpellProc::Create );
}