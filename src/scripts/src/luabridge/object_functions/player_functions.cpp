

#include "LUAEngine.h"

namespace lua_engine
{
	void bindPlayerMethods(luabridge::module & m)
	{
		m	.subclass<Player, Unit>("Player")
		.method(&Player::Gossip_SendPOI, "Gossip_SendPOI", "sendPOI", "sendpoi", NULL)
		.method(&Player::_AdvanceSkillLine, "AdvanceSkillLine", "advanceSkill", "advanceskill", NULL)  //wiki listed
		.method(&Player::_AddSkillLine, "AddSkillLine", "addSkill", "addskill", NULL)  //wiki listed
		.method(&Player::_GetSkillLineMax, "GetSkillLineMax", "getMaxSkill", "getmaxskill", NULL) //wiki listed
		.method(&Player::_GetSkillLineCurrent, "GetSkillLineCurrent", "getSkill", "getskill", NULL)  //wiki listed
		.method(&Player::_RemoveSkillLine, "RemoveSkillLine", "removeSkill", "removeskill", NULL)  //wiki listed
		.method(&Player::_UpdateMaxSkillCounts, "UpdateMaxSkillLineCounts", "updateMaxSkillCount", "updatemaxskillcount", NULL)  //wiki listed
		.method(&Player::_ModifySkillBonus, "ModifySkillBonus", "modSkillBonus", "modskillbonus", NULL)  //wiki listed
		.method(&Player::_ModifySkillBonusByType, "ModifySkillBonusByType", "modSkillBonusByType", "modskillbonusbytype", NULL)  //wiki listed
		.method(&Player::_HasSkillLine, "HasSkillLine", "hasSkill", "hasskill", NULL)  //wiki listed
		.method(&Player::RemoveSpellsFromLine, "RemoveSpellsFromLine", "removeSkillSpells", "removeskillspells", NULL)  //wiki listed
		.method(&Player::_RemoveAllSkills, "RemoveAllSkills", "removeSkills", "removeskills", NULL)  //wiki listed
		.method(&Player::_RemoveLanguages, "RemoveLanguages", "removeLanguages", "removelanguages", NULL)  //wiki listed
		.method(&Player::_AddLanguages, "AddLanguages", "addLanguages", "addlanguages", NULL)  //wiki listed
		.method(&Player::_AdvanceAllSkills, "AdvanceAllSkills", "advanceSkills", "advanceSkills", NULL)  //wiki listed
		.method(&Player::_ModifySkillMaximum, "ModifySkillMaximum", "modMaxSkill", "modmaxskill", NULL)  //wiki listed
		.method(&Player::_LearnSkillSpells, "LearnSkillSpells", "learnSkillSpells", "learnskillspells", NULL)  //wiki listed
		.method(&Player::Cooldown_AddStart, "Cooldown_AddStart", "cooldown_addstart", NULL)
		.method(&Player::Cooldown_Add, "Cooldown_Add", "cooldown_add", NULL)
		.method(&Player::Cooldown_AddItem, "Cooldown_AddItem", "cooldown_additem", NULL)
		.method((bool (Player::*)(SpellEntry*))&Player::Cooldown_CanCast, "Cooldown_CanCast", "cooldown_cancast", NULL)

		.method(&Player::GetTaxiPath, "GetTaxiPath", "getTaxiPath", "gettaxipath", NULL) //wiki listed
		.method(&Player::GetTaxiState, "GetTaxiState", "getTaxiState", "gettaxistate", NULL) //wiki listed
		.method(&Player::TaxiStart, "TaxiStart", "startTaxi", "starttaxi",  NULL) //wiki listed
		.method(&Player::SetTaxiPath, "SetTaxiPath", "setTaxiPath", "settaxipath", NULL) //wiki listed
		.method(&Player::SetTaxiPos, "SetTaxiPos", "setTaxiPos", "settaxipos", NULL)
		.method(&Player::SetTaxiState, "SetTaxiState", "setTaxiState", "settaxistate", NULL) //wiki listed

		.method(&Player::HasQuests, "HasQuests", "hasQuests", "hasquests", NULL) //wiki listed
		.method(&Player::GetOpenQuestSlot, "GetOpenQuestSlot", "getOpenQuestSlot", "getopenquestslot", NULL) //wiki listed
		.method(&Player::GetQuestLogForEntry, "GetQuestLogForEntry", "getQuestLogEntry", "getquestlogentry", NULL) //wiki listed
		.method(&Player::GetQuestLogInSlot, "GetQuestLogInSlot", "getQuestLogSlot", "getquestlogslot", NULL) //wiki listed
		.method(&Player::GetQuestSharer, "GetQuestSharer", "getQuestSharer", "getquestsharer", NULL) //wiki listed
		.method(&Player::SetQuestSharer, "SetQuestSharer", "setQuestSharer", "setquestsharer", NULL) //wiki listed
		.method(&Player::SetQuestLogSlot, "SetQuestLogSlot", "setQuestLogSlot", "setquestlogslot", NULL) //wiki listed
		.method(&Player::HasFinishedDaily, "HasFinishedDaily", "hasFinishedDaily", "hasfinisheddaily", NULL) //wiki listed
		.method(&Player::AddToFinishedQuests, "AddToFinishedQuests", "addFinishedQuests", "addfinishedquests", NULL) //wiki listed
		.method(&Player::HasFinishedQuest, "HasFinishedQuest", "hasFinishedQuest", "hasfinishedquest", NULL) //wiki listed
		.method(&Player::GetQuestRewardStatus, "GetQuestRewardStatus", "getQuestRewardStatus", "getquestrewardstatus", NULL) //wiki listed
		.method(&Player::HasQuestForItem, "HasQuestForItem", "hasQuestForItem", "hasquestforitem", NULL) //wiki listed
		.method(&Player::HasQuestSpell, "hasQuestSpell", "HasQuestSpell", "hasquestspell", NULL) //wiki listed
		.method(&Player::RemoveQuestSpell, "RemoveQuestSpell", "removeQuestSpell", "removequestspell", NULL) //wiki listed
		.method(&Player::HasQuest, "HasQuest", "hasQuest", "hasquest", NULL) //wiki listed
		.method(&Player::RemoveQuestMob, "RemoveQuestMob", "removeQuestMob", "removequestmob", NULL) //wiki listed

		.method(&Player::GetTeam, "GetTeam", "getTeam", "getteam", NULL) //wiki listed
		.method(&Player::GetTeamInitial, "GetTeamInitial", "getInitialTeam", "getinitialteam") //wiki listed
		.method(&Player::SetTeam, "SetTeam", "setTeam", "setteam", NULL) //wiki listed
		.method(&Player::ResetTeam, "ResetTeam", "resetTeam", "resetteam", NULL) //wiki listed
		.method(&Player::IsTeamHorde, "IsTeamHorde", "isHordeTeam", "ishordeteam", NULL) //wiki listed
		.method(&Player::IsTeamAlliance, "IsTeamAlliance", "isAllianceTeam", "isallianceteam", NULL) //wiki listed
		.method(&Player::IsInFeralForm, "IsInFeralForm", "isInFeralForm", "isinferalform", NULL)

		.method(&Player::GetMaxLevel, "GetMaxLevel", "getMaxLevel", "getmaxlevel", NULL)
		.method(&Player::GetSelection, "GetSelection", "getSelection", "getselection", NULL)//wiki listed
		.method(&Player::GetTarget, "GetTarget", "getTarget", "gettarget", NULL)//wiki listed
		.method(&Player::SetSelection, "SetSelection", "setSelection", "setselection", NULL) //wiki listed
		.method(&Player::SetTarget, "SetTarget", "setTarget", "settarget", NULL) //wiki listed

		.method(&Player::HasSpell, "HasSpell", "hasSpell", "hasspell", NULL) //wiki listed
		.method(&Player::addSpell, "addSpell", "AddSpell", "addspell", NULL)//wiki listed
		.method(&Player::removeSpell, "RemoveSpell", "removeSpell", "removespell" , NULL)
		.method(&Player::AddShapeShiftSpell, "AddShapeShiftSpell", "addShapeshiftSpell", "addshapeshiftspell", NULL) //wiki listed
		.method(&Player::RemoveShapeShiftSpell, "RemoveShapeShiftSpell", "removeShapeshiftSpell", "removeshapeshiftspell", NULL) //wiki listed

		//faction
		.method(&Player::ModStanding, "ModStanding", "modStanding", "modstanding", NULL) //wiki listed
		.method(&Player::GetStanding, "GetStanding", "getStanding", "getstanding", NULL) //wiki listed
		.method(&Player::GetBaseStanding, "GetBaseStanding", "getBaseStanding", "getbasestanding", NULL) //wiki listed
		.method(&Player::SetStanding, "SetStanding", "setStanding", "setstanding", NULL) //wiki listed
		.method(&Player::SetAtWar, "SetAtWar", "setAtWar", "setatwar", NULL) //wiki listed
		//BIND(IsAtWar)
		.method(&Player::GetStandingRank, "GetStandingRank", "getStandingRank", "getstandingrank", NULL) //wiki listed
		.static_method("GetReputationRankFromStanding", &Player::GetReputationRankFromStanding)
		.method(&Player::SetFactionInactive, "SetFactionInactive", "setFactionInactive", "setfactioninactive", NULL)
		.method(&Player::GetExaltedCount, "GetExaltedCount", "getExaltedCount", "getexaltedcount", NULL) //wiki listed

		.method(&Player::GetPVPRank, "GetPVPRank", "getPVPRank", "getpvprank", NULL) //wiki listed
		.method(&Player::SetPVPRank, "SetPVPRank", "setPVPRank", "setpvprank", NULL) //wiki listed
		.method(&Player::GetMaxPersonalRating, "GetMaxPersonalRating", "getMaxPersonalRating", "getmaxpersonalrating", NULL) //wiki listed
		.method(&Player::HasTitle, "HasTitle", "hasTitle", "hastitle", NULL) //wiki listed
		.method(&Player::SetKnownTitle, "SetKnownTitle", "setKnownTitle", "setknowntitle", NULL) //wiki listed

		.method(&Player::InGroup, "IsInGroup", "isInGroup", "isingroup", NULL)//wiki listed
		.method(&Player::IsGroupLeader, "IsGroupLeader", "isGroupLeader", "isgroupleader", NULL) //wiki listed
		.method(&Player::HasBeenInvited, "HasBeenInvited", "hasBeenInvited", "hasbeeninvited", NULL)
		.method(&Player::SetInviter, "SetInviter", "setInviter", "setinviter", NULL)
		.method(&Player::GetInviter, "GetInviter", "getInviter", "getinviter", NULL)

		.method(&Player::IsGroupMember, "IsGroupMember", "isGroupMember", "isgroupmember" , NULL) //wiki listed
		.method(&Player::IsBanned, "IsBanned", "isBanned", "isbanned", NULL) //wiki listed

		.method((void (Player::*)())&Player::SetBanned, "SetBanned", "setBanned", "setbanned", NULL)   //wiki listed
		.method((void (Player::*)(string))&Player::SetBanned, "SetBannedWithReason", "setBannedWithReason", "setbannedwithreason", NULL)   //wiki listed
		//.method("SetBannedWithTimeStamp", (void (Player::*)(uint32,string&))&Player::SetBanned)
		.method(&Player::UnSetBanned, "UnSetBanned", "clearBan", "clearBan", NULL) //wiki listed
		.method(&Player::GetBanReason, "GetBanReason", "getBanReason", "getbanreason", NULL) //wiki listed

		.method(&Player::IsInGuild, "IsInGuild", "isInGuild", "isinguild" , NULL) //wiki listed
		.method(&Player::GetGuildId, "GetGuildID", "getGuildID", "getguildid", NULL) //wiki listed
		.method(&Player::GetGuild, "GetGuild", "getGuild", "getguild", NULL)
		.method(&Player::SetGuildId, "setGuildID", "SetGuildID", "setguildid", NULL) //wiki listed
		.method(&Player::GetGuildRank, "GetGuildRank", "getGuildRank", "getguildrank", NULL) //wiki listed
		.method(&Player::SetGuildRank, "SetGuildRank", "setGuildRank", "setguildrank", NULL) //wiki listed
		.method(&Player::GetGuildInvitersGuid, "GetGuildInvitersGuid", "getGuildInvitersGUID", "getguildinvitersguid", NULL)
		.method(&Player::SetGuildInvitersGuid, "SetGuildInvitersGuid", "setGuildInvitersGUID", "setguildinvitersguid", NULL)
		.method(&Player::UnSetGuildInvitersGuid, "UnSetGuildInvitersGuid", "unsetGuildInvitersGUID", "unsetguildinvitersguid", NULL)

		.method(&Player::RequestDuel, "RequestDuel", "requestDuel", "requestduel", NULL) //wiki listed
		.method(&Player::EndDuel, "EndDuel", "endDuel", "endduel", NULL) //wiki listed
		.method(&Player::DuelCountdown, "DuelCountdown", "duelCountdown", "duelcountdown", NULL) //wiki listed
		.method(&Player::SetDuelStatus, "SetDuelStatus", "setDuelStatus", "setduelstatus", NULL) //wiki listed
		.method(&Player::GetDuelStatus, "GetDuelStatus", "getDuelStatus", "getduelstatus", NULL)//wiki listed
		.method(&Player::SetDuelState, "SetDuelState", "setDuelState", "setduelstate", NULL) //wiki listed
		.method(&Player::GetDuelState, "GetDuelState", "getDuelState", "getduelstate", NULL) //wiki listed
		.method(&Player::GetDuelTeam, "GetDuelTeam", "getDuelTeam", "getduelteam", NULL) //wiki listed
		.method(&Player::SetDuelTeam, "SetDuelTeam", "setDuelTeam", "setduelteam", NULL) //wiki listed

		.method(&Player::SetBindPoint, "SetBindPoint", "setBindPoint", "setbindpoint", NULL) //wiki listed
		.method(&Player::KillPlayer, "KillPlayer", "killPlayer", "killplayer", NULL) //wiki listed
		.method(&Player::ResurrectPlayer, "ResurrectPlayer", "resurrectPlayer", "resurrectplayer", NULL) //wiki listed

		.method(&Player::GetShapeShift, "GetShapeShift", "getShapeshift", "getshapeshift" , NULL) //wiki listed
		.method(&Player::SetShapeShift, "SetShadeShift", "setShapeshift", "setshapeshift", NULL) //wiki listed
		.method(&Player::CanSee, "CanSee", "canSee", "cansee", NULL) //wiki listed
		.method(&Player::IsVisible, "IsVisible", "isVisible", "isvisible" , NULL) //wiki listed
		.method(&Player::Reset_Spells, "ResetSpells", "resetSpells", "resetspells", NULL)  //wiki listed
		.method(&Player::Reset_Talents, "ResetTalents", "resetTalents", "resettalents", NULL) //wiki listed
		.method(&Player::GetSelectedGo, "GetSelectedGO", "getSelectedGO", "getselectedgo", NULL) //wiki listed
		.method(&Player::IsMounted, "IsMounted", "isMounted", "ismounted", NULL) //wiki listed
		.method(&Player::Dismount, "Dismount", "dismount", NULL) //wiki listed
		.method(&Player::Kick, "Kick", "kick", NULL) //wiki listed
		.method(&Player::SoftDisconnect, "SoftDisconnect", "softDisconnect", "softdisconnect" , NULL) //wiki listed

		.method(&Player::GetSummonedObject, "GetSummonedObject", "getSummonedObject", "getsummonedobject", NULL)
		.method(&Player::SetSummonedObject, "SetSummonedObject", "setSummonedObject", "setsummonedobject", NULL)
		.method(&Player::ClearCooldownsOnLine, "ClearCooldownLine", "clearCooldownLine", "clearcooldownline", NULL)
		.method(&Player::ResetAllCooldowns, "ResetAllCooldowns", "resetAllCooldowns", "resetallcooldowns", NULL) //wiki listed
		.method(&Player::ClearCooldownForSpell, "ClearCooldownForSpell", "clearCooldownForSpell", "clearcooldownforspell", NULL) //wiki listed

		.method(&Player::Phase, "Phase", "phase", NULL) //redundant
		.method(&Player::ExitInstance, "ExitInstance", "exitInstance", "exitinstance", NULL)
		.method(&Player::GetAreaID, "GetAreaID", "getAreaID", "getareaid", NULL) //redundant
		.method(&Player::SetAreaID, "SetAreaID", "setAreaID", "setareaid", NULL) //redundant
		.method(&Player::EventTeleport, "Teleport", "teleport", NULL) //wiki listed
		.method(&Player::EventTeleportTaxi, "TeleportTaxi", "teleportTaxi", "teleporttaxi", NULL)
		.method((bool (Player::*)(uint32, uint32, float, float, float, float))&Player::SafeTeleport, "SafeTeleport", "safeTeleport", "safeteleport", NULL) //wiki listed
		.method(&Player::GetDungeonDifficulty, "GetDungeonDifficulty", "getDungeonDifficulty", "getdungeondifficulty", NULL) //wiki listed
		.method(&Player::GetRaidDifficulty, "GetRaidDifficulty", "getRaidDifficulty", "getraiddifficulty", NULL) //wiki listed

		.method(&Player::GetGold, "GetGold", "getGold", "getgold", NULL) //wiki listed
		.method(&Player::ModGold, "ModGold", "modGold", "modgold", NULL) //wiki listed
		.method(&Player::HasGold, "hasGold", "HasGold", "hasgold", NULL) //wiki listed
		.method(&Player::SetGold, "SetGold", "setGold", "setgold", NULL) //wiki listed
		.method(&Player::GiveGold, "GiveGold", "giveGold", "givegold", NULL)//wiki listed
		.method(&Player::TakeGold, "TakeGold", "takeGold", "takegold", NULL) //wiki listed

		.method(&Player::SetFarsightTarget, "SetFarsightTarget", "setFarsightTarget", "setfarsighttarget", NULL) //wiki listed
		.method(&Player::GetFarsightTarget, "GetFarsightTarget", "getFarsightTarget", "getfarsighttarget", NULL) //wiki listed

		.method(&Player::SetXp, "SetXP", "setXP", "setxp" , NULL) //wiki listed
		.method(&Player::GetXp, "GetXP", "getXP", "getxp", NULL) //wiki listed
		.method(&Player::SetNextLevelXp, "SetNextLevelXP", "setNextLevelXP", "setnextlevelxp", NULL) //wiki listed

		.method(&Player::SetCurrentTalentPoints, "SetCurrentTalentPoints", "setCurrentTalentPoints", "setcurrenttalentpoints", NULL)  //wiki listed
		.method(&Player::GetCurrentTalentPoints, "GetCurrentTalentPoints", "getCurrentTalentPoints", "getcurrenttalentpoints", NULL) //wiki listed

		.method(&Player::SetHonorCurrency, "SetHonorCurrency", "setHonorCurrency", "sethonorcurrency", NULL) //wiki listed
		.method(&Player::GetHonorCurrency, "GetHonorCurrency", "getHonorCurrency", "gethonorcurrency", NULL) //wiki listed
		.method(&Player::ModHonorCurrency, "ModHonorCurrency", "modHonorCurrency", "modhonorcurrency", NULL) //wiki listed

		.method(&Player::GetArenaCurrency, "GetArenaCurrency", "getArenaCurrency", "getarenacurrency", NULL)//wiki listed
		.method(&Player::SetArenaCurrency, "SetArenaCurrency", "setArenaCurrency", "setarenacurrency", NULL) //wiki listed
		.method(&Player::ModArenaCurrency, "ModArenaCurrency", "modArenaCurrency", "modarenacurrency", NULL)//wiki listed

		.method(&Player::SetGlyph, "SetGlyph", "setGlyph", "setglyph", NULL) //wiki listed
		.method(&Player::GetGlyph, "GetGlyph", "getGlyph", "getglyph", NULL) //wiki listed

		.method(&Player::ResetPvPTimer, "ResetPVPTimer", "resetPVPTimer", "resetpvptimer", NULL)//wiki listed
		.method(&Player::StopPvPTimer, "StopPVPTimer", "stopPVPTimer", "stoppvptimer", NULL) //wiki listed
		.method(&Player::PvPToggle, "PVPToggle", "pvpToggle" , "pvptoggle", NULL) //wiki listed

		.method(&Player::LearnTalent, "learnTalent", "LearnTalent", "learntalent", NULL) //too complex
		.method(&Player::AddComboPoints, "AddComboPoints", "addComboPoints", "addcombopoints", NULL) //wiki listed

		.method(&Player::GetTradeTarget, "GetTradeTarget", "getTradeTarget", "gettradetarget", NULL) //unused?
		.method(&Player::Possess, "Possess", "possess", NULL) //wiki listed
		.method(&Player::UnPossess, "Unpossess", "unpossess", NULL) //wiki listed

		.method(&Player::IsAttacking, "IsAttacking", "isAttacking", "isattacking", NULL) //redundant
		.method(&Player::PlaySound, "PlaySound", "playSound", "playsound", NULL) //redundant
		.method(&Player::GetItemInterface, "GetItemInterface", "getItemInterface", "getiteminterface", NULL) //wiki listed
		.method(&Player::RemoteRevive, "Resurrect", "resurrect", NULL)
		.method(&Player::GetName, "GetName", "getName", "getname", NULL) //wiki listed

		.method(&Player::GetSession, "GetSession", "getSession", "getsession", NULL)  //wiki listed
		.method(&Player::SaveToDB, "SaveToDB", "saveToDB", "savetodb", NULL)
		.method(&Player::RecalculateHonor, "RecalculateHonor", "recalculateHonor", "recalculatehonor", NULL)
		.property_rw("m_arenaPoints", &Player::m_arenaPoints)
		.method(&Player::getPlayerInfo, "GetPlayerInfo", "getPlayerInfo", "getplayerinfo", NULL)
		.method(&Player::GetGroup, "GetGroup", "getGroup", "getgroup", NULL)
		.property_rw("m_killsLifetime", &Player::m_killsLifetime)
		.method(&Player::GetQuestLogForEntry, "GetQuestLogForEntry", "getQuestLogForEntry", "getquestlogforentry", NULL)
		.method(&Player::GetQuestLogInSlot, "GetQuestLogInSlot", "getQuestLogInSlot", "getquestloginslot", NULL);

		m	.class_<WorldSession>("WorldSession")
		.method(&WorldSession::GetAccountNameS, "GetAccountName", "getAccountName", "getaccountname", NULL)
		.method(&WorldSession::GetPermissions, "GetPermissions", "getPermissions", "getpermissions", NULL)
		.method(&WorldSession::HasGMPermissions, "IsGM", "isGM", "isgm", NULL)
		.method((const MovementInfo * (WorldSession::*)() const)&WorldSession::GetMovementInfo, "GetMovementInfo", "getMovementInfo", "getmovementinfo", NULL);

		m	.class_<MovementInfo>("MovementInfo")
		.property_ro("flags", &MovementInfo::flags);

		m	.class_<ChatHandler>("sChatHandler")
		.method(&ChatHandler::FillMessageData, "FillMessageData", "fillMessageData", "fillmessagedata", NULL)
		.method(&ChatHandler::FillSystemMessageData, "FillSystemMessageData", "fillSystemMessageData", "fillsystemmessagedata", NULL);

		luabridge::tdstack<ChatHandler*>::push(m.L, ChatHandler::getSingletonPtr());
		lua_setglobal(m.L, "sChatHandler");

		m	.class_<PlayerInfo>("PlayerInfo")
		.property_ro("guid", &PlayerInfo::guid)
		.property_ro("acct", &PlayerInfo::acct)
		.property_ro("name", &PlayerInfo::name)
		.property_ro("race", &PlayerInfo::race)
		.property_ro("gender", &PlayerInfo::gender)
		.property_ro("class", &PlayerInfo::cl)
		.property_ro("team", &PlayerInfo::team)
		//.property_ro("lastOnline", &PlayerInfo::lastOnline) time_t will be uint64 :S
		.property_ro("lastZone", &PlayerInfo::lastZone)
		.property_ro("lastLevel", &PlayerInfo::lastLevel)
		.property_ro("loggedInPlayer", &PlayerInfo::m_loggedInPlayer)
		.property_ro("group", &PlayerInfo::m_Group)
		.property_ro("subGroup", &PlayerInfo::subGroup)
		.property_ro("guild", &PlayerInfo::guild);

		m	.class_<Group>("Group")
		.method(&Group::AddMember, "AddMember", "addMember", "addmember", NULL)
		.method(&Group::RemovePlayer, "RemovePlayer", "removePlayer", "removeplayer", NULL)
		.method((bool (Group::*)(Player*))&Group::HasMember, "HasMember", "hasMember", "hasmember", NULL)
		.method((bool (Group::*)(PlayerInfo*))&Group::HasMember, "HasMemberByInfo", "hasMemberByInfo", "hasmemberbyinfo", NULL)
		.method(&Group::SetLeader, "SetLeader", "setLeader", "setleader", NULL)
		.method(&Group::SetLooter, "SetLooter", "setLooter", "setlooter", NULL)
		.method(&Group::SendPacketToAll, "SendPacketToAll", "sendPacketToAll", "sendpackettoall", NULL)
		.method(&Group::SendPacketToAllButOne, "SendPacketToAllButOne", "sendPacketToAllButOne", "sendpackettoallbutone", NULL)
		.method(&Group::GetLeader, "GetLeader", "getLeader", "getleader", NULL)
		.method(&Group::GetLooter, "GetLooter", "getLooter", "getlooter", NULL)
		.method(&Group::ExpandToRaid, "ExpandToRaid", "expandToRaid", "expandtoraid", NULL)
		.method(&Group::GetGroupType, "GetGroupType", "getGroupType", "getgrouptype", NULL)
		.method(&Group::GetID, "GetID", "getID", "getid", NULL)
		.method(&Group::SetDungeonDifficulty, "SetDungeonDifficulty", "setDungeonDifficulty", "setdungeondifficulty", NULL)
		.method(&Group::SetRaidDifficulty, "SetRaidDifficulty", "setRaidDifficulty", "setraiddifficulty", NULL);

		m	.class_<Guild>("Guild")
		.property_ro("creationDay", &Guild::creationDay)
		.property_ro("creationMonth", &Guild::creationMonth)
		.property_ro("creationYear", &Guild::creationYear)
		.method(&Guild::AddGuildMember, "AddGuildMember", "addGuildMember", "addguildmember", NULL)
		.method(&Guild::SetMOTD, "SetMOTD", "setMOTD", "setmotd", NULL)
		.method(&Guild::GetMOTD, "GetMOTD", "getMOTD", "getmotd", NULL)
		.method(&Guild::SetGuildInformation, "SetGuildInformation", "setGuildInformation", "setguildinformation", NULL)
		.method(&Guild::GetGuildInformation, "GetGuildInformation", "getGuildInformation", "getguildinformation", NULL)
		.method(&Guild::SendGuildRoster, "SendGuildRoster", "sendGuildRoster", "sendguildroster", NULL)
		.method(&Guild::RemoveGuildMember, "RemoveGuildMember", "removeGuildMember", "removeguildmember", NULL)
		.method(&Guild::PromoteGuildMember, "PromoteGuildMember", "promoteGuildMember", "promoteguildmember", NULL)
		.method(&Guild::DemoteGuildMember, "DemoteGuildMember", "demoteGuildMember", "demoteguildmember", NULL)
		.method(&Guild::ChangeGuildMaster, "ChangeGuildMaster", "changeGuildMaster", "changeguildmaster", NULL)
		.method(&Guild::SendPacket, "SendPacket", "sendPacket", "sendpacket", NULL)
		.method(&Guild::GuildChat, "GuildChat", "guildChat", "guildchat", NULL)
		.method(&Guild::OfficerChat, "OfficerChat", "officerChat", "officerchat", NULL)
		.method(&Guild::SendGuildLog, "SendGuildLog", "sendGuildLog", "sendguildlog", NULL)
		.method(&Guild::SendGuildBankLog, "SendGuildBankLog", "sendGuildBankLog", "sendguildbanklog", NULL)
		.method(&Guild::SetPublicNote, "SetPublicNote", "setPublicNote", "setpublicnote", NULL)
		.method(&Guild::SetOfficerNote, "SetOfficerNote", "setOfficerNote", "setofficernote", NULL)
		.method(&Guild::Disband, "Disband", "disband", "disband", NULL)
		.method(&Guild::GetGuildName, "GetGuildName", "getGuildName", "getguildname", NULL)
		.method(&Guild::GetGuildLeader, "GetGuildLeader", "getGuildLeader", "getguildleader", NULL)
		.method(&Guild::GetGuildId, "GetGuildId", "getGuildId", "getguildid", NULL)
		.method(&Guild::GetBankTabCount, "GetBankTabCount", "getBankTabCount", "getbanktabcount", NULL)
		.method(&Guild::GetBankBalance, "GetBankBalance", "getBankBalance", "getbankbalance", NULL)
		.method(&Guild::GetNumMembers, "GetNumMembers", "getNumMembers", "getnummembers", NULL)
		.method(&Guild::DepositMoney, "DepositMoney", "depositMoney", "depositmoney", NULL)
		.method(&Guild::WithdrawMoney, "WithdrawMoney", "withdrawMoney", "withdrawmoney", NULL)
		.method(&Guild::SpendMoney, "SpendMoney", "spendMoney", "spendmoney", NULL)
		.method(&Guild::SendGuildInfo, "SendGuildInfo", "sendGuildInfo", "sendguildinfo", NULL)
		.method(&Guild::SetTabardInfo, "SetTabardInfo", "setTabardInfo", "settabardinfo", NULL)
		.method(&Guild::SendGuildBank, "SendGuildBank", "sendGuildBank", "sendguildbank", NULL)
		.method(&Guild::SendGuildBankInfo, "SendGuildBankInfo", "sendGuildBankInfo", "sendguildbankinfo", NULL)
		.method(&Guild::GetNumMembers, "GetNumMembers", "getNumMembers", "getnummembers", NULL)
		.static_method(&Guild::SendGuildCommandResult, "SendGuildCommandResult", "sendGuildCommandResult", "sendguildcommandresult", NULL);

	}
}