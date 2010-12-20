/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "StdAfx.h"
#include "LUAEngine.h"
class LuaCreature : public CreatureAIScript
{
public:
	PObjectBinding m_binding;

	LuaCreature(Creature* creature) : CreatureAIScript(creature), m_binding(NULL) {}
	~LuaCreature() {}
	ARCEMU_INLINE void SetUnit(Creature * ncrc) { _unit = ncrc; }
	void OnCombatStart(Unit* mTarget)
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ENTER_COMBAT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_ENTER_COMBAT);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnCombatStop(Unit* mTarget)
	{
		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LEAVE_COMBAT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_LEAVE_COMBAT);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnTargetDied(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_DIED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnDied(Unit *mKiller)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DIED);
		push_unit(mKiller);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetParried(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_PARRIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_PARRIED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetDodged(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DODGED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_DODGED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetBlocked(Unit* mTarget, int32 iAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_BLOCKED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_BLOCKED);
		push_unit(mTarget);
		push_int(iAmount);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnTargetCritHit(Unit* mTarget, int32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_CRIT_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_CRIT_HIT);
		push_unit(mTarget);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnParried(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_PARRY]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_PARRY);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnDodged(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DODGED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DODGED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	}
	void OnBlocked(Unit* mTarget, int32 iAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_BLOCKED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_BLOCKED);
		push_unit(mTarget);
		push_int(iAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnCritHit(Unit* mTarget, int32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CRIT_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_CRIT_HIT);
		push_unit(mTarget);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnHit(Unit* mTarget, float fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_HIT);
		push_unit(mTarget);
		push_float(fAmount);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnAssistTargetDied(Unit* mAssistTarget)
	{

		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ASSIST_TARGET_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_ASSIST_TARGET_DIED);
		push_unit(mAssistTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnFear(Unit* mFeared, uint32 iSpellId)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FEAR]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_FEAR);
		push_unit(mFeared);
		push_int(iSpellId);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnFlee(Unit* mFlee)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FLEE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_FLEE);
		push_unit(mFlee);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnCallForHelp()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CALL_FOR_HELP]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_CALL_FOR_HELP);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
	}
	void OnLoad()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOAD]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_LOAD);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
			uint32 iid = _unit->GetInstanceID();
		if (_unit->GetMapMgr() == NULL || _unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			iid = 0;
		/*OnLoadInfo.push_back(_unit->GetMapId());
		OnLoadInfo.push_back(iid);
		OnLoadInfo.push_back(GET_LOWGUID_PART(_unit->GetGUID()));*/
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_REACH_WP]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_REACH_WP);
		push_int(iWaypointId);
		push_bool(bForwards);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOOT_TAKEN]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_LOOT_TAKEN);
		push_unit(pPlayer);
		push_int(pItemPrototype->ItemId);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void AIUpdate()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_AIUPDATE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_AIUPDATE);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
	}
	void OnEmote(Player * pPlayer, EmoteType Emote)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_EMOTE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_EMOTE);
		push_unit(pPlayer);
		push_int( (int)Emote);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DAMAGE_TAKEN]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DAMAGE_TAKEN);
		push_unit(mAttacker);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void StringFunctionCall(int fRef)
	{

		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall( (lua_function)fRef);
		push_unit(_unit);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
	}
	void Destroy()
	{
		{
			li::CreatureInterfaceMap::iterator it;
			li::CreatureInterfaceMap::iterator itr = lua_instance->m_creatureInterfaceMap.find(_unit->GetEntry() ), itend = lua_instance->m_creatureInterfaceMap.upper_bound(_unit->GetEntry() );
			for(;itr != itend;)
			{
				it = itr++;
				if(it->second != NULL && it->second == this)
					lua_instance->m_creatureInterfaceMap.erase(it);
			}
		}
		{
			//Function Ref clean up
			li::ObjectFRefMap::iterator it, itr = lua_instance->m_creatureFRefs.find(_unit->GetLowGUID() );
			li::ObjectFRefMap::iterator itend = lua_instance->m_creatureFRefs.upper_bound(_unit->GetLowGUID() );
			while(itr != itend)
			{
				it = itr++;
				sEventMgr.RemoveEvents(_unit, itr->second->head_node->val.obj_ref+EVENT_LUA_CREATURE_EVENTS);
				cleanup_varparam(it->second, lua_state);
					
			}
		}
		delete this;
	}
};

namespace lua_engine
{
	CreatureAIScript * createluacreature(Creature * src)
	{
		LuaCreature * script = NULL;
		if(src != NULL)
		{
			uint32 id = src->GetEntry();
			li::ObjectBindingMap::iterator itr = lua_instance->m_unitBinding.find(id);
			PObjectBinding bind = (itr != lua_instance->m_unitBinding.end() ) ? itr->second : NULL;
			if( bind != NULL)
			{
				script = new LuaCreature(src);
				script->m_binding = bind;
				lua_instance->m_creatureInterfaceMap.insert( make_pair(id, script) );
			}
		}
		return script;
	}

	void bindUnitMethods(luabridge::module & m)
	{
		m.	subclass<Unit, Object>("Unit")
			.method("GetAIInterface", &Unit::GetAIInterface)
			//.method("GetMobInterface", &Unit::GetMobInterface)
			.method("IsPVPFlagged", &Unit::IsPvPFlagged)
			.method("SetPvPFlag", &Unit::SetPvPFlag)
			.method("RemovePvPFlag", &Unit::RemovePvPFlag)
			.method("IsFFAPvPFlagged", &Unit::IsFFAPvPFlagged)
			.method("SetFFAPvPFlag", &Unit::SetFFAPvPFlag)
			.method("RemoveFFAPvPFlag", &Unit::RemoveFFAPvPFlag)
			.method("IsSanctuaryFlagged", &Unit::IsSanctuaryFlagged)
			.method("SetSanctuaryFlag", &Unit::SetSanctuaryFlag)
			.method("RemoveSanctuaryFlag", &Unit::RemoveSanctuaryFlag)
			/*.method("delayBaseAttackTime", &Unit::delayBaseAttackTime)
			.method("delayRangedAttackTime", &Unit::delayRangedAttackTime)
			.method("delayOffHandAttackTime", &Unit::delayOffHandAttackTime)
			.method("delayAllAttackTime", &Unit::delayAllAttackTime)*/
			.method("setAttackTimer", &Unit::setAttackTimer)
			.method("isAttackReady", &Unit::isAttackReady)
			.method("getLevel", &Unit::getLevel)
			.method("setLevel", &Unit::setLevel)
			.method("modLevel", &Unit::modLevel)
			.method("getClassMask", &Unit::getClassMask)
			.method("getRaceMask", &Unit::getRaceMask)
			.method("getStandState", &Unit::getStandState)
			.method("addStateFlag", &Unit::addStateFlag)
			.method("hasStateFlag", &Unit::hasStateFlag)
			.method("clearStateFlag", &Unit::clearStateFlag)
			.method("GetAP", &Unit::GetAP)
			.method("GetRAP", &Unit::GetRAP)
			.method("CastSpell", (uint8(Unit::*)(Unit*,uint32,bool))&Unit::CastSpell)
			.method("CastSpellAoF", &Unit::CastSpellAoF)
			.method("IsCasting", &Unit::IsCasting)
			.method("IsInInstance", &Unit::IsInInstance)
			.method("Demorph", &Unit::DeMorph)
			.method("IsDazed", &Unit::IsDazed)
			.method("GetStealthLevel", &Unit::GetStealthLevel)
			.method("GetStealthDetectBonus", &Unit::GetStealthDetectBonus)
			.method("IsStealthed", &Unit::IsStealth)
			.method("SetInvisibility", &Unit::SetInvisibility)
			.method("IsInvisible", &Unit::IsInvisible)
			.method("HasAura", &Unit::HasAura)
			.method("GetAuraStackCount", &Unit::GetAuraStackCount)
			.method("HasAuraWithMechanic", &Unit::HasAuraWithMechanics)
			.method("RemoveAura", (bool(Unit::*)(uint32) )&Unit::RemoveAura)
			.method("RemoveAllAuras", (void(Unit::*)() )&Unit::RemoveAllAuras)
#define BIND(name) .method(#name,&Unit::name)
			BIND(isAlive)
			BIND(IsDead)
			//BIND(setDeathState)
			//BIND(getDeathState)
			//BIND(RemoveAllMovementImpairing)
			BIND(RemoveNegativeAuras)
			BIND(RemoveAllNonPersistentAuras)
			//BIND(InterruptSpell)
			BIND(setAItoUse)
			BIND(GetThreatModifyer)
			BIND(ModThreatModifyer)
			BIND(GetGeneratedThreatModifyer)
			BIND(ModGeneratedThreatModifyer)
			BIND(IsPacified)
			BIND(IsStunned)
			BIND(IsFeared)
			BIND(Heal)
			BIND(Energize)
			BIND(setEmoteState)
			BIND(GetOldEmote)
			BIND(GetFaction)
			BIND(GetHealthPct)
			BIND(SetFaction)
			BIND(SendChatMessage)
			BIND(SendChatMessageToPlayer)
			BIND(SendChatMessageAlternateEntry)
			BIND(SetHealthPct)
			BIND(GetManaPct)
			BIND(GetCurrentSpell)
			BIND(SetCurrentSpell)
			BIND(RemoveStealth)
			BIND(RemoveInvisibility)
			BIND(create_guardian)
			BIND(DisableAI)
			BIND(EnableAI)
			BIND(IsSpiritHealer)
			BIND(Phase)
			BIND(SetFacing)
			BIND(SetMinDamage)
			BIND(GetMinDamage)
			BIND(SetMaxDamage)
			BIND(GetMaxDamage)
			BIND(SetMinOffhandDamage)
			BIND(GetMinOffhandDamage)
			BIND(SetMaxOffhandDamage)
			BIND(GetMaxOffhandDamage)
			BIND(SetMaxRangedDamage)
			BIND(GetMaxRangedDamage)
			BIND(SetMount)
			BIND(GetMount)
			BIND(SetCreatedBySpell)
			BIND(GetCreatedBySpell)
			BIND(SetStat)
			BIND(GetStat)
			BIND(SetResistance)
			BIND(GetResistance)
			BIND(SetBaseMana)
			BIND(SetBaseHealth)
			BIND(SetAttackPower)
			BIND(GetAttackPower)
			BIND(GetRangedAttackPower)
			BIND(SetRangedAttackPower)
			BIND(getRace)
			BIND(setRace)
			BIND(getClass)
			BIND(setClass)
			BIND(getGender)
			BIND(setGender)
			BIND(SetPowerType)
			BIND(GetPowerType)
			BIND(SetHealth)
			BIND(GetHealth)
			BIND(SetMaxHealth)
			BIND(GetMaxHealth)
			BIND(ModHealth)
			BIND(ModMaxHealth)
			BIND(SetPower)
			BIND(ModPower)
			BIND(GetPower)
			BIND(SetMaxPower)
			BIND(GetMaxPower)
			BIND(SetDisplayId)
			BIND(GetDisplayId)
			BIND(GetNativeDisplayId)
			BIND(SetNativeDisplayId);
#undef BIND
#define BIND(name) .method(#name, &AIInterface::name)
			m.	class_<AIInterface>("AIInterface")
				/*BIND(getUnit)
				BIND(findClosestTarget)
				BIND(findRandomTarget)
				BIND(isMoving)*/
				BIND(GetPetOwner)
				BIND(SetPetOwner)
				.method("SetUnitToFollow", (void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFollow)
				//BIND(SetUnitToFollow)
				BIND(getUnitToFollow)
				BIND(SetFollowDistance)
				//BIND(getFollowDistance)
				BIND(SetUnitToFollowAngle)
				//BIND(getFollowAngle)
				BIND(getUnitToFear)
				//BIND(SetUnitToFear)
				.method("SetUnitToFear", (void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFear)
				/*BIND(setForcedTarget)
				BIND(getForcedTarget)
				BIND(hasForcedTarget)
				BIND(getPanicHp)
				BIND(setPanicHp)
				BIND(getPanicDuration)
				BIND(setPanicDuration)
				BIND(hasPanicked)
				BIND(getCallHelpHp)
				BIND(setCallHelpHp)
				BIND(hasCalledHelp)
				BIND(getCombatDisable)
				BIND(disableCombat)
				BIND(getMeleeDisable)
				BIND(disableMelee)
				BIND(getSpellDisable)
				BIND(disableSpells)
				BIND(getRangedDisable)
				BIND(disableRanged)
				BIND(getTargettingDisable)
				BIND(disableTargetting)*/
				BIND(getSoullinkedWith)
				BIND(GetIsSoulLinked)
				BIND(SetSoulLinkedWith)
				BIND(getAIState)
				//BIND(SetAIState)
				BIND(addSpellToList)
				//BIND(Spell_remove)
				//BIND(Spell_getnext)
				BIND(SetNextSpell)
				BIND(MoveTo)
				//BIND(calcMoveTimeToLocation)
				BIND(StopMovement)
				BIND(hasWaypoints)
				//BIND(Movement_allowmovement)
				.property_rw("m_canmove", &AIInterface::m_canMove)
				.property_rw("m_moveRun", &AIInterface::m_moveRun)
				.property_rw("m_moveFly", &AIInterface::m_moveFly)
				//BIND(Movement_canmove)
				//BIND(Movement_isflying)
				//BIND(calcAggroRange)
				//BIND(calcCombatRange)
				BIND(getNextTarget)
				.method("setNextTarget", (void(AIInterface::*)(Unit*) )&AIInterface::setNextTarget)
				BIND(AttackReaction)
				BIND(HealReaction);
#undef BIND
/*#define BIND(name) .method(#name, &MobAI::name)
			m	.subclass<MobAI,AIInterface>("MobAI")
				BIND(addNewTarget)
				BIND(getThreatByGUID)
				BIND(getThreatByPtr)
				BIND(getMostHated)
				BIND(setMostHated)
				BIND(getSecondHated)
				BIND(setSecondHated)
				BIND(clearMostHated)
				BIND(clearSecondHated)
				BIND(findTarget)
				BIND(modThreatByGUID)
				BIND(modThreatByPtr)
				BIND(removeThreatByPtr)
				BIND(removeThreatByGUID)
				BIND(wipeHateList)
				BIND(addAssistTarget)
				BIND(Waypoint_setmovetype)
				BIND(Waypoint_getmovetype)
				BIND(Waypoint_getcurrent)
				BIND(Waypoint_changeID)
				BIND(Waypoint_add)
				BIND(Waypoint_saveall)
				BIND(Waypoint_getWP)
				BIND(Waypoint_delete)
				BIND(Waypoint_deleteall)
				BIND(Waypoint_setnext)
				BIND(Waypoint_getcount)
				BIND(Waypoint_ismovingbackward)
				BIND(Waypoint_setbackwardmove)
				BIND(isInInstance)
				BIND(canResetHp)
				BIND(setCanResetHp)
				BIND(getAutoWipeRange)
				BIND(setAutoWipeRange)
				BIND(hasMeleeAgent)
				BIND(setMeleeAgent)
				BIND(setRangedAgent)
				BIND(hasRangedAgent)
				BIND(hasSpellAgent)
				BIND(setSpellAgent)
				BIND(hasPanicAgent)
				BIND(setPanicAgent)
				BIND(hasCallHelpAgent)
				BIND(setCallHelpAgent)
				BIND(getShootSpell)
				BIND(setShootSpell);
#undef BIND*/

	}
}

#ifdef REWRITE
namespace luaUnit
{
	int GossipSendPOI(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L, 2);
		int icon = luaL_checkint(L, 3);
		int flags = luaL_checkint(L, 4);
		int data = luaL_checkint(L, 5);
		const char * name = luaL_checkstring(L, 6);

		plr->Gossip_SendPOI(x, y, icon, flags, data, name);
		return 0;
	}

	int GossipComplete(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
 		plr->Gossip_Complete();
 		return 0;
	 }

	int IsPlayer(lua_State * L, Unit * ptr)
	{
		if(!ptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		if(ptr->IsPlayer())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);

		return 1;
	}

	int IsCreature(lua_State * L, Unit * ptr)
	{
		if(!ptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		if(ptr->GetTypeId()==TYPEID_UNIT)
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);

		return 1;
	}

	int Emote(lua_State * L, Unit * ptr)
	{
		if(ptr==NULL) return 0;
		uint32 emote_id = luaL_checkint(L, 1);
		uint32 time = luaL_checkint(L, 2);
		if(emote_id==0) 
			return 0;
		if (time > 0)
			ptr->EventAddEmote((EmoteType)emote_id,time);
		else 
			ptr->Emote((EmoteType)emote_id);
		return 1;
	}

	int GetName(lua_State * L, Unit * ptr)
	{
		if(!ptr)
			return 0;

		switch(ptr->GetTypeId())
		{
		case TYPEID_UNIT:
			lua_pushstring(L, TO_CREATURE(ptr)->GetCreatureInfo() ? TO_CREATURE(ptr)->GetCreatureInfo()->Name : "Unknown");
			break;

		case TYPEID_PLAYER:
			lua_pushstring(L, TO_PLAYER(ptr)->GetName());
			break;

		default:
			lua_pushstring(L, "Unknown");
			break;
		}

		return 1;
	}

	int PhaseSet(lua_State * L, Unit * ptr)
	{
		uint32 newphase = CHECK_ULONG(L,1);
		bool Save = (luaL_optint(L,2,false)>0 ? true:false); 
		Creature * crt = 0;
		Player * p_target = 0;
		//Save is only for creatures. if you want to save to DB with players, use your own query (security purposes).
		//Lua: CharDBQuery("UPDATE `characters` SET `phase`='"..phase.."' WHERE (`name`='"..player:GetName().."'",0)
		if (!ptr)
			return 0;

		switch (ptr->GetTypeId())
		{
		case TYPEID_UNIT:
			crt = TO_CREATURE(ptr);
			crt->Phase(PHASE_SET, newphase);
			if (crt->m_spawn) 
				crt->m_spawn->phase = newphase; 
			if (Save)
			{
				crt->SaveToDB();
				crt->m_loadedFromDB = true;
			}
			break;

		case TYPEID_PLAYER:
			p_target = TO_PLAYER(ptr);
			p_target->Phase(PHASE_SET, newphase);
			if( p_target->GetSession() )
			{
				WorldPacket data(SMSG_SET_PHASE_SHIFT, 4);
				data << newphase;
				p_target->GetSession()->SendPacket(&data);
			}
			break;

		default:
			break;
		}
		return 0;
	}

	int PhaseAdd(lua_State * L, Unit * ptr)
	{
		uint32 newphase = CHECK_ULONG(L,1);
		bool Save = (luaL_optint(L,2,false)>0 ? true:false);
		Creature * crt = 0;
		Player * p_target = 0;
		//Save is only for creatures. if you want to save to DB with players, use your own query (security purposes).
		//Lua: CharDBQuery("UPDATE `characters` SET `phase`='"..player:GetPhase().."' WHERE (`name`='"..player:GetName().."'",0)
		if (!ptr)
			return 0;

		switch (ptr->GetTypeId())
		{
		case TYPEID_UNIT:
			crt = TO_CREATURE(ptr);
			crt->Phase(PHASE_ADD, newphase);
			if (crt->m_spawn) 
				crt->m_spawn->phase |= newphase; 
			if (Save)
			{
				crt->SaveToDB();
				crt->m_loadedFromDB = true;
			}
			break;

		case TYPEID_PLAYER:
			p_target = TO_PLAYER(ptr);
			p_target->Phase(PHASE_ADD, newphase);
			if( p_target->GetSession() )
			{
				WorldPacket data(SMSG_SET_PHASE_SHIFT, 4);
				data << p_target->m_phase;
				p_target->GetSession()->SendPacket(&data);
			}
			break;

		default:
			break;
		}
		return 0;
	}

	int PhaseDelete(lua_State * L, Unit * ptr)
	{
		uint32 newphase = CHECK_ULONG(L,1);
		bool Save = (luaL_checkint(L,2)>0 ? true:false);
		Creature * crt = 0;
		Player * p_target = 0;
		//Save is only for creatures. if you want to save to DB with players, use your own query (security purposes).
		//Lua: CharDBQuery("UPDATE `characters` SET `phase`='"..player:GetPhase().."' WHERE (`name`='"..player:GetName().."'",0)
		if (!ptr)
			return 0;

		switch (ptr->GetTypeId())
		{
		case TYPEID_UNIT:
			crt = TO_CREATURE(ptr);
			crt->Phase(PHASE_DEL, newphase);
			if (crt->m_spawn) 
				crt->m_spawn->phase &= ~newphase; 
			if (Save)
			{
				crt->SaveToDB();
				crt->m_loadedFromDB = true;
			}
			break;

		case TYPEID_PLAYER:
			p_target = TO_PLAYER(ptr);
			p_target->Phase(PHASE_DEL, newphase);
			if( p_target->GetSession() )
			{
				WorldPacket data(SMSG_SET_PHASE_SHIFT, 4);
				data << p_target->m_phase;
				p_target->GetSession()->SendPacket(&data);
			}
			break;

		default:
			break;
		}
		return 0;
	}

	int GetPhase(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushnumber(L,ptr->m_phase);
		return 1;
	}

	int SendChatMessage(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 typ = CHECK_ULONG(L,1);
		uint32 lang = CHECK_ULONG(L,2);
		const char * message = luaL_checklstring(L, 3, NULL);
		if(message == NULL)
			return 0;

		ptr->SendChatMessage(typ, lang, message);
		return 0;
	}

	int PlayerSendChatMessage(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 type = CHECK_ULONG(L,1);
		uint32 lang = CHECK_ULONG(L,2);
		const char * msg = luaL_checklstring(L, 3, NULL);
		Player * plr = TO_PLAYER(ptr);
		if(msg == NULL || !plr)
			return 0;
		WorldPacket *data = sChatHandler.FillMessageData( type, lang, msg, plr->GetGUID(), 0 );
		plr->GetSession()->SendChatPacket(data, 1, lang, plr->GetSession());
		for(set< Object* >::iterator itr = plr->GetInRangePlayerSetBegin(); itr != plr->GetInRangePlayerSetEnd(); ++itr)
		{
			(static_cast< Player* >(*itr))->GetSession()->SendChatPacket(data, 1, lang, plr->GetSession());
		}
		return 0;
	}

	int AggroWithInRangeFriends(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		// If Pointer isn't in combat skip everything
		if (!ptr->CombatStatus.IsInCombat())
		  return 0;

		Unit * pTarget = ptr->GetAIInterface()->getNextTarget();
		if (!pTarget)
		  return 0;

		Unit * pUnit = NULL;
		for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			Object * obj = TO_OBJECT(*itr);
			// No Object, Object Isn't a Unit, Unit is Dead
			if (!obj || !obj->IsUnit() || TO_UNIT(obj)->IsDead())
			   continue;

			 if (!isFriendly(obj, ptr))
			   continue;

			if (ptr->GetDistance2dSq(obj) > 10*10) // 10yrd range?
			   continue;

			 pUnit = TO_UNIT(obj);
			if (!pUnit) // Should never happen!
			   continue;

			pUnit->GetAIInterface()->setNextTarget(pTarget);
			pUnit->GetAIInterface()->AttackReaction(pTarget, 1, 0);
		}
		return 0;
	}

