/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

class FrostBrandAttackSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(FrostBrandAttackSpellProc);

	void Init(Object *obj)
	{		
		mProcChance = TO_ITEM(obj)->GetProto()->Delay * 9 / 600;
	}
};

class FlametongueWeaponSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(FlametongueWeaponSpellProc);

	bool CanProc(Unit *victim, SpellEntry *CastingSpell)
	{
		if ( mTarget->IsPlayer() )
			return true;
		return false;
	}

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite)
	{
		Item *item;
		EnchantmentInstance *enchant;
		SpellEntry *sp = NULL;
		uint32 wp_speed = 0;

		item = TO_PLAYER(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
		if ( item != NULL )
		{
			enchant = item->GetEnchantment(TEMP_ENCHANTMENT_SLOT);
			if ( enchant != NULL )
			{
				sp = dbcSpell.LookupEntryForced(enchant->Enchantment->spell[0]);
				if ( sp != NULL && sp->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON__PASSIVE_ )
					wp_speed = item->GetProto()->Delay;
			}
		}

		//this part will be enabled once HandleProc() will get as additional parameter if melee damage is done by main or off hand weapons.
		//at the moment Flametongue procs even if the damage is done by the other hand.
		/*if ( ! wp_speed )
		{
			item = TO_PLAYER(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
			if ( item != NULL )
			{
				enchant = item->GetEnchantment(TEMP_ENCHANTMENT_SLOT);
				if ( enchant != NULL )
				{
					sp = dbcSpell.LookupEntryForced(enchant->Enchantment->spell[0]);
					if ( sp != NULL && sp->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON__PASSIVE_ )
						wp_speed = item->GetProto()->Delay;
				}
			}
		}*/

		if ( wp_speed )
		{
			*dmg_overwrite = (sp->EffectBasePoints[0] + 1) * wp_speed / 100000;
			return false;
		}
		
		return true;
	}
};

void SpellProcMgr::SetupShamman()
{
	AddByNameHash( SPELL_HASH_FROSTBRAND_ATTACK, &FrostBrandAttackSpellProc::Create );

	AddById( 10444, &FlametongueWeaponSpellProc::Create );
}
