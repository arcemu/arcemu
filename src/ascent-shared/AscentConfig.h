/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

// Ascent Configuration Header File
// Feel free to customize to your needs.

#ifndef __ASCENTCONFIG_H
#define __ASCENTCONFIG_H

/** Enable/disable Ascent world server cluster support.
 * Warning: Ascent will no longer function independantly when built in cluster mode. 
 * It will require a realmserver to function.
 * As of last update, it is nowhere near complete, only enable for dev use.
 * Default: Disabled
 */
#ifdef ENABLE_CLUSTERING
#  error Sorry but clustering is not finished yet. dont bother to try to compile it. it wont work.
#  define CLUSTERING 1
#endif
//#define CLUSTERING 1

/** Enable/disable 3D geometry calculations
 * Warning: This may be resource heavy.
 * As of last update, it was nowhere near complete. Only enable for dev use.
 * Default: Disabled
 */
#ifdef ENABLE_COLLISION
# define COLLISION 1
#endif
//#define COLLISION 1

/** Use memory mapping for map files for faster access (let OS take care of caching)
 * (currently only available under windows)
 * Only recommended under X64 builds, X86 builds will most likely run out of address space.
 * Default: Disabled
 */
//#define USE_MEMORY_MAPPING_FOR_MAPS

/** Enable/disable Ascent's Voice Chat support.
 * While not completely finished (but close!) you can link your Ascent server to a voice chat
 * server (also compiled by default) and use the client's ingame voice chat function.
 * At the moment it is only good for joining, you can't leave a voice channel yet :P
 * Also, it only works for parties. Custom chat channels are not fully supported yet.
 * However, if you want to play around with it, feel free to enable this define.
 */
//#define VOICE_CHAT 1

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
 * Enable it if you do not plan on using Ascent with PostgreSQL or SQLite.
 */
#define OPTIMIZE_SERVER_FOR_MYSQL 1

#endif		// __ASCENTCONFIG_H

