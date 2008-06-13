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

bool Refocus(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->GetPlayerTarget();
    if(playerTarget == 0) return true;

    SpellSet::const_iterator itr = playerTarget->mSpells.begin();
    for(; itr != playerTarget->mSpells.end(); ++itr)
    {
        if((*itr) == 24531)       // skip calling spell.. otherwise spammies! :D
            continue;

        if((*itr) == 19434 || (*itr) == 20900 || (*itr) == 20901 || (*itr) == 20902 || (*itr) == 20903 || (*itr) == 20904 || (*itr) == 27632
            || (*itr) == 2643 || (*itr) == 14288|| (*itr) == 14289|| (*itr) == 14290 || (*itr) == 25294 || (*itr) == 14443 || (*itr) == 18651 || (*itr) == 20735 || (*itr) == 21390
            || (*itr) == 1510 || (*itr) == 14294 || (*itr) == 14295 || (*itr) == 1540 || (*itr) == 22908
            || (*itr) == 3044 || (*itr) == 14281 || (*itr) == 14282 || (*itr) == 14283 || (*itr) == 14284 || (*itr) == 14285 || (*itr) == 14286 || (*itr) == 14287)
            playerTarget->ClearCooldownForSpell((*itr));
    }
    return true;
}

bool Readiness(uint32 i, Spell * pSpell)
{
    if(!pSpell->p_caster)
		return true;
    pSpell->p_caster->ClearCooldownsOnLine(50 , pSpell->m_spellInfo->Id);//Beast Mastery
    pSpell->p_caster->ClearCooldownsOnLine(163, pSpell->m_spellInfo->Id);//Marksmanship
    pSpell->p_caster->ClearCooldownsOnLine(51 , pSpell->m_spellInfo->Id);//Survival
    return true;
}

void SetupHunterSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(24531, &Refocus);
    mgr->register_dummy_spell(23989, &Readiness);
}
