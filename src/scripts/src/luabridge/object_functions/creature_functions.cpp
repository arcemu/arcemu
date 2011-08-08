
#include "LUAEngine.h"
#include "lua_creature.h"


LuaCreature::LuaCreature(Creature* creature) : CreatureAIScript(creature), m_binding(NULL) {}
LuaCreature::~LuaCreature() {}

void LuaCreature::OnCombatStart(Unit* mTarget)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ENTER_COMBAT]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_ENTER_COMBAT);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);


	RELEASE_LOCK
}

void LuaCreature::OnCombatStop(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LEAVE_COMBAT]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_LEAVE_COMBAT);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnTargetDied(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DIED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_TARGET_DIED);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnDied(Unit* mKiller)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DIED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_DIED);
	push_unit(mKiller);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnTargetParried(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_PARRIED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_TARGET_PARRIED);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnTargetDodged(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DODGED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_TARGET_DODGED);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnTargetBlocked(Unit* mTarget, int32 iAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_BLOCKED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_TARGET_BLOCKED);
	push_unit(mTarget);
	push_int(iAmount);
	lua_engine::ExecuteLuaFunction(4);

	RELEASE_LOCK
}

void LuaCreature::OnTargetCritHit(Unit* mTarget, int32 fAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_CRIT_HIT]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_TARGET_CRIT_HIT);
	push_unit(mTarget);
	push_int(fAmount);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaCreature:: OnParried(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_PARRY]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_PARRY);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnDodged(Unit* mTarget)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DODGED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_DODGED);
	push_unit(mTarget);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaCreature::OnBlocked(Unit* mTarget, int32 iAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_BLOCKED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_BLOCKED);
	push_unit(mTarget);
	push_int(iAmount);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaCreature::OnCritHit(Unit* mTarget, int32 fAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CRIT_HIT]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_CRIT_HIT);
	push_unit(mTarget);
	push_int(fAmount);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaCreature::OnHit(Unit* mTarget, float fAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_HIT]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_HIT);
	push_unit(mTarget);
	push_float(fAmount);
	lua_engine::ExecuteLuaFunction(4);

	RELEASE_LOCK
}

void LuaCreature::OnAssistTargetDied(Unit* mAssistTarget)
{

	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ASSIST_TARGET_DIED]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_ASSIST_TARGET_DIED);
	push_unit(mAssistTarget);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnFear(Unit* mFeared, uint32 iSpellId)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FEAR]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_FEAR);
	push_unit(mFeared);
	push_int(iSpellId);
	lua_engine::ExecuteLuaFunction(4);

	RELEASE_LOCK
}

void LuaCreature::OnFlee(Unit* mFlee)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FLEE]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_FLEE);
	push_unit(mFlee);
	lua_engine::ExecuteLuaFunction(3);

	RELEASE_LOCK
}

void LuaCreature::OnCallForHelp()
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CALL_FOR_HELP]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_CALL_FOR_HELP);
	lua_engine::ExecuteLuaFunction(2);

	RELEASE_LOCK
}

void LuaCreature::OnLoad()
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOAD]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_LOAD);
	lua_engine::ExecuteLuaFunction(2);

	RELEASE_LOCK
}

void LuaCreature::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_REACH_WP]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_REACH_WP);
	push_int(iWaypointId);
	push_bool(bForwards);
	lua_engine::ExecuteLuaFunction(4);

	RELEASE_LOCK
}

void LuaCreature::OnLootTaken(Player* pPlayer, ItemPrototype* pItemPrototype)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOOT_TAKEN]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_LOOT_TAKEN);
	push_player(pPlayer);
	push_int(pItemPrototype->ItemId);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaCreature::AIUpdate()
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_AIUPDATE]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_AIUPDATE);
	lua_engine::ExecuteLuaFunction(2);

	RELEASE_LOCK
}

void LuaCreature::OnEmote(Player* pPlayer, EmoteType Emote)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_EMOTE]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_EMOTE);
	push_player(pPlayer);
	push_int((int)Emote);
	lua_engine::ExecuteLuaFunction(4);

	RELEASE_LOCK
}

