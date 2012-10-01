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

#ifndef WOWSERVER_CHAT_H
#define WOWSERVER_CHAT_H

#include "SkillNameMgr.h"

class ChatHandler;
class WorldSession;
class Player;
class Unit;

enum ChatMsg
{
    CHAT_MSG_ADDON									= -1,
    CHAT_MSG_SYSTEM                                 = 0,    //28,   CHAT_MSG_SYSTEM                 = 0x00,         0
    CHAT_MSG_SAY									= 1,
    CHAT_MSG_PARTY									= 2,
    CHAT_MSG_RAID									= 3,
    CHAT_MSG_GUILD									= 4,
    CHAT_MSG_OFFICER								= 5,
    CHAT_MSG_YELL									= 6,
    CHAT_MSG_WHISPER								= 7,
    CHAT_MSG_WHISPER_MOB							= 8,//CHAT_MSG_WHISPER_INFORM
    CHAT_MSG_WHISPER_INFORM							= 9,//CHAT_MSG_REPLY
    CHAT_MSG_EMOTE									= 10,
    CHAT_MSG_TEXT_EMOTE								= 11,
    CHAT_MSG_MONSTER_SAY							= 12,
    CHAT_MSG_MONSTER_PARTY							= 13,
    CHAT_MSG_MONSTER_YELL							= 14,
    CHAT_MSG_MONSTER_WHISPER						= 15,
    CHAT_MSG_MONSTER_EMOTE							= 16,
    CHAT_MSG_CHANNEL								= 17,
    CHAT_MSG_CHANNEL_JOIN							= 18,
    CHAT_MSG_CHANNEL_LEAVE							= 19,
    CHAT_MSG_CHANNEL_LIST							= 20,
    CHAT_MSG_CHANNEL_NOTICE							= 21,
    CHAT_MSG_CHANNEL_NOTICE_USER					= 22,
    CHAT_MSG_AFK									= 23,
    CHAT_MSG_DND									= 24,
    CHAT_MSG_IGNORED								= 25,
    CHAT_MSG_SKILL									= 26,
    CHAT_MSG_LOOT									= 27,
    CHAT_MSG_MONEY                                  = 28,
    CHAT_MSG_OPENING                                = 29,
    CHAT_MSG_TRADESKILLS                            = 30,
    CHAT_MSG_PET_INFO                               = 31,
    CHAT_MSG_COMBAT_MISC_INFO                       = 32,
    CHAT_MSG_COMBAT_XP_GAIN                         = 33,
    CHAT_MSG_COMBAT_HONOR_GAIN                      = 34,
    CHAT_MSG_COMBAT_FACTION_CHANGE                  = 35,
    CHAT_MSG_BG_EVENT_NEUTRAL						= 36,
    CHAT_MSG_BG_EVENT_ALLIANCE						= 37,
    CHAT_MSG_BG_EVENT_HORDE							= 38,
    CHAT_MSG_RAID_LEADER							= 39,
    CHAT_MSG_RAID_WARNING							= 40,
    CHAT_MSG_RAID_WARNING_WIDESCREEN				= 41,
    CHAT_MSG_RAID_BOSS_EMOTE                        = 42,
    CHAT_MSG_FILTERED								= 43,
    CHAT_MSG_BATTLEGROUND							= 44,
    CHAT_MSG_BATTLEGROUND_LEADER					= 45,
    CHAT_MSG_RESTRICTED								= 46,
    CHAT_MSG_ACHIEVEMENT							= 48,
    CHAT_MSG_GUILD_ACHIEVEMENT						= 49,
    CHAT_MSG_PARTY_LEADER							= 51,
};

enum Languages
{
    LANG_UNIVERSAL                              = 0x00,
    LANG_ORCISH                                 = 0x01,
    LANG_DARNASSIAN                             = 0x02,
    LANG_TAURAHE                                = 0x03,
    LANG_DWARVISH                               = 0x06,
    LANG_COMMON                                 = 0x07,
    LANG_DEMONIC                                = 0x08,
    LANG_TITAN                                  = 0x09,
    LANG_THELASSIAN                             = 0x0A,
    LANG_DRACONIC                               = 0x0B,
    LANG_KALIMAG                                = 0x0C,
    LANG_GNOMISH                                = 0x0D,
    LANG_TROLL                                  = 0x0E,
    LANG_GUTTERSPEAK                            = 0x21,
    LANG_DRAENEI                                = 0x23,
    NUM_LANGUAGES                               = 0x24
};

/*#define MSG_COLOR_YELLOW	"|r"
#define MSG_COLOR_RED	   "|cffff2020"
#define MSG_COLOR_GREEN	 "|c1f40af20"
#define MSG_COLOR_LIGHTRED  "|cffff6060"*/

#define MSG_COLOR_LIGHTRED       "|cffff6060"
#define MSG_COLOR_LIGHTBLUE      "|cff00ccff"
#define MSG_COLOR_TORQUISEBLUE	 "|cff00C78C"
#define MSG_COLOR_SPRINGGREEN	 "|cff00FF7F"
#define MSG_COLOR_GREENYELLOW	 "|cffADFF2F"
#define MSG_COLOR_BLUE           "|cff0000ff"
#define MSG_COLOR_PURPLE		 "|cffDA70D6"
#define MSG_COLOR_GREEN	         "|cff00ff00"
#define MSG_COLOR_RED            "|cffff0000"
#define MSG_COLOR_GOLD           "|cffffcc00"
#define MSG_COLOR_GOLD2			 "|cffFFC125"
#define MSG_COLOR_GREY           "|cff888888"
#define MSG_COLOR_WHITE          "|cffffffff"
#define MSG_COLOR_SUBWHITE       "|cffbbbbbb"
#define MSG_COLOR_MAGENTA        "|cffff00ff"
#define MSG_COLOR_YELLOW         "|cffffff00"
#define MSG_COLOR_ORANGEY		 "|cffFF4500"
#define MSG_COLOR_CHOCOLATE		 "|cffCD661D"
#define MSG_COLOR_CYAN           "|cff00ffff"
#define MSG_COLOR_IVORY			 "|cff8B8B83"
#define MSG_COLOR_LIGHTYELLOW	 "|cffFFFFE0"
#define MSG_COLOR_SEXGREEN		 "|cff71C671"
#define MSG_COLOR_SEXTEAL		 "|cff388E8E"
#define MSG_COLOR_SEXPINK		 "|cffC67171"
#define MSG_COLOR_SEXBLUE		 "|cff00E5EE"
#define MSG_COLOR_SEXHOTPINK	 "|cffFF6EB4"

#define CHECKSESSION if(m_session == NULL) return NULL; \
	if(m_session->GetPlayer() == NULL) return NULL;


int32 GetSpellIDFromLink(const char* spelllink);
uint16 GetItemIDFromLink(const char* itemlink, uint32* itemid);

class ChatCommand
{
	public:
		const char* 	   Name;
		char			   CommandGroup;
		bool (ChatHandler::*Handler)(const char* args, WorldSession* m_session) ;
		std::string		Help;
		ChatCommand*     ChildCommands;
		uint32			 NormalValueField;
		uint32			 MaxValueField;
		// ValueType: 0 = nothing, 1 = uint, 2 = float
		uint16			 ValueType;
};

class SERVER_DECL CommandTableStorage : public Singleton<CommandTableStorage>
{
		// List command containers ex. .character is a container of .character additem
		ChatCommand* _modifyCommandTable;
		ChatCommand* _debugCommandTable;
		ChatCommand* _waypointCommandTable;
		ChatCommand* _GMTicketCommandTable;
		ChatCommand* _GuildCommandTable;
		ChatCommand* _GameObjectCommandTable;
		ChatCommand* _BattlegroundCommandTable;
		ChatCommand* _NPCCommandTable;
		ChatCommand* _CheatCommandTable;
		ChatCommand* _accountCommandTable;
		ChatCommand* _honorCommandTable;
		ChatCommand* _petCommandTable;
		ChatCommand* _recallCommandTable;
		ChatCommand* _questCommandTable;
		ChatCommand* _serverCommandTable;
		ChatCommand* _gmCommandTable;
		ChatCommand* _characterCommandTable;
		ChatCommand* _lookupCommandTable;
		ChatCommand* _adminCommandTable;
		ChatCommand* _kickCommandTable;
		ChatCommand* _banCommandTable;
		ChatCommand* _unbanCommandTable;
		ChatCommand* _instanceCommandTable;
		ChatCommand* _arenaCommandTable;
		ChatCommand* _achievementCommandTable;
		ChatCommand* _vehicleCommandTable;
		ChatCommand* _commandTable;

		ChatCommand* GetSubCommandTable(const char* name);
	public:
		void Init();
		void Dealloc();
		void Load();
		void Override(const char* command, const char* level);
		ARCEMU_INLINE ChatCommand* Get() { return _commandTable; }
};

class SERVER_DECL ChatHandler : public Singleton<ChatHandler>
{
		friend class CommandTableStorage;
	public:
		ChatHandler();
		~ChatHandler();

		WorldPacket* FillMessageData(uint32 type, uint32 language,  const char* message, uint64 guid, uint8 flag = 0) const;
		WorldPacket* FillSystemMessageData(const char* message) const;

		int ParseCommands(const char* text, WorldSession* session);

