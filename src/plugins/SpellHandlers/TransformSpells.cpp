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

bool PolymorphAuraHandler( uint32 i, Aura* aura, bool apply )
{
	Unit *caster = aura->GetUnitCaster();
	Unit *target = aura->GetTarget();

	uint32 displayId = 0;

	SpellEntry *spe = aura->GetSpellProto();

	if( spe->EffectMiscValue[ i ] == 0 )
	{
		LOG_ERROR( "No miscvalue for transform effect of spell %u", spe->Id );
	}
	else
	{
		CreatureInfo *ci = CreatureNameStorage.LookupEntry( spe->EffectMiscValue[ i ] );
		if( ci == NULL )
		{
			LOG_ERROR( "No creature data for creature %u while trying to apply a transform aura for spell %u", spe->EffectMiscValue[ i ], spe->Id );
		}
		else
		{
			displayId = ci->Male_DisplayID;
		}
	}

	if( displayId == 0 )
	{
		switch( spe->Id )
		{
		case 28270:	 // Cow
			displayId = 1060;
			break;
		
		case 28272:	 // Pig
			displayId = 16356 + RandomUInt(2);
			break;
		
		case 28271:	 // Turtle
			displayId = 16359 + RandomUInt(2);
			break;
		
		default:
			displayId = 856;
			break;
		}
	}

	if( apply )
	{
		if( target->IsCreature() )
			target->GetAIInterface()->AttackReaction( caster, 1, spe->Id );
		
		target->SetDisplayId( displayId );
		
		// remove the current spell (for channalers)
		if( target->m_currentSpell && target->GetGUID() != caster->GetGUID() && target->m_currentSpell->getState() == SPELL_STATE_CASTING )
		{
			target->m_currentSpell->cancel();
			target->m_currentSpell = 0;
		}
		
		sEventMgr.AddEvent( aura, &Aura::EventPeriodicHeal1, (uint32)1000, EVENT_AURA_PERIODIC_HEAL, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
		target->polySpell = spe->Id;
	}
	else
	{
		target->SetDisplayId( target->GetNativeDisplayId() );
		target->polySpell = 0;
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
	
	
	mgr->register_dummy_aura( 118, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 851, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 12824, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 12825, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 12826, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 13323, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 15534, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 22274, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 23603, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 28270, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 28271, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 28272, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 61025, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 61305, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 61721, &PolymorphAuraHandler );
	mgr->register_dummy_aura( 61780, &PolymorphAuraHandler );
}
