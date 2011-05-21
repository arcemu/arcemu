PROJECT(world)
SET( prefix ${ROOT_PATH}/src/arcemu-world )
SET(sources
  vmap/BIH.cpp
  vmap/MapTree.cpp
  vmap/ModelInstance.cpp
  vmap/TileAssembler.cpp
  vmap/VMapFactory.cpp
  vmap/VMapManager2.cpp
  vmap/WorldModel.cpp
	AchievementMgr.cpp 
	CollideInterface.cpp  
	VoiceChatHandler.cpp 
	DayWatcherThread.cpp 
	CommonScheduleThread.cpp 
	ArenaTeam.cpp 
	DatabaseCleaner.cpp 
	ObjectStorage.cpp  
	AddonMgr.cpp 
	AIInterface.cpp 
	AlteracValley.cpp 
	ArathiBasin.cpp 
	AreaTrigger.cpp 
	Arenas.cpp 
	AuctionHouse.cpp 
	AuctionMgr.cpp 
	BattlegroundCommands.cpp 
	BattlegroundHandler.cpp 
	BattlegroundMgr.cpp 

	StrandOfTheAncient.cpp 
	Channel.cpp 
	ChannelHandler.cpp 
	CharacterHandler.cpp 
	Chat.cpp 
	ChatHandler.cpp 
	CombatHandler.cpp 
	Container.cpp 
	Corpse.cpp 
	Creature.cpp 
	debugcmds.cpp 
	DuelHandler.cpp 
	DynamicObject.cpp 
	EquipmentSetMgr.cpp
	EventableObject.cpp 
	EyeOfTheStorm.cpp 
	EventMgr.cpp 
	faction.cpp 
	GameObject.cpp 
	GMTicket.cpp 
	GMTicketCommands.cpp
	Gossip.cpp
	Group.cpp 
	GroupHandler.cpp 
	Guild.cpp 
	GuildHandler.cpp 
	HonorHandler.cpp 
	InstanceCommands.cpp 
	Item.cpp 
	ItemHandler.cpp 
	ItemInterface.cpp 
	Level0.cpp 
	Level1.cpp 
	Level2.cpp 
	Level3.cpp 
	LfgHandler.cpp 
	LfgMgr.cpp 
	LootMgr.cpp 
	LocalizationMgr.cpp 
	LogonCommClient.cpp 
	LogonCommHandler.cpp 
	MailSystem.cpp 
	MapCell.cpp 
	Map.cpp 
	MapScriptInterface.cpp 
	MapMgr.cpp 
	MiscHandler.cpp 
	MovementHandler.cpp 
	NPCHandler.cpp 
	Object.cpp 
	ObjectMgr.cpp 
	Opcodes.cpp 
	Pet.cpp 
	PetHandler.cpp 
	Player.cpp 
	PlayerPacketWrapper.cpp 
	QueryHandler.cpp 
	Quest.cpp 
	QuestHandler.cpp 
	QuestMgr.cpp 
	QuestCommands.cpp 
	RaidHandler.cpp 
	RecallCommands.cpp 
	ReputationHandler.cpp 
	ScriptMgr.cpp 
	SocialHandler.cpp 
	SpeedDetector.cpp 
	SpellAuras.cpp 
	SpellTarget.cpp 
	Spell.cpp 
	StdAfx.cpp
	SpellEffects.cpp 
	SpellHandler.cpp 
	HackFixes.cpp 
	Stats.cpp 
	TaxiHandler.cpp 
	TaxiMgr.cpp 
	TerrainMgr.cpp 
	TradeHandler.cpp 
	TransporterHandler.cpp 
	Unit.cpp 
	UnixMetric.cpp 
	Vehicle.cpp 
	WarsongGulch.cpp 
	WayPoints.cpp 
	WeatherMgr.cpp 
	World.cpp 
	WorldCreator.cpp 
	WorldSession.cpp 
	WorldSocket.cpp 
	WorldState.cpp 
	WorldRunnable.cpp 
	WordFilter.cpp 
	Main.cpp 
	Master.cpp 
	CConsole.cpp 
	ConsoleCommands.cpp 
	ConsoleListener.cpp 
	WUtil.cpp 
	SpellProc.cpp
	SpellProc_DeathKnight.cpp
	SpellProc_Druid.cpp
	SpellProc_Hunter.cpp
	SpellProc_Items.cpp
	SpellProc_Mage.cpp
	SpellProc_Paladin.cpp
	SpellProc_Priest.cpp
	SpellProc_Rogue.cpp
	SpellProc_Shaman.cpp
	SpellProc_Warlock.cpp
	SpellProc_Warrior.cpp
	SpellMgr.cpp
	Spell_DeathKnight.cpp
	Spell_Druid.cpp
	Spell_Hunter.cpp
	Spell_Mage.cpp
	Spell_Paladin.cpp
	Spell_Priest.cpp
	Spell_Rogue.cpp
	Spell_Shaman.cpp
	Spell_Warlock.cpp
	Spell_Warrior.cpp
	DeathKnight
	Druid
	Hunter
	Mage
	Paladin
	Priest
	Rogue
	Shaman
	Warlock
	Warrior )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include(PCHSupport)
include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )
add_executable( ${PROJECT_NAME} ${SRCS} )

IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} StdAfx.h ${prefix}/StdAfx.cpp)
ENDIF()
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)

add_dependencies( ${PROJECT_NAME} shared g3dlite)
target_link_libraries( ${PROJECT_NAME} shared  g3dlite ${PCRE_LIBRARIES} )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )





