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

#include "StdAfx.h"
#include "Setup.h"

bool Penance(uint32 i, Spell * pSpell)
{
	if( !pSpell->p_caster || !pSpell->p_caster->isAlive() || 
		!pSpell->GetUnitTarget() || !pSpell->GetUnitTarget()->isAlive() )
		return false;

	Unit *target = pSpell->GetUnitTarget();
	Player *player = pSpell->p_caster;

	// index 0 contains the spell for the first tick, index 1 is the peroidic cast spell.
	uint32 hostileSpell[] = {0, 0};
	uint32 friendlySpell[] = {0, 0};

	switch( pSpell->GetProto()->Id )
	{
	case 47540: //Rank 1
		hostileSpell[0] = 47666;
		hostileSpell[1] = 47758;

		friendlySpell[0] = 47750;
		friendlySpell[1] = 47757;
		break;
	case 53005:
		hostileSpell[0] = 52998;
		hostileSpell[1] = 53001;

		friendlySpell[0] = 52983;
		friendlySpell[1] = 52986;
		break;
	case 53006:
		hostileSpell[0] = 52999;
		hostileSpell[1] = 53002;

		friendlySpell[0] = 52984;
		friendlySpell[1] = 52987;
		break;
	case 53007:
		hostileSpell[0] = 53000;
		hostileSpell[1] = 53003;

		friendlySpell[0] = 52985;
		friendlySpell[1] = 52988;
		break;
	}

	if( isAttackable(player, target) ) // Do holy damage
	{
		// First tick is instant.
		player->CastSpell(target, hostileSpell[0], true);
		player->CastSpell(target, hostileSpell[1], false);
	}
	else // Heal
	{ 
		player->CastSpell(target, friendlySpell[0], true);
		player->CastSpell(target, friendlySpell[1], false);
	}
	return true;
}

void SetupPriestSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(47540, &Penance);
	mgr->register_dummy_spell(53005, &Penance);
	mgr->register_dummy_spell(53006, &Penance);
	mgr->register_dummy_spell(53007, &Penance);
}

