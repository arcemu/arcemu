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

// Shadowstep
bool Shadowstep(uint32 i, Spell* pSpell)
{
	if( !pSpell )
	{
		return true;
	}
	Unit* unitTarget = pSpell->GetUnitTarget();
	Object* m_caster = pSpell->m_caster;
	Player* p_caster = pSpell->p_caster;
	if( !unitTarget || !m_caster || !p_caster )
	{
		return true;
	}

	/* this is rather tricky actually. we have to calculate the orientation of the creature/player, and then calculate a little bit of distance behind that. */
	float ang;
	if( unitTarget == m_caster )
	{
		/* try to get a selection */
		unitTarget = m_caster->GetMapMgr()->GetUnit(pSpell->p_caster->GetSelection());
		if( (!unitTarget ) || !isAttackable(pSpell->p_caster, unitTarget, !(pSpell->GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) || (unitTarget->CalcDistance(pSpell->p_caster) > 28.0f))
		{
			return true;
		}
	}

	if( unitTarget->GetTypeId() == TYPEID_UNIT )
	{
		if( unitTarget->GetUInt64Value( UNIT_FIELD_TARGET ) != 0 )
		{
			/* We're chasing a target. We have to calculate the angle to this target, this is our orientation. */
			ang = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionX(), unitTarget->GetPositionY());

			/* convert degree angle to radians */
			ang = ang * float(M_PI) / 180.0f;
		}
		else
		{
			/* Our orientation has already been set. */
			ang = unitTarget->GetOrientation();
		}
	}
	else
	{
		/* Players orientation is sent in movement packets */
		ang = unitTarget->GetOrientation();
	}

	// avoid teleporting into the model on scaled models
	const static float shadowstep_distance = 1.6f * unitTarget->GetFloatValue(OBJECT_FIELD_SCALE_X);
	float new_x = unitTarget->GetPositionX() - (shadowstep_distance * cosf(ang));
	float new_y = unitTarget->GetPositionY() - (shadowstep_distance * sinf(ang));

	/* Send a movement packet to "charge" at this target. Similar to warrior charge. */
	p_caster->z_axisposition = 0.0f;
	p_caster->SafeTeleport(p_caster->GetMapId(), p_caster->GetInstanceID(), LocationVector(new_x, new_y, (unitTarget->GetPositionZ() + 0.1f), unitTarget->GetOrientation()));

	return true;
}


void SetupRogueSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(5938, &Shiv);
	mgr->register_dummy_spell(14185, &Preparation);
	mgr->register_dummy_spell(36554, &Shadowstep);
	mgr->register_dummy_spell(36563, &Shadowstep);
	mgr->register_dummy_spell(41176, &Shadowstep);
	mgr->register_dummy_spell(44373, &Shadowstep);
	mgr->register_dummy_spell(45273, &Shadowstep);
	mgr->register_dummy_spell(46463, &Shadowstep);
	mgr->register_dummy_spell(55965, &Shadowstep);
	mgr->register_dummy_spell(55966, &Shadowstep);
}
