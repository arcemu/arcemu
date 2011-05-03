

#include "LUAEngine.h"

namespace lua_engine
{
	void bindPlayerMethods(luabridge::module & m)
	{
		m	.subclass<Player, Unit>("Player")
			.method(&Player::Gossip_SendPOI, "Gossip_SendPOI", "sendPOI", "sendpoi", NULL)
			.method( &Player::_AdvanceSkillLine, "AdvanceSkillLine", "advanceSkill", "advanceskill", NULL)
			.method( &Player::_AddSkillLine, "AddSkillLine", "addSkill", "addskill", NULL)
			.method( &Player::_GetSkillLineMax, "GetSkillLineMax", "getMaxSkill", "getmaxskill", NULL)
			.method( &Player::_GetSkillLineCurrent, "GetSkillLineCurrent", "getSkill", "getskill", NULL)
			.method( &Player::_RemoveSkillLine, "RemoveSkillLine", "removeSkill", "removeskill", NULL)
			.method( &Player::_UpdateMaxSkillCounts, "UpdateMaxSkillLineCounts", "updateMaxSkillCount", "updatemaxskillcount", NULL)
			.method( &Player::_ModifySkillBonus, "ModifySkillBonus", "modSkillBonus", "modskillbonus", NULL)
			.method( &Player::_ModifySkillBonusByType, "ModifySkillBonusByType", "modSkillBonusByType", "modskillbonusbytype", NULL)
			.method( &Player::_HasSkillLine, "HasSkillLine", "hasSkill", "hasskill", NULL)
			.method( &Player::RemoveSpellsFromLine, "RemoveSpellsFromLine", "removeSkillSpells", "removeskillspells", NULL)
			.method( &Player::_RemoveAllSkills, "RemoveAllSkills", "removeSkills", "removeskills", NULL)
			.method( &Player::_RemoveLanguages, "RemoveLanguages", "removeLanguages", "removelanguages", NULL)
			.method( &Player::_AddLanguages, "AddLanguages", "addLanguages", "addlanguages", NULL)
			.method( &Player::_AdvanceAllSkills, "AdvanceAllSkills", "advanceSkills", "advanceSkills", NULL)
			.method( &Player::_ModifySkillMaximum, "ModifySkillMaximum", "modMaxSkill", "modmaxskill", NULL)
			.method( &Player::_LearnSkillSpells, "LearnSkillSpells", "learnSkillSpells", "learnskillspells", NULL)
			.method(&Player::Cooldown_AddStart, "Cooldown_AddStart", "cooldown_addstart", NULL)
			.method(&Player::Cooldown_Add, "Cooldown_Add", "cooldown_add", NULL)
			.method(&Player::Cooldown_AddItem, "Cooldown_AddItem", "cooldown_additem", NULL)
			.method( (bool (Player::*)(SpellEntry*) )&Player::Cooldown_CanCast, "Cooldown_CanCast", "cooldown_cancast", NULL)

			.method(&Player::GetTaxiPath, "GetTaxiPath", "getTaxiPath", "gettaxipath", NULL)
			.method(&Player::GetTaxiState, "GetTaxiState", "getTaxiState", "gettaxistate", NULL)
			.method(&Player::TaxiStart, "TaxiStart", "startTaxi", "starttaxi",  NULL)
			.method(&Player::SetTaxiPath, "SetTaxiPath", "setTaxiPath", "settaxipath", NULL)
			.method(&Player::SetTaxiPos, "SetTaxiPos", "setTaxiPos", "settaxipos", NULL)
			.method(&Player::SetTaxiState, "SetTaxiState", "setTaxiState", "settaxistate", NULL)

			.method(&Player::HasQuests, "HasQuests", "hasQuests", "hasquests", NULL)
			.method(&Player::GetOpenQuestSlot, "GetOpenQuestSlot", "getOpenQuestSlot", "getopenquestslot", NULL)
			.method(&Player::GetQuestLogForEntry, "GetQuestLogForEntry", "getQuestLogEntry", "getquestlogentry", NULL)
			.method(&Player::GetQuestLogInSlot, "GetQuestLogInSlot", "getQuestLogSlot", "getquestlogslot", NULL)
			.method(&Player::GetQuestSharer, "GetQuestSharer", "getQuestSharer", "getquestsharer", NULL)
			.method(&Player::SetQuestSharer, "SetQuestSharer", "setQuestSharer", "setquestsharer", NULL)
			.method(&Player::SetQuestLogSlot, "SetQuestLogSlot", "setQuestLogSlot", "setquestlogslot", NULL)
			.method(&Player::HasFinishedDaily, "HasFinishedDaily", "hasFinishedDaily", "hasfinisheddaily", NULL)
			.method(&Player::AddToFinishedQuests, "AddToFinishedQuests", "addFinishedQuests", "addfinishedquests", NULL)
			.method(&Player::HasFinishedQuest, "HasFinishedQuest", "hasFinishedQuest", "hasfinishedquest", NULL)
			.method(&Player::GetQuestRewardStatus, "GetQuestRewardStatus", "getQuestRewardStatus", "getquestrewardstatus", NULL)
			.method(&Player::HasQuestForItem, "HasQuestForItem", "hasQuestForItem", "hasquestforitem", NULL)
			.method(&Player::HasQuestSpell, "hasQuestSpell", "HasQuestSpell", "hasquestspell", NULL)
			.method(&Player::RemoveQuestSpell, "RemoveQuestSpell", "removeQuestSpell", "removequestspell", NULL)
			.method(&Player::HasQuest, "HasQuest", "hasQuest", "hasquest", NULL)
			.method(&Player::RemoveQuestMob, "RemoveQuestMob", "removeQuestMob", "removequestmob", NULL)

			.method(&Player::GetTeam, "GetTeam", "getTeam", "getteam", NULL)
			.method(&Player::GetTeamInitial, "GetTeamInitial", "getInitialTeam", "getinitialteam")
			.method(&Player::SetTeam, "SetTeam", "setTeam", "setteam", NULL)
			.method(&Player::ResetTeam, "ResetTeam", "resetTeam", "resetteam", NULL)
			.method(&Player::IsTeamHorde, "IsTeamHorde", "isHordeTeam", "ishordeteam", NULL)
			.method(&Player::IsTeamAlliance, "IsTeamAlliance", "isAllianceTeam", "isallianceteam", NULL)
			.method(&Player::IsInFeralForm, "IsInFeralForm", "isInFeralForm", "isinferalform", NULL)
			
			.method(&Player::GetMaxLevel, "GetMaxLevel", "getMaxLevel", "getmaxlevel", NULL)
			.method(&Player::GetSelection, "GetSelection", "getSelection", "getselection", NULL)
			.method(&Player::GetTarget, "GetTarget", "getTarget", "gettarget", NULL)
			.method(&Player::SetSelection, "SetSelection", "setSelection", "setselection", NULL)
			.method(&Player::SetTarget, "SetTarget", "setTarget", "settarget", NULL)

			.method(&Player::HasSpell, "HasSpell", "hasSpell", "hasspell", NULL)
			.method(&Player::addSpell, "addSpell", "AddSpell", "addspell", NULL)
			.method(&Player::removeSpell, "RemoveSpell", "removeSpell", "removespell" ,NULL)
			.method(&Player::AddShapeShiftSpell, "AddShapeShiftSpell", "addShapeshiftSpell", "addshapeshiftspell", NULL)
			.method(&Player::RemoveShapeShiftSpell, "RemoveShapeShiftSpell", "removeShapeshiftSpell", "removeshapeshiftspell", NULL)

			//faction
			.method(&Player::ModStanding, "ModStanding", "modStanding", "modstanding", NULL)
			.method(&Player::GetStanding, "GetStanding", "getStanding", "getstanding", NULL)
			.method(&Player::GetBaseStanding, "GetBaseStanding", "getBaseStanding", "getbasestanding", NULL)
			.method(&Player::SetStanding, "SetStanding", "setStanding", "setstanding", NULL)
			.method(&Player::SetAtWar, "SetAtWar", "setAtWar", "setatwar", NULL)
			//BIND(IsAtWar)
			.method(&Player::GetStandingRank, "GetStandingRank", "getStandingRank", "getstandingrank", NULL)
			.static_method("GetReputationRankFromStanding", &Player::GetReputationRankFromStanding)
			.method(&Player::SetFactionInactive, "SetFactionInactive", "setFactionInactive", "setfactioninactive", NULL)
			.method(&Player::GetExaltedCount, "GetExaltedCount", "getExaltedCount", "getexaltedcount", NULL)

			.method(&Player::GetPVPRank, "GetPVPRank", "getPVPRank", "getpvprank", NULL)
			.method(&Player::SetPVPRank, "SetPVPRank", "setPVPRank", "setpvprank", NULL)
			.method(&Player::GetMaxPersonalRating, "GetMaxPersonalRating", "getMaxPersonalRating", "getmaxpersonalrating", NULL)
			.method(&Player::HasTitle, "HasTitle", "hasTitle", "hastitle", NULL)
			.method(&Player::SetKnownTitle, "SetKnownTitle", "setKnownTitle", "setknowntitle", NULL)

			.method(&Player::InGroup, "IsInGroup", "isInGroup", "isingroup", NULL)
			.method(&Player::IsGroupLeader, "IsGroupLeader", "isGroupLeader", "isgroupleader", NULL)
			.method(&Player::HasBeenInvited, "HasBeenInvited", "hasBeenInvited", "hasbeeninvited", NULL)
			.method(&Player::SetInviter, "SetInviter", "setInviter", "setinviter", NULL)
			.method(&Player::GetInviter, "GetInviter", "getInviter", "getinviter", NULL)

			.method(&Player::IsGroupMember, "IsGroupMember", "isGroupMember", "isgroupmember" ,NULL)
			.method(&Player::IsBanned, "IsBanned", "isBanned", "isbanned", NULL)
			
			.method( (void (Player::*)() )&Player::SetBanned, "SetBanned", "setBanned", "setbanned", NULL)
			.method( (void (Player::*)(string) )&Player::SetBanned, "SetBannedWithReason", "setBannedWithReason", "setbannedwithreason", NULL)
			//.method("SetBannedWithTimeStamp", (void (Player::*)(uint32,string&))&Player::SetBanned)
			.method(&Player::UnSetBanned, "UnSetBanned", "clearBan", "clearBan", NULL)
			.method(&Player::GetBanReason, "GetBanReason", "getBanReason", "getbanreason", NULL)

			.method(&Player::IsInGuild, "IsInGuild", "isInGuild", "isinguild" ,NULL)
			.method( &Player::GetGuildId, "GetGuildID", "getGuildID", "getguildid", NULL)
			.method( &Player::SetGuildId, "setGuildID", "SetGuildID", "setguildid", NULL)
			.method(&Player::GetGuildRank, "GetGuildRank", "getGuildRank", "getguildrank", NULL)
			.method(&Player::SetGuildRank, "SetGuildRank", "setGuildRank", "setguildrank", NULL)
			.method(&Player::GetGuildInvitersGuid, "GetGuildInvitersGuid", "getGuildInvitersGUID", "getguildinvitersguid", NULL)
			.method(&Player::SetGuildInvitersGuid, "SetGuildInvitersGuid", "setGuildInvitersGUID", "setguildinvitersguid", NULL)
			.method(&Player::UnSetGuildInvitersGuid, "UnSetGuildInvitersGuid", "unsetGuildInvitersGUID", "unsetguildinvitersguid", NULL)

			.method(&Player::RequestDuel, "RequestDuel", "requestDuel", "requestduel", NULL)
			.method(&Player::EndDuel, "EndDuel", "endDuel", "endduel", NULL)
			.method(&Player::DuelCountdown, "DuelCountdown", "duelCountdown", "duelcountdown", NULL)
			.method(&Player::SetDuelStatus, "SetDuelStatus", "setDuelStatus", "setduelstatus", NULL)
			.method(&Player::GetDuelStatus, "GetDuelStatus", "getDuelStatus", "getduelstatus", NULL)
			.method(&Player::SetDuelState, "SetDuelState", "setDuelState", "setduelstate", NULL)
			.method(&Player::GetDuelState, "GetDuelState", "getDuelState", "getduelstate", NULL)
			.method(&Player::GetDuelTeam, "GetDuelTeam", "getDuelTeam", "getduelteam", NULL)
			.method(&Player::SetDuelTeam, "SetDuelTeam", "setDuelTeam", "setduelteam", NULL)

			.method(&Player::SetBindPoint, "SetBindPoint", "setBindPoint", "setbindpoint", NULL)
			.method(&Player::KillPlayer, "KillPlayer", "killPlayer", "killplayer", NULL)
			.method(&Player::ResurrectPlayer, "ResurrectPlayer", "resurrectPlayer", "resurrectplayer", NULL)

			.method(&Player::GetShapeShift, "GetShapeShift", "getShapeshift", "getshapeshift" ,NULL)
			.method(&Player::SetShapeShift, "SetShadeShift", "setShapeshift", "setshapeshift", NULL)
			.method(&Player::CanSee, "CanSee", "canSee", "cansee", NULL)
			.method(&Player::IsVisible, "IsVisible", "isVisible", "isvisible" ,NULL)
			.method(&Player::Reset_Spells, "ResetSpells", "resetSpells", "resetspells", NULL)
			.method(&Player::Reset_Talents, "ResetTalents", "resetTalents", "resettalents", NULL)
			.method(&Player::GetSelectedGo, "GetSelectedGO", "getSelectedGO", "getselectedgo", NULL)
			.method(&Player::IsMounted, "IsMounted", "isMounted", "ismounted", NULL)
			.method(&Player::Dismount, "Dismount", "dismount", NULL)
			.method(&Player::Kick, "Kick", "kick", NULL)
			.method(&Player::SoftDisconnect, "SoftDisconnect", "softDisconnect", "softdisconnect" ,NULL)

			.method(&Player::GetSummonedObject, "GetSummonedObject", "getSummonedObject", "getsummonedobject", NULL)
			.method(&Player::SetSummonedObject, "SetSummonedObject", "setSummonedObject", "setsummonedobject", NULL)
			.method(&Player::ClearCooldownsOnLine, "ClearCooldownLine", "clearCooldownLine", "clearcooldownline", NULL)
			.method(&Player::ResetAllCooldowns, "ResetAllCooldowns", "resetAllCooldowns", "resetallcooldowns", NULL)
			.method(&Player::ClearCooldownForSpell, "ClearCooldownForSpell", "clearCooldownForSpell", "clearcooldownforspell", NULL)

			.method(&Player::Phase, "Phase", "phase", NULL)
			.method(&Player::ExitInstance, "ExitInstance", "exitInstance", "exitinstance", NULL)
			.method(&Player::GetAreaID, "GetAreaID", "getAreaID", "getareaid", NULL)
			.method(&Player::SetAreaID, "SetAreaID", "setAreaID", "setareaid", NULL)
			.method(&Player::EventTeleport, "Teleport", "teleport", NULL)
			.method(&Player::EventTeleportTaxi, "TeleportTaxi", "teleportTaxi", "teleporttaxi", NULL)
			.method( (bool (Player::*)(uint32,uint32,float,float,float,float) )&Player::SafeTeleport, "SafeTeleport", "safeTeleport", "safeteleport", NULL)
			.method(&Player::GetDungeonDifficulty, "GetDungeonDifficulty", "getDungeonDifficulty", "getdungeondifficulty", NULL)
			.method(&Player::GetRaidDifficulty, "GetRaidDifficulty", "getRaidDifficulty", "getraiddifficulty", NULL)

			.method(&Player::GetGold, "GetGold", "getGold", "getgold", NULL)
			.method(&Player::ModGold, "ModGold", "modGold", "modgold", NULL)
			.method(&Player::HasGold, "hasGold", "HasGold", "hasgold", NULL)
			.method(&Player::SetGold, "SetGold", "setGold", "setgold", NULL)
			.method(&Player::GiveGold, "GiveGold", "giveGold", "givegold", NULL)
			.method(&Player::TakeGold, "TakeGold", "takeGold", "takegold", NULL)

			.method(&Player::SetFarsightTarget, "SetFarsightTarget", "setFarsightTarget", "setfarsighttarget", NULL)
			.method(&Player::GetFarsightTarget, "GetFarsightTarget", "getFarsightTarget", "getfarsighttarget", NULL)
			
			.method(&Player::SetXp, "SetXP", "setXP", "setxp" ,NULL)
			.method(&Player::GetXp, "GetXP", "getXP", "getxp", NULL)
			.method(&Player::SetNextLevelXp, "SetNextLevelXP", "setNextLevelXP", "setnextlevelxp", NULL)
			
			.method(&Player::SetTalentPoints, "SetTalentPoints", "setTalentPoints", "settalentpoints", NULL)
			.method(&Player::ModTalentPoints, "ModTalentPoints", "modTalentPoints", "modtalentpoints", NULL)
			.method(&Player::GetTalentPoints, "GetTalentPoints", "getTalentPoints", "gettalentpoints", NULL)

			.method(&Player::SetHonorCurrency, "SetHonorCurrency", "setHonorCurrency", "sethonorcurrency", NULL)
			.method(&Player::GetHonorCurrency, "GetHonorCurrency", "getHonorCurrency", "gethonorcurrency", NULL)
			.method(&Player::ModHonorCurrency, "ModHonorCurrency", "modHonorCurrency", "modhonorcurrency", NULL)

			.method(&Player::GetArenaCurrency, "GetArenaCurrency", "getArenaCurrency", "getarenacurrency", NULL)
			.method(&Player::SetArenaCurrency, "SetArenaCurrency", "setArenaCurrency", "setarenacurrency", NULL)
			.method(&Player::ModArenaCurrency, "ModArenaCurrency", "modArenaCurrency", "modarenacurrency", NULL)

			.method(&Player::SetGlyph, "SetGlyph", "setGlyph", "setglyph", NULL)
			.method(&Player::GetGlyph, "GetGlyph", "getGlyph", "getglyph", NULL)

			.method(&Player::ResetPvPTimer, "ResetPVPTimer", "resetPVPTimer", "resetpvptimer", NULL)
			.method(&Player::StopPvPTimer, "StopPVPTimer", "stopPVPTimer", "stoppvptimer", NULL)
			.method(&Player::PvPToggle, "PVPToggle", "pvpToggle" , "pvptoggle", NULL)

			.method(&Player::LearnTalent, "learnTalent", "LearnTalent", "learntalent", NULL)
			.method(&Player::AddComboPoints, "AddComboPoints", "addComboPoints", "addcombopoints", NULL)

			.method(&Player::GetTradeTarget, "GetTradeTarget", "getTradeTarget", "gettradetarget", NULL)
			.method(&Player::Possess, "Possess", "possess", NULL)
			.method(&Player::UnPossess, "Unpossess", "unpossess", NULL)

			.method(&Player::IsAttacking, "IsAttacking", "isAttacking", "isattacking", NULL)
			.method(&Player::PlaySound, "PlaySound", "playSound", "playsound", NULL)
			.method(&Player::GetItemInterface, "GetItemInterface", "getItemInterface", "getiteminterface", NULL)
			.method( &Player::RemoteRevive, "Resurrect", "resurrect", "ResurrectPlayer", "resurrectPlayer", "resurrectplayer", NULL)
			.method(&Player::GetName, "GetName", "getName", "getname", NULL)

			.method(&Player::GetSession, "GetSession", "getSession", "getsession", NULL)
			.method( &Player::SaveToDB, "SaveToDB", "saveToDB", "savetodb", NULL);

			m	.class_<WorldSession>("WorldSession")
			.method(&WorldSession::GetAccountNameS, "GetAccountName", "getAccountName", "getaccountname", NULL)
			.method( &WorldSession::GetPermissions, "GetPermissions", "getPermissions", "getpermissions", NULL)
			.method( &WorldSession::HasGMPermissions, "IsGM", "isGM", "isgm", NULL)
			.method( &WorldSession::GetMovementInfo, "GetMovementInfo", "getMovementInfo", "getmovementinfo", NULL);

			m	.class_<MovementInfo>("MovementInfo")
				.property_ro("flags", &MovementInfo::flags);

			m	.class_<ChatHandler>("sChatHandler")
				.method(&ChatHandler::FillMessageData, "FillMessageData", "fillMessageData", "fillmessagedata", NULL)
				.method(&ChatHandler::FillSystemMessageData, "FillSystemMessageData", "fillSystemMessageData", "fillsystemmessagedata", NULL);

			luabridge::tdstack<ChatHandler*>::push(m.L, ChatHandler::getSingletonPtr() );
			lua_setglobal(m.L, "sChatHandler");
	}
}