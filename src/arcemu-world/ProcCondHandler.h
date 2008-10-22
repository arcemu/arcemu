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

#ifndef _PROCHANDLER_H_
#define _PROCHANDLER_H_

//this should be a value greater then one from dbc. Note that we waist memory if we pick a too large one
// 65spellids would mean : 65k*4 bytes = 256KByte memory compiled in 32k bit mode. That is nothing :P
#define MAX_SPELL_ID_FROMDBC 65535

struct SpellEntry;
struct ProcTriggerSpell;
class Unit;

enum ProcCondHandlerRes
{
	PROCCOND_CONTINUE_EXECUTION		= 1,	//we meet conditions and should cast this spell
	PROCCOND_BREAK_EXECUTION		= 2,	//it is not time to take further actions
	PROCCOND_FATAL_EXIT				= 3,	//should not happen
};

struct ProcCondSharedDataStruct
{
	Unit			*owner;			//the proc is registered to this guy
	uint32			ProcFlags;		//contains the ProcFlags that called prochandler
	Unit			*victim;		//depending on procflags this can represent the target the suffered the event. Ex : damagde received, damage applied
	SpellEntry		*CastingSpell;	//in case this is a spellcast Proc then we have a value here
	uint32			Fulldmg;		//un case of a damage proc this contains full damage that can char receive
	uint32			Absdmg;			//fulldmg - absdmg = damage applied to target
	std::list< struct ProcTriggerSpell >::iterator cur_itr;		//we are iterating through a list of registered handlers. We are at this one atm;
	int				dmg_overwrite[3];	//we can force spell effects to take values as we desire like spells that recover health based on dmg
};

//we registerer our functions in handler lists. This is made through function since we set non used 
//slots to NULL with function ....
void InitProcCondHandlers();

//an idea is to make this for spellhashes instead of ids
//at this step i just move old way to this one and maybe in next phase improve it
extern ProcCondHandlerRes (*G_ProcCondHandlers[MAX_SPELL_ID_FROMDBC])(ProcCondSharedDataStruct *shareddata);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//							Same thing but PPM related conditions
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

extern float (*G_ProcPPMCondHandlers[MAX_SPELL_ID_FROMDBC])(ProcCondSharedDataStruct *shareddata);

#endif
