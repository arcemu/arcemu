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

#include "Game/World/WorldConfig.h"

#include "Game/Instances/Definitions.h"

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
#include "Math/Math.h"
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

#include "../shared/Server/ServerController.h"

#ifdef WIN32
#include "System/printStackTrace.h"
#endif

#include "Math/LocationVector.h"
#include "Game/Network/WoWGuid.h"
#include "Game/Network/ByteBufferExtension.h"

#include "Game/AI/Waypoint.h"

#include "Network/AuthCodes.h"

#include "Game/WUtil.h"
#include "Game/Entities/Update/UpdateFields.h"
#include "Game/Entities/Update/UpdateMask.h"
#include "Game/Network/Opcodes.h"
#include "Util/CallBack.h"
#include "Framework/WordFilter/WordFilter.h"
#include "Framework/TimedEvents/Events.h"
#include "Framework/TimedEvents/EventMgr.h"
#include "Framework/TimedEvents/EventableObject.h"
#include "Game/Entities/ObjectConstants.h"
#include "Game/Entities/Object.h"
#include "Game/Loot/LootMgr.h"
#include "Game/Spell/SpellProc.h"
#include "Game/Entities/Summons/SummonHandler.h"
#include "Game/Entities/Vehicle.h"
#include "Game/Entities/Vendor.h"
#include "Game/Entities/Unit.h"
#include "Game/Gossip/Gossip.h"
#include "Game/Entities/SpawnTimedEmotes.h"

#include "Game/Achievements/AchievementMgr.h"

//VMAP
#include "ModelInstance.h"
#include "WorldModel.h"
#include "MapTree.h"
#include "BIH.h"
#include "VMapFactory.h"
#include "VMapManager2.h"
#include "VMapDefinitions.h"

#include "Game/Addons/AddonMgr.h"
#include "Game/AI/AIEvents.h"
#include "Game/AI/AIInterface.h"
#include "Game/AreaTrigger.h"
#include "Game/BattleGround/BattlegroundMgr.h"
#include "Game/BattleGround/Battleground.h"
#include "Map/CellHandler.h"
#include "Chat/Chat.h"
#include "Game/Entities/Corpse.h"
#include "Game/Quest/Quest.h"
#include "Game/Quest/QuestMgr.h"
#include "Game/Entities/Creature.h"
#include "Game/Entities/Summons/Summon.h"
#include "Game/Entities/Summons/CompanionSummon.h"
#include "Game/Entities/Summons/GuardianSummon.h"
#include "Game/Entities/Summons/PossessedSummon.h"
#include "Game/Entities/Summons/TotemSummon.h"
#include "Game/Entities/Summons/WildSummon.h"
#include "Game/Entities/DynamicObject.h"
#include "Game/Entities/GameObject.h"
#include "Game/Entities/ObjectLocator.h"
#include "Game/Group/Group.h"
#include "Game/Guild/Guild.h"
#include "Game/Honor/HonorHandler.h"
#include "Game/Item/ItemPrototype.h"
#include "Game/Item/ItemPrototypeFilter.h"
#include "Game/Entities/Item.h"
#include "Game/Entities/Container.h"
#include "Game/Auction/AuctionHouse.h"
#include "Game/Auction/AuctionMgr.h"
#include "Game/LFG/LfgMgr.h"
#include "Game/Mail/MailMessage.h"
#include "Game/Mail/MailBox.h"
#include "Game/Mail/MailSystem.h"
#include "Map/Map.h"
#include "Map/MapCell.h"
#include "Map/TerrainMgr.h"
#include "PacketHandlers/MiscHandler.h"
#include "Game/Network/NameTables.h"
#include "PacketHandlers/NPCHandler.h"
#include "Game/Entities/Pet.h"
#include "Server/WorldSocket.h"
#include "Game/Network/MovementInfo.h"
#include "Game/Network/WorldSession.h"
#include "Game/WorldStates/WorldStatesHandler.h"
#include "Map/MapMgr.h"
#include "Map/MapScriptInterface.h"
#include "Game/Entities/Player.h"
#include "Game/Item/PlayerItemPrototypeFilter.h"
#include "Game/Factions/faction.h"
#include "Game/Skill/Skill.h"
#include "Game/Skill/SkillNameMgr.h"
#include "Game/Spell/SpellNameHashes.h"
#include "Game/Spell/Spell.h"
#include "Game/Spell/SpellMgr.h"
#include "Game/Spell/SpellAuras.h"
#include "Game/Taxi/TaxiMgr.h"
#include "Game/Entities/Transporter.h"
#include "Game/Weather/Weather.h"
#include "Game/Network/WeatherPacketBuilder.h"
#include "Game/Weather/WeatherProvider.h"
#include "Game/Weather/WeatherProviderFactory.h"
#include "Game/Weather/WeatherInfo.h"
#include "Game/Weather/NullWeatherProvider.h"
#include "Game/Weather/OpenWeatherProvider.h"
#include "Game/Weather/GeneratedWeatherProvider.h"
#include "Game/Weather/WeatherMgr.h"
#include "Game/World/World.h"
#include "Game/EquipmentSet/EquipmentSetMgr.h"
#include "Game/Item/ItemInterface.h"
#include "Game/Item/ItemIterator.h"
#include "Game/Commands/ItemCommands.h"
#include "Game/Entities/Stats.h"
#include "Game/Instances/WorldCreator.h"
#include "Game/Storage/ObjectMgr.h"
#include "Threading/CThreads.h"
#include "Game/Scripting/ScriptMgr.h"
#include "Chat/Channel.h"
#include "Chat/ChannelMgr.h"
#include "Game/Arena/ArenaTeam.h"
#include "Game/Arena/Arenas.h"
#include "Server/LogonSocket.h"
#include "Server/LogonCommHandler.h"
#include "Server/MainServerDefines.h"
#include "Game/World/WorldRunnable.h"
#include "Container/Storage.h"
#include "Game/Storage/ObjectStorage.h"
#include "Server/DatabaseCleaner.h"
#include "Game/World/DayWatcherThread.h"
#include "Game/World/CommonScheduleThread.h"
#include "Localization/LocalizationMgr.h"
#include "Framework/Collision/CollideInterface.h"
#include "Server/BaseConsole.h"
#include "Server/CConsole.h"
#include "Framework/SpeedCheatDetector/SpeedDetector.h"
#include "Game/WorldStates/WorldStates.h"
#include "Game/Entities/DeathKnight.h"
#include "Game/Entities/Rogue.h"
#include "Game/Entities/Druid.h"
#include "Game/Entities/Priest.h"
#include "Game/Entities/Paladin.h"
#include "Game/Entities/Warrior.h"
#include "Game/Entities/Warlock.h"
#include "Game/Entities/Mage.h"
#include "Game/Entities/Hunter.h"
#include "Game/Entities/Shaman.h"
#include "Game/Network/Messenger.h"
#include "Game/Commands/PlayerCommands.h"
#include "Game/Commands/CreatureCommands.h"
#include "Game/Commands/AccountCommands.h"
#include "Game/Network/PlayerMessenger.h"
#include "Game/Network/AccountPackets.h"
#include "Game/Network/ItemPackets.h"
#include "Game/Network/LFGPackets.h"
#include "Game/Graveyard/GraveyardService.h"
#include "PacketHandlers/InventoryListBuilder.h"
#include "Game/Entities/Player/PlayerMatcher.h"
#include "Game/Entities/Player/PlayerVisitor.h"
#include "../shared/Util/Progress.h"
#include "Game/Util/PvPCaptureRateBonusCalculator.h"

#endif
