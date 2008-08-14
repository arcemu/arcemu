/*
* ArcEmu MMORPG Server
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

void InitProcCondHandlers()
{
	//we set all handlers to null. A function pointer or a normal pointer still take up the same space
	memset(G_ProcCondHandlers, NULL, MAX_SPELL_ID_FROMDBC * sizeof( void * ) );

	G_ProcCondHandlers[ 16959 ] = PrimalFury;
}

ProcCondHandlerRes (*G_ProcCondHandlers[MAX_SPELL_ID_FROMDBC])(ProcCondSharedDataStruct *shareddata);