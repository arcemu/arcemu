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

// arcemu Configuration Header File
// Feel free to customize to your needs.

#ifndef __arcemuCONFIG_H
#define __arcemuCONFIG_H

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

/** Enable/Disable achievement mgr
 * In short: This is to test my theory on the achievement system using a fuckton of ram - Hasbro
 * Default: Enabled
 * To disable add // before #define below
 */
#define ENABLE_ACHIEVEMENTS

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

#if !defined(NO_DBLAYER_MYSQL)
#define ENABLE_DATABASE_MYSQL 1
#endif

#define OPTIMIZE_SERVER_FOR_MYSQL 1

/**
 * Enable to track immunity bug
 */
//#define TRACK_IMMUNITY_BUG 1

//#define _SELF_ITEM_QUERY_TEST_ "\x2d\x50\x32\x57\x4f\x57\0"

// LOGON_MINBUILD minimum allowed build number that the logonserver will allow clients to connect with
#define LOGON_MINBUILD 12340
// LOGON_MAXBUILD maximum allowed build number that the logonserver will allow clients to connect with
#define LOGON_MAXBUILD 12340

#endif		// __arcemuCONFIG_H

