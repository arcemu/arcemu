/*
 * Why Scripts for Arcemu MMORPG Server <http://www.arcemu.org/>
 * Copyright (C) 2009 WhyScripts Team <http://www.whydb.org/>
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

// quest 5163 - Are We There, Yeti?
bool ReleaseUmisYeti( uint32 i, Spell * pSpell )
{
	if ( pSpell->p_caster == NULL || pSpell->GetUnitTarget() == NULL || !pSpell->GetUnitTarget()->IsCreature() )
		return true;

	QuestLogEntry * qLogEntry = pSpell->p_caster->GetQuestLogForEntry( 5163 );
	if ( qLogEntry == NULL )
		return true;

	Creature * target = static_cast< Creature * >( pSpell->GetUnitTarget() );
	static const uint32 friends[] = { 10978, 7583, 10977 };
	for ( uint32 j = 0; j < sizeof( friends ) / sizeof( uint32 ); j++ )
	{
		if ( target->GetEntry() == friends[j] && qLogEntry->GetMobCount( j ) < qLogEntry->GetQuest()->required_mobcount[j] )
		{
			qLogEntry->SetMobCount( j, 1 );
			qLogEntry->SendUpdateAddKill( j );
			qLogEntry->UpdatePlayerFields();
			return true;
		}
	}

	return true;
}
void SetupWinterspring( ScriptMgr * mgr )
{
	mgr->register_dummy_spell( 17166, &ReleaseUmisYeti );
}