		void SystemMessage(WorldSession* m_session, const char* message, ...);
		void ColorSystemMessage(WorldSession* m_session, const char* colorcode, const char* message, ...);
		void RedSystemMessage(WorldSession* m_session, const char* message, ...);
		void GreenSystemMessage(WorldSession* m_session, const char* message, ...);
		void BlueSystemMessage(WorldSession* m_session, const char* message, ...);
		void RedSystemMessageToPlr(Player* plr, const char* message, ...);
		void GreenSystemMessageToPlr(Player* plr, const char* message, ...);
		void BlueSystemMessageToPlr(Player* plr, const char* message, ...);
		void SystemMessageToPlr(Player* plr, const char* message, ...);

	protected:

		bool hasStringAbbr(const char* s1, const char* s2);
		void SendMultilineMessage(WorldSession* m_session, const char* str);

		bool ExecuteCommandInTable(ChatCommand* table, const char* text, WorldSession* m_session);
		bool ShowHelpForCommand(WorldSession* m_session, ChatCommand* table, const char* cmd);
		void SendHighlightedName(WorldSession* m_session, const char* prefix, const char* full_name, string & lowercase_name, string & highlight, uint32 id);
		void SendItemLinkToPlayer(ItemPrototype* iProto, WorldSession* pSession, bool ItemCount, Player* owner, uint32 language = NULL);

		ChatCommand* getCommandTable();

		// Level 0 commands
		bool HandleHelpCommand(const char* args, WorldSession* m_session);
		bool HandleCommandsCommand(const char* args, WorldSession* m_session);
		bool HandleNYICommand(const char* args, WorldSession* m_session);
		bool HandleAcctCommand(const char* args, WorldSession* m_session);
		bool HandleStartCommand(const char* args, WorldSession* m_session);
		bool HandleInfoCommand(const char* args, WorldSession* m_session);
		bool HandleNetworkStatusCommand(const char* args, WorldSession* m_session);
		bool HandleDismountCommand(const char* args, WorldSession* m_session);
		bool HandleSaveCommand(const char* args, WorldSession* m_session);
		bool HandleGMListCommand(const char* args, WorldSession* m_session);
		bool HandleGMStatusCommand(const char* args, WorldSession* m_session);
		bool HandleGmLogCommentCommand(const char* args , WorldSession* m_session);
		bool HandleRatingsCommand(const char* args , WorldSession* m_session);
		bool HandleSimpleDistanceCommand(const char* args , WorldSession* m_session);
		bool HandlePhaseCommand(const char* args , WorldSession* m_session);

		// Level 1 commands
		bool CmdSetValueField(WorldSession* m_session, uint32 field, uint32 fieldmax, const char* fieldname, const char* args);
		bool CmdSetFloatField(WorldSession* m_session, uint32 field, uint32 fieldmax, const char* fieldname, const char* args);
		bool HandleSummonCommand(const char* args, WorldSession* m_session);
		bool HandleAppearCommand(const char* args, WorldSession* m_session);
		bool HandleAnnounceCommand(const char* args, WorldSession* m_session);
		bool HandleGMAnnounceCommand(const char* args, WorldSession* m_session);
		bool HandleWAnnounceCommand(const char* args, WorldSession* m_session);
		bool HandleGMOnCommand(const char* args, WorldSession* m_session);
		bool HandleGMOffCommand(const char* args, WorldSession* m_session);
		bool HandleGPSCommand(const char* args, WorldSession* m_session);
		bool HandleKickCommand(const char* args, WorldSession* m_session);
		bool HandleTaxiCheatCommand(const char* args, WorldSession* m_session);
		bool HandleModifySpeedCommand(const char* args, WorldSession* m_session);
		bool HandleModifyTPsCommand(const char* args, WorldSession* m_session);

		bool HandleDeveloperCommand(const char* args, WorldSession* m_session);