	int MoveRandomArea(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		float x1 = CHECK_FLOAT(L, 1);
		float y1 = CHECK_FLOAT(L, 2);
		float z1 = CHECK_FLOAT(L, 3);
		float x2 = CHECK_FLOAT(L, 4);
		float y2 = CHECK_FLOAT(L, 5);
		float z2 = CHECK_FLOAT(L, 6);
		float o2 = CHECK_FLOAT(L, 7);

		ptr->GetAIInterface()->MoveTo( x1+(RandomFloat(x2-x1)), y1+(RandomFloat(y2-y1)), z1+(RandomFloat(z2-z1)), o2 );
		return 0;
	}

	int SetMovementType(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 typ = CHECK_ULONG(L,1);
		AIInterface * ai = ptr->GetAIInterface();
		if(ai->AIType_isMob() )
			TO_AIMOB(ai)->setWPMoveType( (MovementType)typ);
		return 0;
	}

	int GetX(lua_State * L, Unit * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L, ptr->GetPositionX());
		else
			lua_pushnil(L); 
		return 1;
	}

	int GetY(lua_State * L, Unit * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L, ptr->GetPositionY());
		else
			lua_pushnil(L);
		return 1;
	}

	int GetZ(lua_State * L, Unit * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L, ptr->GetPositionZ());
		else
			lua_pushnil(L);
		return 1;
	}

	int GetO(lua_State * L, Unit * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L, ptr->GetOrientation());
		else
			lua_pushnil(L);
		return 1;
	}

	int CastSpell(lua_State * L, Unit * ptr)
	{
		uint32 sp = CHECK_ULONG(L,1);
		if (sp && ptr)
			ptr->CastSpell(ptr,dbcSpell.LookupEntry(sp),true);
		return 0;
	}

	int FullCastSpell(lua_State * L, Unit * ptr)
	{
		uint32 sp = CHECK_ULONG(L,1);
		if (sp && ptr)
			ptr->CastSpell(ptr,dbcSpell.LookupEntry(sp),false);
		return 0;
	}
	int FullCastSpellOnTarget(lua_State * L, Unit * ptr)
	{
		if(ptr != NULL)
		{
			uint32 sp = CHECK_ULONG(L,1);
			Object * target = CHECK_OBJECT(L,2);
			if(sp && target != NULL)
				ptr->CastSpell(target->GetGUID(),sp,false);
		}
		return 0;
	}
	int CastSpellOnTarget(lua_State * L, Unit * ptr)
	{
		uint32 sp = CHECK_ULONG(L,1);
		Object * target = CHECK_OBJECT(L,2);
		if(ptr != NULL && sp && target != NULL)
			ptr->CastSpell(target->GetGUID(),sp,true);
		return 0;
	}
	int SpawnCreature(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 entry = CHECK_ULONG(L,1);
		float x = CHECK_FLOAT(L, 2);
		float y = CHECK_FLOAT(L, 3);
		float z = CHECK_FLOAT(L, 4);
		float o = CHECK_FLOAT(L, 5);
		uint32 faction = CHECK_ULONG(L,6);
		uint32 duration = CHECK_ULONG(L,7);
		uint32 equip1 = luaL_optint(L, 8, 1);
		uint32 equip2 = luaL_optint(L, 9, 1);
		uint32 equip3 = luaL_optint(L, 10, 1);
		uint32 phase = luaL_optint(L, 11, ptr->m_phase); 
		bool save = luaL_optint(L, 12, 0) ? true : false; 

		if(!entry)
		{
			lua_pushnil(L);
			return 1;
		}
		CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
		CreatureInfo *i = CreatureNameStorage.LookupEntry(entry);
	    
		if(p == NULL || i == NULL) {
			lua_pushnil(L);
			return 1;
		}
		CreatureSpawn * sp = new CreatureSpawn;
		uint32 gender = i->GenerateModelId(&sp->displayid);
		sp->entry = entry;
		sp->form = 0;
		sp->id = objmgr.GenerateCreatureSpawnID();
		sp->movetype = 0;
		sp->x = x;
		sp->y = y;
		sp->z = z;
		sp->o = o;
		sp->emote_state = 0;
		sp->flags = 0;
		sp->factionid = faction;
		sp->bytes0 = sp->setbyte(0,2,gender);
		sp->bytes1 = 0;
		sp->bytes2 = 0;
		sp->stand_state = 0;
		sp->death_state = 0;
		sp->channel_target_creature = sp->channel_target_go = sp->channel_spell = 0;
		sp->MountedDisplayID = 0;
		sp->Item1SlotDisplay = equip1;
		sp->Item2SlotDisplay = equip2;
		sp->Item3SlotDisplay = equip3;
		sp->CanFly = 0;
		sp->phase = phase;
		Creature * pCreature = ptr->GetMapMgr()->CreateCreature(entry);
		if(pCreature == NULL)
		{
			lua_pushnil(L);
			return 1;
		}
		pCreature->Load(sp, (uint32)NULL, NULL);
		pCreature->m_loadedFromDB = true;
		pCreature->SetFaction(faction);
		pCreature->SetInstanceID(ptr->GetInstanceID());
		pCreature->SetMapId(ptr->GetMapId());
		pCreature->SetEquippedItem(MELEE,equip1);
		pCreature->SetEquippedItem(OFFHAND,equip2);
		pCreature->SetEquippedItem(RANGED,equip3);
		pCreature->m_noRespawn = true;
		pCreature->PushToWorld(ptr->GetMapMgr());
		if (duration)
			pCreature->Despawn(duration,0);
		if (save)
			pCreature->SaveToDB();
		PUSH_UNIT(L,pCreature);
		return 1;
	}
	int SpawnGameObject(lua_State * L, Unit * ptr)
	{
		if (ptr == NULL) return 0;
		uint32 entry_id = CHECK_ULONG(L,1);
		float x = CHECK_FLOAT(L, 2);
		float y = CHECK_FLOAT(L, 3);
		float z = CHECK_FLOAT(L, 4);
		float o = CHECK_FLOAT(L, 5);
		uint32 duration = CHECK_ULONG(L, 6);
		float scale = (float)(luaL_optint(L, 7, 100) / 100.0f); 
		uint32 phase = luaL_optint(L, 8, ptr->m_phase);
		bool save = luaL_optint(L, 9, 0) ? true : false; 
		if (entry_id)
		{
			GameObject *go = ptr->GetMapMgr()->CreateGameObject(entry_id);
			uint32 mapid = ptr->GetMapId();
			go->SetInstanceID(ptr->GetInstanceID());
			go->CreateFromProto(entry_id,mapid,x,y,z,o);
			go->Phase(PHASE_SET, phase);
			go->SetScale(scale);
			// Create spawn instance
			GOSpawn * gs = new GOSpawn;
			gs->entry = go->GetEntry();
			gs->facing = go->GetOrientation();
			gs->faction = go->GetFaction();
			gs->flags = go->GetUInt32Value(GAMEOBJECT_FLAGS);
			gs->id = objmgr.GenerateGameObjectSpawnID();
			gs->o = 0.0f;
			gs->o1 = go->GetParentRotation(0);
			gs->o2 = go->GetParentRotation(2);
			gs->o3 = go->GetParentRotation(3);
			gs->scale = go->GetScale();
			gs->x = go->GetPositionX();
			gs->y = go->GetPositionY();
			gs->z = go->GetPositionZ();
			gs->state = go->GetByte(GAMEOBJECT_BYTES_1, 0);
			//gs->stateNpcLink = 0;
			gs->phase = go->GetPhase();

			go->m_spawn = gs;
			go->PushToWorld(ptr->GetMapMgr());

			if (duration)
				sEventMgr.AddEvent(go,&GameObject::ExpireAndDelete,EVENT_GAMEOBJECT_UPDATE,duration,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			if (save)
				go->SaveToDB();
			PUSH_GO(L,go);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int RegisterEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		const char * typeName = luaL_typename(L,1);
		int delay=luaL_checkint(L,2);
		int repeats=luaL_checkint(L,3);
		if(!delay) return 0;
		lua_settop(L,1);
		int functionRef = 0;
		if(!strcmp(typeName,"function") )
			functionRef = lua_ref(L,true);
		else if(!strcmp(typeName,"string"))
			functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,1));
		if(functionRef)
		{
			Creature * creature = TO_CREATURE(ptr);
			sEventMgr.AddEvent(creature,&Creature::TriggerScriptEvent, functionRef, EVENT_LUA_CREATURE_EVENTS, delay, repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			std::map< uint64,std::set<int> > & objRefs = lua_engine::getObjectFunctionRefs();
			std::map< uint64,std::set<int> >::iterator itr = objRefs.find(ptr->GetGUID());
			if(itr == objRefs.end() )
			{
				std::set<int> refs;
				refs.insert(functionRef);
				objRefs.insert(make_pair(ptr->GetGUID(),refs));
			}
			else
			{
				std::set<int> & refs = itr->second;
				refs.insert(functionRef);
			}
		}
		return 0;
	}
	/* This one just simply calls the function directly w/o any arguments, 
	the trick to arguments is done Lua side through closures(function that
	calls the wanted function  with the wanted arguments */
	int CreateLuaEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();

		const char * typeName = luaL_typename(L,1);
		int delay=luaL_checkint(L,2);
		int repeats=luaL_checkint(L,3);
		if(!delay) return 0;
		lua_settop(L,1);
		int functionRef = 0;
		if(!strcmp(typeName,"function") )
			functionRef = lua_ref(L,true);
		else if(!strcmp(typeName,"string"))
			functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,1));
		if(functionRef)
		{
			TimedEvent * ev = TimedEvent::Allocate(ptr,new CallbackP1<LuaEngine,int>(&sLuaMgr,&LuaEngine::CallFunctionByReference,functionRef),EVENT_LUA_CREATURE_EVENTS,delay,repeats);
			ptr->event_AddEvent(ev);
			std::map< uint64,std::set<int> > & objRefs = lua_engine::getObjectFunctionRefs();
			std::map< uint64,std::set<int> >::iterator itr = objRefs.find(ptr->GetGUID());
			if(itr == objRefs.end() )
			{
				std::set<int> refs;
				refs.insert(functionRef);
				objRefs.insert(make_pair(ptr->GetGUID(),refs));
			}
			else
			{
				std::set<int> & refs = itr->second;
				refs.insert(functionRef);
			}
		}
		return 0;
	}
	int RemoveEvents(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();
		sEventMgr.RemoveEvents(ptr,EVENT_LUA_CREATURE_EVENTS);
		//Unref all contained references
		std::map< uint64,std::set<int> > & objRefs = lua_engine::getObjectFunctionRefs();
		std::map< uint64,std::set<int> >::iterator itr = objRefs.find(ptr->GetGUID());
		if(itr != objRefs.end() )
		{
			std::set<int> & refs = itr->second;
			for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
				lua_unref(L,(*it));
			refs.clear();
		}
		return 0;
	}

	int SetFaction(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		int faction = luaL_checkint(L,1);
		if(!faction)
			return 0;

		ptr->SetFaction(faction);
		return 0;
	}
	int GetNativeFaction(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET()
		if (ptr->IsPlayer())
		{
			RET_INT(TO_PLAYER(ptr)->GetInitialFactionId());
		}
		else
		{
			if (TO_CREATURE(ptr)->GetProto())
			{
				RET_INT(TO_CREATURE(ptr)->GetProto()->Faction);
			}
			else
			{
				RET_INT(ptr->GetFaction());
			}
		}
	}
	int SetStandState(lua_State * L, Unit * ptr) //states 0..8
	{
		if (!ptr)
			return 0;
		int state = luaL_checkint(L,1);
		if (state < 0)
			return 0;
		ptr->SetStandState(state);
		return 0;
	}
	int IsInCombat(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || !ptr->IsInWorld() )
			RET_NIL()
		if(ptr->CombatStatus.IsInCombat())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int SetScale(lua_State * L, Unit * ptr)
	{
		float scale = CHECK_FLOAT(L,1);
		if (scale && ptr)
			ptr->SetFloatValue(OBJECT_FIELD_SCALE_X,(float)scale);
		else
			RET_BOOL(false)
		RET_BOOL(true)
	}

	int SetModel(lua_State * L, Unit * ptr)
	{
		uint32 model = CHECK_ULONG(L,1);
		if(ptr != NULL)
			ptr->SetDisplayId(model);
		else
			RET_BOOL(false)
		RET_BOOL(true)
	}

	int SetNPCFlags(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int flags = luaL_checkint(L,1);
		if (flags && ptr)
			ptr->SetUInt32Value(UNIT_NPC_FLAGS,flags);
		return 0;
	}
	int SetMount(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 DsplId = CHECK_ULONG(L, 1);
		ptr->SetMount(DsplId);
		return 0;
	}

	int DestroyCustomWaypointMap(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		TO_CREATURE(ptr)->DestroyCustomWaypointMap();
		return 0;
	}

	int CreateCustomWaypointMap(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * pCreature = TO_CREATURE(ptr);
		if(pCreature->m_custom_waypoint_map)
		{
			for(WayPointMap::iterator itr = pCreature->m_custom_waypoint_map->begin(); itr != pCreature->m_custom_waypoint_map->end(); ++itr)
				delete (*itr);
			delete pCreature->m_custom_waypoint_map;
		}

		pCreature->m_custom_waypoint_map = new WayPointMap;
		AIInterface * ai = pCreature->GetAIInterface();
		if(ai->AIType_isMob() )
			TO_AIMOB(ai)->setCustomWaypointMap(pCreature->m_custom_waypoint_map);
		return 0;
	}

	int CreateWaypoint(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		float o = CHECK_FLOAT(L,4);
		int waittime = luaL_checkint(L,5);
		int flags = luaL_checkint(L,6);
		int modelid = luaL_checkint(L,7);

		Creature * pCreature = TO_CREATURE(ptr);
		if(!pCreature->m_custom_waypoint_map)
		{
			pCreature->m_custom_waypoint_map = new WayPointMap;
			AIInterface * ai = ptr->GetAIInterface();
			if(ai->AIType_isMob() )
				TO_AIMOB(ai)->setCustomWaypointMap(pCreature->m_custom_waypoint_map);
		}

		if(!modelid)
			modelid = pCreature->GetDisplayId();

		WayPoint * wp = new WayPoint;
		wp->id = (uint32)pCreature->m_custom_waypoint_map->size() + 1;
		wp->x = x;
		wp->y = y;
		wp->z = z;
		wp->o = o;
		wp->flags = flags;
		wp->backwardskinid = modelid;
		wp->forwardskinid = modelid;
		wp->backwardemoteid = wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
		wp->waittime = waittime;
		pCreature->m_custom_waypoint_map->push_back(wp);
		AIInterface * ai = pCreature->GetAIInterface();
		if(ai->AIType_isMob() )
			TO_AIMOB(ai)->addWaypoint(wp);
		return 0;
	}
	int CreateCustomWaypoint(lua_State * L, Unit * ptr)
	{
		Creature * crc = NULL;
		if(ptr != NULL && ptr->IsCreature() )
			crc = TO_CREATURE(ptr);
		if(crc != NULL)
		{
			uint32 id = CHECK_ULONG(L,1);
			float x = CHECK_FLOAT(L,2);
			float y = CHECK_FLOAT(L,3);
			float z = CHECK_FLOAT(L,4);
			float o = CHECK_FLOAT(L,5);
			uint32 waitime = CHECK_ULONG(L,6);
			uint32 flags = CHECK_ULONG(L,7);
			uint32 model = luaL_optint(L,8,0);
			WayPoint * wp = new WayPoint;
			wp->id = id;
			wp->x = x;
			wp->y = y;
			wp->z = z;
			wp->o = o;
			wp->waittime = waitime;
			wp->flags = flags;
			wp->backwardskinid = model;
			wp->forwardskinid = model;
			AIInterface * ai = crc->GetAIInterface();
			if(ai->AIType_isMob() )
				TO_AIMOB(ai)->addWaypoint(wp);
		}
		return 0;
	}
	int DeleteAllWaypoints(lua_State * L, Unit * ptr)
	{
		Creature * crc = NULL;
		if(ptr != NULL && ptr->IsCreature() )
			crc = TO_CREATURE(ptr);
		AIInterface * ai = crc->GetAIInterface();
		if(ai->AIType_isMob() )
			TO_AIMOB(ai)->deleteWaypoints();
		return 0;
	}

	int MoveToWaypoint(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int id = luaL_checkint(L,1);
		if (id)
		{
			AIInterface * ai = ptr->GetAIInterface();
			if(ai->AIType_isMob() )
			{
				TO_AIMOB(ai)->setWPMoveType(MOVEMENTTYPE_WANTEDWP);
				TO_AIMOB(ai)->setWaypointToMove(id);
			}
		}
		return 0;
	}

	int RemoveItem(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int id = luaL_checkint(L,1);
		int count = luaL_checkint(L,2);

		TO_PLAYER(ptr)->GetItemInterface()->RemoveItemAmt(id,count);
		return 0;
	}

	int AddItem(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int id = luaL_checkint(L,1);
		int count = luaL_checkint(L,2);

		Player * plr = TO_PLAYER(ptr);
		ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(id);
		if (proto==NULL)
			return 0;

		Item * add = plr->GetItemInterface()->FindItemLessMax(id,count,false);
		if (add==NULL)
		{
			add=objmgr.CreateItem(id,plr);
			add->SetStackCount(count);
			if(plr->GetItemInterface()->AddItemToFreeSlot(add))
				plr->SendItemPushResult(false, true, false, true, plr->GetItemInterface()->LastSearchItemBagSlot(),
				 plr->GetItemInterface()->LastSearchItemSlot(), count, add->GetEntry(), add->GetItemRandomSuffixFactor(),
				 add->GetItemRandomPropertyId(), add->GetStackCount() );					
			else
				delete add;
		}
		else
		{
			add->ModStackCount(count);
			plr->SendItemPushResult(false, true, false, false,
        	 static_cast<uint8>(plr->GetItemInterface()->GetBagSlotByGuid( add->GetGUID() )), 0xFFFFFFFF,
        	 count , add->GetEntry(), add->GetItemRandomSuffixFactor(), add->GetItemRandomPropertyId(), add->GetStackCount()  ); 	 
		}
		PUSH_ITEM(L,add);
		return 1;
	}
	int GetInstanceID(lua_State * L, Unit * ptr)
	{
		//TEST_UNIT()
		if(!ptr || ptr->GetMapMgr() == NULL || ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL )
			lua_pushnil(L);
		else
			lua_pushinteger(L,ptr->GetInstanceID());
		return 1;
	}

	int GetClosestPlayer(lua_State * L, Unit * ptr)
	{
		if(!ptr)
			return 0;

		float dist=0;
		float d2=0;
		Player * ret=NULL;

		for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
		{
			d2=(*itr)->GetDistanceSq(ptr);
			if(!ret||d2<dist)
			{
				dist=d2;
				ret=TO_PLAYER(*itr);
			}
		}

		if(ret==NULL)
			lua_pushnil(L);
		else
			PUSH_UNIT(L,ret);

		return 1;
	}
	int GetRandomPlayer(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL ) 
			return 0;

		int flag = luaL_checkint( L, 1 );
		Player* ret = NULL;
		vector<Player*> players;
		switch( flag )
		{
		case RANDOM_ANY:
			{
				uint32 count = (uint32)ptr->GetInRangePlayersCount();
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if (count==r)
					{
						ret= TO_PLAYER(*itr);
						break;
					}
					++count;
				}
			}
			break;
		case RANDOM_IN_SHORTRANGE:

			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj && obj->CalcDistance(obj,ptr)<=8)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_IN_MIDRANGE:
			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					float distance = obj->CalcDistance(obj,ptr);
					if (distance<20 && distance>8)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_IN_LONGRANGE:
			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj && obj->CalcDistance(obj,ptr)>=20)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_WITH_MANA:
			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_MANA)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_WITH_ENERGY:
			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_ENERGY)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_WITH_RAGE:
			{
				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_RAGE)
						players.push_back(obj);
				}
				if(players.size())
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		case RANDOM_NOT_MAINTANK:
			{
				uint32 count = 0;
				Unit* mt = ( ptr->GetAIInterface() != NULL && ptr->GetAIInterface()->AIType_isMob() ) ? TO_AIMOB(ptr->GetAIInterface())->getMostHated() : NULL;
				if (mt == NULL || !mt->IsPlayer())
					return 0;

				for(set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = TO_PLAYER(*itr);
					if (obj != mt)
						players.push_back(obj);
				}
				if(players.size() )
					ret = players[RandomUInt(players.size()-1)];
			}
			break;
		}

		if(ret==NULL)
			lua_pushnil(L);
		else
			PUSH_UNIT(L,ret);

		return 1;
	}
	int GetRandomFriend(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();

		Unit * ret=NULL;
		uint32 count = 0;
		vector<Object*> allies;

		for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			Object* obj = TO_OBJECT(*itr);
			if (obj->IsUnit() && isFriendly(obj,ptr))
				allies.push_back(obj);
		}
		if(allies.size() )
			PUSH_UNIT(L,allies[RandomUInt(allies.size()-1)]);
		else
			lua_pushnil(L);
		return 1;
	}
	int GetRandomEnemy(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();

		Unit * ret=NULL;
		uint32 count = 0;
		vector<Object*> enemies;

		for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			Object* obj = TO_OBJECT(*itr);
			if (obj->IsUnit() && isHostile(ptr,obj) )
				enemies.push_back(obj);
		}
		if(enemies.size())
			PUSH_UNIT(L,enemies[RandomUInt(enemies.size()-1)]);
		else
			lua_pushnil(L);
		return 1;
	}

	int StopMovement(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int tim = luaL_checkint(L,1);
		ptr->GetAIInterface()->StopMovement(tim);
		return 0;
	}

	int RemoveAura(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		int auraid = luaL_checkint(L,1);
		ptr->RemoveAura(auraid);
		return 0;
	}

	int CanAttack(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		Unit * target = CHECK_UNIT(L,1);
		if (!target) return 0;
		if (isAttackable(ptr, target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int PlaySoundToSet(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();
		int soundid = luaL_checkint(L,1);
		ptr->PlaySoundToSet(soundid);
		return 0;
	}

	int PlaySoundToPlayer(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER();
		int soundid = luaL_checkint(L,1);
		Player* plr = TO_PLAYER(ptr);
		WorldPacket data;
        data.Initialize(SMSG_PLAY_OBJECT_SOUND);
		data << uint32(soundid) << plr->GetGUID();
		plr->GetSession()->SendPacket(&data);
		return 0;
	}

	int GetUnitBySqlId(lua_State * L, Unit * ptr)
	{
		int sqlid = luaL_checkint(L,1);
		if(!ptr || !sqlid)
			return 0;
		PUSH_UNIT(L,ptr->GetMapMgr()->GetSqlIdCreature(sqlid));
		return 1;
	}

	int GetInventoryItem(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int8 containerslot = luaL_checkint(L, 1);
		int16 slot = luaL_checkint(L, 2);
		Player * plr = TO_PLAYER(ptr);
		PUSH_ITEM(L, plr->GetItemInterface()->GetInventoryItem(containerslot, slot));
		return 1;
	}

	int GetInventoryItemById(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 entry = CHECK_ULONG(L, 1);
		Player * plr = TO_PLAYER(ptr);
		int16 slot = plr->GetItemInterface()->GetInventorySlotById(entry);
		if (slot == -1) //check bags
		{
			for (uint8 contslot = INVENTORY_SLOT_BAG_START; contslot != INVENTORY_SLOT_BAG_END; contslot++)
			{
				Container * bag = static_cast<Container*>(plr->GetItemInterface()->GetInventoryItem(contslot));
				for (uint8 bslot = 0; bslot != bag->GetNumSlots(); bslot++)
				{
					if (bag->GetItem(bslot) && bag->GetItem(bslot)->GetEntry() == entry)
					{
						PUSH_ITEM(L, bag->GetItem(bslot));
						return 1;
					}
				}
			}
		}
		PUSH_ITEM(L, plr->GetItemInterface()->GetInventoryItem(slot));
		return 1;
	}

	int SetZoneWeather(lua_State * L, Unit * ptr)
	{
		/*
		WEATHER_TYPE_NORMAL            = 0, // NORMAL (SUNNY)
		WEATHER_TYPE_FOG               = 1, // FOG
		WEATHER_TYPE_RAIN              = 2, // RAIN
		WEATHER_TYPE_HEAVY_RAIN        = 4, // HEAVY_RAIN
		WEATHER_TYPE_SNOW              = 8, // SNOW
		WEATHER_TYPE_SANDSTORM         = 16 // SANDSTORM
		*/
		uint32 zone_id = CHECK_ULONG(L, 1);
		uint32 type = CHECK_ULONG(L, 2);
		float Density = CHECK_FLOAT(L, 3); //min: 0.30 max: 2.00
		if (Density<0.30f || Density>2.0f || !zone_id || !type)
			return 0;

		uint32 sound;
		if(Density<=0.30f)
			sound = 0;

		switch(type)
		{
			case 2:                                             //rain
			case 4:                                             
				if(Density  <0.40f)
					 sound = 8533;
				else if(Density  <0.70f)
					sound = 8534;
				else
					sound = 8535;
				break;
			case 8:                                             //snow
				if(Density  <0.40f)
					sound = 8536;
				else if(Density  <0.70f)
					sound = 8537;
				else
					sound = 8538;
				break;
			case 16:                                             //storm
				if(Density  <0.40f)
					sound = 8556;
				else if(Density  <0.70f)
					sound = 8557;
				else
					sound = 8558;
				break;
			default:											//no sound
				sound = 0;
				break;
		}
		WorldPacket data(SMSG_WEATHER, 9);
		data.Initialize(SMSG_WEATHER);
		if(type == 0 ) // set all parameter to 0 for sunny.
			data << uint32(0) << float(0) << uint32(0) << uint8(0);		
		else if (type == 1) // No sound/density for fog
			data << type << float(0) << uint32(0) << uint8(0);		
		else
			data << type << Density << sound << uint8(0) ;

		sWorld.SendZoneMessage(&data, zone_id, 0);

		return 0;
	}

	int SetPlayerWeather(lua_State * L, Unit * ptr)
	{
		/*
		WEATHER_TYPE_NORMAL            = 0, // NORMAL (SUNNY)
		WEATHER_TYPE_FOG               = 1, // FOG
		WEATHER_TYPE_RAIN              = 2, // RAIN
		WEATHER_TYPE_HEAVY_RAIN        = 4, // HEAVY_RAIN
		WEATHER_TYPE_SNOW              = 8, // SNOW
		WEATHER_TYPE_SANDSTORM         = 16 // SANDSTORM
		*/
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 type = CHECK_ULONG(L, 1);
		float Density = CHECK_FLOAT(L, 2); //min: 0.30 max: 2.00
		if (Density<0.30f || Density>2.0f || !type)
			return 0;

		uint32 sound;
		if(Density<=0.30f)
			sound = 0;

		switch(type)
		{
			case 2:                                             //rain
			case 4:                                             
				if(Density  <0.40f)
					 sound = 8533;
				else if(Density  <0.70f)
					sound = 8534;
				else
					sound = 8535;
				break;
			case 8:                                             //snow
				if(Density  <0.40f)
					sound = 8536;
				else if(Density  <0.70f)
					sound = 8537;
				else
					sound = 8538;
				break;
			case 16:                                             //storm
				if(Density  <0.40f)
					sound = 8556;
				else if(Density  <0.70f)
					sound = 8557;
				else
					sound = 8558;
				break;
			default:											//no sound
				sound = 0;
				break;
		}
		WorldPacket data(SMSG_WEATHER, 9);
		data.Initialize(SMSG_WEATHER);
		if(type == 0 ) // set all parameter to 0 for sunny.
			data << uint32(0) << float(0) << uint32(0) << uint8(0);		
		else if (type == 1) // No sound/density for fog
			data << type << float(0) << uint32(0) << uint8(0);		
		else
			data << type << Density << sound << uint8(0) ;

		plr->GetSession()->SendPacket(&data);

		return 0;
	}

	int Despawn(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int delay = luaL_checkint(L,1);
		int respawntime = luaL_checkint(L,2);
		TO_CREATURE(ptr)->Despawn(delay,respawntime);
		return 0;
	}
	int GetInRangeFriends(lua_State * L, Unit * ptr)
	{
		Object * pC = NULL;
		uint32 count = 0;
		lua_newtable(L);
		for( set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
		{
			if( (*itr) ->IsUnit() && isFriendly(ptr, (*itr)) )
			{
				count++,
				pC = *itr;
				lua_pushinteger(L,count);
				PUSH_UNIT(L,pC);
				lua_rawset(L,-3);
			}
		}
		return 1;
	}

	int GetInRangeEnemies(lua_State * L, Unit * ptr)
	{
		uint32 count = 0;
		lua_newtable(L);
		for( set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
		{
			if( (*itr) ->IsUnit() && !isFriendly(ptr, (*itr)) )
			{
				count++,
				lua_pushinteger(L,count);
				PUSH_UNIT(L,*itr);
				lua_rawset(L,-3);
			}
		}
		return 1;
	}

	int GetInRangeUnits(lua_State * L, Unit * ptr)
	{
		uint32 count = 0;
		lua_newtable(L);
		for( set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
		{
			if( (*itr) ->IsUnit() )
			{
				count++,
				lua_pushinteger(L,count);
				PUSH_UNIT(L,*itr);
				lua_rawset(L,-3);
			}
		}
		return 1;
	}

	int GetItemCount(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int itemid = luaL_checkint(L,1);
		lua_pushinteger(L, TO_PLAYER(ptr)->GetItemInterface()->GetItemCount(itemid,false));
		return 1;
	}

	int GetPrimaryCombatTarget(lua_State * L, Unit * ptr)
	{//should use now instead of GetTarget
		TEST_PLAYER()
		if (!ptr->CombatStatus.IsInCombat())
		{
			lua_pushinteger(L, 0);
			return 1;
		}
		else
			PUSH_UNIT(L, ptr->GetMapMgr()->GetUnit(TO_PLAYER(ptr)->CombatStatus.GetPrimaryAttackTarget()));
		return 1;
	}

	int GetMainTank(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
			Unit* ret = NULL;
		if(ptr->GetAIInterface() != NULL && ptr->GetAIInterface()->AIType_isMob() )
			ret = TO_AIMOB(ptr->GetAIInterface() )->getMostHated();
		if (!ret)
			lua_pushnil(L);
		else
			PUSH_UNIT(L,ret);
		return 1;
	}

	int GetAddTank(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit* ret = NULL;
		AIInterface * ai = ptr->GetAIInterface();
		if(ai->AIType_isMob() )
			ret = TO_AIMOB(ai)->getSecondHated();
		if(ret==NULL)
			lua_pushnil(L);
		else
			PUSH_UNIT(L,ret);
		return 1;
	}

	int ClearThreatList(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		ptr->ClearHateList();
		return 0;
	}

	int GetTauntedBy(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()

		Unit * ret = ptr->GetAIInterface()->getForcedTarget();
		PUSH_UNIT(L,ret);
		return 1;
	}

	int SetTauntedBy(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		ptr->GetAIInterface()->setForcedTarget(target);
		return 0;
	}

	int ModThreat(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L,1);
		int32 amount = luaL_checkint(L,2);
		AIInterface * ai = ptr->GetAIInterface();
		if(ai->AIType_isMob() )
			TO_AIMOB(ai)->modThreatByPtr(target, amount);
		return 0;
	}

	int GetThreatByPtr(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L,1);
		AIInterface * ai = ptr->GetAIInterface();
		if(ai->AIType_isMob() && target != NULL)
			lua_pushnumber(L, TO_AIMOB(ai)->getThreatByPtr(target) );
		return 1;
	}

	int GetSoulLinkedWith(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()

		Unit * ret = ptr->GetAIInterface()->getSoulLinker();
		PUSH_UNIT(L, ret);
		return 1;
	}

	int SetSoulLinkedWith(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		if(target != NULL)
			ptr->GetAIInterface()->setSoulLinker(target->GetGUID() );
		return 1;
	}

	int ChangeTarget(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		if (!target || !isHostile(ptr,target) || ptr == target)
			return 0;
		else
			ptr->GetAIInterface()->setNextTarget(target);
		return 0;
	}

	int HasFinishedQuest(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		int questid = luaL_checkint(L,1);
		if( TO_PLAYER(ptr)->HasFinishedQuest(questid) )
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int FinishQuest(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		int quest_id = luaL_checkint(L,1);
		Player *plr = TO_PLAYER(ptr);
		Quest * qst = QuestStorage.LookupEntry(quest_id);
		if (qst)
		{
			if (plr->HasFinishedQuest(quest_id)) 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
			else
			{
				QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
				if (IsPlrOnQuest)
				{	
					sQuestMgr.GenerateQuestXP(plr, qst);
					sQuestMgr.BuildQuestComplete(plr, qst);

					IsPlrOnQuest->Finish();
					plr->AddToFinishedQuests(quest_id);
					lua_pushnumber(L, 1);
					return 1;
				}
				else
				{
					lua_pushnumber(L, 2);
					return 1;
				}		
			}
		}
		else
			return 0;
	}

	int StartQuest(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		int quest_id = luaL_checkint(L,1);
		Player *plr = TO_PLAYER(ptr);

		Quest * qst = QuestStorage.LookupEntry(quest_id);
		if (qst)
		{
			if (plr->HasFinishedQuest(quest_id)) {
				lua_pushnumber(L, 0);
				return 1;
			}
			else
			{
				QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
				if (IsPlrOnQuest) 
				{
					lua_pushnumber(L, 1);
					return 1;
				}
				else
				{
					int32 open_slot = plr->GetOpenQuestSlot();

					if (open_slot == -1)
					{
						sQuestMgr.SendQuestLogFull(plr);
						lua_pushnumber(L, 2);
						return 1;
					}
					else
					{
						QuestLogEntry *qle = new QuestLogEntry();
						qle->Init(qst, plr, (uint32)open_slot);
						qle->UpdatePlayerFields();
			
						// If the quest should give any items on begin, give them the items.
						for(uint32 i = 0; i < 4; ++i)
						{
							if(qst->receive_items[i])
							{
								Item *item = objmgr.CreateItem( qst->receive_items[i], plr);
								if(item==NULL)
									return false;

								if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
									item->DeleteMe();
							}
						}

						if(qst->srcitem && qst->srcitem != qst->receive_items[0])
						{
							Item * item = objmgr.CreateItem( qst->srcitem, plr);
							if(item)
							{
								item->SetStackCount( qst->srcitemcount ? qst->srcitemcount : 1);
								if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
									item->DeleteMe();
							}
						}
					

						//if(qst->count_required_item || qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)	// gameobject quests deactivate
						//	plr->UpdateNearbyGameObjects();
						//ScriptSystem->OnQuestEvent(qst, static_cast< Creature* >( qst_giver ), _player, QUEST_EVENT_ON_ACCEPT);
					
						sHookInterface.OnQuestAccept( plr, qst, NULL );

						lua_pushnumber(L, 3);
						return 1;
					}
				}
			}
		}
		else
		{
			return 0;
		}
		
		lua_pushnumber(L, 999);
		return 1;
	} //StartQuest

	int UnlearnSpell(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int spellid = luaL_checkint(L,1);
		TO_PLAYER(ptr)->removeSpell(spellid,false,false,0);
		return 0;
	}

	int LearnSpell(lua_State * L, Unit* ptr)
	{
		TEST_PLAYER()
		int spellid = luaL_checkint(L,1);
		TO_PLAYER(ptr)->addSpell(spellid);
		return 0;
	}
	int LearnSpells(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		if(!strcmp("table",luaL_typename(L,1)) )
		{
			int table = lua_gettop(L);
			lua_pushnil(L);
			while(lua_next(L,table) != 0)
			{
				if(lua_isnumber(L,-1) )
					TO_PLAYER(ptr)->addSpell(CHECK_ULONG(L,-1));
				lua_pop(L,1);
			}
			lua_pushboolean(L,1);
			lua_replace(L,table);
			lua_settop(L,table); // Paroxysm: The stack should be balanced but just in case.
		}
		else
		{
			lua_settop(L,0);
			lua_pushboolean(L,0);
		}
		return 1;
	}

	int MarkQuestObjectiveAsComplete(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int questid = luaL_checkint(L,1);
		int objective = luaL_checkint(L,2);
		Player * pl = TO_PLAYER(ptr);
		if (!pl->HasFinishedQuest(questid)) 
		{
			QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
			qle->SetMobCount(objective, qle->GetQuest()->required_mobcount[objective]);
			qle->SendUpdateAddKill(objective);
			if (qle->CanBeFinished())
			{
				qle->SendQuestComplete();
				qle->UpdatePlayerFields();
			}
		}
		return 0;
	}

	int SendAreaTriggerMessage(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char * msg = luaL_checkstring(L,1);
		if (!msg) return 0;
		TO_PLAYER(ptr)->SendAreaTriggerMessage(msg);
		return 0;
	}

	int SendBroadcastMessage(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char * msg = luaL_checkstring(L,1);
		if (!msg) return 0;
		TO_PLAYER(ptr)->BroadcastMessage(msg);
		return 0;
	}

	int TeleportUnit(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 mapId = CHECK_ULONG(L, 1);
		float posX = CHECK_FLOAT(L, 2);
		float posY = CHECK_FLOAT(L, 3);
		float posZ = CHECK_FLOAT(L, 4);
		float Orientation = CHECK_FLOAT(L,5);
		if(!posX || !posY || !posZ || !mapId) 
		{
			if (mapId) 
			{
				Log.Notice("LuaEngineMgr", "LUATeleporter ERROR - Wrong Coordinates given (Map, X, Y, Z) :: Map%f%s%f%s%f%s%u", mapId, " X", posX, " Y", posY, " Z", posZ);
				return 0;
			} 
			else 
				mapId = 0; //MapId is false reported as empty if you use Eastern Kingdoms (0) So lets override it IF it is reported as empty.
		}
		LocationVector vec(posX,posY,posZ,Orientation);
		TO_PLAYER(ptr)->SafeTeleport(mapId,0,vec);
		return 0;
	}

	int GetHealth(lua_State * L, Unit * ptr)
	{
		if (!ptr)
			lua_pushinteger( L, 0 );
		else
			lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) );
		return 1;
	}

	int GetMaxHealth(lua_State * L, Unit * ptr)
	{
		if (!ptr)
			lua_pushinteger( L, 0 );
		else
			lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

		return 1;
	}

	int SetHealth(lua_State * L, Unit * ptr)
	{
		int val = luaL_checkint( L, 1 );
		if( ptr != NULL && val > 0 )
		{
			if( (uint32)val > ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
				ptr->SetHealth(ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			else
				ptr->SetHealth(val );
		}
		return 0;
	}

	int SetMaxHealth(lua_State * L, Unit * ptr)
	{
		int val = luaL_checkint( L, 1 );
		if( ptr != NULL && val > 0 )
		{
			if( (uint32)val < ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) )
				ptr->SetHealth(val );
			ptr->SetUInt32Value( UNIT_FIELD_MAXHEALTH, val );
		}
		return 0;
	}

	int WipeHateList(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		TO_AIMOB(ptr->GetAIInterface() )->wipeHateList();
		return 0;
	}

	int WipeTargetList(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		TO_AIMOB(ptr->GetAIInterface() )->wipeHateList();
		return 0;
	}

	int WipeCurrentTarget(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		TO_AIMOB(ptr->GetAIInterface() )->wipeHateList();
		return 0;
	}

	int GetPlayerClass(lua_State * L, Unit * ptr)
	{
	   if(!ptr || ptr->GetTypeId() != TYPEID_PLAYER) 
	   {
		  lua_pushstring(L, "Unknown");
		  return 1;
	   }
	   int plrclass = TO_PLAYER(ptr)->getClass();

	   switch(plrclass) 
	   {
		  case 1:
			 lua_pushstring(L, "Warrior");
			 break;
		  case 2:
			 lua_pushstring(L, "Paladin");
			 break;
		  case 3:
			 lua_pushstring(L, "Hunter");
			 break;
		  case 4:
			 lua_pushstring(L, "Rogue");
			 break;
		  case 5:
			 lua_pushstring(L, "Priest");
			 break;
		  case 6:
			 lua_pushstring(L, "Death Knight");
			 break;
		  case 7:
			 lua_pushstring(L, "Shaman");
			 break;
		  case 8:
			 lua_pushstring(L, "Mage");
			 break;
		  case 9:
			 lua_pushstring(L, "Warlock");
			 break;
		  //case 10:
		  case 11:
			 lua_pushstring(L, "Druid");
			 break;
		  default:
			 lua_pushstring(L, "Unknown");
			 break;
	   }

	   return 1;
	}


	int ClearHateList(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		ptr->ClearHateList();
		return 0;
	}

	int SetMana(lua_State * L, Unit * ptr)
	{
		int val = luaL_checkint( L, 1);
		if (ptr != NULL)
			ptr->SetUInt32Value( UNIT_FIELD_POWER1, val );
		return 0;
	}

	int SetMaxMana(lua_State * L, Unit * ptr)
	{
		int val = luaL_checkint( L, 1);
		if (ptr != NULL && val > 0)
		{
			if( (uint32)val < ptr->GetPower(POWER_TYPE_MANA) )
				ptr->SetPower(POWER_TYPE_MANA, val);
			ptr->SetMaxPower(POWER_TYPE_MANA, val);
		}
		return 1;
	}

	int GetPlayerRace(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		lua_pushinteger( L, TO_PLAYER(ptr)->getRace() );
		return 1;
	}

	int SetFlying(lua_State * L, Unit * ptr)
	{
		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << ptr->GetNewGUID();
		data << uint32(0);
		ptr->SendMessageToSet(&data, true);
		ptr->GetAIInterface()->disableMelee();
		ptr->GetAIInterface()->setAIMoveFlags(AIMOVESTATE_FLY);
		ptr->Emote(EMOTE_ONESHOT_LIFTOFF);
		return 0;
	}
	 
	int Land(lua_State * L, Unit * ptr)
	{
		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << ptr->GetNewGUID();
		data << uint32(0);
		ptr->SendMessageToSet(&data, true);
		ptr->GetAIInterface()->disableMelee(false);
		ptr->GetAIInterface()->setAIMoveFlags(AIMOVESTATE_RUN);
		ptr->Emote(EMOTE_ONESHOT_LAND);
		return 0;
	}

	int HasAura(lua_State * L, Unit * ptr)
	{
		int spellid = luaL_checkint(L,1);
		if (!ptr || !spellid)
			return 0;
		else
		{
			if(ptr->HasAura(spellid))
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L,0);
		}
		return 1;
	}

	int ReturnToSpawnPoint(lua_State * L, Unit * ptr)
	{
		float x = ptr->GetSpawnX();
		float y = ptr->GetSpawnY();
		float z = ptr->GetSpawnZ();
		float o = ptr->GetSpawnO();
		if (ptr)
			ptr->GetAIInterface()->MoveTo(x, y, z, o);
		return 0;
	}

	int GetGUID(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		PUSH_GUID(L,ptr->GetGUID());
		return 1;
	}

	int GetDistance(lua_State * L, Unit * ptr)
	{
		if (!ptr)
			return 0;
		Object * target = CHECK_OBJECT(L, 1);
		lua_pushnumber(L,(float)ptr->GetDistance2dSq(target) );
		return 1;
	}

	int GetDistanceYards(lua_State * L, Unit * ptr)
	{
		Object * target = CHECK_OBJECT(L, 1);
		if(!ptr || !target)
			return 0;
		LocationVector vec = ptr->GetPosition();
		lua_pushnumber(L,(float)vec.Distance(target->GetPosition()));
		return 1;
	}

	int GetDuelState(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		lua_pushnumber(L,plr->GetDuelState());
		/*Returns
		  DUEL_STATE_REQUESTED = 0
		  DUEL_STATE_STARTED = 1
		  DUEL_STATE_FINISHED = 2 (Default)
		*/
		return 1;
	}

	int GetCreatureNearestCoords(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 entryid = CHECK_ULONG(L,4);
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		PUSH_UNIT(L,ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x, y, z, entryid));
		return 1;
	}

	int GetGameObjectNearestCoords(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 entryid = CHECK_ULONG(L,4);
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		PUSH_GO(L,ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x, y, z, entryid));
		return 1;
	}

	int SetPosition(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		float o = CHECK_FLOAT(L,4);
		ptr->SetFacing(o);
		ptr->SetOrientation(o);

		WorldPacket data(SMSG_MONSTER_MOVE, 50);
		data << ptr->GetNewGUID();
		data << uint8(0);
		data << ptr->GetPositionX();
		data << ptr->GetPositionY();
		data << ptr->GetPositionZ();
		data << getMSTime();
		data << uint8(0x00);
		data << uint32(256);
		data << uint32(1);
		data << uint32(1);
		data << x << y << z;

		ptr->SendMessageToSet(&data, true);
		ptr->SetPosition(x,y,z,o,true);
		return 0;
	}

	int GetLandHeight(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		if (!ptr || !x || !y) 
			return 0;
		float lH = ptr->GetMapMgr()->GetLandHeight(x,y);
		lua_pushnumber(L, lH);
		return 1;
	}

	int IsInPhase(lua_State * L, Unit * ptr)
	{
		uint32 phase = luaL_checkint(L,1);
		lua_pushboolean(L, ((ptr->m_phase & phase) != 0) ? 1 : 0);
		return 1;
	}

	int HasFlag(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		uint32 index = CHECK_ULONG(L,1);
		uint32 flag = CHECK_ULONG(L,2);
		lua_pushboolean(L, ptr->HasFlag(index,flag) ? 1 : 0);
		return 1;
	}

	int QuestAddStarter(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * unit = TO_CREATURE(ptr);
		uint32 quest_id = (uint32)luaL_checknumber(L, 1);
		if (!unit->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER))
			unit->SetUInt32Value(UNIT_NPC_FLAGS, unit->GetUInt32Value(UNIT_NPC_FLAGS)+UNIT_NPC_FLAG_QUESTGIVER);
		if(!quest_id)
			return 0;

		Quest * qst = QuestStorage.LookupEntry(quest_id);

		if (!qst)
			return 0;

		uint32 quest_giver = unit->GetEntry();
		
		char my_query1 [200];
		sprintf(my_query1,"SELECT id FROM creature_quest_starter WHERE id = %d AND quest = %d", quest_giver, quest_id);
		QueryResult *selectResult1 = WorldDatabase.Query(my_query1);
		if (selectResult1)
			delete selectResult1; //already has quest
		else
		{
			char my_insert1 [200];
			sprintf(my_insert1, "INSERT INTO creature_quest_starter (id, quest) VALUES (%d,%d)", quest_giver, quest_id);
			WorldDatabase.Execute(my_insert1);
		}
		sQuestMgr.LoadExtraQuestStuff();

		QuestRelation *qstrel = new QuestRelation;
		qstrel->qst = qst;
		qstrel->type = QUESTGIVER_QUEST_START;

		uint8 qstrelid;
		if ( unit->HasQuests() )
		{
			qstrelid = (uint8)unit->GetQuestRelation(quest_id);
			unit->DeleteQuest(qstrel);
		}
		unit->_LoadQuests();
		return 0;
	}

	int QuestAddFinisher(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * unit = TO_CREATURE(ptr);
		uint32 quest_id = CHECK_ULONG(L, 1);
		if (!unit->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER))
			unit->SetUInt32Value(UNIT_NPC_FLAGS, unit->GetUInt32Value(UNIT_NPC_FLAGS)+UNIT_NPC_FLAG_QUESTGIVER);
		if(!quest_id)
			return 0;
		Quest * qst = QuestStorage.LookupEntry(quest_id);

		if (!qst)
			return 0;

		uint32 quest_giver = unit->GetEntry();
		
		char my_query1 [200];
		sprintf(my_query1,"SELECT id FROM creature_quest_finisher WHERE id = %d AND quest = %d", quest_giver, quest_id);
		QueryResult *selectResult1 = WorldDatabase.Query(my_query1);
		if (selectResult1)
		{
			delete selectResult1; //already has quest
		}
		else
		{
			char my_insert1 [200];
			sprintf(my_insert1, "INSERT INTO creature_quest_finisher (id, quest) VALUES (%d,%d)", quest_giver, quest_id);
			WorldDatabase.Execute(my_insert1);
		}
		sQuestMgr.LoadExtraQuestStuff();

		QuestRelation *qstrel = new QuestRelation;
		qstrel->qst = qst;
		qstrel->type = QUESTGIVER_QUEST_END;

		uint8 qstrelid;
		if ( unit->HasQuests() )
		{
			qstrelid = (uint8)unit->GetQuestRelation(quest_id);
			unit->DeleteQuest(qstrel);
		}
		unit->_LoadQuests();
		return 0;
	}

	int CastSpellAoF(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		uint32 sp = CHECK_ULONG(L, 4);
		if(!sp || !ptr)
			return 0;
		ptr->CastSpellAoF(x,y,z,dbcSpell.LookupEntry(sp), true);
		return 0;
	}

	int FullCastSpellAoF(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		uint32 sp = CHECK_ULONG(L, 4);
		if (!sp || !ptr)
			return 0;
		ptr->CastSpellAoF(x,y,z,dbcSpell.LookupEntry(sp), false);
		return 0;
	}

	int SetInFront(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		if (!target || !ptr)
			return 0;
		ptr->setInFront(target);
		return 0;
	}

	int RemoveAllAuras(lua_State *L, Unit * ptr)
	{
		if (!ptr)
			return 0;
		ptr->RemoveAllAuras();
		return 0;
	}

	int CancelSpell(lua_State * L, Unit * ptr)
	{
		if(!ptr)
			return 0;
		ptr->CancelSpell(ptr->GetCurrentSpell());
		return 0;
	}

	int IsAlive(lua_State * L, Unit * ptr)
	{
		if(ptr)
		{
			if(ptr->IsAlive())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int IsDead(lua_State * L, Unit * ptr)
	{
		if(ptr)
		{
			if(ptr->IsAlive())
				lua_pushboolean(L, 0);
			else
				lua_pushboolean(L, 1);
		}
		return 1;
	}

	int IsInWorld(lua_State * L, Unit * ptr)
	{
		if (ptr)
		{
			if(ptr->IsInWorld())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int GetZoneId(lua_State *L, Unit * ptr)
	{
		if(!ptr)
			return 0;
		lua_pushinteger(L,(ptr->GetZoneId()));
		return 1;
	}

	int Root(lua_State * L, Unit * ptr)
	{
		if(ptr)
			ptr->Root();
		return 0;
	}

	int Unroot(lua_State * L, Unit * ptr)
	{
		if(ptr)
			ptr->Unroot();
		return 0;
	}

	int IsCreatureMoving(lua_State * L, Unit * ptr)
	{
		/*if(ptr && ptr->GetTypeId() == TYPEID_UNIT)
		{
			if(ptr->GetAIInterface()->m_creatureState == MOVING)
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}*/
		lua_pushboolean(L, 0);
		return 1;
	}

	int SetOutOfCombatRange(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int range = luaL_checkint(L, 1);
		if (range)
			TO_AIMOB(ptr->GetAIInterface())->setAutoWipeRange(range);
		return 0;
	}

	int ModifyRunSpeed(lua_State * L, Unit * ptr)
	{
		float Speed = CHECK_FLOAT(L, 1);
		if (ptr && Speed)
			ptr->m_runSpeed = Speed;
		return 0;
	}

	int ModifyWalkSpeed(lua_State * L, Unit * ptr)
	{
		float Speed = CHECK_FLOAT(L,1);
		if (ptr && Speed)
			ptr->m_walkSpeed = Speed;
		return 0;
	}

	int ModifyFlySpeed(lua_State * L, Unit * ptr)
	{
		float Speed = CHECK_FLOAT(L,1);
		if(ptr && Speed)
			ptr->m_flySpeed = Speed;
		return 0;
	}

	int IsFlying(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		if(ptr->GetAIInterface()->isFlying())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int SetRotation(lua_State * L, Unit * ptr)
	{
		ptr->SetRotation(ptr->GetGUID());
		return 0;
	}

	int SetOrientation(lua_State * L, Unit * ptr)
	{
		float O = CHECK_FLOAT(L, 1);
		if(ptr)
			ptr->SetOrientation(O);
		return 0;
	}

	int GetSpawnX(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnX());
		return 1;
	}

	int GetSpawnY(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnY());
		return 1;
	}

	int GetSpawnZ(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnZ());
		return 1;
	}

	int GetSpawnO(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnO());
		return 1;
	}

	int GetInRangePlayersCount(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetInRangePlayersCount());
		return 1;
	}

	int GetEntry(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetEntry());
		return 1;
	}

	int SetMoveRunFlag(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int enabled = luaL_checkint(L, 1);

		ptr->GetAIInterface()->setAIMoveFlags ((enabled > 0) ? AIMOVESTATE_RUN : AIMOVESTATE_WALK);
		return 0;
	}

	int HandleEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		int event_id = luaL_checkint(L, 2);
		int misc_1 = luaL_checkint(L, 3);
		ptr->GetAIInterface()->HandleEvent(event_id, target, misc_1);
		return 1;
	}

	int GetCurrentSpellId(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if (ptr->GetCurrentSpell())
			lua_pushnumber(L, ptr->GetCurrentSpell()->GetProto()->Id);
		else 
			lua_pushnil(L);
		return 1;
	}

	int GetCurrentSpell(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if(ptr->GetCurrentSpell() != 0)
			PUSH_SPELL(L,ptr->GetCurrentSpell());
		else
			lua_pushnil(L);
		return 1;
	}

	int AddAssistTargets(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * Friend = CHECK_UNIT(L,1);
		if(Friend)
		{
			if( isFriendly(ptr, Friend))
				TO_AIMOB(ptr->GetAIInterface() )->addAssistTarget(Friend);
		}
		return 0;
	}

	int GetAIState(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		lua_pushnumber(L, ptr->GetAIInterface()->getAIState());
		return 1;
	}

	int GetFloatValue(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		if (ptr) 
			lua_pushnumber(L, ptr->GetFloatValue(field));
		return 1;
	}

	int SendPacket(lua_State * L, Unit * ptr)
	{
		WorldPacket * data = CHECK_PACKET(L,1);
		int self = lua_toboolean(L,2);
		if (data && ptr)
			ptr->SendMessageToSet(data,(self > 0 ) ? true : false);
		return 0;
	}

	int SendPacketToGroup(lua_State * L, Unit * ptr)
	{
		WorldPacket * data = CHECK_PACKET(L,1);
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (!data) return 0;
		plr->GetGroup()->SendPacketToAll(data);
		return 0;
	}

	int SendPacketToPlayer(lua_State * L, Unit * ptr)
	{
		WorldPacket * data = CHECK_PACKET(L,1);
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (data)
			plr->GetSession()->SendPacket(data);
		return 0;
	}

	int ModUInt32Value(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		int value = luaL_checkint(L, 2);
		if (ptr) 
			ptr->ModSignedInt32Value(field, value);
		return 0;
	}

	int ModFloatValue(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		float value = CHECK_FLOAT(L, 2);
		if (ptr) 
			ptr->ModFloatValue(field, value);
		return 0;
	}

	int SetUInt32Value(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		int value = luaL_checkint(L, 2);
		if (ptr) 
			ptr->SetUInt32Value(field, value);
		return 0;
	}

	int SetUInt64Value(lua_State * L, Unit * ptr)
	{
		uint32 field = CHECK_ULONG(L,1);
		uint64 guid = CHECK_GUID(L,2);
		if (ptr) 
			ptr->SetUInt64Value(field, guid);
		return 0;
	}

	int RemoveFlag(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L,1);
		int value = luaL_checkint(L,2);
		if (ptr)
			ptr->RemoveFlag(field,value);
		return 0;
	}

	int SetFlag(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L,1);
		int value = luaL_checkint(L,2);
		if (ptr)
			ptr->SetFlag(field,value);
		return 0;
	}

	int SetFloatValue(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		float value = CHECK_FLOAT(L, 2);
		if (ptr) 
			ptr->SetFloatValue(field, value);
		return 0;
	}

	int GetUInt32Value(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		if (ptr) 
			lua_pushnumber(L, ptr->GetUInt32Value(field));
		return 1;
	}

	int GetUInt64Value(lua_State * L, Unit * ptr)
	{
		int field = luaL_checkint(L, 1);
		if (ptr)
			PUSH_GUID(L, ptr->GetUInt64Value(field));
		return 1;
	}

	int AdvanceQuestObjective(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int questid = luaL_checkint(L,1);
		int objective = luaL_checkint(L,2);
		Player * pl = TO_PLAYER(ptr);
		QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
		if(qle != NULL)
		{
			qle->SetMobCount(objective, qle->GetMobCount(objective) + 1);
			qle->SendUpdateAddKill(objective);
			if (qle->CanBeFinished())
				qle->SendQuestComplete();

			qle->UpdatePlayerFields();
		}
		return 0;
	}

	int GetManaPct(lua_State * L, Unit * ptr)
	{
		if(!ptr) 
			return 0;
		if (ptr->GetPowerType() == (uint8)POWER_TYPE_MANA)
			lua_pushnumber(L, (int)(ptr->GetPower(POWER_TYPE_MANA) * 100.0f / ptr->GetMaxPower(POWER_TYPE_MANA)));
		else
			lua_pushnil(L);
		return 1;
	}

	int GetPowerPct(lua_State * L, Unit * ptr)
	{
		if (!ptr)
		{
			lua_pushnil(L);
			return 1;
		}
		int powertype = luaL_optint(L, 1, -1);
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		lua_pushnumber(L, (int)(ptr->GetPower(powertype) * 100.0f / ptr->GetMaxPower(powertype)));
		return 1;
	}

	int GetMana(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL )
			lua_pushinteger( L, 0 );
		else
			lua_pushinteger( L, ptr->GetPower(POWER_TYPE_MANA) );

		return 1;
	}

	int GetPower(lua_State * L, Unit * ptr)
	{
		if (!ptr)
		{
			lua_pushnil(L);
			return 1;
		}
		int powertype = luaL_optint(L, 1, -1);
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		lua_pushnumber(L, ptr->GetPower(powertype));
		return 1;
	}

	int GetMaxMana(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL )
			lua_pushinteger( L, 0 );
		else
			lua_pushinteger( L, ptr->GetMaxPower(POWER_TYPE_MANA) );

		return 1;
	}

	int GetMaxPower(lua_State * L, Unit * ptr)
	{
		if (!ptr)
		{
			lua_pushnil(L);
			return 1;
		}
		int powertype = luaL_optint(L, 1, -1);
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		lua_pushnumber(L, ptr->GetMaxPower(powertype));
		return 1;
	}


	int SetPowerType(lua_State * L, Unit * ptr)
	{/* POWER_TYPE_MANA         = 0,
		POWER_TYPE_RAGE         = 1,
		POWER_TYPE_FOCUS        = 2,
		POWER_TYPE_ENERGY       = 3,
		POWER_TYPE_HAPPINESS    = 4,
		POWER_TYPE_RUNES        = 5,
		POWER_TYPE_RUNIC_POWER  = 6 */
		int type = luaL_checkint(L, 1);
		if(!ptr || type < 0)
			return 0;
		ptr->SetPowerType(type);
		return 0;
	}

	int SetMaxPower(lua_State * L, Unit * ptr)
	{
		int amount = luaL_checkint(L,1);
		int powertype = luaL_optint(L, 2, -1);
		if (!ptr || amount < 0) return 0;
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		ptr->SetMaxPower(powertype, amount);
		return 0;
	}

	int SetPower(lua_State * L, Unit * ptr)
	{
		int amount = luaL_checkint(L,1);
		int powertype = luaL_optint(L, 2, -1);
		if (!ptr || amount < 0) return 0;
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		ptr->SetPower(powertype, amount);
		return 0;
	}

	int SetPowerPct(lua_State * L, Unit * ptr)
	{
		int amount = luaL_checkint(L,1);
		int powertype = luaL_optint(L, 2, -1);
		if (!ptr || amount < 0) return 0;
		if (powertype == -1)
			powertype = ptr->GetPowerType();
		ptr->SetPower( powertype, (int)(amount/100) * (ptr->GetMaxPower(powertype)) );
		return 0;
	}


	int GetPowerType(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushinteger(L, ptr->GetPowerType());
		return 1;
	}

	int Strike(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET();

		Unit * target = CHECK_UNIT(L, 1);
		uint32 weapon_damage_type = luaL_checkint(L, 2);
		uint32 sp = CHECK_ULONG(L, 3);
		int32 adddmg = luaL_checkint(L, 4);
		uint32 exclusive_damage = CHECK_ULONG(L, 5);
		int32 pct_dmg_mod = luaL_checkint(L, 6);

		if (!target)
			return 0;
		ptr->Strike(target,weapon_damage_type,dbcSpell.LookupEntry(sp),adddmg,pct_dmg_mod,exclusive_damage,false,false);
		return 0;
	}

	int SetAttackTimer(lua_State * L, Unit * ptr)
	{
		int32 timer = luaL_checkint(L, 1);
		bool offhand = CHECK_BOOL(L,2);
		if(!timer || !ptr)
			return 0;
		ptr->setAttackTimer(timer,offhand);
		return 0;
	}

	int Kill(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		if (!ptr || !target)
			return 0;
		ptr->DealDamage(target,target->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0,true);
		return 0;
	}

	int DealDamage(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		uint32 damage = CHECK_ULONG(L, 2);
		uint32 spellid = CHECK_ULONG(L, 3);
		if(!ptr || !target)
			return 0;
		ptr->DealDamage(target,damage,0,0,spellid,true);
		return 0;
	}

	int SetNextTarget(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		if (ptr && target)
			ptr->GetAIInterface()->setNextTarget(target);
		return 0;
	}

	int GetNextTarget(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		PUSH_UNIT(L, ptr->GetAIInterface()->getNextTarget());
		return 1;
	}

	int SetPetOwner(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * owner = CHECK_UNIT(L, 1);
		if (owner)
			ptr->GetAIInterface()->setOwner(owner);
		return 0;
	}

	int DismissPet(lua_State * L, Unit * ptr)
	{
		/* TODO: */
		TEST_UNIT()
			TO_PET(ptr)->Dismiss();
		return 0;
	}

	int IsPet(lua_State * L, Unit * ptr)
	{
		if (ptr)
		{
			if (ptr->IsPet())
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int GetPetOwner(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		PUSH_UNIT(L, ptr->GetAIInterface()->getOwner());
		return 1;
	}

	int SetUnitToFollow(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		float dist = CHECK_FLOAT(L, 2);
		float angle = CHECK_FLOAT(L, 3);
		if(!ptr || !dist || !angle)
			return 0;
		ptr->GetAIInterface()->setUnitToFollow( (target != NULL) ? target->GetGUID() : 0 );
		ptr->GetAIInterface()->setFollowDistance(dist);
		ptr->GetAIInterface()->setFollowAngle(angle);
		return 0;
	}

	int GetUnitToFollow(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		PUSH_UNIT(L,ptr->GetAIInterface()->getUnitToFollow());
		return 1;
	}

	int IsInFront(lua_State * L, Unit * ptr)
	{
		Object * target = CHECK_OBJECT(L, 1);
		if (ptr && target)
		{
			if(ptr->isInFront(target))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int IsInBack(lua_State * L, Unit * ptr)
	{
		Object * target = CHECK_OBJECT(L, 1);
		if (ptr && target)
		{
			if(ptr->isInBack(target))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int SetPacified(lua_State * L, Unit * ptr)
	{
		bool pacified = CHECK_BOOL(L, 1);
		if (!ptr)
			return 0;
		ptr->m_pacified = pacified ? 1 : 0;
		if (pacified)
			ptr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
		else
			ptr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
		return 0;
	}

	int CreateGuardian(lua_State * L, Unit * ptr)
	{
		uint32 guardian_entry = CHECK_ULONG(L,1);
		uint32 duration = CHECK_ULONG(L, 2);
		float angle = CHECK_FLOAT(L, 3);
		uint32 lvl  = CHECK_ULONG(L, 4);
		if (!ptr || !guardian_entry || !lvl)
			return 0;
		PUSH_UNIT(L, TO_UNIT(ptr->create_guardian(guardian_entry,duration,angle,lvl,NULL)));
		return 1;
	}

	int IsInArc(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L, 1);
		float degrees = CHECK_FLOAT(L, 2);
		if (!target || !ptr || !degrees)
			return 0;
		else
		{
			if (ptr->isInArc(target,degrees))
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L,0);
		}
		return 1;
	}

	int IsInWater(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		if (ptr)
		{
			if (TO_PLAYER(ptr)->m_UnderwaterState)
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}
		
	int GetAITargetsCount(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		lua_pushnumber(L,  TO_AIMOB( ptr->GetAIInterface())->getAITargetsCount());
		return 1;
	}

	int GetUnitByGUID(lua_State * L, Unit * ptr)
	{
		uint64 guid  = CHECK_GUID(L,1);
		if (ptr && guid)
			PUSH_UNIT(L,ptr->GetMapMgr()->GetUnit(guid));
		return 1;
	}

	int GetAITargets(lua_State * L, Unit * ptr)
	{
		/*TEST_UNIT()
		Unit * ret = NULL;
		TargetMap::iterator itr;
		lua_newtable(L);
		int count = 0;
		for (itr = ptr->GetAIInterface()->GetAITargets()->begin(); itr!= ptr->GetAIInterface()->GetAITargets()->end(); itr++)
		{
			ret = ptr->GetMapMgr()->GetUnit(itr->first);
			count++;
			lua_pushvalue(L,count);
			PUSH_UNIT(L,ret);
			lua_rawset(L,-3);
		}*/
		lua_pushnil(L);
		return 1;
	}

	int GetInRangeObjectsCount(lua_State * L, Unit * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetInRangeCount());
		return 1;
	}

	int GetInRangePlayers(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 count = 0;
		lua_newtable(L);
		for(std::set< Object* >::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
		{
			if( (*itr) ->GetTypeId() == TYPEID_PLAYER)
			{
				count++,
				lua_pushinteger(L,count);
				PUSH_UNIT(L,*itr);
				lua_rawset(L,-3);
			}
		}
		return 1;
	}

	int GetGroupPlayers(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * _player = TO_PLAYER(ptr);
		Group* party = _player->GetGroup();
		uint32 count = 0;
		lua_newtable(L);
		if (party)
		{
			GroupMembersSet::iterator itr;
			SubGroup * sgrp;
			party->getLock().Acquire();
			for(uint32 i = 0; i < party->GetSubGroupCount(); i++)
			{
				sgrp = party->GetSubGroup(i);
				for(itr = sgrp->GetGroupMembersBegin(); itr != sgrp->GetGroupMembersEnd(); ++itr)
				{
					if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetZoneId() == _player->GetZoneId() && _player->GetInstanceID() == (*itr)->m_loggedInPlayer->GetInstanceID())
					{
						count++,
						lua_pushinteger(L,count);
						PUSH_UNIT(L,(*itr)->m_loggedInPlayer);
						lua_rawset(L,-3);
					}
				}
			}
			party->getLock().Release();		
		}
		return 1;
	}

	int GetDungeonDifficulty(lua_State * L, Unit * ptr)
	{	/*
		MODE_NORMAL_10MEN	=	0,
		MODE_NORMAL_25MEN	=	1,
		MODE_HEROIC_10MEN	=	2,
		MODE_HEROIC_25MEN	=	3
		*/
		if (ptr->GetTypeId() == TYPEID_PLAYER)
		{
			Player * plr = TO_PLAYER(ptr);
			if (plr->GetGroup())
			{
				if (plr->GetGroup()->GetGroupType() == GROUP_TYPE_PARTY)
					lua_pushnumber(L,plr->GetGroup()->m_difficulty);
				else
					lua_pushnumber(L,plr->GetGroup()->m_raiddifficulty);
			}
			else
			{
				if (!plr->IsInInstance())
					return 0;
				Instance * pInstance = sInstanceMgr.GetInstanceByIds(plr->GetMapId(), plr->GetInstanceID());
				lua_pushinteger(L,pInstance->m_difficulty);
			}
			return 1;
		}
		else
		{
			if (!ptr->IsInInstance())
			{	
				lua_pushboolean(L,0);
				return 1;
			}
			Instance * pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
			lua_pushinteger(L,pInstance->m_difficulty);
		}
		return 1;
	}

	int GetInstanceOwner(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if (!ptr->IsInInstance())
			lua_pushnil(L);
		else 
		{
			Instance * pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
			if (pInstance->m_creatorGuid != 0) // creator guid is 0 if its owned by a group.
			{
				Player * owner = pInstance->m_mapMgr->GetPlayer(pInstance->m_creatorGuid);
				PUSH_UNIT(L,owner);
			}
			else
			{
				uint32 gId = pInstance->m_creatorGroup;
				PUSH_UNIT(L,objmgr.GetGroupById(gId)->GetLeader()->m_loggedInPlayer);
			}
		}
		return 1;
	}

	int IsGroupFull(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		lua_pushboolean(L,plr->GetGroup()->IsFull() ? 1 : 0);
		return 1;
	}

	int GetGroupLeader(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		PUSH_UNIT(L,plr->GetGroup()->GetLeader()->m_loggedInPlayer);
		return 1;
	}

	int SetGroupLeader(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * _plr = CHECK_PLAYER(L,1);
		bool silent = CHECK_BOOL(L,2);
		Player * plr = TO_PLAYER(ptr);
		plr->GetGroup()->SetLeader(_plr, silent);
		return 0;
	}

	int AddGroupMember(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * _plr = CHECK_PLAYER(L,1);
		int32 subgroup = luaL_optint(L,2,-1);
		plr->GetGroup()->AddMember(_plr->getPlayerInfo(),subgroup);
		return 0;
	}

	int SetDungeonDifficulty(lua_State * L, Unit * ptr)
	{   /*
		MODE_NORMAL_10MEN	=	0,
		MODE_NORMAL_25MEN	=	1,
		MODE_HEROIC_10MEN	=	2,
		MODE_HEROIC_25MEN	=	3
		*/
		uint32 difficulty = CHECK_ULONG(L,1);
		if (!ptr) return 0;
		if (ptr->IsInInstance()) 
		{
			if (ptr->IsPlayer())
			{
				Player * plr = TO_PLAYER(ptr);
				if (plr->GetGroup())
					(difficulty > 1 ? plr->GetGroup()->m_difficulty : plr->GetGroup()->m_raiddifficulty) = difficulty;
				else
				{
					Instance * pInstance = sInstanceMgr.GetInstanceByIds(plr->GetMapId(), plr->GetInstanceID());
					pInstance->m_difficulty = difficulty;
				}
			}
			else
			{
				Instance * pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
				pInstance->m_difficulty = difficulty;
			}
		}
		return 0;
	}

	int ExpandToRaid(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		plr->GetGroup()->ExpandToRaid();
		return 0;
	}

	int GetInRangeGameObjects(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_newtable(L);
		uint32 count = 0;
		for (std::set<Object*>::iterator itr = ptr->GetInRangeSetBegin();itr!= ptr->GetInRangeSetEnd();itr++)
		{
			if( (*itr) ->GetTypeId() == TYPEID_GAMEOBJECT)
			{
				count++,
				lua_pushinteger(L,count);
				PUSH_GO(L,*itr);
				lua_rawset(L,-3);
			}
		}
		return 1;
	}
		
	int HasInRangeObjects(lua_State * L, Unit * ptr)
	{
		if(ptr)
		{
			if(ptr->HasInRangeObjects())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}

	int SetFacing(lua_State * L, Unit * ptr)
	{
		float newo = CHECK_FLOAT(L, 1);
		if (!ptr)
			return 0;
		ptr->SetFacing(newo);
		return 0;
	}

	int CalcToDistance(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L, 1);
		float y = CHECK_FLOAT(L, 2);
		float z = CHECK_FLOAT(L, 3);
		if (!ptr | !x | !y | !z)
			return 0;
		lua_pushnumber(L,ptr->CalcDistance(x,y,z));
		return 1;
	}

	int CalcAngle(lua_State * L, Unit * ptr)
	{
		float x = CHECK_FLOAT(L,1 );
		float y = CHECK_FLOAT(L, 2);
		float x2 = CHECK_FLOAT(L, 3);
		float y2 = CHECK_FLOAT(L, 4);
		if(!x || !y || !x2 || !y2|| !ptr)
			return 0;
		lua_pushnumber(L,ptr->calcAngle(x,y,x2,y2));
		return 1;
	}

	int CalcRadAngle(lua_State * L, Unit * ptr)
	{
		float ang = 0;
		float x = CHECK_FLOAT(L,1 );
		float y = CHECK_FLOAT(L, 2);
		float dx = CHECK_FLOAT(L, 3);
		float dy = CHECK_FLOAT(L, 4);
		if (!x || !y || !dx|| !dy || !ptr)
			return 0;
		else
		{
			ang = ptr->calcRadAngle(x,y,dx,dy);
			lua_pushnumber(L,ang);
		}
		return 1;
	}

	int IsInvisible(lua_State * L, Unit * ptr) //THIS IS NOT "IS" IT'S SET!
	{
		if (!ptr) return 0;
		bool enabled = CHECK_BOOL(L,1);
		if (enabled) 
		{
			ptr->m_invisFlag = INVIS_FLAG_TOTAL;
			ptr->m_invisible = true;
		}
		else
		{
			ptr->m_invisFlag = INVIS_FLAG_NORMAL;
			ptr->m_invisible = false;
		}
		return 0;
	}

	int MoveFly(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		bool enabled = CHECK_BOOL(L,1);
		ptr->GetAIInterface()->setAIMoveFlags( (enabled ) ? AIMOVESTATE_FLY : AIMOVESTATE_RUN );
		return 0;
	}

	int IsInvincible(lua_State * L, Unit * ptr) //THIS IS NOT "IS" IT'S SET!
	{
		bool enabled = CHECK_BOOL(L,1);
		if (ptr)
			ptr->bInvincible = enabled;
		return 0;
	}

	int ResurrectPlayer(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->RemoteRevive();
		return 0;
	}

	int KickPlayer(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int delay = luaL_checkint(L,1);
		TO_PLAYER(ptr)->Kick(delay);
		return 0;
	}

	int CanCallForHelp(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		bool enabled = CHECK_BOOL(L,1);
		TO_AIMOB(ptr->GetAIInterface() )->setCallHelpAgent(enabled);
		return 0;
	}

	int CallForHelpHp(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		float hp = CHECK_FLOAT(L, 1);
		ptr->GetAIInterface()->setCallHelpHp( (uint32)hp);
		return 0;
	}

	int SetDeathState(lua_State * L, Unit * ptr)
	{
		int state = luaL_checkint(L, 1);
		if (ptr)
		{
			switch (state)
			{
			case 0:
				ptr->setDeathState(ALIVE);
				break;
			case 1:
				ptr->setDeathState(JUST_DIED);
				break;
			case 2:
				ptr->setDeathState(CORPSE);
				break;
			case 3:
				ptr->setDeathState(DEAD);
				break;
			}
		}
		return 0;
	}

	int SetCreatureName(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		//Paroxysm : This method makes alot of sense...
		/*
		uint32 id = CHECK_ULONG(L,1);
		if(!ptr|!id)
			return 0;
		TO_PLAYER(ptr)->SetCreatureInfo(CreatureNameStorage.LookupEntry(id));
		*/
		return 0;
	}

	int SetBindPoint(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		float x = CHECK_FLOAT(L, 1);
		float y = CHECK_FLOAT(L, 2);
		float z = CHECK_FLOAT(L, 3);
		uint32 map = CHECK_ULONG(L, 4);
		uint32 zone = CHECK_ULONG(L, 5);
		if (!x || !y || !z || !zone)
			return 0;
		plr->SetBindPoint(x, y, z, map, zone);
		return 0;
	}

	int SoftDisconnect(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->SoftDisconnect();
		return 0;
	}

	int Possess(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Unit * target = CHECK_UNIT(L,1);
		if (target)
			TO_PLAYER(ptr)->Possess(target);
		return 0;
	}

	int Unpossess(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->UnPossess();
		return 0;
	}

	int RemoveFromWorld(lua_State * L, Unit * ptr)
	{
		if (ptr)
			ptr->RemoveFromWorld(true);
		return 0;
	}

	int SpellNonMeleeDamageLog(lua_State * L, Unit * ptr)
	{
		Unit * pVictim = CHECK_UNIT(L,1);
		uint32 spellid = CHECK_ULONG(L,2);
		uint32 damage = CHECK_ULONG(L,3);
		bool allowproc = CHECK_BOOL(L,4);
		bool static_dmg = CHECK_BOOL(L,5);
		bool no_remove_auras = CHECK_BOOL(L,6);
		if(pVictim && spellid && damage)
		{
			ptr->SpellNonMeleeDamageLog(pVictim,spellid,damage,allowproc,static_dmg,no_remove_auras);
		}
		return 0;
	}

	int NoRespawn(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		bool enabled = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->m_noRespawn = enabled;
		return 0;
	}
	
	int GetMapId(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushnumber(L, ptr->GetMapId());
		return 1;
	}

	int AttackReaction(lua_State * L, Unit * ptr)
	{
		Unit * target = CHECK_UNIT(L,1);
		uint32 damage = luaL_checkint(L,2);
		uint32 spell = luaL_checkint(L,3);
		if(ptr && target && damage)
			ptr->GetAIInterface()->AttackReaction(target,damage,spell);
		return 0;
	}

	int EventCastSpell(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		Unit * target = CHECK_UNIT(L, 1);
		uint32 sp = CHECK_ULONG(L,2);
		uint32 delay = CHECK_ULONG(L,3);
		uint32 repeats = CHECK_ULONG(L,4);
		if (ptr && sp)
		{
			switch(ptr->GetTypeId())
			{
			case TYPEID_PLAYER:
				sEventMgr.AddEvent(ptr, &Player::EventCastSpell,target,dbcSpell.LookupEntry(sp),EVENT_PLAYER_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				break;
			case TYPEID_UNIT:
				sEventMgr.AddEvent(ptr, &Unit::EventCastSpell,target,dbcSpell.LookupEntry(sp),EVENT_CREATURE_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				break;
			}
		}
		return 0;
	}

	int IsPlayerMoving(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		lua_pushboolean(L, (TO_PLAYER(ptr)->m_isMoving) ? 1 : 0);
		return 1;
	}

	int IsPlayerAttacking(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		if( TO_PLAYER(ptr)->IsAttacking())
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int GetFactionStanding(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 faction = luaL_checkint(L,1);
		if (faction)
		{
			switch (TO_PLAYER(ptr)->GetStandingRank(faction))
			{
			case STANDING_HATED:
				lua_pushstring(L, "Hated");
				break;
			case STANDING_HOSTILE:
				lua_pushstring(L, "Hostile");
				break;
			case STANDING_UNFRIENDLY:
				lua_pushstring(L, "Unfriendly");
				break;
			case STANDING_NEUTRAL:
				lua_pushstring(L, "Neutral");
				break;
			case STANDING_FRIENDLY:
				lua_pushstring(L, "Friendly");
				break;
			case STANDING_HONORED:
				lua_pushstring(L, "Honored");
				break;
			case STANDING_REVERED:
				lua_pushstring(L, "Revered");
				break;
			case STANDING_EXALTED:
				lua_pushstring(L, "Exalted");
				break;
			}
		}
		return 1;
	}

	int SetPlayerAtWar(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		uint32 faction = CHECK_ULONG(L,1);
		bool set = CHECK_BOOL(L,3);
		if (faction)
		{
			TO_PLAYER(ptr)->SetAtWar(faction,set);
		}
		return 0;
	}

	/*int IsPlayerAtWar(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		uint32 faction = luaL_checkint(L,1);
		if (ptr && faction)
			if( ((Player*)ptr)->IsAtWar(faction))
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L,0);
		return 1;
	}*/

	int SetPlayerStanding(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 faction = luaL_checkint(L,1);
		uint32 value = luaL_checkint(L,2);
		if(faction && value)
			TO_PLAYER(ptr)->SetStanding(faction,value);
		return 0;
	}

	int SetPlayerSpeed(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		float Speed = CHECK_FLOAT(L,1);
		if (Speed<1 || Speed>255)
			return 0;
		plr->SetPlayerSpeed(RUN, Speed);
		plr->SetPlayerSpeed(SWIM, Speed);
		plr->SetPlayerSpeed(RUNBACK, Speed / 2);
		plr->SetPlayerSpeed(FLY, Speed * 2);
		return 0;
	}

	int GiveHonor(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 honor = CHECK_ULONG(L,1);
		plr->m_honorToday += honor;
		plr->m_honorPoints += honor;
		plr->RecalculateHonor();
		return 0;
	}

	int TakeHonor(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 honor = CHECK_ULONG(L,1);
		if (plr->m_honorToday-honor < 0 || plr->m_honorPoints-honor < 0)
			return 0;
		plr->m_honorToday -= honor;
		plr->m_honorPoints -= honor;
		plr->RecalculateHonor();
		return 0;
	}

	int GetStanding(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 faction = luaL_checkint(L,1);
		if (faction)
			lua_pushinteger(L, TO_PLAYER(ptr)->GetStanding(faction));
		return 0;
	}

	int RemoveThreatByPtr(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		if (target)
			TO_AIMOB(ptr->GetAIInterface())->removeThreatByPtr(target);
		return 0;
	}

	int HasItem(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 itemid = luaL_checkint(L,1);
		if (itemid)
		{
			if (TO_PLAYER(ptr)->GetItemInterface()->GetItemCount(itemid,false) > 0)
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L,0);
		}
		return 1;
	}

	int PlaySpellVisual(lua_State * L, Unit * ptr)
	{
		uint64 guid = CHECK_GUID(L,1);
		uint32 spell = luaL_checkint(L,2);
		if (ptr && guid && spell)
		{
			WorldPacket data( SMSG_PLAY_SPELL_VISUAL, 12 );
			data << guid;
			data << uint32( spell );
			ptr->SendMessageToSet(&data, ptr->IsPlayer());
		}
		return 1;
	}

	int GetLevel(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushinteger(L, ptr->getLevel());
		return 1;
	}

	int SetLevel(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 level = CHECK_ULONG(L,1);
		if (level <= sWorld.m_levelCap && level > 0)
		{
			if (ptr->IsPlayer())
			{
				LevelInfo * Info = objmgr.GetLevelInfo(ptr->getRace(),ptr->getClass(),level);
				if (Info)
					TO_PLAYER(ptr)->ApplyLevelInfo(Info, level);
			}
			else
				ptr->setLevel(level);
		}
		return 0;
	}

	int AddSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L,1);
		uint32 current = luaL_checkint(L,2);
		uint32 max = luaL_checkint(L,3);
		Player * plr = TO_PLAYER(ptr);
		if (!max) 
			max = 475;
		if (current > max)
			return 0;
		plr->_AddSkillLine(skill,current,max);
		plr->_UpdateMaxSkillCounts();
		return 0;
	}

	int RemoveSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L,1);
		if(!ptr || !skill) 
			return 0;
		Player * plr = TO_PLAYER(ptr);
		plr->_RemoveSkillLine(skill);
		plr->_UpdateMaxSkillCounts();
		return 0;
	}

	int FlyCheat(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		bool enabled = CHECK_BOOL(L,1);
		TO_PLAYER(ptr)->FlyCheat = enabled;
		return 0;
	}

	int AdvanceSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L,1);
		uint32 count = luaL_checkint(L,2);
		Player * plr = TO_PLAYER(ptr);
		if (skill && count)
		{
			if( plr->_HasSkillLine(skill))
				plr->_AdvanceSkillLine(skill,count);
		}
		return 0;
	}

	int RemoveAurasByMechanic(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		uint32 mechanic = luaL_checkint(L,1);
		bool hostileonly = CHECK_BOOL(L,2);
		if(ptr && mechanic)
			ptr->RemoveAllAurasByMechanic(mechanic,-1,hostileonly);
		return 0;
	}

	int RemoveAurasType(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		uint32 type = luaL_checkint(L,1);
		if (ptr && type)
			ptr->RemoveAllAuraType(type);
		return 0;
	}

	int AddAura(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		uint32 spellid = luaL_checkint(L,1);
		uint32 duration = luaL_checkint(L,2);
		bool temp = CHECK_BOOL(L,3);
		if(ptr && spellid)
		{
			Aura * aura = new Aura(dbcSpell.LookupEntry(spellid), duration, ptr, ptr,temp);
			ptr->AddAura(aura);
			lua_pushboolean(L, 1);
		}
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int SetAIState(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 state = luaL_checkint(L,1);
		ptr->GetAIInterface()->setAIState( AI_State(state) );
		return 0;
	}

	int SetStealth(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint32 stealthlevel = CHECK_ULONG(L,1);
		ptr->SetStealth(stealthlevel);
		return 0;
	}

	int GetStealthLevel(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushinteger(L,ptr->GetStealthLevel());
		return 1;
	}

	int IsStealthed(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if(ptr->IsStealth())
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int RemoveStealth(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		ptr->RemoveStealth();
		return 0;
	}

	int InterruptSpell(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		ptr->InterruptSpell();
		return 0;
	}

	int IsPoisoned(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if(ptr->IsPoisoned())
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int RegisterAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 time = luaL_checkint(L,1);
		sEventMgr.AddEvent( TO_CREATURE(ptr), &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT,time,0,0);
		return 0;
	}

	int ModifyAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 newtime = luaL_checkint(L,1);
		sEventMgr.ModifyEventTimeAndTimeLeft(ptr,EVENT_SCRIPT_UPDATE_EVENT,newtime);
		return 0;
	}

	int RemoveAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		sEventMgr.RemoveEvents(ptr, EVENT_SCRIPT_UPDATE_EVENT);
		return 0;
	}

	int deleteWaypoint(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 wp = luaL_checkint(L,1);
		if(wp)
			TO_AIMOB(ptr->GetAIInterface() )->deleteWaypoint( (uint8)wp);
		return 0;
	}

	int DealGoldCost(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		int32 debt = luaL_checkint(L,1);
		if (debt < 0) return 0;
		if (!plr->HasGold(debt))
			lua_pushboolean(L, 0);
		else
		{
			plr->ModGold(-debt);
			lua_pushboolean(L,1);
		}
		return 1;
	}

	int DealGoldMerit(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 profit = CHECK_ULONG(L,1);
		TO_PLAYER(ptr)->ModGold(profit);
		return 0;
	}

	int DeMorph(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		ptr->DeMorph();
		return 0;
	}

	int Attack(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Unit * target = CHECK_UNIT(L, 1);
		if (target)
		{
			ptr->GetAIInterface()->setForcedTarget(target);
			lua_pushboolean(L,1);
		}
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int CanUseCommand(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		char cmdlevel = (char)luaL_checkstring(L,1)[0];
		Player * plr = TO_PLAYER(ptr);
		if (plr->GetSession()->CanUseCommand(cmdlevel))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int GetTarget(lua_State * L, Unit * ptr)
	{
		Log.Notice("LuaEngine", "GetTarget is outdated. Please use GetPrimaryCombatTarget.");
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Unit * target = plr->GetMapMgr()->GetUnit(plr->GetTarget());
		if(target != NULL)
			PUSH_UNIT(L,target);
		else
			lua_pushnil(L);
		return 1;
	}

	int GetSelection(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Unit * selection = plr->GetMapMgr()->GetUnit(plr->GetSelection());
		if (selection)
			PUSH_UNIT(L,selection);
		else
			lua_pushnil(L);
		return 1;
	}
	int GetSelectedGO(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		PUSH_GO(L,plr->GetSelectedGo());
		return 1;
	}

	int RepairAllPlayerItems(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		Player * plr = TO_PLAYER(ptr);
		Item * pItem = NULL;
		Container * pContainer = NULL;
		uint32 j, i;

		for( i = 0; i < MAX_INVENTORY_SLOT; i++ )
		{
			pItem = plr->GetItemInterface()->GetInventoryItem( i );
			if( pItem != NULL )
			{
				if( pItem->IsContainer() )
				{
					pContainer = static_cast<Container*>( pItem );
					for( j = 0; j < pContainer->GetProto()->ContainerSlots; ++j )
					{
						pItem = pContainer->GetItem( j );
						if( pItem != NULL ){
							pItem->SetDurabilityToMax();
						}
					}
				}
				else
				{
					if( pItem->GetProto()->MaxDurability > 0 && i < INVENTORY_SLOT_BAG_END && pItem->GetDurability() <= 0 )
					{
						pItem->SetDurabilityToMax();
						plr->ApplyItemMods( pItem, i, true );
					}
					else
					{
						pItem->SetDurabilityToMax();
					}                    
				}
			}
		}
		return 0;
	}

	int SetKnownTitle(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int title = luaL_checkint(L,1);
		Player * plr = TO_PLAYER(ptr);
		plr->SetKnownTitle(RankTitles(title),true);
		plr->SaveToDB(false);
		return 0;
	}

	int UnsetKnownTitle(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int title = luaL_checkint(L,1);
		Player * plr = TO_PLAYER(ptr);
		plr->SetKnownTitle( RankTitles(title), false );
		plr->SaveToDB(false);
		return 0;
	}

	int LifeTimeKills(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int kills = luaL_checkint(L, 1);
		const char * check = luaL_checklstring(L,2,NULL);
		Player * plr = TO_PLAYER(ptr);
		int killscheck = plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
		if (check && strncmp(check, "add", 4)==0 && kills > 0) 
		{ 
			plr->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, killscheck+kills);
			plr->SaveToDB(false); 
			return 0; 
		}
		else if (check && strncmp(check, "del", 4)==0 && killscheck >= kills) 
		{ 
			plr->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, killscheck-kills);
			plr->SaveToDB(false); 
			return 0; 
		}
		else if (check && strncmp(check, "set", 4)==0 && kills >= 0) 
		{ 
			plr->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, kills); 
			plr->SaveToDB(false); 
			return 0; 
		}
		else if (check == NULL || kills == 0) 
		{ 
			lua_pushinteger( L, killscheck ); 
			return 1; 
		}
		return 0;
	}

	int HasTitle(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int title = luaL_checkint(L, 1);
		Player * plr = TO_PLAYER(ptr);
		if (plr->HasTitle(RankTitles(title)) )
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int GetMaxSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L, 1);
		lua_pushinteger(L, TO_PLAYER(ptr)->_GetSkillLineMax(skill));
		return 1;
	}

	int GetCurrentSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L, 1);
		lua_pushinteger(L, TO_PLAYER(ptr)->_GetSkillLineCurrent(skill));
		return 1;
	}

	int HasSkill(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 skill = luaL_checkint(L, 1);
		lua_pushboolean(L, (TO_PLAYER(ptr)->_HasSkillLine(skill)) ? 1 : 0);
		return 1;
	}

	int GetGuildName(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Guild * pGuild = objmgr.GetGuild(TO_PLAYER(ptr)->GetGuildId());
		if(pGuild != NULL)
			lua_pushstring(L,pGuild->GetGuildName());
		else
			lua_pushnil(L);
		return 1;
	}

	int ClearCooldownForSpell(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		plr->ClearCooldownForSpell(luaL_checkint(L, 1));
		return 0;
	}

	int HasSpell(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 sp = CHECK_ULONG(L,1);
		lua_pushboolean(L, (sp && TO_PLAYER(ptr)->HasSpell(sp)) ? 1 : 0);
		return 1;
	}

	int ClearAllCooldowns(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->ResetAllCooldowns();
		return 0;
	}

	int ResetAllTalents(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->Reset_Talents();
		return 0;
	}

	int GetAccountName(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char * aName = TO_PLAYER(ptr)->GetSession()->GetAccountNameS();
		lua_pushstring(L,aName);
		return 1;
	}

	int GetGmRank(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char * level = TO_PLAYER(ptr)->GetSession()->GetPermissions();
		if(level != NULL)
			lua_pushstring(L,level);
		else
			lua_pushnil(L);
		return 1;
	}

	int IsGm(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		if(TO_PLAYER(ptr)->GetSession()->HasGMPermissions())
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int SavePlayer(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		TO_PLAYER(ptr)->SaveToDB(false);
		return 0;
	}

	int HasQuest(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 quest_id = CHECK_ULONG(L,1);
		if (quest_id && TO_PLAYER(ptr)->HasQuest(quest_id))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int CreatureHasQuest(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * ctr = TO_CREATURE(ptr);
		uint32 questid = CHECK_ULONG(L,1);
		Quest * qst = QuestStorage.LookupEntry(questid);
		if (ctr->HasQuest(qst->id, qst->type))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}

	int RemovePvPFlag(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if(plr != NULL && plr->IsPvPFlagged())
			plr->RemovePvPFlag();
		return 0;
	}

	int RemoveNegativeAuras(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		ptr->RemoveNegativeAuras();
		return 0;
	}

	int GossipMiscAction(lua_State * L, Unit * ptr)
	{
		Player* plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L, 2));
		uint32 miscint = (uint32)luaL_checkint(L, 3);
		int actionid = luaL_checkint(L, 1);
		if ( !plr || (!crc && actionid < 9) ) return 0;
		if (actionid == 1) plr->GetSession()->SendInventoryList(crc); 
		else if (actionid == 2) plr->GetSession()->SendTrainerList(crc);
		else if (actionid == 3) plr->GetSession()->SendInnkeeperBind(crc);
		else if (actionid == 4) plr->GetSession()->SendBankerList(crc);
		else if (actionid == 5) plr->GetSession()->SendBattlegroundList(crc,miscint);
		else if (actionid == 6) plr->GetSession()->SendAuctionList(crc);
		else if (actionid == 7) plr->GetSession()->SendTabardHelp(crc);
		else if (actionid == 8) plr->GetSession()->SendSpiritHealerRequest(crc);
		else if (actionid == 9) plr->SendTalentResetConfirm();
		else if (actionid == 10) plr->SendPetUntrainConfirm();
		return 0;
	}

	int SendVendorWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Creature * object = TO_CREATURE(CHECK_UNIT(L, 1)); //NOT entry. The unit pointer.
		if(plr != NULL && object != NULL)
			plr->GetSession()->SendInventoryList(object);
		return 0;
	}

	int SendTrainerWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L, 1)); //NOT entry. The unit pointer.
		if(crc != NULL)
			plr->GetSession()->SendTrainerList(crc);
		return 0;
	}

	int SendInnkeeperWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L, 1)); //NOT entry. The unit pointer.
		if(crc != NULL)
			plr->GetSession()->SendInnkeeperBind(crc);
		return 0;
	}

	int SendBankWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L, 1)); //NOT entry. The unit pointer.
		if(crc != NULL)
			plr->GetSession()->SendBankerList(crc);
		return 0;
	}

	int SendAuctionWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L, 1)); //NOT entry. The unit pointer.
		if(crc != NULL)
			plr->GetSession()->SendAuctionList(crc);
		return 0;
	}

	int SendBattlegroundWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Creature * crc = TO_CREATURE(CHECK_UNIT(L,1));
		uint32 bgid = luaL_checkint(L,2);
		if(bgid && crc != NULL)
			plr->GetSession()->SendBattlegroundList(crc, bgid); //player filler ftw
		return 0;
	}

	int SendLootWindow(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint64 guid = CHECK_GUID(L,1);
		uint8 loot_type = (uint8)luaL_checkint(L,2);
		uint8 loot_type2 = 1;
		Player * plr = TO_PLAYER(ptr);
		plr->SetLootGUID(guid);
		uint32 guidtype = GET_TYPE_FROM_GUID(guid);
		if (guidtype == HIGHGUID_TYPE_UNIT)
		{
			Unit* pUnit = plr->GetMapMgr()->GetUnit(guid);
			CreatureProto * proto = TO_CREATURE(pUnit)->GetProto();
			switch (loot_type) 
			{
				default:
					lootmgr.FillCreatureLoot(&pUnit->loot, pUnit->GetEntry(), pUnit->GetMapMgr() ? ( pUnit->GetMapMgr()->iInstanceMode ? true : false ) : false);
					pUnit->loot.gold = proto ? proto->money : 0;
					loot_type2 = 1;
					break;
				case 2:
					lootmgr.FillSkinningLoot(&pUnit->loot, pUnit->GetEntry());
					loot_type2 = 2;
					break;
				case 3:
					lootmgr.FillPickpocketingLoot(&pUnit->loot, pUnit->GetEntry());
					loot_type2 = 2;
					break;
			}
		}
		else if (guidtype == HIGHGUID_TYPE_GAMEOBJECT)
		{
			GameObject* pGO = plr->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
			switch (loot_type) 
			{
				default:
					lootmgr.FillGOLoot(&pGO->loot, pGO->GetEntry(), pGO->GetMapMgr() ? ( pGO->GetMapMgr()->iInstanceMode ? true : false ) : false);
					loot_type2 = 1;
					break;
				case 5:
					lootmgr.FillSkinningLoot(&pGO->loot, pGO->GetEntry());
					loot_type2 = 2;
					break;
			}
		}
		else if(guidtype == HIGHGUID_TYPE_ITEM)
		{
			Item *pItem = plr->GetItemInterface()->GetItemByGUID(guid);
			switch (loot_type) 
			{
				case 6:
					lootmgr.FillItemLoot(pItem->loot,pItem->GetEntry());
					loot_type2 = 1;
					break;
				default:
					break;
			}
		}
		plr->SendLoot(guid, 2, plr->GetMapId());
		return 0;
	}

	int AddLoot(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 itemid = luaL_checkint(L,1);
		uint32 mincount = luaL_checkint(L,2);
		uint32 maxcount = luaL_checkint(L,3);
		uint32 ffa_loot = luaL_checkint(L,4);
		bool perm = ((luaL_optint(L,5,0) == 1) ? true : false);
		if (perm)
		{
			float chance = CHECK_FLOAT(L, 6);
			QueryResult* result = WorldDatabase.Query("SELECT * FROM loot_creatures WHERE entryid = %u, itemid = %u", ptr->GetEntry(), itemid);
			if (!result)
				WorldDatabase.Execute("REPLACE INTO loot_creatures VALUES (%u, %u, %f, 0, 0, 0, %u, %u, %u)", ptr->GetEntry(), itemid, chance, mincount, maxcount, ffa_loot);
		}
		lootmgr.AddLoot(&ptr->loot,itemid,mincount,maxcount,ffa_loot);
		return 0;
	}

	int VendorAddItem(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * ctr = TO_CREATURE(ptr);
		uint32 itemid = (uint32)luaL_checknumber(L, 1);
		uint32 amount = (uint32)luaL_checknumber(L, 2);
		uint32 costid = (uint32)luaL_checknumber(L, 3);
		ItemExtendedCostEntry * ec = (costid > 0) ? dbcItemExtendedCost.LookupEntryForced(costid) : NULL;
		if (itemid && amount)
			ctr->AddVendorItem(itemid, amount, ec);
		return 0;
	}

	int VendorRemoveItem(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * ctr = TO_CREATURE(ptr);
		uint32 itemid = (uint32)luaL_checknumber(L, 1); 
		int slot = ctr->GetSlotByItemId(itemid);
		if(itemid && slot > 0)
			ctr->RemoveVendorItem(itemid);
		return 0;
	}

	int VendorRemoveAllItems(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * ctr = TO_CREATURE(ptr);
		uint32 i = 0;
		if (ctr->HasItems())
		{
			uint32 creatureitemids[200];
			uint32 count = ctr->GetSellItemCount();
			for(std::vector<CreatureItem>::iterator itr = ctr->GetSellItemBegin(); itr != ctr->GetSellItemEnd(); ++itr)
			{
				creatureitemids[i] = itr->itemid;
				i+=1;
			}
			for (i=0; i<count; i++) {
				ctr->RemoveVendorItem(creatureitemids[i]);
			}
		}
		return 0;
	}

	int EquipWeapons(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint32 equip1 = luaL_checkint(L, 1);
		uint32 equip2 = luaL_checkint(L, 2);
		uint32 equip3 = luaL_checkint(L, 3);
		ptr->SetEquippedItem(MELEE,equip1);
		ptr->SetEquippedItem(OFFHAND,equip2);
		ptr->SetEquippedItem(RANGED,equip3);
		return 0;
	}

	int Dismount(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if(ptr->IsPlayer())
		{
			Player * plr = TO_PLAYER(ptr);
			plr->RemoveAura(plr->m_MountSpellId);
			plr->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0);
		}
		else
			ptr->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0);
		return 0;
	}

	int GiveXp(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * pl = TO_PLAYER(ptr);
		uint32 exp = luaL_checkint(L, 1);
		pl->GiveXP(exp, pl->GetGUID(), true);
		return 0;
	}

	int AdvanceAllSkills(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 skillvalue = luaL_checkint(L, 1);
		plr->_AdvanceAllSkills(skillvalue);
		return 0;
	}

	int GetTeam(lua_State * L, Unit * ptr) //returns 0 for alliance, 1 for horde.
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		lua_pushinteger(L, plr->GetTeam());
		return 1;
	}

	int StartTaxi(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		TaxiPath * tp = CHECK_TAXIPATH(L, 1);
		uint32 mount_id = luaL_checkint(L, 2);
		plr->TaxiStart(tp, mount_id, 0);
		return 0;
	}

	int IsOnTaxi(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushboolean(L, TO_PLAYER(ptr)->GetTaxiState() ? 1 : 0);
		return 1;
	}

	int GetTaxi(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		PUSH_TAXIPATH(L, TO_PLAYER(ptr)->GetTaxiPath());
		return 1;
	}
	
	int SetPlayerLock(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		bool lock = CHECK_BOOL(L,1);
		if (lock) 
		{
			ptr->m_pacified = 1;
			ptr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
			WorldPacket data1(9);
			data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			data1 << ptr->GetNewGUID() << uint8(0x00);
			TO_PLAYER(ptr)->GetSession()->SendPacket(&data1);
		}
		else
		{
			ptr->m_pacified = 0;
			ptr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
			WorldPacket data1(9);
			data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			data1 << ptr->GetNewGUID() << uint8(0x01);
			TO_PLAYER(ptr)->GetSession()->SendPacket(&data1);
		}
		return 0;
	}

	int MovePlayerTo(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		float o = CHECK_FLOAT(L,4);
		uint32 mov_flag = CHECK_ULONG(L,5); //0 - walk, 256 - teleport, 4096 - run, 12288 - fly
		float moveSpeed = (float)luaL_optnumber(L,6,1.0f);
		if (moveSpeed == 1.0f)
		{
			if (mov_flag == 0)
				moveSpeed = 2.5f*0.001f;
			else
				moveSpeed = 7.0f*0.001f;
		}
		ptr->SetFacing(o);
		ptr->SetOrientation(o);
		float distance = ptr->CalcDistance(ptr->GetPositionX(), ptr->GetPositionY(), ptr->GetPositionZ(), x, y, z);
		uint32 moveTime = uint32(distance / moveSpeed);
		WorldPacket data(SMSG_MONSTER_MOVE, 50);
		data << ptr->GetNewGUID();
		data << uint8(0);
		data << ptr->GetPositionX();
		data << ptr->GetPositionY();
		data << ptr->GetPositionZ();
		data << getMSTime();
		data << uint8(0x00);
		data << uint32(mov_flag);
		data << moveTime;
		data << uint32(1);
		data << x << y << z;

		ptr->SendMessageToSet(&data, true);
		ptr->SetPosition(x,y,z,o);
		return 0;
	}

	int ChannelSpell(lua_State * L, Unit * ptr)
	{
		uint32 Csp = luaL_checkint(L, 1);
		Object * target = CHECK_OBJECT(L, 2);
		if (Csp && target != NULL) 
		{
			ptr->CastSpell(target->GetGUID(),dbcSpell.LookupEntry(Csp),false);
			ptr->SetChannelSpellTargetGUID( target->GetGUID());
			ptr->SetChannelSpellId(Csp);
		}
		return 0;
	}

	int StopChannel(lua_State * L, Unit * ptr)
	{
		ptr->SetChannelSpellTargetGUID( 0);
		ptr->SetChannelSpellId(0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// WORLDSTATES/WORLD PVP NOT SUPPORTED
	//////////////////////////////////////////////////////////////////////////
	/*
	int SetWorldState(lua_State * L, Unit * ptr)
	{
		int zone = luaL_checkint(L, 1);
		int index = luaL_checkint(L, 2);
		int value = luaL_checkint(L, 3);
		
		if(!zone || !index || !value)
				lua_pushnil(L);

		ptr->GetMapMgr()->SetWorldState(zone, index, value);
		lua_pushboolean(L, 1);
		return 1;
	}
	*/

	int EnableFlight(lua_State * L, Unit * ptr)
	{	
		TEST_PLAYER()
		bool Switch = CHECK_BOOL(L,1);
		if (Switch)
		{
			WorldPacket fly(835, 13);
			ptr->EnableFlight();
			fly << ptr->GetNewGUID();
			fly << uint32(2);
			ptr->SendMessageToSet(&fly, true);
		}
		else
		{
			WorldPacket fly(836, 13);
			ptr->DisableFlight();
			fly << ptr->GetNewGUID();
			fly << uint32(5);
			ptr->SendMessageToSet(&fly, true);
		}
		return 0;
	}

	int GetCoinage(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		lua_pushinteger(L, plr->GetGold());
		return 1;
	}

	int FlagPvP(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		plr->SetPvPFlag();
		return 0;
	}

	int IsMounted(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		if (ptr->IsPlayer())
		{
			Player * plr = TO_PLAYER(ptr);
			if(plr != NULL && plr->IsMounted())
				lua_pushboolean(L,1);
			else
				lua_pushboolean(L,0);
		}
		else
			lua_pushboolean(L, (ptr->GetMount() > 0) ? 1 : 0);
		return 1;
	}

	//credits to alvanaar for the following 9 functions:
	int IsGroupedWith(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * target = CHECK_PLAYER(L,1);
		if(ptr->GetGroup()->HasMember(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int GetGroupType(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Group * group = plr->GetGroup();
		if(group != NULL)
			lua_pushinteger(L,group->GetGroupType() );
		else
			lua_pushnil(L);
		return 1;
	}

	int GetTotalHonor(lua_State * L, Unit * ptr) // I loathe typing "honour" like "honor".
	{
		TEST_PLAYER()
		lua_pushinteger(L, TO_PLAYER(ptr)->m_honorPoints);
		return 1;
	}
	
	int GetHonorToday(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushinteger(L, TO_PLAYER(ptr)->m_honorToday);
		return 1;
	}

	int GetHonorYesterday(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushinteger(L, TO_PLAYER(ptr)->m_honorYesterday);
		return 1;
	}

	int GetArenaPoints(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushinteger(L, TO_PLAYER(ptr)->m_arenaPoints);
		return 1;
	}
	
	int AddArenaPoints(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 pnts = luaL_checkint(L, 1);
		Player* plr = TO_PLAYER(ptr);
		if (pnts) 
		{
			plr->m_arenaPoints += pnts;
			plr->RecalculateHonor();
		}
		return 0;
	}

	int RemoveArenaPoints(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 pnts = luaL_checkint(L, 1);
		Player* plr = TO_PLAYER(ptr);
		int32 npts = plr->m_arenaPoints-pnts;
		if(npts >= 0) 
		{
			plr->m_arenaPoints = npts;
			plr->RecalculateHonor();
		}
		return 0;
	}
	
	int AddLifetimeKills(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 pnts = luaL_checkint(L, 1);
		Player * plr = TO_PLAYER(ptr);
		plr->m_killsLifetime += pnts;
		return 0;
	}

	int GetGender(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		lua_pushinteger(L, ptr->getGender());
		return 1;
	}

	int SetGender(lua_State * L, Unit * ptr)
	{
		if (!ptr) return 0;
		uint8 gender = luaL_checkint(L,1);
		ptr->setGender(gender);
		return 0;
	}
	//next 5 credits: alvanaar
	int SendPacketToGuild(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		WorldPacket * data = CHECK_PACKET(L,1);
		Player* plr = TO_PLAYER(ptr);
		Guild* guild = plr->GetGuild();
		if (data != NULL && guild != NULL)
			guild->SendPacket(data);
		return 0;
	}
	
	int GetGuildId(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		if (plr->GetGuild() != NULL)
			lua_pushinteger(L, plr->GetGuildId());
		else
			lua_pushnil(L);
		return 1;
	}
	
	int GetGuildRank(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (plr->GetGuild() != NULL)
			lua_pushinteger(L, plr->GetGuildRank());
		else
			lua_pushnil(L);
		return 1;
	}
	
	int SetGuildRank(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()

		Player* plr = TO_PLAYER(ptr);
		int rank = luaL_checkint(L, 1);
		if(plr->IsInGuild() )
			plr->SetGuildRank(rank);
		return 0;
	}
	
	int IsInGuild(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()

		Player* plr = TO_PLAYER(ptr);
		if(plr->IsInGuild())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int SendGuildInvite(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * sender = TO_PLAYER(ptr);
		Player * plyr = CHECK_PLAYER(L,1);
		std::string inviteeName = plyr->GetName();
		Guild *pGuild = sender->getPlayerInfo()->guild;
		if(!plyr)
		{
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_INVITE_S,inviteeName.c_str(),GUILD_PLAYER_NOT_FOUND);
		}
		else if(!pGuild)
		{
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		}
		else if( plyr->GetGuildId() )
		{
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_INVITE_S,plyr->GetName(),ALREADY_IN_GUILD);
		}
		else if( plyr->GetGuildInvitersGuid())
		{
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_INVITE_S,plyr->GetName(),ALREADY_INVITED_TO_GUILD);
		}
		else if(plyr->GetTeam()!=sender->GetTeam() && sender->GetSession()->GetPermissionCount() == 0 && !sWorld.interfaction_guild)
		{
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_INVITE_S,"",GUILD_NOT_ALLIED);
		}
		else {
			Guild::SendGuildCommandResult(sender->GetSession(), GUILD_INVITE_S,inviteeName.c_str(),GUILD_U_HAVE_INVITED);
			WorldPacket data(SMSG_GUILD_INVITE, 100);
			data << sender->GetName();
			data << pGuild->GetGuildName();
			plyr->GetSession()->SendPacket(&data);
			plyr->SetGuildInvitersGuid( sender->GetLowGUID() );
		}
		return 0;
	}

	int DemoteGuildMember(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		if (target)
			plr->GetGuild()->DemoteGuildMember(target->getPlayerInfo(), plr->GetSession());
		return 0;
	}

	int PromoteGuildMember(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		if (target)
			plr->GetGuild()->PromoteGuildMember(target->getPlayerInfo(), plr->GetSession());
		return 0;
	}

	int SetGuildMotd(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		const char * szNewMotd = luaL_checkstring(L,1);
		if (szNewMotd != NULL)
			plr->GetGuild()->SetMOTD(szNewMotd, plr->GetSession());
		return 0;
	}

	int GetGuildMotd(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if(plr->GetGuild() != NULL)
			lua_pushstring(L,plr->GetGuild()->GetMOTD());
		else
			lua_pushnil(L);
		return 1;
	}

	int SetGuildInformation(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		const char * gi = luaL_checkstring(L,1);
		if (gi)
			plr->GetGuild()->SetGuildInformation(gi, plr->GetSession());
		return 0;
	}

	int AddGuildMember(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		int32 rank = luaL_optinteger(L, 2, -1);
		if (target)
			plr->GetGuild()->AddGuildMember(target->getPlayerInfo(), plr->GetSession(), rank);
		return 0;
	}

	int RemoveGuildMember(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		if (target)
			plr->GetGuild()->RemoveGuildMember(target->getPlayerInfo(), plr->GetSession());
		return 0;
	}

	int SetPublicNote(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		const char * note = luaL_checkstring(L,2);
		if (target && note)
			plr->GetGuild()->SetPublicNote(target->getPlayerInfo(), note, plr->GetSession());
		return 0;
	}

	int SetOfficerNote(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		const char * note = luaL_checkstring(L,2);
		if (target && note)
			plr->GetGuild()->SetOfficerNote(target->getPlayerInfo(), note, plr->GetSession());
		return 0;
	}

	int DisbandGuild(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (plr->GetGuild() != NULL)
			plr->GetGuild()->Disband();
		return 0;
	}

	int ChangeGuildMaster(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		Player * target = CHECK_PLAYER(L,1);
		if (target)
			plr->GetGuild()->ChangeGuildMaster(target->getPlayerInfo(), plr->GetSession());
		return 0;
	}

	int SendGuildChatMessage(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		const char * message = luaL_checkstring(L,1);
		bool officer = CHECK_BOOL(L,2);
		if(plr->GetGuild() != NULL && message != NULL)
			(officer) ? plr->GetGuild()->OfficerChat(message, plr->GetSession(), 0) :  plr->GetGuild()->GuildChat(message, plr->GetSession(), 0);
		return 0;
	}

	int SendGuildLog(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (plr->GetGuild() != NULL)
			plr->GetGuild()->SendGuildLog(plr->GetSession());
		return 0;
	}

	int GuildBankDepositMoney(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 amount = luaL_checkint(L,1);
		if (plr->GetGuild() != NULL)
			plr->GetGuild()->DepositMoney(plr->GetSession(), amount);
		return 0;
	}

	int GuildBankWithdrawMoney(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		uint32 amount = luaL_checkint(L,1);
		if (plr->GetGuild() != NULL)
			plr->GetGuild()->WithdrawMoney(plr->GetSession(), amount);
		return 0;
	}

	int SetByteValue(lua_State * L, Unit * ptr)
	{
		uint32 index = luaL_checkint(L,1);
		uint32 index1 = luaL_checkint(L,2);
		uint8 value = luaL_checkint(L,3);
		ptr->SetByte(index,index1,value);
		return 0;
	}

	int GetByteValue(lua_State * L, Unit * ptr)
	{
		uint32 index = luaL_checkint(L,1);
		uint32 index1 = luaL_checkint(L,2);
		lua_pushinteger(L,ptr->GetByte(index,index1));
		return 1;
	}

	int IsPvPFlagged(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushboolean(L,TO_PLAYER(ptr)->IsPvPFlagged() ? 1 : 0);
		return 1;
	}

	int IsFFAPvPFlagged(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		lua_pushboolean(L,TO_PLAYER(ptr)->IsFFAPvPFlagged() ? 1 : 0);
		return 1;
	}

	int GetGuildLeader(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Guild * pGuild = TO_PLAYER(ptr)->GetGuild();
		if(pGuild != NULL) 
		{
			Player * plr = objmgr.GetPlayer(pGuild->GetGuildLeader());
			if (plr != NULL)
				lua_pushstring(L,plr->GetName());
			else
				lua_pushnil(L);
		}
		else
			lua_pushnil(L);
		return 1;
	}

	int GetGuildMemberCount(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Guild * pGuild = TO_PLAYER(ptr)->GetGuild();
		(pGuild != NULL) ? lua_pushinteger(L,pGuild->GetNumMembers()) : lua_pushnil(L);
		return 1;
	}

	int IsFriendly(lua_State * L, Unit * ptr)
	{
		Unit * obj = CHECK_UNIT(L,1);
		if (!obj || !ptr) return 0;
		if (isFriendly(ptr, obj))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}
	
	int IsInChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		if(!ptr || !channel_name)
			return 0;

		Channel* pChannel = channelmgr.GetChannel(channel_name, TO_PLAYER(ptr));
		if(pChannel->HasMember(TO_PLAYER(ptr))) // Channels: "General", "Trade", "LocalDefense", "GuildRecruitment", "LookingForGroup", (or any custom channel)
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int JoinChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Channel* pChannel = channelmgr.GetChannel(channel_name, TO_PLAYER(ptr));
		const char* pw = luaL_optstring(L, 2, pChannel->m_password.c_str());

		if(!ptr || !channel_name || pChannel->HasMember(TO_PLAYER(ptr)) || !pChannel)
			return 0;
		else
			pChannel->AttemptJoin(TO_PLAYER(ptr), pw);
		return 1;
	}

	int LeaveChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Channel* pChannel = channelmgr.GetChannel(channel_name, TO_PLAYER(ptr));
		if(!ptr || !channel_name || !pChannel || !pChannel->HasMember(TO_PLAYER(ptr)))
			return 0;
		else
			pChannel->Part(TO_PLAYER(ptr), true);
		return 1;
	}

	int SetChannelName(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* current_name = luaL_checkstring(L, 1);
		const char* new_name = luaL_checkstring(L, 2);
		Channel* pChannel = channelmgr.GetChannel(current_name, TO_PLAYER(ptr));
		if(!current_name || !new_name || !ptr || !pChannel || pChannel->m_name == new_name)
			return 0;
		pChannel->m_name = new_name;
		return 1;
	}

	int SetChannelPassword(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		const char* pass = luaL_checkstring(L, 2);
		Channel* pChannel = channelmgr.GetChannel(channel_name, TO_PLAYER(ptr));
		if(!pass || !ptr || pChannel->m_password == pass)
			return 0;
		pChannel->Password(TO_PLAYER(ptr), pass);
		return 1;
	}

	int GetChannelPassword(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Channel* pChannel = channelmgr.GetChannel(channel_name, TO_PLAYER(ptr));
		if(!ptr)
			return 0;
		lua_pushstring(L, pChannel->m_password.c_str());
		return 1;
	}

	int KickFromChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Player* plr = TO_PLAYER(ptr);
		Channel* pChannel = channelmgr.GetChannel(channel_name, plr);
		if(!plr || !pChannel)
			return 0;
		pChannel->Kick(plr, plr, false);
		return 1;
	}

	int BanFromChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Player* plr = TO_PLAYER(ptr);
		Channel* pChannel = channelmgr.GetChannel(channel_name, plr);
		if(!plr|| !pChannel)
			return 0;
		pChannel->Kick(plr, plr, true);
		return 1;
	}

	int UnbanFromChannel(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		Player* plr = TO_PLAYER(ptr);
		Channel* pChannel = channelmgr.GetChannel(channel_name, plr);
		if(!plr || !pChannel)
			return 0;
		pChannel->Unban(plr, plr->getPlayerInfo());
		return 1;
	}

	int GetChannelMemberCount(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		const char* channel_name = luaL_checkstring(L, 1);
		if(!ptr || !channel_name)
			return 0;
		lua_pushnumber(L, channelmgr.GetChannel(channel_name, TO_PLAYER(ptr))->GetNumMembers());
		return 1;
	}

	int GetPlayerMovementVector(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		MovementInfo * movement_info = plr->GetSession()->GetMovementInfo();
		if(movement_info != NULL) 
		{
			lua_newtable(L);
			lua_pushstring(L, "x");
			lua_pushnumber(L, movement_info->x);
			lua_rawset(L, -3);
			lua_pushstring(L, "y");
			lua_pushnumber(L, movement_info->y);
			lua_rawset(L, -3);
			lua_pushstring(L, "z");
			lua_pushnumber(L, movement_info->z);
			lua_rawset(L, -3);
			lua_pushstring(L, "o");
			lua_pushnumber(L, movement_info->orientation);
			lua_rawset(L, -3);
		}
		else
			lua_pushnil(L);
		return 1;
	}

	int GetPlayerMovementFlags(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		MovementInfo * move_info = TO_PLAYER(ptr)->GetSession()->GetMovementInfo();
		if (move_info != NULL)
			lua_pushnumber(L, move_info->flags);
		else
			RET_NIL()
		return 1;
	}

	int Repop(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		if (plr->IsDead())
			plr->RepopRequestedPlayer();
		return 0;
	}

	int SetMovementFlags(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		int movetype = luaL_checkint(L,1); //0: walk, 1: run, 2: fly.
		if (movetype == 2)
			ptr->GetAIInterface()->setAIMoveFlags(AIMOVESTATE_FLY);
		else if (movetype == 1)
			ptr->GetAIInterface()->setAIMoveFlags(AIMOVESTATE_RUN);
		else
			ptr->GetAIInterface()->setAIMoveFlags(AIMOVESTATE_WALK);
		return 0;
	}

	int GetSpawnId(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		Creature * cre = TO_CREATURE(ptr);
		lua_pushnumber(L,cre->GetSQL_id());
		return 1;
	}

	int ResetTalents(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player * plr = TO_PLAYER(ptr);
		plr->Reset_Talents();
		return 0;
	}

	int SetTalentPoints(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 spec = luaL_checkint(L,1); //0 or 1
		uint32 points = luaL_checkint(L,2);
		TO_PLAYER(ptr)->m_specs[spec].m_customTalentPointOverride = points;
		return 0;
	}

	int GetTalentPoints(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 spec = luaL_checkint(L,1); //0 or 1
		PlayerSpec plrSpec = TO_PLAYER(ptr)->m_specs[spec];
		uint32 Lvl = TO_PLAYER(ptr)->getLevel();
		uint32 FreePoints = 0;
		if (Lvl > 9)
		{
			FreePoints = plrSpec.m_customTalentPointOverride > 0 ? plrSpec.m_customTalentPointOverride : Lvl - 9; // compensate for additional given talentpoints
			for (std::map<uint32, uint8>::iterator itr = plrSpec.talents.begin(); itr != plrSpec.talents.end(); ++itr)
				FreePoints -= (itr->second+1);
		}
		lua_pushnumber(L,FreePoints);
		return 1;
	}

	int EventChat(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		uint8 typ = luaL_checkint(L,1);
		uint32 lang = luaL_checkint(L,2);
		const char* message = luaL_checkstring(L,3);
		uint32 delay = luaL_checkint(L,4);
		if (message != NULL && delay)
			ptr->SendChatMessage(typ, lang, message, delay);
		return 0;
	}

	int GetEquippedItemBySlot(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int16 slot = luaL_checkint(L,1);
		Player * plr = TO_PLAYER(ptr);
		Item * pItem = plr->GetItemInterface()->GetInventoryItem(slot);
		if (pItem)
			PUSH_ITEM(L,pItem);
		else
			lua_pushnil(L);
		return 1;
	}

	int GetGuildMembers(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Player* plr = TO_PLAYER(ptr);
		Guild* pGuild = plr->GetGuild();
		uint32 count = 0;
		lua_newtable(L);
		if(pGuild != NULL)
		{
			GuildMemberMap::iterator itr;
			pGuild->getLock().Acquire();
			for(itr = pGuild->GetGuildMembersBegin(); itr != pGuild->GetGuildMembersEnd(); ++itr)
			{
				count++;
				lua_pushinteger(L, count);
				//Paroxysm : Why do we push player names are opposed to objects?
				//hyper: because guild members might not be logged in
				//ret = (*itr).first->m_loggedInPlayer;
				//PUSH_UNIT(L, ((Unit*)ret), false);
				lua_pushstring(L, (*itr).first->name);
				lua_rawset(L, -3);
			}
			pGuild->getLock().Release();
		}
		else
			lua_pushnil(L);
		return 1;
	}

