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

#include "WorldConfig.h"

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

#include "System/Platform.h"
#include "Util/Util.h"
#include "Util/MersenneTwister.h"
#include "Container/ByteBuffer.h"

#include "Network/PacketBuffer.h"
#include "Network/Packet.h"
typedef PacketBuffer WorldPacket;

#include "Logging/Log.h"
#include "../shared/Config/ConfigEnv.h"
#include "System/crc32.h"

extern SERVER_DECL SessionLogWriter* Anticheat_Log;
extern SERVER_DECL SessionLogWriter* GMCommand_Log;
extern SERVER_DECL SessionLogWriter* Player_Log;

#define sCheatLog (*Anticheat_Log)
#define sGMLog (*GMCommand_Log)
#define sPlrLog (*Player_Log)

#include <zlib.h>

#include "../shared/Database/DatabaseEnv.h"
#include "DBC/DBCStores.h"

#include <Network/Network.h>

#include "../shared/Crypto/MD5.h"
#include "../shared/Crypto/BigNumber.h"
#include "../shared/Crypto/Sha1.h"
#include "Server/WowCrypt.h"
#include "Container/FastQueue.h"
#include "Container/CircularQueue.h"
#include "../shared/Threading/RWLock.h"
#include "../shared/Threading/CRefCounter.h"
#include "Util/arcemu_getopt.h"
#include "Threading/TLSObject.h"
#include "Util/Tokenizer.h"

#ifdef WIN32
#include "System/printStackTrace.h"
#endif

#include "LocationVector.h"
#include "ByteBufferExtension.h"

#include "Waypoint.h"

#include "Network/AuthCodes.h"

#include "WUtil.h"
#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "Util/CallBack.h"
#include "WordFilter.h"
#include "Events.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Entities/ObjectConstants.h"
#include "Entities/Object.h"
#include "LootMgr.h"
#include "Spell/SpellProc.h"
#include "SummonHandler.h"
#include "Entities/Vehicle.h"
#include "Entities/Unit.h"
#include "Gossip.h"
#include "SpawnTimedEmotes.h"

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
#include "Map/CellHandler.h"
#include "Chat/Chat.h"
#include "Entities/Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "Entities/Creature.h"
#include "Entities/Summons/Summon.h"
#include "Entities/Summons/CompanionSummon.h"
#include "Entities/Summons/GuardianSummon.h"
#include "Entities/Summons/PossessedSummon.h"
#include "Entities/Summons/TotemSummon.h"
#include "Entities/Summons/WildSummon.h"
#include "Entities/DynamicObject.h"
#include "Entities/GameObject.h"
#include "Group.h"
#include "Guild.h"
#include "HonorHandler.h"
#include "ItemPrototype.h"
#include "Entities/Item.h"
#include "Entities/Container.h"
#include "AuctionHouse.h"
#include "AuctionMgr.h"
#include "LfgMgr.h"
#include "MailMessage.h"
#include "MailBox.h"
#include "MailSystem.h"
#include "Map/Map.h"
#include "Map/MapCell.h"
#include "Map/TerrainMgr.h"
#include "PacketHandlers/MiscHandler.h"
#include "NameTables.h"
#include "PacketHandlers/NPCHandler.h"
#include "Entities/Pet.h"
#include "Server/WorldSocket.h"
#include "MovementInfo.h"
#include "WorldSession.h"
#include "WorldStatesHandler.h"
#include "Map/MapMgr.h"
#include "Map/MapScriptInterface.h"
#include "Entities/Player.h"
#include "faction.h"
#include "Skill.h"
#include "SkillNameMgr.h"
#include "Spell/SpellNameHashes.h"
#include "Spell/Spell.h"
#include "Spell/SpellMgr.h"
#include "Spell/SpellAuras.h"
#include "TaxiMgr.h"
#include "Entities/Transporter.h"
#include "WeatherMgr.h"
#include "World.h"
#include "EquipmentSetMgr.h"
#include "ItemInterface.h"
#include "ItemIterator.h"
#include "Game/Commands/ItemCommands.h"
#include "Stats.h"
#include "WorldCreator.h"
#include "ObjectMgr.h"
#include "Threading/CThreads.h"
#include "ScriptMgr.h"
#include "Chat/Channel.h"
#include "Chat/ChannelMgr.h"
#include "ArenaTeam.h"
#include "Arenas.h"
#include "Server/LogonSocket.h"
#include "Server/LogonCommHandler.h"
#include "Server/MainServerDefines.h"
#include "WorldRunnable.h"
#include "Container/Storage.h"
#include "ObjectStorage.h"
#include "DatabaseCleaner.h"
#include "DayWatcherThread.h"
#include "CommonScheduleThread.h"
#include "LocalizationMgr.h"
#include "CollideInterface.h"
#include "Server/Master.h"
#include "Server/BaseConsole.h"
#include "Server/CConsole.h"
#include "SpeedDetector.h"
#include "WorldStates.h"
#include "Entities/DeathKnight.h"
#include "Entities/Rogue.h"
#include "Entities/Druid.h"
#include "Entities/Priest.h"
#include "Entities/Paladin.h"
#include "Entities/Warrior.h"
#include "Entities/Warlock.h"
#include "Entities/Mage.h"
#include "Entities/Hunter.h"
#include "Entities/Shaman.h"
#include "Game/Commands/PlayerCommands.h"
#include "Game/Commands/CreatureCommands.h"
#include "Game/Commands/AccountCommands.h"
#include "PlayerMessenger.h"
#include "Game/Network/AccountPackets.h"
#include "Game/Network/ItemPackets.h"

#endif
