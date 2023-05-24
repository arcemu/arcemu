/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

#include "Setup.h"

bool OrbOfDeceptionAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		if(target->getRace() == RACE_ORC)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10139);
			else
				target->SetDisplayId(10140);
		}
		else
		if(target->getRace() == RACE_TAUREN)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10136);
			else
				target->SetDisplayId(10147);
		}
		else
		if(target->getRace() == RACE_TROLL)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10135);
			else
				target->SetDisplayId(10134);
		}
		else
		if(target->getRace() == RACE_UNDEAD)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10146);
			else
				target->SetDisplayId(10145);
		}
		else
		if(target->getRace() == RACE_BLOODELF)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(17829);
			else
				target->SetDisplayId(17830);
		}
		else
		if(target->getRace() == RACE_GNOME)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10148);
			else
				target->SetDisplayId(10149);
		}
		else
		if(target->getRace() == RACE_DWARF)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10141);
			else
				target->SetDisplayId(10142);
		}
		else
		if(target->getRace() == RACE_HUMAN)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10137);
			else
				target->SetDisplayId(10138);
		}
		else
		if(target->getRace() == RACE_NIGHTELF)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(10143);
			else
				target->SetDisplayId(10144);
		}
		else
		if(target->getRace() == RACE_DRAENEI)
		{
			if(target->getGender() == 0)
				target->SetDisplayId(17827);
			else
				target->SetDisplayId(17828);
		}
	}
	else
	{
		target->SetDisplayId(target->GetNativeDisplayId());
	}

	return true;
}

bool IllusionBlackDragonkinAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		// TODO: Sniff the spell / item, we need to know the real displayID
		// guessed this may not be correct
		// human = 7820
		// dwarf = 7819
		// Halfling = 7818
		// maybe 7842 as its from a lesser npc
		target->SetDisplayId( 7842 );
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
	}

	return true;
}

bool WispAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		target->SetDisplayId( 10045 );
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
	}

	return true;
}

bool RedOgreCustomAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		target->SetDisplayId( 11549 );
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
	}

	return true;
}

bool TimeLostFigurineAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		target->SetDisplayId( 18628 );
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
	}

	return true;
}

bool MurlocCostumeAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *target = aura->GetTarget();

	if( apply )
	{
		target->SetDisplayId( 21723 );
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
	}

	return true;
}

void setupTransformSpellHandlers( ScriptMgr *mgr )
{
	mgr->register_dummy_aura( 16739, &OrbOfDeceptionAuraHandler );
	mgr->register_dummy_aura( 19937, &IllusionBlackDragonkinAuraHandler );
	mgr->register_dummy_aura( 20584, &WispAuraHandler );
	mgr->register_dummy_aura( 30167, &RedOgreCustomAuraHandler );
	mgr->register_dummy_aura( 41301, &TimeLostFigurineAuraHandler );
	mgr->register_dummy_aura( 42365, &MurlocCostumeAuraHandler );
}
