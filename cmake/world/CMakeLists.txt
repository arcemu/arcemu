PROJECT(world)
SET( prefix ${ROOT_PATH}/src/world )
SET(sources
  DBC/DBCStores.cpp
	Game/Commands/AccountCommands.cpp
	Game/Network/AccountPackets.cpp
	Game/Network/ItemPackets.cpp
	AchievementMgr.cpp 
	CollideInterface.cpp  
	PacketHandlers/VoiceChatHandler.cpp 
	DayWatcherThread.cpp 
	CommonScheduleThread.cpp 
	ArenaTeam.cpp 
	PacketHandlers/ArenaTeamHandler.cpp 
	DatabaseCleaner.cpp 
	ObjectStorage.cpp  
	AddonMgr.cpp 
	AIInterface.cpp 
	AIEvents.cpp
	PacketHandlers/AreaTrigger.cpp 
	Arenas.cpp 
	AuctionHouse.cpp 
	PacketHandlers/AuctionHouseHandler.cpp
	AuctionMgr.cpp 
	Battleground.cpp
	Chat/BattlegroundCommands.cpp 
	PacketHandlers/BattlegroundHandler.cpp 
	BattlegroundMgr.cpp 
	Chat/Channel.cpp 
	PacketHandlers/ChannelHandler.cpp 
	PacketHandlers/CharacterHandler.cpp 
	Chat/Chat.cpp 
	PacketHandlers/ChatHandler.cpp 
	PacketHandlers/CombatHandler.cpp 
	Entities/Container.cpp 
	Entities/Corpse.cpp 
	Entities/Creature.cpp 
	Game/Commands/CreatureCommands.cpp
	Entities/Summons/Summon.cpp
	Entities/Summons/CompanionSummon.cpp
	Entities/Summons/GuardianSummon.cpp
	Entities/Summons/PossessedSummon.cpp
	Entities/Summons/TotemSummon.cpp
	Entities/Summons/WildSummon.cpp
	Chat/DebugCommands.cpp 
	PacketHandlers/DuelHandler.cpp 
	Entities/DynamicObject.cpp 
	EquipmentSetMgr.cpp
	EventableObject.cpp 
	EventMgr.cpp 
	faction.cpp 
	Entities/GameObject.cpp 
	PacketHandlers/GMTicketHandler.cpp 
	Chat/GMTicketCommands.cpp
	Gossip.cpp
	Group.cpp 
	PacketHandlers/GroupHandler.cpp 
	Guild.cpp 
	PacketHandlers/GuildHandler.cpp 
	HonorHandler.cpp 
	Chat/InstanceCommands.cpp 
	Entities/Item.cpp
	Game/Commands/ItemCommands.cpp
	PacketHandlers/ItemHandler.cpp 
	ItemInterface.cpp 
	Chat/Level0.cpp 
	Chat/Level1.cpp 
	Chat/Level2.cpp 
	Chat/Level3.cpp 
	PacketHandlers/LfgHandler.cpp 
	LfgMgr.cpp 
	LootMgr.cpp 
	LocalizationMgr.cpp 
	Server/LogonSocket.cpp 
	Server/LogonCommHandler.cpp 
	MailMessage.cpp
	MailBox.cpp
	PacketHandlers/MailHandler.cpp
	MailSystem.cpp 
	Map/MapCell.cpp 
	Map/Map.cpp 
	Map/MapScriptInterface.cpp 
	Map/MapMgr.cpp 
	MessageRouter.cpp
	Messenger.cpp
	PacketHandlers/MiscHandler.cpp 
	PacketHandlers/MovementHandler.cpp 
	MovementInfo.cpp
	PacketHandlers/NPCHandler.cpp 
	Entities/Object.cpp 
	ObjectMgr.cpp 
	Opcodes.cpp 
	Entities/Pet.cpp 
	PacketHandlers/PetHandler.cpp 
	Entities/Player.cpp 
	Game/Commands/PlayerCommands.cpp
	PlayerMessenger.cpp
	Entities/PlayerPacketWrapper.cpp 
	PacketHandlers/QueryHandler.cpp 
	Quest.cpp 
	PacketHandlers/QuestHandler.cpp 
	QuestMgr.cpp 
	Chat/QuestCommands.cpp 
	PacketHandlers/RaidHandler.cpp 
	Chat/RecallCommands.cpp 
	ReputationHandler.cpp 
	ScriptMgr.cpp 
	PacketHandlers/SocialHandler.cpp 
	SpeedDetector.cpp 
	Spell/SpellAuras.cpp 
	Spell/SpellTarget.cpp 
	Spell/Spell.cpp 
	StdAfx.cpp
	SummonHandler.cpp
	Spell/SpellEffects.cpp 
	PacketHandlers/SpellHandler.cpp 
	Spell/HackFixes.cpp 
	Stats.cpp 
	PacketHandlers/TaxiHandler.cpp 
	TaxiMgr.cpp 
	Map/TerrainMgr.cpp 
	PacketHandlers/TradeHandler.cpp
	Entities/Transporter.cpp
	Entities/Unit.cpp 
	UpdateBuilder.cpp
	Entities/Vehicle.cpp 
	PacketHandlers/VehicleHandler.cpp
	Chat/WayPoints.cpp 
	WeatherMgr.cpp 
	World.cpp 
	WorldCreator.cpp 
	WorldSession.cpp 
	Server/WorldSocket.cpp 
	WorldStatesHandler.cpp
	WorldRunnable.cpp 
	WordFilter.cpp 
	Server/Main.cpp 
	Server/Master.cpp 
	Server/CConsole.cpp 
	Server/ConsoleCommands.cpp 
	Server/ConsoleListener.cpp 
	WUtil.cpp 
	Spell/SpellProc.cpp
	Spell/SpellProc_DeathKnight.cpp
	Spell/SpellProc_Druid.cpp
	Spell/SpellProc_Hunter.cpp
	Spell/SpellProc_Items.cpp
	Spell/SpellProc_Mage.cpp
	Spell/SpellProc_Paladin.cpp
	Spell/SpellProc_Priest.cpp
	Spell/SpellProc_Rogue.cpp
	Spell/SpellProc_Shaman.cpp
	Spell/SpellProc_Warlock.cpp
	Spell/SpellProc_Warrior.cpp
	Spell/SpellMgr.cpp
	Spell/Spell_DeathKnight.cpp
	Spell/Spell_Druid.cpp
	Spell/Spell_Hunter.cpp
	Spell/Spell_Mage.cpp
	Spell/Spell_Paladin.cpp
	Spell/Spell_Priest.cpp
	Spell/Spell_Rogue.cpp
	Spell/Spell_Shaman.cpp
	Spell/Spell_Warlock.cpp
	Spell/Spell_Warrior.cpp
	Entities/DeathKnight.cpp
	Entities/Druid.cpp
	Entities/Hunter.cpp
	Entities/Mage.cpp
	Entities/Paladin.cpp
	Entities/Priest.cpp
	Entities/Rogue.cpp
	Entities/Shaman.cpp
	Entities/Warlock.cpp
	Entities/Warrior.cpp
	Server/WowCrypt.cpp
	PacketHandlers/NewHandlers/ItemPacketHandlers.cpp
	PacketHandlers/NewHandlers/PlayerPacketHandlers.cpp 
)

