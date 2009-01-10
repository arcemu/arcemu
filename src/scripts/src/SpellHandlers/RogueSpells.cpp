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
     Player * playerTarget = pSpell->p_caster; 
     if(playerTarget == 0) return true; 

     playerTarget->ClearCooldownForSpell( 5277 );        // Evasion Rank 1  
     playerTarget->ClearCooldownForSpell( 26669 );       // Evasion Rank 2  
     playerTarget->ClearCooldownForSpell( 2983 );        // Sprint Rank 1  
     playerTarget->ClearCooldownForSpell( 8696 );        // Sprint Rank 2  
     playerTarget->ClearCooldownForSpell( 11305 );       // Sprint Rank 3  
     playerTarget->ClearCooldownForSpell( 1856 );        // Vanish Rank 1  
     playerTarget->ClearCooldownForSpell( 1857 );        // Vanish Rank 2  
     playerTarget->ClearCooldownForSpell( 26889 );       // Vanish Rank 3  
     playerTarget->ClearCooldownForSpell( 14177 );       // Cold Blood  
     playerTarget->ClearCooldownForSpell( 14183 );       // Premeditation  
     playerTarget->ClearCooldownForSpell( 36554 );       // Shadowstep  
     return true;  
}

void SetupRogueSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(14185, &Preparation);
}