void LuaCreature::OnDamageTaken(Unit* mAttacker, uint32 fAmount)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DAMAGE_TAKEN]);
	push_creature(_unit);
	push_int(CREATURE_EVENT_ON_DAMAGE_TAKEN);
	push_unit(mAttacker);
	push_int(fAmount);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaCreature::Destroy()
{
	sEventMgr.RemoveEvents(_unit, EVENT_LUA_CREATURE_EVENTS);
	PLUA_INSTANCE li_ = lua_instance.get();
	{
		std::pair<li::CreatureInterfaceMap::iterator, li::CreatureInterfaceMap::iterator> interfaces = li_->m_creatureInterfaceMap.equal_range(_unit->GetEntry());
		li::CreatureInterfaceMap::iterator it;
		for(; interfaces.first != interfaces.second;)
		{
			it = interfaces.first++;
			if(it->second != NULL && it->second == this)
				li_->m_creatureInterfaceMap.erase(it);
		}
	}
	{
		//Function Ref clean up
		std::pair<li::ObjectFRefMap::iterator, li::ObjectFRefMap::iterator> frefs = li_->m_creatureFRefs.equal_range(_unit->GetLowGUID());
		for(; frefs.first != frefs.second; ++frefs.first)
			cleanup_varparam(frefs.first->second, li_->lu);

		li_->m_creatureFRefs.erase(_unit->GetLowGUID());
	}
	delete this;
}

