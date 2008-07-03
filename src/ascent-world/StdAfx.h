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

#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define _GAME

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../ascent-shared/Common.h"
#include "../ascent-shared/MersenneTwister.h"
#include "../ascent-shared/WorldPacket.h"
#include "../ascent-shared/Log.h"
#include "../ascent-shared/NGLog.h"
#include "../ascent-shared/ByteBuffer.h"
#include "../ascent-shared/StackBuffer.h"
#include "../ascent-shared/Config/ConfigEnv.h"
#include "../ascent-shared/crc32.h"
#include "../ascent-shared/LocationVector.h"

#include <zlib.h>

#include "../ascent-shared/Database/DatabaseEnv.h"
#include "../ascent-shared/Database/DBCStores.h"
#include "../ascent-shared/Database/dbcfile.h"

#include <Network/Network.h>

#include "../ascent-shared/Auth/MD5.h"
#include "../ascent-shared/Auth/BigNumber.h"
#include "../ascent-shared/Auth/Sha1.h"
#include "../ascent-shared/Auth/WowCrypt.h"
#include "../ascent-shared/CrashHandler.h"
#include "../ascent-shared/FastQueue.h"
#include "../ascent-shared/CircularQueue.h"
#include "../ascent-shared/Threading/RWLock.h"
#include "../ascent-shared/Threading/Condition.h"
#include "../ascent-shared/ascent_getopt.h"

#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"

#ifdef CLUSTERING
	#include "../ascent-realmserver/WorkerOpcodes.h"
#endif

#include "Packets.h"

#include "../ascent-shared/CallBack.h"
#include "WordFilter.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Object.h"
#include "LootMgr.h"
#include "Unit.h"

#include "AddonMgr.h"
#include "AIInterface.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "AlteracValley.h"
#include "ArathiBasin.h"
#include "EyeOfTheStorm.h"
#include "Arenas.h"
#include "CellHandler.h"
#include "Chat.h"
#include "Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "Creature.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Group.h"
#include "Guild.h"
#include "HonorHandler.h"
#include "ItemPrototype.h"
#include "Item.h"
#include "Container.h"
#include "AuctionHouse.h"
#include "AuctionMgr.h"
#include "LfgMgr.h"
#include "MailSystem.h"
#include "Map.h"
#include "MapCell.h"
#include "TerrainMgr.h"
#include "MiscHandler.h"
#include "NameTables.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include "MapMgr.h"
#include "MapScriptInterface.h"
#include "Player.h"
#include "faction.h"
#include "Skill.h"
#include "SkillNameMgr.h"
#include "SpellNameHashes.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "TaxiMgr.h"
#include "TransporterHandler.h"
#include "WarsongGulch.h"
#include "WeatherMgr.h"
#include "World.h"
#include "ItemInterface.h"
#include "Stats.h"
#include "WorldCreator.h"


#include "ObjectMgr.h"

#include "CThreads.h"
#include "ScriptMgr.h"

#include "Channel.h"
#include "ChannelMgr.h"
#include "ArenaTeam.h"

#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "MainServerDefines.h"
#include "WorldRunnable.h"

#include "../ascent-shared/Storage.h"
#include "ObjectStorage.h"
#include "DatabaseCleaner.h"
#include "DayWatcherThread.h"
#include "VoiceChatHandler.h"
#include "LocalizationMgr.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#ifdef COLLISION
#include "CollideInterface.h"
#endif

#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1
#include "SpeedDetector.h"
//#define PVP_REALM_MEANS_CONSTANT_PVP
#endif
