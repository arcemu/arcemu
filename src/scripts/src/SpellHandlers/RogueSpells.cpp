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

bool Preparation(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->p_caster;
    if(playerTarget == 0) return true;

    playerTarget->ClearCooldownsOnLine(39, pSpell->GetProto()->Id); // line - subtlety
    playerTarget->ClearCooldownsOnLine(38, pSpell->GetProto()->Id); // line - combat
    playerTarget->ClearCooldownsOnLine(253, pSpell->GetProto()->Id);// line - assassination
    return true;
}

void SetupRogueSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(14185, &Preparation);
}
