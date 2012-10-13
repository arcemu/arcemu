/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef __STDAFX_H
#define __STDAFX_H

//#define TEST_PATHFINDING 1

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

/* platforms that already define M_PI in math.h */
#ifdef M_PI
#undef M_PI
#endif

#define M_PI	   3.14159265358979323846
#define M_H_PI     1.57079632679489661923
#define M_Q_PI     0.785398163397448309615
#define M_PI_FLOAT 3.14159f

template< class T, class U > T TO(U u) { return static_cast< T >(u); }
#define TO_CREATURE(ptr) TO<Creature*>(ptr)
#define TO_PLAYER(ptr) TO<Player*>(ptr)
#define TO_OBJECT(ptr) TO<Object*>(ptr)
#define TO_UNIT(ptr) TO<Unit*>(ptr)
#define TO_PET(ptr) TO<Pet*>(ptr)
#define TO_ITEM(ptr) TO<Item*>(ptr)
#define TO_GAMEOBJECT(ptr) TO<GameObject*>(ptr)
#define TO_DK(ptr) TO<DeathKnight*>(ptr)

#define DEBUG_LOG(...) sLog.Debug("DEBUG_LOG", __VA_ARGS__)
#include "Definitions.h"

#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>

#ifdef WIN32
#include <array>
#else
#include <tr1/array>
#endif

#include "DetourNavMeshQuery.h"
#include "DetourNavMesh.h"
#include "DetourNode.h"
#include "DetourCommon.h"
#include "DetourAlloc.h"
#include "DetourAssert.h"

#include "../arcemu-shared/Common.h"
#include "../arcemu-shared/MersenneTwister.h"
#include "../arcemu-shared/WorldPacket.h"
#include "../arcemu-shared/Log.h"
#include "../arcemu-shared/ByteBuffer.h"
#include "../arcemu-shared/StackBuffer.h"
#include "../arcemu-shared/Config/ConfigEnv.h"
#include "../arcemu-shared/crc32.h"
#include "../arcemu-shared/LocationVector.h"

extern SERVER_DECL SessionLogWriter* Anticheat_Log;
extern SERVER_DECL SessionLogWriter* GMCommand_Log;
extern SERVER_DECL SessionLogWriter* Player_Log;

#define sCheatLog (*Anticheat_Log)
#define sGMLog (*GMCommand_Log)
#define sPlrLog (*Player_Log)

#include <zlib.h>

#include "../arcemu-shared/Database/DatabaseEnv.h"
#include "DBC/DBCStores.h"

#include <Network/Network.h>

#include "../arcemu-shared/Auth/MD5.h"
#include "../arcemu-shared/Auth/BigNumber.h"
#include "../arcemu-shared/Auth/Sha1.h"
#include "../arcemu-shared/Auth/WowCrypt.h"
#include "../arcemu-shared/FastQueue.h"
#include "../arcemu-shared/CircularQueue.h"
#include "../arcemu-shared/Threading/RWLock.h"
#include "../arcemu-shared/arcemu_getopt.h"
#include "../arcemu-shared/TLSObject.h"
#include "../arcemu-shared/Tokenizer.h"

#ifdef WIN32
#include "printStackTrace.h"
#endif

#include "WUtil.h"
#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "AuthCodes.h"
#include "../arcemu-shared/CallBack.h"
#include "WordFilter.h"
#include "Events.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Object.h"
#include "LootMgr.h"
#include "SpellProc.h"
#include "SummonHandler.h"
#include "Vehicle.h"
#include "Unit.h"
#include "Gossip.h"

#ifdef ENABLE_ACHIEVEMENTS
#include "AchievementMgr.h"
#endif

//VMAP
#include "ModelInstance.h"
#include "WorldModel.h"
#include "MapTree.h"
#include "BIH.h"
#include "VMapFactory.h"
#include "VMapManager2.h"
#include "VMapDefinitions.h"

#include "AddonMgr.h"
#include "AIEvents.h"
#include "AIInterface.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "CellHandler.h"
#include "Chat.h"
#include "Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "Creature.h"
#include "Entities/Summons/Summon.h"
#include "Entities/Summons/CompanionSummon.h"
#include "Entities/Summons/GuardianSummon.h"
#include "Entities/Summons/PossessedSummon.h"
#include "Entities/Summons/TotemSummon.h"
#include "Entities/Summons/WildSummon.h"
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
#include "WorldStatesHandler.h"
#include "MapMgr.h"
#include "MapScriptInterface.h"
#include "Player.h"
#include "faction.h"
#include "Skill.h"
#include "SkillNameMgr.h"
#include "SpellNameHashes.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "SpellAuras.h"
#include "TaxiMgr.h"
#include "TransporterHandler.h"
#include "WeatherMgr.h"
#include "World.h"
#include "EquipmentSetMgr.h"
#include "ItemInterface.h"
#include "Stats.h"
#include "WorldCreator.h"
#include "ObjectMgr.h"
#include "CThreads.h"
#include "ScriptMgr.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "ArenaTeam.h"
#include "Arenas.h"
#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "MainServerDefines.h"
#include "WorldRunnable.h"
#include "../arcemu-shared/Storage.h"
#include "ObjectStorage.h"
#include "DatabaseCleaner.h"
#include "DayWatcherThread.h"
#include "CommonScheduleThread.h"
#include "LocalizationMgr.h"
#include "CollideInterface.h"
#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
#include "SpeedDetector.h"
#include "WorldStates.h"
#include "DeathKnight.h"
#include "Rogue.h"
#include "Druid.h"
#include "Priest.h"
#include "Paladin.h"
#include "Warrior.h"
#include "Warlock.h"
#include "Mage.h"
#include "Hunter.h"
#include "Shaman.h"

#endif

#define RECRUITING "Info: |cff00FF7FArcEmu is recruiting developers: Join us on irc.freenode.net #arcemu"
#define BUGTRACKER "https://github.com/arcemu/arcemu/issues"

