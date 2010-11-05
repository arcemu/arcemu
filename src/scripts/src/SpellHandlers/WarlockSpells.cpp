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

#include "Setup.h"

bool SoulLink( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if(!s->u_caster || !s->u_caster->isAlive() || !unitTarget || !unitTarget->isAlive())
		return false;
	
	uint32 pet_dmg = s->forced_basepoints[0]*20/100;
	if( pet_dmg )
	{
		unitTarget->ModHealth(pet_dmg);
		unitTarget->DealDamage( s->u_caster,pet_dmg,0,0,25228,true);
	}

	return true;
}

bool LifeTap( uint32 i, Spell *s ){
	Player *playerTarget = s->GetPlayerTarget();

	if(!s->p_caster || !playerTarget)
		return false;
	
	uint32 mod;	// spirit bonus coefficient multiplied by 2
	if( s->GetProto()->Id == 1454) mod = 2;
	else if(s->GetProto()->Id == 1455) mod = 3;
	else if(s->GetProto()->Id == 1456) mod = 4;
	else if(s->GetProto()->Id == 11687) mod = 5;
	else mod = 6;
	
	uint32 damage = s->GetProto()->EffectBasePoints[i] + 1 + mod * playerTarget->GetStat(STAT_SPIRIT) / 2;
	
	if (damage >= playerTarget->GetHealth())
		return false;
	
	s->p_caster->DealDamage(playerTarget,damage,0,0, s->GetProto()->Id);
	damage = damage * (100 + playerTarget->m_lifetapbonus) / 100;	// Apply improved life tap
	if(playerTarget->GetPower( POWER_TYPE_MANA ) + damage > playerTarget->GetMaxPower( POWER_TYPE_MANA ))
		playerTarget->SetPower( POWER_TYPE_MANA,playerTarget->GetMaxPower( POWER_TYPE_MANA ));
	else
		playerTarget->SetPower( POWER_TYPE_MANA, playerTarget->GetPower( POWER_TYPE_MANA ) + damage);
	s->SendHealManaSpellOnPlayer( s->p_caster, playerTarget, damage, POWER_TYPE_MANA, s->GetProto()->Id );

	return true;
}

bool SoulShatter( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if( !s->u_caster || !s->u_caster->isAlive() || !unitTarget || !unitTarget->isAlive() )
		return false;
	
	s->u_caster->CastSpell(unitTarget, 32835, false);

	return true;
}

void SetupWarlockSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell( 25228, &SoulLink );

	mgr->register_dummy_spell( 1455, &LifeTap );
	mgr->register_dummy_spell( 1456, &LifeTap );
	mgr->register_dummy_spell( 1457, &LifeTap );
	mgr->register_dummy_spell( 11687, &LifeTap );
	mgr->register_dummy_spell( 11688, &LifeTap );
	mgr->register_dummy_spell( 11689, &LifeTap );
	mgr->register_dummy_spell( 27222, &LifeTap );
	mgr->register_dummy_spell( 57946, &LifeTap );

	mgr->register_dummy_spell( 29858, &SoulShatter );


}
