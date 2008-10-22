/*
* ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"

/******************************************************************************/
//					DRUID												   	  //
/******************************************************************************/

//Primal Fury Proc
ProcCondHandlerRes PrimalFury(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	Player * p = static_cast< Player * >( shareddata->owner );
	if( p->GetShapeShift() != FORM_BEAR && p->GetShapeShift() != FORM_DIREBEAR )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Blood Frenzy
ProcCondHandlerRes BloodFrenzy(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() || !shareddata->CastingSpell )
		return PROCCOND_BREAK_EXECUTION;

	if( static_cast< Player * >( shareddata->owner )->GetShapeShift() != FORM_CAT || (
		shareddata->CastingSpell->NameHash != SPELL_HASH_CLAW &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_RAKE &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_RAVAGE &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_SHRED ) )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Seal Fate
ProcCondHandlerRes SealFate(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() || !shareddata->CastingSpell )
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell->Id == 14189 ||
		shareddata->CastingSpell->Id == 16953 ||
		shareddata->CastingSpell->Id == 16959)
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell->Effect[0] != 80 &&
		shareddata->CastingSpell->Effect[1] != 80 &&
		shareddata->CastingSpell->Effect[2] != 80 &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_ )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//intencity
ProcCondHandlerRes Intencity(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->CastingSpell || shareddata->CastingSpell->Id != 5229)
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Nature's Guardian
ProcCondHandlerRes NatureGuardian(ProcCondSharedDataStruct *shareddata)
{
	if( shareddata->owner->GetHealthPct() > 30 )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Bloodlust
ProcCondHandlerRes Bloodlust(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	if( static_cast< Player* >( shareddata->owner )->GetShapeShift() != FORM_BEAR ||
		static_cast< Player* >( shareddata->owner )->GetShapeShift() != FORM_DIREBEAR )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Bloodlust2
ProcCondHandlerRes Bloodlust2(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	if( static_cast< Player* >( shareddata->owner )->GetShapeShift() != FORM_CAT )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//holy concentration
ProcCondHandlerRes HolyConcentration(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell == NULL )
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell->NameHash != SPELL_HASH_FLASH_HEAL &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_BINDING_HEAL &&
		shareddata->CastingSpell->NameHash != SPELL_HASH_GREATER_HEAL )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Mace Stun Effect
ProcCondHandlerRes MaceStunEffect(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	uint32 talentlevel = 0;
	switch( shareddata->cur_itr->origId )
	{
		//mace specialization
		case 12284:	{talentlevel = 1;}break;
		case 12701:	{talentlevel = 2;}break;
		case 12702:	{talentlevel = 3;}break;
		case 12703:	{talentlevel = 4;}break;
		case 12704:	{talentlevel = 5;}break;
	}

	//warrior/rogue mace specialization can trigger only when using maces
	Item* it;
	if( static_cast< Player* >( shareddata->owner )->GetItemInterface() )
	{
		it = static_cast< Player* >( shareddata->owner )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		if( it != NULL && it->GetProto() )
		{
			uint32 reqskill = GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
			if( reqskill != SKILL_MACES && reqskill != SKILL_2H_MACES )
				return PROCCOND_BREAK_EXECUTION; //not the right weapon
		}
		else
			return PROCCOND_BREAK_EXECUTION; //no weapon no joy
	}
	else
		return PROCCOND_BREAK_EXECUTION; //no weapon no joy

	//let's recalc chance to cast since we have a full 100 all time on this one
	//how lame to get talentpointlevel for this spell :(
	//float chance=it->GetProto()->Delay*100*talentlevel/60000;
	float chance = float( it->GetProto()->Delay ) * float( talentlevel ) / 600.0f;
	if( !Rand( chance ) )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//sword specialization
ProcCondHandlerRes SwordSpecialization(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	if( static_cast< Player* >( shareddata->owner )->GetItemInterface())
	{
		Item* it = static_cast< Player* >( shareddata->owner )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		if( it != NULL && it->GetProto() )
		{
			uint32 reqskill=GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
			if( reqskill != SKILL_SWORDS && reqskill != SKILL_2H_SWORDS )
				return PROCCOND_BREAK_EXECUTION; //not the right weapon
		}
		else
			return PROCCOND_BREAK_EXECUTION; //no weapon no joy
	}
	else
		return PROCCOND_BREAK_EXECUTION; //no weapon no joy

	return PROCCOND_CONTINUE_EXECUTION;
}

//deep wound
ProcCondHandlerRes DeepWound(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	//deep wound requires a melee weapon
	Item* it;
	if( static_cast< Player* >( shareddata->owner )->GetItemInterface())
	{
		it = static_cast< Player* >( shareddata->owner )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		if( it != NULL && it->GetProto() )
		{
			//class 2 means weapons ;)
			if( it->GetProto()->Class != 2 )
				return PROCCOND_BREAK_EXECUTION;
		}
		else return PROCCOND_BREAK_EXECUTION; //no weapon no joy
	}
	else return PROCCOND_BREAK_EXECUTION; //no weapon no joy

	return PROCCOND_CONTINUE_EXECUTION;
}

/******************************************************************************/
//					Warrior												   	  //
/******************************************************************************/

//Blood Frenzy
ProcCondHandlerRes BloodFrenzy_war(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell == NULL )
		return PROCCOND_BREAK_EXECUTION;

	if( shareddata->CastingSpell->NameHash != SPELL_HASH_REND && 
		shareddata->CastingSpell->NameHash != SPELL_HASH_DEEP_WOUND )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

//Unbridled Wrath
ProcCondHandlerRes UnbridledWrath(ProcCondSharedDataStruct *shareddata)
{
	if( !shareddata->owner->IsPlayer() )
		return PROCCOND_BREAK_EXECUTION;

	uint32 talentlevel = 0;
	switch( shareddata->cur_itr->origId )
	{
		case 12999:	{talentlevel = 1;}break;
		case 13000:	{talentlevel = 2;}break;
		case 13001:	{talentlevel = 3;}break;
		case 13002:	{talentlevel = 4;}break;
	}

	//let's recalc chance to cast since we have a full 100 all time on this one
	Item* it;
	if( static_cast< Player* >( shareddata->owner )->GetItemInterface() )
	{
		it = static_cast< Player* >( shareddata->owner )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		if( !( it != NULL && it->GetProto() ) )
			return PROCCOND_BREAK_EXECUTION; //no weapon no joy
	}
	else
		return PROCCOND_BREAK_EXECUTION; //no weapon no joy
	//float chance=float(it->GetProto()->Delay)*float(talentlevel)/600.0f;
	float chance = float( it->GetProto()->Delay ) * float(talentlevel ) / 300.0f; //zack this had a very low proc rate. Kinda liek a waisted talent
	uint32 myroll = RandomUInt( 100 );
	if( myroll > chance )
		return PROCCOND_BREAK_EXECUTION;

	return PROCCOND_CONTINUE_EXECUTION;
}

void InitProcCondHandlers()
{
	//we set all handlers to null. A function pointer or a normal pointer still take up the same space
	memset(G_ProcCondHandlers, NULL, MAX_SPELL_ID_FROMDBC * sizeof( void * ) );

	G_ProcCondHandlers[ 16959 ] = PrimalFury;
	G_ProcCondHandlers[ 16953 ] = BloodFrenzy;
	G_ProcCondHandlers[ 14189 ] = SealFate;
	G_ProcCondHandlers[ 17106 ] = Intencity;
	G_ProcCondHandlers[ 31616 ] = NatureGuardian;
	G_ProcCondHandlers[ 37309 ] = Bloodlust;
	G_ProcCondHandlers[ 37310 ] = Bloodlust2;
	G_ProcCondHandlers[ 34754 ] = HolyConcentration;
	G_ProcCondHandlers[ 5530 ] = MaceStunEffect;
	G_ProcCondHandlers[ 4350 ] = SwordSpecialization;
	G_ProcCondHandlers[ 12721 ] = DeepWound;
	G_ProcCondHandlers[ 30069 ] = BloodFrenzy_war;
	G_ProcCondHandlers[ 30070 ] = BloodFrenzy_war;
	G_ProcCondHandlers[ 12964 ] = UnbridledWrath;
}

ProcCondHandlerRes (*G_ProcCondHandlers[MAX_SPELL_ID_FROMDBC])(ProcCondSharedDataStruct *shareddata);