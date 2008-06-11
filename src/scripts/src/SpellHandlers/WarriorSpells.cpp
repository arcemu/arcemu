/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool Charge(uint32 i, Spell* pSpell)
{
    uint32 rage_to_gen;
    switch(pSpell->m_spellInfo->Id)
    {
    case 100:   // Charge Rank 1
        rage_to_gen = 90;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}		
        break;

    case 6178:  // Charge Rank 2
        rage_to_gen = 120;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}
        break;

    default:    // Charge Rank 3 +
        rage_to_gen = 150;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}

        break;
    }

    // Add the rage to the caster
    if(!pSpell->u_caster)
        return true;

    uint32 val = pSpell->u_caster->GetUInt32Value(UNIT_FIELD_POWER2);
    uint32 max = pSpell->u_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER2);
    val += rage_to_gen;
    if(val > max)
        val = max;
    
    pSpell->u_caster->SetUInt32Value(UNIT_FIELD_POWER2, val);
  //  script_debuglog("WarriorSpells.cpp :: Charge generate %u rage on "I64FMT, rage_to_gen, pSpell->u_caster->GetGUID());
    return true;
}

bool Execute(uint32 i, Spell* pSpell)
{
    //uint32 uSpellId = pSpell->m_spellInfo->Id;
    uint32 base_dmg = pSpell->damage;
    /*
    Attempt to finish off a wounded foe, causing 125 damage and converting each extra point
    of rage into 3 additional damage.  Only usable on enemies that have less than 20% health.
    */

    Unit * target = pSpell->GetUnitTarget();
    if(!target || !pSpell->u_caster) return true;

    // "Only usable on enemies that have less than 20% health."
    if(target->GetHealthPct() > 20)
    {
        // send failed
        pSpell->SendCastResult(SPELL_FAILED_BAD_TARGETS);
        return true;
    }

    // get the caster's rage points, and convert them
    // formula is 3 damage * spell rank * rage points
    uint32 add_damage = (3 * pSpell->m_spellInfo->RankNumber);
    add_damage *= pSpell->u_caster->GetUInt32Value(UNIT_FIELD_POWER2) / 10;   // rage is *10 always
    
    // send spell damage log
	//pSpell->u_caster->SpellNonMeleeDamageLog(target, 20647, base_dmg + add_damage, false);
	SpellEntry *sp_for_the_logs = dbcSpell.LookupEntry(20647);
	pSpell->u_caster->Strike( target, MELEE, sp_for_the_logs, base_dmg + add_damage, 0, 0, true, true );
	// zero rage
    pSpell->u_caster->SetUInt32Value(UNIT_FIELD_POWER2, 0);
    return true;
}

bool Bloodrage(uint32 i, Spell* pSpell)
{
  // Put the player in Combat (gotta find when to put him ooc)


  return true;
}

/* Module info */
void SetupWarriorSpells(ScriptMgr * mgr)
{
    /**** Charge ****/
    mgr->register_dummy_spell(100, &Charge);      // Rank 1
    mgr->register_dummy_spell(6178, &Charge);     // Rank 2
    mgr->register_dummy_spell(11578, &Charge);    // Rank 3

    /**** Execute ****/
    /* log isn't working */
    /*
      WORLD: got cast spell packet, spellId - 25236, data length = 9
      Spell::cast 25236, Unit: 2
      WORLD: Spell effect id = 3, damage = 925
      WORLD: Recvd CMSG_ATTACKSWING Message
      WORLD: Sent SMSG_ATTACKSTART
      Player::Update:  No valid current selection to attack, stopping attack
    */
    mgr->register_dummy_spell(5308, &Execute);     // Rank 1
    mgr->register_dummy_spell(20658, &Execute);    // Rank 2
    mgr->register_dummy_spell(20660, &Execute);    // Rank 3
    mgr->register_dummy_spell(20661, &Execute);    // Rank 4
    mgr->register_dummy_spell(20662, &Execute);    // Rank 5
    mgr->register_dummy_spell(25234, &Execute);    // Rank 6
    mgr->register_dummy_spell(25236, &Execute);    // Rank 7

    /**** Bloodrage ****/
    /* server debug when blood rages is cast */
    /*

    WORLD: got cast spell packet, spellId - 2687, data length = 6
    Spell::cast 2687, Unit: 2
    WORLD: Spell effect id = 30, damage = 100
    WORLD: Spell effect id = 64, damage = 1
    Spell::cast 29131, Unit: 2
    WORLD: Spell effect id = 6, damage = 10
    WORLD: Spell effect id = 6, damage = 1
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = true
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = true
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = false
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = false
    
    */
    mgr->register_dummy_spell(2687, &Bloodrage);    // Bloodrage
    mgr->register_dummy_spell(29131, &Bloodrage);   // Bloodrage this is also called
}