		// Debug Commands
		bool HandleDebugInFrontCommand(const char* args, WorldSession* m_session);
		bool HandleShowReactionCommand(const char* args, WorldSession* m_session);
		bool HandleAIMoveCommand(const char* args, WorldSession* m_session);
		bool HandleMoveInfoCommand(const char* args, WorldSession* m_session);
		bool HandleDistanceCommand(const char* args, WorldSession* m_session);
		bool HandleFaceCommand(const char* args, WorldSession* m_session);
		bool HandleSetBytesCommand(const char* args, WorldSession* m_session);
		bool HandleGetBytesCommand(const char* args, WorldSession* m_session);
		bool HandleDebugLandWalk(const char* args, WorldSession* m_session);
		bool HandleDebugUnroot(const char* args, WorldSession* m_session);
		bool HandleDebugRoot(const char* args, WorldSession* m_session);
		bool HandleDebugWaterWalk(const char* args, WorldSession* m_session);
		bool HandleAggroRangeCommand(const char* args, WorldSession* m_session);
		bool HandleKnockBackCommand(const char* args, WorldSession* m_session);
		bool HandleFadeCommand(const char* args, WorldSession* m_session);
		bool HandleThreatModCommand(const char* args, WorldSession* m_session);
		bool HandleCalcThreatCommand(const char* args, WorldSession* m_session);
		bool HandleThreatListCommand(const char* args, WorldSession* m_session);
		bool HandleNpcSpawnLinkCommand(const char* args, WorldSession* m_session);
		bool HandleDebugDumpCoordsCommmand(const char* args, WorldSession* m_session);
		bool HandleSendpacket(const char* args, WorldSession* m_session);
		bool HandleSQLQueryCommand(const char* args, WorldSession* m_session);
		bool HandleRangeCheckCommand(const char* args , WorldSession* m_session);
		bool HandleSendFailed(const char* args , WorldSession* m_session);
		bool HandlePlayMovie(const char* args, WorldSession* m_session);
		bool HandleAuraUpdateAdd(const char* args, WorldSession* m_session);
		bool HandleAuraUpdateRemove(const char* args, WorldSession* m_session);
		bool HandleDebugSpawnWarCommand(const char* args, WorldSession* m_session);
		bool HandleUpdateWorldStateCommand( const char *args, WorldSession *session );
		bool HandleInitWorldStatesCommand( const char *args, WorldSession *session );
		bool HandleClearWorldStatesCommand( const char *args, WorldSession *session );

		// WayPoint Commands
		bool HandleWPAddCommand(const char* args, WorldSession* m_session);
		bool HandleWPShowCommand(const char* args, WorldSession* m_session);
		bool HandleWPHideCommand(const char* args, WorldSession* m_session);
		bool HandleWPDeleteCommand(const char* args, WorldSession* m_session);
		bool HandleWPFlagsCommand(const char* args, WorldSession* m_session);
		bool HandleWPMoveHereCommand(const char* args, WorldSession* m_session);
		bool HandleWPWaitCommand(const char* args, WorldSession* m_session);
		bool HandleWPEmoteCommand(const char* args, WorldSession* m_session);
		bool HandleWPSkinCommand(const char* args, WorldSession* m_session);
		bool HandleWPChangeNoCommand(const char* args, WorldSession* m_session);
		bool HandleWPInfoCommand(const char* args, WorldSession* m_session);
		bool HandleWPMoveTypeCommand(const char* args, WorldSession* m_session);
		bool HandleSaveWaypoints(const char* args, WorldSession* m_session);
		bool HandleGenerateWaypoints(const char* args, WorldSession* m_session);
		bool HandleDeleteWaypoints(const char* args, WorldSession* m_session);

		// Guild commands
		bool HandleGuildJoinCommand(const char* args, WorldSession* m_session);
		bool HandleGuildMembersCommand(const char* args, WorldSession* m_session);
		bool CreateGuildCommand(const char* args, WorldSession* m_session);
		bool HandleRenameGuildCommand(const char* args, WorldSession* m_session);
		bool HandleGuildRemovePlayerCommand(const char* args, WorldSession* m_session);
		bool HandleGuildDisbandCommand(const char* args, WorldSession* m_session);

		// Level 2 commands
		bool HandleNameCommand(const char* args, WorldSession* m_session);
		bool HandleSubNameCommand(const char* args, WorldSession* m_session);
		bool HandleDeleteCommand(const char* args, WorldSession* m_session);
		bool HandleDeMorphCommand(const char* args, WorldSession* m_session);
		bool HandleItemCommand(const char* args, WorldSession* m_session);
		bool HandleItemRemoveCommand(const char* args, WorldSession* m_session);
		bool HandleNPCFlagCommand(const char* args, WorldSession* m_session);
		bool HandleSaveAllCommand(const char* args, WorldSession* m_session);
		bool HandleStartBGCommand(const char* args, WorldSession* m_session);
		bool HandlePauseBGCommand(const char* args, WorldSession* m_session);
		bool HandleResetScoreCommand(const char* args, WorldSession* m_session);
		bool HandleBGInfoCommnad(const char* args, WorldSession* m_session);
		bool HandleInvincibleCommand(const char* args, WorldSession* m_session);
		bool HandleInvisibleCommand(const char* args, WorldSession* m_session);
		bool HandleKillCommand(const char* args, WorldSession* m_session);
		bool HandleKillByPlrCommand(const char* args , WorldSession* m_session);
		bool HandleGOSelect(const char* args, WorldSession* m_session);
		bool HandleGODelete(const char* args, WorldSession* m_session);
		bool HandleGOSpawn(const char* args, WorldSession* m_session);
		bool HandleGOPhaseCommand(const char* args, WorldSession* m_session);
		bool HandleGOInfo(const char* args, WorldSession* m_session);
		bool HandleGOEnable(const char* args, WorldSession* m_session);
		bool HandleGOActivate(const char* args, WorldSession* m_session);
		bool HandleGORotate(const char* args, WorldSession* m_session);
		bool HandleGOMove(const char* args, WorldSession* m_session);
		bool HandleAddAIAgentCommand(const char* args, WorldSession* m_session);
		bool HandleListAIAgentCommand(const char* args, WorldSession* m_session);
		bool HandleRepairItemsCommand(const char* args, WorldSession* m_session);
		bool HandleSetTitle(const char* args, WorldSession* m_session);