SET( headers
	Game/Commands/AccountCommands.h
	Game/Network/AccountPackets.h
	Game/Network/ItemPackets.h
	AchievementMgr.h
	AddonMgr.h
	AIInterface.h
	AIEvents.h
	AreaTrigger.h
	Arenas.h
	ArenaTeam.h
	AuctionHouse.h
	AuctionMgr.h
	Server/BaseConsole.h
	Battleground.h
	BattlegroundMgr.h
	ByteBufferExtension.h
	Server/CConsole.h
	Map/CellHandler.h
	Chat/Channel.h
	Chat/ChannelMgr.h
	Chat/Chat.h
	CollideInterface.h
	CommonScheduleThread.h
	Entities/Summons/CompanionSummon.h
	Server/ConsoleCommands.h
	Entities/Container.h
	Entities/Corpse.h
	Entities/Creature.h
	Game/Commands/CreatureCommands.h
	DatabaseCleaner.h
	DayWatcherThread.h
	DBC/DBCStores.h
	Entities/DeathKnight.h
	Definitions.h
	Entities/Druid.h
	Entities/DynamicObject.h
	EquipmentSetMgr.h
	Events.h
	EventableObject.h
	EventMgr.h
	faction.h
	Entities/GameObject.h
	Gossip.h
	Group.h
	Entities/Summons/GuardianSummon.h
	Guild.h
	HonorHandler.h
	Entities/Hunter.h
	Entities/Item.h
	Game/Commands/ItemCommands.h
	ItemInterface.h
	ItemIterator.h
	ItemPrototype.h
	LfgMgr.h
	LocalizationMgr.h
	LocationVector.h
	Server/LogonSocket.h
	Server/LogonCommHandler.h
	LootMgr.h
	Entities/Mage.h
	MailMessage.h
	MailBox.h
	MailSystem.h
	Server/MainServerDefines.h
	Map/Map.h
	Map/MapCell.h
	Map/MapMgr.h
	Map/MapScriptInterface.h
	Server/Master.h
	MessageRouter.h
	Messenger.h
	MovementInfo.h
	PacketHandlers/MiscHandler.h
	NameTables.h
	PacketHandlers/NPCHandler.h
	Entities/ObjectConstants.h
	Entities/Object.h
	ObjectMgr.h
	ObjectStorage.h
	Opcodes.h
	Entities/Paladin.h
	Entities/Pet.h
	Entities/Player.h
	Entities/PlayerCache.h
	Game/Commands/PlayerCommands.h
	PlayerMessenger.h
	Entities/Summons/PossessedSummon.h
	Entities/Priest.h
	Quest.h
	QuestMgr.h
	Entities/Rogue.h
	ScriptMgr.h
	ScriptSetup.h
	Entities/Shaman.h
	Skill.h
	SkillNameMgr.h
	SpawnTimedEmotes.h
	SpeedDetector.h
	Spell/Spell.h
	Spell/SpellAuras.h
	Spell/SpellFailure.h
	Spell/SpellMgr.h
	Spell/SpellNameHashes.h
	Spell/SpellProc.h
	Spell/SpellTarget.h
	Stats.h
	StdAfx.h
	Entities/Summons/Summon.h
	SummonHandler.h
	TaxiMgr.h
	Map/TerrainMgr.h
	Entities/Summons/TotemSummon.h
	Entities/Transporter.h
	Entities/Unit.h
	UpdateBuilder.h
	UpdateFields.h
	UpdateMask.h
	Entities/Vehicle.h
	Entities/Warlock.h
	Entities/Warrior.h
	Waypoint.h
	WeatherMgr.h
	Entities/Summons/WildSummon.h
	WordFilter.h
	World.h
	WorldConfig.h
	WorldCreator.h
	WorldRunnable.h
	WorldSession.h
	Server/WorldSocket.h
	WorldStatesHandler.h
	WorldStates.h
	Server/WowCrypt.h
	WoWGuid.h
	WUtil.h
	PacketHandlers/NewHandlers/ItemPacketHandlers.h
	PacketHandlers/NewHandlers/PlayerPacketHandlers.h 
	PacketHandlers/NewHandlers/PacketHandlerMacros.h
)
	

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include(PCHSupport)
include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )

# On Linux we cannot link to the executable, so we need to build an object library
# then link an executable, and a static library from that.
# After that we can finally link the test executable to the static library

IF( WIN32 )
    add_executable( ${PROJECT_NAME} ${SRCS} ${HEADERS} )

    IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} StdAfx.h ${prefix}/StdAfx.cpp)
    ENDIF()

    set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)

    add_dependencies( ${PROJECT_NAME} shared g3dlite vmaplib detour recast)

    target_link_libraries( ${PROJECT_NAME} shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )

ELSE( WIN32 )
    add_library( worldobjlib OBJECT ${SRCS} ${HEADERS} )
    add_executable( ${PROJECT_NAME} $<TARGET_OBJECTS:worldobjlib> )
    add_library( ${PROJECT_NAME}lib $<TARGET_OBJECTS:worldobjlib> )

    set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)
    set_target_properties(${PROJECT_NAME}lib PROPERTIES ENABLE_EXPORTS TRUE)

    add_dependencies( ${PROJECT_NAME} shared g3dlite vmaplib detour recast)
    add_dependencies( ${PROJECT_NAME}lib shared g3dlite vmaplib detour recast)

    target_link_libraries( ${PROJECT_NAME} shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )
    target_link_libraries( ${PROJECT_NAME}lib shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )
ENDIF( WIN32 )


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

ENABLE_TESTING()

ADD_EXECUTABLE( WorldUtilTest ${prefix}/Tests/UtilTest.cpp )

IF( WIN32 )
    ADD_DEPENDENCIES( WorldUtilTest ${PROJECT_NAME} )
    TARGET_LINK_LIBRARIES( WorldUtilTest shared ${PROJECT_NAME} )
ELSE( WIN32 )
    ADD_DEPENDENCIES( WorldUtilTest ${PROJECT_NAME}lib )
    TARGET_LINK_LIBRARIES( WorldUtilTest shared ${PROJECT_NAME}lib )
ENDIF( WIN32 )

ADD_TEST( NAME WorldUtilTest COMMAND WorldUtilTest )

IF( WIN32 )
	FILE( COPY ${INSTALLED_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
ENDIF( WIN32 )