namespace lua_engine
{
	CreatureAIScript* createluacreature(Creature* src)
	{
		LuaCreature* script = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		if(src != NULL)
		{
			uint32 id = src->GetEntry();
			li::ObjectBindingMap::iterator itr = ref->m_unitBinding.find(id);
			PObjectBinding bind = (itr != ref->m_unitBinding.end()) ? itr->second : NULL;
			if(bind != NULL)
			{
				script = new LuaCreature(src);
				script->m_binding = bind;
				ref->m_creatureInterfaceMap.insert(make_pair(id, script));
			}
		}
		return script;
	}
	void bindCreatureMethods(luabridge::module & m)
	{
#define prop(name) .property_ro(#name, &CreatureProto::name)
		m	.class_<CreatureProto>("CreatureProto")
		prop(Id)
		prop(MinLevel)
		prop(MaxLevel)
		prop(Faction)
		prop(MinHealth)
		prop(MaxHealth)
		prop(Mana)
		prop(NPCFLags)
		prop(MinDamage)
		prop(MaxDamage)
		prop(boss)
		prop(walk_speed)
		prop(run_speed)
		prop(fly_speed)
		prop(isTrainingDummy)
		prop(guardtype)
		prop(summonguard);
#undef prop
#define prop(name) .property_ro(#name, &CreatureInfo::name)
		m	.class_<CreatureInfo>("CreatureInfo")
		prop(Id)
		prop(Name)
		prop(SubName)
		prop(Type)
		prop(Family)
		prop(Rank);
#undef prop
		m	.class_<CreatureAIScript>("CreatureAIScript")
		.method(&CreatureAIScript::RegisterAIUpdateEvent, "RegisterAIUpdateEvent", "registerAIUpdateEvent", "registeraiupdateevent", "registeraiupdatevent", NULL)
		.method(&CreatureAIScript::ModifyAIUpdateEvent, "ModifyAIUpdateEvent", "modifyaiupdateevent", "modifyaiupdatevent", "modifyAIUpdateEvent", NULL)
		.method(&CreatureAIScript::RemoveAIUpdateEvent, "removeAIUpdateEvent", "removeaiupdatevent", "RemoveAIUpdateEvent", "removeaiupdateevent", NULL);

#define bind(name) .method(#name, &Creature::name)
		m	.class_decl<lua_creature>("Creature");
		m	.subclass<Creature, Unit>("Creature")
		.method(&Creature::IsVehicle, "IsVehicle", "isVehicle", "isvehicle", NULL)
		//bind(isGuard)
		//bind(isNeutralGuard)
		.method((void (Creature::*)(bool))&Creature::RemoveFromWorld, "RemoveFromWorld" , "removefromworld", NULL)
		.method(&Creature::HasItems, "HasItems", "hasItems", "hasitems", NULL)
		.method(&Creature::GetProto, "GetCreatureProto", "getCreatureProto", "getcreatureproto", "GetProto", "getProto", "getproto", NULL)
		.method(&Creature::GetItemIdBySlot, "GetItemIdBySlot", "GetItemIDBySlot", "getitemidbyslot", "getItemIDBySlot", NULL)
		.method(&Creature::GetItemAmountBySlot, "GetItemAmountBySlot", "getItemAmountBySlot", "getitemamountbyslot", NULL)
		.method(&Creature::GetSlotByItemId, "getSlotByItemID", "GetSlotByItemId", "getslotbyitemid", NULL)
		.method(&Creature::GetItemAmountByItemId, "getitemamountbyitemid", "getItemAmountByItemId", "GetItemAmountByItemId", NULL)
		.method(&Creature::HasQuests, "hasQuests", "HasQuests", "hasquests", NULL)
		.method(&Creature::HasQuest, "hasQuest", "HasQuest", "hasquest", NULL)
		.method(&Creature::AddQuest, "addQuest", "AddQuest", "addquest", NULL)
		.method(&Creature::DeleteQuest, "deleteQuest", "DeleteQuest", "deletequest", NULL)
		.method(&Creature::FindQuest, "findQuest", "findquest", "FindQuest", NULL)
		.method(&Creature::GetQuestRelation, "getQuestRelation", "getquestrelation", "GetQuestRelation", NULL)
		.method(&Creature::NumOfQuests, "NumOfQuests", "numOfQuests", "numofquests", NULL)
		.method(&Creature::isVendor, "IsVendor", "isVendor", "isvendor", NULL)
		.method(&Creature::isTrainer, "IsTrainer", "isTrainer", "istrainer", NULL)
		.method(&Creature::isClass, "IsClass", "isclass", "isClass", "isClassTrainer", "IsClassTrainer", "isclasstrainer", NULL)
		.method(&Creature::isProf, "IsProf", "isProf", "isprof", "IsProfTrainer", "isProfTrainer", "isproftrainer", NULL)
		.method(&Creature::isQuestGiver, "isQuestGiver", "IsQuestGiver", "isquestgiver", NULL)
		.method(&Creature::isTaxi, "isTaxi", "IsTaxi", "istaxi", "isFlightMaster", "IsFlightMaster", "isflightmaster", NULL)
		bind(isGossip)
		.method(&Creature::isCharterGiver, "isCharterGiver", "ischartergiver", "IsCharterGiver", NULL)
		.method(&Creature::isGuildBank, "isGuildBank", "IsGuildBank", "isguildbank", "isGuildBanker", "IsGuildBanker", "isguildbanker", NULL)
		.method(&Creature::isBattleMaster, "IsBattleMaster", "isBattleMaster", "isbattlermaster", NULL)
		.method(&Creature::isBanker, "isBanker", "IsBanker", "isbanker", NULL)
		.method(&Creature::isInnkeeper, "isInnKeeper", "isInnKeeper", "isinnkeeper", NULL)
		.method(&Creature::isSpiritHealer, "IsSpiritHealer", "isSpiritHealer", "isspirithealer", "ispirithealer", NULL)
		.method(&Creature::isTabardDesigner, "isTabardDesigner", "IsTabardDesigner", "istabarddesigner", "istabardesigner", NULL)
		.method(&Creature::isAuctioner, "isAuctioner", "isAuctioneer", "IsAuctioneer", "isauctioneer", NULL)
		.method(&Creature::isStableMaster, "isStableMaster", "isstablemaster", "istablemaster", "IsStableMaster", NULL)
		.method(&Creature::isArmorer, "IsArmorer", "isArmorer", "isarmorer", NULL)
		.property_rw("canregeneratehp", &Creature::m_canRegenerateHP)
		.method(&Creature::CanSee, "cansee", "canSee", "CanSee", NULL)
		.property_ro("isSkinned", &Creature::Skinned)
		.method(&Creature::GetSQL_id, "GetSQL_Id", "GetSQLID", "getSQLID", "getsqlid", NULL)
		//.method(&Creature::GetTotemSlot, "getTotemSlot", "GetTotemSlot", "gettotemslot", "getotemslot", NULL)
		.method(&Creature::GetGroup, "getGroup", "GetGroup", "getgroup", NULL)
		.method(&Creature::IsPickPocketed, "isPickPocketed", "IsPickPocketed", "ispickpocketed", NULL)
		.method(&Creature::GetCreatureInfo, "getCreatureInfo", "GetCreatureInfo", "getcreatureinfo", "getinfo", "GetInfo", "getInfo", NULL)
		bind(SetCreatureProto) //hrmm, probably not a good idea to have this method available
		.method(&Creature::IsTotem, "isTotem", "IsTotem", "istotem", NULL)
		.method(&Creature::IsExotic, "isExotic", "IsExotic", "isexotic", NULL)
		.method(&Creature::GetScript, "GetScript", "getScript", "getscript", "getInterface", "GetInterface", "getinterface", NULL)
		.method(&Creature::isCritter, "isCritter", "IsCritter", "iscritter", NULL)
		.method(&Creature::isTrainingDummy, "isTrainingDummy", "IsTrainingDummy", "isDummy", "istrainingdummy", "isdummy", NULL)
		.method(&Creature::Despawn, "Despawn", "despawn", NULL)
		.property_rw("m_noRespawn", &Creature::m_noRespawn)
		.property_rw("m_norespawn", &Creature::m_noRespawn)
		.property_ro("m_escorter", &Creature::m_escorter)
		.method((void (Creature::*)(lua_function, uint32, uint32, variadic_parameter*, lua_stack))&lua_creature::RegisterScriptEngineFunction, "RegisterEvent", "registerevent", NULL)
		.method((void (Creature::*)())&lua_creature::RemoveScriptEngineEvents, "RemoveEvents", "removeevents", "removevents", NULL)
		.method(&Creature::GetSQL_id, "GetSpawnId", "GetSpawnID", "getSpawnId", "getSpawnID", "getspawnid", NULL)
		.method(&Creature::SaveToDB, "SaveToDB", "saveToDB", "savetodb", NULL);
#undef bind
#define BIND(name) .method(#name, &AIInterface::name)
		m.	class_<AIInterface>("AIInterface")
		.method(&AIInterface::GetPetOwner, "GetPetOwner", "getPetOwner", "getpetowner", "getOwner", "getowner", "GetOwner", NULL)
		.method(&AIInterface::SetPetOwner, "setPetOwner", "SetPetOwner", "setpetowner", "SetOwner", "setOwner", "setowner", NULL)
		.method((void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFollow, "setUnitToFollow", "SetUnitToFollow", "setunittofollow", NULL)
		.method(&AIInterface::getUnitToFollow, "GetUnitToFollow", "getUnitToFollow", "getunittofollow", "getunitofollow", NULL)
		.method(&AIInterface::SetFollowDistance, "setFollowDistance", "SetFollowDistance", "setfollowdistance", NULL)
		.method(&AIInterface::SetUnitToFollowAngle, "setFollowAngle", "setfollowangle", "SetFollowAngle", "SetUnitToFollowAngle", "setunittofollowangle", "setUnitToFollowAngle", NULL)
		.method(&AIInterface::getUnitToFear, "GetUnitToFear", "getUnitToFear", "getunittofear", "getunitofear", NULL)
		.method((void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFear, "SetUnitToFear", "setUnitToFear", "setunittofear", "setunitofear", NULL)
		.method(&AIInterface::getSoullinkedWith, "getSoullinkedWith", "GetSoullinkedWith", "getsoullinkedwith", NULL)
		.method(&AIInterface::GetIsSoulLinked, "isSoullinked", "IsSoullinked", "issoullinked", "GetIsSoullinked", "getIsSoullinked", "getissoullinked", NULL)
		.method(&AIInterface::SetSoulLinkedWith, "setsoullinkediwith", "setSoullinkedWith", "SetSoullinkedWith", NULL)
		.method(&AIInterface::getAIState, "getaistate", "getAIState", "GetAIState", NULL)
		.method(&AIInterface::SetAIState, "setAIState", "setaistate", "SetAIState", NULL)
		BIND(addSpellToList)
		.method(&AIInterface::SetNextSpell, "setnextspell", "SetNextSpell", "setNextSpell", NULL)
		.method(&AIInterface::MoveTo, "MoveTo", "moveTo", "moveto", NULL)
		//BIND(calcMoveTimeToLocation)
		.method(&AIInterface::StopMovement, "stopmovement", "StopMovement", "stopMovement", NULL)
		.method(&AIInterface::hasWaypoints, "HasWaypoints", "hasWaypoints", "haswaypoints", NULL)
		//BIND(Movement_allowmovement)
		//BIND(Movement_canmove)
		//BIND(Movement_isflying)
		//BIND(calcAggroRange)
		//BIND(calcCombatRange)
		.method(&AIInterface::getNextTarget, "getNextTarget", "GetNextTarget", "getnexttarget", "getnextarget", NULL)
		.method((void(AIInterface::*)(Unit*))&AIInterface::setNextTarget, "setNextTarget", "SetNextTarget", "setnexttarget", "setnextarget", NULL)
		.method(&AIInterface::AttackReaction, "attackreaction", "AttackReaction", "attackReaction", NULL)
		.method(&AIInterface::HealReaction, "healReaction", "HealReaction", "hellreaction", NULL)
		.property_rw("disable_combat", &AIInterface::disable_combat)
		.property_rw("disable_melee", &AIInterface::disable_melee)
		.property_rw("disable_spell", &AIInterface::disable_spell)
		.property_rw("disable_targeting", &AIInterface::disable_targeting)
		.property_rw("m_canRangedAttack", &AIInterface::m_canRangedAttack)
		.property_rw("m_canCallForHelp", &AIInterface::m_canCallForHelp)
		.property_rw("m_CallForHelpHealth", &AIInterface::m_CallForHelpHealth)
		.property_rw("m_canFlee", &AIInterface::m_canFlee)
		.property_rw("m_FleeHealth", &AIInterface::m_FleeHealth)
		.property_rw("m_FleeDuration", &AIInterface::m_FleeDuration)
		.method(&AIInterface::ClearHateList, "clearhatelist", "clearHateList", "ClearHateList", NULL)
		.method(&AIInterface::WipeHateList, "wipeHateList", "WipeHateList", "wipehatelist", "WipeThreatList", "wipeThreatList", "wipethreatlist", NULL)
		.method(&AIInterface::WipeTargetList, "wipetargetlist", "wipeTargetList", "WipeTargetList", NULL)
		.method(&AIInterface::RemoveThreatByPtr, "removethreatbyptr", "removeThreatByPtr", "RemoveThreatByPtr", NULL)
		.method(&AIInterface::modThreatByPtr, "modThreatByPtr", "ModThreatByPtr", "modthreatbyptr", NULL)
		.method(&AIInterface::getThreatByPtr, "getThreatByPtr", "getthreatbyptr", "gethreatbyptr", "GetThreatbyPtr", NULL)
		.method(&AIInterface::GetMostHated, "getMostHated", "getmosthated", "GetMostHated", "GetMainTank", "getMainTank", "getmaintank", NULL)
		.method(&AIInterface::GetSecondHated, "getSecondHated", "GetSecondHated", "getsecondhated", "getAddTank", "GetAddTank", "getaddtank", NULL)
		.method(&AIInterface::taunt, "taunt", "Taunt", NULL)
		.method(&AIInterface::getTauntedBy, "getTauntedBy", "gettauntedby", "getauntedby", "GetTauntedBy", NULL)
		.method(&AIInterface::GetIsTaunted, "GetIsTaunted", "isTaunted", "istaunted", NULL)
		.method(&AIInterface::getOutOfCombatRange, "getOutOfCombatRange", "getoutofcombatrange", "GetOutOfCombatRange", "getresetdistance", "getResetDistance", "GetResetDistance", NULL)
		.method(&AIInterface::setOutOfCombatRange, "setOutOfCombatRange", "setoutofcombatrange", "SetOutOfCombatRange", "setresetdistance", "setResetDistance", "SetResetDistance", NULL)
		.method(&AIInterface::getCurrentWaypoint, "getCurrentWaypoint", "getcurrentwaypoint", "GetCurrentWaypoint", NULL)
		.method(&AIInterface::changeWayPointID, "ChangeWaypointID", "changeWaypointID", "changewaypointid", NULL)
		.method(&AIInterface::getWayPoint, "getWayPoint", "GetWayPoint", "getwaypoint", NULL)
		.method(&AIInterface::deleteWayPoint, "deleteWayPoint", "DeleteWayPoint", "deletewaypoint", NULL)
		.method(&AIInterface::deleteWaypoints, "deleteWayPoints", "DeleteWayPoints", "deletewaypoints", NULL)
		.method(&AIInterface::setMoveType, "setmovetype", "SetMoveType", "setMoveType", NULL)
		.method(&AIInterface::getMoveType, "getMoveType", "GetMoveType", "getmovetype", NULL)
		.method(&AIInterface::setWaypointToMove, "setwaypointtomove", "setwaypointomove", "setWayPointToMove", "setWaypointToMove", "SetWaypointToMove", "SetNextWaypoint", "setnextwaypoint", "setNextWaypoint", NULL)
		.method(&AIInterface::IsFlying, "isFlying", "isflying", "IsFlying", NULL)
		.method(&AIInterface::getCurrentAgent, "getCurrentAgent", "GetCurrentAgent", "getcurrentagent", NULL)
		.method(&AIInterface::setCurrentAgent, "setcurrentagent", "setCurrentAgent", "SetCurrentAgent", NULL)
		.method(&AIInterface::SetAllowedToEnterCombat, "setAllowedToEnterCombat", "SetAllowedToEnterCombat", "setallowedtoentercombat", NULL)
		.method(&AIInterface::GetAllowedToEnterCombat, "getAllowedToEnterCombat", "GetAllowedToEnterCombat", "getallowedtoentercombat", NULL)
		.method(&AIInterface::getAITargetsCount, "getAITargetsCount", "getaitargetscount", "GetAITargetsCount", NULL)
		.method(&AIInterface::addWayPoint, "addWayPoint", "AddWayPoint", "addwaypoint", NULL)
		.method(&AIInterface::addAssistTargets, "AddAssistTargets", "addAssistTargets", "addassisttargets", NULL)
		.method(&AIInterface::SetAIType, "SetAIType", "setAIType", "setaitype", NULL)
		.method(&AIInterface::getAIType, "GetAIType", "getAIType", "getaitype", NULL);
#undef BIND

		//waypoint struct
		m	.class_<WayPoint>("WayPoint")
		.constructor<void (*)() >()
		.property_rw("id", &WayPoint::id)
		.property_rw("x", &WayPoint::x)
		.property_rw("y", &WayPoint::y)
		.property_rw("z", &WayPoint::z)
		.property_rw("o", &WayPoint::o)
		.property_rw("waittime", &WayPoint::waittime)
		.property_rw("flags", &WayPoint::flags)
		.property_rw("backwardskinid", &WayPoint::backwardskinid)
		.property_rw("forwardskinid", &WayPoint::forwardskinid);

	}
}