		// Level 3 commands
		bool HandleMassSummonCommand(const char* args, WorldSession* m_session);
		bool HandleWorldPortCommand(const char* args, WorldSession* m_session);
		bool HandlePortToCreatureSpawnCommand(const char* args, WorldSession* m_session);
		bool HandlePortToGameObjectSpawnCommand(const char* args, WorldSession* m_session);
		bool HandleMoveCommand(const char* args, WorldSession* m_session);
		bool HandleLearnCommand(const char* args, WorldSession* m_session);
		bool HandleReviveCommand(const char* args, WorldSession* m_session);
		bool HandleGenderChanger(const char* args, WorldSession* m_session);
		bool HandleAddGraveCommand(const char* args, WorldSession* m_session);
		bool HandleExploreCheatCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketListCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketGetByIdCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketRemoveByIdCommand(const char* args, WorldSession* m_session);
#ifndef GM_TICKET_MY_MASTER_COMPATIBLE
		void SendGMSurvey();
		bool HandleGMTicketAssignToCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketReleaseCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketCommentCommand(const char* args, WorldSession* m_session);
		bool HandleGMTicketDeletePermanentCommand(const char* args, WorldSession* m_session);
#endif
		bool HandleGMTicketToggleTicketSystemStatusCommand(const char* args, WorldSession* m_session);
		bool HandleAddSkillCommand(const char* args, WorldSession* m_session);
		bool HandleAddInvItemCommand(const char* args, WorldSession* m_session);
		bool HandleResetReputationCommand(const char* args, WorldSession* m_session);
		bool HandleLearnSkillCommand(const char* args, WorldSession* m_session);
		bool HandleModifySkillCommand(const char* args, WorldSession* m_session);
		bool HandleRemoveSkillCommand(const char* args, WorldSession* m_session);
		bool HandleNpcInfoCommand(const char* args, WorldSession* m_session);
		bool HandleCreaturePhaseCommand(const char* args, WorldSession* m_session);
		bool HandleEmoteCommand(const char* args, WorldSession* m_session);
		bool HandleIncreaseWeaponSkill(const char* args, WorldSession* m_session);
		bool HandleCastSpellCommand(const char* args, WorldSession* m_session);
		bool HandleCastSpellNECommand(const char* args, WorldSession* m_session);
		bool HandleCastSelfCommand(const char* args, WorldSession* m_session);
		bool HandleLevelUpCommand(const char* args, WorldSession* m_session);
		bool HandleModifyGoldCommand(const char* args, WorldSession* m_session);
		bool HandleMonsterSayCommand(const char* args, WorldSession* m_session);
		bool HandleMonsterYellCommand(const char* args, WorldSession* m_session);
		bool HandleNpcComeCommand(const char* args, WorldSession* m_session);
		bool HandleNPCEquipOneCommand(const char* args, WorldSession* m_session);
		bool HandleNPCEquipTwoCommand(const char* args, WorldSession* m_session);
		bool HandleNPCEquipThreeCommand(const char* args, WorldSession* m_session);
		bool HandleClearCooldownsCommand(const char* args, WorldSession* m_session);
		bool HandleBattlegroundCommand(const char* args, WorldSession* m_session);
		bool HandleSetWorldStateCommand(const char* args, WorldSession* m_session);
		bool HandleSetWorldStatesCommand(const char* args, WorldSession* m_session);
		bool HandlePlaySoundCommand(const char* args, WorldSession* m_session);
		bool HandleSetBattlefieldStatusCommand(const char* args, WorldSession* m_session);
		bool HandleNpcReturnCommand(const char* args, WorldSession* m_session);
		bool HandleAccountBannedCommand(const char* args, WorldSession* m_session);
		bool HandleAccountLevelCommand(const char* args, WorldSession* m_session);
		bool HandleResetTalentsCommand(const char* args, WorldSession* m_session);
		bool HandleResetSpellsCommand(const char* args, WorldSession* m_session);
		bool HandleNpcFollowCommand(const char* args, WorldSession* m_session);
		bool HandleFormationLink1Command(const char* args, WorldSession* m_session);
		bool HandleFormationLink2Command(const char* args, WorldSession* m_session);
		bool HandleNullFollowCommand(const char* args, WorldSession* m_session);
		bool HandleFormationClearCommand(const char* args, WorldSession* m_session);
		bool HandleResetSkillsCommand(const char* args, WorldSession* m_session);
		bool HandleGetSkillLevelCommand(const char* args, WorldSession* m_session);
		bool HandleGetSkillsInfoCommand(const char* args, WorldSession* m_session);
		bool HandlePlayerInfo(const char* args, WorldSession* m_session);
		bool HandleCreatureSpawnCommand(const char* args, WorldSession* m_session);
		bool HandleCreatureRespawnCommand(const char* args, WorldSession* m_session);
		bool HandleNPCCanFlyCommand(const char* args, WorldSession* m_session);
		bool HandleNPCOnGOCommand(const char* args, WorldSession* m_session);
		bool HandleNPCLootCommand(const char* args, WorldSession* m_session);
		bool HandleNPCCastCommand(const char* args, WorldSession* m_session);

