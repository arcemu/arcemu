/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
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

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool RockbiterWeapon(uint32 i, Spell* pSpell)
{
    uint32 enchantment_entry = 0;
    switch(pSpell->GetProto()->RankNumber)
    {
    case 1:
        enchantment_entry = 3021;
        break;
    case 2:
        enchantment_entry = 3024;
        break;
    case 3:
        enchantment_entry = 3027;
        break;
    case 4:
        enchantment_entry = 3030;
        break;
    case 5:
        enchantment_entry = 3033;
        break;
    case 6:
        enchantment_entry = 3036;
        break;
    case 7:
        enchantment_entry = 3039;
        break;
    case 8:
        enchantment_entry = 3042;
        break;
    case 9:
        enchantment_entry = 3018;
        break;
    }

    if(!enchantment_entry || !pSpell->p_caster)
        return true;

    Item * item = pSpell->p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
    EnchantEntry * enchant = dbcEnchant.LookupEntry(enchantment_entry);
    if(!item || !enchant)
        return true;

    int32 Slot = item->HasEnchantment(enchant->Id);
    if(Slot >= 0)
        item->ModifyEnchantmentTime(Slot, 1800);
    else
    {
		//check if enchantment slot 1 is taken. If there was no enchantment there function will quit
		item->RemoveEnchantment(1);
		//this will also apply bonuses for us
        Slot = item->AddEnchantment(enchant, 1800, false, true, false, 1);   // 5min
        if(Slot < 0) return true;
    }

    sLog.outDebug("ShamanSpells.cpp :: Rockbiter Weapon Rank %u, enchant %u, slot %u", pSpell->GetProto()->RankNumber,
        enchantment_entry, Slot);
    
    return true;
}

void SetupShamanSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(8017, &RockbiterWeapon); // rank 1
    mgr->register_dummy_spell(8018, &RockbiterWeapon); // rank 2
    mgr->register_dummy_spell(8019, &RockbiterWeapon); // rank 3
    mgr->register_dummy_spell(10399, &RockbiterWeapon);// rank 4
    mgr->register_dummy_spell(16314, &RockbiterWeapon);// rank 5
    mgr->register_dummy_spell(16315, &RockbiterWeapon);// rank 6
    mgr->register_dummy_spell(16316, &RockbiterWeapon);// rank 7
	mgr->register_dummy_spell(25479, &RockbiterWeapon);// rank 8
    mgr->register_dummy_spell(25485, &RockbiterWeapon);// rank 9
}