#ifdef ENABLE_ACHIEVEMENTS

	int AddAchievement(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int32 achievementID = luaL_checkint(L,1);
		Player * plr = TO_PLAYER(ptr);
		if (plr->GetAchievementMgr().GMCompleteAchievement(NULL, achievementID))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		return 1;
	}

	int RemoveAchievement(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		int32 achievementID = luaL_checkint(L,1);
		TO_PLAYER(ptr)->GetAchievementMgr().GMResetAchievement(achievementID);
		return 0;
	}

	int HasAchievement(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		uint32 achievementID = luaL_checkint(L,1);
		lua_pushboolean(L, TO_PLAYER(ptr)->GetAchievementMgr().HasCompleted(achievementID) ? 1 : 0);
		return 1;
	}

#endif

	int GetAreaId(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		RET_NUMBER(ptr->GetMapMgr()->GetAreaID(ptr->GetPositionX(), ptr->GetPositionY() ) );
	}

	int ResetPetTalents(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER()
		Pet * pet = TO_PLAYER(ptr)->GetSummon();
		if (pet != NULL) 
		{
			pet->WipeTalents();
			pet->SetTPs(pet->GetTPsForLevel(pet->getLevel()));
			pet->SendTalentsToOwner();
		}
		return 0;
	}

	int IsDazed(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		lua_pushboolean(L,(ptr->IsDazed())?1:0);
		return 1;
	}

	int GetAura(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		uint32 slot = CHECK_ULONG(L,1);
		if (slot > MAX_TOTAL_AURAS_START && slot < MAX_TOTAL_AURAS_END)
			RET_NUMBER(ptr->m_auras[slot]->GetSpellId());
		RET_NIL()
	}

	int GetAuraObject(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		uint32 slot = CHECK_ULONG(L,1);
		if (slot > MAX_TOTAL_AURAS_START && slot < MAX_TOTAL_AURAS_END)
		{
			PUSH_AURA(L, ptr->m_auras[slot]);
			return 1;
		}
		RET_NIL()
	}

	int IsRooted(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		if(ptr->isRooted() )
			RET_BOOL(true)
		RET_BOOL(false)
	}

	int HasAuraWithMechanic(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		uint32 mechanic = CHECK_ULONG(L,1);
		if(mechanic && ptr->HasAuraWithMechanics(mechanic) )
			RET_BOOL(true)
		RET_BOOL(false)
	}

	int HasNegativeAura(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		for (uint32 x = MAX_NEGATIVE_VISUAL_AURAS_START; x < MAX_NEGATIVE_VISUAL_AURAS_END; ++x)
		{
			if (ptr->m_auras[x] && ptr->m_auras[x]->m_spellProto)
				RET_BOOL(true)
		}
		RET_BOOL(false)
	}

	int HasPositiveAura(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		for (uint32 x = MAX_POSITIVE_VISUAL_AURAS_START; x < MAX_POSITIVE_VISUAL_AURAS_END; ++x)
		{
			if (ptr->m_auras[x] && ptr->m_auras[x]->m_spellProto)
				RET_BOOL(true)
		}
		RET_BOOL(false)
	}

	int GetClosestEnemy(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		float closest_dist = 99999.99f;
		float current_dist = 0;
		Object * closest_unit = NULL;
		Unit * ret = NULL;
		for (set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			closest_unit = (*itr);
			if(!closest_unit->IsUnit() || !isHostile(ptr,closest_unit) )
				continue;
			current_dist = ptr->GetDistance2dSq(closest_unit);
			if(current_dist < closest_dist)
			{
				closest_dist = current_dist;
				ret = TO_UNIT(closest_unit);
			}
		}
		PUSH_UNIT(L,ret);
		return 1;
	}

	int GetClosestFriend(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		float closest_dist = 99999.99f;
		float current_dist = 0.0f;
		Object * closest_unit = NULL;
		Unit * ret = NULL;
		for (set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			closest_unit = (*itr);
			if (!closest_unit->IsUnit() || isHostile(closest_unit, ptr))
				continue;
			current_dist = closest_unit->GetDistanceSq(ptr);
			if(current_dist < closest_dist)
			{
				closest_dist = current_dist;
				ret = TO_UNIT(closest_unit);
			}
		}
		PUSH_UNIT(L,ret);
		return 1;
	}

	int GetClosestUnit(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		float closest_dist = 99999.99f;
		float current_dist = 0;
		Object * closest_unit = NULL;
		Unit * ret = NULL;
		for (set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			closest_unit = (*itr);
			if(!closest_unit->IsUnit())
				continue;
			current_dist = ptr->GetDistance2dSq(closest_unit);
			if(current_dist < closest_dist)
			{
				closest_dist = current_dist;
				ret = TO_UNIT(closest_unit);
			}
		}
		PUSH_UNIT(L,ret);
		return 1;
	}

	int GetObjectType(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		if (ptr->IsPlayer())
			lua_pushstring(L, "Player");
		else
			lua_pushstring(L, "Unit");
		return 1;
	}
	int GetCurrentWaypoint(lua_State * L, Unit * ptr)
	{
		TEST_UNIT()
		RET_NUMBER( TO_AIMOB(ptr->GetAIInterface())->getCurrentWaypoint());
	}
	int DisableMelee(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET()
		bool disable = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->disableMelee(disable);
		RET_BOOL(true)
	}
	int DisableSpells(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET()
		bool disable = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->disableSpells(disable);
		RET_BOOL(true)
	}
	int DisableRanged(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET()
		bool disable = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->disableRanged(disable);
		RET_BOOL(true)
	}
	int DisableCombat(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET()
		bool disable = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->disableCombat( disable);
		RET_BOOL(true)
	}
	int DisableTargeting(lua_State * L, Unit * ptr)
	{
		TEST_UNIT_RET()
		bool disable = CHECK_BOOL(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->disableTargetting(disable);
		RET_BOOL(true)
	}
	int IsInGroup(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		if(TO_PLAYER(ptr)->InGroup() )
			RET_BOOL(true)
		RET_BOOL(false)
	}
	int GetLocation(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		lua_pushnumber(L,ptr->GetPositionX());
		lua_pushnumber(L,ptr->GetPositionY());
		lua_pushnumber(L,ptr->GetPositionZ());
		lua_pushnumber(L,ptr->GetOrientation());
		return 4;
	}
	int GetByte(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER()
		uint32 index = luaL_checkint(L,1);
		uint32 index2 = luaL_checkint(L,2);
		uint8 value = ptr->GetByte(index,index2);
		RET_INT(value);
	}
	int SetByte(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		int index = luaL_checkint(L,1);
		int index2 = luaL_checkint(L,2);
		uint8 value = luaL_checkint(L,3);
		ptr->SetByte(index,index2,value);
		RET_BOOL(true)
	}

	int GetSpawnLocation(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		lua_pushnumber(L,ptr->GetSpawnX());
		lua_pushnumber(L,ptr->GetSpawnY());
		lua_pushnumber(L,ptr->GetSpawnZ());
		lua_pushnumber(L,ptr->GetSpawnO());
		return 4;
	}
	int GetObject(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		uint64 guid = CHECK_GUID(L,1);
		Object * obj = ptr->GetMapMgr()->_GetObject(guid);
		if(obj != NULL && obj->IsUnit() )
			PUSH_UNIT(L,obj);
		else if(obj != NULL && obj->IsGameObject() )
			PUSH_GO(L,obj);
		else
			lua_pushnil(L);
		return 1;
	}
	int GetSecondHated(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		PUSH_UNIT(L, TO_AIMOB(ptr->GetAIInterface())->getSecondHated() );
		return 1;
	}
	int SaveToInstance(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER();
		Instance * dungeon = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(),ptr->GetInstanceID());
		dungeon->SaveToDB();
		sInstanceMgr.BuildRaidSavedInstancesForPlayer(TO_PLAYER(ptr));
		return 0;
	}
	int UseAI(lua_State * L, Unit * ptr)
	{
		bool check = CHECK_BOOL(L,1);
		ptr->setAItoUse(check);
		return 0;
	}
	int FlagFFA(lua_State * L, Unit * ptr)
	{

		TEST_UNITPLAYER();
		bool set = CHECK_BOOL(L,1);
		if(set)
			ptr->SetFFAPvPFlag();
		else
			ptr->RemoveFFAPvPFlag();
		return 0;
	}
	int TeleportCreature(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		float x = CHECK_FLOAT(L,1);
		float y = CHECK_FLOAT(L,2);
		float z = CHECK_FLOAT(L,3);
		ptr->SetPosition(x,y,z,ptr->GetOrientation());
		WorldPacket data(SMSG_MONSTER_MOVE,50);
		data << ptr->GetNewGUID();
		data << uint8(0);
		data << ptr->GetPositionX() << ptr->GetPositionY() << ptr->GetPositionZ();
		data << getMSTime();
		data << uint8(0x0);
		data << uint32(0x100);
		data << uint32(1) << uint32(1);
		data << x << y << z;
		ptr->SendMessageToSet(&data,false);
		return 0;
	}
	int IsInDungeon(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		if(ptr->GetMapMgr()->GetMapInfo() && ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_MULTIMODE)
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}
	int IsInRaid(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		if(ptr->GetMapMgr()->GetMapInfo() && ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}
	int IsHostile(lua_State*  L, Unit * ptr)
	{
		Object * B = CHECK_OBJECT(L,1);
		lua_pushboolean(L, isHostile(ptr,B));
		return 1;
	}
	int IsAttackable(lua_State*  L, Unit * ptr)
	{
		Object * B = CHECK_OBJECT(L,1);
		lua_pushboolean(L, isAttackable(ptr,B));
		return 1;
	}
	int GetNumWaypoints(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		RET_NUMBER( TO_AIMOB(ptr->GetAIInterface() )->getWaypointsCount() );
		return 1;
	}
	int GetMovementType(lua_State * L, Unit * ptr)
	{
		TEST_UNIT();
		RET_NUMBER( TO_AIMOB(ptr->GetAIInterface() )->getWPMoveType() );
		return 1;
	}
	int GetQuestLogSlot(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER();
		uint32 entry = CHECK_ULONG(L,1);
		QuestLogEntry * qle = TO_PLAYER(ptr)->GetQuestLogForEntry(entry);
		if (!qle)
			RET_NUMBER(-1);
		lua_pushnumber(L, qle->GetSlot());
		return 1;
	}

	int GetAuraStackCount(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER_RET();
		uint32 id = CHECK_ULONG(L,1);
		RET_NUMBER(ptr->GetAuraStackCount(id));
	}

	int AddAuraObject(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();
		Aura * aura = CHECK_AURA(L,1);
		if (!aura) return 0;
		ptr->AddAura(aura);
		return 0;
	}

	int GetAuraObjectById(lua_State * L, Unit * ptr)
	{
		TEST_UNITPLAYER();
		uint32 id = CHECK_ULONG(L,1);
		PUSH_AURA(L, ptr->FindAura(id));
		return 1;
	}

	int StopPlayerAttack(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER();
		TO_PLAYER(ptr)->smsg_AttackStop( TO_PLAYER(ptr)->GetSelection() );
		return 0;
	}

	int GetQuestObjectiveCompletion(lua_State * L, Unit * ptr)
	{
		TEST_PLAYER_RET()
		int questid = luaL_checkint(L,1);
		int objective = luaL_checkint(L,2);
		Player * pl = TO_PLAYER(ptr);
		QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
		if (qle != NULL)
			lua_pushnumber(L, qle->GetMobCount(objective));
		else
			lua_pushnil(L);
		return 1;
	}
}
#endif