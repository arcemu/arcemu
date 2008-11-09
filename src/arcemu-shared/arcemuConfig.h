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

// arcemu Configuration Header File
// Feel free to customize to your needs.

#ifndef __arcemuCONFIG_H
#define __arcemuCONFIG_H

/** Enable/disable arcemu world server cluster support.
 * Warning: arcemu will no longer function independantly when built in cluster mode. 
 * It will require a realmserver to function.
 * As of last update, it is nowhere near complete, only enable for dev use.
 * Default: Disabled
 */
#ifdef ENABLE_CLUSTERING
#  error Sorry but clustering is not finished yet. dont bother to try to compile it. it wont work.
#  define CLUSTERING 1
#endif
//#define CLUSTERING 1

#ifndef ENABLE_IMPROVED_TICKETSYSTEM
#define GM_TICKET_MY_MASTER_COMPATIBLE
#endif
//#undef GM_TICKET_MY_MASTER_COMPATIBLE

/** Use memory mapping for map files for faster access (let OS take care of caching)
 * (currently only available under windows)
 * Only recommended under X64 builds, X86 builds will most likely run out of address space.
 * Default: Disabled
 */
//#define USE_MEMORY_MAPPING_FOR_MAPS

/** Enable/disable arcemu's Voice Chat support.
 * While not completely finished (but close!) you can link your arcemu server to a voice chat
 * server (also compiled by default) and use the client's ingame voice chat function.
 * At the moment it is only good for joining, you can't leave a voice channel yet :P
 * Also, it only works for parties. Custom chat channels are not fully supported yet.
 * However, if you want to play around with it, feel free to enable this define.
 */

#ifdef ENABLE_VOICE_CHAT
#define VOICE_CHAT 1
#endif

/** Enable/disable movement compression.
 * This allows the server to compress long-range creatures movement into a buffer and then flush
 * it periodically, compressed with deflate. This can make a large difference to server bandwidth.
 * Currently this sort of compression is only used for player and creature movement, although
 * it may be expanded in the future.
 * Default: disabled
 */

//#define ENABLE_COMPRESSED_MOVEMENT 1
//#define ENABLE_COMPRESSED_MOVEMENT_FOR_PLAYERS 1
//#define ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES 1

/**
 * DATABASE LAYER SET UP
 */

/**
 * Enable/disable database backends.
 * Currently supported, MySQL, (under development): PostgreSQL, SQLite
 */

#if !defined(NO_DBLAYER_MYSQL)
#define ENABLE_DATABASE_MYSQL 1
#endif
// #define ENABLE_DATABASE_POSTGRES 1
// #define ENABLE_DATABASE_SQLITE 1

/**
 * Optimize the server for MySQL usage.
 * This may give a small boost to performance.
 * Enable it if you do not plan on using arcemu with PostgreSQL or SQLite.
 */
#define OPTIMIZE_SERVER_FOR_MYSQL 1

/**
 * Enable to track immunity bug
 */
//#define TRACK_IMMUNITY_BUG 1

//#define _SELF_ITEM_QUERY_TEST_ "\x2d\x50\x32\x57\x4f\x57\0"

#endif		// __arcemuCONFIG_H

