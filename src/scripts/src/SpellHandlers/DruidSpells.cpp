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

bool Starfall(uint32 i, Spell* pSpell)
{
	Unit* m_caster = pSpell->u_caster;
	if( m_caster == NULL )
		return true;
	uint8 am = 0;
	for (Object::InRangeSet::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); ++itr)
	{
		if(!(*itr)->IsUnit())
			continue;
		Unit* Target = static_cast<Unit*>((*itr));
		if(isAttackable(Target, m_caster) && m_caster->CalcDistance((*itr)) <= pSpell->GetRadius(i))
		{
			m_caster->CastSpell(Target, pSpell->CalculateEffect(i, Target), true);
			++am;
			if(am >= 2)
				return true;
		}
	}
	return true;
}

void SetupDruidSpells(ScriptMgr * mgr)
{
	uint32 StarfallIds[] =
	{
		50286, // Rank 1
		53196, // Rank 2
		53197, // Rank 3
		53198, // Rank 4
		0,
	};
	mgr->register_dummy_spell(StarfallIds, &Starfall); 
}
