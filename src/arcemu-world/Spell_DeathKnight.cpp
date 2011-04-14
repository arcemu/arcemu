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

class BloodPlagueSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(BloodPlagueSpell);

	int32 DoCalculateEffect(uint32 i, Unit *target, int32 value)
	{
		if( p_caster != NULL && i == 0 )
			value += (uint32)( p_caster->GetAP() * 0.055 * 1.15 );

		return value;
	}
};

class IcyTouchSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(IcyTouchSpell);

	int32 DoCalculateEffect(uint32 i, Unit *target, int32 value)
	{
		if( p_caster != NULL && i == 0 )
			value += (uint32)( p_caster->GetAP() * 0.1 );

		return value;
	}
};

class FrostFeverSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(FrostFeverSpell);

	int32 DoCalculateEffect(uint32 i, Unit *target, int32 value)
	{
		if( p_caster != NULL && i == 0 )
			value += (uint32)( p_caster->GetAP() * 0.055 * 1.15 );

		return value;
	}
};

class BloodBoilSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(BloodBoilSpell);

	int32 DoCalculateEffect(uint32 i, Unit *target, int32 value)
	{
		if( p_caster != NULL && i == 0 )
		{
			int32 ap = p_caster->GetAP();

			value += (uint32)( ap * 0.08 );

			// Does additional damage if target has diseases (http://www.tankspot.com/forums/f14/48814-3-1-blood-boil-mechanics-tested.html)
			if( target != NULL && ( target->HasAura(55078) || target->HasAura(55095) ) )
				value += (uint32)( ap * 0.015 + 95 );
		}

		return value;
	}
};

class BloodStrikeSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(BloodStrikeSpell);

	int32 DoCalculateEffect(uint32 i, Unit *target, int32 value)
	{
		if( target != NULL )
		{
			uint32 count = target->GetAuraCountWithDispelType( DISPEL_DISEASE, m_caster->GetGUID() );
			if( count )
				value += value * count * (GetProto()->EffectBasePoints[2] +1) / 200;
		}

		return value;
	}
};

class DeathCoilSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(DeathCoilSpell);

	uint8 CanCast(bool tolerate)
	{
		uint8 result = Spell::CanCast(tolerate);

		if( result == SPELL_CANCAST_OK )
		{
			if( m_caster != NULL && m_caster->IsInWorld() )
			{
				Unit *target = m_caster->GetMapMgr()->GetUnit( m_targets.m_unitTarget );

				if( target == NULL || ! ( isAttackable(m_caster, target, false) || target->getRace() == RACE_UNDEAD ) )
					result = SPELL_FAILED_BAD_TARGETS;
			}
		}

		return result;
	}
};

class RuneStrileSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(RuneStrileSpell);

	void HandleEffects(uint64 guid, uint32 i)
	{
		Spell::HandleEffects(guid, i);

		if( u_caster != NULL )
			u_caster->RemoveAura( 56817 );
	}
};

void SpellFactoryMgr::SetupDeathKnight()
{
	AddById( 55078, &BloodPlagueSpell::Create );
	AddById( 45477, &IcyTouchSpell::Create );
	AddById( 55095, &FrostFeverSpell::Create );

	AddById( 48721, &BloodBoilSpell::Create ); // Rank 1
	AddById( 49939, &BloodBoilSpell::Create ); // Rank 2
	AddById( 49940, &BloodBoilSpell::Create ); // Rank 3
	AddById( 49941, &BloodBoilSpell::Create ); // Rank 4

	AddById( 45902, &BloodStrikeSpell::Create ); // Rank 1
	AddById( 49926, &BloodStrikeSpell::Create ); // Rank 2
	AddById( 49927, &BloodStrikeSpell::Create ); // Rank 3
	AddById( 49928, &BloodStrikeSpell::Create ); // Rank 4
	AddById( 49929, &BloodStrikeSpell::Create ); // Rank 5
	AddById( 49930, &BloodStrikeSpell::Create ); // Rank 6

	AddById( 47541, &DeathCoilSpell::Create ); // Rank 1
	AddById( 49892, &DeathCoilSpell::Create ); // Rank 2
	AddById( 49893, &DeathCoilSpell::Create ); // Rank 3
	AddById( 49894, &DeathCoilSpell::Create ); // Rank 4
	AddById( 49895, &DeathCoilSpell::Create ); // Rank 5

	AddById( 56815, &RuneStrileSpell::Create );
}