		// Ban
		bool HandleBanCharacterCommand(const char* args, WorldSession* m_session);
		bool HandleBanAllCommand(const char* args, WorldSession* m_session);
		bool HandleUnBanCharacterCommand(const char* args, WorldSession* m_session);

		// BattleGround
		bool HandleSetBGScoreCommand(const char* args, WorldSession* m_session);
		bool HandleInitializeAllQueuedBattlegroundsCommand(const char* args, WorldSession* m_session);
		bool HandleGetBattlegroundQueueCommand(const char* args, WorldSession* m_session);

		// Instance
		bool HandleCreateInstanceCommand(const char* args, WorldSession* m_session);
		bool HandleResetAllInstancesCommand(const char* args, WorldSession* m_session);
		bool HandleResetInstanceCommand(const char* args, WorldSession* m_session);
		bool HandleShutdownInstanceCommand(const char* args, WorldSession* m_session);
		//bool HandleDeleteInstanceCommand(const char* args, WorldSession *m_session);
		bool HandleGetInstanceInfoCommand(const char* args, WorldSession* m_session);
		bool HandleShowInstancesCommand(const char* args, WorldSession* m_session);

		// Unsorted?
		Player* getSelectedChar(WorldSession* m_session, bool showerror = true);
		Creature* getSelectedCreature(WorldSession* m_session, bool showerror = true);
		bool HandleGOScale(const char* args, WorldSession* m_session);
		bool HandleReviveStringcommand(const char* args, WorldSession* m_session);
		bool HandleMountCommand(const char* args, WorldSession* m_session);
		bool HandleGetPosCommand(const char* args, WorldSession* m_session);
		bool HandleGetTransporterTime(const char* args, WorldSession* m_session);
		bool HandleSendItemPushResult(const char* args, WorldSession* m_session);
		bool HandleGOAnimProgress(const char* args, WorldSession* m_session);
		bool HandleGOFactionCommand( const char *args, WorldSession *session );
		bool HandleGODamageCommand( const char *args, WorldSession *session );
		bool HandleGORebuildCommand( const char *args, WorldSession *session );
		bool HandleGOExport(const char* args, WorldSession* m_session);
		bool HandleRemoveAurasCommand(const char* args, WorldSession* m_session);
		bool HandleParalyzeCommand(const char* args, WorldSession* m_session);
		bool HandleUnParalyzeCommand(const char* args, WorldSession* m_session);
		bool HandleSetMotdCommand(const char* args, WorldSession* m_session);
		bool HandleAddItemSetCommand(const char* args, WorldSession* m_session);
		bool HandleTriggerCommand(const char* args, WorldSession* m_session);
		bool HandleModifyValueCommand(const char* args, WorldSession* m_session);
		bool HandleModifyBitCommand(const char* args, WorldSession* m_session);
		bool HandleBattlegroundExitCommand(const char* args, WorldSession* m_session);
		bool HandleExitInstanceCommand(const char* args, WorldSession* m_session);
		bool HandleCooldownCheatCommand(const char* args, WorldSession* m_session);
		bool HandleCastTimeCheatCommand(const char* args, WorldSession* m_session);
		bool HandlePowerCheatCommand(const char* args, WorldSession* m_session);
		bool HandleGodModeCommand(const char* args, WorldSession* m_session);
		bool HandleShowCheatsCommand(const char* args, WorldSession* m_session);
		bool HandleAuraStackCheatCommand(const char* args, WorldSession* m_session);
		bool HandleItemStackCheatCommand(const char* args, WorldSession* m_session);
		bool HandleTriggerpassCheatCommand(const char* args, WorldSession* m_session);
		bool HandleFlyCommand(const char* args, WorldSession* m_session);
		bool HandleRemoveRessurectionSickessAuraCommand(const char* args, WorldSession* m_session);
		bool HandleDBReloadCommand(const char* args, WorldSession* m_session);

