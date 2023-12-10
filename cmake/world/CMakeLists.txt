PROJECT(world)

# The idea here is fairly simple: build a lib, and build an executable, then link the lib to the executable.

SET( prefix ${ROOT_PATH}/src/world )
SET(sources
  DBC/DBCStores.cpp
	Game/Commands/AccountCommands.cpp
	Game/Network/AccountPackets.cpp
	Game/Network/ItemPackets.cpp
	Game/Network/LFGPackets.cpp
	Game/Achievements/AchievementMgr.cpp 
	Framework/Collision/CollideInterface.cpp  
	PacketHandlers/VoiceChatHandler.cpp 
	Game/World/DayWatcherThread.cpp 
	Game/World/CommonScheduleThread.cpp 
	Game/Arena/ArenaTeam.cpp 
	PacketHandlers/ArenaTeamHandler.cpp 
	Server/DatabaseCleaner.cpp 
	Game/Addons/AddonMgr.cpp 
	Game/AI/AIInterface.cpp 
	Game/AI/AIEvents.cpp
	PacketHandlers/AreaTrigger.cpp 
	Game/Arena/Arenas.cpp 
	Game/Auction/AuctionHouse.cpp 
	PacketHandlers/AuctionHouseHandler.cpp
	Game/Auction/AuctionMgr.cpp 
	Game/BattleGround/Battleground.cpp
	Chat/BattlegroundCommands.cpp 
	PacketHandlers/BattlegroundHandler.cpp 
	Game/BattleGround/BattlegroundMgr.cpp 
	Chat/Channel.cpp 
	PacketHandlers/ChannelHandler.cpp 
	PacketHandlers/CharacterHandler.cpp 
	Chat/Chat.cpp 
	PacketHandlers/ChatHandler.cpp 
	PacketHandlers/CombatHandler.cpp 
	Game/Entities/Container.cpp 
	Game/Entities/Corpse.cpp 
	Game/Entities/Creature.cpp 
	Game/Commands/CreatureCommands.cpp
	Game/Entities/Summons/Summon.cpp
	Game/Entities/Summons/CompanionSummon.cpp
	Game/Entities/Summons/GuardianSummon.cpp
	Game/Entities/Summons/PossessedSummon.cpp
	Game/Entities/Summons/TotemSummon.cpp
	Game/Entities/Summons/WildSummon.cpp
	Chat/DebugCommands.cpp 
	PacketHandlers/DuelHandler.cpp 
	Game/Entities/DynamicObject.cpp 
	Game/EquipmentSet/EquipmentSetMgr.cpp
	Framework/TimedEvents/EventableObject.cpp 
	Framework/TimedEvents/EventMgr.cpp 
	Game/Factions/faction.cpp 
	Game/Entities/GameObject.cpp 
	PacketHandlers/GMTicketHandler.cpp 
	Chat/GMTicketCommands.cpp
	Game/Gossip/Gossip.cpp
	Game/Group/Group.cpp 
	PacketHandlers/GroupHandler.cpp 
	Game/Guild/Guild.cpp 
	PacketHandlers/GuildHandler.cpp 
	Game/Honor/HonorHandler.cpp 
	Chat/InstanceCommands.cpp 
	Game/Entities/Item.cpp
	Game/Commands/ItemCommands.cpp
	PacketHandlers/ItemHandler.cpp 
	Game/Item/ItemInterface.cpp 
	Game/Item/PlayerItemPrototypeFilter.cpp
	Chat/Level0.cpp 
	Chat/Level1.cpp 
	Chat/Level2.cpp 
	Chat/Level3.cpp 
	Game/LFG/LfgMgr.cpp 
	Game/Loot/LootMgr.cpp 
	Localization/LocalizationMgr.cpp 
	Server/LogonSocket.cpp 
	Server/LogonCommHandler.cpp 
	Game/Mail/MailMessage.cpp
	Game/Mail/MailBox.cpp
	PacketHandlers/MailHandler.cpp
	Game/Mail/MailSystem.cpp 
	Map/MapCell.cpp 
	Map/Map.cpp 
	Map/MapScriptInterface.cpp 
	Map/MapMgr.cpp 
	Game/Network/MessageRouter.cpp
	Game/Network/Messenger.cpp
	PacketHandlers/MiscHandler.cpp 
	PacketHandlers/MovementHandler.cpp 
	Game/Network/MovementInfo.cpp
	PacketHandlers/NPCHandler.cpp 
	Game/Entities/Object.cpp
    Game/Entities/ObjectLocator.cpp	
	Game/Storage/ObjectMgr.cpp
	Game/Storage/ObjectStorage.cpp
	Game/Network/Opcodes.cpp 
	Game/Entities/Pet.cpp 
	PacketHandlers/PetHandler.cpp 
	Game/Entities/Player.cpp 
	Game/Commands/PlayerCommands.cpp
	Game/Network/PlayerMessenger.cpp
	Game/Entities/PlayerPacketWrapper.cpp 
	PacketHandlers/QueryHandler.cpp 
	Game/Quest/Quest.cpp 
	PacketHandlers/QuestHandler.cpp 
	Game/Quest/QuestMgr.cpp 
	Chat/QuestCommands.cpp 
	PacketHandlers/RaidHandler.cpp 
	Chat/RecallCommands.cpp 
	Game/Reputation/ReputationHandler.cpp 
	Game/Scripting/ScriptMgr.cpp 
	PacketHandlers/SocialHandler.cpp 
	Framework/SpeedCheatDetector/SpeedDetector.cpp 
	Game/Spell/SpellAuras.cpp 
	Game/Spell/SpellTarget.cpp 
	Game/Spell/Spell.cpp 
	StdAfx.cpp
	Game/Entities/Summons/SummonHandler.cpp
	Game/Spell/SpellEffects.cpp 
	PacketHandlers/SpellHandler.cpp 
	Game/Spell/HackFixes.cpp 
	Game/Entities/Stats.cpp 
	PacketHandlers/TaxiHandler.cpp 
	Game/Taxi/TaxiMgr.cpp 
	Map/TerrainMgr.cpp 
	PacketHandlers/TradeHandler.cpp
	Game/Entities/Transporter.cpp
	Game/Entities/Unit.cpp 
	Game/Entities/Update/UpdateBuilder.cpp
	Game/Entities/Vehicle.cpp
	Game/Entities/Vendor.cpp
	PacketHandlers/VehicleHandler.cpp
	Chat/WayPoints.cpp
	Game/Weather/WeatherProviderFactory.cpp
	Game/Weather/GeneratedWeatherProvider.cpp
	Game/Weather/OpenWeatherData.cpp
	Game/Weather/OpenWeatherDataParser.cpp
	Game/Weather/OpenWeatherProvider.cpp
	Game/Weather/WeatherInfo.cpp
	Game/Weather/WeatherMgr.cpp 
	Game/Network/WeatherPacketBuilder.cpp
	Game/World/World.cpp 
	Game/Instances/WorldCreator.cpp 
	Game/Network/WorldSession.cpp 
	Server/WorldSocket.cpp 
	Game/WorldStates/WorldStatesHandler.cpp
	Game/World/WorldRunnable.cpp 
	Framework/WordFilter/WordFilter.cpp 
	Server/MainServerDefines.cpp
	Server/CConsole.cpp 
	Server/ConsoleCommands.cpp 
	Server/ConsoleListener.cpp 
	Game/WUtil.cpp 
	Game/Spell/SpellProc.cpp
	Game/Spell/SpellProc_DeathKnight.cpp
	Game/Spell/SpellProc_Druid.cpp
	Game/Spell/SpellProc_Hunter.cpp
	Game/Spell/SpellProc_Items.cpp
	Game/Spell/SpellProc_Mage.cpp
	Game/Spell/SpellProc_Paladin.cpp
	Game/Spell/SpellProc_Priest.cpp
	Game/Spell/SpellProc_Rogue.cpp
	Game/Spell/SpellProc_Shaman.cpp
	Game/Spell/SpellProc_Warlock.cpp
	Game/Spell/SpellProc_Warrior.cpp
	Game/Spell/SpellMgr.cpp
	Game/Spell/Spell_DeathKnight.cpp
	Game/Spell/Spell_Druid.cpp
	Game/Spell/Spell_Hunter.cpp
	Game/Spell/Spell_Mage.cpp
	Game/Spell/Spell_Paladin.cpp
	Game/Spell/Spell_Priest.cpp
	Game/Spell/Spell_Rogue.cpp
	Game/Spell/Spell_Shaman.cpp
	Game/Spell/Spell_Warlock.cpp
	Game/Spell/Spell_Warrior.cpp
	Game/Entities/DeathKnight.cpp
	Game/Entities/Druid.cpp
	Game/Entities/Hunter.cpp
	Game/Entities/Mage.cpp
	Game/Entities/Paladin.cpp
	Game/Entities/Priest.cpp
	Game/Entities/Rogue.cpp
	Game/Entities/Shaman.cpp
	Game/Entities/Warlock.cpp
	Game/Entities/Warrior.cpp
	Server/WowCrypt.cpp
	PacketHandlers/NewHandlers/ItemPacketHandlers.cpp
	PacketHandlers/NewHandlers/PlayerPacketHandlers.cpp
	PacketHandlers/NewHandlers/LFGPacketHandlers.cpp
	PacketHandlers/NewHandlers/MiscPacketHandlers.cpp
	PacketHandlers/InventoryListBuilder.cpp
	Math/Math.cpp
)

SET( headers
	Game/Commands/AccountCommands.h
	Game/Network/AccountPackets.h
	Game/Network/ItemPackets.h
	Game/Network/LFGPackets.h
	Game/Achievements/AchievementMgr.h
	Game/Addons/AddonMgr.h
	Game/AI/AIInterface.h
	Game/AI/AIEvents.h
	Game/AreaTrigger.h
	Game/Arena/Arenas.h
	Game/Arena/ArenaTeam.h
	Game/Auction/AuctionHouse.h
	Game/Auction/AuctionMgr.h
	Server/BaseConsole.h
	Game/BattleGround/Battleground.h
	Game/BattleGround/BattlegroundMgr.h
	Game/Network/ByteBufferExtension.h
	Server/CConsole.h
	Map/CellHandler.h
	Chat/Channel.h
	Chat/ChannelMgr.h
	Chat/Chat.h
	Framework/Collision/CollideInterface.h
	Game/World/CommonScheduleThread.h
	Game/Entities/Summons/CompanionSummon.h
	Server/ConsoleCommands.h
	Game/Entities/Container.h
	Game/Entities/Corpse.h
	Game/Entities/Creature.h
	Game/Commands/CreatureCommands.h
	Server/DatabaseCleaner.h
	Game/World/DayWatcherThread.h
	DBC/DBCStores.h
	Game/Entities/DeathKnight.h
	Game/Instances/Definitions.h
	Game/Entities/Druid.h
	Game/Entities/DynamicObject.h
	Game/EquipmentSet/EquipmentSetMgr.h
	Framework/TimedEvents/Events.h
	Framework/TimedEvents/EventableObject.h
	Framework/TimedEvents/EventMgr.h
	Game/Factions/faction.h
	Game/Entities/GameObject.h
	Game/Gossip/Gossip.h
	Game/Group/Group.h
	Game/Entities/Summons/GuardianSummon.h
	Game/Guild/Guild.h
	Game/Honor/HonorHandler.h
	Game/Entities/Hunter.h
	Game/Entities/Item.h
	Game/Commands/ItemCommands.h
	Game/Item/ItemInterface.h
	Game/Item/ItemIterator.h
	Game/Item/ItemPrototype.h
	Game/Item/ItemPrototypeFilter.h
	Game/Item/PlayerItemPrototypeFilter.h
	Game/LFG/LfgMgr.h
	Localization/LocalizationMgr.h
	Math/LocationVector.h
	Server/LogonSocket.h
	Server/LogonCommHandler.h
	Game/Loot/LootMgr.h
	Game/Entities/Mage.h
	Game/Mail/MailMessage.h
	Game/Mail/MailBox.h
	Game/Mail/MailSystem.h
	Server/MainServerDefines.h
	Map/Map.h
	Map/MapCell.h
	Map/MapMgr.h
	Map/MapScriptInterface.h
	Game/Network/MessageRouter.h
	Game/Network/Messenger.h
	Game/Network/MovementInfo.h
	PacketHandlers/MiscHandler.h
	Game/Network/NameTables.h
	PacketHandlers/NPCHandler.h
	Game/Entities/ObjectConstants.h
	Game/Entities/Object.h
	Game/Entities/ObjectLocator.h
	Game/Storage/ObjectMgr.h
	Game/Storage/ObjectStorage.h
	Game/Network/Opcodes.h
	Game/Entities/Paladin.h
	Game/Entities/Pet.h
	Game/Entities/Player.h
	Game/Entities/PlayerCache.h
	Game/Commands/PlayerCommands.h
	Game/Network/PlayerMessenger.h
	Game/Entities/Summons/PossessedSummon.h
	Game/Entities/Priest.h
	Game/Quest/Quest.h
	Game/Quest/QuestMgr.h
	Game/Entities/Rogue.h
	Game/Scripting/ScriptMgr.h
	Game/Scripting/ScriptSetup.h
	Game/Entities/Shaman.h
	Game/Skill/Skill.h
	Game/Skill/SkillNameMgr.h
	Game/Entities/SpawnTimedEmotes.h
	Framework/SpeedCheatDetector/SpeedDetector.h
	Game/Spell/Spell.h
	Game/Spell/SpellAuras.h
	Game/Spell/SpellFailure.h
	Game/Spell/SpellMgr.h
	Game/Spell/SpellNameHashes.h
	Game/Spell/SpellProc.h
	Game/Spell/SpellTarget.h
	Game/Entities/Stats.h
	StdAfx.h
	Game/Entities/Summons/Summon.h
	Game/Entities/Summons/SummonHandler.h
	Game/Taxi/TaxiMgr.h
	Map/TerrainMgr.h
	Game/Entities/Summons/TotemSummon.h
	Game/Entities/Transporter.h
	Game/Entities/Unit.h
	Game/Entities/Update/UpdateBuilder.h
	Game/Entities/Update/UpdateFields.h
	Game/Entities/Update/UpdateMask.h
	Game/Entities/Vehicle.h
	Game/Entities/Vendor.h
	Game/Entities/Warlock.h
	Game/Entities/Warrior.h
	Game/AI/Waypoint.h
	Game/Weather/Weather.h
	Game/Weather/WeatherProvider.h
	Game/Weather/WeatherProviderFactory.h
	Game/Weather/GeneratedWeatherProvider.h
	Game/Weather/NullWeatherProvider.h
	Game/Weather/OpenWeatherData.h
	Game/Weather/OpenWeatherDataParser.h
	Game/Weather/OpenWeatherProvider.h
	Game/Weather/WeatherInfo.h
	Game/Weather/WeatherMgr.h
	Game/Network/WeatherPacketBuilder.h
	Game/Entities/Summons/WildSummon.h
	Framework/WordFilter/WordFilter.h
	Game/World/World.h
	Game/World/WorldConfig.h
	Game/Instances/WorldCreator.h
	Game/World/WorldRunnable.h
	Game/Network/WorldSession.h
	Server/WorldSocket.h
	Game/WorldStates/WorldStatesHandler.h
	Game/WorldStates/WorldStates.h
	Server/WowCrypt.h
	Game/Network/WoWGuid.h
	Game/WUtil.h
	PacketHandlers/NewHandlers/ItemPacketHandlers.h
	PacketHandlers/NewHandlers/PlayerPacketHandlers.h 
	PacketHandlers/NewHandlers/PacketHandlerMacros.h
	PacketHandlers/NewHandlers/LFGPacketHandlers.h
	PacketHandlers/NewHandlers/MiscPacketHandlers.h
	Math/Math.h
	PacketHandlers/InventoryListBuilder.h
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

SET( exe_sources
	Server/Main.cpp
	Server/Master.cpp
)

SET( exe_headers
	Server/Master.h
)

foreach(src IN ITEMS ${exe_sources} )
  SET( EXE_SRCS ${EXE_SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${exe_headers} )
  SET( EXE_HEADERS ${EXE_HEADERS} ${prefix}/${src} )
endforeach(src)

include(PCHSupport)
include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )

add_library( ${PROJECT_NAME}lib ${SRCS} ${HEADERS} )
add_executable( ${PROJECT_NAME} ${EXE_SRCS} ${EXE_HEADERS} )

IF( WIN32 )
    IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME}lib StdAfx.h ${prefix}/StdAfx.cpp)
    ENDIF()
ENDIF( WIN32 )

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)
set_target_properties(${PROJECT_NAME}lib PROPERTIES ENABLE_EXPORTS TRUE)

add_dependencies( ${PROJECT_NAME}lib shared g3dlite vmaplib detour recast)
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}lib shared g3dlite vmaplib detour recast)

target_link_libraries( ${PROJECT_NAME}lib shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} ${PROJECT_NAME}lib shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

############################################################### Tests ######################################################################

ENABLE_TESTING()

ADD_EXECUTABLE( WorldUtilTest ${prefix}/Tests/UtilTest.cpp )

ADD_DEPENDENCIES( WorldUtilTest ${PROJECT_NAME}lib )
TARGET_LINK_LIBRARIES( WorldUtilTest shared ${PROJECT_NAME}lib ${PCRE_LIBRARIES})

ADD_TEST( NAME WorldUtilTest COMMAND WorldUtilTest )

IF( WIN32 )
	FILE( COPY ${INSTALLED_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
ENDIF( WIN32 )
