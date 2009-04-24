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

//Alice : Correct formula for Rogue - Preparation

bool Preparation(uint32 i, Spell * pSpell) 
{ 
	if(!pSpell->p_caster) return true; 

	pSpell->p_caster->ClearCooldownForSpell( 5277 );        // Evasion Rank 1  
	pSpell->p_caster->ClearCooldownForSpell( 26669 );       // Evasion Rank 2  
	pSpell->p_caster->ClearCooldownForSpell( 2983 );        // Sprint Rank 1  
	pSpell->p_caster->ClearCooldownForSpell( 8696 );        // Sprint Rank 2  
	pSpell->p_caster->ClearCooldownForSpell( 11305 );       // Sprint Rank 3  
	pSpell->p_caster->ClearCooldownForSpell( 1856 );        // Vanish Rank 1  
	pSpell->p_caster->ClearCooldownForSpell( 1857 );        // Vanish Rank 2  
	pSpell->p_caster->ClearCooldownForSpell( 26889 );       // Vanish Rank 3  
	pSpell->p_caster->ClearCooldownForSpell( 14177 );       // Cold Blood  
	pSpell->p_caster->ClearCooldownForSpell( 14183 );       // Premeditation  
	pSpell->p_caster->ClearCooldownForSpell( 36554 );       // Shadowstep  
	return true;  
}

bool Shiv(uint32 i, Spell *pSpell){
	Unit *pTarget = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !pTarget) return true;

	pSpell->p_caster->CastSpell(pTarget->GetGUID(), 5940, true);

	if( pSpell->p_caster->GetItemInterface() )
	{
		Item *it = pSpell->p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if(!it) return true;

		EnchantmentInstance * ench = it->GetEnchantment( 1 ); // temp enchantment slot
		if(ench) {
			EnchantEntry* Entry = ench->Enchantment;
			for( uint32 c = 0; c < 3; c++ )
			{
				if( Entry->type[c] && Entry->spell[c] )
				{
					SpellEntry *sp = dbcSpell.LookupEntry( Entry->spell[c] );
					if(!sp) return true;

					if( sp->c_is_flags & SPELL_FLAG_IS_POISON )
					{
						pSpell->p_caster->CastSpell(pTarget->GetGUID(),Entry->spell[c], true);
					}
				}
			}
		}
	}
	return true;
}

void SetupRogueSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(5938, &Shiv);
	mgr->register_dummy_spell(14185, &Preparation);
}