		// Honor
		bool HandleAddHonorCommand(const char* args, WorldSession* m_session);
		bool HandleAddKillCommand(const char* args, WorldSession* m_session);
		bool HandleGlobalHonorDailyMaintenanceCommand(const char* args, WorldSession* m_session);
		bool HandleNextDayCommand(const char* args, WorldSession* m_session);
		bool HandlePVPCreditCommand(const char* args, WorldSession* m_session);

		bool HandleUnlearnCommand(const char* args, WorldSession* m_session);
		bool HandleModifyLevelCommand(const char* args, WorldSession* m_session);

		// Pet
		bool HandleCreatePetCommand(const char* args, WorldSession* m_session);
		bool HandleAddPetSpellCommand(const char* args, WorldSession* m_session);
		bool HandleRemovePetSpellCommand(const char* args, WorldSession* m_session);
		bool HandleRenamePetCommand(const char* args, WorldSession* m_session);
		bool HandleDismissPetCommand(const char* args, WorldSession* m_session);
		bool HandlePetLevelCommand(const char* args, WorldSession* m_session);
#ifdef USE_SPECIFIC_AIAGENTS
		bool HandlePetSpawnAIBot(const char* args, WorldSession* m_session);
#endif

		// Shutdown
		bool HandleShutdownCommand(const char* args, WorldSession* m_session);
		bool HandleShutdownRestartCommand(const char* args, WorldSession* m_session);
		bool HandleCancelShutdownCommand(const char* args, WorldSession* m_session);

		// Whispers
		bool HandleAllowWhispersCommand(const char* args, WorldSession* m_session);
		bool HandleBlockWhispersCommand(const char* args, WorldSession* m_session);

		// Skills
		bool HandleAdvanceAllSkillsCommand(const char* args, WorldSession* m_session);

		// Kill
		bool HandleKillBySessionCommand(const char* args, WorldSession* m_session);
		bool HandleKillByPlayerCommand(const char* args, WorldSession* m_session);
		bool HandleKillByIPCommand(const char* args, WorldSession* m_session);

		// Castall
		bool HandleCastAllCommand(const char* args, WorldSession* m_session);

		// Recall (Teleport)
		bool HandleRecallListCommand(const char* args, WorldSession* m_session);
		bool HandleRecallGoCommand(const char* args, WorldSession* m_session);
		bool HandleRecallAddCommand(const char* args, WorldSession* m_session);
		bool HandleRecallDelCommand(const char* args, WorldSession* m_session);
		bool HandleModPeriodCommand(const char* args, WorldSession* m_session);
		bool HandleGlobalPlaySoundCommand(const char* args, WorldSession* m_session);
		bool HandleRecallPortPlayerCommand(const char* args, WorldSession* m_session);
		bool HandleRecallPortUsCommand(const char* args, WorldSession* m_session);

		// Bans
		bool HandleIPBanCommand(const char* args, WorldSession* m_session);
		bool HandleIPUnBanCommand(const char* args, WorldSession* m_session);
		bool HandleAccountUnbanCommand(const char* args, WorldSession* m_session);

		// Item removal
		bool HandleRemoveItemCommand(const char* args, WorldSession* m_session);

		// Rename
		bool HandleRenameCommand(const char* args, WorldSession* m_session);
		bool HandleForceRenameCommand(const char* args, WorldSession* m_session);

		// Reputation
		bool HandleGetStandingCommand(const char* args, WorldSession* m_session);
		bool HandleSetStandingCommand(const char* args, WorldSession* m_session);

		// Lookups
		bool HandleLookupItemCommand(const char* args, WorldSession* m_session);
		bool HandleLookupCreatureCommand(const char* args, WorldSession* m_session);
		bool HandleLookupObjectCommand(const char* args, WorldSession* m_session);
		bool HandleLookupSpellCommand(const char* args, WorldSession* m_session);
		bool HandleLookupSkillCommand(const char* args, WorldSession* m_session);
		bool HandleLookupFactionCommand(const char* args, WorldSession* m_session);
#ifdef ENABLE_ACHIEVEMENTS
		bool HandleLookupAchievementCmd(const char* args, WorldSession* m_session);
#endif
		// Deprecated GMScript reload.
		//bool HandleReloadScriptsCommand(const char * args, WorldSession * m_session);
		bool HandleNpcPossessCommand(const char* args, WorldSession* m_session);
		bool HandleNpcUnPossessCommand(const char* args, WorldSession* m_session);
		bool HandleRehashCommand(const char* args, WorldSession* m_session);

