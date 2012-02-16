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

class FrostBrandAttackSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(FrostBrandAttackSpellProc);

		void Init(Object* obj)
		{
			if(obj == NULL)
			{
				mDeleted = true;
				return;
			}

			mProcChance = TO< Item* >(obj)->GetProto()->Delay * 9 / 600;
		}
};

class EarthShieldSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(EarthShieldSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			int32 value = mOrigSpell->EffectBasePoints[0];
			dmg_overwrite[0] = value;

			return false;
		}

		void CastSpell(Unit* victim, SpellEntry* CastingSpell, int* dmg_overwrite)
		{
			Unit* caster = mTarget->GetMapMgr()->GetUnit(mCaster);
			if(caster == NULL)
				return;

			Spell* spell = sSpellFactoryMgr.NewSpell(caster, mSpell, true, NULL);
			SpellCastTargets targets(mTarget->GetGUID());
			spell->prepare(&targets);
		}

};

class FlametongueWeaponSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(FlametongueWeaponSpellProc);

		void Init(Object* obj)
		{
			if(obj == NULL)
			{
				mDeleted = true;
				//initializing anyway all fields.
				mItemGUID = 0;
				damage = 0;
				return;
			}

			mItemGUID = obj->GetGUID();
			damage = 0;
			uint32 wp_speed;
			Item* item = TO< Item* >(obj);
			EnchantmentInstance* enchant = item->GetEnchantment(TEMP_ENCHANTMENT_SLOT);
			if(enchant != NULL)
			{
				SpellEntry* sp = dbcSpell.LookupEntryForced(enchant->Enchantment->spell[0]);
				if(sp != NULL && sp->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON__PASSIVE_)
				{
					wp_speed = item->GetProto()->Delay;
					damage = (sp->EffectBasePoints[0] + 1) * wp_speed / 100000;
				}
			}
		}

		bool CanDelete(uint32 spellId, uint64 casterGuid = 0, uint64 misc = 0)//in this case misc is the item guid.
		{
			if(mSpell->Id == spellId && mCaster == casterGuid && misc == mItemGUID && !mDeleted)
				return true;

			return false;
		}

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			if(mTarget->IsPlayer())
				return true;
			return false;
		}

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			Item* item;

			if(weapon_damage_type == OFFHAND)
				item = TO< Player* >(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
			else
				item = TO< Player* >(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

			if(item != NULL && item->GetGUID() == mItemGUID)
			{
				dmg_overwrite[0] = damage;
				return false;
			}

			return true;
		}

	private:
		uint64 mItemGUID;
		int damage;
};

void SpellProcMgr::SetupShamman()
{
	AddByNameHash(SPELL_HASH_FROSTBRAND_ATTACK, &FrostBrandAttackSpellProc::Create);

	AddById(10444, &FlametongueWeaponSpellProc::Create);
	AddById(379, &EarthShieldSpellProc::Create);
}
