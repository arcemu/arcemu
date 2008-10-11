/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team arcemu
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

bool Preparation(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->p_caster;
    if(playerTarget == 0)
		return true;

    playerTarget->ClearCooldownForSpell( 5277 );	// Evasion Rank 1
    playerTarget->ClearCooldownForSpell( 26669 );	// Evasion Rank 2
    playerTarget->ClearCooldownForSpell( 2983 );	// Sprint Rank 1
    playerTarget->ClearCooldownForSpell( 8696 );	// Sprint Rank 2
    playerTarget->ClearCooldownForSpell( 11305 );	// Sprint Rank 3
    playerTarget->ClearCooldownForSpell( 1856 );	// Vanish Rank 1
    playerTarget->ClearCooldownForSpell( 1857 );	// Vanish Rank 2
    playerTarget->ClearCooldownForSpell( 26889 );	// Vanish Rank 3
    playerTarget->ClearCooldownForSpell( 14177 );	// Cold Blood
    playerTarget->ClearCooldownForSpell( 14183 );	// Premeditation
    playerTarget->ClearCooldownForSpell( 36554 );	// Shadowstep

    return true;
}

void SetupRogueSpells(ScriptMgr * mgr)
{
    //mgr->register_dummy_spell(14185, &Preparation);
}
