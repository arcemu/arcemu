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
	pSpell->p_caster->ClearCooldownForSpell( 36554 );       // Shadowstep  
	if( pSpell->p_caster->HasAura( 56819 ) )                // Glyph of Preparation item = 42968 casts 57127 that apply aura 56819.
	{
		pSpell->p_caster->ClearCooldownForSpell( 13877 );   // Blade Flurry
		pSpell->p_caster->ClearCooldownForSpell( 51722 );   // Dismantle
		pSpell->p_caster->ClearCooldownForSpell( 1766 );    // Kick
	}
	return true;  
}

bool Shiv(uint32 i, Spell *pSpell){
	Unit *pTarget = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !pTarget) return true;

	pSpell->p_caster->CastSpell(pTarget->GetGUID(), 5940, true);

	Item *it = pSpell->p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if(!it) return true;

	EnchantmentInstance * ench = it->GetEnchantment( TEMP_ENCHANTMENT_SLOT );
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
		if( unitTarget->GetTargetGUID() != 0 )
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
	p_caster->SafeTeleport(p_caster->GetMapId(), p_caster->GetInstanceID(), LocationVector(new_x, new_y, (unitTarget->GetPositionZ() + 0.1f), ang));

	return true;
}

bool ImprovedSprint(uint32 i, Spell* pSpell)
{
	if( i == 0 )
	{
		Unit *target = pSpell->GetUnitTarget();
		if (target == NULL)
			return true;

		target->RemoveAllAurasByMechanic(MECHANIC_ENSNARED, -1, true);
		target->RemoveAllAurasByMechanic(MECHANIC_ROOTED, -1, true);
	}
	
	return true;
}

bool CutToTheChase(uint32 i, Aura *pAura, bool apply)
{
	Unit *target = pAura->GetTarget();

	if (apply)
	{
		static uint32 classMask[3] = { 0x20000, 0x8, 0 };
		target->AddProcTriggerSpell(pAura->GetSpellProto(), pAura->GetSpellProto(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_CAST_SPELL | PROC_TARGET_SELF, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(pAura->GetSpellId(), pAura->m_casterGuid);

	return true;
}

bool DeadlyBrew(uint32 i, Aura *pAura, bool apply)
{
	Unit *target = pAura->GetTarget();

	if (apply)
	{
		static uint32 classMask[3] = { 0x1000A000, 0, 0 };
		target->AddProcTriggerSpell(pAura->GetSpellProto(), pAura->GetSpellProto(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_CAST_SPELL , 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(pAura->GetSpellId(), pAura->m_casterGuid);

	return true;
}

void SetupRogueSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(5938, &Shiv);
	mgr->register_dummy_spell(14185, &Preparation);
	uint32 ShadowstepIds[] =
	{
		36554,
		36563,
		41176,
		44373,
		45273,
		46463,
		55965,
		55966,
		0,
	};
	mgr->register_dummy_spell(ShadowstepIds, &Shadowstep); // Alleycat - This should REALLY be handled in the core. It's just a hack fix here.

	mgr->register_dummy_spell(30918, &ImprovedSprint);

	uint32 CutToTheChaseIds[] = { 51664, 51665, 51667, 51668, 51669, 0 };
	mgr->register_dummy_aura(CutToTheChaseIds, &CutToTheChase);

	mgr->register_dummy_aura(51625, &DeadlyBrew);
	mgr->register_dummy_aura(51626, &DeadlyBrew);
}
