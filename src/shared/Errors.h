/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_ERRORS_H
#define WOWSERVER_ERRORS_H

#include "printStackTrace.h"
// TODO: handle errors better

#if CODE_ANALYSIS
#include<codeanalysis\sourceannotations.h>
#define ANALYSIS_ASSUME( EXPR ) __analysis_assume( EXPR )
#else
#define ANALYSIS_ASSUME( EXPR )
#endif

// An assert isn't necessarily fatal, but we want to stop anyways
#define WPAssert( EXPR ) if (!(EXPR)) { arcAssertFailed(__FILE__,__LINE__,#EXPR); ((void(*)())0)(); } ANALYSIS_ASSUME( EXPR )

#define WPError( assertion, errmsg ) if( ! (assertion) ) { Log::getSingleton( ).outError( "%s:%i ERROR:\n  %s\n", __FILE__, __LINE__, (char *)errmsg ); assert( false ); }
#define WPWarning( assertion, errmsg ) if( ! (assertion) ) { Log::getSingleton( ).outError( "%s:%i WARNING:\n  %s\n", __FILE__, __LINE__, (char *)errmsg ); }

// This should always halt everything.  If you ever find yourself wanting to remove the assert( false ), switch to WPWarning or WPError
#define WPFatal( assertion, errmsg ) if( ! (assertion) ) { Log::getSingleton( ).outError( "%s:%i FATAL ERROR:\n  %s\n", __FILE__, __LINE__, (char *)errmsg ); assert( #assertion &&0 ); abort(); }

#define ASSERT WPAssert

#endif