		/* QUEST COMMANDS */
		bool HandleQuestAddBothCommand(const char* args, WorldSession* m_session);
		bool HandleQuestAddFinishCommand(const char* args, WorldSession* m_session);
		bool HandleQuestAddStartCommand(const char* args, WorldSession* m_session);
		bool HandleQuestDelBothCommand(const char* args, WorldSession* m_session);
		bool HandleQuestDelFinishCommand(const char* args, WorldSession* m_session);
		bool HandleQuestFinisherCommand(const char* args, WorldSession* m_session);
		bool HandleQuestDelStartCommand(const char* args, WorldSession* m_session);
		bool HandleQuestFinishCommand(const char* args, WorldSession* m_session);
		bool HandleQuestFailCommand( const char *args, WorldSession *m_session );
		bool HandleQuestGiverCommand(const char* args, WorldSession* m_session);
		bool HandleQuestItemCommand(const char* args, WorldSession* m_session);
		bool HandleQuestListCommand(const char* args, WorldSession* m_session);
		bool HandleQuestLoadCommand(const char* args, WorldSession* m_session);
		bool HandleQuestLookupCommand(const char* args, WorldSession* m_session);
		bool HandleQuestRemoveCommand(const char* args, WorldSession* m_session);
		bool HandleQuestRewardCommand(const char* args, WorldSession* m_session);
		bool HandleQuestStarterSpawnCommand(const char* args, WorldSession* m_session);
		bool HandleQuestFinisherSpawnCommand(const char* args, WorldSession* m_session);
		bool HandleQuestStartCommand(const char* args, WorldSession* m_session);
		bool HandleQuestStatusCommand(const char* args, WorldSession* m_session);

		/** AI AGENT DEBUG COMMANDS */
		bool HandleAIAgentDebugBegin(const char* args, WorldSession* m_session);
		bool HandleAIAgentDebugContinue(const char* args, WorldSession* m_session);
		bool HandleAIAgentDebugSkip(const char* args, WorldSession* m_session);

		bool HandleArenaCreateTeamCommand(const char* args, WorldSession* m_session);
		bool HandleArenaSetTeamLeaderCommand(const char* args, WorldSession* m_session);
		bool HandleArenaResetAllRatingsCommand(const char* args, WorldSession* m_session);
		bool HandleNpcSelectCommand(const char* args, WorldSession* m_session);
		bool HandleWaypointAddFlyCommand(const char* args, WorldSession* m_session);
		bool HandleWhisperBlockCommand(const char* args, WorldSession* m_session);
		bool HandleDispelAllCommand(const char* args, WorldSession* m_session);
		bool HandleShowItems(const char* args, WorldSession* m_session);
		bool HandleShowSkills(const char* args, WorldSession* m_session);
		bool HandleCollisionTestIndoor(const char* args, WorldSession* m_session);
		bool HandleGetDeathState(const char* args, WorldSession* m_session);
		bool HandleCollisionTestLOS(const char* args, WorldSession* m_session);
		bool HandleRenameAllCharacter(const char* args, WorldSession* m_session);
		bool HandleCollisionGetHeight(const char* args, WorldSession* m_session);
		bool HandleAccountMuteCommand(const char* args, WorldSession* m_session);
		bool HandleAccountUnmuteCommand(const char* args, WorldSession* m_session);
		/* For skill related GM commands */
		SkillNameMgr* SkillNameManager;

		bool HandleFixScaleCommand(const char* args, WorldSession* m_session);
		bool HandleAddTrainerSpellCommand(const char* args, WorldSession* m_session);
#ifdef ENABLE_ACHIEVEMENTS
		bool HandleAchievementCompleteCommand(const char* args, WorldSession* m_session);
		bool HandleAchievementCriteriaCommand(const char* args, WorldSession* m_session);
		bool HandleAchievementResetCommand(const char* args, WorldSession* m_session);
#endif

		//////////////////////////////////// Vehicle commands /////////////////////////////////////////////////
		bool HandleVehicleEjectPassengerCommand( const char *args, WorldSession *session );
		bool HandleVehicleEjectAllPassengersCommand( const char *args, WorldSession *session );
		bool HandleVehicleInstallAccessoriesCommand( const char *args, WorldSession *session );
		bool HandleVehicleRemoveAccessoriesCommand( const char *args, WorldSession *session );
		bool HandleVehicleAddPassengerCommand( const char *args, WorldSession *session );
};


#define sChatHandler ChatHandler::getSingleton()
#endif
